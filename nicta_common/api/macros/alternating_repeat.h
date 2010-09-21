/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_ALTERNATING_REPEAT_H
#define NICTA_ALTERNATING_REPEAT_H

#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>


#define NICTA_ALTERNATING_REPEAT_SUBMACRO_0(r, n, I, elem) \
                 BOOST_PP_COMMA_IF(I) BOOST_PP_CAT(elem, n)

#define NICTA_ALTERNATING_REPEAT_SUBMACRO_1(r, n, I, elem) \
                 BOOST_PP_COMMA_IF(I) BOOST_PP_CAT(elem, BOOST_PP_ADD(n, 1))

#define NICTA_ALTERNATING_REPEAT_MACRO_0(z, n, seq) \
                 BOOST_PP_COMMA_IF(n) BOOST_PP_SEQ_FOR_EACH_I(NICTA_ALTERNATING_REPEAT_SUBMACRO_0, n, seq)

#define NICTA_ALTERNATING_REPEAT_MACRO_1(z, n, seq) \
                 BOOST_PP_COMMA_IF(n) BOOST_PP_SEQ_FOR_EACH_I(NICTA_ALTERNATING_REPEAT_SUBMACRO_1, n, seq)


#define NICTA_ALTERNATING_REPEAT_0(n, seq) \
                 BOOST_PP_REPEAT(n, NICTA_ALTERNATING_REPEAT_MACRO_0, seq)

#define NICTA_ALTERNATING_REPEAT_1(n, seq) \
                 BOOST_PP_REPEAT(n, NICTA_ALTERNATING_REPEAT_MACRO_1, seq)

#endif


// ***********************************************************************************
//
//  [ SYNOPSIS ]
//
//  Will sometimes need to repeat an expression with alternation, this can be
//  somewhat tedious, and difficult to work out.  We allow flexibilty to begin
//  expansion with either 0 or 1.
//
//
//  [ EXAMPLE ]
//
//  NICTA_ALTERNATING_REPEAT_1(2, (b))         // expands to:   b1, b2                // <--- note: starts at 1 not 0
//
//  NICTA_ALTERNATING_REPEAT_0(2, (a)(b))      // expands to:   a0, b0, a1, b1
//
//  NICTA_ALTERNATING_REPEAT_1(2, (a)(b))      // expands to:   a1, b1, a2, b2        // <--- note: starts at 1 not 0
//
//  NICTA_ALTERNATING_REPEAT_0(2, (a)(b)(c))   // expands to:   a0, b0, c0, a1, b1, c1
//
//  NICTA_ALTERNATING_REPEAT_0(3, (a)(b)(c))   // expands to:   a0, b0, c0, a1, b1, c1, a2, b2, c2
//
//
// ***********************************************************************************
