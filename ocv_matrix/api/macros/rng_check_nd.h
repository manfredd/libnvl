/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_RNG_CHECK_ND_H
#define NICTA_OCV_MATRIX_RNG_CHECK_ND_H

#include "ocv_matrix/matrix_T0CU.h"

#include <boost/preprocessor/cat.hpp>

#include <stdexcept>


#define RNG_CHECK_ND(z, n, i)                                                                                 \
                                                                                                              \
    if (BOOST_PP_CAT(i, n) >= static_cast<unsigned int> (this->matrix<T, 0, C, U>::mtx_->dim[n].size))        \
    {                                                                                                         \
        throw std::out_of_range("index out of range");                                                        \
    }
/**/


#endif

