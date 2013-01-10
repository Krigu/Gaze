
#include <iostream>
#include <algorithm>

#include <QtNetwork>
#include <QtWebKit>
#include <QtGui/qapplication.h>
#include <QtGui/qlabel.h>
#include <QtCore/qdatetime.h>

#include "BrowserWindow.hpp"
#include "video/LiveSource.hpp"
#include "video/VideoSource.hpp"
#include "MessageWindow.hpp"
#include "ImageWindow.hpp"
#include "ImageLinkLabel.hpp"
#include "threads/ThreadManager.hpp"
#include "actions/ActionManager.hpp"


using namespace std;

BrowserWindow::BrowserWindow(const int cameraChannel) : cameraChannel(cameraChannel) {
    
    init();
    showBookmarkPage();

    // Change to bookmark mode
    actionManager->setMode(1);
}

BrowserWindow::~BrowserWindow() {
    // TODO which objects to free?
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

    progress = 0;
    isCalibrating = false;

    QNetworkProxyFactory::setUseSystemConfiguration(true);

    view = new QWebView(this);

    connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
    connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

    // add our own webpage to control the user-agent
    webpage = new GazeWebPage;
    view->setPage(webpage);

    // TODO introduce setUpEyeWidget, setUpBookmarksWin, etc.?
    eye_widget = new CVWidget;
    eye_widget->setWindowModality(Qt::WindowModal);
    eye_widget->setAttribute(Qt::WA_DeleteOnClose, false); //only hide the widget

    bookmarksWin = new BookmarksWindow(settings);
    bookmarksWin->setWindowModality(Qt::WindowModal);
    bookmarksWin->setAttribute(Qt::WA_DeleteOnClose, false);

    settingsWin = new SettingsWindow();
    settingsWin->setWindowModality(Qt::WindowModal);
    settingsWin->setAttribute(Qt::WA_DeleteOnClose, false);

    setUpLinkWindow();

    setUpNavigation();

    // Create browser window
    QVBoxLayout* hLayout = new QVBoxLayout();
    hLayout->setMargin(0);
    hLayout->addWidget(view);
    hLayout->addWidget(navigationWidget);

    // Set layout in QWidget
    QWidget *window = new QWidget();
    window->setLayout(hLayout);
    
    // add the pointer of the users gaze to the layout
    gazePointer = new GazePointer(window, Qt::WindowStaysOnTopHint);
    gazePointer->hide();
    
    setupMenus();
    setCentralWidget(window);
    
    setUpGazeActions();

    setUpGazeActions();

}

