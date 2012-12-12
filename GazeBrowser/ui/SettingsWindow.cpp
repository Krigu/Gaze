/* 
 * File:   SettingsWindow.cpp
 * Author: fri
 * 
 * Created on December 9, 2012, 7:27 PM
 */

#include <QtGui>
#include "SettingsWindow.hpp"
#include "config/GazeConfig.hpp"

SettingsWindow::SettingsWindow() {
    glintGroup = new QGroupBox(tr("Glints Configuration"));
    lGlintThreshold = new QLabel(tr("Glint Threshold"));
    sGlintThreshold = setUpSpinBox(0, 255, 1, GazeConfig::GLINT_THRESHOLD);
    lGlintMinDiameter = new QLabel(tr("Min Diameter"));
    sGlintMinDiameter = setUpSpinBox(0, 255, 1, GazeConfig::GLINT_MIN_DIAMETER);
    lGlintMaxDiameter = new QLabel(tr("Max Diameter"));
    sGlintMaxDiameter = setUpSpinBox(0, 255, 1, GazeConfig::GLINT_MAX_DIAMETER);
    lGlintMinPixel = new QLabel(tr("Min number of Pixels"));
    sGlintMinPixel = setUpSpinBox(0, 255, 1, GazeConfig::GLINT_MIN_PIXEL);
    lGlintMaxPixel = new QLabel(tr("Max number of Pixels"));
    sGlintMaxPixel = setUpSpinBox(0, 255, 1, GazeConfig::GLINT_MAX_PIXEL);
    lGlintMinDistance = new QLabel(tr("Min Distance"));
    sGlintMinDistance = setUpSpinBox(0, 255, 1, GazeConfig::GLINT_MIN_DISTANCE);
    lGlintMaxDistance = new QLabel(tr("Max Distance"));
    sGlintMaxDistance = setUpSpinBox(0, 255, 1, GazeConfig::GLINT_MAX_DISTANCE);

    QGridLayout *glintLayout = new QGridLayout;

    glintLayout->addWidget(lGlintThreshold, 0, 0);
    glintLayout->addWidget(sGlintThreshold, 0, 1);
    glintLayout->addWidget(lGlintMinDiameter, 1, 0);
    glintLayout->addWidget(sGlintMinDiameter, 1, 1);
    glintLayout->addWidget(lGlintMaxDiameter, 2, 0);
    glintLayout->addWidget(sGlintMaxDiameter, 2, 1);
    glintLayout->addWidget(lGlintMinPixel, 3, 0);
    glintLayout->addWidget(sGlintMinPixel, 3, 1);
    glintLayout->addWidget(lGlintMaxPixel, 4, 0);
    glintLayout->addWidget(sGlintMaxPixel, 4, 1);
    glintLayout->addWidget(lGlintMinDistance, 5, 0);
    glintLayout->addWidget(sGlintMinDistance, 5, 1);
    glintLayout->addWidget(lGlintMaxDistance, 6, 0);
    glintLayout->addWidget(sGlintMaxDistance, 6, 1);

    glintGroup->setLayout(glintLayout);

    //
    // haar
    //
    haarGroup = new QGroupBox(tr("Haar Cascade Configuration"));
    QGridLayout *haarLayout = new QGridLayout;

    lHaarEyeRegMinHeight = new QLabel(tr("Min Height of Eye Region"));
    sHaarEyeRegMinHeight = setUpSpinBox(0, 255, 1, GazeConfig::HAAR_EYEREGION_MIN_HEIGHT);
    haarLayout->addWidget(lHaarEyeRegMinHeight, 0,0);
    haarLayout->addWidget(sHaarEyeRegMinHeight, 0,1);

    lHaarEyeRegMinWidth = new QLabel(tr("Min Width of Eye Region"));
    sHaarEyeRegMinWidth = setUpSpinBox(0, 500, 1, GazeConfig::HAAR_EYEREGION_MIN_WIDTH);
    haarLayout->addWidget(lHaarEyeRegMinWidth, 1,0);
    haarLayout->addWidget(sHaarEyeRegMinWidth, 1,1);
    
    lHaarEyeMinHeight = new QLabel(tr("Min eye height"));
    sHaarEyeMinHeight = setUpSpinBox(0, 255, 1, GazeConfig::HAAR_EYE_MIN_HEIGHT);
    haarLayout->addWidget(lHaarEyeMinHeight, 2,0);
    haarLayout->addWidget(sHaarEyeMinHeight, 2,1);
    
    lHaarEyeMinWidth = new QLabel(tr("Min eye width"));
    sHaarEyeMinWidth = setUpSpinBox(0, 255, 1, GazeConfig::HAAR_EYE_MIN_WIDTH);
    haarLayout->addWidget(lHaarEyeMinWidth, 3,0);
    haarLayout->addWidget(sHaarEyeMinWidth, 3,1);
    
    lHaarEyeMaxHeight = new QLabel(tr("Max eye height"));
    sHaarEyeMaxHeight = setUpSpinBox(0, 255, 1, GazeConfig::HAAR_EYE_MAX_HEIGHT);
    haarLayout->addWidget(lHaarEyeMaxHeight, 4,0);
    haarLayout->addWidget(sHaarEyeMaxHeight, 4,1);
    
    lHaarEyeMaxWidth = new QLabel(tr("Max eye width"));
    sHaarEyeMaxWidth = setUpSpinBox(0, 255, 1, GazeConfig::HAAR_EYE_MAX_WIDTH);
    haarLayout->addWidget(lHaarEyeMaxWidth, 5,0);
    haarLayout->addWidget(sHaarEyeMaxWidth, 5,1);
    
    lHaarMaxTries = new QLabel(tr("Max tries to find eye region"));
    sHaarMaxTries = setUpSpinBox(0, 10, 1, GazeConfig::HAAR_FINDREGION_MAX_TRIES);
    haarLayout->addWidget(lHaarMaxTries, 6,0);
    haarLayout->addWidget(sHaarMaxTries, 6,1);
    
    haarGroup->setLayout(haarLayout);
    
    //
    // RANSAC stuff
    //
    pupilGroup = new QGroupBox(tr("Pupil Settings"));
    QGridLayout *pupilLayout = new QGridLayout;
    
    lPupilMinRadius = new QLabel("Min Pupil Radius");
    sPupilMinRadius = setUpSpinBox(0, 255, 1, GazeConfig::PUPIL_MIN_RADIUS);
    pupilLayout->addWidget(lPupilMinRadius,0,0);
    pupilLayout->addWidget(sPupilMinRadius,0,1);
    
    lPupilMaxRadius = new QLabel("Max Pupil Radius");
    sPupilMaxRadius = setUpSpinBox(0, 255, 1, GazeConfig::PUPIL_MAX_RADIUS);
    pupilLayout->addWidget(lPupilMaxRadius,1,0);
    pupilLayout->addWidget(sPupilMaxRadius,1,1);
    pupilGroup->setLayout(pupilLayout);
    
    //
    //
    //
    
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(glintGroup);
    layout->addWidget(haarGroup);
    layout->addWidget(pupilGroup);
    setLayout(layout);
}

QSpinBox * SettingsWindow::setUpSpinBox(int min, int max, int step, int default_value) {
    QSpinBox *spinBox = new QSpinBox;
    spinBox->setRange(min, max);
    spinBox->setSingleStep(step);
    spinBox->setValue(default_value);
    return spinBox;
}

SettingsWindow::~SettingsWindow() {
}

