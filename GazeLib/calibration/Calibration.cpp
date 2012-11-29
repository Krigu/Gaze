/*
 * Calibration.cpp
 *
 *  Created on: Nov 29, 2012
 *      Author: krigu
 */

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "../utils/gui.hpp"

#include "Calibration.hpp"

using namespace std;
using namespace cv;

Calibration::Calibration() {

	namedWindow("Config");
	out = Mat::zeros(Size(1680, 1050), CV_8UC3);

	//Point center(720, 452);
	Point center(835, 524);
	calibrationPoints.push_back(center);
	//Point topLeft(86, 86);
	Point topLeft(100, 100);
	calibrationPoints.push_back(topLeft);
	//Point topRight(1360, 88);
	Point topRight(1577, 100);
	calibrationPoints.push_back(topRight);
	//Point bottomLeft(86, 815);
	Point bottomLeft(100, 945);
	calibrationPoints.push_back(bottomLeft);
	//Point bottomRight(1356, 815);
	Point bottomRight(1572, 945);
	calibrationPoints.push_back(bottomRight);

	for (vector<Point>::iterator it = calibrationPoints.begin();
			it != calibrationPoints.end(); ++it) {
		cross(out, (*it), 5, Scalar(255, 0, 0));
	}

	Point p1(6, 2); // center
	Point p2(-2, 4);  // top left
	Point p3(12, 5);  // top right
	Point p4(12, -1);  // bottom right
	Point p5(-3, -1);  // bottom left
	gazeVectors.push_back(p1); // center
	gazeVectors.push_back(p2);
	gazeVectors.push_back(p3);
	gazeVectors.push_back(p4);
	gazeVectors.push_back(p5);

}

void std::Calibration::createMatrix() {
	Mat vecX(gazeVectors.size(), 4, CV_32F);
	Mat vecY(gazeVectors.size(), 4, CV_32F);
	Mat pointsX(calibrationPoints.size(), 1, CV_32F);
	Mat pointsY(calibrationPoints.size(), 1, CV_32F);

	// Iterate over each calibration point and its measurements
	int i = 0;
	for (vector<Point>::iterator it = gazeVectors.begin();
			it != gazeVectors.end(); ++it) {

		float x = it->x;
		float y = it->y;

		// Formaula from "Eye Gaze Tracking under natural head movements"
		vecX.at<float>(i, 0) = 1;
		vecX.at<float>(i, 1) = x;
		vecX.at<float>(i, 2) = y;
		vecX.at<float>(i, 3) = x * y;
		//vectorsX.at<float>(i, 4) = x * x;
		//vectorsX.at<float>(i, 4) = y * y;

		vecY.at<float>(i, 0) = 1;
		vecY.at<float>(i, 1) = x;
		vecY.at<float>(i, 2) = y;
		vecY.at<float>(i, 3) = y * y;

		i++;
	}
	i = 0;
	for (vector<Point>::iterator it = calibrationPoints.begin();
			it != calibrationPoints.end(); ++it) {

		pointsX.at<float>(0, i) = calibrationPoints.at(i).x;
		pointsY.at<float>(0, i) = calibrationPoints.at(i).y;

		i++;
	}
	cout << "Gaze Vector: " << vecY << endl;
	cout << "Calib X: " << pointsY << endl;
	Mat outX;
	Mat outY;
	// src1 * dest = src2
	solve(vecX, pointsX, vectorsX, DECOMP_SVD);
	solve(vecY, pointsY, vectorsY, DECOMP_SVD);

	cout << "Output " << vectorsX << endl;

//	Point center(720, 452);
//	Point topLeft(86, 86);
//	Point topRight(1360, 88);
//	Point bottomLeft(86, 815);
//	Point bottomRight(1356, 815);

//	Point p1(-7, -1); // center
//	Point p2(2, -5);  // top left
//	Point p3(-14, -2);  // top right
//	Point p4(-13, 3);  // bottom right
//	Point p5(4, 2);  // bottom left

//	printPoint(outX, outY, center, p1);
//	printPoint(outX, outY, topLeft, p2);
//	printPoint(outX, outY, topRight, p3);
//	printPoint(outX, outY, bottomLeft, p4);
//	printPoint(outX, outY, bottomRight, p5);

}

Calibration::~Calibration() {

}

void std::Calibration::printPoint(cv::Point vector) {
	//out = Mat::zeros(Size(1680, 1050), CV_8UC3);

	float a0 = vectorsX.at<float>(0, 0);
	float a1 = vectorsX.at<float>(0, 1);
	float a2 = vectorsX.at<float>(0, 2);
	float a3 = vectorsX.at<float>(0, 3);

	float b0 = vectorsY.at<float>(0, 0);
	float b1 = vectorsY.at<float>(0, 1);
	float b2 = vectorsY.at<float>(0, 2);
	float b3 = vectorsY.at<float>(0, 4);

	float x = a0 + a1 * vector.x + a2 * vector.y + a3 * vector.x * vector.y;
	float y = b0 + b1 * vector.x + b2 * vector.y + b3 * vector.y * vector.y;

//	cout << "vector:" << vector.x << "/ " << vector.y << endl;
//	cout << "should be:" << p.x << "/ " << p.y << endl;
	cout << vectorsX << endl;
	cout << "GVec: " << vector.x << "y: " << vector.y << " Point x: " << x << " y: " << y << endl;

	Point calcPoint(x,1050 - y);
	cross(out, calcPoint, 5, Scalar(0, 255, 0));

	imshow("Config", out);
}

void std::Calibration::printBluePoint(cv::Point vector) {

	cout << "x: " << vector.x << " y: " << vector.y << endl;
	int x = vector.x;
	int y = 1050 - vector.y;
	Point p(x,y);
	cross(out, p, 5, Scalar(255, 255, 255));

	imshow("Config", out);
}
