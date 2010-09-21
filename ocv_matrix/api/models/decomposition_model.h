/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_DECOMPOSITION_MODEL_H
#define NICTA_OCV_MATRIX_DECOMPOSITION_MODEL_H

#include <opencv/cxcore.h>


namespace nicta {
namespace ocv {
namespace matrix {

    enum DecompositionModel
    {
        LU       = CV_LU      ,               // Gaussian elimination with optimal pivot chosen
        SVD      = CV_SVD     ,               // Singular Value Decomposition  -- [use to invert non-square matrices]
        SVD_PSYM = CV_SVD_SYM                 // Singular Value Decomposition for a symmetric **POSITIVELY** -defined (square) matrix
    };


    template <DecompositionModel model>
    struct decomposition_model
    {
        enum
        {
            value = static_cast<int> (model)
        };
    };

} } } // namespace nicta::ocv::matrix


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
