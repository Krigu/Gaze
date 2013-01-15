#include <iostream>

#include "utils/geometry.hpp"

#include "ActionManager.hpp"
#include "GazeAction.hpp"

using namespace std;

ActionManager::ActionManager(std::map<int, std::vector<GazeAction*> > actionMap) : actionMap(actionMap), mode(0), running(false) {
}

ActionManager::~ActionManager() {
}

void ActionManager::setMode(int actionMode) {
    if ((unsigned)actionMode >= actionMap.size()) {
        mode = 0;
    } else {
        mode = actionMode;
    }
}

void ActionManager::estimatedPoint(cv::Point p) {
    if (!running)
        return;

    cout << "ActionManager Point: " << p << "Mode: " << mode << endl;

    map<int, vector<GazeAction*> >::iterator mapIter = actionMap.find(mode);
    if (mapIter != actionMap.end()) {

        vector< GazeAction* > actions = mapIter->second;

        std::vector<GazeAction*>::iterator iter;
        for (iter = actions.begin(); iter != actions.end(); iter++) {
            cv::Rect r = (*iter)->getRegion();
            if (isPointInRect(p, r)) {
                (*iter)->focus();
            } else {
                (*iter)->unfocus();
            }
        }
    }

}

void ActionManager::clearActions() {
    actionMap.clear();
}

void ActionManager::pause() {
    this->running = false;
}

void ActionManager::resume() {
    this->running = true;
}
