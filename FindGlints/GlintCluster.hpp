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
	// TODO: There was a &
	std::vector<cv::Point> glints;
	int distanceToLastMeasurement;

public:
	//TODO ask mr C++ if this is valid code
	GlintCluster(std::vector<cv::Point> & glints, cv::Point lastMeasurement);

	int const averageDistanceToCenter() const;
	cv::Point centerPoint();
// TODO: WTF?
	std::vector<cv::Point> const& glintsInCluster() const;

};

bool operator< (const GlintCluster& g1, const GlintCluster& g2);

//bool operator< (GlintCluster &gc1, GlintCluster &gc2) {
//	return gc1.averageDistanceToCenter() < gc2.averageDistanceToCenter();
//}

#endif /* GLINTCLUSTER_HPP_ */
