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
 * Calculate to absolute distance between to points
 */
float calcPoint2fDistance(cv::Point2f point1, cv::Point2f point2);

/**
 * Calculates the barycenter of a Rect
 */
cv::Point calcRectBarycenter(cv::Rect& rect);

double calcAngle(cv::Point start, cv::Point end);

cv::Point2f calcAverage(std::vector<cv::Point2f> points);

/**
 * Calculates the median distance between a reference point and a vector of points
 * 
 * @param referencePoint reference point
 * @param scores vector of points
 * @return median point
 */
cv::Point2f calcMedianPoint(cv::Point2f reference, std::vector< cv::Point2f > scores);

/**
 * 
 * @param points
 * @param tolerance
 * @return 
 */
bool isRectangle(std::vector< cv::Point > points, int tolerance);


#endif /* GEOMETRY_HPP_ */
