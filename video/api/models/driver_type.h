/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_DRIVER_TYPE_H
#define NICTA_VIBE_VIDEO_DRIVER_TYPE_H

#include <opencv/highgui.h>


namespace nicta {
namespace vibe {
namespace video {

    enum DriverType
    {
        Native     = CV_CAP_V4L      ,               // VFW, V4L(2)  [range:  200 - 299]:  CV_CAP_V4L <= Native < CV_CAP_IEEE1394
        Firewire   = CV_CAP_IEEE1394 ,               // IEEE1394     [range:  300 - 399]:  CV_CAP_IEEE1394 <= Firewire < CV_CAP_STEREO
        AutoDetect = CV_CAP_ANY                      // For Single Camera Use:  will automatically detect appropriate driver [ <= 99]
    };

} } } // namespace nicta::vibe::video


#endif
