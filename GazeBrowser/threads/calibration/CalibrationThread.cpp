
#include <iostream>

#include <opencv2/core/core.hpp>

#include "CalibrationThread.hpp"
#include "calibration/Calibration.hpp"
#include "detection/GazeTracker.hpp"
#include "video/LiveSource.hpp"
#include "exception/GazeExceptions.hpp"
#include "../Sleeper.hpp"

using namespace std;

CalibrationThread::CalibrationThread(int width, int height, ImageSource *camera, QMutex *cameraLock) 
       : width(width), height(height), camera(camera), cameraLock(cameraLock), running(false){
}

void CalibrationThread::run()
{   
    if(!cameraLock->tryLock()){
        emit error("Cannot calibrate, is the camera in use?");
        return;
    }
    
    running = true;
    
    try{
        Calibration *calib;
        bool calibrated=false;
        while(!calibrated && running){
            calib = new Calibration;
            //calibrated = calibrate(*calib);
            calibrated = true;
            if(!calibrated)
                delete calib;
        }
   
        cameraLock->unlock();
        
        if(calibrated)
            emit(calibrationFinished(*calib));
        else if (!running)
            emit hasStopped(EV_GO_IDLE);
        
    } catch(GazeException& e) {
        cameraLock->unlock();
        emit error(e.what());
    }
}

bool CalibrationThread::calibrate(Calibration & calibration){
     int x_offset = 40;
     int y_offset = 40;
     
     int height = this->height - 2 * y_offset;
     int width =  this->width - 2 * x_offset;

     GazeTracker tracker(*camera, this);
     cout << "Begin calib" << endl;
     tracker.initializeCalibration();
     cout << "After calib" << endl;
     for(unsigned short i=0;i<3;i++){
         for(unsigned short j=0;j<3;j++){
            int point_x = width / 2 * j + x_offset; 
            int point_y = height / 2 * i + y_offset;
             
            cout << "Calibration Point: " << point_x << "/" << point_y << endl;
            
            QString code = QString("calibrationCircle.move(%1,%2);")
                    .arg(point_x).arg(point_y);
            
            emit jsCommand(code);
            Sleeper::msleep(3000);
            
            Point2f p(point_x, point_y);
            measurements.clear();
            tracker.track(5);
            CalibrationData data(p, measurements);
            calibration.addCalibrationData(data);
            cout << "Point: " << p << "Vector: " << data.getMeasuredMedianVector() << endl;
         }
     }
     
     return calibration.calibrate(100,3);
}

void CalibrationThread::imageProcessed(Mat& resultImage){
    //TODO move the sleep into another (non-UI) thread?
    Sleeper::msleep(33);
    emit cvImage(resultImage);
}

void CalibrationThread::imageProcessed(Mat& resultImage, MeasureResult &result, Point2f &gazeVector){
    //TODO move the sleep into another (non-UI) thread?
    Sleeper::msleep(33);
    emit cvImage(resultImage);
   
    if(result == MEASURE_OK){
            measurements.push_back(gazeVector);
    }

    std::cout << "Measured: " << result << " " << gazeVector << std::endl;
}

void CalibrationThread::stop(){
    this->running = false;
}