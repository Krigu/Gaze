/* 
 * File:   ImageWindow.h
 * Author: krigu
 *
 * Created on January 2, 2013, 4:53 PM
 */

#ifndef IMAGEWINDOW_H
#define	IMAGEWINDOW_H

#include <QWidget>

class QImage;
class QLabel;
class QImage;
class QVBoxLayout;
class QWebView;

struct Link;

class ImageWindow : public QWidget {
    Q_OBJECT

public:
    // imagesPerPage should be a    multiple of 3
    ImageWindow(QWebView* webview, int imagesPerPage = 9);
    virtual ~ImageWindow();

    void addLink(Link link);
public slots:
    void imageLabelClicked(QString str);
protected:
    void showEvent(QShowEvent* event);
private:
    QWebView* webview;
    int imagesPerPage;
    int page;
    
    
    std::vector<Link> links;

};

#endif	/* IMAGEWINDOW_H */

