#include <iostream>
#include <algorithm>

#include <QtNetwork>
#include <QtWebKit>
#include <QtGui/qapplication.h>
#include <QtGui/qlabel.h>
#include <QtCore/qdatetime.h>

#include "UIConstants.hpp"
#include "BrowserWindow.hpp"
#include "video/LiveSource.hpp"
#include "video/VideoSource.hpp"
#include "MessageWindow.hpp"
#include "ImageWindow.hpp"
#include "ImageLinkLabel.hpp"
#include "threads/ThreadManager.hpp"
#include "actions/ActionManager.hpp"
#include "threads/Sleeper.hpp"


using namespace std;

BrowserWindow::BrowserWindow(const int cameraChannel) : cameraChannel(cameraChannel) {

    init();
    showBookmarkPage();

    // Change to bookmark mode
    actionManager->setMode(1);
}

BrowserWindow::~BrowserWindow() {
    delete imageWindow;
    delete settingsWin;
}

void BrowserWindow::init() {


    screenSize = QApplication::desktop()->screenGeometry().size();
    settings = new QSettings("gazebrowser.ini", QSettings::IniFormat);

    QFile file;
    file.setFileName(":js/jquery-1.8.3.min.js");
    file.open(QIODevice::ReadOnly);
    jQuery = file.readAll();
    file.close();

    isCalibrating = false;

    QNetworkProxyFactory::setUseSystemConfiguration(true);

    view = new QWebView(this);
    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

    // add our own webpage to control the user-agent
    webpage = new GazeWebPage;
    view->setPage(webpage);

    eye_widget = new CVWidget;
    eye_widget->setWindowModality(Qt::WindowModal);
    eye_widget->setAttribute(Qt::WA_DeleteOnClose, false); //only hide the widget

    bookmarksWin = new BookmarksWindow(settings);
    bookmarksWin->setWindowModality(Qt::WindowModal);
    bookmarksWin->setAttribute(Qt::WA_DeleteOnClose, false);

    settingsWin = new SettingsWindow();
    settingsWin->setWindowModality(Qt::WindowModal);
    settingsWin->setAttribute(Qt::WA_DeleteOnClose, false);

    // setUpLinkWindow();

    setUpNavigation();

    // Create browser window
    QVBoxLayout* hLayout = new QVBoxLayout();
    hLayout->setMargin(0);
    hLayout->addWidget(view);
    hLayout->addWidget(navigationWidget);

    // Set layout in QWidget
    QWidget *window = new QWidget();
    window->setLayout(hLayout);

    imageWindow = new ImageWindow(window, view, Qt::WindowStaysOnTopHint);
    //imageWindow->setWindowModality(Qt::WindowModal);
    //imageWindow->setAttribute(Qt::WA_DeleteOnClose, false);
    imageWindow->resize(screenSize.width(), screenSize.height());
    imageWindow->setVisible(false);
    // Let the browser know when the image window closes, to change the tracked actions
    connect(imageWindow, SIGNAL(hideWindow()), this, SLOT(hideImageWindowEvent()));

    // add the pointer of the users gaze to the layout
    gazePointer = new GazePointer(window, Qt::WindowStaysOnTopHint);
    gazePointer->hide();

    setupMenus();
    setCentralWidget(window);

    setUpGazeActions();

}

