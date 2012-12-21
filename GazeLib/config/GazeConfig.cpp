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
const unsigned int GazeConfig::MAX_RANSAC_ITERATIONS;
const unsigned int GazeConfig::NUM_OF_SMOOTHING_FRAMES;

//
// Glints Configuration
//

int GazeConfig::GLINT_THRESHOLD;
int GazeConfig::GLINT_MIN_DIAMETER;
int GazeConfig::GLINT_MAX_DIAMETER;
int GazeConfig::GLINT_MIN_PIXEL;
int GazeConfig::GLINT_MAX_PIXEL;
int GazeConfig::GLINT_MIN_DISTANCE;
int GazeConfig::GLINT_MAX_DISTANCE;
int GazeConfig::GLINT_BARYCENTRE_DEVIATION;
//
// Haar configuration
//

int GazeConfig::HAAR_EYEREGION_MIN_HEIGHT;
int GazeConfig::HAAR_EYEREGION_MIN_WIDTH;
int GazeConfig::HAAR_EYE_MIN_HEIGHT;
int GazeConfig::HAAR_EYE_MIN_WIDTH;
int GazeConfig::HAAR_EYE_MAX_HEIGHT;
int GazeConfig::HAAR_EYE_MAX_WIDTH;
int GazeConfig::HAAR_FINDREGION_MAX_TRIES;

//
// RANSAC stuff
//
int GazeConfig::PUPIL_MIN_RADIUS;
int GazeConfig::PUPIL_MAX_RADIUS;

string GazeConfig::inHomeDirectory(string suffix) {
	string home(getenv("HOME"));
	return home + "/" + suffix;
}
