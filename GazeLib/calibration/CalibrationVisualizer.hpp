#ifndef CALIBRATIONVISUALIZER_HPP
#define	CALIBRATIONVISUALIZER_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../calibration/Calibration.hpp"

/**
 * Visualizes the calibration. This class is mostly used
 * for debugging.
 * 
 */
class CalibrationVisualizer {
public:
    /**
     * 
     * @param calibration Pointer to a calibration
     * 
     */
    CalibrationVisualizer(Calibration* calibration);
    virtual ~CalibrationVisualizer();

    /**
     * Calculates the screen-position of a list of gaze-vectors.
     * 
     * @param points List of gaze-vectors to be displayed as coordinates
     */
    void printCalibration(std::vector<cv::Point2f> points);
   
    /**
     * Prints the actual and the calibrated point to the console.
     * Also displays an image with the actual point and the median 
     * points of each calibrationData entry.
     */
    void printCalibration();
    
    /**
     * Prints the calculated coefficients to the console
     */
    void printCoefficients();
private:
    Calibration* calib;
    cv::RNG rng;
    cv::Mat display;

};

#endif	/* CALIBRATIONVISUALIZER_HPP */

