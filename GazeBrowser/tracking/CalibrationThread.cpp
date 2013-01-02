
#include <iostream>

#include "CalibrationThread.hpp"
#include "calibration/Calibration.hpp"
#include "detection/GazeTracker.hpp"
#include "video/LiveSource.hpp"
#include "exception/GazeExceptions.hpp"
#include "TrackingThread.hpp"
#include "ActionManager.hpp"
#include "../threads/Sleeper.hpp"


using namespace std;

CalibrationThread::CalibrationThread(int width, int height, ImageSource *camera, QMutex *cameraLock) 
       : width(width), height(height), camera(camera), cameraLock(cameraLock){
}

void CalibrationThread::run()
{
    try{
        Calibration *calib;
        bool calibrated=false;
        while(!calibrated){
            calib = new Calibration;
            //calibrated = calibrate(*calib);
            calibrated = true;
            if(!calibrated)
                delete calib;
        }
        
        ActionManager *manager = new ActionManager;
        QThread *thread = new QThread;
        TrackingThread *trackingThread = new TrackingThread(calib);
        
        connect(trackingThread, SIGNAL(estimatedPoint(cv::Point)), manager, SLOT(measuredPoint(cv::Point)));
        
        // TODO: worst hack ever, remove as soon as possible
        connect(this, SIGNAL(jsCommand(QString)), trackingThread, SLOT(testRun(void)));
        
        trackingThread->moveToThread(thread);
        thread->start();
        
        emit jsCommand("");
        
        //trackingThread->start();
   
        cout << "CalibrationThread is dead!" << endl;
    } catch(GazeException& e) {
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