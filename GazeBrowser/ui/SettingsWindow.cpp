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

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(glintGroup);
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

