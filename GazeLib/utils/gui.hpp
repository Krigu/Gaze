/*
 * gui.hpp
 *
 *  Created on: Nov 9, 2012
 *      Author: krigu
 */

#ifndef GUI_HPP_
#define GUI_HPP_

#include "opencv2/core/core.hpp"

// creates a rectangle and checks whether it is within the frames boundaries
cv::Rect createRect(cv::Mat &frame, cv::Point &position, int width, int height);
void cross(cv::Mat & im, cv::Point &center, short size);
void cross(cv::Mat & im, cv::Point &center, short size, cv::Scalar color);

#endif /* GUI_HPP_ */

