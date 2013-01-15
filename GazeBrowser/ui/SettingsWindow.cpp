#include <QtGui>
#include "SettingsWindow.hpp"
#include "config/GazeConfig.hpp"
#include "AutoSaveSpinbox.hpp"

SettingsWindow::SettingsWindow() {

    QGroupBox *generalGroup = new QGroupBox(tr("General configuration"));
    QGridLayout *generalLayout = new QGridLayout;

    QLabel *eyeLabel = new QLabel(tr("Eye"));
    rLeftEye = new QRadioButton(tr("Left eye"));
    connect(rLeftEye, SIGNAL(toggled(bool)), this, SLOT(onEyeSelectionToggled()));
    rLeftEye->setAutoExclusive(false);
    rRightEye = new QRadioButton(tr("Right eye"));
    connect(rRightEye, SIGNAL(toggled(bool)), this, SLOT(onEyeSelectionToggled()));
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

    QGridLayout *starburst = new QGridLayout();
    starburst->addWidget(new QLabel("Starburst Edge Threshold"));
    starburst->addWidget(setUpSpinBox(0, 255, 1, GazeConfig::STARBURST_EDGE_THRESHOLD));
    eyeBox->addLayout(starburst);
    
    generalLayout->addLayout(eyeBox, 0, 0);
    generalGroup->setLayout(generalLayout);

    QGroupBox *glintGroup = new QGroupBox(tr("Glints Configuration"));
    QLabel *lGlintThreshold = new QLabel(tr("Threshold for binarization"));
    QSpinBox *sGlintThreshold = setUpSpinBox(0, 255, 1, GazeConfig::GLINT_THRESHOLD);
    QLabel *lGlintDistanceTolerance = new QLabel(tr("Distance tolerance for glints"));
    QSpinBox *sGlintMinDistance = setUpSpinBox(0, 100, 1, GazeConfig::GLINT_DISTANCE_TOLERANCE);
    QLabel *lGlintMaxDistance = new QLabel(tr("Max blob distance"));
    QSpinBox *sGlintMaxDistance = setUpSpinBox(0, 100, 1, GazeConfig::GLINT_DISTANCE);
    QLabel *lGlintAngleTolerance = new QLabel(tr("Angle tolerance"));
    QSpinBox *sGlintAngleTolerance = setUpSpinBox(0, 100, 1, GazeConfig::GLINT_ANGLE_TOLERANCE);


    QGridLayout *glintLayout = new QGridLayout;

    glintLayout->addWidget(lGlintThreshold, 0, 0);
    glintLayout->addWidget(sGlintThreshold, 0, 1);
    glintLayout->addWidget(lGlintDistanceTolerance, 1, 0);
    glintLayout->addWidget(sGlintMinDistance, 1, 1);
    glintLayout->addWidget(lGlintMaxDistance, 2, 0);
    glintLayout->addWidget(sGlintMaxDistance, 2, 1);
    glintLayout->addWidget(lGlintAngleTolerance, 3, 0);
    glintLayout->addWidget(sGlintAngleTolerance, 3, 1);

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

}

QSpinBox * SettingsWindow::setUpSpinBox(int min, int max, int step, int& default_value) {
    QSpinBox *spinBox = new AutoSaveSpinbox(default_value);
    spinBox->setRange(min, max);
    spinBox->setSingleStep(step);
    spinBox->setValue(default_value);

    connect(spinBox, SIGNAL(valueChanged(int)),
            spinBox, SLOT(adjustGazeValue(int)));

    return spinBox;
}

void SettingsWindow::onEyeSelectionToggled() {
    if (rLeftEye->isChecked())
        GazeConfig::DETECT_LEFT_EYE = true;
    else
        GazeConfig::DETECT_LEFT_EYE = false;
}

SettingsWindow::~SettingsWindow() {
}

