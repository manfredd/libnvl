/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_INTERPLN_MODEL_H
#define NICTA_VIBE_IMAGE_INTERPLN_MODEL_H

#include <opencv/cv.h>


namespace nicta {
namespace vibe {
namespace image {

    enum InterpolationModel
    {
        Nearest  = CV_INTER_NN     ,      // nearest-neighbour interpolation
        Bilinear = CV_INTER_LINEAR ,      // bilinear interpolation
        Bicubic  = CV_INTER_CUBIC  ,      // bicubic interpolation
        Area     = CV_INTER_AREA          // resampling using pixel area relation
                                          //    - is preferred for image *decimation* for moire-free results
                                          //    - in case of zooming, it is similar to Nearest(-Neighbour) method
    };


    template <nicta::vibe::image::InterpolationModel model>
    struct interpolation_model
    {
        static const int value = static_cast<int> (model);
    };

} } } // namespace nicta::vibe::image


#endif
