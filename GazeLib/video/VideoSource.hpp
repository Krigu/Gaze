#ifndef VIDEOSOURCE_H_
#define VIDEOSOURCE_H_

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "ImageSource.hpp"

/**
 * The VideoSource class provides acces to images within an video file.
 * this class can be used to mock/test the GazeLib algorithms
 */
class VideoSource: public ImageSource {
private:
	cv::VideoCapture *videoCapture;
public:
    /**
     * creates a VideoSource and opens the file at the given path
     * @param videoPath
     */
	VideoSource(std::string videoPath);
	~VideoSource();
	virtual bool nextGrayFrame(cv::Mat& frame);
};


#endif /* VIDEOSOURCE_H_ */
