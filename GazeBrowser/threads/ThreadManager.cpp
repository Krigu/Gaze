/* 
 * File:   ThreadManager.cpp
 * Author: fri
 * 
 * Created on January 2, 2013, 11:45 AM
 */

#include <opencv2/core/core.hpp>

#include <iostream>

#include "ThreadManager.hpp"
#include "calibration/CalibrationThread.hpp"
#include "tracking/TrackingThread.hpp"
#include "../ui/BrowserWindow.hpp"
#include "calibration/Calibration.hpp"
#include "idle/IdleThread.hpp"
#include "actions/ActionManager.hpp"

ThreadManager::ThreadManager(BrowserWindow *parent) : parent(parent), state(ST_STARTED_UP){
    
    // register the OpenCV datatypes for emitting them afterwards
    qRegisterMetaType< cv::Mat > ("cv::Mat");
    qRegisterMetaType< cv::Point > ("cv::Point");
    qRegisterMetaType< Calibration >("Calibration");
    qRegisterMetaType< PROGRAM_EVENTS >("PROGRAM_EVENTS");
    
    trackingThread = new QThread;
    calibrationThread = new QThread;
    idleThread = new QThread;
    
    cameraLock = new QMutex(QMutex::NonRecursive);
    
    // this will fail if the UI has not been displayed yet
    calibrator = new CalibrationThread(parent->view->width(), 
                        parent->view->height(), 
                        parent->source,
                        cameraLock
                 );
    
    tracker = new TrackingThread(parent->source, cameraLock);
    idle = new IdleThread(parent->source, cameraLock);
    
    calibrator->moveToThread(calibrationThread);
    tracker->moveToThread(trackingThread);
    idle->moveToThread(idleThread);
    calibrationThread->start();
    trackingThread->start();
    idleThread->start();
    
    // connect all threads with their signals and slots
    setUpSignalHandling();
    
    // setup the state machine
    fsmSetupStateMachine();
}

void ThreadManager::setUpSignalHandling() {
    // the signals for starting the threads
    connect(this, SIGNAL(runCalibration(void)), calibrator, SLOT(run(void)));
    connect(this, SIGNAL(runTracker(Calibration)), tracker, SLOT(track(Calibration)));
    connect(this, SIGNAL(runIdleThread(void)), idle, SLOT(displayCamera(void)));
    
    // the callback signal when a thread has stopped
    connect(calibrator, SIGNAL(hasStopped(PROGRAM_EVENTS)), this, SLOT(threadStopped(PROGRAM_EVENTS)));
    connect(tracker, SIGNAL(hasStopped(PROGRAM_EVENTS)), this, SLOT(threadStopped(PROGRAM_EVENTS)));
    connect(idle, SIGNAL(hasStopped(PROGRAM_EVENTS)), this, SLOT(threadStopped(PROGRAM_EVENTS)));
    
    // signals for displaying an error-message
    connect(calibrator, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(tracker, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(idle, SIGNAL(error(QString)), this, SLOT(error(QString)));
    
    // the signals for the calibratino animation and for starting the tracker
    connect(calibrator, SIGNAL(calibrationFinished(Calibration)), this, SLOT(calibrationFinished(Calibration)));
    connect(calibrator, SIGNAL(jsCommand(QString)), parent, SLOT(execJsCommand(QString)));
    
    // the signal for displaying an image in the eye_widget of the browser ui
    connect(calibrator, SIGNAL(cvImage(cv::Mat)), parent, SLOT(showCvImage(cv::Mat)));
    connect(tracker, SIGNAL(cvImage(cv::Mat)), parent, SLOT(showCvImage(cv::Mat)));
    connect(idle, SIGNAL(cvImage(cv::Mat)), parent, SLOT(showCvImage(cv::Mat)));
}

ThreadManager::~ThreadManager() {
    delete idle;
    delete tracker;
    delete calibrator;
    delete cameraLock;
    delete calibrationThread;
    delete trackingThread;
    delete idleThread;
    delete [] transitions;
}

/*
 * public function for users of this class
 *
 */
void ThreadManager::calibrate(){
    fsmProcessEvent(EV_CALIBRATE);
}

void ThreadManager::goIdle(){
    fsmProcessEvent(EV_GO_IDLE);
}

/*
 *
 * thread callback functions
 * 
 */

void ThreadManager::error(QString message) {
    parent->alertMessage(message);
}

void ThreadManager::calibrationFinished(Calibration calib){
    state = ST_TRACKING;
    emit runTracker(calib);
}

void ThreadManager::threadStopped(PROGRAM_EVENTS nextEvent){
    fsmProcessEvent(nextEvent);
}

/*
 *
 * the methods handling the state machine
 * 
 */

void ThreadManager::fsmSetupStateMachine(){
    state_transitions tmp[] = {
    
        {ST_STARTED_UP, EV_GO_IDLE, ST_IDLE, &ThreadManager::fsmGoIdle},
        {ST_IDLE, EV_GO_IDLE, ST_IDLE, &ThreadManager::fsmGoIdle},
        {ST_IDLE, EV_CALIBRATE, ST_CALIBRATING, &ThreadManager::fsmStopIdle},
        
        {ST_CALIBRATING, EV_CALIBRATE, ST_CALIBRATING, &ThreadManager::fsmCalibrate},
        {ST_CALIBRATING, EV_GO_IDLE, ST_IDLE, &ThreadManager::fsmStopCalibration},
        //{ST_CALIBRATING, EV_TRACKING, NULL}, <-- see calibrationFinished()
        
        {ST_TRACKING, EV_GO_IDLE, ST_IDLE,  &ThreadManager::fsmStopTracking},
        
    };
    
    int num_of_transitions = sizeof(tmp) / sizeof(tmp[0]);
    this->transitions = (state_transitions*) malloc( sizeof(state_transitions) * num_of_transitions );
    memcpy (this->transitions, tmp, sizeof (tmp)) ;
    this->num_of_transitions = num_of_transitions;
}

bool ThreadManager::fsmProcessEvent(PROGRAM_EVENTS event){
    bool processed = false;
    for(int i=0;i<num_of_transitions;++i){
        
        if(this->state == transitions[i].state
           && event == transitions[i].event) {
            
            (this->*transitions[i].func)();
            this->state = transitions[i].next_state;
            
            processed = true;
            break;
        }
    }
    return processed;
}

void ThreadManager::fsmGoIdle(){
    parent->trackingStarted(false);
    emit runIdleThread();
}

void ThreadManager::fsmCalibrate(){
    parent->trackingStarted(true);
    emit runCalibration();
}

void ThreadManager::fsmStopIdle(){
    idle->stop();
}

void ThreadManager::fsmStopCalibration(){
    calibrator->stop();
}

void ThreadManager::fsmStopTracking(){
    tracker->stop();
}