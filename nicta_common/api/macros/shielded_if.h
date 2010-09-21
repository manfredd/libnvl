/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_SHIELDED_IF_H
#define NICTA_SHIELDED_IF_H

#include <boost/preprocessor/control/if.hpp>


#define NICTA_SHIELDED_IF(n, pretext, body, posttext) BOOST_PP_IF(n, pretext, ) body BOOST_PP_IF(n, posttext, )

#endif


// ****************************************************************************************************
//
//  [ SYNOPSIS ]
//
//  Will sometimes have an expression which expands containing multiple commas,
//  in which case, if call BOOST_PP_IF, the commas will be interpreted as the
//  ith-arguments to the BOOST_PP_IF macro, instead of the _body_ of the desired
//  expansion.
//
//  The above macro shields the body from this unintended side effect.
//
//
//  [ NOTES ]
//
//  The above macro has an *EMPTY* else branch!!
//  
//
//  [ PROBLEM EXAMPLE ]
//
//  Assume:  n = 3
//
//  
//  BOOST_PP_IF(n, template < BOOST_PP_ENUM_PARAMS(n, typename T) >, )
//  // expands to:  BOOST_PP_IF(3, template < typename T0, typename T1, typename T2 > , )
//  
//  // issue:  BOOST_PP_IF takes only 3 args BOOST_PP_IF(n, t, f)
//  //         but above expansion effects 5 arguments to BOOST_PP_IF, resulting in an error!
//
//
//  [ RESOLUTION ]
//
//  NICTA_SHIELDED_IF(n, template <, BOOST_PP_ENUM_PARAMS(n, typename T), >)
//  // expands to:  template < typename T0, typename T1, typename T2 >   ...as desired :o)
//
//
// ****************************************************************************************************

