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

/**
 * The ImageWindow can display a list of links from the WebView. 
 * the links are displayed in three rows and four columns. this way a link can
 * be opened easyer by gaze interaction than when the link is displayed inside
 * a webpage.
 * 
 */
class ImageWindow : public QWidget {
    Q_OBJECT

public:
    ImageWindow(QWidget* parent, QWebView* webview, Qt::WindowFlags f);
    virtual ~ImageWindow();

    
    void addLink(Link link);
    void clearLinks();
    
    /**
     * 
     * @return the number of link in the current window
     */
    int linkSize();

    /**
     * move forward and show the next twelve links on the website
     */
    void forward();
    
    /**
     * move backward and show the previous 12 links
     */
    void back();
    
    /**
     * opens the link at the given position. 
     * @param the position where the user is looking at
     */
    void openLink(cv::Point);
public slots:
    /**
     * opens the clicked link
     * @param str url
     */
    void imageLabelClicked(QString str);
    
    /**
     * closes the image window
     * @param 
     */
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

