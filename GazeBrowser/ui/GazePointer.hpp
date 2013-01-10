/* 
 * File:   GazePointer.hpp
 * Author: fri
 *
 * Created on January 10, 2013, 3:36 PM
 */

#ifndef GAZEPOINTER_HPP
#define	GAZEPOINTER_HPP

#include <QtGui>
#include <opencv2/core/core.hpp>

class GazePointer : public QWidget{

Q_OBJECT

public:
    GazePointer(QWidget *parent=0, Qt::WindowFlags f=0);
    GazePointer(const GazePointer& orig);
    QSize sizeHint() const;
    virtual ~GazePointer();

public slots:
    void commitAction(cv::Point p);
    void prepareAction(cv::Point p,int hitCount,int commitCount);
    void abortAction();

    protected:      
        void paintEvent(QPaintEvent *event);
        void resizeEvent(QResizeEvent *event);
    
    private:
        int percentage;

};

#endif	/* GAZEPOINTER_HPP */

