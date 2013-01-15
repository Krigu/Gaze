/*! \mainpage GazeLib API Documentation
 *
 * \section intro_sec Introduction
 *
 * This is the API Documentation of the GazeLib
 *
 * \section About GazeLib
 * GazeLib is an OpenCV based image processing library for building a Remote Gaze Tracker.
 * 
 * \subsection Algorithms
 * This Libraray contains algorithms for detecting the pupil (Starburst) and finding the glints inside the eye (FindGlints)<br/>
 * There are also classes for making a Gaze Tracking calibration and for calculating the point where
 * a user is looking at (Calibration).
 *  
 * \subsection Examples
 * The GazeLib is used in the proof of concept application Gaze-Browser. This applications allows webbrowsing with eye movements.
 */
#ifndef GAZETRACKER_H_
#define GAZETRACKER_H_

#include "opencv2/core/core.hpp"

#include "../video/ImageSource.hpp"
#include "../detection/eye/FindEyeRegion.hpp"
#include "../detection/glint/FindGlints.hpp"
#include "../detection/pupil/Starburst.hpp"
#include "../calibration/Calibration.hpp"

#include "../config/GazeConfig.hpp"

/**
 * the result of each measure
 */
enum MeasureResult {
    MEASURE_OK,
    FINDPUPIL_FAILED,
    FINDGLINT_FAILED,
};

/**
 * Clients of the GazeLib need to implement this interface to get measure 
 * results and live image data. The TrackerCallback can also control and abort
 * an active Gaze Tracking session
 */
class TrackerCallback {
public:
    virtual ~TrackerCallback(){
        
    };
    
    /**
     * ImageProcessed(Mat) is called, when the GazeLib has processed BUT NOT 
     * measured an image. This happens if the Library is for example searching
     * for an Eyeregion and therefore not measuring any GazeVector.
     * <br/> the return value of this method controls whether the GazeLib 
     * should abort or continue the tracking
     * 
     * @param resultImage the image that was processed by the GazeLib
     * @return true if the tracking should continue. false if the GazeLib should
     *  stop
     */
    virtual bool imageProcessed(Mat &resultImage) = 0;
    
    /**
     * If the GazeLib is tracking, for each processed frame 
     * imageProcessed(Mat, MeasureResult) is called. clients of the GazeLib can
     * use this method the get the measured GazeVector and to control whether
     * the tracking should be continued or stopped.
     * 
     * @param resultImage the processed image
     * @param result whether the last image could be processed
     * @param gazeVector the measured GazeVector if result==MEASURE_OK
     * @return true if the tracking should continue. false if the GazeLib should
     *  stop
     */
    virtual bool imageProcessed(Mat &resultImage, MeasureResult &result, Point2f &gazeVector) = 0;
};


/**
 * The GazeTracker class is the Entrypoint to the GazeLib. clients can use this 
 * class to start the Gaze Tracking. to productively use this class you need to
 * implement a TrackerCallback too.
 */
class GazeTracker {
public:
    
    /**
     * 
     * @param imageSource either a camera or a video source
     * @param callback the object that should be notified with the measure results
     */
    GazeTracker(ImageSource & imageSource, TrackerCallback *callback = NULL);
    virtual ~GazeTracker();

    /**
     * tracks the pupil and glints and calculates the GazeVector. you can either track 
     * infinitely or during a defined timespan. the tracking aborts if the face 
     * is not found for a longer period.
     * @param duration 0 or a tracking duration in seconds
     */
    void track(unsigned int duration=0);
    
    /**
     * this method blocks the callee and searches for a eye region in the camera sight.
     * useful for waiting until the tracking can be started
     */
    void initializeCalibration();

private:    
    
    ImageSource& imageSrc;
    FindGlints glintFinder;
    Starburst starburst;
    Calibration c;
    TrackerCallback *tracker_callback;
    Point2f last_gaze_vectors[GazeConfig::NUM_OF_SMOOTHING_FRAMES];
    bool isRunning;
    unsigned int framenumber;
    FindEyeRegion eyeFinder;
    Rect frameRegion;     

    void getNextFrame(Mat & frame);
    bool findEyeRegion(Mat & frame, cv::Point2f& frameCenter, bool calibrationMode = false);

    void smoothSignal(Point2f &measured, Point2f &smoothed, Point2f data[], unsigned int framenumber);
    MeasureResult measureFrame(Mat &frame, Point2f &gazeVector, Point2f glintCenter);



protected:
    void adjustRect(cv::Point2f& currentCenter, cv::Rect& frameRegion);



};

#endif /* GAZETRACKER_H_ */
