/* 
 * File:   IdleThread.cpp
 * Author: fri
 * 
 * Created on January 2, 2013, 5:33 PM
 */

#include "IdleThread.hpp"
#include "video/LiveSource.hpp"
#include "../Sleeper.hpp"

IdleThread::IdleThread(ImageSource *camera, QMutex *cameraLock) : camera(camera), cameraLock(cameraLock) {
}

IdleThread::~IdleThread() {
}

/**
 * reads images directly from the camera and emits them to be displayed in 
 * the eye_widget. this method only works wit LiveSource (real webcams). 
 * when started with a VideoSource (movie files) an error is displayed.
 */
void IdleThread::displayCamera(void){
    if(!cameraLock->tryLock()){
        emit error("Idle-Thread: is the camera in use?");
        return;
    }
    
    LiveSource *live = dynamic_cast<LiveSource*>(camera);
    if(live == 0) {
        cameraLock->unlock();
        emit error("Idle-Thread: Sorry, I can only display images from real cameras.");
        return;
    }
    
    while(true){
        cv::Mat frame;
        if(live->nextGrayFrame(frame)){
            Sleeper::msleep(33);
            emit cvImage(frame);
        } else {
            emit error("Could not read an image from camera");
            break;
        }
    }
    
    cameraLock->unlock();
}