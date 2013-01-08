/* 
 * File:   ThreadManager.cpp
 * Author: fri
 * 
 * Created on January 2, 2013, 11:45 AM
 */

#include <opencv2/core/core.hpp>

#include "ThreadManager.hpp"
#include "worker/CalibrationWorker.hpp"
#include "worker/TrackingWorker.hpp"
#include "worker/IdleWorker.hpp"
#include "../ui/BrowserWindow.hpp"
#include "calibration/Calibration.hpp"
#include "actions/ActionManager.hpp"

ThreadManager::ThreadManager(BrowserWindow *parent) : parent(parent), state(ST_STARTED_UP), calibration(NULL){
    
    // register the OpenCV datatypes for emitting them afterwards
    qRegisterMetaType< cv::Mat > ("cv::Mat");
    qRegisterMetaType< cv::Point > ("cv::Point");
    qRegisterMetaType< Calibration >("Calibration");
    qRegisterMetaType< PROGRAM_EVENTS >("PROGRAM_STATES");
    
    trackingThread = new QThread;
    calibrationThread = new QThread;
    idleThread = new QThread;
    
    cameraLock = new QMutex(QMutex::NonRecursive);
    
    // this will fail if the UI has not been displayed yet
    calibrator = new CalibrationWorker(parent->view->width(), 
                        parent->view->height(), 
                        parent->source,
                        cameraLock
                 );
    
    tracker = new TrackingWorker(parent->source, cameraLock);
    idle = new IdleWorker(parent->source, cameraLock);
    
    calibrator->moveToThread(calibrationThread);
    tracker->moveToThread(trackingThread);
    idle->moveToThread(idleThread);

    // connect all threads with their signals and slots
    setUpSignalHandling();
    
    calibrationThread->start();
    trackingThread->start();
    idleThread->start();
    
    // setup the state machine
    fsmSetupStateMachine();
}

