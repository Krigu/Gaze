#ifndef GUI_HPP_
#define GUI_HPP_

#include "opencv2/core/core.hpp"

/*
 * creates a rectangle and makes sure that this rect lies within the given image.
 * if the rectangle would exceed the image it will be clipped to the boundaries.
 * 
 * @param frame the image where the rect will be placed in
 * @param position the upper left corner of the rect
 * @param width the rects width
 * @param height the rects height
 * @return a rect that can be placed within the given image
 */
cv::Rect createRect(cv::Mat &frame, cv::Point &position, int width, int height);


/**
 * draws a white cross in the image
 * @param im the image to draw at
 * @param center the center of the cross
 * @param size the length of the crosses lines
 */
void cross(cv::Mat & im, cv::Point &center, short size);

/**
 * draws a cross in the image with the given color
 * <br /> the callee has to make sure that the cv::Mat supports colors when 
 * choosing a color != white or black
 * @param im the image to draw at
 * @param center the center of the cross
 * @param size the length of the crosses lines
 * @param color the color to draw the image
 */
void cross(cv::Mat & im, cv::Point &center, short size, cv::Scalar color);

/// overloaded cross function for Point2f
void cross(cv::Mat & im, cv::Point2f &center, short size);

/// overloaded cross function for Point2f
void cross(cv::Mat & im, cv::Point2f &center, short size, cv::Scalar color);
#endif /* GUI_HPP_ */

