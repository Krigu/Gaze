#ifndef IDLETHREAD_HPP
#define	IDLETHREAD_HPP

#include <QtCore>
#include <opencv2/core/core.hpp>

#include "../StateMachineDefinition.hpp"

class ImageSource;

/**
 * The IdleWorker is the application logic for a QThread. this thread
 * grabs frames from the camera and emity them to the Qt Signal Queue
 */
class IdleWorker : public QObject {
    
    Q_OBJECT
    
public:
    IdleWorker(ImageSource *camera, QMutex *cameraLock);
    virtual ~IdleWorker();
     
public slots:
    
    /**
     * reads images directly from the camera and emits them to be displayed in 
     * the eye_widget. this method only works wit LiveSource (real webcams). 
     * when started with a VideoSource (movie files) an error is displayed.
     */
    void displayCamera(void);
    
    /**
     * stops the thread and notifies the ThreadManager with the next state
     * @param nextState
     */
    void stop(PROGRAM_STATES nextState);
    
    
    signals:
    void error(QString);
    void cvImage(cv::Mat*);
    void hasStopped(PROGRAM_STATES);
    
private:
    ImageSource *camera;
    QMutex *cameraLock;
    bool running;
    PROGRAM_STATES nextStateAfterStop;
    
};

#endif	/* IDLETHREAD_HPP */

