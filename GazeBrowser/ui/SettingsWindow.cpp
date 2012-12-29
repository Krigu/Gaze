/* 
 * File:   SettingsWindow.cpp
 * Author: fri
 * 
 * Created on December 9, 2012, 7:27 PM
 */

#include <QtGui>
#include "SettingsWindow.hpp"
#include "config/GazeConfig.hpp"
#include "AutoSaveSpinbox.hpp"

// TODO: Adjust titles of constants. p.E. glint angle tolerance
SettingsWindow::SettingsWindow() {

    QGroupBox *generalGroup = new QGroupBox(tr("General configuration"));
    QGridLayout *generalLayout = new QGridLayout;



    QLabel *eyeLabel = new QLabel(tr("Eye"));
    rLeftEye = new QRadioButton(tr("Left eye"));
    //QObject::connect(radio1,SIGNAL(clicked(bool)),this,SLOT(clickkedstate(bool)));
    rLeftEye->setAutoExclusive(false);
    rRightEye = new QRadioButton(tr("Right eye"));
    //QObject::connect(radio2,SIGNAL(clicked(bool)),this,SLOT(clickkedstate(bool)));
    rLeftEye->setChecked(GazeConfig::DETECT_LEFT_EYE);
    rRightEye->setChecked(!GazeConfig::DETECT_LEFT_EYE);

    QVBoxLayout *eyeBox = new QVBoxLayout;
    eyeBox->addWidget(eyeLabel);
    eyeBox->addWidget(rLeftEye);
    eyeBox->addWidget(rRightEye);
    eyeBox->addStretch(1);

    QButtonGroup *btnGroupEye = new QButtonGroup(eyeBox);
    btnGroupEye->addButton(rLeftEye);
    btnGroupEye->addButton(rRightEye);

    generalLayout->addLayout(eyeBox, 0, 0);

    QLabel *detectionLabel = new QLabel(tr("Detect"));
    rPupil = new QRadioButton(tr("Iris"));
    //QObject::connect(radio1,SIGNAL(clicked(bool)),this,SLOT(clickkedstate(bool)));
    rPupil->setAutoExclusive(false);
    rIris = new QRadioButton(tr("Pupil"));
    //QObject::connect(radio2,SIGNAL(clicked(bool)),this,SLOT(clickkedstate(bool)));

    rPupil->setChecked(GazeConfig::DETECT_PUPIL);
    rPupil->setAutoExclusive(true);
    rIris->setChecked(!GazeConfig::DETECT_PUPIL);
    rIris->setAutoExclusive(true);

    QVBoxLayout *detectionBox = new QVBoxLayout;
    detectionBox->addWidget(detectionLabel);
    detectionBox->addWidget(rPupil);
    detectionBox->addWidget(rIris);
    detectionBox->addStretch(1);
    generalLayout->addLayout(detectionBox, 1, 0);

    QButtonGroup *btnGroupDetection = new QButtonGroup(eyeBox);
    btnGroupDetection->addButton(rPupil);
    btnGroupDetection->addButton(rIris);

    generalGroup->setLayout(generalLayout);

    QGroupBox *glintGroup = new QGroupBox(tr("Glints Configuration"));
    QLabel *lGlintThreshold = new QLabel(tr("threshold for binarization"));
    QSpinBox *sGlintThreshold = setUpSpinBox(0, 255, 1, GazeConfig::GLINT_THRESHOLD);
    QLabel *lGlintDistanceTolerance = new QLabel(tr("Distance tolerance for glints"));
    QSpinBox *sGlintMinDistance = setUpSpinBox(0, 100, 1, GazeConfig::GLINT_DISTANCE_TOLERANCE);
    QLabel *lGlintMaxDistance = new QLabel(tr("Max blob distance"));
    QSpinBox *sGlintMaxDistance = setUpSpinBox(0, 100, 1, GazeConfig::GLINT_DISTANCE);


    QGridLayout *glintLayout = new QGridLayout;

    glintLayout->addWidget(lGlintThreshold, 0, 0);
    glintLayout->addWidget(sGlintThreshold, 0, 1);
    glintLayout->addWidget(lGlintDistanceTolerance, 1, 0);
    glintLayout->addWidget(sGlintMinDistance, 1, 1);
    glintLayout->addWidget(lGlintMaxDistance, 2, 0);
    glintLayout->addWidget(sGlintMaxDistance, 2, 1);

    glintGroup->setLayout(glintLayout);

    //
    // haar
    //
    QGroupBox *haarGroup = new QGroupBox(tr("Haar Cascade Configuration"));
    QGridLayout *haarLayout = new QGridLayout;

    QLabel *lHaarEyeRegMinHeight = new QLabel(tr("Min height of eye region"));
    QSpinBox *sHaarEyeRegMinHeight = setUpSpinBox(0, 1000, 1, GazeConfig::HAAR_EYEREGION_MIN_HEIGHT);
    haarLayout->addWidget(lHaarEyeRegMinHeight, 0, 0);
    haarLayout->addWidget(sHaarEyeRegMinHeight, 0, 1);

    QLabel *lHaarEyeRegMinWidth = new QLabel(tr("Min width of eye region"));
    QSpinBox *sHaarEyeRegMinWidth = setUpSpinBox(0, 1000, 1, GazeConfig::HAAR_EYEREGION_MIN_WIDTH);
    haarLayout->addWidget(lHaarEyeRegMinWidth, 1, 0);
    haarLayout->addWidget(sHaarEyeRegMinWidth, 1, 1);

    QLabel *lHaarEyeMinHeight = new QLabel(tr("Min eye height"));
    QSpinBox *sHaarEyeMinHeight = setUpSpinBox(0, 1000, 1, GazeConfig::HAAR_EYE_MIN_HEIGHT);
    haarLayout->addWidget(lHaarEyeMinHeight, 2, 0);
    haarLayout->addWidget(sHaarEyeMinHeight, 2, 1);

    QLabel *lHaarEyeMinWidth = new QLabel(tr("Min eye width"));
    QSpinBox *sHaarEyeMinWidth = setUpSpinBox(0, 1000, 1, GazeConfig::HAAR_EYE_MIN_WIDTH);
    haarLayout->addWidget(lHaarEyeMinWidth, 3, 0);
    haarLayout->addWidget(sHaarEyeMinWidth, 3, 1);

    QLabel *lHaarEyeMaxHeight = new QLabel(tr("Max eye height"));
    QSpinBox *sHaarEyeMaxHeight = setUpSpinBox(0, 1280, 1, GazeConfig::HAAR_EYE_MAX_HEIGHT);
    haarLayout->addWidget(lHaarEyeMaxHeight, 4, 0);
    haarLayout->addWidget(sHaarEyeMaxHeight, 4, 1);

    QLabel *lHaarEyeMaxWidth = new QLabel(tr("Max eye width"));
    QSpinBox *sHaarEyeMaxWidth = setUpSpinBox(0, 1280, 1, GazeConfig::HAAR_EYE_MAX_WIDTH);
    haarLayout->addWidget(lHaarEyeMaxWidth, 5, 0);
    haarLayout->addWidget(sHaarEyeMaxWidth, 5, 1);

    QLabel *lHaarMaxTries = new QLabel(tr("Max tries to find eye region"));
    QSpinBox *sHaarMaxTries = setUpSpinBox(0, 100, 1, GazeConfig::HAAR_FINDREGION_MAX_TRIES);
    haarLayout->addWidget(lHaarMaxTries, 6, 0);
    haarLayout->addWidget(sHaarMaxTries, 6, 1);

    haarGroup->setLayout(haarLayout);

    //
    //
    //
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(generalGroup);
    layout->addWidget(glintGroup);
    layout->addWidget(haarGroup);
    setLayout(layout);

    // register handlers
    connect(rLeftEye, SIGNAL(toggled(bool)), this, SLOT(onEyeSelectionToggled()));
    connect(rRightEye, SIGNAL(toggled(bool)), this, SLOT(onEyeSelectionToggled()));

    connect(rIris, SIGNAL(toggled(bool)), this, SLOT(onStarburstSelectionToggled()));
    connect(rPupil, SIGNAL(toggled(bool)), this, SLOT(onStarburstSelectionToggled()));

}

QSpinBox * SettingsWindow::setUpSpinBox(int min, int max, int step, int& default_value) {
    QSpinBox *spinBox = new AutoSaveSpinbox(default_value);
    spinBox->setRange(min, max);
    spinBox->setSingleStep(step);
    spinBox->setValue(default_value);

    return spinBox;
}

void SettingsWindow::onEyeSelectionToggled(bool checked) {
    if (rLeftEye->isChecked())
        GazeConfig::DETECT_LEFT_EYE = true;
    else
        GazeConfig::DETECT_LEFT_EYE = false;
}

void SettingsWindow::onStarburstSelectionToggled(bool checked) {
    if (rIris->isChecked())
        GazeConfig::DETECT_PUPIL = true;
    else
        GazeConfig::DETECT_PUPIL = false;    
}

SettingsWindow::~SettingsWindow() {
}

