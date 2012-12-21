/*
 * geometry.cpp
 *
 *  Created on: Nov 9, 2012
 *      Author: krigu
 */

#include <vector>
#include "geometry.hpp"
#include "../exception/GazeExceptions.hpp"

#include <iostream>

using namespace std;

const double Rad2Deg = 180.0 / 3.1415;
const double Deg2Rad = 3.1415 / 180.0;

int calcPointDistance(cv::Point *point1, cv::Point *point2) {
    int distX = point1->x - point2->x;
    int distY = point1->y - point2->y;
    int sum = distX * distX + distY * distY;

    return sqrt(sum);
}

// TODO use template for calcPointDistanfce and calcPoint2fDistance

// TODO is computional challenging sqrt necessary for only comparing the distances in distanceSorter

float calcPoint2fDistance(cv::Point2f point1, cv::Point2f point2) {
    int distX = point1.x - point2.x;
    int distY = point1.y - point2.y;
    int sum = distX * distX + distY * distY;

    return sqrt(sum);
}

cv::Point calcRectBarycenter(cv::Rect& rect) {
    int x = rect.width / 2;
    int y = rect.height / 2;

    return cv::Point(rect.x + x, rect.y + y);
}

// Calculates the angle in degrees

double calcAngle(cv::Point start, cv::Point end) {
    return atan2(end.y - start.y, end.x - start.x) * (180 / 3.141);
}

cv::Point2f calcAverage(std::vector<cv::Point2f> points) {
    int amount = points.size();

    if (amount == 0) {
        throw new WrongArgumentException("Cannot calc the average of 0 Points!");
    }

    float sumX = 0;
    float sumY = 0;
    for (std::vector<cv::Point2f>::iterator it = points.begin(); it != points.end();
            ++it) {
        sumX += it->x;
        sumY += it->y;
    }

    float x = sumX / amount;
    float y = sumY / amount;

    return cv::Point2f(x, y);
}

struct distanceSorter {
    cv::Point2f reference;

    distanceSorter(cv::Point2f  reference) : reference(reference) {
    }

    bool operator()(cv::Point2f first, cv::Point2f  second) {
        return calcPoint2fDistance(reference, first) < calcPoint2fDistance(reference, second);
    }
};

cv::Point2f calcMedianPoint(cv::Point2f reference, std::vector< cv::Point2f > scores) {

    cv::Point2f median;
    size_t size = scores.size();
    sort(scores.begin(), scores.end(), distanceSorter(reference));

    if (size % 2 == 0) {
        cv::Point2f p1 = scores[size / 2 - 1];
        cv::Point2f p2 = scores[size / 2];
        float x = (p1.x + p2.x) / 2;
        float y = (p1.y + p2.y) / 2;
        median = cv::Point2f(x, y);
    } else {
        median = scores.at(size / 2);
    }

    return median;
}

//bool isRectangle(cv::Point p1, cv::Point p2, cv::Point p3, cv::Point p4, int tolerance) {

//    tolerance = tolerance * tolerance;
//
//    // Find barycentre
//    double cx = (p1.x + p2.x + p3.x + p4.x) / 4;
//    double cy = (p1.y + p2.y + p3.y + p4.y) / 4;
//
//    // calc squared distance from point to barycentre
//    double dd1 = pow(cx - p1.x, 2) + pow(cy - p1.y, 2);
//    double dd2 = pow(cx - p2.x, 2) + pow(cy - p2.y, 2);
//    double dd3 = pow(cx - p3.x, 2) + pow(cy - p3.y, 2);
//    double dd4 = pow(cx - p4.x, 2) + pow(cy - p4.y, 2);
//
//    return fabs(dd1 - dd2) <= tolerance && fabs(dd1 - dd3) <= tolerance && fabs(dd1 - dd4) <= tolerance
//}

bool isRectangle(vector<cv::Point> points, int tolerance) {
    // TODO: Optimize
    double tol = static_cast<double> (tolerance);
    int errors = 0; // Two points have to be ortagonal, one does not matter
    for (std::vector<int>::size_type i = 0; i != points.size(); i++) {
        for (std::vector<int>::size_type j = i + 1; j != points.size(); j++) {
            //cout << "Angle i: " << i << " j: " << j << " " << points[i] << " " << points[j] << ": " << calcAngle(points[i], points[j]) << endl;
            double angle = calcAngle(points[i], points[j]);
            if (fabs(fmod(angle, 90.0)) > tol)
                errors++;
            if (errors > 2)
                return false;
        }
    }

    return true;
}
