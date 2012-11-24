/*
 * IVideoSource.hpp
 *
 *  Created on: Nov 24, 2012
 *      Author: krigu
 */

#ifndef IVIDEOSOURCE_HPP_
#define IVIDEOSOURCE_HPP_

#include "opencv2/core/core.hpp"

class ImageSource {

public:
	virtual ~ImageSource() {}
	virtual bool nextGrayFrame(cv::Mat& frame) = 0;
};

#endif /* IVIDEOSOURCE_HPP_ */
