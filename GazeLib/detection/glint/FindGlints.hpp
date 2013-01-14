#ifndef FINDGLINTS_HPP_
#define FINDGLINTS_HPP_

#include <string>

#include <opencv2/objdetect/objdetect.hpp>

#include "GlintCluster.hpp"

using namespace std;

/**
 * This class tries to find 4 rectangularly aligned glints
 * 
 */
class FindGlints {
private:
    cv::Mat distanceMatrix(vector<cv::Point>& glintCenter);
    void findClusters(vector<cv::Point>& blobs, vector<GlintCluster>& clusters,
            cv::Point2f& lastMeasurement);


public:
    /**
     * 
     * Tries to find 4 rectangularly aligned glints
     * in an image.
     * 
     * @param image 
     * @param glintCenters  vector in which the 4 glint centers are pushed
     * @param lastMeasurement the position of the glint from the last frame
     * @return true if glints can be found, false otherwise
     */
    bool findGlints(cv::Mat & image, vector<cv::Point>& glintCenters,
            cv::Point2f& lastMeasurement);
    /**
     * Tries to find 4  rectangularly aligned glints in a list of glints 
     * 
     * @param glints
     * @return true if 
     */
    bool findRectangularCluster(vector<cv::Point>& glints);
};

#endif /* FINDGLINTS_HPP_ */
