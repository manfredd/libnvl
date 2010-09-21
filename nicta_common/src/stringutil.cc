/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "nicta_common/stringutil.h"

#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi.hpp>

#include <string>


char *
nicta::stringutil :: 
ccc(const std::string& s) throw()
{
    // Return a non-const char* from a string

    return const_cast<char *> (s.c_str());
}


bool
nicta::stringutil :: 
is_numeric(const std::string& s) throw()
{
    // Return true if string represents a number

    long double n;
    std::string::const_iterator first = s.begin();
    std::string::const_iterator last  = s.end();
    return boost::spirit::qi::phrase_parse(first,
                                           last,
                                           (boost::spirit::qi::long_double[boost::phoenix::ref(n) = boost::spirit::qi::_1]),
                                           boost::spirit::ascii::space)
           && (first == last);
}
