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

using std::cout;
using std::endl;

TrackingThread::TrackingThread(Calibration *calibData) : calibData(calibData) {
}

TrackingThread::~TrackingThread() {
}

void TrackingThread::testRun()
{
    unsigned long i=0;
    while(true){
        cout << "I'm Tracking! " << ++i << endl;
        Sleeper::msleep(2000);
        Point p;
        p.x = -5;
        p.y = 5;
        emit estimatedPoint(p);
    }
}

void TrackingThread::imageProcessed(Mat& resultImage){

}
void TrackingThread::imageProcessed(Mat &resultImage, MeasureResult &result, Point2f &gazeVector){

}
    