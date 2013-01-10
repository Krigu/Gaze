/* 
 * File:   MessageWindow.hpp
 * Author: krigu
 *
 * Created on December 7, 2012, 4:07 PM
 */

#ifndef MESSAGEWINDOW_HPP
#define	MESSAGEWINDOW_HPP

#include <QApplication>

class MessageWindow : public QMessageBox{
public:
    MessageWindow();
    void showException(QString message);
    void showInfo(QString message, bool autohide, int durationInSeconds);
private:

};

#endif	/* MESSAGEWINDOW_HPP */

