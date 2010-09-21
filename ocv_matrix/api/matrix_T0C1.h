/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_MATRIX_T0C1_H
#define NICTA_OCV_MATRIX_MATRIX_T0C1_H

#include "ocv_matrix/extents.h"
#include "ocv_matrix/matrix_type.h"
#include "ocv_matrix/models/vector_model.h"
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
    class matrix <T, 0, C, 1>
    {
    public:

        typename nicta::ocv::matrix::matrix_type<T, 0, C, 1>::type &
        operator()(unsigned int channel = 0) throw (std::out_of_range);

        const typename nicta::ocv::matrix::matrix_type<T, 0, C, 1>::type &
        operator()(unsigned int channel = 0) const throw (std::out_of_range);

        bool operator==(const nicta::ocv::matrix::matrix<T, 0, C, 1>& rhs) const throw ();
		bool operator!=(const nicta::ocv::matrix::matrix<T, 0, C, 1>& rhs) const throw ();

        unsigned int channels() const throw ();
        nicta::ocv::matrix::VectorModel model() const throw ();

        bool is_view() const throw ();

        template <typename nicta::ocv::matrix::Native N = nicta::ocv::matrix::Handle>
        typename nicta::ocv::matrix::native_type<1, N>::type &
        native_handle() throw ();

        template <typename nicta::ocv::matrix::Native N = nicta::ocv::matrix::Handle>
        const typename nicta::ocv::matrix::native_type<1, N>::type &
        native_handle() const throw ();


        void set(T value) throw ();
        void negate() throw ();
        void zero() throw ();
        void ones() throw ();
        void identity(T value = static_cast<T> (1)) throw ();


    protected:

        ~matrix() throw ();

        matrix() throw ();

        explicit matrix(const nicta::ocv::matrix::extents<1>& e,
                        T* data = 0,
                        nicta::ocv::matrix::VectorModel model = nicta::ocv::matrix::Row);

        matrix(const nicta::ocv::matrix::extents<1>& e,
               T value,
               nicta::ocv::matrix::VectorModel model = nicta::ocv::matrix::Row);

        explicit matrix(const nicta::ocv::matrix::matrix<T, 0, C, 1>& m);
        explicit matrix(const ::CvMat* m) throw (std::invalid_argument, std::bad_exception);



        ::CvMat* mtx_;
        mutable unsigned char* ptr_;

        bool init_hdr_;
        ::CvMat header_;

        bool reshaped_;
        nicta::ocv::matrix::VectorModel model_;

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
                type = CV_MAKETYPE((boost::mpl::at<typename nicta::ocv::matrix::matrix<T, 0, C, 1>::mtype::m_type, T>::type::value), C)
            };
        };


        BOOST_STATIC_ASSERT (C > 0);
        BOOST_STATIC_ASSERT (C <= CV_CN_MAX);
        BOOST_STATIC_ASSERT (1 <= CV_MAX_DIM);
    };

} } } // namespace nicta::ocv::matrix


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename T
         , unsigned int C
         >
typename nicta::ocv::matrix::matrix_type< T
                                        , 0
                                        , C
                                        , 1
                                        >::type &
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 1
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
                                              , 1
                                              >::type &
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 1
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
                          , 1
                          > ::
operator==(const nicta::ocv::matrix::matrix<T, 0, C, 1>& rhs) const throw ()
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
                          , 1
                          > ::
operator!=(const nicta::ocv::matrix::matrix<T, 0, C, 1>& rhs) const throw ()
{
    // Canonical Op!= :  Query if matrices are not equal

    return !this->nicta::ocv::matrix::matrix<T, 0, C, 1>::operator==(rhs);
}


template < typename T
         , unsigned int C
         >
unsigned int
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 1
                          > ::
