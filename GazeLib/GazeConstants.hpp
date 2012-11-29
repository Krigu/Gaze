/*
 * GazeConstants.hpp
 *
 *  Created on: Nov 9, 2012
 *      Author: fri
 */

#ifndef GAZECONSTANTS_HPP_
#define GAZECONSTANTS_HPP_

#include <string>

class GazeConstants {
public:
	static const double GLINT_THRESHOLD = 230;

	//
	// Glints Configuration
	//

	// Amounts of glints (per eye)
	static const char GLINT_COUNT = 4;

	static const short GLINT_MIN_DIAMETER = 2;
	static const short GLINT_MAX_DIAMETER = 6;

	static const short GLINT_RADIUS = 6;
	
    // TODO calculate from diameter
	static const short GLINT_MIN_PIXEL = 1;
	// TODO calculate from diameter
	static const short GLINT_MAX_PIXEL = 50;
	// Min distance between two glints
	static const short GLINT_MIN_DISTANCE = 1;
	// Max distance between two glints
	static const short GLINT_MAX_DISTANCE = 25;

	//
	// Haar configuration
	//

	// Max height of eyeregion
	static const short HAAR_EYEREGION_MAX_HEIGHT = 100;
	// Max width of eyeregion
	static const short HAAR_EYEREGION_MAX_WIDTH = 400;
	// Max height of eye
	static const short HAAR_EYE_MIN_HEIGHT = 20;
	// Max width of eye
	static const short HAAR_EYE_MIN_WIDTH = 20;
	// Max height of eye
	static const short HAAR_EYE_MAX_HEIGHT = 200;
	// Max width of eye
	static const short HAAR_EYE_MAX_WIDTH = 200;
	// Max amount of tries to find eye region
	static const short HAAR_FINDREGION_MAX_TRIES = 10;

	//
	// Pupilcenter
	//
	static const short PUPIL_SEARCH_AREA_WIDHT_HEIGHT = 90;


	// RANSAC stuff
	static const short PUPIL_MIN_RADIUS = 15;
	static const short PUPIL_MAX_RADIUS = 17;

	static const unsigned short MAX_RANSAC_ITERATIONS = 10;

	GazeConstants();

	/**
	 * returns a path to the users homedirectory
	 *
	 * you may add a suffix for example to point directly to a file
	 */
	static std::string inHomeDirectory(std::string suffix);

	virtual ~GazeConstants();
};

#endif /* GAZECONSTANTS_HPP_ */
