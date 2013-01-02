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
    TrackingThread(ImageSource *camera, QMutex *cameraLock);
    virtual ~TrackingThread();
    
    void imageProcessed(Mat& resultImage);
    void imageProcessed(Mat &resultImage, MeasureResult &result, Point2f &gazeVector);
  
    signals:
    void estimatedPoint(cv::Point);
    
public slots:
    void track(Calibration calibration);
    
signals:
    void error(QString);
    void cvImage(cv::Mat);

private:
    ImageSource *camera;
    QMutex *cameraLock;
};

#endif	/* TRACKINGTHREAD_HPP */

