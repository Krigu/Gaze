/* 
 * File:   MessageWindow.cpp
 * Author: krigu
 * 
 * Created on December 7, 2012, 4:07 PM
 */

#include <QObject>
#include <QMessageBox>

#include "MessageWindow.hpp"

MessageWindow::MessageWindow() {
}

void MessageWindow::showException(QString message) {

    QMessageBox::critical(this, tr("Critical error"), message);

}

