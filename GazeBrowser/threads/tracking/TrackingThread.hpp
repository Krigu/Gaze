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

#include "../StateMachineDefinition.hpp"

class Calibration;

class TrackingThread : public QObject, public TrackerCallback {
    Q_OBJECT

public:
    TrackingThread(ImageSource *camera, QMutex *cameraLock);
    virtual ~TrackingThread();

    bool imageProcessed(Mat& resultImage);
    bool imageProcessed(Mat &resultImage, MeasureResult &result, Point2f &gazeVector);
    
public slots:
    void track(Calibration calibration);
    void stop(PROGRAM_STATES nextState);

signals:
    void error(QString);
    void cvImage(cv::Mat);
    void hasStopped(PROGRAM_STATES);
    void estimatedPoint(cv::Point);

private:
    ImageSource *camera;
    QMutex *cameraLock;
    bool running;
    PROGRAM_STATES nextStateAfterStop;
    Calibration *calibration;
};

#endif	/* TRACKINGTHREAD_HPP */

