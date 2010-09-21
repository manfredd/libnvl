/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_IPLIMAGE_DELETER_H
#define NICTA_VIBE_VIDEO_IPLIMAGE_DELETER_H


#include "video/frame.h"

#include <opencv/cxcore.h>

namespace nicta {
namespace vibe {
namespace video {

    struct iplimage_deleter
    {
        void operator()(::IplImage* ptr) const throw ()
        {
            ::cvReleaseImage(&ptr);
            ptr = 0;
        }


        void operator()(nicta::vibe::video::iplframe::type* ptr) const throw ()
        {
            ::cvReleaseImage(&ptr->get<nicta::vibe::video::Image>());
            ptr->get<nicta::vibe::video::Image>() = 0;

            delete ptr;
            ptr = 0;
        }
    };

} } }  // namespace nicta::vibe::video


#endif
