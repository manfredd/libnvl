/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_ORIGIN_MODEL_H
#define NICTA_VIBE_IMAGE_ORIGIN_MODEL_H

#include <opencv/cxtypes.h>


namespace nicta {
namespace vibe {
namespace image {

    enum OriginModel
    {
        TopLeft    = IPL_ORIGIN_TL ,
        BottomLeft = IPL_ORIGIN_BL
    };


    template <nicta::vibe::image::OriginModel model>
    struct origin_model
    {
        static const int value = static_cast<int> (model);
    };

} } } // namespace nicta::vibe::image


#endif
