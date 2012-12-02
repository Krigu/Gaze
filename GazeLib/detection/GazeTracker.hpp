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

#include "../GazeConstants.hpp"

class TrackerCallback{
public:
	virtual ~TrackerCallback() = 0;
	virtual void imageProcessed(Mat &result) = 0;
};

class GazeTracker {

public:
	enum MeasureResult{
		MEASURE_OK,
		FINDPUPIL_FAILED,
		FINDGLINT_FAILED,
	};

	GazeTracker(ImageSource & imageSource, TrackerCallback *callback=NULL);
	virtual ~GazeTracker();

	bool startTracking();

private:
	ImageSource& imageSrc;
	FindEyeRegion eyeFinder;
	FindGlints glintFinder;
	Starburst starburst;
	Calibration c;
	TrackerCallback *tracker_callback;
	Point2f last_gaze_vectors[GazeConstants::NUM_OF_SMOOTHING_FRAMES];
	bool isRunning;
	unsigned int framenumber;
	Rect frameRegion;

	void smoothSignal(Point2f &measured, Point2f &smoothed, Point2f data[], unsigned int framenumber);
	MeasureResult measureFrame(Mat &frame, Point2f &gazeVector);

protected:
	bool initialize(cv::Mat& frame, cv::Rect& frameRegion, cv::Point2f& frameCenter);
	void adjustRect(cv::Point2f& currentCenter, cv::Rect& frameRegion);

};

#endif /* GAZETRACKER_H_ */
