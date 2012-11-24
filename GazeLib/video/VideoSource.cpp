/*
 * VideoSource.cpp
 *
 *  Created on: Nov 24, 2012
 *      Author: krigu
 */

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "VideoSource.hpp"

using namespace std;
using namespace cv;

VideoSource::VideoSource(string videoPath) {
	// TODO: proper implemented?
	videoCapture = VideoCapture(videoPath);
}

bool VideoSource::nextGrayFrame(cv::Mat img) {
	// Check if capture is open
	if (!videoCapture.isOpened())
		return false;

	// Check if there are more frames
	if (!videoCapture.read(img))
		return false;

	 // Check for invalid input
	if (!img.data)
		return false;

	// Convert to grayscale
	cvtColor(img, img, CV_BGR2GRAY);

	return true;
}
