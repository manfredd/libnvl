/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_MATRIX_T1C2_H
#define NICTA_OCV_MATRIX_MATRIX_T1C2_H

#include "ocv_matrix/extents.h"
#include "ocv_matrix/matrix_T0C2.h"
#include "ocv_matrix/matrix_type.h"
#include "ocv_matrix/native_type.h"

#include <opencv/cxcore.h>
#include <opencv/cxtypes.h>

#include <boost/static_assert.hpp>

#include <cstddef>
#include <stdexcept>


namespace nicta {
namespace ocv {
namespace matrix {

    /* forward declaration */
    template < typename T
             , unsigned int D
             , unsigned int C
             , unsigned int U
             >
    class matrix;


    template <typename T, unsigned int C>
    class matrix <T, 1, C, 2> : public matrix<T, 0, C, 2>
    {
    public:

        typename nicta::ocv::matrix::matrix_type<T, 1, C, 2>::type &
        operator[](unsigned int i) throw (std::out_of_range);

        const typename nicta::ocv::matrix::matrix_type<T, 1, C, 2>::type &
        operator[](unsigned int i) const throw (std::out_of_range);

        unsigned int extent(unsigned int i) const throw (std::out_of_range);
        void extents(nicta::ocv::matrix::extents<1>& ext) const throw (std::invalid_argument);

        template <typename nicta::ocv::matrix::Native N = nicta::ocv::matrix::Handle>
        typename nicta::ocv::matrix::native_type<2, N>::type &
        native_handle() throw ();

        template <typename nicta::ocv::matrix::Native N = nicta::ocv::matrix::Handle>
        const typename nicta::ocv::matrix::native_type<2, N>::type &
        native_handle() const throw ();


    protected:

        matrix() throw ();
        explicit matrix(const nicta::ocv::matrix::extents<2>& e, T* data = 0);
        matrix(const nicta::ocv::matrix::extents<2>& e, T value);
        explicit matrix(const nicta::ocv::matrix::matrix<T, 1, C, 2>& m);
        explicit matrix(const ::CvMat* m) throw (std::invalid_argument, std::bad_exception);



        BOOST_STATIC_ASSERT (C > 0);
        BOOST_STATIC_ASSERT (C <= CV_CN_MAX);
        BOOST_STATIC_ASSERT (2 <= CV_MAX_DIM);
    };

} } } // namespace nicta::ocv::matrix


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename T
         , unsigned int C
         >
typename nicta::ocv::matrix::matrix_type< T
                                        , 1
                                        , C
                                        , 2
                                        >::type &
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 2
                          > :: 
operator[](unsigned int i) throw (std::out_of_range)
{
    // Set matrix[i][j]..[n] element
    // NOTE:  Sparse matrices not supported (that'd be another class)!

    if (i >= static_cast<unsigned int> (this->matrix<T, 0, C, 2>::mtx_->cols))
    {
        throw std::out_of_range("index out of range");
    }

    this->matrix<T, 0, C, 2>::ptr_ += i * (C * sizeof(T));
    return *static_cast<nicta::ocv::matrix::matrix<T, 0, C, 2> *> (this);
}


template < typename T
         , unsigned int C
         >
const typename nicta::ocv::matrix::matrix_type< T
                                              , 1
                                              , C
                                              , 2
                                              >::type &
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 2
                          > :: 
operator[](unsigned int i) const throw (std::out_of_range)
{
    // Get matrix[i][j]..[n] element
    // NOTE:  Sparse matrices not supported (that'd be another class)!

    if (i >= static_cast<unsigned int> (this->matrix<T, 0, C, 2>::mtx_->cols))
    {
        throw std::out_of_range("index out of range");
    }

    this->matrix<T, 0, C, 2>::ptr_ += i * (C * sizeof(T));
    return *static_cast<const nicta::ocv::matrix::matrix<T, 0, C, 2> *> (this);
}


template < typename T
         , unsigned int C
         >
unsigned int
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 2
                          > ::
extent(unsigned int i) const throw (std::out_of_range)
{
    // Get size of matrix dimension 'i'
    // NOTE:  indexed from zero!

    if (i)
    {
        throw std::out_of_range("index out of range");
    }
    return ::cvGetDimSize(this->matrix<T, 0, C, 2>::mtx_, 0);
}


template < typename T
         , unsigned int C
         >
void
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 2
                          > ::
extents(nicta::ocv::matrix::extents<1>& e) const throw (std::invalid_argument)
{
    // Get sizes of matrix dimensions in range

    if (e.size())
    {
        throw std::invalid_argument("input parameter inappropriately constructed - did you not construct a null extents object?");
    }
    e = ::cvGetDimSize(this->matrix<T, 0, C, 2>::mtx_, 0);
}


template < typename T
         , unsigned int C
         >
template <typename nicta::ocv::matrix::Native N>
typename nicta::ocv::matrix::native_type<2, N>::type &
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 2
                          > ::
native_handle() throw ()
{
    // Return underlying openCV data-type

    return this->matrix<T, 0, C, 2>::template native_handle<N>();
}


template < typename T
         , unsigned int C
         >
template <typename nicta::ocv::matrix::Native N>
const typename nicta::ocv::matrix::native_type<2, N>::type &
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 2
                          > ::
native_handle() const throw ()
{
    // Return underlying openCV data-type

    return this->matrix<T, 0, C, 2>::template native_handle<N>();
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 2
                          > :: 
matrix() throw ()
: matrix<T, 0, C, 2>()
{
    // Constructor (protected) - yes! 1-D matrix:  1xN (row) --or--  Nx1 (column) matrix types allowed!
    // NOTE:  for users to create 1-D matrices, they should create them from matrix<T, 1, C, 1> types, **NOT** matrix<T, 1, C, 2> types!
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 2
                          > :: 
matrix(const nicta::ocv::matrix::extents<2>& e, T* data)
: matrix<T, 0, C, 2>(e, static_cast<T *> (data))
{
    // Constructor (protected) - yes! 1-D matrix:  1xN (row) --or--  Nx1 (column) matrix types allowed!
    // NOTE:  for users to create 1-D matrices, they should create them from matrix<T, 1, C, 1> types, **NOT** matrix<T, 1, C, 2> types!
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 2
                          > ::
matrix(const nicta::ocv::matrix::extents<2>& e, T value)
: matrix<T, 0, C, 2>(e, value)
{
    // Constructor (protected) - yes! 1-D matrix:  1xN (row) --or--  Nx1 (column) matrix types allowed!
    // NOTE:  for users to create 1-D matrices, they should create them from matrix<T, 1, C, 1> types, **NOT** matrix<T, 1, C, 2> types!
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 2
                          > :: 
matrix(const nicta::ocv::matrix::matrix<T, 1, C, 2>& m)
: matrix<T, 0, C, 2>(m)
{
    // Copy Constructor (protected) - yes! 1-D matrix:  1xN (row) --or--  Nx1 (column) matrix types allowed!
    // NOTE:  for users to create 1-D matrices, they should create them from matrix<T, 1, C, 1> types, **NOT** matrix<T, 1, C, 2> types!
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 2
                          > ::
matrix(const ::CvMat* m) throw (std::invalid_argument, std::bad_exception)
: matrix<T, 0, C, 2>(m)
{
    // Constructor (protected) - yes! 1-D matrix:  1xN (row) --or--  Nx1 (column) matrix types allowed!
    // NOTE:  for users to create 1-D matrices, they should create them from matrix<T, 1, C, 1> types, **NOT** matrix<T, 1, C, 2> types!
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
