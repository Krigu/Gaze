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
#include "opencv2/imgproc/imgproc.hpp"

#define PI 3.14159265358979323846
#define PI2 6.28318530717958647692

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

/**
 * 
 * @param points
 * @param tolerance
 * @return 
 */
bool isRectangle(std::vector< cv::Point > points, int tolerance);

void orientateFourPoints(std::vector< cv::Point >& points );

bool isPointInRect(cv::Point& p, cv::Rect& rect);

int normal(int mean, int stdev);

#endif /* GEOMETRY_HPP_ */
