#ifndef CALIBRATIONTHREAD_HPP_
#define CALIBRATIONTHREAD_HPP_

 #include <QtGui>
#include <QtWebKit>


 class CalibrationThread : public QThread
 {
     Q_OBJECT
 
     private:
         int width;
         int height;
     
public:
    CalibrationThread(int width, int height);
 
signals:
    void jsCommand(QString);
    
 protected:
     void run();
 };

#endif