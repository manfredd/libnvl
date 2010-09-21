/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_SVD_MODEL_H
#define NICTA_OCV_MATRIX_SVD_MODEL_H

#include <opencv/cxcore.h>


namespace nicta {
namespace ocv {
namespace matrix {

namespace svd {

    enum SVD                                                  // convenience to help get matrices out of svd() tuple:  A = U * W * transpose(V)
    {
        W = 0 ,
        U = 1 ,
        V = 2 
    };

} // namespace nicta::ocv::matrix::svd



    enum SVDSourceModel
    {
        Immutable = 0               ,                         // do not permit the source matrix to be modified in svd computation
        Mutable   = CV_SVD_MODIFY_A                           // source matrix can be modified (we are prepared to junk the source matrix)
    };


    enum SVDModel
    {
        Flat         = 0                       ,              // return U, V as is
        Transpose_U  = CV_SVD_U_T              ,              // return transpose(U)
        Transpose_V  = CV_SVD_V_T              ,              // return transpose(V)
        Transpose_UV = CV_SVD_U_T | CV_SVD_V_T                // return transpose(U), transpose(V)
    };



    template <SVDSourceModel model>
    struct SVD_source_model
    {
        enum
        {
            value = static_cast<int> (model)
        };
    };


    template <SVDModel model>
    struct SVD_model
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
