#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_

#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define PI 3.14159265358979323846
#define PI2 6.28318530717958647692

/**
 * Calculates the absolute distance between to points
 */
int calcPointDistance(cv::Point *point1, cv::Point *point2);

/**
 * Calculates the absolute distance between to float points
 */
float calcPoint2fDistance(cv::Point2f point1, cv::Point2f point2);

/**
 * Calculates the barycenter of a Rect
 */
cv::Point calcRectBarycenter(cv::Rect& rect);

/**
 * calculates the angle between two points
 */
double calcAngle(cv::Point start, cv::Point end);

/**
 * calculates the average point of list of points. <br /> the average is 
 * calculated using x=sum_of_all_x/num_of_points and y=sum_of_all_y/num_of_points.
 * @param points
 * @return 
 */
cv::Point2f calcAverage(std::vector<cv::Point2f> points);

/**
 * Finds the point in a list of points who lies on the median distance to a
 * given reference points.
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
 * checks whether the given four points build a rectangle
 * @param points a list of points
 * @param tolerance ratio for accepting rectangles. lower parameters accept 
 *  only better rectangles
 * @return true if the given points build a rectangle
 */
bool isRectangle(std::vector< cv::Point > points, int tolerance);

/**
 * sort the given list of points for building a rectangle CCW.
 * 
 * @param points
 */
void orientateFourPoints(std::vector< cv::Point >& points );

/**
 * checks whether a given point lies within the given rect.
 * @param p
 * @param rect
 * @return true if the points coordinates lie within rect
 */
bool isPointInRect(cv::Point& p, cv::Rect& rect);

/**
 * calculates the parameter for building a normal distribution
 * @param mean
 * @param stdev
 * @return 
 */
int normal(int mean, int stdev);

#endif /* GEOMETRY_HPP_ */
