/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef STRINGUTIL_TEST_H
#define STRINGUTIL_TEST_H

#include <cppunit/extensions/HelperMacros.h>

class StringUtilTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(StringUtilTest);
    CPPUNIT_TEST(isNumeric);
    CPPUNIT_TEST(atomicToString);
    CPPUNIT_TEST(stringToAtomic);
    CPPUNIT_TEST_SUITE_END();

public:

    virtual void setUp();
    virtual void tearDown();

protected:

    void isNumeric();
    void atomicToString();
    void stringToAtomic();
};

#endif
