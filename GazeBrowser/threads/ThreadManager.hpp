#ifndef THREADMANAGER_HPP
#define	THREADMANAGER_HPP

#include <QtCore>
#include <calibration/Calibration.hpp>

#include "StateMachineDefinition.hpp"

class GazeTrackWorker;
class BrowserWindow;
class IdleWorker;

/**
 * the ThreadManager controls the GazeTrack and Idle Thread and implements the GazeBrowsers StateMachine
 */
class ThreadManager : public QObject {
    Q_OBJECT

public:
    ThreadManager(BrowserWindow *parent);
    virtual ~ThreadManager();
    
    /**
     * executes the EV_CALIBRATE event and starts the calibration (if IDLE)
     */
    void calibrate();
    
    /**
     * stops the tracking and goes IDLE
     */
    void goIdle();
    
    /**
     * continues a tracking (if the system is calibrated)
     */
    void resumeTracking();
    
    /**
     * obvious...
     * @return true if calibrated
     */
    bool isCalibrated();

public slots:
    void error(QString message);
    void info(QString message);
    void calibrationFinished();
    void threadStopped(PROGRAM_STATES nextState);

private:

    BrowserWindow *parent;
    PROGRAM_STATES state;

    // our threads "application logic"
    GazeTrackWorker *gazeTracker;
    IdleWorker *idle;

    // the threads
    QThread *gazeTrackerThread;
    QThread *idleThread;

    // the Camera-Lock
    QMutex *cameraLock;

    // the state transitions for the state machine
    state_transitions *transitions;
    int num_of_transitions;

    void setUpSignalHandling();

    void fsmSetupStateMachine();
    bool fsmProcessEvent(PROGRAM_EVENTS event);
    void fsmGoIdle(PROGRAM_STATES nextState);
    void fsmCalibrate(PROGRAM_STATES nextState);
    void fsmTrack(PROGRAM_STATES nextState);
    void fsmStopIdle(PROGRAM_STATES nextState);
    void fsmStopGazeTracking(PROGRAM_STATES nextState);
    void fsmPermanentError(PROGRAM_STATES nextState);

    // copying the ThreadManager makes absolutely no sense, 
    // let's therefore disable it
    ThreadManager(const ThreadManager& orig);
};

#endif	/* THREADMANAGER_HPP */

