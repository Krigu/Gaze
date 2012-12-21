/*
 * File:   RectGlintTest.cpp
 * Author: krigu
 *
 * Created on Dec 16, 2012, 4:14:08 PM
 */

#include <opencv2/core/core.hpp>

#include "RectGlintTest.h"
#include "detection/glint/FindGlints.hpp"
#include "utils/geometry.hpp"

using namespace cv;

CPPUNIT_TEST_SUITE_REGISTRATION(RectGlintTest);

RectGlintTest::RectGlintTest() {
}

RectGlintTest::~RectGlintTest() {
}

void RectGlintTest::setUp() {
}

void RectGlintTest::tearDown() {
}

void RectGlintTest::testFindGlints() {

    vector<cv::Point> glints;
    glints.push_back(Point(100, 100));
    glints.push_back(Point(0, 100));
    glints.push_back(Point(0, 0));
    glints.push_back(Point(100, 0));

    CPPUNIT_ASSERT(isRectangle(glints, 10));



    FindGlints findGlints;
    //  (88, 122) (122, 113) (93, 114) (123, 97) (95, 97)
    //vector<cv::Point> glints;
    glints.clear();
    glints.push_back(Point(88, 122));
    glints.push_back(Point(122, 113));
    glints.push_back(Point(93, 114));
    glints.push_back(Point(123, 97));
    glints.push_back(Point(95, 97));

    bool result = findGlints.findRectangularCluster(glints);
    CPPUNIT_ASSERT(result);
    CPPUNIT_ASSERT(glints.size() == 4);
    cout << glints.at(0)  << " " << glints.at(1) << " " << glints.at(2) << glints.at(3) << endl;
    CPPUNIT_ASSERT(glints.at(0).x == 122);

    glints.clear();
    glints.push_back(Point(130, 79));
    glints.push_back(Point(93, 77));
    glints.push_back(Point(120, 64));
    glints.push_back(Point(93, 62));

    result = findGlints.findRectangularCluster(glints);

    CPPUNIT_ASSERT(!result);

}
