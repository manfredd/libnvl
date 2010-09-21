/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_STREAM_MODEL_H
#define NICTA_VIBE_VIDEO_STREAM_MODEL_H


namespace nicta {
namespace vibe {
namespace video {

    enum StreamModel
    {
        Directory = 0 ,                    // dir of image-files
        File      = 1 ,                    // avi-video stream
        Camera    = 2                      // live-camera stream
    };


    template <StreamModel model>
    struct stream_model
    {
        enum
        {
            value = static_cast<int> (model)
        };
    };

} } } // namespace nicta::vibe::video


#endif
