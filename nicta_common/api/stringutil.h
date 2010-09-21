/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_STRINGUTIL_H
#define NICTA_STRINGUTIL_H

#include "nicta_common/nicta_exception.h"

#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/utility/enable_if.hpp>

#include <sstream>
#include <string>


namespace nicta {
namespace stringutil {

    template <typename T>
    typename boost::enable_if< boost::mpl::or_< boost::is_arithmetic<T>
                                              , boost::is_pointer<T>
                                              >
                             , std::string
                             >::type
    atomic_to_string(const T& t) throw(nicta::nicta_exception);


    template <typename T>
    typename boost::enable_if< boost::is_arithmetic<T>
                             , T
                             >::type
    string_to_atomic(const std::string& s) throw(nicta::nicta_exception);


	char* ccc(const std::string& s) throw();

    bool is_numeric(const std::string& s) throw();

} } // namespace nicta::stringutil




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template <typename T>
typename boost::enable_if< boost::mpl::or_< boost::is_arithmetic<T>
                                          , boost::is_pointer<T>
                                          >
                         , std::string
                         >::type
nicta::stringutil :: 
atomic_to_string(const T& t) throw(nicta::nicta_exception)
{
    // Convert atomic type to string if possible

    std::ostringstream oss;
    try
    {
        oss << t;
        return oss.str();
    }
    catch (...)
    {
        throw nicta::nicta_exception("failed to map parameter to string", "atomic_to_string", "stringutil");
    }
}


template <typename T>
typename boost::enable_if< boost::is_arithmetic<T>
                         , T
                         >::type
nicta::stringutil :: 
string_to_atomic(const std::string& s) throw(nicta::nicta_exception)
{
    // Convert string to atomic type if possible

    try
    {
        if (nicta::stringutil::is_numeric(s) == true)
        {
            T t;
            std::istringstream iss(s);
            iss >> t;
            return t;
        }
        else
        {
            throw nicta::nicta_exception("string does not represent a number", "string_to_atomic", "stringutil");
        }
    }
    catch (...)
    {
        throw nicta::nicta_exception("failed to map string to sought type", "string_to_atomic", "stringutil");
    }
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
