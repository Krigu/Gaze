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

class Calibration;

class CalibrationData {
    friend class Calibration;
private:
	cv::Point actualPoint;
	cv::Point2f medianVector;
    
protected:   
        // distance between actualPoint and measured point
    int distance;
public:
	CalibrationData(cv::Point point, std::vector<cv::Point2f> & vectors);
	cv::Point2f const& getMeasuredMedianVector() const;
	cv::Point const& getActualPoint() const;
    int const getDistance() const;
    
    friend bool operator<(const CalibrationData& d1, const CalibrationData& d2);
};



class Calibration {
private:
	// TODO remove display and don't show image
	cv::Mat display;
	cv::vector<CalibrationData> calibrationData;

	cv::Mat coefficientsX;
	cv::Mat coefficientsY;
    cv::RNG rng;


public:
	Calibration();
	virtual ~Calibration();

	void calcCoefficients();
    
	void addCalibrationData(CalibrationData data);  
    cv::Point calculateCoordinates(cv::Point2f vector);
    void calcCalibrationDataDistance();
    int calcAverageDeviation();
    
	void printCalibration(std::vector<cv::Point2f> points);    
    void printCalibration();

	

};

/* namespace std */
#endif /* CALIBRATION_HPP_ */