void BrowserWindow::setUpGazeActions() {

    // Some convienence variables
    int h = screenSize.height();
    int w = screenSize.width();
    int wThird = screenSize.width() / 3;
    int wFourth = screenSize.width() / 4;
    int hThird = screenSize.height() / 3;

    int prepareHits = 20;
    int commitHits = 30;

    // Screen bottom left
    Rect buttonBack(0, h - 150, wFourth, 150);
    GazeAction *actBack = new GazeAction("Back", buttonBack, prepareHits, commitHits);
    connect(actBack, SIGNAL(commitAction(cv::Point)), SLOT(back(cv::Point)));

    // Screen bottom left center
    Rect buttonLinks(wFourth, h - 150, wFourth, 150);
    GazeAction *actLinks = new GazeAction("Links", buttonLinks, prepareHits, commitHits);
    connect(actLinks, SIGNAL(commitAction(cv::Point)), SLOT(showLinks(cv::Point)));

    // Screen bottom left right
    Rect buttonBookmarks(wFourth * 2, h - 150, wFourth, 150);
    GazeAction *actBookmarks = new GazeAction("Bookmarks", buttonBookmarks, prepareHits, commitHits);
    connect(actBookmarks, SIGNAL(commitAction(cv::Point)), SLOT(showBookmarkPage(cv::Point)));

    // Screen bottom right
    Rect buttonForward(wFourth * 3, h - 150, wFourth, 150);
    GazeAction *actForward = new GazeAction("Forward", buttonForward, prepareHits, commitHits);
    connect(actForward, SIGNAL(commitAction(cv::Point)), SLOT(forward(cv::Point)));

    // Above Screen
    Rect scrollUp(0, -500, w, 500);
    GazeAction *actScrollUp = new GazeAction("Up", scrollUp, prepareHits, commitHits);
    connect(actScrollUp, SIGNAL(commitAction(cv::Point)), SLOT(scrollUp(cv::Point)));

    // Bellow screen
    Rect scrollDown(0, h, w, 500);
    GazeAction *actScrollDown = new GazeAction("Down", scrollDown, prepareHits, commitHits);
    connect(actScrollDown, SIGNAL(commitAction(cv::Point)), SLOT(scrollDown(cv::Point)));

    // Left of screen
    Rect scrollLeft(-500, 0, 500, h);
    GazeAction *actLinksBack = new GazeAction("Left", scrollLeft, prepareHits, commitHits);
    connect(actLinksBack, SIGNAL(commitAction(cv::Point)), SLOT(forward(cv::Point)));

    // Right of screen
    Rect scrollRight(w, 0, 500, h);
    GazeAction *actLinksForward = new GazeAction("Right", scrollRight, prepareHits, commitHits);
    connect(actLinksForward, SIGNAL(commitAction(cv::Point)), SLOT(forward(cv::Point)));

    vector<GazeAction*> browserWindowActions;
    browserWindowActions.push_back(actBack);
    browserWindowActions.push_back(actLinks);
    browserWindowActions.push_back(actForward);
    browserWindowActions.push_back(actScrollDown);
    browserWindowActions.push_back(actScrollUp);
    //    browserWindowActions.push_back(actScrollRight);
    //    browserWindowActions.push_back(actScrollLeft);

    vector<GazeAction*> bookmarkWindowActions;
    // First link
    for (int i = 0; i < 3; i++) { // rows
        for (int j = 0; j < 3; j++) { // cols
            int x = wThird * j;
            int y = hThird * i;

            Rect linkRect(x, y, wThird, hThird);
            GazeAction *actOpenLink = new GazeAction("Open Link", linkRect, prepareHits, commitHits);
            connect(actOpenLink, SIGNAL(commitAction(cv::Point)), SLOT(openLink(cv::Point)));
            connect(actOpenLink, SIGNAL(commitAction(cv::Point)), gazePointer, SLOT(commitAction(cv::Point)));
            connect(actOpenLink, SIGNAL(prepareAction(cv::Point,int)), gazePointer, SLOT(prepareAction(cv::Point,int)));
            connect(actOpenLink, SIGNAL(abortAction()), gazePointer, SLOT(abortAction()));
            bookmarkWindowActions.push_back(actOpenLink);
        }
    }

    vector<GazeAction*> linkWindowActions;
    // First link
    for (int i = 0; i < 4; i++) { // rows
        for (int j = 0; j < 3; j++) { // cols
            int x = wFourth * j;
            int y = hThird * i;

            Rect linkRect(x, y, wThird, hThird);
            GazeAction *actOpenLink = new GazeAction("Open Link", linkRect, prepareHits, commitHits);
            connect(actOpenLink, SIGNAL(commitAction(cv::Point)), imageWindow, SLOT(openLink(cv::Point)));
            linkWindowActions.push_back(actOpenLink);
        }
    }
    // Left of screen
    Rect previousPage(-500, 0, 500, h);
    GazeAction *actLinkWindowBack = new GazeAction("back", previousPage, prepareHits, commitHits);
    connect(actLinksBack, SIGNAL(commitAction(cv::Point)), imageWindow, SLOT(back(cv::Point)));
    linkWindowActions.push_back(actLinkWindowBack);

    // Right of screen
    Rect nexPage(w, 0, 500, h);
    GazeAction *actLinkWindowForward = new GazeAction("forward", nexPage, prepareHits, commitHits);
    connect(actLinksForward, SIGNAL(commitAction(cv::Point)), imageWindow, SLOT(forward(cv::Point)));
    linkWindowActions.push_back(actLinkWindowForward);

    // TODO: dynamic? & memory stuff
    typedef map<int, vector< GazeAction* > > GazeMap;
    GazeMap m;
    m.insert(GazeMap::value_type(0, browserWindowActions));
    m.insert(GazeMap::value_type(1, bookmarkWindowActions));
    m.insert(GazeMap::value_type(2, linkWindowActions));

    // Initialize Gaze Actions
    actionManager = new ActionManager(m);
}

