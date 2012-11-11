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
	// Amounts of glints (per eye)
	static const char GLINT_COUNT = 4;

	static const short GLINT_MIN_DIAMETER = 2;
	static const short GLINT_MAX_DIAMETER = 6;

	// Min distance between two glints
	static const short GLINT_MIN_DISTANCE = 6;
	// Max distance between two glints
	static const short GLINT_MAX_DISTANCE = 25;

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
