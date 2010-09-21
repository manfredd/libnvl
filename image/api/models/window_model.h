/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_WINDOW_MODEL_H
#define NICTA_VIBE_IMAGE_WINDOW_MODEL_H

#include <opencv/highgui.h>


namespace nicta {
namespace vibe {
namespace image {

    enum WindowModel
    {
        resize   = !CV_WINDOW_AUTOSIZE ,
        autosize = CV_WINDOW_AUTOSIZE  
    };


    template <nicta::vibe::image::WindowModel model>
    struct window_model
    {
        enum
        {
            value = static_cast<int> (model)
        };
    };

} } } // namespace nicta::vibe::image


#endif