void BrowserWindow::setUpGazeActions() {

    // consider height of menu bar
    int yOrigin = menuBar()->height();

    // Some convienence variables
    int h = screenSize.height();
    int w = screenSize.width();
    int wThird = screenSize.width() / 3;
    int wFourth = screenSize.width() / 4;
    int hThird = (screenSize.height() - yOrigin) / 3;

    vector<GazeAction*> browserWindowActions;
    browserWindowActions.push_back(createGazeAction("Back", Rect(0, h - 150, wFourth, 150), &BrowserWindow::backCallback));
    browserWindowActions.push_back(createGazeAction("Link", Rect(wFourth, h - 150, wFourth, 150), &BrowserWindow::showLinksCallback));
    browserWindowActions.push_back(createGazeAction("Bookmarks", Rect(wFourth * 2, h - 150, wFourth, 150), &BrowserWindow::showBookmarkPageCallback));
    browserWindowActions.push_back(createGazeAction("Forward", Rect(wFourth * 3, h - 150, wFourth, 150), &BrowserWindow::forwardCallback));
    browserWindowActions.push_back(createGazeAction("Scroll up", Rect(0, -500, w, 500), &BrowserWindow::scrollUpCallback));
    browserWindowActions.push_back(createGazeAction("Scroll down", Rect(0, h, w, 500), &BrowserWindow::scrollDownCallback));
    browserWindowActions.push_back(createGazeAction("Scroll left", Rect(-500, 0, 500, h), &BrowserWindow::scrollLeftCallback));
    browserWindowActions.push_back(createGazeAction("Scroll right", Rect(w, 0, 500, h), &BrowserWindow::scrollRightCallback));

    vector<GazeAction*> bookmarkWindowActions;
    // First link
    for (int i = 0; i < 3; i++) { // rows
        for (int j = 0; j < 3; j++) { // cols
            bookmarkWindowActions.push_back(createGazeAction("Open Link", Rect(wThird * j, hThird * i + yOrigin, wThird, hThird), &BrowserWindow::openLinkCallback));
        }
    }

    vector<GazeAction*> linkWindowActions;
    for (int i = 0; i < 3; i++) { // 3 rows
        for (int j = 0; j < 4; j++) { // 4 cols

            int x = wFourth * j;
            int y = hThird * i + yOrigin;
            
            linkWindowActions.push_back(createGazeAction("Open Link", Rect(x, y, wFourth, hThird), &BrowserWindow::selectLinkPageCallback));
        }
    }
    // Left of screen
    linkWindowActions.push_back(createGazeAction("Image window back", Rect(-500, 0, 500, h), &BrowserWindow::previousLinkPageCallback));
    // Right of screen
    linkWindowActions.push_back(createGazeAction("Image window forward", Rect(w, 0, 500, h), &BrowserWindow::nextPageCallback));
    // Close image window (action is placed above the screen)
    linkWindowActions.push_back(createGazeAction("Image window close", Rect(0, -500, w, 500), &BrowserWindow::closeLinkWindow));

    typedef map<int, vector< GazeAction* > > GazeMap;
    GazeMap m;
    m.insert(GazeMap::value_type(0, browserWindowActions));
    m.insert(GazeMap::value_type(1, bookmarkWindowActions));
    m.insert(GazeMap::value_type(2, linkWindowActions));

    // Initialize Gaze Actions
    actionManager = new ActionManager(m);
}

void BrowserWindow::setUpLinkWindow() {

    /*    imageWindow = new ImageWindow(view);
        imageWindow->setWindowModality(Qt::WindowModal);
        imageWindow->setAttribute(Qt::WA_DeleteOnClose, false);
        imageWindow->resize(screenSize.width(), screenSize.height());
        imageWindow->setVisible(false);
        // Let the browser know when the image window closes, to change the tracked actions
        connect(imageWindow, SIGNAL(hideWindow()), this, SLOT(hideImageWindowEvent()));*/

}

