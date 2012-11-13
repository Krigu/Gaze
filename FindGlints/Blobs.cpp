/*
 * Blobs.cpp
 *
 *  Created on: Nov 11, 2012
 *      Author: krigu
 */

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
// TODO: entfernen
#include "opencv2/highgui/highgui.hpp"

#include "Blobs.hpp"
#include "GazeConstants.hpp"
#include "utils/log.hpp"

using namespace std;
using namespace cv;

Blobs::Blobs(std::vector<std::vector<cv::Point> > & contours) {

	// Add all blobs to vector
	for (int i = 0; i < contours.size(); i++) {

		Moments m = moments(contours[i], false);

		Blob b;
		b.centerX = m.m10 / m.m00;
		b.centerY = m.m01 / m.m00;
		b.size = m.m00;

		blobs.push_back(b);
	}
}

Blobs::~Blobs() {

}

void Blobs::removeInvalidSize() {

	std::vector<Blob>::iterator iter;
	for (iter = blobs.begin(); iter != blobs.end();) {
		if (iter->size < GazeConstants::GLINT_MIN_PIXEL
				|| iter->size > GazeConstants::GLINT_MAX_PIXEL)
			iter = blobs.erase(iter);
		else
			++iter;
	}
}

void Blobs::removeInvalidShape() {
	std::vector<Blob>::iterator iter;
	for (iter = blobs.begin(); iter != blobs.end();) {
		if (iter->centerX == 0 || iter->centerY == 0)
			iter = blobs.erase(iter);
		else
			++iter;
	}
}

void Blobs::blobCenters(std::vector<cv::Point> & points) {
	std::vector<Blob>::iterator iter;
	for (iter = blobs.begin(); iter != blobs.end(); ++iter) {
		Point p(iter->centerX, iter->centerY);
		points.push_back(p);
	}
}
