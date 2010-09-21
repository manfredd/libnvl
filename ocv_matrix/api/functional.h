/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_FUNCTIONAL_H
#define NICTA_OCV_MATRIX_FUNCTIONAL_H

#include "ocv_matrix/matrix.h"
#include "ocv_matrix/matrix_T1C1.h"
#include "ocv_matrix/matrix_T2C2.h"
#include "ocv_matrix/native_type.h"

#include <opencv/cxcore.h>
#include <opencv/cxtypes.h>

#include <functional>
#include <numeric>
#include <stdexcept>


namespace nicta {
namespace ocv {
namespace matrix {

    template < typename Functor
             , typename T
             , unsigned int D
             , unsigned int C
             >
    void
    matrix_op(nicta::ocv::matrix::matrix<T, D, C, D>& m,
              const Functor& ftor
             ) throw ();

    template < typename Functor
             , typename T
             , unsigned int D
             , unsigned int C
             >
    void
    matrix_op(const nicta::ocv::matrix::matrix<T, D, C, D>& m,
              Functor& ftor
             ) throw ();

    template < typename Functor
             , typename T
             , unsigned int D
             , unsigned int C
             >
    void
    matrix_op(nicta::ocv::matrix::matrix<T, D, C, D>& left,
              nicta::ocv::matrix::matrix<T, D, C, D>& right,
              const Functor& ftor
             ) throw (std::length_error);

    template < typename Functor
             , typename T
             , unsigned int D
             , unsigned int C
             >
    void
    matrix_op(nicta::ocv::matrix::matrix<T, D, C, D>& left,
              const nicta::ocv::matrix::matrix<T, D, C, D>& right,
              const Functor& ftor
             ) throw (std::length_error);

    template < typename Functor
             , typename T
             , unsigned int D
             , unsigned int C
             >
    void
    matrix_op(const nicta::ocv::matrix::matrix<T, D, C, D>& left,
              const nicta::ocv::matrix::matrix<T, D, C, D>& right,
              Functor& ftor
             ) throw (std::length_error);





    template < typename Functor
             , typename T
             , unsigned int C
             >
    void
    matrix_op(nicta::ocv::matrix::matrix<T, 2, C, 2>& m,
              const Functor& ftor
             ) throw ();

    template < typename Functor
             , typename T
             , unsigned int C
             >
    void
    matrix_op(const nicta::ocv::matrix::matrix<T, 2, C, 2>& m,
              Functor& ftor
             ) throw ();

    template < typename Functor
             , typename T
             , unsigned int C
             >
    void
    matrix_op(nicta::ocv::matrix::matrix<T, 2, C, 2>& left,
              nicta::ocv::matrix::matrix<T, 2, C, 2>& right,
              const Functor& ftor
             ) throw (std::length_error);

    template < typename Functor
             , typename T
             , unsigned int C
             >
    void
    matrix_op(nicta::ocv::matrix::matrix<T, 2, C, 2>& left,
              const nicta::ocv::matrix::matrix<T, 2, C, 2>& right,
              const Functor& ftor
             ) throw (std::length_error);

    template < typename Functor
             , typename T
             , unsigned int C
             >
    void
    matrix_op(const nicta::ocv::matrix::matrix<T, 2, C, 2>& left,
              const nicta::ocv::matrix::matrix<T, 2, C, 2>& right,
              Functor& ftor
             ) throw (std::length_error);





    template < typename Functor
             , typename T
             , unsigned int C
             >
    void
    matrix_op(nicta::ocv::matrix::matrix<T, 1, C, 1>& m,
              const Functor& ftor
             ) throw ();

    template < typename Functor
             , typename T
             , unsigned int C
             >
    void
    matrix_op(const nicta::ocv::matrix::matrix<T, 1, C, 1>& m,
              Functor& ftor
             ) throw ();

