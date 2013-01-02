/* 
 * File:   ImageLinkLabel.cpp
 * Author: krigu
 * 
 * Created on January 2, 2013, 7:28 PM
 */

#include "ImageLinkLabel.h"

ImageLinkLabel::ImageLinkLabel(Link link) : link(link) {

    setPixmap(QPixmap::fromImage(link.image));
}

ImageLinkLabel::~ImageLinkLabel() {
}

void ImageLinkLabel::mousePressEvent(QMouseEvent * event) {
    emit clicked(link.href);
}
