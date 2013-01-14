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

/**
 * The Calibration class provides methods to calculate the Gaze Tracking calibration.
 * This class must be used to initially calibrate the Gaze Tracking System. After the 
 * calibration a call to calcCoordinates() will calculate the point on the screen 
 * where a user is looking at.
 */
class Calibration {
    friend class CalibrationVisualizer; // For debugging and visualizing
private:   

protected:
    cv::Mat coefficientsX;
    cv::Mat coefficientsY;    
    cv::vector<CalibrationData> calibrationData;
    
    /**
     * solves the equation system for the calibration coefficients
     */
    void calcCoefficients();
    
    /**
     * calculates the distance between each calibration-point (should) 
     * and the measured point (is)
     */
    void calcCalibrationDataDistance();
    
    /**
     * calculats the average deviation of all calibration data members
     */
    int calcAverageDeviation();
    
    /**
     * removes the worst calibratondata entry
     */
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
