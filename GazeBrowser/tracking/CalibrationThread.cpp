
#include <iostream>

#include "CalibrationThread.hpp"
#include "calibration/Calibration.hpp"
#include "detection/GazeTracker.hpp"
#include "video/LiveSource.hpp"
#include "exception/GazeExceptions.hpp"

using namespace std;

CalibrationThread::CalibrationThread(int width, int height, ImageSource *camera) 
       : width(width), height(height), camera(camera){
}

void CalibrationThread::run()
{
    try{
        Calibration calib;
        calibrate(calib);
        
        // todo: now start/continue the tracker fred
        //calib.calculateCoordinates();
    } catch(GazeException& e) {
        emit error(e.what());
    }
}

void CalibrationThread::calibrate(Calibration & calibration){
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
            msleep(3000);
            
            Point2f p(point_x, point_y);
            measurements.clear();
            tracker.track(5);
            CalibrationData data(p, measurements);
            calibration.addCalibrationData(data);
            cout << "Point: " << p << "Vector: " << data.getAverageVector() << endl;
         }
     }
     
     calibration.calcCoefficients();
     
     cout << "Calibrationdata:" << endl;
     calibration.printCalibration();
     
}

void CalibrationThread::imageProcessed(Mat& resultImage){
    //TODO move the sleep into another (non-UI) thread?
    msleep(10);
    emit cvImage(resultImage);
}

void CalibrationThread::imageProcessed(Mat& resultImage, MeasureResult &result, Point2f &gazeVector){
    //TODO move the sleep into another (non-UI) thread?
    msleep(10);
    emit cvImage(resultImage);
    switch (result) {
        case MEASURE_OK:
            measurements.push_back(gazeVector);
            break;
    }
    std::cout << "Measured: " << result << " " << gazeVector << std::endl;
}