    template < typename Functor
             , typename T
             , unsigned int C
             >
    void
    matrix_op(nicta::ocv::matrix::matrix<T, 1, C, 1>& left,
              nicta::ocv::matrix::matrix<T, 1, C, 1>& right,
              const Functor& ftor
             ) throw (std::length_error);

    template < typename Functor
             , typename T
             , unsigned int C
             >
    void
    matrix_op(nicta::ocv::matrix::matrix<T, 1, C, 1>& left,
              const nicta::ocv::matrix::matrix<T, 1, C, 1>& right,
              const Functor& ftor
             ) throw (std::length_error);

    template < typename Functor
             , typename T
             , unsigned int C
             >
    void
    matrix_op(const nicta::ocv::matrix::matrix<T, 1, C, 1>& left,
              const nicta::ocv::matrix::matrix<T, 1, C, 1>& right,
              Functor& ftor
             ) throw (std::length_error);


} } } // namespace nicta::ocv::matrix



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename Functor
         , typename T
         , unsigned int C
         , unsigned int D
         >
void
nicta::ocv::matrix ::
matrix_op(nicta::ocv::matrix::matrix<T, D, C, D>& m,
          const Functor& ftor
         ) throw ()
{
    // Apply ftor to each element of the matrix

    ::CvMatND* mtx = m.template native_handle< >();
    int* d = new int[mtx->dims];
    ::cvGetDims(mtx, &d[0]);

    unsigned long int size = std::accumulate(d,
                                             d + mtx->dims,
                                             1,
                                             std::multiplies<int>());
    delete[] d;
    for (unsigned long int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            T& e = reinterpret_cast<T *> (mtx->data.ptr + i * sizeof(T))[c];
            ftor(e);
        }
    }
}


template < typename Functor
         , typename T
         , unsigned int C
         , unsigned int D
         >
void
nicta::ocv::matrix ::
matrix_op(const nicta::ocv::matrix::matrix<T, D, C, D>& m,
          Functor& ftor
         ) throw ()
{
    // Apply ftor to each element of the matrix
    // NOTE:  ftor is stateful

    const ::CvMatND* mtx = m.template native_handle< >();
    int* d = new int[mtx->dims];
    ::cvGetDims(mtx, &d[0]);

    unsigned long int size = std::accumulate(d,
                                             d + mtx->dims,
                                             1,
                                             std::multiplies<int>());
    delete[] d;
    for (unsigned long int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            const T& e = reinterpret_cast<T *> (mtx->data.ptr + i * sizeof(T))[c];
            ftor(e);
        }
    }
}


template < typename Functor
         , typename T
         , unsigned int C
         , unsigned int D
         >
void
nicta::ocv::matrix ::
matrix_op(nicta::ocv::matrix::matrix<T, D, C, D>& left,
          nicta::ocv::matrix::matrix<T, D, C, D>& right,
          const Functor& ftor
         ) throw (std::length_error)
{
    // Apply ftor to each element of both matrices

    ::CvMatND* lhs = left.template native_handle< >();
    ::CvMatND* rhs = right.template native_handle< >();
    if (lhs->dims != rhs->dims)
    {
        throw std::length_error("matrix dimension counts differ");
    }

    int* d1 = new int[lhs->dims];
    int* d2 = new int[rhs->dims];
    ::cvGetDims(lhs, &d1[0]);
    ::cvGetDims(rhs, &d2[0]);

    std::pair<int*, int*> p = std::mismatch(d1,
                                            d1 + lhs->dims - 1,
                                            d2,
                                            std::equal_to<int>());
    if (*p.first != *p.second)
    {
        delete[] d2;
        delete[] d1;
        throw std::length_error("matrix dimension lengths differ");
    }
    unsigned long int size = std::accumulate(d1,
                                             d1 + lhs->dims,
                                             1,
                                             std::multiplies<int>());
    delete[] d2;
    delete[] d1;
    for (unsigned long int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            T& e1 = reinterpret_cast<T *> (lhs->data.ptr + i * sizeof(T))[c];
            T& e2 = reinterpret_cast<T* > (rhs->data.ptr + i * sizeof(T))[c];
            ftor(e1, e2);
        }
    }
}


