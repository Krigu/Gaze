#include <iostream>

#include "../utils/gui.hpp"
#include "../utils/log.hpp"
#include "../utils/geometry.hpp"
#include "../exception/GazeExceptions.hpp"
#include "Calibration.hpp"

using namespace std;
using namespace cv;

CalibrationData::CalibrationData(cv::Point point,
        std::vector<cv::Point2f> & vectors) :
actualPoint(point), distance(-1) {

    medianVector = calcMedianPoint(point, vectors);
    //averageVector = calcAverage(vectors);
}

const cv::Point2f& CalibrationData::getMeasuredMedianVector() const {
    return medianVector;
}

const cv::Point& CalibrationData::getActualPoint() const {
    return actualPoint;
}

int CalibrationData::getDistance() const {
    return distance;
}

/**
 * < Operator for using the sort algorithm
 */
bool operator<(const CalibrationData& d1, const CalibrationData& d2) {
    return d1.distance < d2.distance;
}

Calibration::Calibration() {

}

Calibration::~Calibration() {

}

void Calibration::calcCoefficients() {
    int matSize = calibrationData.size();

    if (matSize == 0) {
        throw WrongArgumentException("Cannot calculate coeffizients with matSize=0!");
    }

    Mat measurementsX(matSize, 4, CV_32F);
    Mat measurementsY(matSize, 4, CV_32F);
    Mat calibrationPointX(matSize, 1, CV_32F);
    Mat calibrationPointY(matSize, 1, CV_32F);

    // Iterate over all calibration data
    int i = 0;
    for (vector<CalibrationData>::iterator it = calibrationData.begin();
            it != calibrationData.end(); ++it) {

        float x = it->getMeasuredMedianVector().x;
        float y = it->getMeasuredMedianVector().y;

        // Formaula from "Eye Gaze Tracking under natural head movements"
        measurementsX.at<float>(i, 0) = 1;
        measurementsX.at<float>(i, 1) = x;
        measurementsX.at<float>(i, 2) = y;
        measurementsX.at<float>(i, 3) = x * y;

        measurementsY.at<float>(i, 0) = 1;
        measurementsY.at<float>(i, 1) = x;
        measurementsY.at<float>(i, 2) = y;
        measurementsY.at<float>(i, 3) = y * y;

        calibrationPointX.at<float>(0, i) = it->getActualPoint().x;
        calibrationPointY.at<float>(0, i) = it->getActualPoint().y;

        i++;
    }

    solve(measurementsX, calibrationPointX, coefficientsX, DECOMP_SVD);
    solve(measurementsY, calibrationPointY, coefficientsY, DECOMP_SVD);


}

void Calibration::addCalibrationData(CalibrationData data) {
    calibrationData.push_back(data);
}

cv::Point Calibration::calcCoordinates(cv::Point2f vector) {
    float a0 = coefficientsX.at<float>(0, 0);
    float a1 = coefficientsX.at<float>(0, 1);
    float a2 = coefficientsX.at<float>(0, 2);
    float a3 = coefficientsX.at<float>(0, 3);

    float b0 = coefficientsY.at<float>(0, 0);
    float b1 = coefficientsY.at<float>(0, 1);
    float b2 = coefficientsY.at<float>(0, 2);
    float b3 = coefficientsY.at<float>(0, 3);

    int x = a0 + a1 * vector.x + a2 * vector.y + a3 * vector.x * vector.y;
    int y = b0 + b1 * vector.x + b2 * vector.y + b3 * vector.y * vector.y;

    return Point(x, y);
}

void Calibration::calcCalibrationDataDistance() {
    // Iterate over all calibrationData
    for (vector<CalibrationData>::iterator it = calibrationData.begin();
            it != calibrationData.end(); ++it) {

        // calulated point
        Point calculatedPoint = calcCoordinates(it->getMeasuredMedianVector());
        Point actualPoint = it->getActualPoint();
        int distance = calcPointDistance(&actualPoint, &calculatedPoint);
        it->distance = distance;
    }
}

int Calibration::calcAverageDeviation() {

    int totalDistance = 0;

    // Iterate over all calibrationData
    for (vector<CalibrationData>::iterator it = calibrationData.begin();
            it != calibrationData.end(); ++it) {

        totalDistance += it->getDistance();
    }

    return totalDistance;
}

void Calibration::removeWorstCalibrationData() {

    sort(calibrationData.begin(), calibrationData.end());
    calibrationData.erase(calibrationData.end() - 3, calibrationData.end());
}

bool Calibration::calibrate(int accuracyThreshold, int maxExceedence) {


    
    // Calculate the coordinate
    calcCoefficients();
    LOG_D(" Calibration actual point: " << calibrationData.at(0).getActualPoint()
            << " Measured point " << calcCoordinates(calibrationData.at(0).getMeasuredMedianVector()));
    calcCalibrationDataDistance();
    LOG_D("Average deviation: " << calcAverageDeviation());
    
    int errors = 0;
    // Check how many distances are greater than threshold and remove them   
    vector<CalibrationData>::iterator it;
    for (it = calibrationData.begin(); it != calibrationData.end();) {
        if (it->getDistance() > accuracyThreshold) {
            it = calibrationData.erase(it);
            errors++;
        } else
            ++it;
    }

    // If the calibration process is not accurate enough, false will be returned
    if (errors > maxExceedence)
        return false;

    // Otherwise consider only those measurements smaller than threshold
    calcCoefficients();
    LOG_D(" Calibration actual point: " << calibrationData.at(0).getActualPoint()
            << " Measured point " << calcCoordinates(calibrationData.at(0).getMeasuredMedianVector()));
    

    return true;
}