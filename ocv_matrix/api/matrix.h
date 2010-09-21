/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_MATRIX_H
#define NICTA_OCV_MATRIX_MATRIX_H

#include "ocv_matrix/detail/initializer.h"
#include "ocv_matrix/detail/xml/file.h"
#include "ocv_matrix/extents.h"
#include "ocv_matrix/macros/idx_advance_nd.h"
#include "ocv_matrix/macros/rng_check_nd.h"
#include "ocv_matrix/matrix_T0CU.h"
#include "ocv_matrix/matrix_type.h"
#include "ocv_matrix/native_type.h"

#include <opencv/cxcore.h>
#include <opencv/cxtypes.h>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

#include <exception>
#include <initializer_list>
#include <stdexcept>


namespace nicta {
namespace ocv {
namespace matrix {

namespace detail {
namespace initializer {

    /* forward declarations */

    template < typename T
             , unsigned int D
             , unsigned int C
             , unsigned int U
             >
    typename boost::enable_if< typename boost::mpl::greater<boost::mpl::int_<D>, boost::mpl::int_<1> >::type
                             , boost::shared_ptr<nicta::ocv::matrix::matrix<T, D, C, U> >
                             >::type
    initializer(const nicta::ocv::matrix::extents<D>& e,
                std::initializer_list<T>&& il
               ) throw (std::length_error);


    template < typename T
             , unsigned int D
             , unsigned int C
             , unsigned int U
             >
    typename boost::enable_if< typename boost::mpl::equal_to<boost::mpl::int_<D>, boost::mpl::int_<1> >::type
                             , boost::shared_ptr<nicta::ocv::matrix::matrix<T, D, C, U> >
                             >::type
    initializer(const nicta::ocv::matrix::extents<D>& e,
                std::initializer_list<T>&& il,
                nicta::ocv::matrix::VectorModel model = nicta::ocv::matrix::Row
               ) throw (std::length_error);

} } // namespace detail::initializer


    template < typename T                  // type
             , unsigned int D              // dimensions
             , unsigned int C = 1          // channels
             , unsigned int U = D          // internal use:  so we may determine openCV 'type' at matrix<T, 0, C, U> ctor
             >
    class matrix : public matrix<T, D - 1, C, U>
    {
    public:

        explicit matrix(const nicta::ocv::matrix::extents<D>& e, T* data = 0);
        matrix(const nicta::ocv::matrix::extents<D>& e, std::initializer_list<T>&& data);
        matrix(const nicta::ocv::matrix::extents<D>& e, T value);
        matrix(const nicta::ocv::matrix::matrix<T, D, C, U>& m);
        matrix(const ::CvMatND* m) throw (std::invalid_argument, std::bad_exception);
        matrix(const char* filename);

        void
        save_as(const char* filename) const;


        matrix<T, D, C, U>&
        operator=(const nicta::ocv::matrix::matrix<T, D, C, U>& rhs) throw ();

        typename nicta::ocv::matrix::matrix_type<T, D, C, U>::type &
        operator[](unsigned int i) throw (std::out_of_range);

        const typename nicta::ocv::matrix::matrix_type<T, D, C, U>::type &
        operator[](unsigned int i) const throw (std::out_of_range);


#define OPERATOR(z, n, _)                                                                                         \
                                                                                                                  \
        nicta::ocv::matrix::matrix<T, 0, C, U> &                                                                  \
        operator()(BOOST_PP_ENUM_PARAMS(BOOST_PP_INC(n), unsigned int i)) throw (std::out_of_range);              \
                                                                                                                  \
        const nicta::ocv::matrix::matrix<T, 0, C, U> &                                                            \
        operator()(BOOST_PP_ENUM_PARAMS(BOOST_PP_INC(n), unsigned int i)) const throw (std::out_of_range);
/**/

#define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
#define BOOST_PP_LOCAL_LIMITS (1, CV_MAX_DIM - 1)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
#undef OPERATOR


        unsigned int extent(unsigned int i) const throw (std::out_of_range);
        void extents(nicta::ocv::matrix::extents<D>& e) const throw (std::invalid_argument);

        template <typename nicta::ocv::matrix::Native N = nicta::ocv::matrix::Handle>
        typename nicta::ocv::matrix::native_type<U, N>::type &
        native_handle() throw ();

        template <typename nicta::ocv::matrix::Native N = nicta::ocv::matrix::Handle>
        const typename nicta::ocv::matrix::native_type<U, N>::type &
        native_handle() const throw ();


