/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_ALGORITHM_UNSIGNED_TRANSLATION_H
#define NICTA_VIBE_IMAGE_ALGORITHM_UNSIGNED_TRANSLATION_H

#include <boost/numeric/conversion/bounds.hpp>


namespace nicta {
namespace vibe {
namespace image {
namespace algorithm {

    struct unsigned_translation
    {
        template <typename V, typename T>
        V operator()(T t) const
        {
            long double scale_factor = static_cast<long double> (boost::numeric::bounds<V>::highest()) /
                                       static_cast<long double> (boost::numeric::bounds<T>::highest());

            return static_cast<V> (t * scale_factor);
        }
    };

} } } }  // namespace nicta::vibe::image::algorithm

#endif
