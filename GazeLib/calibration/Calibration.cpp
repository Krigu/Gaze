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
		pointOnScreen(point), measuredVectors(vectors) {

	averageVector = calcAverage(measuredVectors);
}

const cv::Point2f& CalibrationData::getAverageVector() const {
	return averageVector;
}

const cv::Point& CalibrationData::getPointOnScreen() const {
	return pointOnScreen;
}

Calibration::Calibration() {

//	//Point center(720, 452);
//	Point center(835, 524);
//	calibrationPoints.push_back(center);
//	//Point topLeft(86, 86);
//	Point topLeft(100, 100);
//	calibrationPoints.push_back(topLeft);
//	//Point topRight(1360, 88);
//	Point topRight(1577, 100);
//	calibrationPoints.push_back(topRight);
//	//Point bottomLeft(86, 815);
//	Point bottomLeft(100, 945);
//	calibrationPoints.push_back(bottomLeft);
//	//Point bottomRight(1356, 815);
//	Point bottomRight(1572, 945);
//	calibrationPoints.push_back(bottomRight);
//
//	for (vector<Point>::iterator it = calibrationPoints.begin();
//			it != calibrationPoints.end(); ++it) {
//		cross(out, (*it), 5, Scalar(255, 0, 0));
//	}
//
//	Point p1(6, 2); // center
//	Point p2(-2, 4);  // top left
//	Point p3(12, 5);  // top right
//	Point p4(12, -1);  // bottom right
//	Point p5(-3, -1);  // bottom left
//	gazeVectors.push_back(p1); // center
//	gazeVectors.push_back(p2);
//	gazeVectors.push_back(p3);
//	gazeVectors.push_back(p4);
//	gazeVectors.push_back(p5);

}

Calibration::~Calibration() {

}

void Calibration::calcCoefficients() {
	int matSize = calibrationData.size();

	if (matSize == 0) {
		throw new WrongArgumentException("Cannot calculate Coeffizients with matSize=0!");
	}

	Mat measurementsX(matSize, 4, CV_32F);
	Mat measurementsY(matSize, 4, CV_32F);
	Mat calibrationPointX(matSize, 1, CV_32F);
	Mat calibrationPointY(matSize, 1, CV_32F);

	// Iterate over all calibration data
	int i = 0;
	for (vector<CalibrationData>::iterator it = calibrationData.begin();
			it != calibrationData.end(); ++it) {

		float x = it->getPointOnScreen().x;
		float y = it->getPointOnScreen().y;

		// Formaula from "Eye Gaze Tracking under natural head movements"
		measurementsX.at<float>(i, 0) = 1;
		measurementsX.at<float>(i, 1) = x;
		measurementsX.at<float>(i, 2) = y;
		measurementsX.at<float>(i, 3) = x * y;

		measurementsY.at<float>(i, 0) = 1;
		measurementsY.at<float>(i, 1) = x;
		measurementsY.at<float>(i, 2) = y;
		measurementsY.at<float>(i, 3) = y * y;

		calibrationPointX.at<float>(0, i) = it->getPointOnScreen().x;
		calibrationPointY.at<float>(0, i) = it->getPointOnScreen().y;

		i++;
	}

	solve(measurementsX, calibrationPointX, coefficientsX, DECOMP_SVD);
	solve(measurementsY, calibrationPointY, coefficientsY, DECOMP_SVD);
}

void Calibration::addCalibrationData(CalibrationData data) {
	calibrationData.push_back(data);
}

void Calibration::printCalibration() {
	display = Mat::zeros(Size(1680, 1050), CV_8UC3);
	for (vector<CalibrationData>::iterator it = calibrationData.begin();
			it != calibrationData.end(); ++it) {

		// calulated point
		Point calcPoint = calculateCoordinates(it->getAverageVector());
        cout << "Calculated Point: " << calcPoint << endl;
		cross(display, calcPoint, 5, Scalar(0, 0, 255));
		Point actualPoint = it->getPointOnScreen();
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
