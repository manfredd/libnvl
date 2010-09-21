/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_CONVERT_AS_MATRIX_H
#define NICTA_OCV_CONVERT_AS_MATRIX_H

#include <image/image.h>
#include <image/image_depth.h>
#include <ocv_matrix/ocv_matrix.h>

#include <boost/shared_ptr.hpp>


namespace nicta {
namespace ocv {
namespace convert {

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

} } } // namespace nicta::ocv::convert



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename T
         , template <typename _> class Pixel
         >
boost::shared_ptr< nicta::ocv::matrix::matrix< T
                                             , 2
                                             , Pixel<typename nicta::vibe::image::depth<T>::type>::components
                                             , 2
                                             >
                 >
nicta::ocv::convert :: 
as_matrix(const nicta::vibe::image::image<typename nicta::vibe::image::depth<T>::type, Pixel>& image) throw ()
{
    // Convert image to matrix type
    // NOTE:  this is a friend function to both image and matrix classes
    // NOTE:  if user has ROI set on image, will pull out ROI part as matrix only
    //
    // NOTE:  if do:  image -> mtx -> image  will get different headers if ROI is set,
    //                  .. and so some IPL functions that calc image stride from its width and align may fail on resultant image
    //
    // NOTE:  am not going to make zero-arg matrix ctor public (no way!) .. so we incur a copy here (oh well..)
    //

    ::CvMat  header;
    ::CvMat* hdrptr = ::cvGetMat(image.template native_handle< >(), &header, 0, 0);
    return boost::shared_ptr< nicta::ocv::matrix::matrix< T
                                                        , 2
                                                        , Pixel<typename nicta::vibe::image::depth<T>::type>::components
                                                        , 2
                                                        >
                            > (new nicta::ocv::matrix::matrix< T, 2, Pixel<typename nicta::vibe::image::depth<T>::type>::components, 2> (hdrptr));
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
