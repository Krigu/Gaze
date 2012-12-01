/*
 * GazeTracker.cpp
 *
 *  Created on: Nov 24, 2012
 *      Author: krigu
 */

#include "GazeTracker.hpp"
#include "../utils/geometry.hpp"
#include "../utils/gui.hpp"
#include "../utils/log.hpp"

//#if __DEBUG_FINDGLINTS == 1 || __DEBUG_TRACKER == 1
#include "opencv2/highgui/highgui.hpp"
//#endif

using namespace cv;

GazeTracker::GazeTracker(ImageSource & imageSource, TrackerCallback *callback) :
		imageSrc(imageSource), tracker_callback(callback), isRunning(false), isStopping(false), framenumber(
				0) {

}

bool GazeTracker::initialize(cv::Mat& frame, cv::Rect& frameRegion,
		cv::Point2f& frameCenter) {

	bool foundEye = false;
	short tries = 0;
	while (!foundEye) {
		foundEye = eyeFinder.findLeftEye(frame, frameRegion);
		tries++;

		if (tries > GazeConstants::HAAR_FINDREGION_MAX_TRIES)
			return false;
	}

	frameCenter = calcRectBarycenter(frameRegion);
	frame = frame(frameRegion);

	return true;
}

void GazeTracker::adjustRect(cv::Point2f& currentCenter, cv::Rect& frameRegion) {
	int width = frameRegion.width;
	int height = frameRegion.height;

	int x = frameRegion.x + currentCenter.x - (width / 2);
	int y = frameRegion.y + currentCenter.y - (height / 2);

	// Range check
	x = (x > 0) ? x : 0;
	y = (y > 0) ? y : 0;

	frameRegion = Rect(x, y, width, height);
}

GazeTracker::~GazeTracker() {
	// TODO Auto-generated destructor stub
}

bool GazeTracker::startTracking() {

	Mat currentFrame;
	Rect frameRegion;
	Point2f glintCenter;
	Point2f pupilCenter;
	Point darkPupilCenter;
	Point lastVector;
	Point currentVector;
	float radius;
	vector<cv::Point> glints;
	int frames;

	bool hasImage = imageSrc.nextGrayFrame(currentFrame);
	// TODO: return error?
	if (!hasImage) {
		LOG_W("No image");
		return false;
	}

	bool foundRegion = initialize(currentFrame, frameRegion, glintCenter);
	if (!foundRegion) {
		LOG_W("No region found");
		return false;
	}

	LOG_D(
			"glintCenter:" << glintCenter << "x: " << frameRegion.x << "y: " << frameRegion.y);


	int keycode = waitKey(50);
	if (keycode == 32) // space
		while (waitKey(100) != 32)
			;

	isRunning = true;
	int noGlints = 0;
	// main loop
	while (true) {
		frames++;
		// Get next frame
		if (!imageSrc.nextGrayFrame(currentFrame)) {
			LOG_D("No more frames");
			break;
		}

		// Adjust rect
		currentFrame = currentFrame(frameRegion);

		if (glintFinder.findGlints(currentFrame, glints, glintCenter)) {

			starburst.processImage(currentFrame, glints, glintCenter,
					pupilCenter, radius);

			circle(currentFrame, pupilCenter, radius, Scalar(255, 255, 255));

			adjustRect(glintCenter, frameRegion);

			// now calculate the gaze vector
			Point2f gaze_vec(glintCenter.x - pupilCenter.x,
					glintCenter.y - pupilCenter.y);
			Point2f smoothed_gace_vec;
			this->smoothSignal(gaze_vec, smoothed_gace_vec,
					this->last_gaze_vectors, framenumber);
			LOG_D("GazeVector: " << gaze_vec);
			LOG_D("SmoothedVector: " << smoothed_gace_vec);

			//c.printPoint(smoothed_gace_vec);

//#if __DEBUG_TRACKER == 1
			cross(currentFrame, glintCenter, 10);
			cross(currentFrame, pupilCenter, 5);
			imshow("Tracker", currentFrame);

			// notify our callback about the processed frames...
			if(this->tracker_callback != NULL)
				tracker_callback->imageProcessed(currentFrame);

//#endif
		} else {
			noGlints++;
			if (noGlints > 5) {
				LOG_W("no glints found. need to reinitialize");
				imageSrc.nextGrayFrame(currentFrame);
				initialize(currentFrame, frameRegion, glintCenter);

				noGlints = 0;
			}
		}

		int keycode = waitKey(50);
		if (keycode == 32) // space
			while (waitKey(100) != 32)
				;

		// TODO: check if it works
		while (!isRunning) {
			sleep(1);
		}

		if (isStopping)
			break;

		++framenumber;
	}
	return true;
}

void GazeTracker::pauseTacking() {
	isRunning = false;
}

void GazeTracker::stopTracking() {
	isStopping = true;
}

void GazeTracker::smoothSignal(Point2f &measured, Point2f &smoothed, Point2f data[],
		unsigned int framenumber) {
	if (framenumber < GazeConstants::NUM_OF_SMOOTHING_FRAMES) {
		// nothing to smooth here
		smoothed.x = measured.x;
		smoothed.y = measured.y;
	} else {
		smoothed.x = 0;
		smoothed.y = 0;
		for (unsigned short i = 0; i < GazeConstants::NUM_OF_SMOOTHING_FRAMES;
				++i) {
			smoothed.x += data[i].x;
			smoothed.y += data[i].y;
		}
		smoothed.x /= GazeConstants::NUM_OF_SMOOTHING_FRAMES;
		smoothed.y /= GazeConstants::NUM_OF_SMOOTHING_FRAMES;
	}
	data[framenumber % GazeConstants::NUM_OF_SMOOTHING_FRAMES] = measured;
}
