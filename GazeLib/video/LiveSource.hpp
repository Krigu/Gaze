#ifndef LIVESOURCE_H_
#define LIVESOURCE_H_

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "ImageSource.hpp"

/**
 * the LiveSource class provides access to a hardware device. per default 
 * a LiveSource open the camera at channel 0
 */
class LiveSource : public ImageSource {
private:
    cv::VideoCapture *videoCapture;
protected:
    void init();
public:
    /**
     * creates a LiveSource and opens the camera at channel 0
     */
    LiveSource();
    
    /**
     * creates a LiveSource and opens the camera at the given channel
     * @param channel the channel to use
     */
    LiveSource(int channel);
    virtual ~LiveSource();
    virtual bool nextGrayFrame(cv::Mat& frame);
};

#endif /* LIVESOURCE_H_ */
