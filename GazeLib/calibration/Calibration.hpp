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

class CalibrationData {
private:
	cv::Point pointOnScreen;
	std::vector<cv::Point2f> measuredVectors;
	cv::Point2f averageVector;
public:
	CalibrationData(cv::Point point, std::vector<cv::Point2f> & vectors);
	cv::Point2f const& getAverageVector() const;
	cv::Point const& getPointOnScreen() const;
};

class Calibration {
private:
	// TODO remove display and don't show image
	cv::Mat display;
	cv::vector<CalibrationData> calibrationData;

	cv::Mat coefficientsX;
	cv::Mat coefficientsY;

public:
	Calibration();
	virtual ~Calibration();

	void calcCoefficients();
	void addCalibrationData(CalibrationData data);
	void printCalibration();
	cv::Point calculateCoordinates(cv::Point2f vector);

};

/* namespace std */
#endif /* CALIBRATION_HPP_ */
