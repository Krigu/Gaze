/*
 * GlintCluster.cpp
 *
 *  Created on: Nov 13, 2012
 *      Author: krigu
 */

#include <iostream>
#include "GlintCluster.hpp"
#include "utils/geometry.hpp"

using namespace std;
using namespace cv;

GlintCluster::GlintCluster(std::vector<cv::Point> & glints,
		cv::Point lastMeasurement) :
		glints(glints) {

	// TODO: Specify what to do. Exception?
	if (glints.size() == 0)
		return;

	int amountOfGlints = glints.size();
	int totalDistance = 0;

	for (vector<Point>::iterator it = glints.begin(); it != glints.end();
			++it) {
		totalDistance += calcPointDistance(*it, lastMeasurement);
	}
	distanceToLastMeasurement = (totalDistance / amountOfGlints);
}

int const GlintCluster::averageDistanceToCenter() const {
	return distanceToLastMeasurement;

}

cv::Point GlintCluster::centerPoint() {
	return cv::Point(100, 100);
}

std::vector<cv::Point> const& GlintCluster::glintsInCluster() const {
	return glints;
}

bool operator< (const GlintCluster& g1, const GlintCluster& g2) {
	cout << "Compare g1:" <<  g1.averageDistanceToCenter() << " g2:" << g2.averageDistanceToCenter() << endl;
	return (g1.averageDistanceToCenter() < g2.averageDistanceToCenter());
}

//ostream& GlintCluster::operator<<(ostream& output, const GlintCluster& p) {
//    output << "(" <<  p.x << ", " << p.y <<")";
//    return output;  // for multiple << operators.
//}

