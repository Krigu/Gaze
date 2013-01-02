/* 
 * File:   ImageLinkLabel.cpp
 * Author: krigu
 * 
 * Created on January 2, 2013, 7:28 PM
 */

#include <QtCore/qglobal.h>

#include "ImageLinkLabel.hpp"

ImageLinkLabel::ImageLinkLabel(Link link) : link(link) {

    setPixmap(QPixmap::fromImage(link.image));
}

ImageLinkLabel::~ImageLinkLabel() {
}

void ImageLinkLabel::mousePressEvent(QMouseEvent * event) {
    Q_UNUSED(event);
    emit clicked(link.href);
}
