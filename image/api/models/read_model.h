/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_READ_MODEL_H
#define NICTA_VIBE_IMAGE_READ_MODEL_H

#include <opencv/highgui.h>


namespace nicta {
namespace vibe {
namespace image {

    enum ReadModel
    {
        Gray_8  = CV_LOAD_IMAGE_GRAYSCALE ,                              // as 1-channel *8-bit* gray img                             [0]
        Color_8 = CV_LOAD_IMAGE_COLOR     ,                              // as *8-bit* color image                                    [1]
        File_8  = CV_LOAD_IMAGE_ANYCOLOR  ,                              // as per file *8_bit* (gray or color as case may be)        [4]

        Gray_n  = CV_LOAD_IMAGE_GRAYSCALE | CV_LOAD_IMAGE_ANYDEPTH,      // as gray, and per file color depth                         [0 | 2 = 2]
        Color_n = CV_LOAD_IMAGE_COLOR     | CV_LOAD_IMAGE_ANYDEPTH,      // as color, and per file color depth                        [1 | 2 = 3]
        File_n  = CV_LOAD_IMAGE_ANYCOLOR  | CV_LOAD_IMAGE_ANYDEPTH       // as per file color & depth                                 [4 | 2 = 6]
    };


    template <typename nicta::vibe::image::ReadModel model = nicta::vibe::image::File_n>
    struct read_model
    {
        static const int value = static_cast<int> (model);
    };

} } } // namespace nicta::vibe::image


#endif