channels() const throw ()
{
    // Return number of channels

    return C;
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::VectorModel
nicta::ocv::matrix::matrix< T
                  , 0
                  , C
                  , 1
                  > :: 
model() const throw ()
{
    // Return the shape-model:  either we are a Row-vector, or a Column-vector

    return this->model_;
}


template < typename T
         , unsigned int C
         >
bool
nicta::ocv::matrix::matrix< T
                  , 0
                  , C
                  , 1
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
typename nicta::ocv::matrix::native_type<1, N>::type &
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 1
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
const typename nicta::ocv::matrix::native_type<1, N>::type &
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 1
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
                          , 1
                          > ::
set(T value) throw ()
{
    // Set all elements to 'value'

    ::cvSet(this->mtx_, ::cvScalarAll(value), 0);
}


template < typename T
         , unsigned int C
         >
void
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 1
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
                          , 1
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
                          , 1
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
                          , 1
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
                          , 1
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
                          , 1
                          > :: 
matrix() throw ()
: mtx_(0)
, ptr_(0)
, init_hdr_(true)
, reshaped_(false)
, model_(nicta::ocv::matrix::Row)
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
                          , 1
                          > :: 
matrix(const nicta::ocv::matrix::extents<1>& e,
       T* data,
       nicta::ocv::matrix::VectorModel model
      )
: mtx_(!data ? (model == nicta::ocv::matrix::Row ? ::cvCreateMat(1, e[0], nicta::ocv::matrix::matrix<T, 0, C, 1>::mtype::type)
                                                 : ::cvCreateMat(e[0], 1, nicta::ocv::matrix::matrix<T, 0, C, 1>::mtype::type))
             : (model == nicta::ocv::matrix::Row ? ::cvInitMatHeader(&this->header_, 1, e[0], nicta::ocv::matrix::matrix<T, 0, C, 1>::mtype::type, data)
                                                 : ::cvInitMatHeader(&this->header_, e[0], 1, nicta::ocv::matrix::matrix<T, 0, C, 1>::mtype::type, data)))
, ptr_(mtx_->data.ptr)
, init_hdr_(!!data)
, reshaped_(false)
, model_(model)
, tuple_(header_, mtx_, ptr_)
{
    // Constructor (protected)
    // NOTE:  representing data as a ROW:  1 x Cols
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 1
                          > ::
matrix(const nicta::ocv::matrix::extents<1>& e,
       T value,
       nicta::ocv::matrix::VectorModel model
      )
: mtx_((model == nicta::ocv::matrix::Row ? ::cvCreateMat(1, e[0], nicta::ocv::matrix::matrix<T, 0, C, 1>::mtype::type)
                                         : ::cvCreateMat(e[0], 1, nicta::ocv::matrix::matrix<T, 0, C, 1>::mtype::type)))
, ptr_(mtx_->data.ptr)
, init_hdr_(false)
, reshaped_(false)
, model_(model)
, tuple_(header_, mtx_, ptr_)
{
    // Constructor (protected)
    // NOTE:  representing data as a ROW:  1 x Cols

    ::cvSet(this->mtx_, ::cvScalarAll(value), 0);
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 1
                          > :: 
matrix(const nicta::ocv::matrix::matrix<T, 0, C, 1>& m)
: mtx_(::cvCloneMat(m.mtx_))
, ptr_(mtx_->data.ptr)
, init_hdr_(false)
, reshaped_(false)
, model_(m.model_)
, tuple_(header_, mtx_, ptr_)
{
    // Copy Constructor (protected)
    // NOTE:  representing data as a ROW:  1 x Cols
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 0
                          , C
                          , 1
                          > ::
matrix(const ::CvMat* m) throw (std::invalid_argument, std::bad_exception)
: mtx_(::cvCloneMat(m))
, ptr_(mtx_->data.ptr)
, init_hdr_(false)
, reshaped_(false)
, model_(m->rows > m->cols ? nicta::ocv::matrix::Column : nicta::ocv::matrix::Row)
, tuple_(header_, mtx_, ptr_)
{
    // Constructor (protected)

    if (m->cols > 1 && m->rows > 1)
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
