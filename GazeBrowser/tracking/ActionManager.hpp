/* 
 * File:   ActionManager.hpp
 * Author: fri
 *
 * Created on December 30, 2012, 7:38 PM
 */

#ifndef ACTIONMANAGER_HPP
#define	ACTIONMANAGER_HPP

#include <QtCore>
#include <opencv2/core/core.hpp>

#include "../tracking/GazeAction.hpp"

class ActionManager : public QObject {
    Q_OBJECT

public:
    ActionManager();
    virtual ~ActionManager();

    void addGazeAction(GazeAction * action);

public slots:
    void measuredPoint(cv::Point p);

private:
    std::vector<GazeAction*> actions;
};

#endif	/* ACTIONMANAGER_HPP */

