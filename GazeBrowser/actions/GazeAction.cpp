/* 
 * File:   GazeAction.cpp
 * Author: krigu
 * 
 * Created on January 2, 2013, 1:07 PM
 */


#include <iostream>

#include "utils/geometry.hpp"

#include "GazeAction.hpp"
#include "ui/GazePointer.hpp"
#include "ui/BrowserWindow.hpp"

using namespace std;

GazeAction::GazeAction(std::string name, cv::Rect region, int prepareHits, int commitHits, BrowserWindow *browserWindow, commitAction callback, GazePointer *gazePointer) : actionName(name), region(region), prepareHits(prepareHits), commitHits(commitHits), hitCounter(0), browserWindow(browserWindow), actionCallback(callback), gazePointer(gazePointer) {

    barycenter = calcRectBarycenter(region);
    
    cout << name << " " << barycenter << endl;
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
        gazePointer->commitAction(barycenter);
        (browserWindow ->*actionCallback) (barycenter);
        hitCounter = 0;
        return;
    }
    if (hitCounter >= prepareHits) {
        int percentage = 100 / (commitHits - prepareHits) * (commitHits - hitCounter);
        gazePointer->prepareAction(barycenter, percentage);
        return;
    }

}

void GazeAction::unfocus() {
    if (hitCounter >= prepareHits) {
        //cout << actionName << " abortAction " << endl;
        gazePointer->abortAction();
    }
    hitCounter = 0;
}