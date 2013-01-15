#ifndef GAZEPOINTER_HPP
#define	GAZEPOINTER_HPP

#include <QtGui>
#include <opencv2/core/core.hpp>

/**
 * The GazePointer draws a circle (or pie) at the position where the user is looking at.
 * this class can visualize the calculated gaze position and give the user a 
 * feedback about the tracking.
 */
class GazePointer : public QWidget {
    Q_OBJECT

public:
    GazePointer(QWidget *parent = 0, Qt::WindowFlags f = 0);
    QSize sizeHint() const;
    virtual ~GazePointer();

    /**
     * hides the GazePointer (because the action is executed)
     */
    void commitAction(cv::Point p);
    
    /**
     * draws a pie at the given position p. the percentage controls how 
     * much of the pie is filled.
     * @param p
     * @param percentage
     */
    void prepareAction(cv::Point p, int percentage);
    
    /**
     * hides the circle/pie
     */
    void abortAction();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    int percentage;

};

#endif	/* GAZEPOINTER_HPP */

