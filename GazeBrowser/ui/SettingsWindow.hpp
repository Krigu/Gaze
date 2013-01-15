#ifndef SETTINGSWINDOW_HPP
#define	SETTINGSWINDOW_HPP

#include <QWidget>

class QGroupBox;
class QLabel;
class QSpinBox;
class QRadioButton;

/**
 * the SettingsWindow shows a popup to configure the GazeConfig settings
 */
class SettingsWindow : public QWidget {
    Q_OBJECT

public:
    SettingsWindow();
    virtual ~SettingsWindow();
protected slots:
    void onEyeSelectionToggled();
private:

    QRadioButton *rLeftEye;
    QRadioButton *rRightEye;

    QSpinBox * setUpSpinBox(int min, int max, int step, int& default_value);
};

#endif	/* SETTINGSWINDOW_HPP */

