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
class CalibrationVisualizer;

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
    int getDistance() const;

    friend bool operator<(const CalibrationData& d1, const CalibrationData& d2);
};

class Calibration {
    friend class CalibrationVisualizer; // For debugging and visualizing
private:   

protected:
    cv::Mat coefficientsX;
    cv::Mat coefficientsY;    
    cv::vector<CalibrationData> calibrationData;
    
    void calcCoefficients(int accuracyThreshold = 0);
    void calcCalibrationDataDistance();
    int calcAverageDeviation();
    void removeWorstCalibrationData();
public:
    Calibration();
    virtual ~Calibration();

    void addCalibrationData(CalibrationData data);
    bool calibrate(int accuracyThreshold, int maxExceedence);

    cv::Point calcCoordinates(cv::Point2f vector);
};

/* namespace std */
#endif /* CALIBRATION_HPP_ */
