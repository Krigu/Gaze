/* 
 * File:   TrackingThread.cpp
 * Author: fri
 * 
 * Created on December 30, 2012, 6:44 PM
 */

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "TrackingWorker.hpp"
#include "calibration/Calibration.hpp"
#include "../threads/Sleeper.hpp"
#include "utils/geometry.hpp"
#include "threads/ThreadManager.hpp"

using std::cout;
using std::endl;

TrackingWorker::TrackingWorker(ImageSource *camera, QMutex *cameraLock) : camera(camera), cameraLock(cameraLock), running(false) {
}

TrackingWorker::~TrackingWorker() {
}

void TrackingWorker::track(Calibration calibration) {

    if(!cameraLock->tryLock()){
        emit error("Cannot track, is the camera in use?");
        return;
    }
    
    running = true;
    
    /*
    srand(time(NULL));

    int x, y;
    cout << "X: ";
    cin >> x;
    cout << "Y: ";
    cin >> y;

    unsigned long i = 0;
    while (i < 35 && running) {
        cout << "I'm Tracking! " << ++i << endl;
#ifdef __APPLE__
        // openCV on OSX does not block when capturing a frame. without this
        // we would emit 4000 frames a second and block the whole UI
        Sleeper::msleep(33);
#endif   
        Point p;
        p.x = normal(x, 40);
        p.y = normal(y, 30);
        emit estimatedPoint(p);
    }*/
    
    GazeTracker tracker(*camera, this);
    tracker.initializeCalibration();
    tracker.track();
    
    cameraLock->unlock();
    
    if(running)
        track(calibration);
    else
        emit hasStopped(nextStateAfterStop);
}

bool TrackingWorker::imageProcessed(Mat& resultImage) {
#ifdef __APPLE__
    // openCV on OSX does not block when capturing a frame. without this
    // we would emit 4000 frames a second and block the whole UI
    Sleeper::msleep(33);
#endif   
    emit cvImage(resultImage);
    
    return running;
}

bool TrackingWorker::imageProcessed(Mat &resultImage, MeasureResult &result, Point2f &gazeVector) {
#ifdef __APPLE__
    // openCV on OSX does not block when capturing a frame. without this
    // we would emit 4000 frames a second and block the whole UI
    Sleeper::msleep(33);
#endif   
    
    if(result == MEASURE_OK){
        emit estimatedPoint(calibration->calcCoordinates(gazeVector));
    }
    emit cvImage(resultImage);

    return running;
}

void TrackingWorker::stop(PROGRAM_STATES nextState) {
    this->nextStateAfterStop = nextState;
    this->running = false;
}