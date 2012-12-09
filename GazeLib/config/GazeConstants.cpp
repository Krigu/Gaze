/*
 * GazeConstants.cpp
 *
 *  Created on: Nov 9, 2012
 *      Author: fri
 */

#include "GazeConstants.hpp"

#include <stdlib.h>

using namespace std;

//
// global constants
//
const char GazeConstants::GLINT_COUNT;
const unsigned short GazeConstants::MAX_RANSAC_ITERATIONS;
const unsigned short GazeConstants::NUM_OF_SMOOTHING_FRAMES;

//
// Glints Configuration
//

double GazeConstants::GLINT_THRESHOLD;
short GazeConstants::GLINT_MIN_DIAMETER;
short GazeConstants::GLINT_MAX_DIAMETER;
short GazeConstants::GLINT_MIN_PIXEL;
short GazeConstants::GLINT_MAX_PIXEL;
short GazeConstants::GLINT_MIN_DISTANCE;
short GazeConstants::GLINT_MAX_DISTANCE;

//
// Haar configuration
//

short GazeConstants::HAAR_EYEREGION_MAX_HEIGHT;
short GazeConstants::HAAR_EYEREGION_MAX_WIDTH;
short GazeConstants::HAAR_EYE_MIN_HEIGHT;
short GazeConstants::HAAR_EYE_MIN_WIDTH;
short GazeConstants::HAAR_EYE_MAX_HEIGHT;
short GazeConstants::HAAR_EYE_MAX_WIDTH;
short GazeConstants::HAAR_FINDREGION_MAX_TRIES;

//
// RANSAC stuff
//
short GazeConstants::PUPIL_MIN_RADIUS;
short GazeConstants::PUPIL_MAX_RADIUS;

string GazeConstants::inHomeDirectory(string suffix) {
	string home(getenv("HOME"));
	return home + "/" + suffix;
}
