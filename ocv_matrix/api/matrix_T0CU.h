/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_MATRIX_T0CU_H
#define NICTA_OCV_MATRIX_MATRIX_T0CU_H

#include "ocv_matrix/extents.h"
#include "ocv_matrix/matrix_type.h"
#include "ocv_matrix/native_type.h"

#include <opencv/cxcore.h>
#include <opencv/cxtypes.h>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/static_assert.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits/is_floating_point.hpp>

#include <algorithm>
#include <functional>
#include <numeric>
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


    template < typename T
             , unsigned int C
             , unsigned int U
             >
    class matrix <T, 0, C, U>
    {
    public:

        typename nicta::ocv::matrix::matrix_type<T, 0, C, U>::type &
        operator()(unsigned int channel = 0) throw (std::out_of_range);

        const typename nicta::ocv::matrix::matrix_type<T, 0, C, U>::type &
        operator()(unsigned int channel = 0) const throw (std::out_of_range);

        bool operator==(const nicta::ocv::matrix::matrix<T, 0, C, U>& rhs) const throw ();
        bool operator!=(const nicta::ocv::matrix::matrix<T, 0, C, U>& rhs) const throw ();

        unsigned int channels() const throw ();

        bool is_view() const throw ();

        template <typename nicta::ocv::matrix::Native N = nicta::ocv::matrix::Handle>
        typename nicta::ocv::matrix::native_type<U, N>::type &
        native_handle() throw ();

        template <typename nicta::ocv::matrix::Native N = nicta::ocv::matrix::Handle>
        const typename nicta::ocv::matrix::native_type<U, N>::type &
        native_handle() const throw ();


        void set(T value) throw ();
        void negate() throw ();
        void zero() throw ();
        void ones() throw ();
        void identity(T value = static_cast<T> (1)) throw ();


    protected:

        ~matrix() throw ();

        matrix() throw ();
        matrix(unsigned int d, const nicta::ocv::matrix::extents<U>& e, T* data = 0);
        matrix(unsigned int d, const nicta::ocv::matrix::extents<U>& e, T value);
        explicit matrix(const nicta::ocv::matrix::matrix<T, 0, C, U>& m);
        explicit matrix(const ::CvMatND* m) throw (std::invalid_argument, std::bad_exception);



        ::CvMatND* mtx_;
        mutable unsigned char* ptr_;

        bool init_hdr_;
        ::CvMatND header_;

        // unfortunate work around for friend-failures
        boost::tuples::tuple< ::CvMatND&              // header_
                            , ::CvMatND*&             // mtx_
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
                type = CV_MAKETYPE((boost::mpl::at<typename nicta::ocv::matrix::matrix<T, 0, C, U>::mtype::m_type, T>::type::value), C)
            };
        };


        BOOST_STATIC_ASSERT (C <= CV_CN_MAX);
        BOOST_STATIC_ASSERT (U <= CV_MAX_DIM);
    };

} } } // namespace nicta::ocv::matrix


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename T
         , unsigned int C
         , unsigned int U
         >
typename nicta::ocv::matrix::matrix_type< T
                                        , 0
                                        , C
                                        , U
                                        >::type &
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
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
         , unsigned int U
         >
const typename nicta::ocv::matrix::matrix_type< T
                                              , 0
                                              , C
                                              , U
                                              >::type &
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
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
         , unsigned int U
         >
bool
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
                          > ::
