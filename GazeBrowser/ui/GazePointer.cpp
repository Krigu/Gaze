/* 
 * File:   GazePointer.cpp
 * Author: fri
 * 
 * Created on January 10, 2013, 3:36 PM
 */

#include <iostream>
#include <QtGui/qwidget.h>

#include "GazePointer.hpp"

GazePointer::GazePointer(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f) {
    resize(150,150);
}

GazePointer::GazePointer(const GazePointer& orig) : QWidget(){
    
}

GazePointer::~GazePointer() {
}


void GazePointer::commitAction(cv::Point p){
    Q_UNUSED(p);
    hide();
}

void GazePointer::prepareAction(cv::Point p,int percentage){
    //TODO: calculate percentage
    this->percentage = percentage;
    percentage = percentage % 100;
    
    move(p.x,p.y);
    show();
    repaint();
}

void GazePointer::abortAction(){
    hide();
}

 void GazePointer::paintEvent(QPaintEvent *)
 {
     QColor color(0, 127, 127, 191);
     int side = qMin(width(), height());

     QPainter painter(this);
     painter.setRenderHint(QPainter::Antialiasing);
     painter.translate(width() / 2, height() / 2);
     painter.scale(side / 200.0, side / 200.0);

     painter.rotate(-90);
     painter.setPen(Qt::NoPen);
     painter.setBrush(color);

     painter.setPen(color);

     QRectF rectangle(0, 0, side/2, side/2);
     painter.drawPie(rectangle, 0, -percentage * 3.6 * 16);
 }
 
  void GazePointer::resizeEvent(QResizeEvent * /* event */)
 {
     int side = qMin(width(), height());
     QRegion maskedRegion(width() / 2 - side / 2, height() / 2 - side / 2, side,
                          side, QRegion::Ellipse);
     setMask(maskedRegion);
 }
  
   QSize GazePointer::sizeHint() const
 {
     return QSize(150, 150);
 }