        template < unsigned int DD
                 , unsigned int CC = C
                 >
        boost::shared_ptr<nicta::ocv::matrix::matrix<T, DD, CC, DD> >
        reshape(nicta::ocv::matrix::extents<DD>& e) const throw (std::length_error, std::bad_exception);


    protected:

        matrix() throw ();
        matrix(unsigned int d, const nicta::ocv::matrix::extents<U>& e, T* data = 0);


        BOOST_STATIC_ASSERT (C > 0);
        BOOST_STATIC_ASSERT (D <= U);
        BOOST_STATIC_ASSERT (U <= CV_MAX_DIM);
        BOOST_STATIC_ASSERT (C <= CV_CN_MAX);
    };

} } } // namespace nicta::ocv::matrix


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
nicta::ocv::matrix::matrix< T
                          , D
                          , C
                          , U
                          > :: 
matrix(const nicta::ocv::matrix::extents<D>& e, T* data)
: matrix<T, D - 1, C, U>(D, e, static_cast<T *> (data))
{
    // Constructor
}


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
nicta::ocv::matrix::matrix< T
                          , D
                          , C
                          , U
                          > :: 
matrix(const nicta::ocv::matrix::extents<D>& e, std::initializer_list<T>&& data)
: matrix<T, D - 1, C, U>(*nicta::ocv::matrix::detail::initializer::template initializer<T, D, C, U>(e, data))
{
    // Constructor
}


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
nicta::ocv::matrix::matrix< T
                          , D
                          , C
                          , U
                          > ::
matrix(const nicta::ocv::matrix::extents<D>& e, T value)
: matrix<T, D - 1, C, U>(D, e, value)
{
    // Constructor
}


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
nicta::ocv::matrix::matrix< T
                          , D
                          , C
                          , U
                          > :: 
matrix(const nicta::ocv::matrix::matrix<T, D, C, U>& m)
: matrix<T, D - 1, C, U>(m)
{
    // Copy Constructor
}


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
nicta::ocv::matrix::matrix< T
                          , D
                          , C
                          , U
                          > ::
matrix(const ::CvMatND* m) throw (std::invalid_argument, std::bad_exception)
: matrix<T, D - 1, C, U>(m)
{
    // Constructor
    // NOTE:  allowed only when:  [ D == U ]

    BOOST_MPL_ASSERT(( boost::mpl::bool_<D == U> ));
}


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
nicta::ocv::matrix::matrix< T
                          , D
                          , C
                          , U
                          > ::
matrix(const char* filename)
: matrix<T, D - 1, C, U>(*nicta::ocv::matrix::detail::xml::template read_xml<T, D, C, U>(filename))
{
    // Constructor - load matrix from file
    // NOTE:  file format should be either of XML [www.w3c.org/XML] or YAML [www.yaml.org] format
    //
    // NOTE:  allowed only when:  [ D == U ]
    //

    BOOST_MPL_ASSERT(( boost::mpl::bool_<D == U> ));
}


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
void
nicta::ocv::matrix::matrix< T
                          , D
                          , C
                          , U
                          > ::
save_as(const char* filename) const
{
    // Save matrix to file
    // NOTE:  will save as XML [www.w3c.org/XML] or YAML [www.yaml.org] format, whatever OpenCV supports for writing of matrices to file
    //
    // NOTE:  allowed only when:  [ D == U ]
    //

    BOOST_MPL_ASSERT(( boost::mpl::bool_<D == U> ));

    ::cvSave(filename, this->matrix<T, 0, C, U>::mtx_);
}


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
nicta::ocv::matrix::matrix<T, D, C, U> &
nicta::ocv::matrix::matrix< T
                          , D
                          , C
                          , U
                          > ::
operator=(const nicta::ocv::matrix::matrix<T, D, C, U>& rhs) throw ()
{
    // Assignment Operator
    // NOTE:  allowed only when:  [ D == U ]

    BOOST_MPL_ASSERT(( boost::mpl::bool_<D == U> ));

    if (this != &rhs)
    {
        if (!this->init_hdr_)
        {
            ::cvReleaseMatND(&this->matrix<T, 0, C, U>::mtx_);
        }
        else
        {
            this->matrix<T, 0, C, U>::init_hdr_ = !this->matrix<T, 0, C, U>::init_hdr_;
        }
        this->matrix<T, 0, C, U>::mtx_ = ::cvCloneMatND(rhs.matrix<T, 0, C, U>::mtx_);
        this->matrix<T, 0, C, U>::ptr_ = this->matrix<T, 0, C, U>::mtx_->data.ptr;
    }
    return *this;
}


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
typename nicta::ocv::matrix::matrix_type< T
                                        , D
                                        , C
                                        , U
                                        >::type &
