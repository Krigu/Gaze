#ifndef CALIBRATIONTHREAD_HPP_
#define CALIBRATIONTHREAD_HPP_

#include <QtCore>
#include "video/ImageSource.hpp"
#include "detection/GazeTracker.hpp"

#include "../StateMachineDefinition.hpp"

class Calibration;
class GazeTracker;

/**
 * the GazeTrackWorker is the application logic for a QThread. this thread is 
 * responsible for tracking and the calibration. <br />
 * <u>Calibration</u><br />
 * when calibrating the Thread emits javascript commands to the webview. this 
 * way it can display points at the screen. 
 * <br /><u>Tracking</u><br />
 * when tracking this thread calculates for each frame the point on the screen
 * and emits this estimated point. WHEN TRACKING NO MEASURED IMAGES ARE DISPLAYED 
 * (for speeding up the processing)
 */
class GazeTrackWorker : public QObject, public TrackerCallback {
    Q_OBJECT

private:
    int width;
    int height;
    ImageSource *camera;
    QMutex *cameraLock;
    vector<Point2f> measurements;
    bool running;
    bool tracking;
    PROGRAM_STATES nextStateAfterStop;
    GazeTracker *tracker;
    Calibration *calibration;
    
    bool calibrate();
   

public:
    GazeTrackWorker(int width, int height, ImageSource *camera, QMutex *cameraLock);
    virtual ~GazeTrackWorker();
    
    /**
     * callback function for the GazeLib integration. this method emits the processed image
     * to the CVWidget for displaying it
     * @param resultImage
     * @return 
     */
    bool imageProcessed(Mat& resultImage);
    
    /**
     * the second callback function for the GazeLib integration. this method stores
     * the measured Gaze Vectors and calculates the points on the screen.
     * @param resultImage
     * @param result
     * @param gazeVector
     * @return 
     */
    bool imageProcessed(Mat &resultImage, MeasureResult &result, Point2f &gazeVector);
    
    
public slots:
    /**
     * starts the calibration and deletes any existing calibrationdata
     */
    void startCalibration(void);
    
    /**
     * starts the gaze tracking or emits an error if not yet calibrated
     */
    void startTracking(void);
    
    /**
     * stops the tracking and notifies the ThreadManager when the thread has finished
     * @param nextState
     */
    void stop(PROGRAM_STATES nextState);
    
    /**
     * obvious, isn't it?
     * @return true if calibrated
     */
    bool isCalibrated();

    
signals:
    void jsCommand(QString);
    void error(QString);
    void info(QString);
    void calibrationFinished();
    void cvImage(cv::Mat*);
    void hasStopped(PROGRAM_STATES);
    void estimatedPoint(cv::Point);

};
#endif