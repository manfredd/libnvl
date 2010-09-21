/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_CONVERT_AS_IMAGE_H
#define NICTA_OCV_CONVERT_AS_IMAGE_H

#include <image/image.h>
#include <ocv_matrix/ocv_matrix.h>

#include <boost/shared_ptr.hpp>

#include <stdexcept>


namespace nicta {
namespace ocv {
namespace convert {

    template < typename Depth
             , template <typename _> class Pixel
             >
    boost::shared_ptr<nicta::vibe::image::image<Depth, Pixel> >
    as_image(const nicta::ocv::matrix::matrix< typename Depth::type
                                             , 2
                                             , Pixel<Depth>::components
                                             , 2
                                             >& matrix) throw (std::domain_error);

} } } // namespace nicta::ocv::convert



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename Depth
         , template <typename _> class Pixel
         >
boost::shared_ptr<nicta::vibe::image::image<Depth, Pixel> >
nicta::ocv::convert :: 
as_image(const nicta::ocv::matrix::matrix< typename Depth::type
                                         , 2
                                         , Pixel<Depth>::components
                                         , 2
                                         >& matrix) throw (std::domain_error)
{
    // Convert matrix to image type
    // NOTE:  this is a friend function to both image and matrix classes
    //
    // NOTE:  am not going to make zero-arg image ctor public (no way!) .. so we incur a copy here (oh well..)
    //

    if (matrix.channels() > 4)
    {
        throw std::domain_error("IplImage channel count restricted to no more than 4; cannot convert from matrix to image");
    }

    ::IplImage  header;
    ::IplImage* hdrptr = ::cvGetImage(matrix.nicta::ocv::matrix::matrix< typename Depth::type
                                                                       , 2
                                                                       , Pixel<Depth>::components
                                                                       , 2
                                                                       >::template native_handle< >(), &header);
    return boost::shared_ptr<nicta::vibe::image::image<Depth, Pixel> > (new nicta::vibe::image::image<Depth, Pixel> (hdrptr));
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
