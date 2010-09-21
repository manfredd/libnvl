/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "stringutil_test.h"
#include "nicta_common/stringutil.h"

#include <string>


CPPUNIT_TEST_SUITE_REGISTRATION(StringUtilTest);


using namespace nicta;


void StringUtilTest :: setUp()
{
}


void StringUtilTest :: tearDown()
{
}


void StringUtilTest :: isNumeric()
{
    CPPUNIT_ASSERT(stringutil::is_numeric("3") == true);
    CPPUNIT_ASSERT(stringutil::is_numeric("+3.e-.0") == false);
    CPPUNIT_ASSERT(stringutil::is_numeric(".3E+1.") == false);
    CPPUNIT_ASSERT(stringutil::is_numeric("3+") == false);
    CPPUNIT_ASSERT(stringutil::is_numeric("x") == false);
    CPPUNIT_ASSERT(stringutil::is_numeric("") == false);
}


void StringUtilTest :: stringToAtomic()
{
    CPPUNIT_ASSERT_EQUAL(3, stringutil::string_to_atomic<int> ("3"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, stringutil::string_to_atomic<double> ("3"), 1e-6);
    CPPUNIT_ASSERT_THROW(stringutil::string_to_atomic<double> (".3E+1."), nicta_exception);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, stringutil::string_to_atomic<double> ("3"), 1e-6);
    CPPUNIT_ASSERT_THROW(stringutil::string_to_atomic<double> ("+3.e-.0"), nicta_exception);
    CPPUNIT_ASSERT_THROW(stringutil::string_to_atomic<double> (".3E+1."), nicta_exception);
    CPPUNIT_ASSERT_THROW(stringutil::string_to_atomic<double> ("x.3E+1."), nicta_exception);

    CPPUNIT_ASSERT_EQUAL(3, stringutil::string_to_atomic<int> ("3"));
    CPPUNIT_ASSERT_THROW(stringutil::string_to_atomic<int> ("+3.e-.0"), nicta_exception);
    CPPUNIT_ASSERT_THROW(stringutil::string_to_atomic<int> ("x+3.e-.0"), nicta_exception);
}


void StringUtilTest :: atomicToString()
{
    CPPUNIT_ASSERT_EQUAL(std::string("3"), stringutil::atomic_to_string(3));
    CPPUNIT_ASSERT_EQUAL(std::string("3"), stringutil::atomic_to_string(3.0));
    CPPUNIT_ASSERT_EQUAL(std::string("3"), stringutil::atomic_to_string(.3e1));
}


