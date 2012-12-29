/*
 * Blobs.cpp
 *
 *  Created on: Nov 11, 2012
 *      Author: krigu
 */

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "Blobs.hpp"
#include "../../config/GazeConfig.hpp"
#include "../../utils/log.hpp"

using namespace std;
using namespace cv;

Blobs::Blobs(std::vector<std::vector<cv::Point> > & contours) {

    // Add all blobs to vector and calculate moments
    for (unsigned int i = 0; i < contours.size(); i++) {

        Moments m = moments(contours[i], false);

        Blob b;
        b.centerX = m.m10 / m.m00;
        b.centerY = m.m01 / m.m00;
        b.size = m.m00;

        //TODO: check if good idea
        if (b.centerX > 0 && b.centerY > 0 && b.size > 0)
            blobs.push_back(b);
    }
}

/**
 * Removes all blobs which don't have a roundish form
 */
void Blobs::removeInvalidShape() {
    std::vector<Blob>::iterator iter;
    for (iter = blobs.begin(); iter != blobs.end();) {
        if (iter->centerX == 0 || iter->centerY == 0)
            iter = blobs.erase(iter);
        else
            ++iter;
    }
}

/**
 * Returns a vector with all blob centers
 */
void Blobs::blobCenters(std::vector<cv::Point> & points) {
    // TODO: check if ok
    points.clear();

    std::vector<Blob>::iterator iter;
    for (iter = blobs.begin(); iter != blobs.end(); ++iter) {
        Point p(iter->centerX, iter->centerY);
        points.push_back(p);
    }
}

int Blobs::blobSize() {
    return blobs.size();
}
