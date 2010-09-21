/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef FWD_NICTA_OCV_CONVERT_AS_MATRIX_H
#define FWD_NICTA_OCV_CONVERT_AS_MATRIX_H


#include <boost/shared_ptr.hpp>

namespace nicta {

namespace vibe {
namespace image {

    /* forward declaration */
    template <typename T>
    struct depth;


    /* forward declaration */
    template < typename Depth
             , template <typename _> class Pixel>
    class image;

} } // namespace nicta::vibe::image



namespace ocv {
namespace matrix {

    /* forward declaration */
    template < typename T
             , unsigned int D
             , unsigned int C
             , unsigned int U
             >
    class matrix;

} // namespace nicta::ocv::matrix



namespace convert {

    /* forward declaration */
    template < typename T
             , template <typename _> class Pixel
             >
    boost::shared_ptr< nicta::ocv::matrix::matrix< T
                                                 , 2
                                                 , Pixel<typename nicta::vibe::image::depth<T>::type>::components
                                                 , 2
                                                 >
                     >
    as_matrix(const nicta::vibe::image::image<typename nicta::vibe::image::depth<T>::type, Pixel>& image) throw ();

} // namespace nicta::ocv::convert
} } // namespace nicta::ocv


#endif
