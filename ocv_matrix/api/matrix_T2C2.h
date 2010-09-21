/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_MATRIX_T2C2_H
#define NICTA_OCV_MATRIX_MATRIX_T2C2_H

#include "ocv_matrix/detail/initializer.h"
#include "ocv_matrix/detail/xml/file.h"
#include "ocv_matrix/detail/exponential.h"
#include "ocv_matrix/detail/logarithm.h"
#include "ocv_matrix/detail/magnitude.h"
#include "ocv_matrix/detail/power.h"
#include "ocv_matrix/detail/reciprocal.h"
#include "ocv_matrix/extents.h"
#include "ocv_matrix/functional.h"
#include "ocv_matrix/matrix_T0C2.h"
#include "ocv_matrix/matrix_T1C2.h"
#include "ocv_matrix/matrix_type.h"
#include "ocv_matrix/models/decomposition_model.h"
#include "ocv_matrix/models/svd_model.h"
#include "ocv_matrix/native_type.h"

#include <fwd/as_image.h>
#include <fwd/as_matrix.h>
#include <nicta_common/quantity.h>

#include <opencv/cxcore.h>
#include <opencv/cxtypes.h>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/utility/enable_if.hpp>

#include <exception>
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


    template < typename T
             , unsigned int C
             >
    class matrix <T, 2, C, 2> : public matrix<T, 1, C, 2>
    {
    public:

        explicit matrix(const nicta::ocv::matrix::extents<2>& e, T* data = 0);
        matrix(const nicta::ocv::matrix::extents<2>& e, std::initializer_list<T>&& data);
        matrix(const nicta::ocv::matrix::extents<2>& e, T value);
        matrix(const nicta::ocv::matrix::matrix<T, 2, C, 2>& m);
        matrix(const ::CvMat* m) throw (std::invalid_argument, std::bad_exception);
        matrix(const char* filename);

        void
        save_as(const char* filename) const;


        matrix<T, 2, C, 2> &
        operator=(const nicta::ocv::matrix::matrix<T, 2, C, 2>& rhs) throw ();

        typename nicta::ocv::matrix::matrix_type<T, 2, C, 2>::type &
        operator[](unsigned int i) throw (std::out_of_range);

        const typename nicta::ocv::matrix::matrix_type<T, 2, C, 2>::type &
        operator[](unsigned int i) const throw (std::out_of_range);

        nicta::ocv::matrix::matrix<T, 0, C, 2> &
        operator()(unsigned int i, unsigned int j) throw (std::out_of_range);

        const nicta::ocv::matrix::matrix<T, 0, C, 2> &
        operator()(unsigned int i, unsigned int j) const throw (std::out_of_range);

        unsigned int rows() const throw ();
        unsigned int columns() const throw();

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
        rows(unsigned int i) const throw (std::out_of_range);

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
        columns(unsigned int i) const throw (std::out_of_range);

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
        rows(unsigned int begin,
             unsigned int end,
             nicta::quantity::step<unsigned int> step = nicta::quantity::step<unsigned int> (1)
            ) const throw (std::out_of_range);

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
        columns(unsigned int begin, unsigned int end) const throw (std::out_of_range);

        unsigned int extent(unsigned int i) const throw (std::out_of_range);
        void extents(nicta::ocv::matrix::extents<2>& e) const throw (std::invalid_argument);

        template <typename nicta::ocv::matrix::Native N = nicta::ocv::matrix::Handle>
        typename nicta::ocv::matrix::native_type<2, N>::type &
        native_handle() throw ();

        template <typename nicta::ocv::matrix::Native N = nicta::ocv::matrix::Handle>
        const typename nicta::ocv::matrix::native_type<2, N>::type &
        native_handle() const throw ();


        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
        operator*(nicta::ocv::matrix::matrix<T, 2>& rhs) const throw (std::domain_error, std::bad_exception);       // matrix multiplication


        double trace() const throw ();

        double determinant() const throw (std::domain_error);

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
        diagonal() const throw ();

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
        transpose() const throw (std::bad_exception);

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
        ATA() const;                                                                                                    // transpose(A) * A

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
        AAT() const;                                                                                                    // A * transpose(A)


        template <nicta::ocv::matrix::DecompositionModel M = nicta::ocv::matrix::LU>
        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
        inverse() const throw (std::domain_error, std::logic_error, std::bad_exception);

        template <nicta::ocv::matrix::SVDSourceModel M = nicta::ocv::matrix::Immutable>
        boost::shared_ptr< boost::tuples::tuple< boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >        // resulting svd matrix     [W]
                                               , boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >        // left orthogonal matrix   [U]
                                               , boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >        // right orthogonal matrix  [V]
                                               >
                         >
        svd(nicta::ocv::matrix::SVDModel m = nicta::ocv::matrix::Flat);

        template <nicta::ocv::matrix::DecompositionModel M = nicta::ocv::matrix::LU>
        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
        solve_sys(const nicta::ocv::matrix::matrix<T, 2>& b) const throw(std::domain_error, std::bad_exception);           // Solve:  Ax = b


        template < unsigned int DD
                 , unsigned int CC = C
                 >
        boost::shared_ptr<nicta::ocv::matrix::matrix<T, DD, CC, DD> >
        reshape(nicta::ocv::matrix::extents<DD>& e) const throw (std::length_error, std::bad_exception);



        // Element-wise Operatations:

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
        operator+(T value) const throw ();

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
        operator-(T value) const throw ();

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
        operator*(T value) const throw ();                                                                    // element-wise multiplicaiton

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
        operator/(T value) const throw (std::logic_error);


        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
        operator+(const nicta::ocv::matrix::matrix<T, 2>& rhs) const throw ();

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
        operator-(const nicta::ocv::matrix::matrix<T, 2>& rhs) const throw ();

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
        multiply(const nicta::ocv::matrix::matrix<T, 2>& rhs) const throw ();                                 // element-wise multiplication

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
        operator/(const nicta::ocv::matrix::matrix<T, 2>& rhs) const;




        template <typename T_>
        friend
        boost::shared_ptr<nicta::ocv::matrix::matrix<T_, 2> >
        abs(const nicta::ocv::matrix::matrix<T_, 2>& m) throw ();

        template <typename T_>
        friend
        boost::shared_ptr<nicta::ocv::matrix::matrix<T_, 2> >
        reciprocal(const nicta::ocv::matrix::matrix<T_, 2>& m, double multiplier) throw (std::domain_error);

        template <typename T_>
        friend
        boost::shared_ptr<nicta::ocv::matrix::matrix<T_, 2> >
        pow(const nicta::ocv::matrix::matrix<T_, 2>& m, double exponent) throw (std::domain_error);

        template <typename T_>
        friend
        boost::shared_ptr<nicta::ocv::matrix::matrix<T_, 2> >
        exp(const nicta::ocv::matrix::matrix<T_, 2>& m) throw ();

        template <typename T_>
        friend
        boost::shared_ptr<nicta::ocv::matrix::matrix<T_, 2> >
        log(const nicta::ocv::matrix::matrix<T_, 2>& m) throw (std::domain_error);





        template < typename Depth
                 , template <typename _> class Pixel
                 >
        friend
        boost::shared_ptr<nicta::vibe::image::image<Depth, Pixel> >
        as_image(const nicta::ocv::matrix::matrix< typename Depth::type
                                                 , 2
                                                 , Pixel<typename Depth::type>::components
                                                 , 2
                                                 >& matrix) throw (std::domain_error);

        template < typename T_
                 , template <typename _> class Pixel
                 >
        friend
        boost::shared_ptr< nicta::ocv::matrix::matrix< T_
                                                     , 2
                                                     , Pixel<typename nicta::vibe::image::depth<T_>::type>::components
                                                     , 2
                                                     >
                          >
        as_matrix(const nicta::vibe::image::image<typename nicta::vibe::image::depth<T_>::type, Pixel>& image) throw ();


    protected:

        matrix() throw ();



        BOOST_STATIC_ASSERT (C > 0);
        BOOST_STATIC_ASSERT (C <= CV_CN_MAX);
        BOOST_STATIC_ASSERT (2 <= CV_MAX_DIM);
    };
 


