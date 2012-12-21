/* 
 * File:   Ransac.hpp
 * Author: fri
 *
 */

#ifndef RANSAC_HPP
#define	RANSAC_HPP

#include <vector>
#include <opencv2/core/core.hpp>

class Ransac {
public:
	bool ransac(float * x, float * y, float * radius, std::vector<cv::Point2f>);

private:
	void fitCircle(float * x, float * y, float * r, std::vector<cv::Point2f>);
};


#endif	/* RANSAC_HPP */

