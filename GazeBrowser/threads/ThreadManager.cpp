/* 
 * File:   ThreadManager.cpp
 * Author: fri
 * 
 * Created on January 2, 2013, 11:45 AM
 */

#include <opencv2/core/core.hpp>

#include "ThreadManager.hpp"
#include "tracking/CalibrationThread.hpp"
#include "tracking/TrackingThread.hpp"
#include "ui/BrowserWindow.hpp"

ThreadManager::ThreadManager(BrowserWindow *parent) : parent(parent) {
    
    // register the OpenCV datatypes for emitting them afterwards
    qRegisterMetaType< cv::Mat > ("cv::Mat");
    qRegisterMetaType< cv::Point > ("cv::Point");
    
    trackingThread = new QThread;
    calibrationThread = new QThread;
    
    cameraLock = new QMutex(QMutex::NonRecursive);
    
    // this will fail if the UI has not been displayed yet
    calibrator = new CalibrationThread(parent->view->width(), 
                        parent->view->height(), 
                        parent->source,
                        cameraLock
                 );
    
    tracker = new TrackingThread(NULL); //TODO NULL????
    
    calibrator->moveToThread(calibrationThread);
    tracker->moveToThread(trackingThread);
    calibrationThread->start();
    trackingThread->start();
    
    connect(calibrator, SIGNAL(jsCommand(QString)), parent, SLOT(execJsCommand(QString)));
    connect(calibrator, SIGNAL(error(QString)), parent, SLOT(alertMessage(QString)));
    connect(calibrator, SIGNAL(cvImage(cv::Mat)), parent, SLOT(showCvImage(cv::Mat)));
    
}

ThreadManager::~ThreadManager() {
    delete tracker;
    delete calibrator;
    delete cameraLock;
    delete calibrationThread;
    delete trackingThread;
}

void ThreadManager::startCalibration(){
    calibrator->run();
}