template < typename Functor
         , typename T
         , unsigned int C
         , unsigned int D
         >
void
nicta::ocv::matrix ::
matrix_op(nicta::ocv::matrix::matrix<T, D, C, D>& left,
          const nicta::ocv::matrix::matrix<T, D, C, D>& right,
          const Functor& ftor
         ) throw (std::length_error)
{
    // Apply ftor to each element of both matrices

    ::CvMatND* lhs = left.template native_handle< >();
    const ::CvMatND* rhs = right.template native_handle< >();
    if (lhs->dims != rhs->dims)
    {
        throw std::length_error("matrix dimension counts differ");
    }

    int* d1 = new int[lhs->dims];
    int* d2 = new int[rhs->dims];
    ::cvGetDims(lhs, &d1[0]);
    ::cvGetDims(rhs, &d2[0]);

    std::pair<int*, int*> p = std::mismatch(d1,
                                            d1 + lhs->dims - 1,
                                            d2,
                                            std::equal_to<int>());
    if (*p.first != *p.second)
    {
        delete[] d2;
        delete[] d1;
        throw std::length_error("matrix dimension lengths differ");
    }
    unsigned long int size = std::accumulate(d1,
                                             d1 + lhs->dims,
                                             1,
                                             std::multiplies<int>());
    delete[] d2;
    delete[] d1;
    for (unsigned long int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            T& e1 = reinterpret_cast<T *> (lhs->data.ptr + i * sizeof(T))[c];
            const T& e2 = reinterpret_cast<T* > (rhs->data.ptr + i * sizeof(T))[c];
            ftor(e1, e2);
        }
    }
}


template < typename Functor
         , typename T
         , unsigned int C
         , unsigned int D
         >
void
nicta::ocv::matrix ::
matrix_op(const nicta::ocv::matrix::matrix<T, D, C, D>& left,
          const nicta::ocv::matrix::matrix<T, D, C, D>& right,
          Functor& ftor
         ) throw (std::length_error)
{
    // Apply ftor to each element of both matrices
    // NOTE:  ftor is stateful

    const ::CvMatND* lhs = left.template native_handle< >();
    const ::CvMatND* rhs = right.template native_handle< >();
    if (lhs->dims != rhs->dims)
    {
        throw std::length_error("matrix dimension counts differ");
    }

    int* d1 = new int[lhs->dims];
    int* d2 = new int[rhs->dims];
    ::cvGetDims(lhs, &d1[0]);
    ::cvGetDims(rhs, &d2[0]);

    std::pair<int*, int*> p = std::mismatch(d1,
                                            d1 + lhs->dims - 1,
                                            d2,
                                            std::equal_to<int>());
    if (*p.first != *p.second)
    {
        delete[] d2;
        delete[] d1;
        throw std::length_error("matrix dimension lengths differ");
    }
    unsigned long int size = std::accumulate(d1,
                                             d1 + lhs->dims,
                                             1,
                                             std::multiplies<int>());
    delete[] d2;
    delete[] d1;
    for (unsigned long int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            const T& e1 = reinterpret_cast<T *> (lhs->data.ptr + i * sizeof(T))[c];
            const T& e2 = reinterpret_cast<T* > (rhs->data.ptr + i * sizeof(T))[c];
            ftor(e1, e2);
        }
    }
}







template < typename Functor
         , typename T
         , unsigned int C
         >
void
nicta::ocv::matrix :: 
matrix_op(nicta::ocv::matrix::matrix<T, 2, C, 2>& m,
          const Functor& ftor
         ) throw ()
{
    // Apply ftor to each element of the matrix

    ::CvMat* mtx = m.template native_handle< >();
    unsigned int size = mtx->rows * mtx->cols;
    for (unsigned int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            T& e = reinterpret_cast<T *> (mtx->data.ptr + i * sizeof(T))[c];
            ftor(e);
        }
    }
}


