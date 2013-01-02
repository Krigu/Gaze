/* 
 * File:   CustomMutexLocker.hpp
 * Author: fri
 *
 * Created on January 2, 2013, 2:00 PM
 */

#ifndef CUSTOMMUTEXLOCKER_HPP
#define	CUSTOMMUTEXLOCKER_HPP

#include <iostream>

class QMutex;

/**
 * CutomMutexLocker is a RAII locker. 
 * The lock is active, as long as this object exists. 
 * <br />
 * in contrast to QMutexLocker CustomMutexLocker uses tryLock()
 * and therefore <b>never</b> blocks the locking. you have
 * to check isLocked() after acquiring the lock.
 * 
 */
class CustomMutexLocker {
    
public:
    inline CustomMutexLocker(QMutex *mutex){
        locked = mutex->tryLockInline();
    }
    
    inline virtual ~CustomMutexLocker(){
        if(locked)
            mutex->unlockInline();
    }
    
    inline bool isLocked(){
        return locked;
    }

private:
    QMutex *mutex;
    bool locked;
    CustomMutexLocker(const CustomMutexLocker& orig);

};

#endif	/* CUSTOMMUTEXLOCKER_HPP */

