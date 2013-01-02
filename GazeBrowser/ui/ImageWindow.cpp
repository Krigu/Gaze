/* 
 * File:   ImageWindow.cpp
 * Author: krigu
 * 
 * Created on January 2, 2013, 4:53 PM
 */

#include <algorithm>
#include <QtGui>
#include <QtWebKit>

#include "ImageWindow.hpp"
#include "ImageLinkLabel.hpp"

using namespace std;

ImageWindow::ImageWindow(QWebView* webview, int imagesPerPage) : webview(webview), imagesPerPage(imagesPerPage), page(0) {

}

ImageWindow::~ImageWindow() {
}

void ImageWindow::addLink(Link link) {
    links.push_back(link);
}

void ImageWindow::showEvent(QShowEvent* event) {

    QWidget::showEvent(event);

    QGridLayout *generalLayout = new QGridLayout;

    int start = page * imagesPerPage;
    int amountOfLinks = links.size();
    int end = min(amountOfLinks, page * imagesPerPage + imagesPerPage);
    int linkCount = 0;

    for (int i = start; i < end; i++) {
        if (linkCount > amountOfLinks)
            break;

        ImageLinkLabel *myLabel = new ImageLinkLabel(links.at(i));
        connect(myLabel, SIGNAL(clicked(QString)), this, SLOT(imageLabelClicked(QString)));

        generalLayout->addWidget(myLabel, i / 3, i % 3, Qt::AlignCenter);

    }



    setLayout(generalLayout);

}

void ImageWindow::imageLabelClicked(QString href) {
    qDebug() << "Link: " << href;
    // Consider relative links
    if (href.startsWith("/")) {
        //href = href.mid(1);
        href = webview->url().host() + href;
    }
    // Consider anchors
    if (href.startsWith("#")) {
        href = webview->url().host() + href;
    }

    qDebug() << "Link: " << href;
    webview->load(QUrl::fromUserInput(href));
    close();
}
