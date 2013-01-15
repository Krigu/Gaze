#ifndef STATEMACHINEDEFINITION_HPP
#define	STATEMACHINEDEFINITION_HPP

class ThreadManager;

/**
 * the State Machine States for the GazeBrowser application
 */
enum PROGRAM_STATES {
    ST_STARTED_UP,
    ST_IDLE,
    ST_CALIBRATING,
    ST_TRACKING, 
    ST_ERROR
};

/**
 * events supported by the state machine
 */
enum PROGRAM_EVENTS {
    EV_GO_IDLE,
    EV_CALIBRATE, 
    EV_CALIBRATION_FINISHED,
    EV_TRACKING,
    EV_START,
    EV_ERROR
};

/**
 * the type defining the state machines transitions
 */
typedef struct {
    /// the current program-state
    PROGRAM_STATES state;
    /// the event that triggers this transition
    PROGRAM_EVENTS event;
    /// the state that will be set after the function call
    PROGRAM_STATES next_state;
    /// the function that is executed by this state/event combination
    void (ThreadManager::*func)(PROGRAM_STATES);
} state_transitions;


#endif	/* STATEMACHINEDEFINITION_HPP */

