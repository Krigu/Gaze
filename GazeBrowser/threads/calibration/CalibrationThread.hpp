#ifndef CALIBRATIONTHREAD_HPP_
#define CALIBRATIONTHREAD_HPP_

#include <QtCore>
#include "video/ImageSource.hpp"
#include "detection/GazeTracker.hpp"

class Calibration;

class CalibrationThread : public QObject, public TrackerCallback {
    Q_OBJECT

private:
    int width;
    int height;
    ImageSource *camera;
    QMutex *cameraLock;
    vector<Point2f> measurements;
    bool running;
    
    bool calibrate(Calibration & calibration);

public:
    CalibrationThread(int width, int height, ImageSource *camera, QMutex *cameraLock);
    void imageProcessed(Mat& resultImage);
    void imageProcessed(Mat &resultImage, MeasureResult &result, Point2f &gazeVector);
    void stop();
    
public slots:
    void run(void);

    
signals:
    void jsCommand(QString);
    void error(QString);
    void calibrationFinished(Calibration);
    void cvImage(cv::Mat);
    void hasStopped();

};
#endif