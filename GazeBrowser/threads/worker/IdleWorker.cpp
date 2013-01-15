#include "IdleWorker.hpp"
#include "video/LiveSource.hpp"
#include "../Sleeper.hpp"
#include "threads/ThreadManager.hpp"

IdleWorker::IdleWorker(ImageSource *camera, QMutex *cameraLock) : camera(camera), cameraLock(cameraLock), running(false) {
}

IdleWorker::~IdleWorker() {
}

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
//#ifdef __APPLE__
//        Sleeper::msleep(33);
//#endif
        if(live->nextGrayFrame(frame)){  
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