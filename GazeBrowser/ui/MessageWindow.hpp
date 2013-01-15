#ifndef MESSAGEWINDOW_HPP
#define	MESSAGEWINDOW_HPP

#include <QApplication>

/**
 * a messagebox for displaying error or info messages
 */
class MessageWindow : public QMessageBox{
public:
    MessageWindow();
    
    /**
     * shows an error message
     * @param message the error message
     */
    void showException(QString message);
    
    /**
     * show an info message
     * @param message the message to dispaly 
     * @param autohide should the popup be hidden automatically?
     * @param durationInSeconds how long should the popup be displayed?
     */
    void showInfo(QString message, bool autohide, int durationInSeconds);
private:

};

#endif	/* MESSAGEWINDOW_HPP */

