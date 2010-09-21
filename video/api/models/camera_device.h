/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_CAMERA_DEVICE_H
#define NICTA_VIBE_VIDEO_CAMERA_DEVICE_H

#include "video/models/device_index.h"
#include "video/models/driver_type.h"
#include "video/models/detail/device_bound.h"

#include <boost/static_assert.hpp>


namespace nicta {
namespace vibe {
namespace video {

    template < typename nicta::vibe::video::DriverType driver = nicta::vibe::video::AutoDetect
             , unsigned short int CameraIndex = 0
             >
    struct camera_device
    {
        enum
        {
            driver_type = static_cast<int> (driver)                    ,
            index       = driver_type + static_cast<int> (CameraIndex)                    // index = nth camera for said driver_type
        };

        BOOST_STATIC_ASSERT (CameraIndex <= nicta::vibe::video::detail::device_bound);
        BOOST_STATIC_ASSERT (static_cast<int> (nicta::vibe::video::camera_device<driver, CameraIndex>::index) >= 
                                               nicta::vibe::video::device_index<driver>::glb);
        BOOST_STATIC_ASSERT (static_cast<int> (nicta::vibe::video::camera_device<driver, CameraIndex>::index) <= 
                                               nicta::vibe::video::device_index<driver>::lub);
    };

} } } // namespace nicta::vibe::video


#endif
