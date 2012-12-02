
#include <QtNetwork>
#include <QtWebKit>
#include <QtGui/qapplication.h>

#include "BrowserWindow.hpp"

#include <iostream>
using namespace std;

 MainWindow::MainWindow(const QUrl& url)
 {
     progress = 0;

     QFile file;
     file.setFileName(":js/jquery-1.8.3.min.js");
     file.open(QIODevice::ReadOnly);
     jQuery = file.readAll();
     file.close();
     
     isCalibrating = false;
     
     QNetworkProxyFactory::setUseSystemConfiguration(true);

     view = new QWebView(this);
     view->load(url);
     //connect(view, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
     connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
     connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
     connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

     QMenu *gazeMenu = menuBar()->addMenu(tr("&Gaze Actions"));
     gazeMenu->addAction("Calibration", this, SLOT(start_calibration()));
     gazeMenu->addAction("Scroll Up", this, SLOT(scrollUp()));
     gazeMenu->addAction("Scroll Down", this, SLOT(scrollDown()));
     gazeMenu->addAction("Find Links", this, SLOT(highlightAllLinks()));
     gazeMenu->addAction("Back", this, SLOT(back()));
     gazeMenu->addAction("Forward", this, SLOT(forward()));
     gazeMenu->addAction("Enable/Disable Eye Widget", this, SLOT(toggle_eye_widget()));
     gazeMenu->addAction("Show me a Demo!", this, SLOT(just_a_demo()));
     //gazeMenu->addAction("Quit Browser", this, SLOT(quit_gazebrowser()));
     
     QMenu *browserMenu = menuBar()->addMenu(tr("&View"));
     // Zoom
     QMenu *zoomMenu = browserMenu->addMenu(tr("&Zoom"));     
     zoomMenu->addAction("Zoom in", this, SLOT(zoomIn()));
     zoomMenu->addAction("Zoom out", this, SLOT(zoomOut()));
     
     setCentralWidget(view);
     
     eye_widget = new CVWidget(this);
     eye_widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
     eye_widget->setStyleSheet("QLabel { background-color : red; color : blue; }");
     eye_widget->setVisible(false);
     
     setUnifiedTitleAndToolBarOnMac(true);
 }

 void MainWindow::adjustTitle()
 {
     if (progress <= 0 || progress >= 100)
         setWindowTitle(view->title());
     else
         setWindowTitle(QString("%1 (%2%)").arg(view->title()).arg(progress));
 }

 void MainWindow::setProgress(int p)
 {
     progress = p;
     adjustTitle();
 }

 void MainWindow::finishLoading(bool)
 {
     progress = 100;
     adjustTitle();
     view->page()->mainFrame()->evaluateJavaScript(jQuery);
     
     if(isCalibrating)
         this->calibrate();
 }

 void MainWindow::highlightAllLinks()
 {
     QString code = "$('a').each( function () { $(this).css('background-color', 'yellow') } )";
     view->page()->mainFrame()->evaluateJavaScript(code);
 }

 void MainWindow::scrollUp()
 {
     QString code = "$('html, body').animate({ scrollTop: $('body').scrollTop() - $(window).height() }, 800);";
     view->page()->mainFrame()->evaluateJavaScript(code);
 }
 
 void MainWindow::scrollDown()
 {
     QString code = "$('html, body').animate({ scrollTop: $('body').scrollTop() + $(window).height() }, 800);";
     view->page()->mainFrame()->evaluateJavaScript(code);
 }
 
 void MainWindow::rotateImages(bool invert)
 {
     QString code;
     if (invert)
         code = "$('img').each( function () { $(this).css('-webkit-transition', '-webkit-transform 2s'); $(this).css('-webkit-transform', 'rotate(180deg)') } )";
     else
         code = "$('img').each( function () { $(this).css('-webkit-transition', '-webkit-transform 2s'); $(this).css('-webkit-transform', 'rotate(0deg)') } )";
     view->page()->mainFrame()->evaluateJavaScript(code);
 }

 void MainWindow::forward(){
     this->exec_webaction(QWebPage::Forward);
 }
 
 void MainWindow::back(){
     this->exec_webaction(QWebPage::Back);
 }
 
 void MainWindow::exec_webaction(QWebPage::WebAction action){
     view->pageAction(action);
 }
 
 void MainWindow::toggle_eye_widget(){
     eye_widget->setGeometry(0,this->height() - 90,120,90);
     eye_widget->setVisible(!eye_widget->isVisible());
 }
 
 //TODO: remove this
 void UICallback::imageProcessed(Mat& im){
         widget->sendImage(&im);
 }
 
 TrackerCallback::~TrackerCallback(){
     // I have no idea, why I had to declare this here...
 }
 
 void MainWindow::just_a_demo(){
        UICallback myCallback(eye_widget);
        string path = GazeConstants::inHomeDirectory("Dropbox/gaze/videos/osx/krigu_cut.mov");
 	VideoSource videoSource(path);
	GazeTracker tracker(videoSource, &myCallback);
	tracker.startTracking();
 }
 void MainWindow::quit_gazebrowser(){
     //this-
     QApplication::exit(0);
 }
 
 void MainWindow::start_calibration(){
     isCalibrating = true;
     QFile file;
     file.setFileName(":/calibration.html");
     file.open(QIODevice::ReadOnly);    
     view->setHtml(file.readAll(), QUrl("qrc:/"));
     file.close();
 }
 
 void MainWindow::calibrate(){
     //TODO calibrate here...
     QString code = "calibrationCircle.move(200,200);";
     view->page()->mainFrame()->evaluateJavaScript(code);
 }
 void MainWindow::zoomIn(){
     qreal zoomFactor = view->zoomFactor();
     zoomFactor++;
     view->setZoomFactor(zoomFactor);
 }
 
 void MainWindow::zoomOut(){
     qreal zoomFactor = view->zoomFactor();
     zoomFactor--;
     view->setZoomFactor(zoomFactor);
 
 }
