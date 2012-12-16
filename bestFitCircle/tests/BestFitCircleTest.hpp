/*
 * File:   newtestclass.hpp
 * Author: fri
 *
 * Created on Dec 16, 2012, 2:39:58 PM
 */

#ifndef BESTFITCIRCLETEST_HPP
#define	BESTFITCIRCLETEST_HPP

#include <cppunit/extensions/HelperMacros.h>

class BestFitCircleTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(BestFitCircleTest);

    CPPUNIT_TEST(testBestFitCircle);

    CPPUNIT_TEST_SUITE_END();

public:
    BestFitCircleTest();
    virtual ~BestFitCircleTest();
    void setUp();
    void tearDown();

private:
    void testBestFitCircle();

};

#endif	/* BESTFITCIRCLETEST_HPP */