operator==(const nicta::ocv::matrix::matrix<T, 0, C, U>& rhs) const throw ()
{
    // Canonical Op== :  Query if matrices are equal (in their entirety!)

    if (this->mtx_->dims != rhs.mtx_->dims) return false;

    int* d1 = new int[this->mtx_->dims];
    int* d2 = new int[rhs.mtx_->dims];
    ::cvGetDims(this->mtx_, &d1[0]);
    ::cvGetDims(rhs.mtx_, &d2[0]);

    std::pair<int*, int*> p = std::mismatch(d1,
                                            d1 + this->mtx_->dims - 1,
                                            d2,
                                            std::equal_to<int>());
    if (*p.first != *p.second)
    {
        delete[] d2;
        delete[] d1;
        return false;
    }
    unsigned long int size = std::accumulate(d1,
                                             d1 + this->mtx_->dims,
                                             1,
                                             std::multiplies<int>());
    delete[] d2;
    delete[] d1;
    for (unsigned long int i = 0; i != size; ++i)
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
         , unsigned int U
         >
bool
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
                          > ::
operator!=(const nicta::ocv::matrix::matrix<T, 0, C, U>& rhs) const throw ()
{
    // Canonical Op!= :  Query if matrices are not equal

    return !this->nicta::ocv::matrix::matrix<T, 0, C, U>::operator==(rhs);
}


template < typename T
         , unsigned int C
         , unsigned int U
         >
unsigned int
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
                          > ::
channels() const throw ()
{
    // Return number of channels

    return C;
}


template < typename T
         , unsigned int C
         , unsigned int U
         >
bool
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
                          > ::
is_view() const throw ()
{
    // Are we a view or a copy in our own right..

    return this->init_hdr_;
}


template < typename T
         , unsigned int C
         , unsigned int U
         >
template <typename nicta::ocv::matrix::Native N>
typename nicta::ocv::matrix::native_type<U, N>::type &
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
                          > ::
native_handle() throw ()
{
    // Return underlying openCV data-type

    return this->tuple_.template get<static_cast<int> (N)>();
}


template < typename T
         , unsigned int C
         , unsigned int U
         >
template <typename nicta::ocv::matrix::Native N>
const typename nicta::ocv::matrix::native_type<U, N>::type &
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
                          > ::
native_handle() const throw ()
{
    // Return underlying openCV data-type

    return this->tuple_.template get<static_cast<int> (N)>();
}


template < typename T
         , unsigned int C
         , unsigned int U
         >
void
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
                          > ::
set(T value) throw ()
{
    // Set all elements to 'value'

    ::cvSet(this->mtx_, ::cvScalarAll(value), 0);
}


template < typename T
         , unsigned int C
         , unsigned int U
         >
void
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
                          > ::
negate() throw ()
{
    // Negate all elements

    BOOST_MPL_ASSERT(( boost::is_floating_point<T> ));

    ::CvMatND* ones = ::cvCloneMatND(this->mtx_);
    ::cvSet(ones, ::cvScalarAll(1), 0);
    ::cvMul(this->mtx_, ones, this->mtx_, -1.0);
    ::cvReleaseMatND(&ones);
}


template < typename T
         , unsigned int C
         , unsigned int U
         >
void
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
                          > :: 
zero() throw ()
{
    // Set all elements to zero

    ::cvSetZero(this->mtx_);
}


template < typename T
         , unsigned int C
         , unsigned int U
         >
void
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
                          > ::
ones() throw ()
{
    // Set all elements to one

    ::cvSet(this->mtx_, ::cvScalarAll(1), 0);
}


template < typename T
         , unsigned int C
         , unsigned int U
         >
void
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
                          > :: 
identity(T value) throw ()
{
    // Force into an identity matrix, having 'value' along elements -- where ith & jth element *indices* equal

    ::cvSetIdentity(this->mtx_, ::cvRealScalar(static_cast<double> (value)));
}


template < typename T
         , unsigned int C
         , unsigned int U
         >
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
                          > :: 
~matrix() throw ()
{
    // Destructor (protected)

    if (!this->init_hdr_)
    {
        ::cvReleaseMatND(&this->mtx_);
    }
}


template < typename T
         , unsigned int C
         , unsigned int U
         >
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
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
         , unsigned int U
         >
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
                          > :: 
matrix(unsigned int d, const nicta::ocv::matrix::extents<U>& e, T* data)
: mtx_(!data ? ::cvCreateMatND(static_cast<int> (d), e(), nicta::ocv::matrix::matrix<T, 0, C, U>::mtype::type)
             : ::cvInitMatNDHeader(&this->header_, U, e(), nicta::ocv::matrix::matrix<T, 0, C, U>::mtype::type, data))
, ptr_(mtx_->data.ptr)
, init_hdr_(!!data)
, tuple_(header_, mtx_, ptr_)
{
    // Constructor (protected)
}


template < typename T
         , unsigned int C
         , unsigned int U
         >
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
                          > ::
matrix(unsigned int d, const nicta::ocv::matrix::extents<U>& e, T value)
: mtx_(::cvCreateMatND(static_cast<int> (d), e(), nicta::ocv::matrix::matrix<T, 0, C, U>::mtype::type))
, ptr_(mtx_->data.ptr)
, init_hdr_(false)
, tuple_(header_, mtx_, ptr_)
{
    // Constructor (protected)

    ::cvSet(this->mtx_, ::cvScalarAll(value), 0);
}


template < typename T
         , unsigned int C
         , unsigned int U
         >
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
                          > :: 
matrix(const nicta::ocv::matrix::matrix<T, 0, C, U>& m)
: mtx_(::cvCloneMatND(m.mtx_))
, ptr_(mtx_->data.ptr)
, init_hdr_(false)
, tuple_(header_, mtx_, ptr_)
{
    // Copy Constructor (protected)
}


template < typename T
         , unsigned int C
         , unsigned int U
         >
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , U
                          > ::
matrix(const ::CvMatND* m) throw (std::invalid_argument, std::bad_exception)
: mtx_(::cvCloneMatND(m))
, ptr_(mtx_->data.ptr)
, init_hdr_(false)
, tuple_(header_, mtx_, ptr_)
{
    // Constructor (protected)

    if (m->dims != U)
    {
        throw std::invalid_argument("unmatched dimension count");
    }
    if (CV_MAT_CN(m->type) != C)
    {
        throw std::invalid_argument("unmatched channel count");
    }
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
