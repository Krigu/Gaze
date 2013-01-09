/* 
 * File:   TCallback.hpp
 * Author: krigu
 *
 * Created on December 13, 2012, 9:55 PM
 */

#ifndef TCALLBACK_HPP
#define	TCALLBACK_HPP

#include "opencv2/core/core.hpp"
#include "detection/GazeTracker.hpp"
#include "calibration/Calibration.hpp"

class TCallback : public TrackerCallback{
public:
    TCallback();
    TCallback(const TCallback& orig);
    virtual ~TCallback();
    virtual bool imageProcessed(Mat &result);
    virtual bool imageProcessed(Mat &resultImage, MeasureResult &result, Point2f &gazeVector);
private:
    Calibration calib;
};

#endif	/* TCALLBACK_HPP */

