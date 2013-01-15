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

