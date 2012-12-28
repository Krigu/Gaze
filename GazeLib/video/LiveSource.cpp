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
    videoCapture = new VideoCapture(0);
    // Set resolution for Microsoft Life cam
    videoCapture->set(CV_CAP_PROP_FRAME_WIDTH, 1280);
    videoCapture->set(CV_CAP_PROP_FRAME_HEIGHT, 720);
}

LiveSource::LiveSource(int channel) {
    videoCapture = new VideoCapture(channel);
}

LiveSource::~LiveSource() {
    videoCapture->release();
    delete videoCapture;
}

bool LiveSource::nextGrayFrame(cv::Mat& frame) {

    //    waitKey(10);

    // Check if capture is open
    if (!videoCapture->isOpened())
        return false;

    // Check if there are more frames
    if (!videoCapture->read(frame))
        return false;

    // Check for invalid input
    if (!frame.data)
        return false;

    // Convert to grayscale
    cvtColor(frame, frame, CV_BGR2GRAY);

    return true;
}
