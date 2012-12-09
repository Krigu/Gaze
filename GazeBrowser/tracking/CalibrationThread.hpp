#ifndef CALIBRATIONTHREAD_HPP_
#define CALIBRATIONTHREAD_HPP_

#include <QtGui>
#include <QtWebKit>
#include "video/ImageSource.hpp"

class Calibration;

class CalibrationThread : public QThread {
    Q_OBJECT

private:
    int width;
    int height;
    ImageSource *camera;
    
    void calibrate(Calibration & calibration);

public:
    CalibrationThread(int width, int height, ImageSource *camera);

signals:
    void jsCommand(QString);
    void error(QString);

protected:
    void run();
};

#endif