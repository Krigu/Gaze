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

ThreadManager::ThreadManager(BrowserWindow *parent) : parent(parent) {
    
    // register the OpenCV datatypes for emitting them afterwards
    qRegisterMetaType< cv::Mat > ("cv::Mat");
    qRegisterMetaType< cv::Point > ("cv::Point");
    qRegisterMetaType< Calibration >("Calibration");
    
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
    
    connect(this, SIGNAL(calibrate(void)), calibrator, SLOT(run(void)));
    connect(calibrator, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(calibrator, SIGNAL(track(Calibration)), this, SLOT(calibrationFinished(Calibration)));
    connect(calibrator, SIGNAL(jsCommand(QString)), parent, SLOT(execJsCommand(QString)));
    connect(calibrator, SIGNAL(cvImage(cv::Mat)), parent, SLOT(showCvImage(cv::Mat)));
    
    connect(tracker, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(this, SIGNAL(track(Calibration)), tracker, SLOT(track(Calibration)));
    connect(tracker, SIGNAL(cvImage(cv::Mat)), parent, SLOT(showCvImage(cv::Mat)));
    
    connect(this, SIGNAL(showIdleImages()), idle, SLOT(displayCamera(void)));
    connect(idle, SIGNAL(error(QString)), this, SLOT(error(QString)));
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
}

void ThreadManager::startCalibration(){
    // let the calibration thread do its work
    emit calibrate();
}

void ThreadManager::showIdle(){
    emit showIdleImages();
}

void ThreadManager::error(QString message) {
    parent->alertMessage(message);
}

void ThreadManager::calibrationFinished(Calibration calib){
    emit track(calib);
}