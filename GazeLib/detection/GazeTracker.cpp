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

GazeTracker::GazeTracker(ImageSource & imageSource) :
		imageSrc(imageSource), isRunning(false), isStopping(false), framenumber(
				0) {

}

bool GazeTracker::initialize(cv::Mat& frame, cv::Rect& frameRegion,
		cv::Point& frameCenter) {

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

void GazeTracker::adjustRect(cv::Point& currentCenter, cv::Rect& frameRegion) {
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
	Point glintCenter;
	Point pupilCenter;
	Point darkPupilCenter;
	Point lastVector;
	Point currentVector;
	float radius;
	vector<cv::Point> glints;
	int frames;

	bool hasImage = imageSrc.nextGrayFrame(currentFrame);
	// TODO: return error?
	if (!hasImage) {
		LOG_D("No image");
		return false;
	}

	bool foundRegion = initialize(currentFrame, frameRegion, glintCenter);
	if (!foundRegion) {
		LOG_D("No region found");
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

		Mat orig = currentFrame.clone();
#if __DEBUG_FINDGLINTS == 1
		rectangle(orig, frameRegion, Scalar(255, 255, 255), 1);
		imshow("Search region", orig);
#endif

		// Adjust rect
		currentFrame = currentFrame(frameRegion);

		if (glintFinder.findGlints(currentFrame, glints, glintCenter)) {

			//TODO: worst fix ever,
			for (vector<Point>::iterator it = glints.begin();
					it != glints.end(); ++it) {

				// the glint_centers are relative to the search region...
				it->x = it->x + frameRegion.x;
				it->y = it->y + frameRegion.y;

			}
			Point absoluteGlintCenter(glintCenter.x + frameRegion.x,
					glintCenter.y + frameRegion.y);
			//Point smoothedGlintCenter;
			LOG_D(
					"Before starburst. Absolute Glintcenter: " << absoluteGlintCenter);

			// smooth the measured signal
			//this->smoothSignal(absoluteGlintCenter, smoothedGlintCenter, this->last_glint_centers, framenumber);
			//LOG_D("GlintCenter: " << absoluteGlintCenter << " Smoothed: " << smoothedGlintCenter);
			//absoluteGlintCenter = smoothedGlintCenter; // TODO two points only for debugging

			starburst.processImage(orig, glints, absoluteGlintCenter,
					pupilCenter, radius);

			//Point smoothedPupilCenter;
			//this->smoothSignal(pupilCenter, smoothedPupilCenter, this->last_pupil_centers, framenumber);
			//LOG_D("PupilCenter: " << pupilCenter << " Smoothed: " << smoothedPupilCenter);
			//pupilCenter = smoothedPupilCenter; // TODO two points only for debugging

			circle(orig, pupilCenter, radius, Scalar(255, 255, 255));

			adjustRect(glintCenter, frameRegion);

			// now calculate the gaze vector
			Point gaze_vec(absoluteGlintCenter.x - pupilCenter.x,
					absoluteGlintCenter.y - pupilCenter.y);
			Point smoothed_gace_vec;
			this->smoothSignal(gaze_vec, smoothed_gace_vec,
					this->last_pupil_centers, framenumber);
			LOG_D("GazeVector: " << gaze_vec);
			LOG_D("SmoothedVector: " << smoothed_gace_vec);

			//c.printPoint(smoothed_gace_vec);

//#if __DEBUG_TRACKER == 1
			cross(orig, absoluteGlintCenter, 10);
			cross(orig, pupilCenter, 5);
			imshow("Tracker", orig);
//#endif
		} else {
			noGlints++;
			if (noGlints > 5) {
				imageSrc.nextGrayFrame(currentFrame);
				initialize(currentFrame, frameRegion, glintCenter);

				noGlints = 0;
			}
		}

		int keycode = waitKey(50);
		if (keycode == 32) // space
			while (waitKey(100) != 32)
				;

#if __DEBUG_FINDGLINTS == 1
		cross(currentFrame, frameCenter, 5);
		imshow("After calib", currentFrame);

		// check the key and add some busy waiting
		int keycode = waitKey(100);
		if (keycode == 32)// space
		while (waitKey(100) != 32)
		;
#endif

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

void GazeTracker::smoothSignal(Point &measured, Point &smoothed, Point data[],
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
