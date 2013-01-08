/* 
 * File:   IdleThread.hpp
 * Author: fri
 *
 * Created on January 2, 2013, 5:33 PM
 */

#ifndef IDLETHREAD_HPP
#define	IDLETHREAD_HPP

#include <QtCore>
#include <opencv2/core/core.hpp>

#include "../StateMachineDefinition.hpp"

class ImageSource;

class IdleThread : public QObject {
    
    Q_OBJECT
    
public:
    IdleThread(ImageSource *camera, QMutex *cameraLock);
    virtual ~IdleThread();
     
public slots:
    void displayCamera(void);
    void stop(PROGRAM_STATES nextState);
    
    
    signals:
    void error(QString);
    void cvImage(cv::Mat);
    void hasStopped(PROGRAM_STATES);
    
private:
    ImageSource *camera;
    QMutex *cameraLock;
    bool running;
    PROGRAM_STATES nextStateAfterStop;
    
};

#endif	/* IDLETHREAD_HPP */