void BrowserWindow::setUpLinkWindow() {

    imageWindow = new ImageWindow(view);
    imageWindow->setWindowModality(Qt::WindowModal);
    imageWindow->setAttribute(Qt::WA_DeleteOnClose, false);
    imageWindow->resize(screenSize.width(), screenSize.height());
    imageWindow->setVisible(false);
    // Let the browser know when the image window closes, to change the tracked actions
    connect(imageWindow, SIGNAL(hideWindow()), this, SLOT(hideImageWindowEvent()));

}

void BrowserWindow::setupMenus() {
    // Add the Slot to the quit button
    // on mac this will be showed in the unified menu bar

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
    //TODO document this, the UI must have been loaded before we start this
    tManager = new ThreadManager(this);
    tManager->goIdle();
    
}

/*
 * Methods handling the Website-Loading:
 *  - adjustTitle()
 *  - setProgress()
 *  - finishLoading()
 *  - goToPage()
 */
void BrowserWindow::adjustTitle() {
    if (progress <= 0 || progress >= 100)
        setWindowTitle(view->title());

    else
        setWindowTitle(QString("%1 (%2%)").arg(view->title()).arg(progress));
}

void BrowserWindow::setProgress(int p) {

    progress = p;
    adjustTitle();
}

void BrowserWindow::finishLoading(bool) {
    progress = 100;
    adjustTitle();
    view->page()->mainFrame()->evaluateJavaScript(jQuery);

    // TODO: cleanup code and don't check for every site
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

void BrowserWindow::openLink(cv::Point p) {
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
    // TODO release of memory from window
    imageWindow->show();

}

void BrowserWindow::showLinks(cv::Point p) {
    Q_UNUSED(p);
    this->showLinks();
}

void BrowserWindow::scrollUp() {

    QString code = "$('html, body').animate({ scrollTop: $('body').scrollTop() - $(window).height() }, 800);";
    view->page()->mainFrame()->evaluateJavaScript(code);
}

void BrowserWindow::scrollUp(cv::Point p) {
    Q_UNUSED(p);
    this->scrollUp();
}

void BrowserWindow::scrollDown() {

    QString code = "$('html, body').animate({ scrollTop: $('body').scrollTop() + $(window).height() }, 800);";
    view->page()->mainFrame()->evaluateJavaScript(code);
}

void BrowserWindow::scrollDown(cv::Point p) {
    Q_UNUSED(p);
    this->scrollDown();
}

void BrowserWindow::forward() {

    view->forward();
}

void BrowserWindow::forward(cv::Point p) {
    Q_UNUSED(p);
    this->forward();
}

void BrowserWindow::back() {

    view->back();
}

void BrowserWindow::back(cv::Point p) {
    Q_UNUSED(p);
    this->back();
}

void BrowserWindow::zoomIn() {

    qreal zoomFactor = view->zoomFactor();
    zoomFactor++;
    view->setZoomFactor(zoomFactor);
}

void BrowserWindow::zoomOut() {

    qreal zoomFactor = view->zoomFactor();
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
    m.showInfo(message, autohide, 4);
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

void BrowserWindow::showBookmarkPage(cv::Point p) {
    Q_UNUSED(p);
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
