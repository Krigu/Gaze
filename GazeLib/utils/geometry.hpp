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

/**
 * this function tries to find the circle that fits best (least squares)
 * to the given points. the code is based on our coaches matlab code fitCircle.m
 * 
 * @param x the x coordinate
 * @param y the y coordinate
 * @param radius the new radius
 * @param pointsToFit the points to which a circle should be fit
 */
void bestFitCircle(float * x, float * y, float * radius,
		std::vector<cv::Point2f> pointsToFit);

#endif /* GEOMETRY_HPP_ */
