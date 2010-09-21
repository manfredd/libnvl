/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_ALGORITHM_IDENTITY_TRANSLATION_H
#define NICTA_VIBE_IMAGE_ALGORITHM_IDENTITY_TRANSLATION_H


namespace nicta {
namespace vibe {
namespace image {
namespace algorithm {

    struct identity_translation
    {
        template <typename V, typename T>
        V operator()(T t) const
        {
            return static_cast<V> (t);
        }
    };

} } } }  // namespace nicta::vibe::image::algorithm

#endif
