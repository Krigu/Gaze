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
#include "threads/ThreadManager.hpp"

using std::cout;
using std::endl;

TrackingThread::TrackingThread(ImageSource *camera, QMutex *cameraLock) : camera(camera), cameraLock(cameraLock), running(false) {
}

TrackingThread::~TrackingThread() {
}

void TrackingThread::track(Calibration calibration) {

    if(!cameraLock->tryLock()){
        emit error("Cannot track, is the camera in use?");
        return;
    }
    
    running = true;
    
    srand(time(NULL));

    int x, y;
    cout << "X: ";
    cin >> x;
    cout << "Y: ";
    cin >> y;

    unsigned long i = 0;
    while (i < 35 && running) {
        cout << "I'm Tracking! " << ++i << endl;
        Sleeper::msleep(500);
        Point p;
        p.x = normal(x, 40);
        p.y = normal(y, 30);
        emit estimatedPoint(p);
    }
    
    cameraLock->unlock();
    
    if(running)
        track(calibration);
    else
        emit hasStopped(nextStateAfterStop);
}

bool TrackingThread::imageProcessed(Mat& resultImage) {
    //TODO move the sleep into another (non-UI) thread?
    Sleeper::msleep(33);
    emit cvImage(resultImage);
    
    return running;
}

bool TrackingThread::imageProcessed(Mat &resultImage, MeasureResult &result, Point2f &gazeVector) {
    //TODO move the sleep into another (non-UI) thread?
    Sleeper::msleep(33);
    emit cvImage(resultImage);
   
    if(result == MEASURE_OK){
            //measurements.push_back(gazeVector);
    }

    std::cout << "Measured: " << result << " " << gazeVector << std::endl;
    
    return running;
}

void TrackingThread::stop(PROGRAM_STATES nextState) {
    this->nextStateAfterStop = nextState;
    this->running = false;
}