/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_MATRIX_T1C1_H
#define NICTA_OCV_MATRIX_MATRIX_T1C1_H

#include "ocv_matrix/detail/initializer.h"
#include "ocv_matrix/detail/xml/file.h"
#include "ocv_matrix/extents.h"
#include "ocv_matrix/matrix_T0C1.h"
#include "ocv_matrix/matrix_type.h"
#include "ocv_matrix/models/vector_model.h"
#include "ocv_matrix/native_type.h"

#include <opencv/cxcore.h>
#include <opencv/cxtypes.h>

#include <boost/mpl/assert.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

#include <initializer_list>
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


    template <typename T, unsigned int C>
    class matrix <T, 1, C, 1> : public matrix<T, 0, C, 1>
    {
    public:

        explicit matrix(const nicta::ocv::matrix::extents<1>& e,
                        T* data = 0,
                        nicta::ocv::matrix::VectorModel model = nicta::ocv::matrix::Row);

        matrix(const nicta::ocv::matrix::extents<1>& e,
               std::initializer_list<T>&& data,
               nicta::ocv::matrix::VectorModel model = nicta::ocv::matrix::Row);

        matrix(const nicta::ocv::matrix::extents<1>& e,
               T value,
               nicta::ocv::matrix::VectorModel model = nicta::ocv::matrix::Row);

        matrix(const nicta::ocv::matrix::matrix<T, 1, C, 1>& m);

        matrix(const ::CvMat* m) throw (std::invalid_argument, std::bad_exception);
        matrix(const char* filename);

        void
        save_as(const char* filename) const;


        matrix<T, 1, C, 1>&
        operator=(const nicta::ocv::matrix::matrix<T, 1, C, 1>& rhs) throw (std::domain_error);

        typename nicta::ocv::matrix::matrix_type<T, 1, C, 1>::type &
        operator[](unsigned int i) throw (std::out_of_range);

        const typename nicta::ocv::matrix::matrix_type<T, 1, C, 1>::type &
        operator[](unsigned int i) const throw (std::out_of_range);

        nicta::ocv::matrix::matrix<T, 0, C, 1> &
        operator()(unsigned int i) throw (std::out_of_range);

        const nicta::ocv::matrix::matrix<T, 0, C, 1> &
        operator()(unsigned int i) const throw (std::out_of_range);

        unsigned int extent(unsigned int i) const throw (std::out_of_range);
        void extents(nicta::ocv::matrix::extents<1>& ext) const throw (std::invalid_argument);

        template <typename nicta::ocv::matrix::Native N = nicta::ocv::matrix::Handle>
        typename nicta::ocv::matrix::native_type<1, N>::type &
        native_handle() throw ();

        template <typename nicta::ocv::matrix::Native N = nicta::ocv::matrix::Handle>
        const typename nicta::ocv::matrix::native_type<1, N>::type &
        native_handle() const throw ();


        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 1, C, 1> >
        transpose() const throw ();

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 1, C, 1> >
        reshape() const throw ();


    protected:


        BOOST_STATIC_ASSERT (C > 0);
        BOOST_STATIC_ASSERT (C <= CV_CN_MAX);
        BOOST_STATIC_ASSERT (1 <= CV_MAX_DIM);
    };

} } } // namespace nicta::ocv::matrix



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 1
                          > :: 
matrix(const nicta::ocv::matrix::extents<1>& e,
       T* data,
       nicta::ocv::matrix::VectorModel model)
