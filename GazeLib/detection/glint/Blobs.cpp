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

        if (b.centerX > 0 && b.centerY > 0 && b.size > 0)
            blobs.push_back(b);
    }
}

void Blobs::blobCenters(std::vector<cv::Point> & points) {
    points.clear();

    std::vector<Blob>::iterator iter;
    for (iter = blobs.begin(); iter != blobs.end(); ++iter) {
        Point p(iter->centerX, iter->centerY);
        points.push_back(p);
    }
}