#ifndef ACTIONMANAGER_HPP
#define	ACTIONMANAGER_HPP

#include <map>
#include <vector>

#include <QtCore>
#include <opencv2/core/core.hpp>

#include "GazeAction.hpp"

/**
 * The ActionManager class is responsible for enabling / disabling the GazeActions.
 * the slot estimatedPoint(cv::Point) is called for each measured Screen position. 
 * the manager then calculates which Action is triggered by this measurement and focuses it.
 * using a mode the ActionManager can trigger different GazeActions for the same screen position
 */
class ActionManager : public QObject {
    Q_OBJECT

public:
    /**
     * 
     * @param actionMap a map of all GazeActions and their position (rect) on the screen
     */
    ActionManager(std::map<int, std::vector<GazeAction*> > actionMap);
    virtual ~ActionManager();

    /**
     * removes all actions
     */
    void clearActions();
    
    /**
     * sets the current mode of the ActionManager
     * @param mode
     */
    void setMode(int mode);
    
    /**
     * when paused all incoming GazeVectors (estimated points) are silently discarded
     */
    void pause();
    
    /**
     * resumes the processing of the incoming measurements
     */
    void resume();   

public slots:
    
    /**
     * accepts measured Gaze positions and executes an action (if any)
     * @param p the position on the screen where the user is looking at
     */
    void estimatedPoint(cv::Point p);

private:
    std::map<int, std::vector<GazeAction*> > actionMap;
    int mode;
    int running;
};

#endif	/* ACTIONMANAGER_HPP */

