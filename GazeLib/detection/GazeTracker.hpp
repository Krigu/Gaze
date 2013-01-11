/*
 * GazeTracker.h
 *
 *  Created on: Nov 24, 2012
 *      Author: krigu
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

enum MeasureResult {
    MEASURE_OK,
    FINDPUPIL_FAILED,
    FINDGLINT_FAILED,
};

class TrackerCallback {
public:
    virtual ~TrackerCallback(){
        
    };
    virtual bool imageProcessed(Mat &resultImage) = 0;
    virtual bool imageProcessed(Mat &resultImage, MeasureResult &result, Point2f &gazeVector) = 0;
};

class GazeTracker {
public:
    GazeTracker(ImageSource & imageSource, TrackerCallback *callback = NULL);
    virtual ~GazeTracker();

    void track(unsigned int duration=0);
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
