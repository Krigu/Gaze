#ifndef CALIBRATIONVISUALIZER_HPP
#define	CALIBRATIONVISUALIZER_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../calibration/Calibration.hpp"

class CalibrationVisualizer {
public:
    CalibrationVisualizer(Calibration* calibration);
    CalibrationVisualizer(const CalibrationVisualizer& orig);
    virtual ~CalibrationVisualizer();

    void printCalibration(std::vector<cv::Point2f> points);
    void printCalibration();
    void printCoefficients();
private:
    Calibration* calib;
    cv::RNG rng;
    // TODO remove display and don't show image
    cv::Mat display;

};

#endif	/* CALIBRATIONVISUALIZER_HPP */

