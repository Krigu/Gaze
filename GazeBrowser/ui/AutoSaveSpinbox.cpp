/* 
 * File:   AutoSaveSpinbox.cpp
 * Author: krigu
 * 
 * Created on December 12, 2012, 1:14 PM
 */
#include <iostream>

#include "AutoSaveSpinbox.hpp"
#include "config/GazeConfig.hpp"

using namespace std;

AutoSaveSpinbox::AutoSaveSpinbox(int& value) : constantValue(value) {
    connect(this, SIGNAL(valueChanged(int)),
             this, SLOT(adjustGazeValue(int)));
}


void AutoSaveSpinbox::adjustGazeValue(int value) {
    cout << "Val changed: "  << value << endl;
    constantValue = value;
    cout << "GazeConfig::GLINT_THRESHOLD: " << GazeConfig::GLINT_THRESHOLD << endl;
}

AutoSaveSpinbox::~AutoSaveSpinbox() {
}

