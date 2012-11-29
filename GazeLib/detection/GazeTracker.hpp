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

class GazeTracker {
private:
	ImageSource& imageSrc;
	FindEyeRegion eyeFinder;
	FindGlints glintFinder;
	Starburst starburst;
	Calibration c;

	Point last_pupil_centers[GazeConstants::NUM_OF_SMOOTHING_FRAMES];
	Point last_glint_centers[GazeConstants::NUM_OF_SMOOTHING_FRAMES];

	bool isRunning;
	bool isStopping;

	void smoothSignal(Point &measured, Point &smoothed, Point data[], unsigned int framenumber);

	unsigned int framenumber;

protected:
	bool initialize(cv::Mat& frame, cv::Rect& frameRegion, cv::Point& frameCenter);
	void adjustRect(cv::Point& currentCenter, cv::Rect& frameRegion);

public:
	GazeTracker(ImageSource & imageSource);
	virtual ~GazeTracker();

	bool startTracking();
	void pauseTacking();
	void stopTracking();
};

#endif /* GAZETRACKER_H_ */
