/*
 * geometry.cpp
 *
 *  Created on: Nov 9, 2012
 *      Author: krigu
 */

#include "geometry.hpp"

int calcPointDistance(cv::Point point1, cv::Point point2) {
	int distX = point1.x - point2.x;
	int distY = point1.y - point2.y;
	int sum = distX * distX + distY * distY;

	// Use Pythagoras to calc distance between two points
	return sqrt(sum);
}

cv::Point calcRectBarycenter(cv::Rect& rect) {
	int x = rect.width / 2;
	int y = rect.height / 2;

	return cv::Point(rect.x + x, rect.y + y);
}
