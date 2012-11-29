/*
 * Calibration.hpp
 *
 *  Created on: Nov 29, 2012
 *      Author: krigu
 */

#ifndef CALIBRATION_HPP_
#define CALIBRATION_HPP_

#include <vector>
#include <opencv2/core/core.hpp>

namespace std {

class Calibration {
private:
	cv::Mat out;
	cv::Mat vectorsX;
	cv::Mat vectorsY;
	vector<cv::Point> calibrationPoints;
	vector<cv::Point> gazeVectors;

public:
	Calibration();
	void createMatrix();
	void printPoint(cv::Point vector);
	void printBluePoint(cv::Point vector);
	virtual ~Calibration();
};

} /* namespace std */
#endif /* CALIBRATION_HPP_ */
