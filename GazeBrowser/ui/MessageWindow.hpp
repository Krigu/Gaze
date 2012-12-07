/* 
 * File:   MessageWindow.hpp
 * Author: krigu
 *
 * Created on December 7, 2012, 4:07 PM
 */

#ifndef MESSAGEWINDOW_HPP
#define	MESSAGEWINDOW_HPP

#include <QApplication>

class MessageWindow : public QWidget{
public:
    MessageWindow();
    void showException(QString message);
private:

};

#endif	/* MESSAGEWINDOW_HPP */

