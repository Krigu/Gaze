/*
 * LiveSource.cpp
 *
 *  Created on: Dec 2, 2012
 *      Author: krigu
 */

#include "LiveSource.hpp"

using namespace std;
using namespace cv;

LiveSource::LiveSource() {
	videoCapture = VideoCapture(0);
}

LiveSource::LiveSource(int channel) {
	videoCapture = VideoCapture(channel);
}

LiveSource::~LiveSource() {
	videoCapture.release();
}

bool LiveSource::nextGrayFrame(cv::Mat& frame) {
	// Check if capture is open
	if (!videoCapture.isOpened())
		return false;

	// Check if there are more frames
	if (!videoCapture.read(frame))
		return false;

	// Check for invalid input
	if (!frame.data)
		return false;

	// Convert to grayscale
	cvtColor(frame, frame, CV_BGR2GRAY);

	return true;
}