nicta::ocv::matrix::matrix< T
                          , D
                          , C
                          , U
                          > :: 
operator[](unsigned int i) throw (std::out_of_range)
{
    // Set matrix[i][j]..[n] element
    // NOTE:  Sparse matrices not supported (that'd be another class)!

    if (i >= static_cast<unsigned int> (this->matrix<T, 0, C, U>::mtx_->dim[this->matrix<T, 0, C, U>::mtx_->dims - D].size))
    {
        throw std::out_of_range("index out of range");
    }

    if (D == this->matrix<T, 0, C, U>::mtx_->dims)
    {
        this->matrix<T, 0, C, U>::ptr_ = this->matrix<T, 0, C, U>::mtx_->data.ptr;
    }
    this->matrix<T, 0, C, U>::ptr_ += i * this->matrix<T, 0, C, U>::mtx_->dim[this->matrix<T, 0, C, U>::mtx_->dims - D].step;
    return *static_cast<nicta::ocv::matrix::matrix<T, D - 1, C, U> *> (this);
}


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
const typename nicta::ocv::matrix::matrix_type< T
                                              , D
                                              , C
                                              , U
                                              >::type &
nicta::ocv::matrix::matrix< T
                          , D
                          , C
                          , U
                          > :: 
operator[](unsigned int i) const throw (std::out_of_range)
{
    // Get matrix[i][j]..[n] element
    // NOTE:  Sparse matrices not supported (that'd be another class)!

    if (i >= static_cast<unsigned int> (this->matrix<T, 0, C, U>::mtx_->dim[this->matrix<T, 0, C, U>::mtx_->dims - D].size))
    {
        throw std::out_of_range("index out of range");
    }

    if (D == this->matrix<T, 0, C, U>::mtx_->dims)
    {
        this->matrix<T, 0, C, U>::ptr_ = this->matrix<T, 0, C, U>::mtx_->data.ptr;
    }
    this->matrix<T, 0, C, U>::ptr_ += i * this->matrix<T, 0, C, U>::mtx_->dim[this->matrix<T, 0, C, U>::mtx_->dims - D].step;
    return *static_cast<const nicta::ocv::matrix::matrix<T, D - 1, C, U> *> (this);
}



#define OPERATOR(z, n, _)                                                                                         \
                                                                                                                  \
template < typename T                                                                                             \
         , unsigned int D                                                                                         \
         , unsigned int C                                                                                         \
         , unsigned int U                                                                                         \
         >                                                                                                        \
nicta::ocv::matrix::matrix<T, 0, C, U> &                                                                          \
nicta::ocv::matrix::matrix< T                                                                                     \
                          , D                                                                                     \
                          , C                                                                                     \
                          , U                                                                                     \
                          > ::                                                                                    \
operator()(BOOST_PP_ENUM_PARAMS(BOOST_PP_INC(n), unsigned int i)) throw (std::out_of_range)                       \
{                                                                                                                 \
    /* Set matrix(i, j, .., n) element */                                                                         \
    /* NOTE:  Sparse matrices not supported (that'd be another class)! */                                         \
                                                                                                                  \
    BOOST_STATIC_ASSERT(( BOOST_PP_INC(n) == D ));                                                                \
                                                                                                                  \
    BOOST_PP_REPEAT(BOOST_PP_INC(n), RNG_CHECK_ND, i)                                                             \
    if (D == this->matrix<T, 0, C, U>::mtx_->dims)                                                                \
    {                                                                                                             \
        this->matrix<T, 0, C, U>::ptr_ = this->matrix<T, 0, C, U>::mtx_->data.ptr;                                \
    }                                                                                                             \
    BOOST_PP_REPEAT(BOOST_PP_INC(n), IDX_ADVANCE_ND, i)                                                           \
    return *static_cast<nicta::ocv::matrix::matrix<T, 0, C, U> *> (this);                                         \
}                                                                                                                 \
                                                                                                                  \
                                                                                                                  \
template < typename T                                                                                             \
         , unsigned int D                                                                                         \
         , unsigned int C                                                                                         \
         , unsigned int U                                                                                         \
         >                                                                                                        \
const nicta::ocv::matrix::matrix<T, 0, C, U> &                                                                    \
nicta::ocv::matrix::matrix< T                                                                                     \
                          , D                                                                                     \
                          , C                                                                                     \
                          , U                                                                                     \
                          > ::                                                                                    \
