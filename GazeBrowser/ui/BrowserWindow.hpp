
#include <QtGui>
#include <QtWebKit>
#include <QSettings>

#include "CVWidget.hpp"
#include "BookmarksWindow.hpp"
#include "SettingsWindow.hpp"

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

    void showEvent(QShowEvent *event);
    void adjustTitle();
    void setProgress(int p);
    void finishLoading(bool);

    // Gaze actions
    void highlightAllLinks();
    void scrollUp();
    void scrollDown();
    void back();
    void forward();
    void show_eye_widget();
    void start_calibration();

    void quit_gazebrowser();
    void preferences();
    void bookmarks();

    // View actions
    void zoomIn();
    void zoomOut();
    void init();
    
    // Navigation actions
    void showBookmarkPage();
    void goToPage();

private:
    QString jQuery;
    QWebView *view;
    CVWidget *eye_widget;
    BookmarksWindow *bookmarksWin;
    CalibrationThread *calibrator;
    SettingsWindow *settingsWin;
    ImageSource *source;
    int progress;
    QSettings * settings;
    bool isCalibrating;
    void calibrate();
    void setupMenus();
    
 private slots:
     void setUpCamera();
    
};