#include <iostream>

#include "AutoSaveSpinbox.hpp"
#include "config/GazeConfig.hpp"

using namespace std;

AutoSaveSpinbox::AutoSaveSpinbox(int& value) : constantValue(value) {

}


void AutoSaveSpinbox::adjustGazeValue(int value) {
    cout << "Val changed: "  << value << endl;
    constantValue = value;
}

AutoSaveSpinbox::~AutoSaveSpinbox() {
}

