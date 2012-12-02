
 #include <QtGui>
 #include <QtWebKit>

#include "ui/CVWidget.hpp"
 
//TODO: just for demo purposes
#include <string>
#include "GazeConstants.hpp"
#include "detection/GazeTracker.hpp"
#include "video/VideoSource.hpp"

 class MainWindow : public QMainWindow
 {
     Q_OBJECT

 public:
     MainWindow(const QUrl& url);
    
 protected slots:

     void adjustTitle();
     void setProgress(int p);
     void finishLoading(bool);

     void highlightAllLinks();
     void scrollUp();
     void scrollDown();
     void rotateImages(bool invert);
     void back();
     void forward();
     void toggle_eye_widget();
     void just_a_demo();
     
 private:
     QString jQuery;
     QWebView *view;
     CVWidget *eye_widget;
     int progress; 
     void exec_webaction(QWebPage::WebAction action);
 };

 
class UICallback : public TrackerCallback{
     
 private:
     CVWidget *widget;
     
 public:
     UICallback(CVWidget *eye_widget) : widget(eye_widget){
         
     }
     ~UICallback(){
         
     }
     
     void imageProcessed(Mat& result);
 };