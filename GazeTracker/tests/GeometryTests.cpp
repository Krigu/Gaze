/*
 * File:   GeometryTests.cpp
 * Author: krigu
 *
 * Created on Dec 20, 2012, 9:34:02 PM
 */

#include "GeometryTests.h"
#include "utils/geometry.hpp"

#include <iostream>
#include <opencv2/core/core.hpp>


CPPUNIT_TEST_SUITE_REGISTRATION(GeometryTests);

using namespace cv;
using namespace std;

GeometryTests::GeometryTests() {
}

GeometryTests::~GeometryTests() {
}

void GeometryTests::setUp() {
}

void GeometryTests::tearDown() {
}

void GeometryTests::testCalcPointDistance() {

    cv::Point point1(0, 0);
    cv::Point point2(100, 100);
    int result = calcPointDistance(&point1, &point2);

    CPPUNIT_ASSERT(result == 141);

}

void GeometryTests::testCalcPoint2fDistance() {

    cv::Point2f point1(0.0, 0.00);
    cv::Point2f point2(100.0, 100.0);
    float result = calcPoint2fDistance(point1, point2);

    CPPUNIT_ASSERT(fabs(result - 141.421) < 0.1);
}

void GeometryTests::testCalcRectBarycenter() {
    cv::Rect rect(Point(0, 0), Point(100, 100));

    cv::Point result = calcRectBarycenter(rect);

    CPPUNIT_ASSERT(result.x == 50);
    CPPUNIT_ASSERT(result.y == 50);

}

void GeometryTests::testCalcAngle() {
//
//    cv::Point origin(0, 0);
//    CPPUNIT_ASSERT(fabs(calcAngle(origin, Point(100, 100)) - 45.0) < 0.01);
//    float angle = calcAngle(origin, Point(0, 100)) - 90.0;
//    cout << "Angle: " << angle << endl;
//    CPPUNIT_ASSERT(fabs(angle) < 0.1);
//    CPPUNIT_ASSERT(fabs(calcAngle(origin, Point(100, 0))) < 0.1);

}

void GeometryTests::testCalcAverage() {

    std::vector<cv::Point2f> scores;

    scores.push_back(cv::Point2f(1.8, 1234));
    scores.push_back(cv::Point2f(123.23, 123.34));
    scores.push_back(cv::Point2f(324.34, 300.4));
    scores.push_back(cv::Point2f(123.23, 4005.6));
    scores.push_back(cv::Point2f(1234.34, 500.789));

    cv::Point2f result = calcAverage(scores);
    // TODO: check with fabs
    CPPUNIT_ASSERT(fabs(result.x - 361.388) < 0.01);
    CPPUNIT_ASSERT(fabs(result.y - 1232.83) < 0.01);

}

void GeometryTests::testCalcMedianPoint() {

    cv::Point2f reference(0, 0);
    std::vector<cv::Point2f> scores;

    scores.push_back(cv::Point2f(100, 100));
    scores.push_back(cv::Point2f(200, 200));
    scores.push_back(cv::Point2f(300, 300));
    scores.push_back(cv::Point2f(400, 400));
    scores.push_back(cv::Point2f(500, 500));

    cv::Point2f result = calcMedianPoint(reference, scores);

    CPPUNIT_ASSERT(result.x == 300);
    CPPUNIT_ASSERT(result.y == 300);

}

void GeometryTests::testIsRectangle() {
    std::vector<cv::Point> glints;
    glints.push_back(Point(100, 100));
    glints.push_back(Point(0, 100));
    glints.push_back(Point(0, 0));
    glints.push_back(Point(100, 0));

    CPPUNIT_ASSERT(isRectangle(glints, 10));
}

void GeometryTests::testIsRectangleArea() {
    std::vector<cv::Point> glints;
    glints.push_back(Point(100, 100));
    glints.push_back(Point(0, 100));
    glints.push_back(Point(0, 0));
    glints.push_back(Point(100, 0));

    CPPUNIT_ASSERT(isRectangle(glints, 10));

    glints.clear();
    // Correct rectangle
    glints.push_back(Point(45, 86));
    glints.push_back(Point(23, 84));        
    glints.push_back(Point(45, 74));
    glints.push_back(Point(24, 72));

    CPPUNIT_ASSERT(isRectangle(glints, 10));

    glints.clear();
    // Incorrect rectangle
    glints.push_back(Point(94, 97));
    glints.push_back(Point(45, 86));
    glints.push_back(Point(23, 84));
    glints.push_back(Point(45, 74));

    CPPUNIT_ASSERT(!isRectangle(glints, 10));
    
        glints.clear();
    // Incorrect rectangle
    glints.push_back(Point(94, 97));
    glints.push_back(Point(45, 86));
    glints.push_back(Point(23, 84));
    glints.push_back(Point(45, 74));

    CPPUNIT_ASSERT(!isRectangle(glints, 10));
    
    
    glints.clear();
    glints.push_back(Point(110, 110));
    glints.push_back(Point(0, 100));
    glints.push_back(Point(-10, -10));
    glints.push_back(Point(100, 0));
    CPPUNIT_ASSERT(isRectangle(glints, 10));
    
    glints.clear();
    glints.push_back(Point(126, 111));
    glints.push_back(Point(104, 109));
    glints.push_back(Point(155, 97));
    glints.push_back(Point(126, 99));
    
    CPPUNIT_ASSERT(!isRectangle(glints, 10));
}