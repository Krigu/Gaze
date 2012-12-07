#ifndef CALIBRATIONTHREAD_HPP_
#define CALIBRATIONTHREAD_HPP_

#include <QtGui>
#include <QtWebKit>

class LiveSource;

class CalibrationThread : public QThread {
    Q_OBJECT

private:
    int width;
    int height;
    LiveSource *camera;

public:
    CalibrationThread(int width, int height, LiveSource *camera);

signals:
    void jsCommand(QString);

protected:
    void run();
};

#endif