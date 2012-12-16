/*
 * geometry.cpp
 *
 *  Created on: Nov 9, 2012
 *      Author: krigu
 */

#include <vector>
#include "geometry.hpp"

const double Rad2Deg = 180.0 / 3.1415;
const double Deg2Rad = 3.1415 / 180.0;

int calcPointDistance(cv::Point point1, cv::Point point2) {
    int distX = point1.x - point2.x;
    int distY = point1.y - point2.y;
    int sum = distX * distX + distY * distY;

    // Use Pythagoras to calc distance between two points
    return sqrt(sum);
}

cv::Point calcRectBarycenter(cv::Rect& rect) {
    int x = rect.width / 2;
    int y = rect.height / 2;

    return cv::Point(rect.x + x, rect.y + y);
}

/// <summary>
/// Calculates angle in radians between two points and x-axis.
/// </summary>

double calcAngle(cv::Point start, cv::Point end) {
    return atan2(start.y - end.y, end.x - start.x) * Rad2Deg;
}

cv::Point2f calcAverage(std::vector<cv::Point2f> points) {
    int amount = points.size();

    // TODO: exception?
    if (amount == 0) {
        return cv::Point(0, 0);
    }

    int sumX = 0;
    int sumY = 0;
    for (std::vector<cv::Point2f>::iterator it = points.begin(); it != points.end();
            ++it) {
        sumX += it->x;
        sumY += it->y;
    }

    float x = sumX / amount;
    float y = sumY / amount;

    return cv::Point2f(x, y);
}

bool isRectangle(cv::Point p1, cv::Point p2, cv::Point p3, cv::Point p4, int tolerance) {

    // Find barycentre
    double cx = (p1.x + p2.x + p3.x + p4.x) / 4;
    double cy = (p1.y + p2.y + p3.y + p4.y) / 4;

    // calc squared distance from point to barycentre
    double dd1 = pow(cx - p1.x, 2) + pow(cy - p1.y, 2);
    double dd2 = pow(cx - p2.x, 2) + pow(cy - p2.y, 2);
    double dd3 = pow(cx - p3.x, 2) + pow(cy - p3.y, 2);
    double dd4 = pow(cx - p4.x, 2) + pow(cy - p4.y, 2);

    return abs(dd1 - dd2) <= tolerance && abs(dd1 - dd3) <= tolerance && abs(dd1 - dd4) <= tolerance;
}
