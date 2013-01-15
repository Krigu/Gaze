#ifndef BOOKMARKSWINDOW_HPP
#define	BOOKMARKSWINDOW_HPP

#include <QWidget>
#include <QSettings>

class QGroupBox;
class QLabel;
class QLineEdit;
class QDialogButtonBox;

/**
 * The BookmarksWindow can be used to set the Browser Favorites
 */
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
     void save();
     void cancel();    

};

#endif	/* BOOKMARKSWINDOW_HPP */

