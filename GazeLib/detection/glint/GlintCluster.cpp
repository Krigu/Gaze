/*
 * GlintCluster.cpp
 *
 *  Created on: Nov 13, 2012
 *      Author: krigu
 */

#include <iostream>
#include "GlintCluster.hpp"
#include "../../utils/geometry.hpp"

using namespace std;
using namespace cv;

/**
 * Constructor which calculates the center of all provided glints.
 * This center is than used to calculate the distance to the last known
 * center of the glints.
 */
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

/**
 * Returns the distance from the glints to the last known glint center
 */
int const GlintCluster::averageDistanceToCenter() const {
	return distanceToLastMeasurement;

}

/**
 * Calculates the center of the glint cluster
 */
cv::Point GlintCluster::centerPoint() {
	int amount = glints.size();
	int sumX = 0;
	int sumY = 0;
	for (vector<Point>::iterator it = glints.begin(); it != glints.end();
			++it) {
		sumX += it->x;
		sumY += it->y;
	}

	float x = sumX / amount;
	float y = sumY / amount;

	return cv::Point(x, y);
}

/**
 * Returns all glint belonging to this cluster
 */
std::vector<cv::Point> const& GlintCluster::glintsInCluster() const {
	return glints;
}

/**
 * < Operator for using the sort algorithm
 */
bool operator<(const GlintCluster& g1, const GlintCluster& g2) {
	return (g1.averageDistanceToCenter() < g2.averageDistanceToCenter());
}

