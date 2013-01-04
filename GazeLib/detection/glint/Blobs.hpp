/*
 * Blobs.hpp
 *
 *  Created on: Nov 11, 2012
 *      Author: krigu
 */

#ifndef BLOBS_HPP_
#define BLOBS_HPP_

struct Blob {
	// Size in pixels
	int size;
	// Center of gravity X
	int centerX;
	// Center of gravity Y
	int centerY;
};

class Blobs {
private:
	std::vector<Blob> blobs;
public:
	Blobs(std::vector<std::vector<cv::Point> > & contours);

	void blobCenters(std::vector<cv::Point> & points);


};

#endif /* BLOBS_HPP_ */
