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
protected slots:
    void onEyeSelectionToggled(bool checked);
    void onStarburstSelectionToggled(bool checked);    
private:

    

    QSpinBox * setUpSpinBox(int min, int max, int step, int& default_value);
};

#endif	/* SETTINGSWINDOW_HPP */

