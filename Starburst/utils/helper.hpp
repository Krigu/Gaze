#ifndef HELPER_HPP
#define	HELPER_HPP

#include "opencv2/core/core.hpp"

//TODO: refactor this!

// creates a rectangle and checks whether it is within the frames boundaries
cv::Rect createRect(cv::Mat &frame, cv::Point &position, int width, int height);
void cross(cv::Mat & im, cv::Point &center, short size);
void cross(cv::Mat & im, cv::Point &center, short size, cv::Scalar color);

class Ransac {
public:
    void ransac(float * x, float * y, float * radius, std::vector<cv::Point>);
    
private:
    void fitCircle(float * x, float * y, float * r, std::vector<cv::Point>);
};

#endif	/* HELPER_HPP */
