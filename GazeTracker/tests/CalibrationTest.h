/*
 * File:   CalibrationTest.h
 * Author: krigu
 *
 * Created on Dec 29, 2012, 6:21:47 PM
 */

#ifndef CALIBRATIONTEST_H
#define	CALIBRATIONTEST_H

#include <cppunit/extensions/HelperMacros.h>

class CalibrationTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(CalibrationTest);

    CPPUNIT_TEST(testCalibrtion);

    CPPUNIT_TEST_SUITE_END();

public:
    CalibrationTest();
    virtual ~CalibrationTest();
    void setUp();
    void tearDown();

private:
    void testCalibrtion();
};

#endif	/* CALIBRATIONTEST_H */

