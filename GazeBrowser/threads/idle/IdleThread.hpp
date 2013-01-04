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

class ImageSource;

class IdleThread : public QObject {
    
    Q_OBJECT
    
public:
    IdleThread(ImageSource *camera, QMutex *cameraLock);
    virtual ~IdleThread();
    void stop();
     
public slots:
    void displayCamera(void);

    
    signals:
    void error(QString);
    void cvImage(cv::Mat);
    void hasStopped();
    
private:
    ImageSource *camera;
    QMutex *cameraLock;
    bool running;
    
};

#endif	/* IDLETHREAD_HPP */

