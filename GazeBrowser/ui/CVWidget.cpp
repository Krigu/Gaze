// found here: http://www.qtforum.de/forum/viewtopic.php?f=3&t=14591
#include "CVWidget.hpp"

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif


CVWidget::CVWidget() : QGLWidget(QGLFormat(QGL::SampleBuffers)) {
    setMinimumSize(90,90);
    this->display_mode = 0;
}

void CVWidget::initializeGL() {
    glClearColor(0.0f,0.0f,0.0f,1.0f);
}
void CVWidget::paintGL() {
    glClear (GL_COLOR_BUFFER_BIT);
    glClearColor (0.0,0.0,0.0,1.0);

    if (!qframe.isNull()) { // glDrawPixels mode selected ...
        qframe = qframe.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        if (this->display_mode == 0) glDrawPixels(qframe.width(),qframe.height(), GL_RGBA, GL_UNSIGNED_BYTE, qframe.bits());
        else { // 2D texture mapping mode selected ...
            glDisable(GL_DEPTH_TEST);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(0,qframe.width(),qframe.height(),0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glEnable(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe.width(), qframe.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe.bits() );
            glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex2f(0,qframe.height());
            glTexCoord2f(0,1); glVertex2f(0,0);
            glTexCoord2f(1,1); glVertex2f(qframe.width(),0);
            glTexCoord2f(1,0); glVertex2f(qframe.width(),qframe.height());
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
        glFlush();
    }
}

void CVWidget::resizeGL(int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode (GL_MODELVIEW);
    // qDebug() << "Resizing...";
}

void CVWidget::sendImage(cv::Mat* img) {
    if (img->channels() > 1)
        qframe = QImage((const unsigned char*)(img->data), img->cols, img->rows, img->step, QImage::Format_RGB888).rgbSwapped();
    else
        qframe = QImage((const unsigned char*)(img->data), img->cols, img->rows, img->step, QImage::Format_Indexed8).rgbSwapped();
    qframe = QGLWidget::convertToGLFormat(qframe);
    this->updateGL();
}

void CVWidget::setDisplayMode(int mode) {
    this->display_mode = mode;
}