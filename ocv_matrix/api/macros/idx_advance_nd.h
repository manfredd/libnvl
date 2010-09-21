/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_IDX_ADVANCE_ND_H
#define NICTA_OCV_MATRIX_IDX_ADVANCE_ND_H

#include "ocv_matrix/matrix_T0CU.h"

#include <boost/preprocessor/cat.hpp>


#define IDX_ADVANCE_ND(z, n, i)                                                                           \
                                                                                                          \
    this->matrix<T, 0, C, U>::ptr_ += BOOST_PP_CAT(i, n) * this->matrix<T, 0, C, U>::mtx_->dim[n].step;
/**/



#endif

