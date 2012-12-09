
 #include <QtGui>
 #include <QtWebKit>

#include "CVWidget.hpp"
 
#include "tracking/CalibrationThread.hpp"

//TODO: just for demo purposes
#include <string>
#include "config/GazeConfig.hpp"
#include "detection/GazeTracker.hpp"
#include "video/ImageSource.hpp"

 class MainWindow : public QMainWindow
 {
     Q_OBJECT

 public:
     MainWindow(const QUrl& url);
    
public slots:
    void execJsCommand(QString command);
    void alertMessage(QString message);
     
 protected slots:

     void adjustTitle();
     void setProgress(int p);
     void finishLoading(bool);

     // Gaze actions
     void highlightAllLinks();
     void scrollUp();
     void scrollDown();
     void rotateImages(bool invert);
     void back();
     void forward();
     void toggle_eye_widget();
     void start_calibration();
     void just_a_demo();
     void quit_gazebrowser();
     void preferences();
     
     // View actions
     void zoomIn();
     void zoomOut();
     
 private:
     QString jQuery;
     QWebView *view;
     CVWidget *eye_widget;
     CalibrationThread *calibrator;
     ImageSource *source;
     int progress; 
     bool isCalibrating;
     void exec_webaction(QWebPage::WebAction action);
     void calibrate();
     void setupMenus();
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