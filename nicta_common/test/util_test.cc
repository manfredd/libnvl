/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "util_test.h"
#include "nicta_common/util.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <cstdlib>
#include <string>
#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION(UtilTest);


using namespace nicta;


void UtilTest :: setUp()
{
}


void UtilTest :: tearDown()
{
}


void UtilTest :: nanosecondResolution()
{
    // Our build system has (and should have) nanosecond resolution for boost::date_time

    CPPUNIT_ASSERT_EQUAL(util::nanosecond_resolution(), true);
}


void UtilTest :: fpsPeriod()
{
    CPPUNIT_ASSERT(util::fps_period(25.0) == boost::posix_time::milliseconds(40));
    CPPUNIT_ASSERT(util::fps_period(0.1)  == boost::posix_time::seconds(10));
    CPPUNIT_ASSERT(util::fps_period(0.0)  == boost::posix_time::seconds(0));
}


void UtilTest :: periodFps()
{
    CPPUNIT_ASSERT_DOUBLES_EQUAL(25.0, util::period_fps(boost::posix_time::milliseconds(40)), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, util::period_fps(boost::posix_time::seconds(10)), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, util::period_fps(boost::posix_time::seconds(0)), 1e-6);
}


void UtilTest :: Sleep()
{
    boost::posix_time::ptime t1(boost::posix_time::second_clock::local_time());
    util::sleep(boost::posix_time::seconds(2));
    util::sleep(boost::posix_time::milliseconds(1000));
    boost::posix_time::ptime t2(boost::posix_time::second_clock::local_time());
    CPPUNIT_ASSERT(t2 - t1 == boost::posix_time::seconds(3));
}


void UtilTest :: environmentVariable()
{
    ::setenv("MyVar", "1234", 1);
    std::string value;
    util::environment_variable("MyVar", value);
    CPPUNIT_ASSERT_EQUAL(value, std::string("1234"));
    ::unsetenv("MyVar");
}


void UtilTest :: deducedFile()
{
    std::vector<std::string> output;
    util::command_capture("pwd", output);

    ::setenv("MyVar", output[0].c_str(), 1);

    std::string result;
    util::deduced_file("MyVar", "Makefile", result);

    ::unsetenv("MyVar");

    boost::filesystem::path p(output[0], boost::filesystem::portable_directory_name);
    p /= "Makefile";

    CPPUNIT_ASSERT_EQUAL(result, p.native_file_string());
}


void UtilTest :: commandCapture()
{
    ::setenv("MyVar", "1234", 1);
    std::vector<std::string> output;
    util::command_capture("echo $MyVar", output);
    ::unsetenv("MyVar");

    CPPUNIT_ASSERT_EQUAL(1, static_cast<int> (output.size()));
    CPPUNIT_ASSERT_EQUAL(output[0], std::string("1234"));
}


