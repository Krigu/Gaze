/* 
 * File:   TrackingThread.cpp
 * Author: fri
 * 
 * Created on December 30, 2012, 6:44 PM
 */

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "TrackingThread.hpp"
#include "calibration/Calibration.hpp"
#include "../threads/Sleeper.hpp"
#include "utils/geometry.hpp"

using std::cout;
using std::endl;

TrackingThread::TrackingThread(Calibration *calibData) : calibData(calibData) {
}

TrackingThread::~TrackingThread() {
}

void TrackingThread::testRun() {

    srand(time(NULL));

    int x, y;
    cout << "X: ";
    cin >> x;
    cout << "Y: ";
    cin >> y;

    unsigned long i = 0;
    while (i < 35) {
        cout << "I'm Tracking! " << ++i << endl;
        Sleeper::msleep(500);
        Point p;
        p.x = normal(x, 40);
        p.y = normal(y, 30);
        emit estimatedPoint(p);
    }
    
    testRun();
}

void TrackingThread::imageProcessed(Mat& resultImage) {

}

void TrackingThread::imageProcessed(Mat &resultImage, MeasureResult &result, Point2f &gazeVector) {

}
