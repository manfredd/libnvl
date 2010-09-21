/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef UTIL_TEST_H
#define UTIL_TEST_H

#include <cppunit/extensions/HelperMacros.h>

class UtilTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(UtilTest);
    CPPUNIT_TEST(nanosecondResolution);
    CPPUNIT_TEST(fpsPeriod);
    CPPUNIT_TEST(periodFps);
    CPPUNIT_TEST(Sleep);
    CPPUNIT_TEST(environmentVariable);
    CPPUNIT_TEST(deducedFile);
    CPPUNIT_TEST(commandCapture);
    CPPUNIT_TEST_SUITE_END();

public:

    virtual void setUp();
    virtual void tearDown();

protected:

    void nanosecondResolution();
    void fpsPeriod();
    void periodFps();
    void Sleep();
    void environmentVariable();
    void deducedFile();
    void commandCapture();
};

#endif
