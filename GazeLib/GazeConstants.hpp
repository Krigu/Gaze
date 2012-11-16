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
	static const double GLINT_THRESHOLD = 225;

	// Amounts of glints (per eye)
	static const char GLINT_COUNT = 4;

	static const short GLINT_MIN_DIAMETER = 2;
	static const short GLINT_MAX_DIAMETER = 6;
	// TODO calculate from diameter
	static const short GLINT_MIN_PIXEL = 1;
	// TODO calculate from diameter
	static const short GLINT_MAX_PIXEL = 50;

	// Min distance between two glints
	static const short GLINT_MIN_DISTANCE = 6;
	// Max distance between two glints
	static const short GLINT_MAX_DISTANCE = 25;

	static const short PUPIL_SEARCH_AREA_WIDHT_HEIGHT = 90;

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
