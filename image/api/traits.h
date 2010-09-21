/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_TRAITS_H
#define NICTA_VIBE_IMAGE_TRAITS_H

#include "image/image_depth.h"
#include "image/image_pixel.h"


namespace nicta {
namespace vibe {
namespace image {

    /* forward declaration */
    template < typename T
             , template <typename T_> class U
             >
    class image;


    struct img_8uC1
    {
        typedef nicta::vibe::image::image< nicta::vibe::image::depth_8u
                                         , nicta::vibe::image::gray_p
                                         >
                type;
    };

} } } // namespace nicta::vibe::image


#endif
