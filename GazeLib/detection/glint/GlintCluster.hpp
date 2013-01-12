/*
 * GlintCluster.hpp
 *
 *  Created on: Nov 13, 2012
 *      Author: krigu
 */

#ifndef GLINTCLUSTER_HPP_
#define GLINTCLUSTER_HPP_

#include <opencv2/core/core.hpp>

class GlintCluster {

private:
	std::vector<cv::Point> glints;
	int distanceToLastMeasurement;
        int clusterWidth;

public:
	GlintCluster(std::vector<cv::Point> & glints, cv::Point lastMeasurement);

	int averageDistanceToCenter() const;
    int width() const;
	cv::Point2f centerPoint();

	std::vector<cv::Point> const& glintsInCluster() const;

};

bool operator<(const GlintCluster& g1, const GlintCluster& g2);

#endif /* GLINTCLUSTER_HPP_ */
