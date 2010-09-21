/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_CONDITIONAL_COMMA_H
#define NICTA_CONDITIONAL_COMMA_H

#include <boost/preprocessor/punctuation/comma_if.hpp>


#define NICTA_CONDITIONAL_COMMA(z, n, text) BOOST_PP_COMMA_IF(n) text

#endif



// ***********************************************************************************
//
//  [ SYNOPSIS ]
//
//  The above macro *precedes* the _text_ argument with a comma, iff n > 0
//
//
//  [ EXAMPLE ]
//
//  NICTA_CONDITIONAL_COMMA(_, 0, abc)         // expands to:   abc
//
//  NICTA_CONDITIONAL_COMMA(_, 1, abc)         // expands to:   , abc
//
//
// ***********************************************************************************
