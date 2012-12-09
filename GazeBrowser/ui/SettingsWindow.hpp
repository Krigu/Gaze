/* 
 * File:   SettingsWindow.hpp
 * Author: fri
 *
 * Created on December 9, 2012, 7:27 PM
 */

#ifndef SETTINGSWINDOW_HPP
#define	SETTINGSWINDOW_HPP

#include <QWidget>

class QGroupBox;
class QLabel;
class QSpinBox;

class SettingsWindow : public QWidget {
    
    Q_OBJECT
    
public:
    SettingsWindow();
    virtual ~SettingsWindow();
    
    /*
short GazeConfig::GLINT_MIN_DIAMETER;
short GazeConfig::GLINT_MAX_DIAMETER;
short GazeConfig::GLINT_MIN_PIXEL;
short GazeConfig::GLINT_MAX_PIXEL;
short GazeConfig::GLINT_MIN_DISTANCE;
short GazeConfig::GLINT_MAX_DISTANCE;*/
private:
    
    //
    // glints stuff
    //
    QGroupBox *glintGroup;
    
    QLabel *lGlintThreshold;
    QSpinBox *sGlintThreshold;
    
    QLabel *lGlintMinDiameter;
    QSpinBox *sGlintMinDiameter;
    
    QLabel *lGlintMaxDiameter;
    QSpinBox *sGlintMaxDiameter;
    
    QLabel *lGlintMinPixel;
    QSpinBox* sGlintMinPixel;
    
    QLabel *lGlintMaxPixel;
    QSpinBox* sGlintMaxPixel;
    
    QLabel *lGlintMinDistance;
    QSpinBox *sGlintMinDistance;
    
    QLabel *lGlintMaxDistance;
    QSpinBox *sGlintMaxDistance;
    
    QSpinBox * setUpSpinBox(int min, int max, int step, int default_value);
};

#endif	/* SETTINGSWINDOW_HPP */

