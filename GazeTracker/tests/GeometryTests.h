/*
 * File:   GeometryTests.h
 * Author: krigu
 *
 * Created on Dec 20, 2012, 9:34:02 PM
 */

#ifndef GEOMETRYTESTS_H
#define	GEOMETRYTESTS_H

#include <cppunit/extensions/HelperMacros.h>

class GeometryTests : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(GeometryTests);

    CPPUNIT_TEST(testCalcPointDistance);
    CPPUNIT_TEST(testCalcPoint2fDistance);
    CPPUNIT_TEST(testCalcRectBarycenter);
    CPPUNIT_TEST(testCalcAngle);
    CPPUNIT_TEST(testCalcAverage);
    CPPUNIT_TEST(testCalcMedianPoint);
    CPPUNIT_TEST(testIsRectangle);
    CPPUNIT_TEST(testIsRectangleArea);
    

    CPPUNIT_TEST_SUITE_END();

public:
    GeometryTests();
    virtual ~GeometryTests();
    void setUp();
    void tearDown();

private:
    void testCalcPointDistance();
    void testCalcPoint2fDistance();
    void testCalcRectBarycenter();
    void testCalcAngle();
    void testCalcAverage();
    void testCalcMedianPoint();
    void testIsRectangle();
    void testIsRectangleArea();

};

#endif	/* GEOMETRYTESTS_H */