// ->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->


    template <typename T>
    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
    abs(const nicta::ocv::matrix::matrix<T, 2>& m) throw ()
    {
        // Take the absolute value of each element  [ | M | ]

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, 1, 2> >
             s_ptr(new nicta::ocv::matrix::matrix<T, 2, 1, 2> (nicta::ocv::matrix::extents<2>(m.extent(0), m.extent(1))));

        nicta::ocv::matrix::matrix_op(*s_ptr, m, nicta::ocv::matrix::detail::magnitude());
        return s_ptr;
    }


    template <typename T>
    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
    reciprocal(const nicta::ocv::matrix::matrix<T, 2>& m, double multiplier = 1.0) throw (std::domain_error)
    {
        // Operator (element-wise) inverse, times a constant:  [ k / mtx ]

        BOOST_MPL_ASSERT(( boost::is_floating_point<T> ));

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, 1, 2> >
             s_ptr(new nicta::ocv::matrix::matrix<T, 2, 1, 2> (nicta::ocv::matrix::extents<2>(m.extent(0), m.extent(1))));

        nicta::ocv::matrix::matrix_op(*s_ptr, m, nicta::ocv::matrix::detail::reciprocal(multiplier));
        return s_ptr;
    }


    template <typename T>
    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
    pow(const nicta::ocv::matrix::matrix<T, 2>& m, double exponent) throw (std::domain_error)
    {
        // Raise each element to an exponent  [ M^p ]

        BOOST_MPL_ASSERT(( boost::is_floating_point<T> ));

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, 1, 2> >
             s_ptr(new nicta::ocv::matrix::matrix<T, 2, 1, 2> (nicta::ocv::matrix::extents<2>(m.extent(0), m.extent(1))));

        nicta::ocv::matrix::matrix_op(*s_ptr, m, nicta::ocv::matrix::detail::power(exponent));
        return s_ptr;
    }


    template <typename T>
    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
    exp(const nicta::ocv::matrix::matrix<T, 2>& m) throw ()
    {
        // Take the exponential of each element  [ e^M ]

        BOOST_MPL_ASSERT(( boost::is_floating_point<T> ));

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, 1, 2> >
             s_ptr(new nicta::ocv::matrix::matrix<T, 2, 1, 2> (nicta::ocv::matrix::extents<2>(m.extent(0), m.extent(1))));

        nicta::ocv::matrix::matrix_op(*s_ptr, m, nicta::ocv::matrix::detail::exponential());
        return s_ptr;
    }


    template <typename T>
    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
    log(const nicta::ocv::matrix::matrix<T, 2>& m) throw (std::domain_error)
    {
        // Take the natural logarithm of each element  [ ln(M) ]

        BOOST_MPL_ASSERT(( boost::is_floating_point<T> ));

        boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, 1, 2> >
             s_ptr(new nicta::ocv::matrix::matrix<T, 2, 1, 2> (nicta::ocv::matrix::extents<2>(m.extent(0), m.extent(1))));

        nicta::ocv::matrix::matrix_op(*s_ptr, m, nicta::ocv::matrix::detail::logarithm());
        return s_ptr;
    }


