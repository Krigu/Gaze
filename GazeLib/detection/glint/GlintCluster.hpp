#ifndef GLINTCLUSTER_HPP_
#define GLINTCLUSTER_HPP_

#include <opencv2/core/core.hpp>

/**
 * An instance of this class represents a quadruple of 4 glints
 */
class GlintCluster {
private:
    std::vector<cv::Point> glints;
    int distanceToLastMeasurement;
    int clusterWidth;

public:
    /**
     * 
     * @param glints list of (4) glints
     * @param lastMeasurement the glintcenter of the last frame
     */
    GlintCluster(std::vector<cv::Point> & glints, cv::Point lastMeasurement);

    /**
     * Get distance between center of all glints to last center
     * 
     * @return the distance between the center of all glints and 
     * the glintcenter from the last frame
     */
    int averageDistanceToCenter() const;

    /**
     * Distance in in pixels between the leftmost and the rightmost glint
     * @return distance in pixels between the leftmost and the rightmost glint
     */
    int width() const;

    /**
     * 
     * @return the center of all glints
     */
    cv::Point2f centerPoint();

    /**
     * Getter for all glints of this cluster
     * 
     * @return vector with all glints of this cluster
     */
    std::vector<cv::Point> const& glintsInCluster() const;

};

bool operator<(const GlintCluster& g1, const GlintCluster& g2);

#endif /* GLINTCLUSTER_HPP_ */