void ThreadManager::setUpSignalHandling() {
    // the callback signal when a thread has stopped
    connect(calibrator, SIGNAL(hasStopped(PROGRAM_STATES)), this, SLOT(threadStopped(PROGRAM_STATES)));
    connect(tracker, SIGNAL(hasStopped(PROGRAM_STATES)), this, SLOT(threadStopped(PROGRAM_STATES)));
    connect(idle, SIGNAL(hasStopped(PROGRAM_STATES)), this, SLOT(threadStopped(PROGRAM_STATES)));
    
    // signals for displaying an error-message
    connect(calibrator, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(tracker, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(idle, SIGNAL(error(QString)), this, SLOT(error(QString)));
    
    // the signals for the calibration animation and for starting the tracker
    connect(calibrator, SIGNAL(calibrationFinished(Calibration)), this, SLOT(calibrationFinished(Calibration)));
    connect(calibrator, SIGNAL(jsCommand(QString)), parent, SLOT(execJsCommand(QString)));
    
    // the signal for displaying an image in the eye_widget of the browser ui
    connect(calibrator, SIGNAL(cvImage(cv::Mat)), parent, SLOT(showCvImage(cv::Mat)));
    connect(tracker, SIGNAL(cvImage(cv::Mat)), parent, SLOT(showCvImage(cv::Mat)));
    connect(idle, SIGNAL(cvImage(cv::Mat)), parent, SLOT(showCvImage(cv::Mat)));
    
    // connect tracker thread with actionmanager to determine gaze action
    connect(tracker, SIGNAL(estimatedPoint(cv::Point)), parent->actionManager, SLOT(estimatedPoint(cv::Point)));
    
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

void ThreadManager::resumeTracking(){
    fsmProcessEvent(EV_TRACKING);
}

/*
 *
 * thread callback functions
 * 
 */

void ThreadManager::error(QString message) {
    parent->alertMessage(message);
    fsmProcessEvent(EV_ERROR);
}

void ThreadManager::calibrationFinished(Calibration calib){
    this->calibration = new Calibration(calib);
    fsmProcessEvent(EV_CALIBRATION_FINISHED);
}

void ThreadManager::threadStopped(PROGRAM_STATES nextState){
    // previous has stopped, now we can start the next one
    this->state = nextState;
    fsmProcessEvent(EV_START);
}

/*
 *
 * the methods handling the state machine
 * 
 */

void ThreadManager::fsmSetupStateMachine(){
    state_transitions tmp[] = {
    
        {ST_STARTED_UP, EV_GO_IDLE, ST_IDLE, &ThreadManager::fsmGoIdle},
        
        {ST_IDLE, EV_START, ST_IDLE, &ThreadManager::fsmGoIdle},
        {ST_IDLE, EV_CALIBRATE, ST_CALIBRATING, &ThreadManager::fsmStopIdle},
        {ST_IDLE, EV_TRACKING, ST_TRACKING, &ThreadManager::fsmStopIdle}, 
        {ST_IDLE, EV_ERROR, ST_ERROR, &ThreadManager::fsmPermanentError},
        
        {ST_CALIBRATING, EV_START, ST_CALIBRATING, &ThreadManager::fsmCalibrate},
        {ST_CALIBRATING, EV_GO_IDLE, ST_IDLE, &ThreadManager::fsmStopCalibration},
        {ST_CALIBRATING, EV_CALIBRATION_FINISHED, ST_TRACKING, &ThreadManager::fsmTrack},
        {ST_CALIBRATING, EV_ERROR, ST_IDLE, &ThreadManager::fsmGoIdle},
        
        {ST_TRACKING, EV_START, ST_TRACKING, &ThreadManager::fsmTrack},
        {ST_TRACKING, EV_GO_IDLE, ST_IDLE,  &ThreadManager::fsmStopTracking},
        {ST_TRACKING, EV_ERROR, ST_IDLE, &ThreadManager::fsmGoIdle},
        
        // ST_ERROR is a sink. the application wont recover from here
        {ST_ERROR, EV_START, ST_ERROR, &ThreadManager::fsmPermanentError},
        {ST_ERROR, EV_GO_IDLE, ST_ERROR, &ThreadManager::fsmPermanentError},
        {ST_ERROR, EV_CALIBRATE, ST_ERROR, &ThreadManager::fsmPermanentError},
        {ST_ERROR, EV_ERROR, ST_ERROR, &ThreadManager::fsmPermanentError},
        
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
            
            (this->*transitions[i].func)(transitions[i].next_state);
            
            processed = true;
            break;
        }
    }
    return processed;
}

void ThreadManager::fsmGoIdle(PROGRAM_STATES nextState){
    this->state = nextState;
    parent->trackingStatus(false, this->calibration != NULL);
    QMetaObject::invokeMethod(idle, "displayCamera", Qt::QueuedConnection);
}

void ThreadManager::fsmCalibrate(PROGRAM_STATES nextState){
    // is this a recalibration?
    if(this->calibration != NULL){
        delete calibration;
        calibration = NULL;
    }
    this->state = nextState;
    parent->trackingStatus(true, false);
    QMetaObject::invokeMethod(calibrator, "run", Qt::QueuedConnection);
}

void ThreadManager::fsmTrack(PROGRAM_STATES nextState){
    parent->trackingStatus(true, false);
    this->state = nextState;
    //TODO are the parentheses around -> needed?
    QMetaObject::invokeMethod(tracker, "track", Qt::QueuedConnection, Q_ARG(Calibration, *(this->calibration)));
}

void ThreadManager::fsmStopIdle(PROGRAM_STATES nextState){
    QMetaObject::invokeMethod(idle, "stop", Qt::DirectConnection, Q_ARG(PROGRAM_STATES, nextState));
}

void ThreadManager::fsmStopCalibration(PROGRAM_STATES nextState){
    QMetaObject::invokeMethod(calibrator, "stop", Qt::DirectConnection, Q_ARG(PROGRAM_STATES, nextState));
}

void ThreadManager::fsmStopTracking(PROGRAM_STATES nextState){
    QMetaObject::invokeMethod(tracker, "stop", Qt::DirectConnection, Q_ARG(PROGRAM_STATES, nextState));
}

void ThreadManager::fsmPermanentError(PROGRAM_STATES nextState){
    this->state = nextState;
    parent->alertMessage("Application is in ST_ERROR state. Please restart the Application");
}