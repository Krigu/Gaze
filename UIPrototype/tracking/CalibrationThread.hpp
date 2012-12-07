#ifndef CALIBRATIONTHREAD_HPP_
#define CALIBRATIONTHREAD_HPP_

 #include <QtGui>
#include <QtWebKit>

#include "video/LiveSource.hpp"

 class CalibrationThread : public QThread
 {
     Q_OBJECT
 
     private:
         int width;
         int height;
         LiveSource *source;
     
public:
    CalibrationThread(int width, int height, LiveSource *source);
    
signals:
    void jsCommand(QString);

 protected:
     void run();
 };

#endif