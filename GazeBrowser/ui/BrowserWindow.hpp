
#include <QtGui>
#include <QtWebKit>
#include <QSettings>

#include "CVWidget.hpp"
#include "BookmarksWindow.hpp"
#include "SettingsWindow.hpp"

#include "threads/worker/CalibrationWorker.hpp"
#include "video/ImageSource.hpp"

#include "threads/ThreadManager.hpp"

class ActionManager;

class GazeWebPage : public QWebPage {
 public:
    GazeWebPage() : QWebPage(){
      //  QWebPage::QWebPage(); 
    }
    QString userAgentForUrl(const QUrl &url ) const{
        Q_UNUSED(url);
        //TODO OS and Version in User-Agent?
        return QString("GazeBrowser");
    }
};

class BrowserWindow : public QMainWindow {
    Q_OBJECT

// let the thread manager access our members
friend class ThreadManager; 
        
    
public:
    BrowserWindow(const QUrl& url);
    BrowserWindow();

public slots:
    void execJsCommand(QString command);
    void showCvImage(cv::Mat mat);

    signals: 
        void isTracking(bool);
        void canResumeTracking(bool);
    
protected slots:

    void showEvent(QShowEvent *event);
    void adjustTitle();
    void setProgress(int p);
    void finishLoading(bool);

    // Browser actions
    void showLinks();
    void scrollUp();
    void scrollDown();
    void back();
    void forward();
    // Overloaded methods for the gaze action callbacks
    void showLinks(cv::Point p);
    void scrollUp(cv::Point p);
    void scrollDown(cv::Point p);
    void back(cv::Point p);
    void forward(cv::Point p);
    // select link
    void openLink(cv::Point p);
    
    void show_eye_widget();
    void start_calibration();
    void stop_tracking();
    void resume_tracking();

    void quit_gazebrowser();
    void preferences();
    void bookmarks();

    // View actions
    void zoomIn();
    void zoomOut();
    void showNavigationWidget();  
    void hideNavigationWidget();
    void init();
    
    // Navigation actions
    void showBookmarkPage();
    void goToPage();

private:
    QString jQuery;
    QWebView *view;
    GazeWebPage *webpage;
    CVWidget *eye_widget;
    BookmarksWindow *bookmarksWin;
    ThreadManager *tManager;
    SettingsWindow *settingsWin;
    ImageSource *source;
    int progress;
    QSettings * settings;
    bool isCalibrating;
    QWidget *navigationWidget;
    QSize screenSize;
    ActionManager *actionManager;
    
    void calibrate();
    void setupMenus();
    void alertMessage(QString message);
    void trackingStatus(bool trackingActive, bool isCalibrated);
    void setUpNavigation();
    void setUpGazeActions();
    
 private slots:
     void setUpCamera();
    
};