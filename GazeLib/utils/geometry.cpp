#include <vector>
#include <iostream>

#include "../exception/GazeExceptions.hpp"
#include "log.hpp"

#include "geometry.hpp"

const double Rad2Deg = 180.0 / 3.1415;
const double Deg2Rad = 3.1415 / 180.0;

using namespace std;

// Comperator to order points by their x-coordinate

bool comparePoint(cv::Point p1, cv::Point p2) {
    return p1.x < p2.x;
}

struct distanceSorter {
    cv::Point2f reference;

    distanceSorter(cv::Point2f reference) : reference(reference) {
    }

    bool operator()(cv::Point2f first, cv::Point2f second) {
        return calcPoint2fDistance(reference, first) < calcPoint2fDistance(reference, second);
    }
};

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
    return atan2(start.y - end.y, start.x - end.x) * Rad2Deg;
}

cv::Point2f calcAverage(std::vector<cv::Point2f> points) {
    int amount = points.size();

    if (amount == 0) {
        throw WrongArgumentException("Cannot calc the average of 0 Points!");
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

bool isRectangle(vector<cv::Point> points, int tolerance) {
    
    orientateFourPoints(points);

    // TODO: optimize
    cv::Point p1 = points.at(0);
    cv::Point p2 = points.at(1);
    cv::Point p3 = points.at(2);
    cv::Point p4 = points.at(3);

    // (1/2)|[(x3-x1)(y4-y2) +(x4-x2)(y1-y3)]|
    float area = 0.5 * abs((p3.x - p1.x) * (p4.y - p2.y) + (p4.x - p2.x) * (p1.y - p3.y));
    if (area < 1)
        return false;

    cv::Mat input(points, false);

    LOG_D("Points " << points);

    cv::RotatedRect r = cv::minAreaRect(input);

    float enclosingArea = r.size.width * r.size.height;
    float ratio = enclosingArea / area;

    // TODO: Add const for ratio
    bool result = ratio < 1.5 && (fmod(fabs(r.angle) + tolerance, 90) <= 2 * tolerance);
    LOG_D("Area: " << area << " Enclosing area: " << enclosingArea
            << " Rotation: " << r.angle << " Ratio: " << ratio << " Result:" << result);

    return result;

}

/**
 * 
 * original matlab code:
 * 
 *  N = size(points,1);
 *  x = points(:,1);
 *  y = points(:,2);
 *  M = [2*x 2*y ones(N,1)];
 *  v = x.^2+y.^2;
 *  pars = M\v;
 *  x0 = pars(1);
 *  y0 = pars(2);
 *  center = pars(1:2);
 *  R = sqrt(sum(center.^2)+pars(3));
 *  if imag(R)~=0,
 *      error('Imaginary squared radius computed');
 *  end;
 * 
 * @param x
 * @param y
 * @param radius
 * @param pointsToFit
 */
void bestFitCircle(float * x, float * y, float * radius,
        std::vector<cv::Point2f> pointsToFit) {

    unsigned int numPoints = pointsToFit.size();

    //setup an opencv mat with our points
    // col 1 = x coordinates and col 2 = y coordinates
    cv::Mat1f points(numPoints, 2);

    unsigned int i = 0;
    for (std::vector<cv::Point2f>::iterator it = pointsToFit.begin(); it != pointsToFit.end(); ++it) {
        points.at<float>(i, 0) = it->x;
        points.at<float>(i, 1) = it->y;
        ++i;
    }

    cv::Mat1f x_coords = points.col(0);
    cv::Mat1f y_coords = points.col(1);
    cv::Mat1f ones = cv::Mat::ones(numPoints, 1, CV_32F);

    // calculate the M matrix
    cv::Mat1f M(numPoints, 3);
    cv::Mat1f x2 = x_coords * 2;
    cv::Mat1f y2 = y_coords * 2;
    x2.col(0).copyTo(M.col(0));
    y2.col(0).copyTo(M.col(1));
    ones.col(0).copyTo(M.col(2));

    // calculate v
    cv::Mat1f v = x_coords.mul(x_coords) + y_coords.mul(y_coords);

    //TODO some error handling in solve()?
    cv::Mat pars;
    cv::solve(M, v, pars, cv::DECOMP_SVD);

    *x = pars.at<float>(0, 0);
    *y = pars.at<float>(0, 1);
    *radius = sqrt(pow(*x, 2) + pow(*y, 2) + pars.at<float>(0, 2));
}

void orientateFourPoints(std::vector< cv::Point >& points) {

    // Order by x-coordinate
    sort(points.begin(), points.end(), comparePoint);

    // Compare y-coordinate of 1 & 2
    // higher value on fist position
    if (points.at(0).y < points.at(1).y)
        swap(points.at(0), points.at(1));

    if (points.at(2).y > points.at(3).y)
        swap(points.at(2), points.at(3));

}

bool isPointInRect(cv::Point& p, cv::Rect& rect) {

    return (rect.x < p.x && p.x < (rect.x + rect.width)) &&
            (rect.y < p.y && p.y < (rect.y + rect.height));

}

int normal(int mean, int stdev) {

    float f1 = (float) rand() / (float) (RAND_MAX + 1.);
    float f2 = (float) rand() / (float) (RAND_MAX + 1.);

    return mean + stdev * cos(2 * PI * f1) * sqrt(-log(f2));

}