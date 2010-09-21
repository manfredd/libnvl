/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_IMPLICIT_CONVERSION_H
#define NICTA_VIBE_IMAGE_IMPLICIT_CONVERSION_H


namespace nicta {
namespace vibe {
namespace image {

    enum ImplicitConversionModel
    {
        Forbid = 0 ,
        Allow  = 1
    };

    template <nicta::vibe::image::ImplicitConversionModel model>
    struct implicit_conversion_model
    {
        static const int value = static_cast<int> (model);
    };

} } } // namespace nicta::vibe::image


#endif