void BrowserWindow::setupMenus() {
    // Add the Slot to the quit button
    // on mac this will be showed in the unified menu bar

    menuBar()->resize(screenSize.width(), MENUBAR_HEIGHT);
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *quitAction = new QAction(tr("Quit Browser"), this);
    quitAction->setMenuRole(QAction::QuitRole);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quit_gazebrowser()));
    fileMenu->addAction(tr("Bookmarks"), this, SLOT(bookmarks()));
    fileMenu->addAction(tr("Preferences"), this, SLOT(preferences()));
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);


    QMenu *navMenu = menuBar()->addMenu(tr("&Navigation"));
    navMenu->addAction(tr("Back"), this, SLOT(back()));
    navMenu->addAction(tr("Forward"), this, SLOT(forward()));
    navMenu->addSeparator();
    navMenu->addAction(tr("Scroll Up"), this, SLOT(scrollUp()));
    navMenu->addAction(tr("Scroll Down"), this, SLOT(scrollDown()));
    navMenu->addSeparator();
    navMenu->addAction(tr("Scroll Left"), this, SLOT(scrollLeft()));
    navMenu->addAction(tr("Scroll Right"), this, SLOT(scrollRight()));
    navMenu->addSeparator();
    navMenu->addAction(tr("Go to page"), this, SLOT(goToPage()));
    navMenu->addAction(tr("Show Bookmarks"), this, SLOT(showBookmarkPage()));

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    // Zoom
    QMenu *zoomMenu = viewMenu->addMenu(tr("&Zoom"));
    zoomMenu->addAction(tr("Zoom in"), this, SLOT(zoomIn()));
    zoomMenu->addAction(tr("Zoom out"), this, SLOT(zoomOut()));
    // Navigationbar
    zoomMenu->addSeparator();
    viewMenu->addAction(tr("Show navigation bar"), this, SLOT(showNavigationWidget()));
    viewMenu->addAction(tr("Hide navigation bar"), this, SLOT(hideNavigationWidget()));

    QMenu *gazeMenu = menuBar()->addMenu(tr("&Gaze Actions"));

    QAction *calibrateMenuAction = new QAction(tr("Calibration"), this);
    connect(this, SIGNAL(isTracking(bool)), calibrateMenuAction, SLOT(setDisabled(bool)));
    connect(calibrateMenuAction, SIGNAL(triggered()), this, SLOT(start_calibration()));
    gazeMenu->addAction(calibrateMenuAction);
    gazeMenu->addSeparator();

    QAction *stopMenuAction = new QAction(tr("Stop tracking"), this);
    stopMenuAction->setDisabled(true);
    connect(this, SIGNAL(isTracking(bool)), stopMenuAction, SLOT(setEnabled(bool)));
    connect(stopMenuAction, SIGNAL(triggered()), this, SLOT(stop_tracking()));
    //gazeMenu->addAction("Calibration", this, SLOT(start_calibration()));
    gazeMenu->addAction(stopMenuAction);

    QAction *continueMenuAction = new QAction(tr("Resume tracking"), this);
    continueMenuAction->setDisabled(true);
    connect(this, SIGNAL(canResumeTracking(bool)), continueMenuAction, SLOT(setEnabled(bool)));
    connect(continueMenuAction, SIGNAL(triggered()), this, SLOT(resume_tracking()));
    //gazeMenu->addAction("Calibration", this, SLOT(start_calibration()));
    gazeMenu->addAction(continueMenuAction);
    gazeMenu->addSeparator();
    gazeMenu->addAction(tr("Show the Eye Widget"), this, SLOT(show_eye_widget()));

}

