/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License,  Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_XEVENT_DETAIL_ALGORITHM_MOUSE_COORDINATES_H
#define NICTA_VIBE_VIDEO_XEVENT_DETAIL_ALGORITHM_MOUSE_COORDINATES_H


#include "video/handlers/detail/mouse_callback.h"

#include <memory>
#include <vector>

namespace nicta {
namespace vibe {
namespace video {
namespace xevent {
namespace detail {
namespace algorithm {

    template < typename T
             , template <typename T_, typename A_ = std::allocator<T_> > class Ctr = std::vector
             >
    struct mouse_coordinates : mouse_event
    {
        void operator()(int event,
                        int x_coordinate,
                        int y_coordinate,
                        int mouse_flags,
                        void* args
                       ) const throw ()
        {
            switch (event)
            {
                case static_cast<int> (nicta::vibe::video::xevent::detail::left_button_down) :
                {
                    Ctr<T>* ctr = static_cast<Ctr<T> *> (args);

                    T coords    = { x_coordinate, y_coordinate };
                    ctr->push_back(coords);

                    break;
                }
            }
        }
    };

} } } } } } // namespace nicta::vibe::video::xevent::detail::algorithm


#endif

