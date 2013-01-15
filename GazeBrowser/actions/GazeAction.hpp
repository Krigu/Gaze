#ifndef GAZEACTION_HPP
#define	GAZEACTION_HPP

#include <string>

#include <opencv2/core/core.hpp>
#include <QtCore>

class GazePointer;
class BrowserWindow;

typedef void (BrowserWindow::*commitAction)(cv::Point);

/**
 *  a GazeAction is an action that can be executed with a Gaze Tracker. each 
 * GazeAction has a position on the screen (usually a rect) where it can be enabled. 
 * if a user focuses the GazeAction for a defined time the action is triggered and 
 * a callback function is executed.
 */
class GazeAction : public QObject {
    Q_OBJECT

public:
    /**
     * 
     * @param name the name of this GazeAction
     * @param region the region where the action can be triggered
     * @param prepareTimeInMillis the time after which the GazePointer is displayed
     * @param commitTimeInMillis the time after which the action is executed
     * @param browserWindow a reference to the browser
     * @param callback the callback function that is executed after commitTime
     * @param gazePointer a reference to the GazePointer
     */
    GazeAction(std::string name, cv::Rect region, int prepareTimeInMillis, int commitTimeInMillis, BrowserWindow *browserWindow, commitAction callback, GazePointer *gazePointer);
    virtual ~GazeAction();

    std::string getActionName() const;
    cv::Rect getRegion() const;

    /**
     * starts measuring the prepare and commit time<br />commits the action
     * if the commit time has been exceeded
     */
    void focus();
    
    /**
     * stops measuring the prepare and commit time
     */
    void unfocus();

private:
    std::string actionName;
    cv::Rect region;
    int prepareTime;
    int commitTime;
    BrowserWindow *browserWindow;
    commitAction actionCallback;
    GazePointer *gazePointer;
    cv::Point barycenter;
    QTime * timer;

};

#endif	/* GAZEACTION_HPP */

