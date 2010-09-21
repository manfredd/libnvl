/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_DETAIL_INITIALIZER_H
#define NICTA_OCV_MATRIX_DETAIL_INITIALIZER_H

#include "ocv_matrix/extents.h"
#include "ocv_matrix/models/vector_model.h"
#include "ocv_matrix/ocv_matrix.h"

#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/int.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>

#include <initializer_list>
#include <stdexcept>
#include <vector>


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

} } } } } // namespace nicta::ocv::matrix::detail::initializer



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
typename boost::enable_if< typename boost::mpl::greater<boost::mpl::int_<D>, boost::mpl::int_<1> >::type
                         , boost::shared_ptr<nicta::ocv::matrix::matrix<T, D, C, U> >
                         >::type
nicta::ocv::matrix::detail::initializer :: 
initializer(const nicta::ocv::matrix::extents<D>& e,
            std::initializer_list<T>&& il
           ) throw (std::length_error)
{
    // Read std::initializer_list, returning an ocv_matrix

    if (static_cast<unsigned long int> (il.size()) != static_cast<unsigned long int> (e.product()))
    {
        throw std::length_error("initializer_list and matrix extents dimensionally inconsistent");
    }

    std::vector<T> data(il);
    nicta::ocv::matrix::matrix<T, D, C, U> m(e, data.data());                                                            // this is a view (shallow-copy) of 'data'
    return boost::shared_ptr<nicta::ocv::matrix::matrix<T, D, C, U> >(new nicta::ocv::matrix::matrix<T, D, C, U>(m));    // here, we've forced a deep-copy of 'data'
}


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
typename boost::enable_if< typename boost::mpl::equal_to<boost::mpl::int_<D>, boost::mpl::int_<1> >::type
                         , boost::shared_ptr<nicta::ocv::matrix::matrix<T, D, C, U> >
                         >::type
nicta::ocv::matrix::detail::initializer :: 
initializer(const nicta::ocv::matrix::extents<D>& e,
            std::initializer_list<T>&& il,
            nicta::ocv::matrix::VectorModel model
           ) throw (std::length_error)
{
    // Read std::initializer_list, returning an ocv_matrix

    if (static_cast<unsigned long int> (il.size()) != static_cast<unsigned long int> (e.product()))
    {
        throw std::length_error("initializer_list and matrix extents dimensionally inconsistent");
    }

    std::vector<T> data(il);
    nicta::ocv::matrix::matrix<T, D, C, U> m(e, data.data(), model);                                                     // this is a view (shallow-copy) of 'data'
    return boost::shared_ptr<nicta::ocv::matrix::matrix<T, D, C, U> >(new nicta::ocv::matrix::matrix<T, D, C, U>(m));    // here, we've forced a deep-copy of 'data'
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
