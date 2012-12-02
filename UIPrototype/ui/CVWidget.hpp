// found here: http://www.qtforum.de/forum/viewtopic.php?f=3&t=14591
#ifndef CVWIDGET_H
#define CVWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <opencv2/core/core.hpp>

class CVWidget : public QGLWidget
{
    Q_OBJECT
public:
    CVWidget(QWidget* parent=0);
    void setDisplayMode(int mode);
    void sendImage(cv::Mat *img);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    QImage qframe;
    int display_mode;
};

#endif // CVWIDGET_H