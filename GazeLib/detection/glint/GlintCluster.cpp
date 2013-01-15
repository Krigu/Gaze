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

    if (glints.empty())
        return;

    int amountOfGlints = glints.size();
    int totalDistance = 0;

    int left = 10000;
    int right = 0;

    for (vector<Point>::iterator it = glints.begin(); it != glints.end(); ++it) {
        
        totalDistance += calcPointDistance(&*it, &lastMeasurement);
        left = min(left, it->x);
        right = max(right, it->x);
    }

    distanceToLastMeasurement = (totalDistance / amountOfGlints);
    clusterWidth = right - left;
}

int GlintCluster::averageDistanceToCenter() const {
    return distanceToLastMeasurement;

}

cv::Point2f GlintCluster::centerPoint() {
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

    return cv::Point2f(x, y);
}

int GlintCluster::width() const {
    return clusterWidth;
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

