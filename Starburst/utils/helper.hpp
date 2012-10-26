#ifndef HELPER_HPP
#define	HELPER_HPP

#include <iostream>

// adds a macro "LOG_D('your message') and LOG_W('your message')"
// the log_d is only printed if you add the __DEBUG Symbol to eclipses "Symbol and Paths"
#define LOG_W(msg)(std::cout << __FILE__ << "(" << __LINE__  << "): "<< msg << std::endl)
#ifdef __DEBUG
#define LOG_D(msg) LOG_W(msg)
#else
#define LOG_D(msg) // DO NOTHING
#endif


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
