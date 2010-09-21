/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_COLORCODE_MODEL_H
#define NICTA_VIBE_IMAGE_COLORCODE_MODEL_H


namespace nicta {
namespace vibe {
namespace image {

    enum ColorCodeModel
    {
        GRAY        ,
        BGR         ,
        RGB         ,
        BGRA        ,
        RGBA        ,
        XYZ         ,
        YCrCb       ,
        HSV         ,
        HLS         ,
        Lab         ,
        Luv         ,
        BayerBG     ,
        BayerGB     ,
        BayerRG     ,
        BayerGR
    };


    template <typename nicta::vibe::image::ColorCodeModel model>
    struct color_code_model
    {
        static const int value = static_cast<int> (model);
    };

} } } // namespace nicta::vibe::image


#endif
