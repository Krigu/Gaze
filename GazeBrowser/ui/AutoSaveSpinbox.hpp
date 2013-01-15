#ifndef AUTOSAVESPINBOX_HPP
#define	AUTOSAVESPINBOX_HPP

#include <QtGui>

/**
 * a SpinBox that automagically adapts the GazeConfig settings
 */
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

