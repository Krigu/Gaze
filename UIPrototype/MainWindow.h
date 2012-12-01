
 #include <QtGui>
 #include <QtWebKit>
 
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
     
 private:
     QString jQuery;
     QWebView *view;
     //TODO: use a CvWidget instead of a label...
     QLabel *label;
     int progress; 
     void exec_webaction(QWebPage::WebAction action);
 };
