/*
 * GlintCluster.cpp
 *
 *  Created on: Nov 13, 2012
 *      Author: krigu
 */

#include <iostream>
#include "GlintCluster.hpp"

int GlintCluster::averageDistance() {
	return 100;

}

cv::Point GlintCluster::center() {
	return cv::Point(100, 100);
}

std::vector<cv::Point> const& GlintCluster::glintsInCluster() const {
	return glints;
}

//ostream& GlintCluster::operator<<(ostream& output, const GlintCluster& p) {
//    output << "(" <<  p.x << ", " << p.y <<")";
//    return output;  // for multiple << operators.
//}

