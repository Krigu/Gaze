/* 
 * File:   ThreadManager.hpp
 * Author: fri
 *
 * Created on January 2, 2013, 11:45 AM
 */

#ifndef THREADMANAGER_HPP
#define	THREADMANAGER_HPP

#include <QtCore>
#include <calibration/Calibration.hpp>

class CalibrationThread;
class TrackingThread;
class BrowserWindow;
class IdleThread;

class ThreadManager : public QObject {
    
    Q_OBJECT
    
public:
    ThreadManager(BrowserWindow *parent);
    virtual ~ThreadManager();
    void startCalibration();
    void showIdle();
    
    signals: 
    void calibrate(void);
    void track(Calibration);
    void showIdleImages(void);
    
public slots:
    void error(QString message);
    void calibrationFinished(Calibration calib);
    
private:
    
    BrowserWindow *parent;
    
    // our threads "application logic"
    CalibrationThread *calibrator;
    TrackingThread *tracker;
    IdleThread *idle;
    
    // the threads
    QThread *trackingThread;
    QThread *calibrationThread;
    QThread *idleThread;
    
    // the Camera-Lock
    QMutex *cameraLock;
    
    // copying the ThreadManager makes absolutely no sense, 
    // let's therefore disable it
    ThreadManager(const ThreadManager& orig);
};

#endif	/* THREADMANAGER_HPP */

