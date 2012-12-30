/* 
 * File:   ActionManager.cpp
 * Author: fri
 * 
 * Created on December 30, 2012, 7:38 PM
 */

#include <iostream>


#include "ActionManager.hpp"


ActionManager::ActionManager() {
}

ActionManager::~ActionManager() {
}

void ActionManager::measuredPoint(cv::Point p){
    std::cout << "got a point!" << p << std::endl;
}
