/* 
 * File:   IdleThread.cpp
 * Author: fri
 * 
 * Created on January 2, 2013, 5:33 PM
 */

#include "IdleWorker.hpp"
#include "video/LiveSource.hpp"
#include "../Sleeper.hpp"
#include "threads/ThreadManager.hpp"

IdleWorker::IdleWorker(ImageSource *camera, QMutex *cameraLock) : camera(camera), cameraLock(cameraLock), running(false) {
}

IdleWorker::~IdleWorker() {
}

/**
 * reads images directly from the camera and emits them to be displayed in 
 * the eye_widget. this method only works wit LiveSource (real webcams). 
 * when started with a VideoSource (movie files) an error is displayed.
 */
void IdleWorker::displayCamera(void){
    if(!cameraLock->tryLock()){
        emit error(tr("Idle-Thread: is the camera in use?"));
        return;
    }
    
    running = true;
    
    LiveSource *live = dynamic_cast<LiveSource*>(camera);
    if(live == 0) {
        cameraLock->unlock();
        emit error(tr("Idle-Thread: Sorry, I can only display images from real cameras."));
        return;
    }
    
    while(running){
        cv::Mat frame;
        if(live->nextGrayFrame(frame)){
#ifdef __APPLE__
            // openCV on OSX does not block when capturing a frame. without this
            // we would emit 4000 frames a second and block the whole UI
            Sleeper::msleep(33);
#endif   
            emit cvImage(new cv::Mat(frame));
        } else {
            emit error(tr("Could not read an image from camera"));
            break;
        }
    }
    
    cameraLock->unlock();
    
    // notify the other threads
    emit hasStopped(nextStateAfterStop);
}

void IdleWorker::stop(PROGRAM_STATES nextState) {
    this->nextStateAfterStop = nextState;
    this->running = false;
}