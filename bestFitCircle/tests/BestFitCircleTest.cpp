/*
 * File:   newtestclass.cpp
 * Author: fri
 *
 * Created on Dec 16, 2012, 2:39:58 PM
 */

#include "BestFitCircleTest.hpp"
#include "utils/geometry.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(BestFitCircleTest);

BestFitCircleTest::BestFitCircleTest() {
}

BestFitCircleTest::~BestFitCircleTest() {
}

void BestFitCircleTest::setUp() {
}

void BestFitCircleTest::tearDown() {
}

void BestFitCircleTest::testBestFitCircle() {
    float x = -1;
    float y = -1;
    float radius = -1;

    const float max_difference = 10E-5;
    
    const float x_expected_1 = 0;
    const float y_expected_1 = 0;
    const float r_expected_1 = 1;

    const float x_expected_2 = 2;
    const float y_expected_2 = -1.5;
    const float r_expected_2 = 4.609722;
    
    /*
     * setup some points, for whichi we know 
     * the best circle
     * 
     * Points for Circle 1:
     *  > 0/1
     *  > 1/0
     *  > 0/-1
     *  > -1/0
     *  --> Center 0/0 with radius 1
     */
    std::vector<cv::Point2f> circle1;
    circle1.push_back(cv::Point2f(0, 1));
    circle1.push_back(cv::Point2f(1, 0));
    circle1.push_back(cv::Point2f(0, -1));
    circle1.push_back(cv::Point2f(-1, 0));

    bestFitCircle(&x, &y, &radius, circle1);

    std::cout << x << "/" << y << " " << radius << std::endl;

    CPPUNIT_ASSERT(fabs(r_expected_1 - radius) < max_difference);
    CPPUNIT_ASSERT(fabs(x_expected_1 - x) < max_difference);
    CPPUNIT_ASSERT(fabs(y_expected_1 - y) < max_difference);

    /*
     * setup some points, for whichi we know 
     * the best circle
     * 
     * Points for Circle 2:
     *  > 5/2
     *  > 1/3
     *  > -1/2
     *  --> Center 2/-1.5 with radius 4.60977
     */
    std::vector<cv::Point2f> circle2;
    circle2.push_back(cv::Point2f(5, 2));
    circle2.push_back(cv::Point2f(1, 3));
    circle2.push_back(cv::Point2f(-1, 2));

    bestFitCircle(&x, &y, &radius, circle2);

    std::cout << x << "/" << y << " " << radius << std::endl;

    CPPUNIT_ASSERT(fabs(x_expected_2 - x) < max_difference);
    CPPUNIT_ASSERT(fabs(y_expected_2 - y) < max_difference);
    CPPUNIT_ASSERT(fabs(r_expected_2 - radius) < max_difference);
}

