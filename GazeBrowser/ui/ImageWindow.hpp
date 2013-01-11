/* 
 * File:   ImageWindow.h
 * Author: krigu
 *
 * Created on January 2, 2013, 4:53 PM
 */

#ifndef IMAGEWINDOW_H
#define	IMAGEWINDOW_H

#include <QWidget>
#include <opencv2/core/core.hpp>

class QImage;
class QLabel;
class QImage;
class QVBoxLayout;
class QWebView;

struct Link;

class ImageWindow : public QWidget {
    Q_OBJECT

public:
    ImageWindow(QWidget* parent, QWebView* webview, Qt::WindowFlags f);
    virtual ~ImageWindow();

    void addLink(Link link);
    void clearLinks();

    void forward();
    void back();
    void openLink(cv::Point);
public slots:
    void imageLabelClicked(QString str);
    void closeWindow(cv::Point);


signals:
    void hideWindow();


protected:
    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);
    void displayLinks();
private:
    QWebView* webview;
    int imagesPerPage;
    int page;

    std::vector<Link> links;

};

#endif	/* IMAGEWINDOW_H */

