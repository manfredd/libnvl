/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "nicta_common/util.h"
#include "nicta_common/nicta_exception.h"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <vector>


bool
nicta::util :: 
nanosecond_resolution() throw()
{
    // Return true if Makefile variable:  BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG  is defined

#ifdef BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG
    return true;
#else
    return false
#endif
}


boost::posix_time::time_duration
nicta::util :: 
fps_period(double fps) throw ()
{
    // Return fps expressed as a time duration

    if (fps > 1 && fps <= 1e3)
    {
        return boost::posix_time::milliseconds(static_cast<unsigned long int> (1e3 * 1.0 / fps));
    }

    if (fps > 1e3 && fps <= 1e6)
    {
        return boost::posix_time::microseconds(static_cast<unsigned long int> (1e6 * 1.0 / fps));
    }

#ifdef BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG
    if (fps > 1e6 && fps <= 1e9)
    {
        return boost::posix_time::nanoseconds(static_cast<unsigned long int> (1e9 * 1.0 / fps));
    }
#endif

    if (fps > 0 && fps <= 1)
    {
        return boost::posix_time::seconds(1) * static_cast<unsigned long int> (1.0 / fps);
    }
    return boost::posix_time::seconds(0);
}


double
nicta::util :: 
period_fps(boost::posix_time::time_duration td) throw ()
{
    // Return fps from a time duration

    double fs = static_cast<double> (td.fractional_seconds());
    if (!fs && !td.seconds())
    {
        return 0;
    }
    if (td.seconds())
    {
        return 1.0 / static_cast<double> (td.seconds());
    }

#ifdef BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG
    return 1e9 / fs;
#else
    return 1e6 / fs;
#endif
}


boost::posix_time::time_duration
nicta::util :: 
sleep(boost::posix_time::time_duration td) throw ()
{
    // Sleep for said time duration, returning time remaining if there was an interrupt had

    struct ::timespec req = { 0 };
    struct ::timespec rem = { 0 };

    ::time_t sec = td.total_seconds();
    unsigned long int nsec = td.fractional_seconds();

#ifndef BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG
    nsec *= 1000;
#endif

    req.tv_sec  = sec;
    req.tv_nsec = nsec;

    int retval  = ::nanosleep(&req, &rem);
    if (retval == -1)
    {
        boost::posix_time::time_duration td_ = boost::posix_time::seconds(rem.tv_sec);

#ifdef BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG
        td_ += boost::posix_time::nanoseconds(rem.tv_nsec);
#else
        td_ += boost::posix_time::microseconds(rem.tv_nsec / 1000);
#endif

        return td_;
    }
    return boost::posix_time::seconds(0);
}


bool
nicta::util :: 
environment_variable(const char* name, std::string& value) throw ()
{
    // Get the value of an environment variable, returning True if successful

    const char* v = std::getenv(name);
    if (v != 0)
    {
        value = v;
        return true;
    }
    return false;
}


void
nicta::util :: 
deduced_file(const char* env_var, const char* filename, std::string& result) throw ()
{
    // Return full path to regular-file 'filename' in directory path 'env_var' into 'result'

    std::string path(".:");
    nicta::util::environment_variable(env_var, path);

    std::vector<std::string> vec;
    boost::algorithm::split(vec, path, boost::algorithm::is_any_of(":"));

    for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it)
    {
        boost::filesystem::path p(*it, boost::filesystem::portable_directory_name);
        p /= filename;
        if (boost::filesystem::exists(p) && boost::filesystem::is_regular_file(p))
        {
            result = p.native_file_string();
            break;
        }
    }
}


