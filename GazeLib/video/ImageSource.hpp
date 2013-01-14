#ifndef IVIDEOSOURCE_HPP_
#define IVIDEOSOURCE_HPP_

#include "opencv2/core/core.hpp"


/**
 * ImageSource is the API for accessing video data from an video or camera device.
 * <br />in client applications you should use the implementations LiveSource and VideoSource
 * @see LiveSource
 * @see VideoSource
 */
class ImageSource {

public:
	virtual ~ImageSource() {}
    /**
     * grabs the next frame in the ImageSource and converts it into 
     * a grascale image
     * 
     * @param frame the next frame in this ImageSource
     * @return 
     */
	virtual bool nextGrayFrame(cv::Mat& frame) = 0;
};

#endif /* IVIDEOSOURCE_HPP_ */
