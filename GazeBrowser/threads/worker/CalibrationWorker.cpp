
#include <iostream>

#include <opencv2/core/core.hpp>

#include "CalibrationWorker.hpp"
#include "calibration/Calibration.hpp"
#include "detection/GazeTracker.hpp"
#include "video/LiveSource.hpp"
#include "exception/GazeExceptions.hpp"
#include "../Sleeper.hpp"

using namespace std;

CalibrationWorker::CalibrationWorker(int width, int height, ImageSource *camera, QMutex *cameraLock) 
       : width(width), height(height), camera(camera), cameraLock(cameraLock), running(false){
}

void CalibrationWorker::run()
{   
    if(!cameraLock->tryLock()){
        emit error("Cannot calibrate, is the camera in use?");
        return;
    }
    
    running = true;
    calibrated = false;
    
    try{
        Calibration *calib;
        bool calibrated=false;
        while(!calibrated && running){
            calib = new Calibration;
            mCalibration = calib; //TODO: hack!
            calibrated = calibrate(*calib);
            if(!calibrated)
                delete calib;
        }
   
        cameraLock->unlock();
        
        //TODO *calib is probably leaked here...
        if(calibrated)
            emit(calibrationFinished(*calib));
        else if (!running)
            emit hasStopped(nextStateAfterStop);
        
    } catch(GazeException& e) {
        cameraLock->unlock();
        emit error(e.what());
    }
}

bool CalibrationWorker::calibrate(Calibration & calibration){
     int x_offset = 40;
     int y_offset = 40;
     
     int height = this->height - 2 * y_offset;
     int width =  this->width - 2 * x_offset;

     GazeTracker tracker(*camera, this);
     cout << "Begin calib" << endl;
     tracker.initializeCalibration();
     cout << "After calib" << endl;
     for(unsigned short i=0;i<3 && running;i++){
         for(unsigned short j=0;j<3 && running;j++){
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
     
     /*
     if(running)
        return calibration.calibrate(100,3);
     else
         return false;*/
     bool ready = calibration.calibrate(100,3);
     
     if(ready){
         this->calibrated = true;
         tracker.track();
         return true;
     } else{
         return false;
     }
}

bool CalibrationWorker::imageProcessed(Mat& resultImage){
#ifdef __APPLE__
    // openCV on OSX does not block when capturing a frame. without this
    // we would emit 4000 frames a second and block the whole UI
    Sleeper::msleep(33);
#endif   
    emit cvImage(resultImage);
    
    return running;
}

bool CalibrationWorker::imageProcessed(Mat& resultImage, MeasureResult &result, Point2f &gazeVector){
#ifdef __APPLE__
    // openCV on OSX does not block when capturing a frame. without this
    // we would emit 4000 frames a second and block the whole UI
    Sleeper::msleep(33);
#endif   
    emit cvImage(resultImage);
   
    if(result == MEASURE_OK){
        if(calibrated){
            emit estimatedPoint(mCalibration->calcCoordinates(gazeVector));
        } else {
            measurements.push_back(gazeVector);
        }
    }

    std::cout << "Measured: " << result << " " << gazeVector << std::endl;

    return running;
}

void CalibrationWorker::stop(PROGRAM_STATES nextState){
    this->nextStateAfterStop = nextState;
    this->running = false;
}