operator()(BOOST_PP_ENUM_PARAMS(BOOST_PP_INC(n), unsigned int i)) const throw (std::out_of_range)                 \
{                                                                                                                 \
    /* Get matrix(i, j, .., n) element */                                                                         \
    /* NOTE:  Sparse matrices not supported (that'd be another class)! */                                         \
                                                                                                                  \
    BOOST_STATIC_ASSERT(( BOOST_PP_INC(n) == D ));                                                                \
                                                                                                                  \
    BOOST_PP_REPEAT(BOOST_PP_INC(n), RNG_CHECK_ND, i)                                                             \
    if (D == this->matrix<T, 0, C, U>::mtx_->dims)                                                                \
    {                                                                                                             \
        this->matrix<T, 0, C, U>::ptr_ = this->matrix<T, 0, C, U>::mtx_->data.ptr;                                \
    }                                                                                                             \
    BOOST_PP_REPEAT(BOOST_PP_INC(n), IDX_ADVANCE_ND, i)                                                           \
    return *static_cast<const nicta::ocv::matrix::matrix<T, 0, C, U> *> (this);                                   \
}
/**/

#define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
#define BOOST_PP_LOCAL_LIMITS (1, CV_MAX_DIM - 1)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
#undef OPERATOR



template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
unsigned int
nicta::ocv::matrix::matrix< T
                          , D
                          , C
                          , U
                          > :: 
extent(unsigned int i) const throw (std::out_of_range)
{
    // Get size of matrix dimension 'i'
    // NOTE:  indexed from zero!

    if (i >= D)
    {
        throw std::out_of_range("index out of range");
    }
    return ::cvGetDimSize(this->matrix<T, 0, C, U>::mtx_, i + U - D);
}


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
void
nicta::ocv::matrix::matrix< T
                          , D
                          , C
                          , U
                          > :: 
extents(nicta::ocv::matrix::extents<D>& e) const throw (std::invalid_argument)
{
    // Get sizes of matrix dimensions in range

    if (e.size())
    {
        throw std::invalid_argument("input parameter inappropriately constructed - did you not construct a null extents object?");
    }
    for (unsigned int i = U - D; i != U; ++i)
    {
        e = this->matrix<T, 0, C, U>::mtx_->dim[i].size;
    }
}


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
template <typename nicta::ocv::matrix::Native N>
typename nicta::ocv::matrix::native_type<U, N>::type &
nicta::ocv::matrix::matrix< T
                          , D
                          , C
                          , U
                          > :: 
native_handle() throw ()
{
    // Return underlying openCV data-type

    return this->matrix<T, 0, C, U>::template native_handle<N>();
}


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
template <typename nicta::ocv::matrix::Native N>
const typename nicta::ocv::matrix::native_type<U, N>::type &
nicta::ocv::matrix::matrix< T
                          , D
                          , C
                          , U
                          > ::
native_handle() const throw ()
{
    // Return underlying openCV data-type

    return this->matrix<T, 0, C, U>::template native_handle<N>();
}


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
template < unsigned int DD
         , unsigned int CC
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, DD, CC, DD> >
nicta::ocv::matrix::matrix< T
                          , D
                          , C
                          , U
                          > :: 
