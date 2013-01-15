#include <algorithm>
#include <QtGui>
#include <QtWebKit>

#include "ImageWindow.hpp"
#include "ImageLinkLabel.hpp"

using namespace std;

ImageWindow::ImageWindow(QWidget* parent, QWebView* webview, Qt::WindowFlags f) : QWidget(parent, f), webview(webview), imagesPerPage(12), page(0) {
    
    
    QWidget *background = new QWidget(parent, f);
    background->setStyleSheet("background-color: darkgray;");
    
    QGridLayout *generalLayout = new QGridLayout;
    generalLayout->setMargin(0);
   
    for (int i = 0; i < imagesPerPage; i++) {

        ImageLinkLabel *myLabel = new ImageLinkLabel();
        connect(myLabel, SIGNAL(clicked(QString)), this, SLOT(imageLabelClicked(QString)));

        generalLayout->addWidget(myLabel, i / 4, i % 4, Qt::AlignCenter);
    }    
    
    background->setLayout(generalLayout);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(background);
    setLayout(layout);

}

ImageWindow::~ImageWindow() {
}

void ImageWindow::addLink(Link link) {
    links.push_back(link);
}

int ImageWindow::linkSize(){
    return links.size();
}

void ImageWindow::clearLinks() {
     
   QList<ImageLinkLabel *> list = this->findChildren<ImageLinkLabel *> ();

    QList<ImageLinkLabel* >::iterator it;
    for (it = list.begin(); it != list.end(); it++) {
        (*it)->setPixmap(NULL);
    }
 
    links.clear();
    page = 0;
}

void ImageWindow::showEvent(QShowEvent* event) {

    QWidget::showEvent(event);
    
    displayLinks();
    
}

void ImageWindow::hideEvent( QHideEvent * event ){
    Q_UNUSED(event);
    emit hideWindow();
}

void ImageWindow::displayLinks(){

    int start = page * imagesPerPage;
    int amountOfLinks = links.size();
    int end = min(amountOfLinks, page * imagesPerPage + imagesPerPage);
    
    QList<ImageLinkLabel *> list = this->findChildren<ImageLinkLabel *> ();
    int j = 0;
    
    for (int i = start; i < end; i++, j++) {
        list.at(j)->setLink(links.at(i));
    }    
       
 
}

void ImageWindow::imageLabelClicked(QString href) {
    qDebug() << "Link: " << href;
    // Consider relative links
    if (href.startsWith("/")) {
        //href = href.mid(1);
        href = webview->url().host() + href;
    }// Consider anchors
    else if (href.startsWith("#")) {
        href = webview->url().host() + "/" + webview->url().path() + href;
    } else if (!href.startsWith("http")) {
        href = webview->url().host() + "/" + href;
    }

    qDebug() << "Link: " << href;
    webview->load(QUrl::fromUserInput(href));

    close();
}

void ImageWindow::forward() {

    int maxPages = (links.size() + imagesPerPage - 1) / imagesPerPage;
    page = min(page + 1, maxPages - 1);
    displayLinks();
}

void ImageWindow::back() {

    page = max(0, page - 1);
    displayLinks();
}

void ImageWindow::openLink(cv::Point p) {
    int w = size().width();
    int h = size().height();

    // get link from point    
    unsigned int index = page * imagesPerPage + (p.y / (h / 3) * 4) + p.x / (w / 4);

    if (index < links.size())
        imageLabelClicked(links.at(index).href);
}

void ImageWindow::closeWindow(cv::Point) {

    close();
}