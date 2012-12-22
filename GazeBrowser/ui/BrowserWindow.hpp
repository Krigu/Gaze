
#include <QtGui>
#include <QtWebKit>
#include <QSettings>

#include "CVWidget.hpp"

#include "tracking/CalibrationThread.hpp"
#include "video/ImageSource.hpp"

class BrowserWindow : public QMainWindow {
    Q_OBJECT

public:
    BrowserWindow(const QUrl& url);
    BrowserWindow();

public slots:
    void execJsCommand(QString command);
    void alertMessage(QString message);
    void showCvImage(cv::Mat mat);

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

    void quit_gazebrowser();
    void preferences();
    void bookmarks();

    // View actions
    void zoomIn();
    void zoomOut();
    void init();

private:
    QString jQuery;
    QWebView *view;
    CVWidget *eye_widget;
    CalibrationThread *calibrator;
    ImageSource *source;
    int progress;
    QSettings * settings;
    bool isCalibrating;
    void exec_webaction(QWebPage::WebAction action);
    void calibrate();
    void setupMenus();
    void showBookmarkPage();
};