reshape(nicta::ocv::matrix::extents<DD>& e) const throw (std::length_error, std::bad_exception)
{
    // Reshape the matrix from  [D1 x D2 x .. x Dn].C  ->  [D1' x D2' x .. x Dm'].CC  -- having differnt number of dimensions, and/or channels
    // NOTE:  we return a **VIEW** into this matrix and ***NOT*** a new matrix!!
    // NOTE:  only makes sense if D == U !!

    BOOST_MPL_ASSERT(( boost::mpl::bool_<D == U> ));
    BOOST_STATIC_ASSERT(( CC > 0 ));
    BOOST_STATIC_ASSERT(( DD > 1 ));
    BOOST_STATIC_ASSERT(( DD <= CV_MAX_DIM ));
    BOOST_STATIC_ASSERT(( CC <= CV_CN_MAX ));

    // do some error checking:
    if (DD <= 2)
    {
        unsigned int rows  = e[0];
        unsigned int width = this->extent(1) * C;
        if (D != 2)
        {
            rows = width * this->matrix<T, D, C, U>::extent(0) / CC;
        }
        if (rows != this->matrix<T, D, C, U>::extent(0))
        {
            unsigned int size = width * this->matrix<T, D, C, U>::extent(0);
            width = size / rows;
            if (width * rows != size)
            {
                throw std::length_error("unsuitable dimension extents");
            }
        }
    }
    else if (DD == D)
    {
        unsigned int msize = this->extent(D - 1) * C;
        unsigned int size  = msize / CC;
        if (size * CC != msize)
        {
            throw std::length_error("unsuitable channel size");
        }
    }
    else // DD > 2 && DD != D
    {
        unsigned int size1 = 1;
        for (unsigned int i = 0; i != D; ++i)
        {
            size1 *= this->extent(i);
        }
        unsigned int size2 = 1;
        for (unsigned int i = 0; i != DD; ++i)
        {
            if (!e[i])
            {
                throw std::length_error("invalid dimension length");
            }
            size2 *= e[i];
        }

        if (size1 != size2)
        {
            throw std::length_error("element count in orginal and reshaped array differ");
        }
    }

    nicta::ocv::matrix::extents<D> ex;
    this->extents(ex);
    boost::shared_ptr<nicta::ocv::matrix::matrix<T, DD, CC, DD> > s_ptr(new nicta::ocv::matrix::matrix<T, DD, CC, DD>());
    if (CC == C && (DD == D && e == ex))
    {
        // no-op
        s_ptr->matrix<T, 0, CC, DD>::mtx_ = this->matrix<T, 0, C, D>::mtx_;
    }
    else if (CC == C && (DD != D || e != ex))
    {
        // reshape dims
        s_ptr->matrix<T, 0, CC, DD>::mtx_ = static_cast< ::CvMatND * > (::cvReshapeMatND(this->matrix<T, 0, CC, U>::mtx_,
                                                                                         sizeof(s_ptr->matrix<T, 0, CC, DD>::header_),
                                                                                         &s_ptr->matrix<T, 0, CC, DD>::header_,
                                                                                         static_cast<int> (0),
                                                                                         static_cast<int> (DD),
                                                                                         e()));
    }
    else if (CC != C && (DD == D && e == ex))
    {
        // reshape channels
        s_ptr->matrix<T, 0, CC, DD>::mtx_ = static_cast< ::CvMatND * > (::cvReshapeMatND(this->matrix<T, 0, CC, U>::mtx_,
                                                                                         sizeof(s_ptr->matrix<T, 0, CC, DD>::header_),
                                                                                         &s_ptr->matrix<T, 0, CC, DD>::header_,
                                                                                         static_cast<int> (CC),
                                                                                         static_cast<int> (0),
                                                                                         static_cast<int *> (0)));
    }
    else // (CC != C && (DD != D && e != ex))
    {
        // reshape dims & channels

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, DD, CC, DD> > temp(new nicta::ocv::matrix::matrix<T, DD, CC, DD>());
        temp->matrix<T, 0, CC, DD>::mtx_ = static_cast< ::CvMatND * > (::cvReshapeMatND(this->matrix<T, 0, C, U>::mtx_,
                                                                                        sizeof(temp->matrix<T, 0, CC, DD>::header_),
                                                                                        &temp->matrix<T, 0, CC, DD>::header_,
                                                                                        static_cast<int> (CC),
                                                                                        static_cast<int> (0),
                                                                                        static_cast<int *> (0)));
        temp->matrix<T, 0, CC, DD>::ptr_ = temp->matrix<T, 0, CC, DD>::mtx_->data.ptr;

        s_ptr->matrix<T, 0, CC, DD>::mtx_  = static_cast< ::CvMatND * > (::cvReshapeMatND(temp->matrix<T, 0, CC, DD>::mtx_,
                                                                                          sizeof(s_ptr->matrix<T, 0, CC, DD>::header_),
                                                                                          &s_ptr->matrix<T, 0, CC, DD>::header_,
                                                                                          static_cast<int> (0),
                                                                                          static_cast<int> (DD),
                                                                                          e()));
    }
    s_ptr->matrix<T, 0, CC, DD>::ptr_ = s_ptr->matrix<T, 0, CC, DD>::mtx_->data.ptr;
    return s_ptr;
}


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
nicta::ocv::matrix::matrix< T
                          , D
                          , C
                          , U
                          > :: 
matrix() throw ()
: matrix<T, D - 1, C, U>()
{
    // Constructor (protected)
}


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
nicta::ocv::matrix::matrix< T
                          , D
                          , C
                          , U
                          > :: 
matrix(unsigned int d, const nicta::ocv::matrix::extents<U>& e, T* data)
: matrix<T, D - 1, C, U>(d, e, data)
{
    // Constructor (protected)
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
