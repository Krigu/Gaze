#include "gui.hpp"

cv::Rect createRect(cv::Mat &frame, cv::Point &position, int width,
		int height) {

	// make sure, that the rectangle stays within the image boundaries
	position.x = position.x >= 0 ? position.x : 0;
	position.y = position.y >= 0 ? position.y : 0;
	width = position.x + width > frame.cols ? frame.cols - position.x : width;
	width = position.y + height > frame.rows ? frame.rows - position.y : height;

	return cv::Rect(position.x, position.y, width, height);
}

void cross(cv::Mat & im, cv::Point &center, short size, cv::Scalar color) {
	cv::line(im, cv::Point(center.x - size, center.y),
			cv::Point(center.x + size, center.y), color);
	cv::line(im, cv::Point(center.x, center.y - size),
			cv::Point(center.x, center.y + size), color);
}

void cross(cv::Mat & im, cv::Point &center, short size) {
	cross(im, center, size, cv::Scalar(255, 255, 255));
}

void cross(cv::Mat & im, cv::Point2f &center, short size) {
	cv::Point p(center.x,center.y);
	cross(im, p, size, cv::Scalar(255, 255, 255));
}

void cross(cv::Mat & im, cv::Point2f &center, short size, cv::Scalar color) {
	cv::Point p(center.x,center.y);
	cross(im, p, size, color);
}
