
#include <QtGui>
#include <QtWebKit>
#include <QSettings>

#include "CVWidget.hpp"
#include "BookmarksWindow.hpp"
#include "SettingsWindow.hpp"

#include "threads/worker/GazeTrackWorker.hpp"
#include "video/ImageSource.hpp"

#include "threads/ThreadManager.hpp"
#include "ImageWindow.hpp"

class ActionManager;
class ImageWindow;

class GazeWebPage : public QWebPage {
public:

    GazeWebPage() : QWebPage() {
        //  QWebPage::QWebPage(); 
        settings()->setMaximumPagesInCache(10000);
    }

    QString userAgentForUrl(const QUrl &url) const {
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
    BrowserWindow(const int cameraChannel = 0);
    ~BrowserWindow();

public slots:
    void execJsCommand(QString command);
    void showCvImage(cv::Mat *mat);

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
    void showBookmarkPage();
    // Overloaded methods for the gaze action callbacks
    void showLinks(cv::Point p);
    void scrollUp(cv::Point p);
    void scrollDown(cv::Point p);
    void back(cv::Point p);
    void forward(cv::Point p);
    void openLink(cv::Point p);
    void showBookmarkPage(cv::Point p);

    void show_eye_widget();
    void start_calibration();
    void stop_tracking();
    void resume_tracking();

    void quit_gazebrowser();
    void preferences();
    void bookmarks();
    
    void hideImageWindowEvent();

    // View actions
    void zoomIn();
    void zoomOut();
    void showNavigationWidget();
    void hideNavigationWidget();
    void init();

    // Navigation actions
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
    ImageWindow *imageWindow;
    int cameraChannel;

    void calibrate();
    void setupMenus();
    void alertError(QString message);
    void alertInfo(QString message, bool autohide);
    void trackingStatus(bool trackingActive, bool isCalibrated);
    void setUpNavigation();
    void setUpGazeActions();
    void setUpLinkWindow();

private slots:
    void setUpCamera();

};