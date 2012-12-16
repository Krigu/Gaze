/* 
 * File:   AutoSaveSpinbox.hpp
 * Author: krigu
 *
 * Created on December 12, 2012, 1:14 PM
 */

#ifndef AUTOSAVESPINBOX_HPP
#define	AUTOSAVESPINBOX_HPP

#include <QtGui>

class AutoSaveSpinbox : public QSpinBox {
    Q_OBJECT
public:
    AutoSaveSpinbox(int& value);
    virtual ~AutoSaveSpinbox();
private:
    int& constantValue;
public slots:
    void adjustGazeValue(int value);
};

#endif	/* AUTOSAVESPINBOX_HPP */

