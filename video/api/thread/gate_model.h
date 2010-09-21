/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_THREAD_GATE_MODEL_H
#define NICTA_VIBE_VIDEO_THREAD_GATE_MODEL_H


namespace nicta {
namespace vibe {
namespace video {
namespace thread {

    struct gate_model
    {
        enum
        {
            shared = 0 
        };


        static const unsigned int size = 1;
    };

} } } } // namespace nicta::vibe::video::thread


#endif
