/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_NATIVE_TYPE_H
#define NICTA_VIBE_IMAGE_NATIVE_TYPE_H

#include <opencv/cxtypes.h>

#include <boost/mpl/at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>


namespace nicta {
namespace vibe {
namespace image {

    enum Native
    {
        Header  = 0 ,        // ::IplImage&
        Handle  = 1          // ::IplImage*&
    };



    /* helper metafunction */

    template <typename nicta::vibe::image::Native N = nicta::vibe::image::Handle>
    struct native_type
    {
        typedef boost::mpl::map< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::Header>, ::IplImage >
                               , boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::Handle>, ::IplImage *>
                               > n_type;

        typedef typename boost::mpl::at<typename nicta::vibe::image::native_type<N>::n_type, boost::mpl::int_<N> >::type type;
    };

} } } // namespace nicta::vibe::image


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