void BrowserWindow::setUpNavigation() {

    QPushButton *back = new QPushButton("", this);
    QPixmap pixmapBack(":img/back.png");
    QIcon iconBack(pixmapBack);
    back->setIcon(iconBack);
    back->setIconSize(pixmapBack.rect().size());
    connect(back, SIGNAL(clicked()), this, SLOT(back()));

    QPushButton *forward = new QPushButton("", this);
    QPixmap pixmapForward(":img/forward.png");
    QIcon iconForward(pixmapForward);
    forward->setIcon(iconForward);
    forward->setIconSize(pixmapForward.rect().size());
    connect(forward, SIGNAL(clicked()), this, SLOT(forward()));

    QPushButton *links = new QPushButton("", this);
    QPixmap pixmapLinks(":img/links.png");
    QIcon iconLinks(pixmapLinks);
    links->setIcon(iconLinks);
    links->setIconSize(pixmapLinks.rect().size());
    connect(links, SIGNAL(clicked()), this, SLOT(showLinks()));

    QPushButton *bookmarks = new QPushButton("", this);
    QPixmap pixmapBookmarks(":img/bookmarks.png");
    QIcon iconBookmarks(pixmapBookmarks);
    bookmarks->setIcon(iconBookmarks);
    bookmarks->setIconSize(pixmapLinks.rect().size());
    connect(bookmarks, SIGNAL(clicked()), this, SLOT(showBookmarkPage()));

    QHBoxLayout * navigationBox = new QHBoxLayout();

    navigationBox->addWidget(back);
    navigationBox->addWidget(links);
    navigationBox->addWidget(bookmarks);
    navigationBox->addWidget(forward);

    navigationWidget = new QWidget();
    navigationWidget->setLayout(navigationBox);
    navigationWidget->setMaximumSize(screenSize.width(), 135);
    navigationWidget->setBackgroundRole(QPalette::Dark);
    navigationWidget->setAutoFillBackground(true);
}

/**
 * this method is called, as soon as the window is displayed. here
 * we trigger the setUpCamera(). if we do this in the constructor
 * we would block the whole UI until openCV has opened the camera.
 */
void BrowserWindow::showEvent(QShowEvent *event) {

    Q_UNUSED(event);
    QTimer::singleShot(1000, this, SLOT(setUpCamera()));

}

/**
 * opens a connection to the webcam .openCV has problems, if VideoCapture 
 * is created outside the main thread...
 */
void BrowserWindow::setUpCamera() {

    source = new LiveSource(cameraChannel);
    tManager = new ThreadManager(this);
    tManager->goIdle();

}

void BrowserWindow::finishLoading(bool) {
    view->page()->mainFrame()->evaluateJavaScript(jQuery);

    if (view->page()->mainFrame()->url().toString() == "qrc:/bookmarks.html") {
        QString html = "$('#%1').append("
                "\"<a href='%2'><img src='http://gaze.frickler.ch/screenshot/?screenshot_url=%3' alt=''></a>"
                ""
                "<br>"
                "<a href='%4'>%5</a>\""
                ")";

        settings->beginGroup("BOOKMARKS");
        for (int i = 0; i < 9; i++) {
            QString desc = settings->value("DESC_" + QString::number(i)).toString();
            QString url = settings->value("URL_" + QString::number(i)).toString();

            if (url.length() > 0 && desc.length() > 0) {
                QString code = QString(html).arg(QString::number(i + 1), url, url, url, desc);

                view->page()->mainFrame()->evaluateJavaScript(code);
            }
        }
        settings->endGroup();

    }

    // Set Gaze Action Mode
    if (view->page()->mainFrame()->url().toString().startsWith("qrc:/")) {
        hideNavigationWidget();
        // Track only gaze actions for bookmarks page
        actionManager->setMode(1);
    } else {
        showNavigationWidget();
        // Track only gaze actions for bookmarks page
        actionManager->setMode(0);
    }

    actionManager->resume();

    if (isCalibrating)
        this->calibrate();

}

void BrowserWindow::goToPage() {
    // TODO: optimize and add a better layout. Maybe create window to make QLineEdit largerF
    bool ok;
    QString text = QInputDialog::getText(this,
            tr("Go to page"),
            tr("URL:"),
            QLineEdit::Normal,
            "", &ok);
    if (ok && !text.isEmpty()) {

        view->load(QUrl::fromUserInput(text));
    }
}

void BrowserWindow::openLinkCallback(cv::Point p) {
    // stop tracking
    actionManager->pause();
    // get index from link    
    int index = (p.y / (screenSize.height() / 3) * 3) + p.x / (screenSize.width() / 3);

    settings->beginGroup("BOOKMARKS");
    QString url = settings->value("URL_" + QString::number(index)).toString();
    settings->endGroup();

    if (!url.isEmpty()) {

        view->load(QUrl::fromUserInput(url));
    }
}

