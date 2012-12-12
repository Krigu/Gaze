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
    
    //
    // Haar configuration
    //
    QGroupBox *haarGroup;
    
    QLabel *lHaarEyeRegMinHeight;
    QSpinBox *sHaarEyeRegMinHeight;
    
    QLabel *lHaarEyeRegMinWidth;
    QSpinBox *sHaarEyeRegMinWidth;
    
    QLabel *lHaarEyeMinHeight;
    QSpinBox *sHaarEyeMinHeight;
    
    QLabel *lHaarEyeMinWidth;
    QSpinBox *sHaarEyeMinWidth;
    
    QLabel *lHaarEyeMaxHeight;
    QSpinBox *sHaarEyeMaxHeight;
    
    QLabel *lHaarEyeMaxWidth;
    QSpinBox *sHaarEyeMaxWidth;
    
    QLabel *lHaarMaxTries;
    QSpinBox *sHaarMaxTries;
    
    //
    // RANSAC stuff
    //
    QGroupBox *pupilGroup;
    
    QLabel *lPupilMinRadius;
    QSpinBox *sPupilMinRadius;
    
    QLabel *lPupilMaxRadius;
    QSpinBox *sPupilMaxRadius;
    
    QSpinBox * setUpSpinBox(int min, int max, int step, int& default_value);
};

#endif	/* SETTINGSWINDOW_HPP */

