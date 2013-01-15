#include <iostream>
#include <QtGui/qwidget.h>

#include "GazePointer.hpp"

GazePointer::GazePointer(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f) {
    resize(75, 75);
}

GazePointer::~GazePointer() {
}

void GazePointer::commitAction(cv::Point p) {
    Q_UNUSED(p);
    hide();
    repaint();
}

void GazePointer::prepareAction(cv::Point p, int percentage) {
    this->percentage = percentage;
    percentage = percentage % 100;

    int x = p.x - this->size().width() / 2;
    int y = p.y - this->size().height() / 2;

    move(x, y);
    show();
    repaint();
}

void GazePointer::abortAction() {
    hide();
    repaint();
}

void GazePointer::paintEvent(QPaintEvent *) {
    QColor color(0, 127, 127, 191);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(0, 0);
    //painter.scale(side / 75.0, side / 75.0);

//    painter.rotate(-90);
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);

    painter.setPen(color);

    QRectF rectangle(0, 0, 75, 75);
    painter.drawPie(rectangle, 0, -percentage * 3.6 * 16);
}

void GazePointer::resizeEvent(QResizeEvent * /* event */) {
//    int side = qMin(width(), height());
//    QRegion maskedRegion(width() / 2 - side / 2, height() / 2 - side / 2, side,
//            side, QRegion::Ellipse);
//    setMask(maskedRegion);
}

QSize GazePointer::sizeHint() const {
    return QSize(75, 75);
}