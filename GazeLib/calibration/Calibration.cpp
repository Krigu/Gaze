/*
 * Calibration.cpp
 *
 *  Created on: Nov 29, 2012
 *      Author: krigu
 */

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "../utils/gui.hpp"
#include "../utils/geometry.hpp"
#include "../exception/GazeExceptions.hpp"
#include "Calibration.hpp"

using namespace std;
using namespace cv;

CalibrationData::CalibrationData(cv::Point point,
        std::vector<cv::Point2f> & vectors) :
actualPoint(point), distance(0) {

    medianVector = calcMedianPoint(point, vectors);
    //averageVector = calcAverage(vectors);
}

const cv::Point2f& CalibrationData::getMeasuredMedianVector() const {
    return medianVector;
}

const cv::Point& CalibrationData::getActualPoint() const {
    return actualPoint;
}

const int CalibrationData::getDistance() const {
    return distance;
}

/**
 * < Operator for using the sort algorithm
 */
bool operator<(const CalibrationData& d1, const CalibrationData& d2) {
    return d1.distance < d2.distance;
}

Calibration::Calibration() : rng(RNG(12345)) {


}

Calibration::~Calibration() {

}

void Calibration::calcCoefficients() {
    int matSize = calibrationData.size();

    if (matSize == 0) {
        throw WrongArgumentException("Cannot calculate Coeffizients with matSize=0!");
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

void Calibration::printCalibration(std::vector<cv::Point2f> points) {

    Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
            255);


    for (vector<cv::Point2f>::iterator it = points.begin();
            it != points.end(); ++it) {

        // calulated point
        Point calcPoint = calculateCoordinates(*it);
        calcPoint.x /= 2;
        calcPoint.y /= 2;
        cross(display, calcPoint, 5, color);
    }

    imshow("Calibration", display);

}

void Calibration::printCalibration() {

    display = Mat::zeros(Size(720, 450), CV_8UC3);

    for (vector<CalibrationData>::iterator it = calibrationData.begin();
            it != calibrationData.end(); ++it) {

        // calulated point
        Point calcPoint = calculateCoordinates(it->getMeasuredMedianVector());
        calcPoint.x /= 2;
        calcPoint.y /= 2;
        cout << "Calculated Point: " << calcPoint << endl;
        cross(display, calcPoint, 5, Scalar(0, 0, 255));
        Point actualPoint = it->getActualPoint();
        actualPoint.x = actualPoint.x / 2;
        actualPoint.y = actualPoint.y / 2;
        cout << "Actual Point: " << actualPoint << endl;
        cross(display, actualPoint, 5, Scalar(0, 255, 0));
    }

    imshow("Calibration", display);

}

cv::Point Calibration::calculateCoordinates(cv::Point2f vector) {
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
        Point calculatedPoint = calculateCoordinates(it->getMeasuredMedianVector());
        Point actualPoint = it->getActualPoint();
        int distance = calcPointDistance(&actualPoint, &calculatedPoint);
        it->distance = distance;
    }
}

int Calibration::calcAverageDeviation() {

    int totalDistance = 0;
    
    sort(calibrationData.begin(), calibrationData.end());

    // Iterate over all calibrationData
    for (vector<CalibrationData>::iterator it = calibrationData.begin();
            it != calibrationData.end(); ++it) {

        totalDistance += it->getDistance();
        cout << "Distance " << it->getDistance() << endl;
    }

    cout << "Average distance: " << totalDistance / calibrationData.size() << endl;

    return totalDistance;
}