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
	int amount = glints.size();
	int sumX = 0;
	int sumY = 0;
	for (vector<Point>::iterator it = glints.begin(); it != glints.end();
			++it) {
		sumX += it->x;
		sumY += it->y;
	}

	int x = sumX / amount;
	int y = sumY / amount;

	return cv::Point(x, y);
}

std::vector<cv::Point> const& GlintCluster::glintsInCluster() const {
	return glints;
}

bool operator<(const GlintCluster& g1, const GlintCluster& g2) {
	return (g1.averageDistanceToCenter() < g2.averageDistanceToCenter());
}

