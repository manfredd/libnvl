/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_IPLIMAGE_DELETER_H
#define NICTA_VIBE_IMAGE_IPLIMAGE_DELETER_H


#include <opencv/cxcore.h>

namespace nicta {
namespace vibe {
namespace image {
namespace detail {

    struct iplimage_deleter
    {
        void operator()(::IplImage* ptr) const throw ()
        {
            ::cvReleaseImage(&ptr);
            ptr = 0;
        }
    };

} } } }  // namespace nicta::vibe::image::detail




#endif
