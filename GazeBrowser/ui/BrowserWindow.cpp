
#include <iostream>

#include <QtNetwork>
#include <QtWebKit>
#include <QtGui/qapplication.h>

#include "BrowserWindow.hpp"
#include "video/LiveSource.hpp"
#include "video/VideoSource.hpp"
#include "MessageWindow.hpp"

using namespace std;

BrowserWindow::BrowserWindow(const QUrl& url) {

    init();
    view->load(url);

}

BrowserWindow::BrowserWindow() {

    init();
    showBookmarkPage();

}

void BrowserWindow::init() {

    settings = new QSettings("gazebrowser.ini", QSettings::IniFormat);

    QFile file;
    file.setFileName(":js/jquery-1.8.3.min.js");
    file.open(QIODevice::ReadOnly);
    jQuery = file.readAll();
    file.close();

    progress = 0;
    isCalibrating = false;
    calibrator = NULL;
    source = NULL;

    QNetworkProxyFactory::setUseSystemConfiguration(true);

    view = new QWebView(this);

    connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
    connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

    eye_widget = new CVWidget;
    eye_widget->setWindowModality(Qt::WindowModal);
    eye_widget->setAttribute(Qt::WA_DeleteOnClose, false); //only hide the widget
    
    bookmarksWin = new BookmarksWindow(settings);
    bookmarksWin->setWindowModality(Qt::WindowModal);
    bookmarksWin->setAttribute(Qt::WA_DeleteOnClose, false);
    
    settingsWin = new SettingsWindow();
    settingsWin->setWindowModality(Qt::WindowModal);
    settingsWin->setAttribute(Qt::WA_DeleteOnClose, false);
    
    setupMenus();
    
    setCentralWidget(view);
}

void BrowserWindow::setupMenus() {
    // Add the Slot to the quit button
    // on mac this will be showed in the unified menu bar
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *quitAction = new QAction("Quit Browser", this);
    quitAction->setMenuRole(QAction::QuitRole);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quit_gazebrowser()));
    fileMenu->addAction("Bookmarks", this, SLOT(bookmarks()));
    fileMenu->addAction("Preferences", this, SLOT(preferences()));
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);


    QMenu *navMenu = menuBar()->addMenu(tr("&Navigation"));
    navMenu->addAction("Back", this, SLOT(back()));
    navMenu->addAction("Forward", this, SLOT(forward()));
    navMenu->addSeparator();
    navMenu->addAction("Scroll Up", this, SLOT(scrollUp()));
    navMenu->addAction("Scroll Down", this, SLOT(scrollDown()));
    navMenu->addSeparator();
    navMenu->addAction("Go to page", this, SLOT(goToPage()));
    navMenu->addAction("Show Bookmarks", this, SLOT(showBookmarkPage()));

    QMenu *browserMenu = menuBar()->addMenu(tr("&View"));
    // Zoom
    QMenu *zoomMenu = browserMenu->addMenu(tr("&Zoom"));
    zoomMenu->addAction("Zoom in", this, SLOT(zoomIn()));
    zoomMenu->addAction("Zoom out", this, SLOT(zoomOut()));

    QMenu *gazeMenu = menuBar()->addMenu(tr("&Gaze Actions"));
    gazeMenu->addAction("Calibration", this, SLOT(start_calibration()));
    gazeMenu->addSeparator();
    gazeMenu->addAction("Find Links", this, SLOT(highlightAllLinks()));
    gazeMenu->addSeparator();
    gazeMenu->addAction("Show the Eye Widget", this, SLOT(show_eye_widget()));

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
    if (view->page()->mainFrame()->url().toString() == "qrc:/" && !isCalibrating) {
        QString html = "$('#%1').append("
                "\"<a href='%2'><img src='http://api.thumbalizr.com/?url=%3&width=260' alt=''></a>"
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
            "http://www.", &ok);
    if (ok && !text.isEmpty()) {
        // Check prefix
        if (!(text.startsWith("http://", Qt::CaseInsensitive) || text.startsWith("https://", Qt::CaseInsensitive))) {
            text = "http://" + text;
        }

        view->load(QUrl(text));
    }
}

/*
 * Methods for the "Gaze Actions"
 *  - highlightAllLinks()
 *  - scrollUp()
 *  - scrollDown()
 *  - forward()
 *  - back()
 *  - zoomIn() and zoomOut()
 */

void BrowserWindow::highlightAllLinks() {
    QString code = "$('a').each( function () { $(this).css('background-color', 'yellow') } )";
    view->page()->mainFrame()->evaluateJavaScript(code);
}

void BrowserWindow::scrollUp() {
    QString code = "$('html, body').animate({ scrollTop: $('body').scrollTop() - $(window).height() }, 800);";
    view->page()->mainFrame()->evaluateJavaScript(code);
}

void BrowserWindow::scrollDown() {
    QString code = "$('html, body').animate({ scrollTop: $('body').scrollTop() + $(window).height() }, 800);";
    view->page()->mainFrame()->evaluateJavaScript(code);
}

void BrowserWindow::forward() {
    view->forward();
}

void BrowserWindow::back() {
    view->back();
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
    isCalibrating = true;
    QFile file;
    file.setFileName(":/calibration.html");
    file.open(QIODevice::ReadOnly);
    view->setHtml(file.readAll(), QUrl("qrc:/"));
    file.close();
}

void BrowserWindow::calibrate() {
    //if (!source)
    //    source = new LiveSource;
    if(!source)
        source = new VideoSource(GazeConfig::inHomeDirectory("Dropbox/gaze/videos/choose_the_correct_eye_720p.mov"));
    qRegisterMetaType< cv::Mat > ("cv::Mat");
    calibrator = new CalibrationThread(view->width(), view->height(), source);
    connect(calibrator, SIGNAL(jsCommand(QString)), this, SLOT(execJsCommand(QString)));
    connect(calibrator, SIGNAL(error(QString)), this, SLOT(alertMessage(QString)));
    connect(calibrator, SIGNAL(cvImage(cv::Mat)), this, SLOT(showCvImage(cv::Mat)));
    calibrator->start();
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

void BrowserWindow::alertMessage(QString message) {
    MessageWindow m;
    m.showException(message);
}

void BrowserWindow::preferences() {
    settingsWin->show();
}

void BrowserWindow::bookmarks() {
    bookmarksWin->resize(500, 300);
    bookmarksWin->show();
}

void BrowserWindow::showCvImage(cv::Mat mat) {
    eye_widget->sendImage(&mat);
}

void BrowserWindow::showBookmarkPage() {
    QFile file;
    file.setFileName(":/bookmarks.html");
    file.open(QIODevice::ReadOnly);
    view->setHtml(file.readAll(), QUrl("qrc:/"));
    file.close();
}

void BrowserWindow::show_eye_widget() {
    eye_widget->show();
}