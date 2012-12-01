
 #include <QtGui>
 #include <QtWebKit>

 class QWebView;
 class QLineEdit;

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
     
 private:
     QString jQuery;
     QWebView *view;
     int progress;
     void exec_webaction(QWebPage::WebAction action);
 };