/* 
 * File:   fitCircle.hpp
 * Author: fri
 *
 * Created on December 16, 2012, 2:32 PM
 */

#ifndef BESTFITCIRCLE_HPP
#define	BESTFITCIRCLE_HPP

#include <vector>
#include <opencv2/core/core.hpp>


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


#endif	/* BESTFITCIRCLE_HPP */

