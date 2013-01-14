#include <iostream>

#include "../utils/gui.hpp"
#include "../utils/geometry.hpp"

#include "CalibrationVisualizer.hpp"

using namespace std;
using namespace cv;

CalibrationVisualizer::CalibrationVisualizer(Calibration* calibration) : calib(calibration), rng(RNG(12345)) {
}

CalibrationVisualizer::~CalibrationVisualizer() {
}

void CalibrationVisualizer::printCalibration(std::vector<cv::Point2f> points) {

    Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
            255);


    for (vector<cv::Point2f>::iterator it = points.begin();
            it != points.end(); ++it) {

        // calulated point
        Point calcPoint = calib->calcCoordinates(*it);
        cross(display, calcPoint, 5, color);
    }

    imshow("Calibration", display);

}

void CalibrationVisualizer::printCalibration() {

    // TODO: Give resolution as parameter
    display = Mat::zeros(Size(1440, 900), CV_8UC3);

    vector<CalibrationData> calibrationData = calib->calibrationData;

    for (vector<CalibrationData>::iterator it = calibrationData.begin();
            it != calibrationData.end(); ++it) {

        // calulated point
        Point calcPoint = calib->calcCoordinates(it->getMeasuredMedianVector());
        cout << "Calculated Point: " << calcPoint << endl;
        cross(display, calcPoint, 5, Scalar(0, 0, 255));
        Point actualPoint = it->getActualPoint();
        cout << "Actual Point: " << actualPoint << endl;
        cross(display, actualPoint, 5, Scalar(0, 255, 0));
    }

    imshow("Calibration", display);

}

void CalibrationVisualizer::printCoefficients() {
    cout << "X: " << endl << calib->coefficientsX << endl;
    cout << "Y: " << endl << calib->coefficientsY << endl;

}

