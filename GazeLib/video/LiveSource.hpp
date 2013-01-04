/*
 * LiveSource.h
 *
 *  Created on: Dec 2, 2012
 *      Author: krigu
 */

#ifndef LIVESOURCE_H_
#define LIVESOURCE_H_

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "ImageSource.hpp"

class LiveSource : public ImageSource {
private:
    cv::VideoCapture *videoCapture;
protected:
    void init();
public:
    LiveSource();
    LiveSource(int channel);
    virtual ~LiveSource();
    virtual bool nextGrayFrame(cv::Mat& frame);
};

#endif /* LIVESOURCE_H_ */
