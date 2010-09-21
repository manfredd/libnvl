/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_DEVICE_INDEX_H
#define NICTA_VIBE_VIDEO_DEVICE_INDEX_H

#include "video/models/driver_type.h"
#include "video/models/detail/device_bound.h"


namespace nicta {
namespace vibe {
namespace video {

    template <typename nicta::vibe::video::DriverType t>
    struct device_index
    {
        enum
        {
            glb = static_cast<int> (t)                           ,
            lub = glb + nicta::vibe::video::detail::device_bound
        };
    };

    template < >
    struct device_index<nicta::vibe::video::AutoDetect>
    {
        enum
        {
            glb = 0   ,
            lub = glb
        };
    };

} } } // namespace nicta::vibe::video


#endif
