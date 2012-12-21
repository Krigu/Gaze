/*
 * File:   RectGlintTest.h
 * Author: krigu
 *
 * Created on Dec 16, 2012, 4:14:08 PM
 */

#ifndef RECTGLINTTEST_H
#define	RECTGLINTTEST_H

#include <cppunit/extensions/HelperMacros.h>

class RectGlintTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(RectGlintTest);

    CPPUNIT_TEST(testFindGlints);

    CPPUNIT_TEST_SUITE_END();

public:
    RectGlintTest();
    virtual ~RectGlintTest();
    void setUp();
    void tearDown();

private:
    void testFindGlints();

};

#endif	/* RECTGLINTTEST_H */

