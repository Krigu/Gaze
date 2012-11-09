/*
 * GazeConstants.cpp
 *
 *  Created on: Nov 9, 2012
 *      Author: fri
 */

#include "GazeConstants.hpp"

#include <iostream>

using namespace std;

GazeConstants::GazeConstants() {
	// TODO Auto-generated constructor stub

}

GazeConstants::~GazeConstants() {
	// TODO Auto-generated destructor stub
}

string GazeConstants::inHomeDirectory(string suffix) {
	string home(getenv("HOME"));
	return home + "/" + suffix;
}
