/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef FWD_NICTA_OCV_CONVERT_AS_IMAGE_H
#define FWD_NICTA_OCV_CONVERT_AS_IMAGE_H


#include <boost/shared_ptr.hpp>
#include <stdexcept>

namespace nicta {

namespace vibe {
namespace image {

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
    template < typename Depth
             , template <typename _> class Pixel
             >
    boost::shared_ptr<nicta::vibe::image::image<Depth, Pixel> >
    as_image(const nicta::ocv::matrix::matrix< typename Depth::type
                                             , 2
                                             , Pixel<Depth>::components
                                             , 2
                                             >& matrix) throw (std::domain_error);

} // namespace nicta::ocv::convert
} } // namespace nicta::ocv


#endif
