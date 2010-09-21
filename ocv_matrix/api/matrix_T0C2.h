/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_MATRIX_T0C2_H
#define NICTA_OCV_MATRIX_MATRIX_T0C2_H

#include "ocv_matrix/extents.h"
#include "ocv_matrix/matrix_type.h"
#include "ocv_matrix/native_type.h"

#include <opencv/cxcore.h>
#include <opencv/cxtypes.h>

#include <boost/mpl/at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/static_assert.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits/is_floating_point.hpp>

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
    class matrix <T, 0, C, 2>
    {
    public:

        typename nicta::ocv::matrix::matrix_type<T, 0, C, 2>::type &
        operator()(unsigned int channel = 0) throw (std::out_of_range);

        const typename nicta::ocv::matrix::matrix_type<T, 0, C, 2>::type &
        operator()(unsigned int channel = 0) const throw (std::out_of_range);

        bool operator==(const nicta::ocv::matrix::matrix<T, 0, C, 2>& rhs) const throw ();
        bool operator!=(const nicta::ocv::matrix::matrix<T, 0, C, 2>& rhs) const throw ();

        unsigned int channels() const throw ();

        bool is_view() const throw ();

        template <typename nicta::ocv::matrix::Native N = nicta::ocv::matrix::Handle>
        typename nicta::ocv::matrix::native_type<2, N>::type &
        native_handle() throw ();

        template <typename nicta::ocv::matrix::Native N = nicta::ocv::matrix::Handle>
        const typename nicta::ocv::matrix::native_type<2, N>::type &
        native_handle() const throw ();


        void set(T value) throw ();
        void negate() throw ();
        void zero() throw ();
        void ones() throw ();
        void identity(T value = static_cast<T> (1)) throw ();


    protected:

        ~matrix() throw ();

        matrix() throw ();
        explicit matrix(const nicta::ocv::matrix::extents<2>& e, T* data = 0);
        matrix(const nicta::ocv::matrix::extents<2>& e, T value);
        explicit matrix(const nicta::ocv::matrix::matrix<T, 0, C, 2>& m);
        explicit matrix(const ::CvMat* m) throw (std::invalid_argument, std::bad_exception);



        ::CvMat* mtx_;
        mutable unsigned char* ptr_;

        bool init_hdr_;
        ::CvMat header_;

        // unfortunate work around for friend-failures
        boost::tuples::tuple< ::CvMat&                // header_
                            , ::CvMat*&               // mtx_
                            , unsigned char*&         // ptr_
                            > tuple_;

    private:

        struct mtype
        {
            /* Used to determine the underlying openCV matrix type, given the fundamental type T */

            typedef boost::mpl::map< boost::mpl::pair<unsigned char, boost::mpl::int_<CV_8U> >
                                   , boost::mpl::pair<signed char, boost::mpl::int_<CV_8S> >
                                   , boost::mpl::pair<unsigned short, boost::mpl::int_<CV_16U> >
                                   , boost::mpl::pair<signed short, boost::mpl::int_<CV_16S> >
                                   , boost::mpl::pair<int, boost::mpl::int_<CV_32S> >
                                   , boost::mpl::pair<float, boost::mpl::int_<CV_32F> >
                                   , boost::mpl::pair<double, boost::mpl::int_<CV_64F> >
                                   > m_type;

            enum
            {
                type = CV_MAKETYPE((boost::mpl::at<typename nicta::ocv::matrix::matrix<T, 0, C, 2>::mtype::m_type, T>::type::value), C)
            };
        };


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
                                        , 0
                                        , C
                                        , 2
                                        >::type &
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > :: 
operator()(unsigned int channel) throw (std::out_of_range)
{
    // Set matrix[i][j]..[n](channel) element
    // NOTE:  Sparse matrices not supported (that'd be another class)!

    if (channel >= C)
    {
        throw std::out_of_range("index out of range");
    }

    return reinterpret_cast<T *> (this->ptr_)[channel];
}


template < typename T
         , unsigned int C
         >
const typename nicta::ocv::matrix::matrix_type< T
                                              , 0
                                              , C
                                              , 2
                                              >::type &
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > :: 
operator()(unsigned int channel) const throw (std::out_of_range)
{
    // Get matrix[i][j]..[n](channel) element
    // NOTE:  Sparse matrices not supported (that'd be another class)!

    if (channel >= C)
    {
        throw std::out_of_range("index out of range");
    }

    return reinterpret_cast<T *> (this->ptr_)[channel];
}


template < typename T
         , unsigned int C
         >
bool
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > :: 
operator==(const nicta::ocv::matrix::matrix<T, 0, C, 2>& rhs) const throw ()
{
    // Canonical Op== :  Query if matrices are equal (in their entirety!)

    if (this->mtx_->rows != rhs.mtx_->rows) return false;
    if (this->mtx_->cols != rhs.mtx_->cols) return false;

    unsigned int size = this->mtx_->rows * this->mtx_->cols;
    for (unsigned int i = 0; i != size; ++i)
    {
        for (unsigned int c = 0; c != C * size; c += size)
        {
            T v1 = reinterpret_cast<T *> (this->mtx_->data.ptr + i * sizeof(T))[c];
            T v2 = reinterpret_cast<T* > (rhs.mtx_->data.ptr + i * sizeof(T))[c];
            if (v1 != v2) return false;
        }
    }
    return true;
}


template < typename T
         , unsigned int C
         >
bool
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > :: 
operator!=(const nicta::ocv::matrix::matrix<T, 0, C, 2>& rhs) const throw ()
{
    // Canonical Op!= :  Query if matrices are not equal

    return !this->nicta::ocv::matrix::matrix<T, 0, C, 2>::operator==(rhs);
}


template < typename T
         , unsigned int C
         >
unsigned int
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > ::
channels() const throw ()
{
    // Return number of channels

    return C;
}


template < typename T
         , unsigned int C
         >
bool
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > ::
is_view() const throw ()
{
    // Are we a view or a copy in our own right..

    return this->init_hdr_;
}


template < typename T
         , unsigned int C
         >
template <typename nicta::ocv::matrix::Native N>
typename nicta::ocv::matrix::native_type<2, N>::type &
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > ::
native_handle() throw ()
{
    // Return underlying openCV data-type

    return this->tuple_.template get<static_cast<int> (N)>();
}


template < typename T
         , unsigned int C
         >
template <typename nicta::ocv::matrix::Native N>
const typename nicta::ocv::matrix::native_type<2, N>::type &
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > ::
native_handle() const throw ()
{
    // Return underlying openCV data-type

    return this->tuple_.template get<static_cast<int> (N)>();
}


template < typename T
         , unsigned int C
         >
void
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > ::
set(T value) throw ()
{
    // Set all elements elements to 'value'

    ::cvSet(this->mtx_, ::cvScalarAll(value), 0);
}


template < typename T
         , unsigned int C
         >
void
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > ::
negate() throw ()
{
    // Negate all elements

    BOOST_MPL_ASSERT(( boost::is_floating_point<T> ));

    ::CvMat* ones = ::cvCloneMat(this->mtx_);
    ::cvSet(ones, ::cvScalarAll(1), 0);
    ::cvMul(this->mtx_, ones, this->mtx_, -1.0);
    ::cvReleaseMat(&ones);
}


template < typename T
         , unsigned int C
         >
void
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > :: 
zero() throw ()
{
    // Set all elements to zero

    ::cvSetZero(this->mtx_);
}


template < typename T
         , unsigned int C
         >
void
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > ::
ones() throw ()
{
    // Set all elements to one

    ::cvSet(this->mtx_, ::cvScalarAll(1), 0);
}


template < typename T
         , unsigned int C
         >
void
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > :: 
identity(T value) throw ()
{
    // Force into an identity matrix, having 'value' along elements -- where ith & jth element *indices* equal

    ::cvSetIdentity(this->mtx_, ::cvRealScalar(static_cast<double> (value)));
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > :: 
~matrix() throw ()
{
    // Destructor (protected)

    if (!this->init_hdr_)
    {
        ::cvReleaseMat(&this->mtx_);
    }
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > :: 
matrix() throw ()
: mtx_(0)
, ptr_(0)
, init_hdr_(true)
, tuple_(header_, mtx_, ptr_)
{
    // Constructor (protected)
    // NOTE:  principally used for access to a mat-header

    // WARNING:  use with extreme care!
    // ..when eventually assign mtx_  **MUST**  also remember to do assignment:   ptr_ = mtx_->data.ptr;
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > :: 
matrix(const nicta::ocv::matrix::extents<2>& e, T* data)
: mtx_(!data ? ::cvCreateMat(e[0], e[1], nicta::ocv::matrix::matrix<T, 0, C, 2>::mtype::type)
             : ::cvInitMatHeader(&this->header_, e[0], e[1], nicta::ocv::matrix::matrix<T, 0, C, 2>::mtype::type, data))
, ptr_(mtx_->data.ptr)
, init_hdr_(!!data)
, tuple_(header_, mtx_, ptr_)
{
    // Constructor (protected)
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > ::
matrix(const nicta::ocv::matrix::extents<2>& e, T value)
: mtx_(::cvCreateMat(e[0], e[1], nicta::ocv::matrix::matrix<T, 0, C, 2>::mtype::type))
, ptr_(mtx_->data.ptr)
, init_hdr_(false)
, tuple_(header_, mtx_, ptr_)
{
    // Constructor (protected)

    ::cvSet(this->mtx_, ::cvScalarAll(value), 0);
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > :: 
matrix(const nicta::ocv::matrix::matrix<T, 0, C, 2>& m)
: mtx_(::cvCloneMat(m.mtx_))
, ptr_(mtx_->data.ptr)
, init_hdr_(false)
, tuple_(header_, mtx_, ptr_)
{
    // Copy Constructor (protected)
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 2
                          > ::
matrix(const ::CvMat* m) throw (std::invalid_argument, std::bad_exception)
: mtx_(::cvCloneMat(m))
, ptr_(mtx_->data.ptr)
, init_hdr_(false)
, tuple_(header_, mtx_, ptr_)
{
    // Constructor (protected)

    if (CV_MAT_CN(m->type) != C)
    {
        throw std::invalid_argument("unmatched channel count");
    }
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
