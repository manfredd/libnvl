/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_WRITE_MODEL_H
#define NICTA_VIBE_IMAGE_WRITE_MODEL_H


namespace nicta {
namespace vibe {
namespace image {

    enum WriteModel
    {
        asGray  = 0 ,
        asColor = 1 ,
        asImage = 2
    };


    template <typename nicta::vibe::image::WriteModel model = nicta::vibe::image::asImage>
    struct write_model
    {
        static const int value = static_cast<int> (model);
    };

} } } // namespace nicta::vibe::image


#endif