template < typename Functor
         , typename T
         , unsigned int C
         >
void
nicta::ocv::matrix ::
matrix_op(const nicta::ocv::matrix::matrix<T, 2, C, 2>& m,
          Functor& ftor
         ) throw ()
{
    // Apply ftor to each element of the matrix
    // NOTE:  ftor is stateful

    const ::CvMat* mtx = m.template native_handle< >();
    unsigned int size = mtx->rows * mtx->cols;
    for (unsigned int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            const T& e = reinterpret_cast<T *> (mtx->data.ptr + i * sizeof(T))[c];
            ftor(e);
        }
    }
}


template < typename Functor
         , typename T
         , unsigned int C
         >
void
nicta::ocv::matrix ::
matrix_op(nicta::ocv::matrix::matrix<T, 2, C, 2>& left,
          nicta::ocv::matrix::matrix<T, 2, C, 2>& right,
          const Functor& ftor
         ) throw (std::length_error)
{
    // Apply ftor to each element of both matrices

    ::CvMat* lhs = left.template native_handle< >();
    ::CvMat* rhs = right.template native_handle< >();
    if (lhs->rows != rhs->rows || lhs->cols != rhs->cols)
    {
        throw std::length_error("matrix dimension lengths differ");
    }

    unsigned int size = lhs->rows * lhs->cols;
    for (unsigned int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            T& e1 = reinterpret_cast<T *> (lhs->data.ptr + i * sizeof(T))[c];
            T& e2 = reinterpret_cast<T* > (rhs->data.ptr + i * sizeof(T))[c];
            ftor(e1, e2);
        }
    }
}


template < typename Functor
         , typename T
         , unsigned int C
         >
void
nicta::ocv::matrix ::
matrix_op(nicta::ocv::matrix::matrix<T, 2, C, 2>& left,
          const nicta::ocv::matrix::matrix<T, 2, C, 2>& right,
          const Functor& ftor
         ) throw (std::length_error)
{
    // Apply ftor to each element of both matrices

    ::CvMat* lhs = left.template native_handle< >();
    const ::CvMat* rhs = right.template native_handle< >();
    if (lhs->rows != rhs->rows || lhs->cols != rhs->cols)
    {
        throw std::length_error("matrix dimension lengths differ");
    }

    unsigned int size = lhs->rows * lhs->cols;
    for (unsigned int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            T& e1 = reinterpret_cast<T *> (lhs->data.ptr + i * sizeof(T))[c];
            const T& e2 = reinterpret_cast<T* > (rhs->data.ptr + i * sizeof(T))[c];
            ftor(e1, e2);
        }
    }
}


template < typename Functor
         , typename T
         , unsigned int C
         >
void
nicta::ocv::matrix ::
matrix_op(const nicta::ocv::matrix::matrix<T, 2, C, 2>& left,
          const nicta::ocv::matrix::matrix<T, 2, C, 2>& right,
          Functor& ftor
         ) throw (std::length_error)
{
    // Apply ftor to each element of both matrices
    // NOTE:  ftor is stateful

    const ::CvMat* lhs = left.template native_handle< >();
    const ::CvMat* rhs = right.template native_handle< >();
    if (lhs->rows != rhs->rows || lhs->cols != rhs->cols)
    {
        throw std::length_error("matrix dimension lengths differ");
    }

    unsigned int size = lhs->rows * lhs->cols;
    for (unsigned int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            const T& e1 = reinterpret_cast<T *> (lhs->data.ptr + i * sizeof(T))[c];
            const T& e2 = reinterpret_cast<T* > (rhs->data.ptr + i * sizeof(T))[c];
            ftor(e1, e2);
        }
    }
}







template < typename Functor
         , typename T
         , unsigned int C
         >
