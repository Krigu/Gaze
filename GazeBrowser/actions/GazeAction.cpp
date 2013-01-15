#include <iostream>

#include "utils/geometry.hpp"

#include "GazeAction.hpp"
#include "ui/GazePointer.hpp"
#include "ui/BrowserWindow.hpp"

using namespace std;

GazeAction::GazeAction(std::string name, cv::Rect region, int prepareHits, int commitHits, BrowserWindow *browserWindow, commitAction callback, GazePointer *gazePointer) : actionName(name), region(region), prepareTime(prepareHits), commitTime(commitHits), browserWindow(browserWindow), actionCallback(callback), gazePointer(gazePointer) {

    barycenter = calcRectBarycenter(region);
    timer = NULL;
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
    if (timer == NULL){
        timer = new QTime();
        timer->start();
    }
    
    int elapsed = timer->elapsed();
    
    if (elapsed >= commitTime) {
        //cout << actionName << " commitAction " << endl;  
        gazePointer->commitAction(barycenter);
        (browserWindow ->*actionCallback) (barycenter);
        delete timer;
        timer = NULL;
        return;
    }
    
    if (elapsed >= prepareTime) {
        int percentage = (double)(elapsed - prepareTime) / (commitTime - prepareTime) * 100;
        gazePointer->prepareAction(barycenter, percentage);
        return;
    }

}

void GazeAction::unfocus() {
    if (timer == NULL)
        return;
    
    if (timer->elapsed() >= prepareTime) {
        //cout << actionName << " abortAction " << endl;
        gazePointer->abortAction();
    }
    delete timer;
    timer = NULL;    
}