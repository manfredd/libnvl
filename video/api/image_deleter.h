/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_IMAGE_DELETER_H
#define NICTA_VIBE_VIDEO_IMAGE_DELETER_H


#include "video/frame.h"

#include <image/image_depth.h>
#include <image/image_pixel.h>

namespace nicta {
namespace vibe {
namespace video {

    template < typename Depth = nicta::vibe::image::depth_8u                                     // the color-depth of the *incoming* stream
             , template <typename _> class Pixel = nicta::vibe::image::bgr_p                     // the color-space of the *incoming* stream
             >
    struct image_deleter
    {
        void operator()(typename nicta::vibe::video::frame<Depth, Pixel>::type* ptr) const throw ()
        {
            delete ptr->template get<nicta::vibe::video::Image>();
            ptr->template get<nicta::vibe::video::Image>() = 0;

            delete ptr;
            ptr = 0;
        }
    };

} } }  // namespace nicta::vibe::video


#endif
