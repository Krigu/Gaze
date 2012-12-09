/*
 * GazeConstants.cpp
 *
 *  Created on: Nov 9, 2012
 *      Author: fri
 */

#include "GazeConfig.hpp"

#include <stdlib.h>

using namespace std;

//
// global constants
//
const char GazeConfig::GLINT_COUNT;
const unsigned short GazeConfig::MAX_RANSAC_ITERATIONS;
const unsigned short GazeConfig::NUM_OF_SMOOTHING_FRAMES;

//
// Glints Configuration
//

double GazeConfig::GLINT_THRESHOLD;
short GazeConfig::GLINT_MIN_DIAMETER;
short GazeConfig::GLINT_MAX_DIAMETER;
short GazeConfig::GLINT_MIN_PIXEL;
short GazeConfig::GLINT_MAX_PIXEL;
short GazeConfig::GLINT_MIN_DISTANCE;
short GazeConfig::GLINT_MAX_DISTANCE;

//
// Haar configuration
//

short GazeConfig::HAAR_EYEREGION_MIN_HEIGHT;
short GazeConfig::HAAR_EYEREGION_MIN_WIDTH;
short GazeConfig::HAAR_EYE_MIN_HEIGHT;
short GazeConfig::HAAR_EYE_MIN_WIDTH;
short GazeConfig::HAAR_EYE_MAX_HEIGHT;
short GazeConfig::HAAR_EYE_MAX_WIDTH;
short GazeConfig::HAAR_FINDREGION_MAX_TRIES;

//
// RANSAC stuff
//
short GazeConfig::PUPIL_MIN_RADIUS;
short GazeConfig::PUPIL_MAX_RADIUS;

string GazeConfig::inHomeDirectory(string suffix) {
	string home(getenv("HOME"));
	return home + "/" + suffix;
}
