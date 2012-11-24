/*
 * VideoSource.h
 *
 *  Created on: Nov 24, 2012
 *      Author: krigu
 */

#ifndef VIDEOSOURCE_H_
#define VIDEOSOURCE_H_

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "ImageSource.hpp"

namespace std {

class VideoSource: public ImageSource {
private:
	cv::VideoCapture videoCapture;
public:
	VideoSource(string videoPath);
	virtual bool nextGrayFrame(cv::Mat img);
};

}
#endif /* VIDEOSOURCE_H_ */