: matrix<T, 0, C, 1>(e, static_cast<T *> (data), model)
{
    // Constructor - yes! 1-D matrix:  1xN (row) --or--  Nx1 (column) matrix types allowed!
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 1
                          > :: 
matrix(const nicta::ocv::matrix::extents<1>& e,
       std::initializer_list<T>&& data,
       nicta::ocv::matrix::VectorModel model)
: matrix<T, 0, C, 1>(*nicta::ocv::matrix::detail::initializer::template initializer<T, 1, C, 1>(e, data, model))
{
    // Constructor - yes! 1-D matrix:  1xN (row) --or--  Nx1 (column) matrix types allowed!
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 1
                          > ::
matrix(const nicta::ocv::matrix::extents<1>& e,
       T value,
       nicta::ocv::matrix::VectorModel model)
: matrix<T, 0, C, 1>(e, value, model)
{
    // Constructor - yes! 1-D matrix:  1xN (row) --or--  Nx1 (column) matrix types allowed!
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 1
                          > :: 
matrix(const nicta::ocv::matrix::matrix<T, 1, C, 1>& m)
: matrix<T, 0, C, 1>(m)
{
    // Copy Constructor - yes! 1-D matrix:  1xN (row) --or--  Nx1 (column) matrix types allowed!
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 1
                          > ::
matrix(const ::CvMat* m) throw (std::invalid_argument, std::bad_exception)
: matrix<T, 0, C, 1>(m)
{
    // Constructor - yes! 1-D matrix:  1xN (row) --or--  Nx1 (column) matrix types allowed!
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 1
                          > ::
matrix(const char* filename)
: matrix<T, 0, C, 1>(*nicta::ocv::matrix::detail::xml::template read_xml<T, 1, C, 1>(filename))
{
    // Constructor - load matrix from file
    // NOTE:  file format should be either of XML [www.w3c.org/XML] or YAML [www.yaml.org] format
}


template < typename T
         , unsigned int C
         >
void
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 1
                          > ::
save_as(const char* filename) const
{
    // Save matrix to file
    // NOTE:  will save as XML [www.w3c.org/XML] or YAML [www.yaml.org] format, whatever OpenCV supports for writing of matrices to file

    ::cvSave(filename, this->matrix<T, 0, C, 1>::mtx_);
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix<T, 1, C, 1> &
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 1
                          > ::
operator=(const nicta::ocv::matrix::matrix<T, 1, C, 1>& rhs) throw (std::domain_error)
{
    // Assignment Operator
    // NOTE:  allowed only when:  [ D == U ]

    if (this != &rhs)
    {
        if (this->matrix<T, 0, C, 1>::model_ != rhs.matrix<T, 0, C, 1>::model_)
        {
            throw std::domain_error("cannot assign vectors with different models");
        }
        if (!this->init_hdr_)
        {
            ::cvReleaseMat(&this->matrix<T, 0, C, 1>::mtx_);
        }
        else
        {
            this->matrix<T, 0, C, 1>::init_hdr_ = !this->matrix<T, 0, C, 1>::init_hdr_;
        }
        this->matrix<T, 0, C, 1>::mtx_ = ::cvCloneMat(rhs.matrix<T, 0, C, 1>::mtx_);
        this->matrix<T, 0, C, 1>::ptr_ = this->matrix<T, 0, C, 1>::mtx_->data.ptr;
    }
    return *this;
}


template < typename T
         , unsigned int C
         >
typename nicta::ocv::matrix::matrix_type< T
                                        , 1
                                        , C
                                        , 1
                                        >::type &
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 1
                          > :: 
operator[](unsigned int i) throw (std::out_of_range)
{
    // Set matrix[i][j]..[n] element
    // NOTE:  Sparse matrices not supported (that'd be another class)!

    this->matrix<T, 0, C, 1>::ptr_ = this->matrix<T, 0, C, 1>::mtx_->data.ptr;
    if (this->matrix<T, 0, C, 1>::model_ == nicta::ocv::matrix::Row)
    {
        if (i >= static_cast<unsigned int> (this->matrix<T, 0, C, 1>::mtx_->cols))
        {
            throw std::out_of_range("index out of range");
        }
        this->matrix<T, 0, C, 1>::ptr_ += this->matrix<T, 0, C, 1>::reshaped_ * (i * this->matrix<T, 0, C, 1>::mtx_->step);
        this->matrix<T, 0, C, 1>::ptr_ += !this->matrix<T, 0, C, 1>::reshaped_ * (i * (C * sizeof(T)));
    }
    else // nicta::ocv::matrix::Column
    {
        if (i >= static_cast<unsigned int> (this->matrix<T, 0, C, 1>::mtx_->rows))
        {
            throw std::out_of_range("index out of range");
        }
        this->matrix<T, 0, C, 1>::ptr_ += !this->matrix<T, 0, C, 1>::reshaped_ * (i * this->matrix<T, 0, C, 1>::mtx_->step);
        this->matrix<T, 0, C, 1>::ptr_ += this->matrix<T, 0, C, 1>::reshaped_ * (i * (C * sizeof(T)));
    }
    return *static_cast<nicta::ocv::matrix::matrix<T, 0, C, 1> *> (this);
}


template < typename T
         , unsigned int C
         >
const typename nicta::ocv::matrix::matrix_type< T
                                              , 1
                                              , C
                                              , 1
                                              >::type &
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 1
                          > :: 
operator[](unsigned int i) const throw (std::out_of_range)
{
    // Get matrix[i][j]..[n] element
    // NOTE:  Sparse matrices not supported (that'd be another class)!

    this->matrix<T, 0, C, 1>::ptr_ = this->matrix<T, 0, C, 1>::mtx_->data.ptr;
    if (this->matrix<T, 0, C, 1>::model_ == nicta::ocv::matrix::Row)
    {
        if (i >= static_cast<unsigned int> (this->matrix<T, 0, C, 1>::mtx_->cols))
        {
            throw std::out_of_range("index out of range");
        }
        this->matrix<T, 0, C, 1>::ptr_ += this->matrix<T, 0, C, 1>::reshaped_ * (i * this->matrix<T, 0, C, 1>::mtx_->step);
        this->matrix<T, 0, C, 1>::ptr_ += !this->matrix<T, 0, C, 1>::reshaped_ * (i * (C * sizeof(T)));
    }
    else // nicta::ocv::matrix::Column
    {
        if (i >= static_cast<unsigned int> (this->matrix<T, 0, C, 1>::mtx_->rows))
        {
            throw std::out_of_range("index out of range");
        }
        this->matrix<T, 0, C, 1>::ptr_ += !this->matrix<T, 0, C, 1>::reshaped_ * (i * this->matrix<T, 0, C, 1>::mtx_->step);
        this->matrix<T, 0, C, 1>::ptr_ += this->matrix<T, 0, C, 1>::reshaped_ * (i * (C * sizeof(T)));
    }
    return *static_cast<const nicta::ocv::matrix::matrix<T, 0, C, 1> *> (this);
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix<T, 0, C, 1> &
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 1
                          > ::
operator()(unsigned int i) throw (std::out_of_range)
{
    // Set matrix(i, j, .., n) element
    // NOTE:  Sparse matrices not supported (that'd be another class)!

    this->matrix<T, 0, C, 1>::ptr_ = this->matrix<T, 0, C, 1>::mtx_->data.ptr;
    if (this->matrix<T, 0, C, 1>::model_ == nicta::ocv::matrix::Row)
    {
        if (i >= static_cast<unsigned int> (this->matrix<T, 0, C, 1>::mtx_->cols))
        {
            throw std::out_of_range("index out of range");
        }
        this->matrix<T, 0, C, 1>::ptr_ += this->matrix<T, 0, C, 1>::reshaped_ * (i * this->matrix<T, 0, C, 1>::mtx_->step);
        this->matrix<T, 0, C, 1>::ptr_ += !this->matrix<T, 0, C, 1>::reshaped_ * (i * (C * sizeof(T)));
    }
    else // nicta::ocv::matrix::Column
    {
        if (i >= static_cast<unsigned int> (this->matrix<T, 0, C, 1>::mtx_->rows))
        {
            throw std::out_of_range("index out of range");
        }
        this->matrix<T, 0, C, 1>::ptr_ += !this->matrix<T, 0, C, 1>::reshaped_ * (i * this->matrix<T, 0, C, 1>::mtx_->step);
        this->matrix<T, 0, C, 1>::ptr_ += this->matrix<T, 0, C, 1>::reshaped_ * (i * (C * sizeof(T)));
    }
    return *static_cast<nicta::ocv::matrix::matrix<T, 0, C, 1> *> (this);
}


template < typename T
         , unsigned int C
         >
const nicta::ocv::matrix::matrix<T, 0, C, 1> &
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 1
                          > ::
operator()(unsigned int i) const throw (std::out_of_range)
{
    // Get matrix(i, j, .., n) element
    // NOTE:  Sparse matrices not supported (that'd be another class)!

    this->matrix<T, 0, C, 1>::ptr_ = this->matrix<T, 0, C, 1>::mtx_->data.ptr;
    if (this->matrix<T, 0, C, 1>::model_ == nicta::ocv::matrix::Row)
    {
        if (i >= static_cast<unsigned int> (this->matrix<T, 0, C, 1>::mtx_->cols))
        {
            throw std::out_of_range("index out of range");
        }
        this->matrix<T, 0, C, 1>::ptr_ += this->matrix<T, 0, C, 1>::reshaped_ * (i * this->matrix<T, 0, C, 1>::mtx_->step);
        this->matrix<T, 0, C, 1>::ptr_ += !this->matrix<T, 0, C, 1>::reshaped_ * (i * (C * sizeof(T)));
    }
    else // nicta::ocv::matrix::Column
    {
        if (i >= static_cast<unsigned int> (this->matrix<T, 0, C, 1>::mtx_->rows))
        {
            throw std::out_of_range("index out of range");
        }
        this->matrix<T, 0, C, 1>::ptr_ += !this->matrix<T, 0, C, 1>::reshaped_ * (i * this->matrix<T, 0, C, 1>::mtx_->step);
        this->matrix<T, 0, C, 1>::ptr_ += this->matrix<T, 0, C, 1>::reshaped_ * (i * (C * sizeof(T)));
    }
    return *static_cast<const nicta::ocv::matrix::matrix<T, 0, C, 1> *> (this);
}


template < typename T
         , unsigned int C
         >
unsigned int
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 1
                          > ::
extent(unsigned int i) const throw (std::out_of_range)
{
    // Get size of matrix dimension 'i'
    // NOTE:  indexed from zero!
    // NOTE:  if a ROW-representation [1 x Cols] -->  so we return number of columns here (unless this->reshaped_ == true, then return 1)!
    // NOTE:  if a COL-representation [Rows x 1] -->  so we return number of rows here (unless this->reshaped_ == true, then return 1)!

    if (i)
    {
        throw std::out_of_range("index out of range");
    }
    if (this->matrix<T, 0, C, 1>::model_ == nicta::ocv::matrix::Row)
    {
        return ::cvGetDimSize(this->matrix<T, 0, C, 1>::mtx_, !this->matrix<T, 0, C, 1>::reshaped_);
    }
    return ::cvGetDimSize(this->matrix<T, 0, C, 1>::mtx_, !!this->matrix<T, 0, C, 1>::reshaped_);
}


template < typename T
         , unsigned int C
         >
void
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 1
                          > ::
extents(nicta::ocv::matrix::extents<1>& e) const throw (std::invalid_argument)
{
    // Get sizes of matrix dimensions in range
    // NOTE:  indexed from zero!
    // NOTE:  if a ROW-representation [1 x Cols] -->  so we return number of columns here (unless this->reshaped_ == true, then return 1)!
    // NOTE:  if a COL-representation [Rows x 1] -->  so we return number of rows here (unless this->reshaped_ == true, then return 1)!

    if (e.size())
    {
        throw std::invalid_argument("input parameter inappropriately constructed - did you not construct a null extents object?");
    }
    if (this->matrix<T, 0, C, 1>::model_ == nicta::ocv::matrix::Row)
    {
        e = ::cvGetDimSize(this->matrix<T, 0, C, 1>::mtx_, !this->matrix<T, 0, C, 1>::reshaped_);
    }
    else
    {
        e = ::cvGetDimSize(this->matrix<T, 0, C, 1>::mtx_, !!this->matrix<T, 0, C, 1>::reshaped_);
    }
}


template < typename T
         , unsigned int C
         >
template <typename nicta::ocv::matrix::Native N>
typename nicta::ocv::matrix::native_type<1, N>::type &
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 1
                          > ::
native_handle() throw ()
{
    // Return underlying openCV data-type

    return this->matrix<T, 0, C, 1>::template native_handle<N>();
}


template < typename T
         , unsigned int C
         >
template <typename nicta::ocv::matrix::Native N>
const typename nicta::ocv::matrix::native_type<1, N>::type &
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 1
                          > ::
native_handle() const throw ()
{
    // Return underlying openCV data-type

    return this->matrix<T, 0, C, 1>::template native_handle<N>();
}


template < typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 1, C, 1> >
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 1
                          > ::
transpose() const throw ()
{
    // Transpose the matrix

    // BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));

    int model = !static_cast<int> (this->matrix<T, 0, C, 1>::model_);
    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 1, C, 1> >
         s_ptr(new nicta::ocv::matrix::matrix<T, 1, C, 1> (nicta::ocv::matrix::extents<1>(this->extent(0)),
                                                                                          0,
                                                                                          static_cast<nicta::ocv::matrix::VectorModel> (model)));
    ::cvTranspose(this->matrix<T, 0, C, 1>::mtx_, s_ptr->matrix<T, 0, C, 1>::mtx_);
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 1, C, 1> >
nicta::ocv::matrix::matrix< T
                          , 1
                          , C
                          , 1
                          > ::
reshape() const throw ()
{
    // Reshape the matrix from  [Rows x Cols].C  ->  [rows x cols'].CC  -- having differnt number of rows, and/or cols, and/or channels
    // NOTE:  we return a **VIEW** into this matrix and ***NOT*** a new matrix!!
    // NOTE:  only makes sense if D == U !!
    //
    // SPECIAL CASE:  here we only allow reshaping to go from-and-to:  [Rows = 1, Cols].C  <-->  [Rows, Cols = 1].C
    //                ..hence channels can't change, and we only go from being a row-vector to being a column-vector (and vice-versa)
    //
    // NOTE:  we use the more general ::cvReshapeMatND to get around the idiotic openCV constraint with ::cvReshape that [CC < 4]
    //

    nicta::ocv::matrix::extents<2> e;
    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 1, C, 1> > s_ptr(new nicta::ocv::matrix::matrix<T, 1, C, 1>());

    if (this->matrix<T, 0, C, 1>::model_ == nicta::ocv::matrix::Row)
    {
        if (!this->matrix<T, 0, C, 1>::reshaped_)
        {
            e = this->matrix<T, 1, C, 1>::extent(), 1;
        }
        else
        {
            e = 1, this->matrix<T, 1, C, 1>::extent();
        }
        s_ptr->matrix<T, 0, C, 1>::mtx_ = static_cast< ::CvMat * > (::cvReshapeMatND(this->matrix<T, 0, C, 1>::mtx_,
                                                                                     sizeof(s_ptr->matrix<T, 0, C, 1>::header_),
                                                                                     &s_ptr->matrix<T, 0, C, 1>::header_,
                                                                                     static_cast<int> (0),
                                                                                     static_cast<int> (2),
                                                                                     e()));
    }
    else // nicta::ocv::matrix::Column
    {
        if (!this->matrix<T, 0, C, 1>::reshaped_)
        {
            e = 1, this->matrix<T, 1, C, 1>::extent();
        }
        else
        {
            e = this->matrix<T, 1, C, 1>::extent(), 1;
        }
        s_ptr->matrix<T, 0, C, 1>::mtx_ = static_cast< ::CvMat * > (::cvReshapeMatND(this->matrix<T, 0, C, 1>::mtx_,
                                                                                     sizeof(s_ptr->matrix<T, 0, C, 1>::header_),
                                                                                     &s_ptr->matrix<T, 0, C, 1>::header_,
                                                                                     static_cast<int> (0),
                                                                                     static_cast<int> (2),
                                                                                     e()));
    }
    s_ptr->matrix<T, 0, C, 1>::ptr_ = s_ptr->matrix<T, 0, C, 1>::mtx_->data.ptr;
    s_ptr->matrix<T, 0, C, 1>::reshaped_ = !this->matrix<T, 0, C, 1>::reshaped_;
    return s_ptr;
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
