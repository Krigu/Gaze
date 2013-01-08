/* 
 * File:   ActionManager.hpp
 * Author: fri
 *
 * Created on December 30, 2012, 7:38 PM
 */

#ifndef ACTIONMANAGER_HPP
#define	ACTIONMANAGER_HPP

#include <map>
#include <vector>

#include <QtCore>
#include <opencv2/core/core.hpp>

#include "GazeAction.hpp"

class ActionManager : public QObject {
    Q_OBJECT

public:
    ActionManager(std::map<int, std::vector<GazeAction*> > actionMap);
    virtual ~ActionManager();

    void clearActions();
    void setMode(int mode);

public slots:
    void estimatedPoint(cv::Point p);

private:
    std::map<int, std::vector<GazeAction*> > actionMap;
    int mode;
};

#endif	/* ACTIONMANAGER_HPP */