void
nicta::ocv::matrix ::
matrix_op(nicta::ocv::matrix::matrix<T, 1, C, 1>& m,
          const Functor& ftor
         ) throw ()
{
    // Apply ftor to each element of the matrix

    ::CvMat* mtx = m.template native_handle< >();
    unsigned int size = mtx->rows * mtx->cols;
    for (unsigned int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            T& e = reinterpret_cast<T *> (mtx->data.ptr + i * sizeof(T))[c];
            ftor(e);
        }
    }
}


template < typename Functor
         , typename T
         , unsigned int C
         >
void
nicta::ocv::matrix ::
matrix_op(const nicta::ocv::matrix::matrix<T, 1, C, 1>& m,
          Functor& ftor
         ) throw ()
{
    // Apply ftor to each element of the matrix
    // NOTE:  ftor is stateful

    const ::CvMat* mtx = m.template native_handle< >();
    unsigned int size = mtx->rows * mtx->cols;
    for (unsigned int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            T& e = reinterpret_cast<T *> (mtx->data.ptr + i * sizeof(T))[c];
            ftor(e);
        }
    }
}


template < typename Functor
         , typename T
         , unsigned int C
         >
void
nicta::ocv::matrix ::
matrix_op(nicta::ocv::matrix::matrix<T, 1, C, 1>& left,
          nicta::ocv::matrix::matrix<T, 1, C, 1>& right,
          const Functor& ftor
         ) throw (std::length_error)
{
    // Apply ftor to each element of both matrices

    ::CvMat* lhs = left.template native_handle< >();
    ::CvMat* rhs = right.template native_handle< >();
    if (lhs->rows != rhs->rows || lhs->cols != rhs->cols)
    {
        throw std::length_error("matrix dimension lengths differ");
    }

    unsigned int size = lhs->rows * lhs->cols;
    for (unsigned int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            T& e1 = reinterpret_cast<T *> (lhs->data.ptr + i * sizeof(T))[c];
            T& e2 = reinterpret_cast<T* > (rhs->data.ptr + i * sizeof(T))[c];
            ftor(e1, e2);
        }
    }
}


template < typename Functor
         , typename T
         , unsigned int C
         >
void
nicta::ocv::matrix ::
matrix_op(nicta::ocv::matrix::matrix<T, 1, C, 1>& left,
          const nicta::ocv::matrix::matrix<T, 1, C, 1>& right,
          const Functor& ftor
         ) throw (std::length_error)
{
    // Apply ftor to each element of both matrices

    ::CvMat* lhs = left.template native_handle< >();
    const ::CvMat* rhs = right.template native_handle< >();
    if (lhs->rows != rhs->rows || lhs->cols != rhs->cols)
    {
        throw std::length_error("matrix dimension lengths differ");
    }

    unsigned int size = lhs->rows * lhs->cols;
    for (unsigned int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            T& e1 = reinterpret_cast<T *> (lhs->data.ptr + i * sizeof(T))[c];
            const T& e2 = reinterpret_cast<T* > (rhs->data.ptr + i * sizeof(T))[c];
            ftor(e1, e2);
        }
    }
}


template < typename Functor
         , typename T
         , unsigned int C
         >
void
nicta::ocv::matrix ::
matrix_op(const nicta::ocv::matrix::matrix<T, 1, C, 1>& left,
          const nicta::ocv::matrix::matrix<T, 1, C, 1>& right,
          Functor& ftor
         ) throw (std::length_error)
{
    // Apply ftor to each element of both matrices
    // NOTE:  ftor is stateful

    const ::CvMat* lhs = left.template native_handle< >();
    const ::CvMat* rhs = right.template native_handle< >();
    if (lhs->rows != rhs->rows || lhs->cols != rhs->cols)
    {
        throw std::length_error("matrix dimension lengths differ");
    }

    unsigned int size = lhs->rows * lhs->cols;
    for (unsigned int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            const T& e1 = reinterpret_cast<T *> (lhs->data.ptr + i * sizeof(T))[c];
            const T& e2 = reinterpret_cast<T* > (rhs->data.ptr + i * sizeof(T))[c];
            ftor(e1, e2);
        }
    }
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
