#ifndef CALIBRATION_HPP_
#define CALIBRATION_HPP_

#include <vector>
#include <opencv2/core/core.hpp>

class Calibration;
class CalibrationVisualizer;

/**
 * 
 * The CalibrationData class represents a calibration point on the screen.
 * The idea is to provide an actual (displayed) point and a vector of 
 * measurements. A set of calibrationData-instances must be given to the 
 * calibration class.
 * 
 * The class takes the vector of the measurements and provides an method to ask
 * the point with the median distance of these measurements. 
 * 
 * 
 */
class CalibrationData {
    friend class Calibration;
private:
    cv::Point actualPoint;
    cv::Point2f medianVector;

protected:
    // distance between actualPoint and measured point
    int distance;
public:
    /**
     * Constructor
     * 
     * @param point Measured  The actual (displayed) point
     * @param vectors A list of gaze-vectors (measurements)
     */
    CalibrationData(cv::Point point, std::vector<cv::Point2f> & vectors);
    
    /**
     * Returns the point with the median distance.
     * 
     * @return 
     */
    cv::Point2f const& getMeasuredMedianVector() const;
    
    /**
     * Getter for the actual (displayed) point
     * 
     * @return  the actual point on the screen
     */
    cv::Point const& getActualPoint() const;
    
    /**
     * Get the distance beetween the actual point and the median distance
     * 
     * @return 
     */
    int getDistance() const;

    /**
     * Operation overload to allow the of the sort() funtion.
     */
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
     * removes the worst 3 calibratondata entries
     */
    void removeWorstCalibrationData();
public:
    /**
     * Default construtor
     */
    Calibration();

    /**
     * Default destructor
     */
    virtual ~Calibration();

    /**
     * Add a new measurement in form of a CalibrationData instance.
     * 
     * @param data Instance of CalibrationData
     */
    void addCalibrationData(CalibrationData data);

    /**
     *
     * Calculates the coefficients. After the 
     * coefficients are available, all calculated points
     * are compared with the actual points.
     *
     * If the method returns false, the calibration is not accurate enough 
     * and has to be repeated.
     * 
     * @param accuracyThreshold Maximum tolerated distance 
     * between the actual point the median of the measured points
     * 
     * @param maxExceedence Maximum allowed measurements with a larger 
     * gap than accuracyThreshold
     * 
     * @return true if the amount of measurements (calibrationData) with a gap 
     * larger than the accuracyThreshold does not exceed maxExceedence. 
     * False otherwise.
     *
     */
    bool calibrate(int accuracyThreshold, int maxExceedence);

    /**
     * Calculates the coordinates on the screen based on a 
     * gaze-vector.
     * 
     * Should only be called after the coefficients have been caluculated.
     * 
     * @param vector Current gaze-vector.
     * @return Estimated point on screen. 
     */
    cv::Point calcCoordinates(cv::Point2f vector);
};

/* namespace std */
#endif /* CALIBRATION_HPP_ */
