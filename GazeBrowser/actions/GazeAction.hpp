/* 
 * File:   GazeAction.hpp
 * Author: krigu
 *
 * Created on January 2, 2013, 1:07 PM
 */

#ifndef GAZEACTION_HPP
#define	GAZEACTION_HPP

#include <string>

#include <opencv2/core/core.hpp>
#include <QtCore>



class GazePointer;
class BrowserWindow;

typedef void (BrowserWindow::*commitAction)(cv::Point);

class GazeAction : public QObject {
    Q_OBJECT

public:
    GazeAction(std::string name, cv::Rect region, int prepareTimeInMillis, int commitTimeInMillis, BrowserWindow *browserWindow, commitAction callback, GazePointer *gazePointer);
    virtual ~GazeAction();

    std::string getActionName() const;
    cv::Rect getRegion() const;

    void focus();
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

