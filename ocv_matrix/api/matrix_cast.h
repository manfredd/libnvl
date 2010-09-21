/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_MATRIX_CAST_H
#define NICTA_OCV_MATRIX_MATRIX_CAST_H

#include "ocv_matrix/extents.h"
#include "ocv_matrix/matrix.h"
#include "ocv_matrix/matrix_T1C1.h"
#include "ocv_matrix/matrix_T2C2.h"

#include <opencv/cxcore.h>
#include <opencv/cxtypes.h>

#include <boost/shared_ptr.hpp>

#include <functional>
#include <numeric>


namespace nicta {
namespace ocv {
namespace matrix {

    template < typename V
             , typename T
             , unsigned int D
             , unsigned int C
             >
    boost::shared_ptr<nicta::ocv::matrix::matrix<V, D, C, D> >
    matrix_cast(const nicta::ocv::matrix::matrix<T, D, C, D>& m) throw ();


    template < typename V
             , typename T
             , unsigned int C
             >
    boost::shared_ptr<nicta::ocv::matrix::matrix<V, 2, C, 2> >
    matrix_cast(const nicta::ocv::matrix::matrix<T, 2, C, 2>& m) throw ();


    template < typename V
             , typename T
             , unsigned int C
             >
    boost::shared_ptr<nicta::ocv::matrix::matrix<V, 1, C, 1> >
    matrix_cast(const nicta::ocv::matrix::matrix<T, 1, C, 1>& m) throw ();

} } } // namespace nicta::ocv::matrix



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename V
         , typename T
         , unsigned int C
         , unsigned int D
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<V, D, C, D> >
nicta::ocv::matrix ::
matrix_cast(const nicta::ocv::matrix::matrix<T, D, C, D>& m) throw ()
{
    // Cast from one underlying type to another
    // NOTE:  dimension and channel lengths are preserved!

    const ::CvMatND* mtx = m.template native_handle< >();
    int* d = new int[mtx->dims];
    ::cvGetDims(mtx, &d[0]);

    unsigned long int size = std::accumulate(d,
                                             d + mtx->dims,
                                             1,
                                             std::multiplies<int>());
    delete[] d;

    nicta::ocv::matrix::extents<D> ex;
    m.extents(ex);
    boost::shared_ptr<nicta::ocv::matrix::matrix<V, D, C, D> > s_ptr(new nicta::ocv::matrix::matrix<V, D, C, D> (ex));

    ::CvMatND* mat = s_ptr->template native_handle< >();
    for (unsigned long int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            T t = reinterpret_cast<T *> (mtx->data.ptr + i * sizeof(T))[c];
            reinterpret_cast<V *> (mat->data.ptr + i * sizeof(V))[c] = static_cast<V> (t);
        }
    }
    return s_ptr;
}


template < typename V
         , typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<V, 2, C, 2> >
nicta::ocv::matrix :: 
matrix_cast(const nicta::ocv::matrix::matrix<T, 2, C, 2>& m) throw ()
{
    // Cast from one underlying type to another
    // NOTE:  dimension and channel lengths are preserved!

    boost::shared_ptr<nicta::ocv::matrix::matrix<V, 2, C, 2> >
         s_ptr(new nicta::ocv::matrix::matrix<V, 2, C, 2> (nicta::ocv::matrix::extents<2>(m.extent(0), m.extent(1))));

    const ::CvMat* mtx = m.template native_handle< >();
    ::CvMat* mat = s_ptr->template native_handle< >();
    unsigned int size = mtx->rows * mtx->cols;
    for (unsigned int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            T t = reinterpret_cast<T *> (mtx->data.ptr + i * sizeof(T))[c];
            reinterpret_cast<V *> (mat->data.ptr + i * sizeof(V))[c] = static_cast<V> (t);
        }
    }
    return s_ptr;
}


template < typename V
         , typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<V, 1, C, 1> >
nicta::ocv::matrix ::
matrix_cast(const nicta::ocv::matrix::matrix<T, 1, C, 1>& m) throw ()
{
    // Cast from one underlying type to another
    // NOTE:  dimension and channel lengths are preserved!

    boost::shared_ptr<nicta::ocv::matrix::matrix<V, 1, C, 1> >
         s_ptr(new nicta::ocv::matrix::matrix<V, 1, C, 1> (nicta::ocv::matrix::extents<2>(m.extent(0), m.extent(1))));

    const ::CvMat* mtx = m.template native_handle< >();
    ::CvMat* mat = s_ptr->template native_handle< >();
    unsigned int size = mtx->rows * mtx->cols;
    for (unsigned int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            T t = reinterpret_cast<T *> (mtx->data.ptr + i * sizeof(T))[c];
            reinterpret_cast<V *> (mat->data.ptr + i * sizeof(V))[c] = static_cast<V> (t);
        }
    }
    return s_ptr;
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
