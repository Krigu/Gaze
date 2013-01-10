#ifndef CALIBRATIONTHREAD_HPP_
#define CALIBRATIONTHREAD_HPP_

#include <QtCore>
#include "video/ImageSource.hpp"
#include "detection/GazeTracker.hpp"

#include "../StateMachineDefinition.hpp"

class Calibration;
class GazeTracker;

class GazeTrackWorker : public QObject, public TrackerCallback {
    Q_OBJECT

private:
    int width;
    int height;
    ImageSource *camera;
    QMutex *cameraLock;
    vector<Point2f> measurements;
    bool running;
    bool tracking;
    PROGRAM_STATES nextStateAfterStop;
    GazeTracker *tracker;
    Calibration *calibration;
    
    bool calibrate();
   

public:
    GazeTrackWorker(int width, int height, ImageSource *camera, QMutex *cameraLock);
    virtual ~GazeTrackWorker();
    bool imageProcessed(Mat& resultImage);
    bool imageProcessed(Mat &resultImage, MeasureResult &result, Point2f &gazeVector);
    
    
public slots:
    void startCalibration(void);
    void startTracking(void);
    void stop(PROGRAM_STATES nextState);
    bool isCalibrated();

    
signals:
    void jsCommand(QString);
    void error(QString);
    void calibrationFinished();
    void cvImage(cv::Mat);
    void hasStopped(PROGRAM_STATES);
    void estimatedPoint(cv::Point);

};
#endif