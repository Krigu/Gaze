#ifndef STARBURST_HPP
#define	STARBURST_HPP

//#include "Algorithm.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "FindEyeRegion.hpp"

class Starburst {
private:
    //cv::Rect eye;
    //cv::Rect eye_region;
	cv::Point2f last_center;
    FindEyeRegion eyeReg;
    cv::Rect search_area;

public:
    void setUp(cv::VideoCapture &capture);
    void processImage(cv::Mat & image);
    void setLastCenter(cv::Point2f last_center);
    void tearDown();
    
private:
    void starburst(cv::Mat &image, cv::Point2f &center, float &radius,  int num_of_lines, int distance_growth);
    void remove_glints(cv::Mat &gray, cv::Rect &pupil_area, short num_of_glints, short interpolation_size);
};


class Ransac {
public:
    void ransac(float * x, float * y, float * radius, std::vector<cv::Point>);

private:
    void fitCircle(float * x, float * y, float * r, std::vector<cv::Point>);
};


#endif	/* STARBURST_HPP */

