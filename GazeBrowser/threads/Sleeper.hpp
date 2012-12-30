/* 
 * File:   Sleeper.hpp
 * Author: fri
 *
 * Created on December 30, 2012, 9:05 PM
 */

#ifndef SLEEPER_HPP
#define	SLEEPER_HPP

#include <QtCore>

/**
 * helper class to make the protected QThread::sleep methods public
 *  
 */
class Sleeper : public QThread {
    
    Q_OBJECT
    
public:
    Sleeper();
    virtual ~Sleeper();
    
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
    
private:

};

#endif	/* SLEEPER_HPP */

