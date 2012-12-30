/* 
 * File:   TrackingThread.hpp
 * Author: fri
 *
 * Created on December 30, 2012, 6:44 PM
 */

#ifndef TRACKINGTHREAD_HPP
#define	TRACKINGTHREAD_HPP

#include <QtCore>
#include "detection/GazeTracker.hpp"

class CalibrationData;

class TrackingThread : public QObject, public TrackerCallback {
    Q_OBJECT
    
public:
    TrackingThread(Calibration *calibData);
    virtual ~TrackingThread();
    
    void imageProcessed(Mat& resultImage);
    void imageProcessed(Mat &resultImage, MeasureResult &result, Point2f &gazeVector);
  
    signals:
    void estimatedPoint(cv::Point);
    
public slots:
    void testRun(void);
    
private:
    Calibration *calibData;
};

#endif	/* TRACKINGTHREAD_HPP */

