/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_UTIL_H
#define NICTA_UTIL_H

#include "nicta_common/nicta_exception.h"

#include <boost/date_time/posix_time/posix_time.hpp>

#include <cstdio>
#include <memory>
#include <string>


namespace nicta {
namespace util {

    bool nanosecond_resolution() throw();

    boost::posix_time::time_duration fps_period(double fps) throw ();

    double period_fps(boost::posix_time::time_duration td) throw ();

    boost::posix_time::time_duration sleep(boost::posix_time::time_duration td) throw ();

    bool environment_variable(const char* name, std::string& value) throw ();

    void deduced_file(const char* env_var, const char* filename, std::string& result) throw ();

    template <template <typename T_, typename A_ = std::allocator<T_> > class Container>
    void command_capture(const char* cmd, Container<std::string>& output) throw (nicta::nicta_exception);

} } // namespace nicta::util




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template <template <typename T_, typename A_ = std::allocator<T_> > class Container>
void
nicta::util :: 
command_capture(const char* cmd, Container<std::string>& output) throw (nicta::nicta_exception)
{
    // Return the output of a shell command 'cmd'

    int bufsize = static_cast<int> (std::pow(static_cast<double> (sizeof(int)), 8.0)) / 2 - 1;
    char* buffer = new char[bufsize];

    ::FILE* fp = ::popen(cmd, "r");
    if (!fp)
    {
        throw nicta::nicta_exception("Call to ::popen failed", "command_capture", "nicta::util");
    }
    while (::fgets(buffer, bufsize, fp))
    {
        if (::strlen(buffer) - 1)
        {
            output.insert(output.end(), std::string(buffer, ::strlen(buffer) - 1));
        }
    }
    int status = ::pclose(fp);
    if (status == -1)
    {
        throw nicta::nicta_exception("Call to ::pclose failed", "command_capture", "nicta::util");
    }

    delete[] buffer;
} 


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
