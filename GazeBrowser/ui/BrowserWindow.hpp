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
#include "GazePointer.hpp"
#include "actions/GazeAction.hpp"


class ActionManager;
class ImageWindow;

/**
 * the GazeWebPage sets the GazeBrowsers user agent 
 * and controls the page caching
 */
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


/**
 * The BrowserWindow class is the EntryPoint of the Application. this class 
 * instantiates all managers and sets up the user interface.
 */
class BrowserWindow : public QMainWindow {
    Q_OBJECT

    // let the thread manager access our members
    friend class ThreadManager;


public:
    BrowserWindow(const int cameraChannel = 0);
    ~BrowserWindow();

public slots:
    /**
     * executes the given javascript command inside the website
     * @param command a valid javascript command
     */
    void execJsCommand(QString command);
    
    /**
     * display the given OpenCV Image in an OpenGL Window
     * @param mat
     */
    void showCvImage(cv::Mat *mat);

signals:
    void isTracking(bool);
    void canResumeTracking(bool);

protected:
    // Overloaded methods for the gaze action callbacks
    void showLinksCallback(cv::Point p);
    void scrollUpCallback(cv::Point p);
    void scrollDownCallback(cv::Point p);
    void backCallback(cv::Point p);
    void forwardCallback(cv::Point p);
    void scrollLeftCallback(cv::Point p);
    void scrollRightCallback(cv::Point p);
    void openLinkCallback(cv::Point p);
    void showBookmarkPageCallback(cv::Point p);

    void previousLinkPageCallback(cv::Point p);
    void nextPageCallback(cv::Point p);
    void selectLinkPageCallback(cv::Point p);
    void closeLinkWindow(cv::Point p);



protected slots:

    void showEvent(QShowEvent *event);
    void finishLoading(bool);

    // Browser actions
    void showLinks();
    void scrollUp();
    void scrollDown();
    void scrollLeft();
    void scrollRight();
    void back();
    void forward();
    void showBookmarkPage();


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
    QSettings * settings;
    bool isCalibrating;
    QWidget *navigationWidget;
    QSize screenSize;
    ActionManager *actionManager;
    ImageWindow *imageWindow;
    int cameraChannel;
    GazePointer *gazePointer;

    void calibrate();
    void setupMenus();
    void alertError(QString message);
    void alertInfo(QString message, bool autohide);
    void trackingStatus(bool trackingActive, bool isCalibrated);
    void setUpNavigation();
    void setUpGazeActions();
    void setUpLinkWindow();

    GazeAction *createGazeAction(string name, cv::Rect rect, commitAction callback);
private slots:
    void setUpCamera();

};
