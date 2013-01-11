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
    GazeAction(std::string name, cv::Rect region, int prepareHits, int commitHits, BrowserWindow *browserWindow, commitAction callback, GazePointer *gazePointer);
    virtual ~GazeAction();

    std::string getActionName() const;
    cv::Rect getRegion() const;

    void focus();
    void unfocus();

private:
    std::string actionName;
    cv::Rect region;
    int prepareHits;
    int commitHits;
    int hitCounter;
    BrowserWindow *browserWindow;
    commitAction actionCallback;
    GazePointer *gazePointer;
    cv::Point barycenter;

};

#endif	/* GAZEACTION_HPP */

