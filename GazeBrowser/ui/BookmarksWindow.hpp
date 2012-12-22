/* 
 * File:   BookmarksWindow.hpp
 * Author: krigu
 *
 * Created on December 22, 2012, 3:54 PM
 */

#ifndef BOOKMARKSWINDOW_HPP
#define	BOOKMARKSWINDOW_HPP

#include <QWidget>
#include <QSettings>

class QGroupBox;
class QLabel;
class QLineEdit;
class QDialogButtonBox;

class BookmarksWindow : public QWidget {
    Q_OBJECT
public:
    BookmarksWindow(QSettings* settings);
    virtual ~BookmarksWindow();
protected:
    QGroupBox * createLinkGroup(int index);
    QDialogButtonBox * createButtonGroup();
    
     
private:
    QSettings* settings;
    
protected slots:
     void accept();
     void reject();    

};

#endif	/* BOOKMARKSWINDOW_HPP */

