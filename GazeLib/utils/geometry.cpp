/*
 * geometry.cpp
 *
 *  Created on: Nov 9, 2012
 *      Author: krigu
 */

#include "geometry.hpp"

int calcPointDistance(cv::Point point1, cv::Point point2) {
	// Use Pythagoras to calc distance between two points
	return sqrt((point1.x - point2.x) ^ 2 + (point1.y - point2.y) ^ 2);
}
