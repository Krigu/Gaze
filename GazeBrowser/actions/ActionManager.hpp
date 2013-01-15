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
    
    
    void pause();
    void resume();   

public slots:
    void estimatedPoint(cv::Point p);

private:
    std::map<int, std::vector<GazeAction*> > actionMap;
    int mode;
    int running;
};

#endif	/* ACTIONMANAGER_HPP */

