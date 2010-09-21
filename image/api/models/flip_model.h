/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_FLIP_MODEL_H
#define NICTA_VIBE_IMAGE_FLIP_MODEL_H


namespace nicta {
namespace vibe {
namespace image {

    enum FlipModel
    {
        x_axis        =  0 ,
        y_axis        =  1 ,
        xy_axis       = -1 ,
        yx_axis       = -1 ,
        identity_axis = -2
    };


    template <nicta::vibe::image::FlipModel model>
    struct flip_model
    {
        static const int value = static_cast<int> (model);
    };

} } } // namespace nicta::vibe::image


#endif