void BrowserWindow::showLinks() {

    imageWindow->clearLinks();
    // Change back to normal tracking mode
    actionManager->pause();
    actionManager->setMode(2);

    QWebElementCollection linkElements = view->page()->mainFrame()->findAllElements("a");
    QWebElement element;

    // get viewport of browser window
    QPoint vp = view->page()->mainFrame()->scrollPosition();

    for (int i = 0; i < linkElements.count(); i++) {
        element = linkElements.at(i);

        // Don't show hidden links
        if (element.geometry().width() == 0)
            continue;

        int x = element.geometry().x();
        int y = element.geometry().y();
        int w = screenSize.width();
        int h = screenSize.height();

        // only draw if in viewport
        if ((vp.x() <= x && x <= (vp.x() + w)) &&
                (vp.y() <= y && y <= (vp.y() + h))) {

            QImage* image = new QImage(view->page()->viewportSize(), QImage::Format_ARGB32);
            QPainter* painter = new QPainter(image);
            // Take an image with some offset
            QRect r;
            r.setX(max(0, x - vp.x() - 30));
            r.setY(max(0, y - vp.y() - 30));
            r.setWidth(element.geometry().width() + 60);
            r.setHeight(element.geometry().height() + 60);

            view->page()->mainFrame()->render(painter, r);
            painter->end();

            // Copy image
            QImage q = image->copy(r);
            // Shrink links if they are to large
            if (q.width() > (screenSize.width() / 4))
                q = q.scaledToWidth(screenSize.width() / 4);

            if (q.height() > (screenSize.height() / 3))
                q = q.scaledToHeight(screenSize.height() / 3);

            Link l;
            l.image = q;
            l.href = element.attribute("href");

            imageWindow->addLink(l);

        }
    }

    if (imageWindow->linkSize() == 0) {
        alertInfo(tr("No links in the viewport!"), true);
        actionManager->setMode(0);
    } else {
        imageWindow->showFullScreen();

    }
    actionManager->resume();
}

void BrowserWindow::previousLinkPageCallback(cv::Point p) {

    Q_UNUSED(p);
    imageWindow->back();
}

void BrowserWindow::nextPageCallback(cv::Point p) {

    Q_UNUSED(p);
    imageWindow->forward();
}

void BrowserWindow::closeLinkWindow(cv::Point p) {

    Q_UNUSED(p);
    imageWindow->close();

}

void BrowserWindow::selectLinkPageCallback(cv::Point p) {

    imageWindow->openLink(p);
}

void BrowserWindow::showLinksCallback(cv::Point p) {

    Q_UNUSED(p);
    actionManager->pause();
    this->showLinks();
}

void BrowserWindow::scrollLeft() {

    QString code = "$('html, body').animate({ scrollLeft: $('body').scrollLeft() - $(window).width() }, 800);";
    view->page()->mainFrame()->evaluateJavaScript(code);
    
}

void BrowserWindow::scrollLeftCallback(cv::Point p) {
    
    Q_UNUSED(p);
    this->scrollLeft();   
}

void BrowserWindow::scrollRight() {

    QString code = "$('html, body').animate({ scrollLeft: $('body').scrollLeft() + $(window).width() }, 800);";
    view->page()->mainFrame()->evaluateJavaScript(code);
}

void BrowserWindow::scrollRightCallback(cv::Point p) {
    Q_UNUSED(p);
    this->scrollRight();
}

void BrowserWindow::scrollUp() {

    QString code = "$('html, body').animate({ scrollTop: $('body').scrollTop() - $(window).height() }, 800);";
    view->page()->mainFrame()->evaluateJavaScript(code);
}

void BrowserWindow::scrollUpCallback(cv::Point p) {

    Q_UNUSED(p);
    actionManager->pause();
    this->scrollUp();
    actionManager->resume();
}

