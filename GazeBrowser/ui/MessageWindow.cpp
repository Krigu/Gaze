/* 
 * File:   MessageWindow.cpp
 * Author: krigu
 * 
 * Created on December 7, 2012, 4:07 PM
 */

#include <QObject>
#include <QMessageBox>
#include <QTimer>

#include "MessageWindow.hpp"

MessageWindow::MessageWindow() {
}

void MessageWindow::showException(QString message) {
    setText(message);
    setIcon(QMessageBox::Critical);
    exec();
}

void MessageWindow::showInfo(QString message, bool autohide, int durationInSeconds){
    
    setText(message);
    setIcon(QMessageBox::Information);
    
    QTimer timer;
    if(autohide){
        
        setText(
                QString(message)
                    .append("\n\n This Popup will be hidden in %1 seconds.")
                    .arg(durationInSeconds)
        );
        
        timer.setSingleShot(true);
        timer.setInterval(durationInSeconds * 1000);
        connect(&timer, SIGNAL(timeout()), this, SLOT(close()));
        timer.start();
    }
    
    exec();
}