// -<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<

} } } // namespace nicta::ocv::matrix



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
matrix(const nicta::ocv::matrix::extents<2>& e, T* data)
: matrix<T, 1, C, 2>(e, static_cast<T *> (data))
{
    // Constructor
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
matrix(const nicta::ocv::matrix::extents<2>& e, std::initializer_list<T>&& data)
: matrix<T, 1, C, 2>(*nicta::ocv::matrix::detail::initializer::template initializer<T, 2, C, 2>(e, data))
{
    // Constructor
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > ::
matrix(const nicta::ocv::matrix::extents<2>& e, T value)
: matrix<T, 1, C, 2>(e, value)
{
    // Constructor
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
matrix(const nicta::ocv::matrix::matrix<T, 2, C, 2>& m)
: matrix<T, 1, C, 2>(m)
{
    // Copy Constructor
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > ::
matrix(const ::CvMat* m) throw (std::invalid_argument, std::bad_exception)
: matrix<T, 1, C, 2>(m)
{
    // Constructor
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > ::
matrix(const char* filename)
: matrix<T, 1, C, 2>(*nicta::ocv::matrix::detail::xml::template read_xml<T, 2, C, 2>(filename))
{
    // Constructor - load matrix from file
    // NOTE:  file format should be either of XML [www.w3c.org/XML] or YAML [www.yaml.org] format
}


template < typename T
         , unsigned int C
         >
void
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > ::
save_as(const char* filename) const
{
    // Save matrix to file
    // NOTE:  will save as XML [www.w3c.org/XML] or YAML [www.yaml.org] format, whatever OpenCV supports for writing of matrices to file

    ::cvSave(filename, this->matrix<T, 0, C, 2>::mtx_);
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix<T, 2, C, 2> &
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
operator=(const nicta::ocv::matrix::matrix<T, 2, C, 2>& rhs) throw ()
{
    // Assignment Operator
    // NOTE:  allowed only when:  [ D == U ]

    if (this != &rhs)
    {
        if (!this->init_hdr_)
        {
            ::cvReleaseMat(&this->matrix<T, 0, C, 2>::mtx_);
        }
        else
        {
            this->matrix<T, 0, C, 2>::init_hdr_ = !this->matrix<T, 0, C, 2>::init_hdr_;
        }
        this->matrix<T, 0, C, 2>::mtx_ = ::cvCloneMat(rhs.matrix<T, 0, C, 2>::mtx_);
        this->matrix<T, 0, C, 2>::ptr_ = this->matrix<T, 0, C, 2>::mtx_->data.ptr;
    }
    return *this;
}


template < typename T
         , unsigned int C
         >
typename nicta::ocv::matrix::matrix_type< T
                                        , 2
                                        , C
                                        , 2
                                        >::type &
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
operator[](unsigned int i) throw (std::out_of_range)
{
    // Set matrix[i][j]..[n] element
    // NOTE:  Sparse matrices not supported (that'd be another class)!

    if (i >= static_cast<unsigned int> (this->matrix<T, 0, C, 2>::mtx_->rows))
    {
        throw std::out_of_range("index out of range");
    }

    this->matrix<T, 0, C, 2>::ptr_  = this->matrix<T, 0, C, 2>::mtx_->data.ptr;
    this->matrix<T, 0, C, 2>::ptr_ += i * this->matrix<T, 0, C, 2>::mtx_->step;
    return *static_cast<nicta::ocv::matrix::matrix<T, 1, C, 2> *> (this);
}


template < typename T
         , unsigned int C
         >
const typename nicta::ocv::matrix::matrix_type< T
                                              , 2
                                              , C
                                              , 2
                                              >::type &
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
operator[](unsigned int i) const throw (std::out_of_range)
{
    // Get matrix[i][j]..[n] element
    // NOTE:  Sparse matrices not supported (that'd be another class)!

    if (i >= static_cast<unsigned int> (this->matrix<T, 0, C, 2>::mtx_->rows))
    {
        throw std::out_of_range("index out of range");
    }

    this->matrix<T, 0, C, 2>::ptr_  = this->matrix<T, 0, C, 2>::mtx_->data.ptr;
    this->matrix<T, 0, C, 2>::ptr_ += i * this->matrix<T, 0, C, 2>::mtx_->step;
    return *static_cast<const nicta::ocv::matrix::matrix<T, 1, C, 2> *> (this);
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix<T, 0, C, 2> &
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > ::
operator()(unsigned int i, unsigned int j) throw (std::out_of_range)
{
    // Set matrix(i, j, .., n) element
    // NOTE:  Sparse matrices not supported (that'd be another class)!

    if (i >= static_cast<unsigned int> (this->matrix<T, 0, C, 2>::mtx_->rows) ||
        j >= static_cast<unsigned int> (this->matrix<T, 0, C, 2>::mtx_->cols))
    {
        throw std::out_of_range("index out of range");
    }

    this->matrix<T, 0, C, 2>::ptr_  = this->matrix<T, 0, C, 2>::mtx_->data.ptr;
    this->matrix<T, 0, C, 2>::ptr_ += i * this->matrix<T, 0, C, 2>::mtx_->step;
    this->matrix<T, 0, C, 2>::ptr_ += j * (C * sizeof(T));
    return *static_cast<nicta::ocv::matrix::matrix<T, 0, C, 2> *> (this);
}


template < typename T
         , unsigned int C
         >
const nicta::ocv::matrix::matrix<T, 0, C, 2> &
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > ::
operator()(unsigned int i, unsigned int j) const throw (std::out_of_range)
{
    // Get matrix(i, j, .., n) element
    // NOTE:  Sparse matrices not supported (that'd be another class)!

    if (i >= static_cast<unsigned int> (this->matrix<T, 0, C, 2>::mtx_->rows) ||
        j >= static_cast<unsigned int> (this->matrix<T, 0, C, 2>::mtx_->cols))
    {
        throw std::out_of_range("index out of range");
    }

    this->matrix<T, 0, C, 2>::ptr_  = this->matrix<T, 0, C, 2>::mtx_->data.ptr;
    this->matrix<T, 0, C, 2>::ptr_ += i * this->matrix<T, 0, C, 2>::mtx_->step;
    this->matrix<T, 0, C, 2>::ptr_ += j * (C * sizeof(T));
    return *static_cast<const nicta::ocv::matrix::matrix<T, 0, C, 2> *> (this);
}


template < typename T
         , unsigned int C
         >
unsigned int
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
rows() const throw ()
{
    // Return number of rows in this matrix

    return ::cvGetDimSize(this->matrix<T, 0, C, 2>::mtx_, 0);
}


template < typename T
         , unsigned int C
         >
unsigned int
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
columns() const throw ()
{
    // Return number of columns in this matrix

    return ::cvGetDimSize(this->matrix<T, 0, C, 2>::mtx_, 1);
}


template < typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
rows(unsigned int i) const throw (std::out_of_range)
{
    // Get the (zero indexed) ith row
    // NOTE:  we return a **VIEW** into this matrix and ***NOT*** a new matrix!!

    if (i >= this->rows())
    {
        throw std::out_of_range("index out of range");
    }

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> > s_ptr(new nicta::ocv::matrix::matrix<T, 2, C, 2> ());

    s_ptr->matrix<T, 0, C, 2>::mtx_ = ::cvGetRow(this->matrix<T, 0, C, 2>::mtx_,
                                                 &s_ptr->matrix<T, 0, C, 2>::header_,
                                                 i);
    s_ptr->matrix<T, 0, C, 2>::ptr_ = s_ptr->matrix<T, 0, C, 2>::mtx_->data.ptr;
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
columns(unsigned int i) const throw (std::out_of_range)
{
    // Get the (zero indexed) ith row
    // NOTE:  we return a **VIEW** into this matrix and ***NOT*** a new matrix!!

    if (i >= this->columns())
    {
        throw std::out_of_range("index out of range");
    }

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> > s_ptr(new nicta::ocv::matrix::matrix<T, 2, C, 2> ());

    s_ptr->matrix<T, 0, C, 2>::mtx_ = ::cvGetCol(this->matrix<T, 0, C, 2>::mtx_,
                                                 &s_ptr->matrix<T, 0, C, 2>::header_,
                                                 i);
    s_ptr->matrix<T, 0, C, 2>::ptr_ = s_ptr->matrix<T, 0, C, 2>::mtx_->data.ptr;
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
rows(unsigned int begin,
     unsigned int end,
     nicta::quantity::step<unsigned int> step
    ) const throw (std::out_of_range)
{
    // Get rows in range [begin, end], taking every nth row in the range (step)
    // NOTE:  users should provide (begin, end) as zero-based indexes
    //
    // NOTE:  we return a **VIEW** into this matrix and ***NOT*** a new matrix!!
    //

    if (begin >= this->rows() || begin > end)
    {
        throw std::out_of_range("index out of range");
    }

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> > s_ptr(new nicta::ocv::matrix::matrix<T, 2, C, 2> ());

    s_ptr->matrix<T, 0, C, 2>::mtx_ = ::cvGetRows(this->matrix<T, 0, C, 2>::mtx_,
                                                  &s_ptr->matrix<T, 0, C, 2>::header_,
                                                  begin,
                                                  end + 1,
                                                  step.operator()<int>());
    s_ptr->matrix<T, 0, C, 2>::ptr_ = s_ptr->matrix<T, 0, C, 2>::mtx_->data.ptr;
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
columns(unsigned int begin, unsigned int end) const throw (std::out_of_range)
{
    // Get all columns in range [begin, end]
    // NOTE:  users should provide (begin, end) as zero-based indexes
    //
    // NOTE:  we return a **VIEW** into this matrix and ***NOT*** a new matrix!!
    //

    if (begin >= this->columns() || begin > end)
    {
        throw std::out_of_range("index out of range");
    }

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> > s_ptr(new nicta::ocv::matrix::matrix<T, 2, C, 2> ());

    s_ptr->matrix<T, 0, C, 2>::mtx_ = ::cvGetCols(this->matrix<T, 0, C, 2>::mtx_,
                                                  &s_ptr->matrix<T, 0, C, 2>::header_,
                                                  begin,
                                                  end + 1);
    s_ptr->matrix<T, 0, C, 2>::ptr_ = s_ptr->matrix<T, 0, C, 2>::mtx_->data.ptr;
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
unsigned int
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
extent(unsigned int i) const throw (std::out_of_range)
{
    // Get size of matrix dimension 'i'
    // NOTE:  indexed from zero!

    if (i >= 2)
    {
        throw std::out_of_range("index out of range");
    }
    return ::cvGetDimSize(this->matrix<T, 0, C, 2>::mtx_, i);
}


template < typename T
         , unsigned int C
         >
void
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
extents(nicta::ocv::matrix::extents<2>& e) const throw (std::invalid_argument)
{
    // Get sizes of matrix dimensions in range

    if (e.size())
    {
        throw std::invalid_argument("input parameter inappropriately constructed - did you not construct a null extents object?");
    }
    e = ::cvGetDimSize(this->matrix<T, 0, C, 2>::mtx_, 0),
        ::cvGetDimSize(this->matrix<T, 0, C, 2>::mtx_, 1);
}


template < typename T
         , unsigned int C
         >
template <typename nicta::ocv::matrix::Native N>
typename nicta::ocv::matrix::native_type<2, N>::type &
nicta::ocv::matrix::matrix< T
                          , 2
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
                          , 2
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
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
operator*(nicta::ocv::matrix::matrix<T, 2>& rhs) const throw (std::domain_error, std::bad_exception)
{
    // Operator* (matrix-multiplication):  [ mtx * mtx] -- define multiplication between two matrices

    BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));
    BOOST_MPL_ASSERT(( boost::is_floating_point<T> ));

    if (this->extent(1) != rhs.extent(0))
    {
        throw std::domain_error("matricies inappropriately dimensioned");
    }

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
         s_ptr(new nicta::ocv::matrix::matrix<T, 2, C, 2> (nicta::ocv::matrix::extents<2>(this->extent(0), rhs.extent(1))));

    ::cvMatMul((this->template matrix<T, 0, C, 2>::mtx_),
               (rhs.template matrix<T, 0, C, 2>::mtx_),
               (s_ptr->template matrix<T, 0, C, 2>::mtx_));
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
double
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
trace() const throw ()
{
    // Return sum of diagonal elements

    BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));

    return ::cvTrace(this->matrix<T, 0, C, 2>::mtx_).val[0];
}


template < typename T
         , unsigned int C
         >
double
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
determinant() const throw (std::domain_error)
{
    // Compute the matrix determinant
    // NOTE:  T must be float or double!

    BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));
    BOOST_MPL_ASSERT(( boost::is_floating_point<T> ));

    if (this->extent(0) != this->extent(1))
    {
        throw std::domain_error("matrix must be square to compute determinant");
    }
    return ::cvDet(this->matrix<T, 0, C, 2>::mtx_);
}


template < typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
diagonal() const throw ()
{
    // Return the diagonal of a matrix (not necessarily a square matrix) -- where ith & jth element *indices* equal
    // NOTE:  we return a **VIEW** into this matrix and ***NOT*** a new matrix!!
    // NOTE:  a 2-D *COL* matrix is returned:  [N x 1] matrix

    // BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> > s_ptr(new nicta::ocv::matrix::matrix<T, 2, C, 2> ());

    s_ptr->matrix<T, 0, C, 2>::mtx_ = ::cvGetDiag(this->matrix<T, 0, C, 2>::mtx_,
                                                  &s_ptr->matrix<T, 0, C, 2>::header_,
                                                  0);
    s_ptr->matrix<T, 0, C, 2>::ptr_ = s_ptr->matrix<T, 0, C, 2>::mtx_->data.ptr;
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
transpose() const throw (std::bad_exception)
{
    // Transpose the matrix

    // BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
         s_ptr(new nicta::ocv::matrix::matrix<T, 2, C, 2> (nicta::ocv::matrix::extents<2>(this->extent(1), this->extent(0))));

    ::cvTranspose(this->matrix<T, 0, C, 2>::mtx_, s_ptr->matrix<T, 0, C, 2>::mtx_);
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
ATA() const
{
    // Computes:  [ transpose(A) * A ]
    //
    // A[m x n] ; Out[n x n]
    //
    // NOTE:  ::cvGEMM is buggy for ATA (always computes AAT instead)!
    //

    BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));
    BOOST_MPL_ASSERT(( boost::is_floating_point<T> ));

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
         s_ptr(new nicta::ocv::matrix::matrix<T, 2> (nicta::ocv::matrix::extents<2>(this->extent(1), this->extent(1))));

    ::cvMulTransposed(this->matrix<T, 0, C, 2>::mtx_,
                      s_ptr->matrix<T, 0, C, 2>::mtx_,
                      1, 0, 1.0);
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
AAT() const
{
    // Computes:  [ A * transpose(A) ]
    //
    // A[m x n] ; Out[m x m]
    //
    // NOTE:  ::cvMulTransposed is buggy for AAT (always computes ATA instead)!
    //

    BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));
    BOOST_MPL_ASSERT(( boost::is_floating_point<T> ));

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
         s_ptr(new nicta::ocv::matrix::matrix<T, 2> (nicta::ocv::matrix::extents<2>(this->extent(0), this->extent(0))));

    ::cvGEMM(this->matrix<T, 0, C, 2>::mtx_,
             this->matrix<T, 0, C, 2>::mtx_,
             1.0,
             static_cast< ::CvArr *> (0),
             0.0,
             s_ptr->matrix<T, 0, C, 2>::mtx_,
             CV_GEMM_A_T);
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
template <nicta::ocv::matrix::DecompositionModel M>
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
inverse() const throw (std::domain_error, std::logic_error, std::bad_exception)
{
    // Find the inverse  --or--  pseudo-inverse [ Inv(M) = Inv(transpose(M) * M) * transpose(M) ]
    // NOTE:  T must be float or double!
    //
    // NOTES:  pseudo-inverse otherwise known as the Moore-Penrose Inverse
    // NOTES:  use  M = nicta::ocv::matrix::SVD  if want to compute inverse of non-square matrices
    //
    // NOTES:  A[n x n]  ->  Inv(A)[n x n]  ;    A[m x n]  ->  Inv(A)[n x m]
    //

    BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));
    BOOST_MPL_ASSERT(( boost::is_floating_point<T> ));

    if (this->extent(0) != this->extent(1) && M != nicta::ocv::matrix::SVD)
    {
        throw std::logic_error("use nicta::ocv::matrix::SVD decomposition-model to invert a non-square matrix");
    }

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
         s_ptr(new nicta::ocv::matrix::matrix<T, 2, C, 2> (nicta::ocv::matrix::extents<2>(this->extent(1), this->extent(0))));

    double flag = ::cvInvert(this->matrix<T, 0, C, 2>::mtx_,
                             s_ptr->matrix<T, 0, C, 2>::mtx_,
                             static_cast<int> (M));
    if (!flag)
    {
        throw std::domain_error("matrix is not invertible");
    }
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
template <nicta::ocv::matrix::SVDSourceModel M>
boost::shared_ptr< boost::tuples::tuple< boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >          // resulting svd matrix     [W]
                                       , boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >          // left orthogonal matrix   [U]
                                       , boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >          // right orthogonal matrix  [V]
                                       >
                 >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
svd(nicta::ocv::matrix::SVDModel m)
{
    // Compute SVD:  [ A = U * W * transpose(V) ] ;  where A = *this* matrix instance
    //
    // NOTES:  iff A is a square-symmetric matrix
    //
    // * diagonal(W) = eigenvalues of A
    // * columns(U)  = eigenvectors of A
    // * columns(V)  = eigenvectors of A
    // 
    //
    // NOTES:  iff A is **NOT** a square-symmetric matrix
    //
    // * diagonal(W) = singular values of A 
    // * can't say anything about U, V
    //
    //
    // NOTES:  A[m x n] ; U[m x m] ; W[m x n] ; V[n x n]
    //
    // NOTES:  by default, V **NOT** transpose(V) is returned (can be overridden by user with appropriate SVDModel value)
    //
    // NOTES:  by default, source matrix (ie. this matrix instance) goes through computation unmodified
    //         ..however, if prepared to junk the source matrix, can speed up computation with appropriate change in SVDSourceModel template param
    //
    //
    // NOTES:  if is say only interested in W but doesn't want U, V outputs, can use boost::tuples::ignore in conjunction with boost::tuples::tie
    //
    //    matrix W, U, V;
    //    boost::tuples::tie(W, U, V) = svd< >();
    //
    //    matrix W;
    //    boost::tuples::tie(W, boost::tuples::ignore, boost::tuples::ignore) = svd< >();
    //
    //

    BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));
    BOOST_MPL_ASSERT(( boost::is_floating_point<T> ));

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
         W(new nicta::ocv::matrix::matrix<T, 2, C, 2> (nicta::ocv::matrix::extents<2>(this->extent(0), this->extent(1))));
    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
         U(new nicta::ocv::matrix::matrix<T, 2, C, 2> (nicta::ocv::matrix::extents<2>(this->extent(0), this->extent(0))));
    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
         V(new nicta::ocv::matrix::matrix<T, 2, C, 2> (nicta::ocv::matrix::extents<2>(this->extent(1), this->extent(1))));

    ::cvSVD(this->matrix<T, 0, C, 2>::mtx_,
            W->matrix<T, 0, C, 2>::mtx_,
            U->matrix<T, 0, C, 2>::mtx_,
            V->matrix<T, 0, C, 2>::mtx_,
            static_cast<int> (M) | m);

    boost::shared_ptr< boost::tuples::tuple< boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
                                           , boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
                                           , boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
                                           >
                     > s_ptr(new boost::tuples::tuple< boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
                                                     , boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
                                                     , boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
                                                     > (W, U, V));
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
template <nicta::ocv::matrix::DecompositionModel M>
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
solve_sys(const nicta::ocv::matrix::matrix<T, 2>& b) const throw(std::domain_error, std::bad_exception)
{
    // Solve a linear system, or least-squares problem:  [ Ax = b ] ;  where A = *this* matrix instance, and we solve for 'x'
    //
    // NOTES:  A must be a *square* matrix
    // NOTES:  use  M = nicta::ocv::matrix::SVD  if want to solve a least-squares problem
    //
    // NOTES:  A[m x m] ; X[m x 1] ; b[m x 1]
    //

    BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));
    BOOST_MPL_ASSERT(( boost::is_floating_point<T> ));

    if (this->extent(0) != this->extent(1))
    {
        throw std::domain_error("matrix must be square to solve linear system");
    }
    if ((b.extent(0) != this->extent(1)) && (b.extent(1) != 1))
    {
        throw std::domain_error("input not appropriately dimensioned");
    }

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
         x(new nicta::ocv::matrix::matrix<T, 2, C, 2> (nicta::ocv::matrix::extents<2>(this->extent(0), 1)));

    int flag = ::cvSolve(this->matrix<T, 0, C, 2>::mtx_,
                         b.matrix<T, 0, C, 2>::mtx_,
                         x->matrix<T, 0, C, 2>::mtx_,
                         static_cast<int> (M));
    if (!flag && M == nicta::ocv::matrix::LU)
    {
        throw std::domain_error("linear system unsolvable, source matrix(A) is singular");
    }
    return x;
}


template < typename T
         , unsigned int C
         >
template < unsigned int DD
         , unsigned int CC
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, DD, CC, DD> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
reshape(nicta::ocv::matrix::extents<DD>& e) const throw (std::length_error, std::bad_exception)
{
    // Reshape the matrix from  [Rows x Cols].C  ->  [rows x cols'].CC  -- having differnt number of rows, and/or cols, and/or channels
    // NOTE:  we return a **VIEW** into this matrix and ***NOT*** a new matrix!!
    // NOTE:  only makes sense if D == U !!
    //
    // NOTE:  we use the more general ::cvReshapeMatND to get around the idiotic openCV constraint with ::cvReshape that [CC < 4]
    // NOTE:  ::cvResahpeMatND is buggy for 2-D matrices, but we've patched the openCV code with some fixes!
    //
    // OpenCV fixes against ver1.1.0pre1:
    //
    // cxcore/cxarray.hpp, Line 3087: remove condition ..  if (!coi)   , Comment: [coi no effect therein, and is always zero anyway!]
    //
    // cxcore/cxarray.hpp, Line 3047: modify ..  if (CV_IS_MAT_CONT(mat->type))  --->  if (!CV_IS_MAT_CONT(mat->type))
    //
    // cxcore/cxarray.hpp, Line 2949: insert some code from ::cvReshape ..
    //                                                                         if (mat != header)
    //                                                                         {
    //                                                                             int hdr_refcount = header->hdr_refcount;
    //                                                                             *header = *mat;
    //                                                                             header->refcount = 0;
    //                                                                             header->hdr_refcount = hdr_refcount;
    //                                                                         }
    //

    BOOST_STATIC_ASSERT(( CC > 0 ));
    BOOST_STATIC_ASSERT(( DD > 1 ));
    BOOST_MPL_ASSERT(( boost::mpl::bool_<CC <= CV_CN_MAX> ));
    BOOST_MPL_ASSERT(( boost::mpl::bool_<DD <= CV_MAX_DIM> ));

    // do some error checking:
    if (DD <= 2)
    {
        unsigned int rows  = e[0];
        if (rows != this->matrix<T, 2, C, 2>::extent(0))
        {
            unsigned int width = this->extent(1) * C;
            unsigned int size = width * this->matrix<T, 2, C, 2>::extent(0);
            width = size / rows;
            if (width * rows != size)
            {
                throw std::length_error("unsuitable dimension extents");
            }
        }
    }
    else // DD > 2 && DD != D
    {
        unsigned int size1 = 1;
        for (unsigned int i = 0; i != 2; ++i)
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

    nicta::ocv::matrix::extents<2> ex;
    this->extents(ex);
    boost::shared_ptr<nicta::ocv::matrix::matrix<T, DD, CC, DD> > s_ptr(new nicta::ocv::matrix::matrix<T, DD, CC, DD> ());
    if (CC == C && (DD == 2 && e == ex))
    {
        // no-op
        s_ptr->matrix<T, 0, CC, DD>::mtx_ = this->matrix<T, 0, C, 2>::mtx_;
    }
    else if (CC == C && (DD != 2 || e != ex))
    {
        // reshape dims
        s_ptr->matrix<T, 0, C, DD>::mtx_ = static_cast< ::CvMat * > (::cvReshapeMatND(this->matrix<T, 0, C, 2>::mtx_,
                                                                                        sizeof(s_ptr->matrix<T, 0, CC, DD>::header_),
                                                                                        &s_ptr->matrix<T, 0, CC, DD>::header_,
                                                                                        static_cast<int> (0),
                                                                                        static_cast<int> (DD),
                                                                                        e()));
    }
    else if (CC != C && (DD == 2 && e == ex))
    {
        // reshape channels
        s_ptr->matrix<T, 0, C, DD>::mtx_ = static_cast< ::CvMat * > (::cvReshapeMatND(this->matrix<T, 0, C, 2>::mtx_,
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
        temp->matrix<T, 0, C, DD>::mtx_ = static_cast< ::CvMat * > (::cvReshapeMatND(this->matrix<T, 0, C, 2>::mtx_,
                                                                                       sizeof(temp->matrix<T, 0, CC, DD>::header_),
                                                                                       &temp->matrix<T, 0, CC, DD>::header_,
                                                                                       static_cast<int> (CC),
                                                                                       static_cast<int> (0),
                                                                                       static_cast<int *> (0)));
        temp->matrix<T, 0, CC, DD>::ptr_ = temp->matrix<T, 0, CC, DD>::mtx_->data.ptr;

        s_ptr->matrix<T, 0, C, DD>::mtx_ = static_cast< ::CvMat * > (::cvReshapeMatND(temp->matrix<T, 0, CC, DD>::mtx_,
                                                                                        sizeof(s_ptr->matrix<T, 0, CC, DD>::header_),
                                                                                        &s_ptr->matrix<T, 0, CC, DD>::header_,
                                                                                        static_cast<int> (0),
                                                                                        static_cast<int> (DD),
                                                                                        e()));
    }
    s_ptr->matrix<T, 0, CC, DD>::ptr_ = s_ptr->matrix<T, 0, CC, DD>::mtx_->data.ptr;
    return s_ptr;
}





// Element-wise Operatations:

template < typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
operator+(T value) const throw ()
{
    // Operator+ (element-wise):  [ mtx + value ]

    BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
         s_ptr(new nicta::ocv::matrix::matrix<T, 2, C, 2> (nicta::ocv::matrix::extents<2>(this->extent(0), this->extent(1))));

    ::cvAddS(this->matrix<T, 0, C, 2>::mtx_,
             ::cvScalarAll(value),
             s_ptr->matrix<T, 0, C, 2>::mtx_,
             0);
    s_ptr->matrix<T, 0, C, 2>::ptr_ = s_ptr->matrix<T, 0, C, 2>::mtx_->data.ptr;
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
operator-(T value) const throw ()
{
    // Operator- (element-wise):  [ mtx - value ]

    BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
         s_ptr(new nicta::ocv::matrix::matrix<T, 2, C, 2> (nicta::ocv::matrix::extents<2>(this->extent(0), this->extent(1))));

    ::cvSubS(this->matrix<T, 0, C, 2>::mtx_,
             ::cvScalarAll(value),
             s_ptr->matrix<T, 0, C, 2>::mtx_,
             0);
    s_ptr->matrix<T, 0, C, 2>::ptr_ = s_ptr->matrix<T, 0, C, 2>::mtx_->data.ptr;
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
operator*(T value) const throw ()
{
    // Operator* (element-wise):  [ mtx * value  --or--  value * mtx ]

    BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
         s_ptr(new nicta::ocv::matrix::matrix<T, 2, C, 2> (nicta::ocv::matrix::extents<2>(this->extent(0), this->extent(1))));

    ::CvMat* ones = ::cvCloneMat(this->matrix<T, 0, C, 2>::mtx_);
    ::cvSet(ones, ::cvScalarAll(1), 0);
    ::cvMul(this->matrix<T, 0, C, 2>::mtx_,
            ones,
            s_ptr->matrix<T, 0, C, 2>::mtx_,
            value);
    ::cvReleaseMat(&ones);
    s_ptr->matrix<T, 0, C, 2>::ptr_ = s_ptr->matrix<T, 0, C, 2>::mtx_->data.ptr;
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
operator/(T value) const throw (std::logic_error)
{
    // Operator/ (element-wise):  [ mtx / value ]
    // NOTE:  though we can do division with integers,
    //        ..OpenCV will *always* round-UP! for fractional values >= 0.5, and round-Down for fractional values < 0.5
    //        In C++ the result for integer arithmetic is IP(a/b) ..
    //         ..Hence, as the OpenCV behaviour varies from what is ordinarily expected, we restrict to floating-point types only!

    BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));
    BOOST_MPL_ASSERT(( boost::is_floating_point<T> ));

    if (!value)
    {
        throw std::logic_error("division by zero");
    }

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
         s_ptr(new nicta::ocv::matrix::matrix<T, 2, C, 2> (nicta::ocv::matrix::extents<2>(this->extent(0), this->extent(1))));

    ::CvMat* vals = ::cvCloneMat(this->matrix<T, 0, C, 2>::mtx_);
    ::cvSet(vals, ::cvScalarAll(value), 0);
    ::cvDiv(this->matrix<T, 0, C, 2>::mtx_,
            vals,
            s_ptr->matrix<T, 0, C, 2>::mtx_,
            1.0);
    ::cvReleaseMat(&vals);
    s_ptr->matrix<T, 0, C, 2>::ptr_ = s_ptr->matrix<T, 0, C, 2>::mtx_->data.ptr;
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
operator+(const nicta::ocv::matrix::matrix<T, 2>& rhs) const throw ()
{
    // Operator+ (element-wise):  [ mtx + mtx ]

    BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
         s_ptr(new nicta::ocv::matrix::matrix<T, 2, C, 2> (nicta::ocv::matrix::extents<2>(this->extent(0), this->extent(1))));

    ::cvAdd(this->matrix<T, 0, C, 2>::mtx_,
            rhs.matrix<T, 0, C, 2>::mtx_,
            s_ptr->matrix<T, 0, C, 2>::mtx_,
            0);
    s_ptr->matrix<T, 0, C, 2>::ptr_ = s_ptr->matrix<T, 0, C, 2>::mtx_->data.ptr;
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
operator-(const nicta::ocv::matrix::matrix<T, 2>& rhs) const throw ()
{
    // Operator- (element-wise):  [ mtx - mtx]

    BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
         s_ptr(new nicta::ocv::matrix::matrix<T, 2, C, 2> (nicta::ocv::matrix::extents<2>(this->extent(0), this->extent(1))));

    ::cvSub(this->matrix<T, 0, C, 2>::mtx_,
            rhs.matrix<T, 0, C, 2>::mtx_,
            s_ptr->matrix<T, 0, C, 2>::mtx_,
            0);
    s_ptr->matrix<T, 0, C, 2>::ptr_ = s_ptr->matrix<T, 0, C, 2>::mtx_->data.ptr;
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
multiply(const nicta::ocv::matrix::matrix<T, 2>& rhs) const throw ()
{
    // Operator* (element-wise):  [ mtx * mtx ]

    BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
         s_ptr(new nicta::ocv::matrix::matrix<T, 2, C, 2> (nicta::ocv::matrix::extents<2>(this->extent(0), this->extent(1))));

    ::cvMul(this->matrix<T, 0, C, 2>::mtx_,
            rhs.matrix<T, 0, C, 2>::mtx_,
            s_ptr->matrix<T, 2, C, 2>::mtx_,
            1.0);
    s_ptr->matrix<T, 0, C, 2>::ptr_ = s_ptr->matrix<T, 0, C, 2>::mtx_->data.ptr;
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2> >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
operator/(const nicta::ocv::matrix::matrix<T, 2>& rhs) const
{
    // Operator/ (element-wise):  [ mtx / mtx ]

    BOOST_MPL_ASSERT(( boost::mpl::bool_<C == 1> ));
    BOOST_MPL_ASSERT(( boost::is_floating_point<T> ));

    boost::shared_ptr<nicta::ocv::matrix::matrix<T, 2, C, 2> >
         s_ptr(new nicta::ocv::matrix::matrix<T, 2, C, 2> (nicta::ocv::matrix::extents<2>(this->extent(0), this->extent(1))));

    ::cvDiv(this->matrix<T, 0, C, 2>::mtx_,
            rhs.matrix<T, 0, C, 2>::mtx_,
            s_ptr->matrix<T, 0, C, 2>::mtx_,
            1.0);
    s_ptr->matrix<T, 0, C, 2>::ptr_ = s_ptr->matrix<T, 0, C, 2>::mtx_->data.ptr;
    return s_ptr;
}


template < typename T
         , unsigned int C
         >
nicta::ocv::matrix::matrix< T
                          , 2
                          , C
                          , 2
                          > :: 
matrix() throw ()
: matrix<T, 1, C, 2>()
{
    // Constructor (protected)
    // NOTE:  aides in creation via shared_ptr to create matrix-VIEWS
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
