/* 
 * File:   GazeAction.cpp
 * Author: krigu
 * 
 * Created on January 2, 2013, 1:07 PM
 */


#include <iostream>

#include "GazeAction.hpp"

using namespace std;

GazeAction::GazeAction(std::string name, cv::Rect region, int minHits) : actionName(name), region(region), minHits(minHits) {
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
    if (hitCounter >= minHits){
        triggerAction();
        hitCounter = 0;
    }
    //cout << actionName << " hits: " << hitCounter << endl;
}

void GazeAction::unfocus() {
    hitCounter = 0;
    //cout << actionName << " hits: " << hitCounter << endl;
}

void GazeAction::triggerAction() {
    
    //cout << "Triggered action " << actionName;
}