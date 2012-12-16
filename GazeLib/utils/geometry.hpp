/*
 * geometry.hpp
 *
 *  Created on: Nov 9, 2012
 *      Author: krigu
 */

#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_

#include <vector>

#include "opencv2/core/core.hpp"

/**
 * Calculate to absolute distance between to points
 */
int calcPointDistance(cv::Point *point1, cv::Point *point2);

/**
 * Calculates the barycenter of a Rect
 */
cv::Point calcRectBarycenter(cv::Rect& rect);

double calcAngle(cv::Point start, cv::Point end);

cv::Point2f calcAverage(std::vector<cv::Point2f> points);

bool isRectangle(cv::Point p1, cv::Point p2, cv::Point p3, cv::Point p4, int tolerance);


#endif /* GEOMETRY_HPP_ */
