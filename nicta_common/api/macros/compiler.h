/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_COMPILER_H
#define NICTA_COMPILER_H


#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#endif



// *******************************************
// GCC_VERSION:
// *******************************************
//
// A GCC 3.2.0 becomes:  GCC_VERSION 30200
//
// A GCC 3.2.1 becomes:  GCC_VERSION 30201
//
//
// Consequently, can now do tests like:
//
// #if GCC_VERSION >= 30200
//    ...
// #else
//    ...
// #endif
//
//
// Similarly, ..
//
// #if ! (GCC_VERSION == 30200)
//    ...
// #else
//    ...
// #endif
//
// *******************************************
