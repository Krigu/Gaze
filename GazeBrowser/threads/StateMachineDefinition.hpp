/* 
 * File:   StateMachineDefinition.hpp
 * Author: fri
 *
 * Created on January 4, 2013, 10:38 AM
 */

#ifndef STATEMACHINEDEFINITION_HPP
#define	STATEMACHINEDEFINITION_HPP

class ThreadManager;

enum PROGRAM_STATES {
    ST_STARTED_UP,
    ST_IDLE,
    ST_CALIBRATING,
    ST_TRACKING, 
    ST_ERROR
};

enum PROGRAM_EVENTS {
    EV_GO_IDLE,
    EV_CALIBRATE, 
    EV_CALIBRATION_FINISHED,
    EV_TRACKING,
    EV_START,
    EV_ERROR
};

typedef struct {
    PROGRAM_STATES state;
    PROGRAM_EVENTS event;
    PROGRAM_STATES next_state;
    void (ThreadManager::*func)(PROGRAM_STATES);
} state_transitions;


#endif	/* STATEMACHINEDEFINITION_HPP */