void BrowserWindow::scrollDown() {

    QString code = "$('html, body').animate({ scrollTop: $('body').scrollTop() + $(window).height() }, 800);";
    view->page()->mainFrame()->evaluateJavaScript(code);
}

void BrowserWindow::scrollDownCallback(cv::Point p) {

    Q_UNUSED(p);
    actionManager->pause();
    this->scrollDown();
    actionManager->resume();
}

void BrowserWindow::forward() {

    view->forward();
}

void BrowserWindow::forwardCallback(cv::Point p) {

    Q_UNUSED(p);
    actionManager->pause();
    this->forward();
}

void BrowserWindow::back() {

    view->back();
}

void BrowserWindow::backCallback(cv::Point p) {

    Q_UNUSED(p);
    actionManager->pause();
    this->back();
}

void BrowserWindow::zoomIn() {

    qreal zoomFactor = view->zoomFactor();
    zoomFactor++;
    view->setZoomFactor(zoomFactor);
}

void BrowserWindow::zoomOut() {

    qreal zoomFactor = view->zoomFactor();

    if (zoomFactor > 1)
        zoomFactor--;

    view->setZoomFactor(zoomFactor);

}

/*
 * Gaze Methods:
 *  - start_calibration() - Loads the calibration screen
 *  - calibrate() - starts the calibrationThread
 * 
 */

void BrowserWindow::start_calibration() {

    hideNavigationWidget();
    isCalibrating = true;
    view->load(QUrl("qrc:/calibration.html"));
}

void BrowserWindow::stop_tracking() {

    tManager->goIdle();
}

void BrowserWindow::resume_tracking() {

    tManager->resumeTracking();
}

void BrowserWindow::calibrate() {

    tManager->calibrate();
}

void BrowserWindow::execJsCommand(QString command) {

    view->page()->mainFrame()->evaluateJavaScript(command);
}

/*
 * Some UI Methods 
 * 
 */

void BrowserWindow::quit_gazebrowser() {
    // todo valid?

    delete settings;

    QApplication::exit(0);
}

void BrowserWindow::alertError(QString message) {

    MessageWindow m;
    m.showException(message);

}

void BrowserWindow::alertInfo(QString message, bool autohide) {

    MessageWindow m;
    m.showInfo(message, autohide, 3);
}

void BrowserWindow::preferences() {

    settingsWin->show();
}

void BrowserWindow::bookmarks() {

    bookmarksWin->resize(500, 300);
    bookmarksWin->show();
}

void BrowserWindow::showCvImage(cv::Mat *mat) {

    if (eye_widget->isVisible())
        eye_widget->sendImage(mat);
    delete mat;
}

void BrowserWindow::showBookmarkPage() {

    view->load(QUrl("qrc:/bookmarks.html"));
}

void BrowserWindow::showBookmarkPageCallback(cv::Point p) {

    Q_UNUSED(p);
    actionManager->pause();
    this->showBookmarkPage();
}

void BrowserWindow::show_eye_widget() {

    eye_widget->show();
}

void BrowserWindow::hideImageWindowEvent() {
    // Change back to normal tracking mode

    actionManager->setMode(0);
}

void BrowserWindow::trackingStatus(bool trackingActive, bool isCalibrated) {
    emit isTracking(trackingActive);
    if (trackingActive)
        emit canResumeTracking(false);

    else
        emit canResumeTracking(isCalibrated);

}

void BrowserWindow::showNavigationWidget() {

    navigationWidget->setVisible(true);
}

void BrowserWindow::hideNavigationWidget() {

    navigationWidget->setVisible(false);
}

GazeAction* BrowserWindow::createGazeAction(string name, cv::Rect rect, commitAction callback) {

    GazeAction* action = new GazeAction(name, rect, GAZE_ACTION_PREPARE_TIME, GAZE_ACTION_COMMIT_TIME, this, callback, gazePointer);

    return action;
}

