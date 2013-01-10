/* 
 * File:   GazeAction.cpp
 * Author: krigu
 * 
 * Created on January 2, 2013, 1:07 PM
 */


#include <iostream>

#include "utils/geometry.hpp"

#include "GazeAction.hpp"

using namespace std;

GazeAction::GazeAction(std::string name, cv::Rect region, int prepareHits, int commitHits) : actionName(name), region(region), prepareHits(prepareHits), commitHits(commitHits), hitCounter(0) {

    cout << "Region: " << region.x << "/" << region.y << "-" << region.width << " - " << region.height << endl;
}

GazeAction::~GazeAction() {
}

string GazeAction::getActionName() const {
    return actionName;
}

cv::Rect GazeAction::getRegion() const {
    return region;
}

void GazeAction::focus() {
    hitCounter++;
    if (hitCounter >= commitHits) {
        //cout << actionName << " commitAction " << endl;        
        emit commitAction(calcRectBarycenter(region));
        hitCounter = 0;
        return;
    }
    if (hitCounter >= prepareHits) {
        //cout << actionName << " prepareAction " << endl;
        int percentage = 100 / (commitHits - prepareHits) * (commitHits - hitCounter);
        emit prepareAction(calcRectBarycenter(region), percentage);
        return;
    }

}

void GazeAction::unfocus() {
    if (hitCounter >= prepareHits) {
        //cout << actionName << " abortAction " << endl;
        emit abortAction();
    }
    hitCounter = 0;
}