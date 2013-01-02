/* 
 * File:   ActionManager.cpp
 * Author: fri
 * 
 * Created on December 30, 2012, 7:38 PM
 */

#include <iostream>

#include "utils/geometry.hpp"

#include "ActionManager.hpp"
#include "GazeAction.hpp"

using namespace std;

ActionManager::ActionManager() {
}

ActionManager::~ActionManager() {
}

void ActionManager::addGazeAction(GazeAction * action) {
    actions.push_back(action);
}

void ActionManager::measuredPoint(cv::Point p) {
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
