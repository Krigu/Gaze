#ifndef CALIBRATIONTHREAD_HPP_
#define CALIBRATIONTHREAD_HPP_

#include <QtGui>
#include <QtWebKit>
#include "video/ImageSource.hpp"
#include "detection/GazeTracker.hpp"

class Calibration;

class CalibrationThread : public QThread, public TrackerCallback {
    Q_OBJECT

private:
    int width;
    int height;
    ImageSource *camera;
    vector<Point2f> measurements;
    
    void calibrate(Calibration & calibration);

public:
    CalibrationThread(int width, int height, ImageSource *camera);
    void imageProcessed(Mat& resultImage);
    void imageProcessed(Mat &resultImage, MeasureResult &result, Point2f &gazeVector);
    
    
signals:
    void jsCommand(QString);
    void error(QString);
    void cvImage(cv::Mat);

protected:
    void run();
};

#endif