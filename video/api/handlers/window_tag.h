/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_DISPLAY_WINDOW_TAG_H
#define NICTA_VIBE_VIDEO_DISPLAY_WINDOW_TAG_H

#include <string>


namespace nicta {
namespace vibe {
namespace video {
namespace display {

    struct window_tag
    {
        void operator()(const std::string& client_id,
                        const std::string& server_id,
                        std::string& tag
                       ) const throw ()
        {
            // Tag title-bar for open windows

            tag = "[" + client_id + "]: " + server_id;
        }
    };

} } } }  // namespace nicta::vibe::video::display


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
