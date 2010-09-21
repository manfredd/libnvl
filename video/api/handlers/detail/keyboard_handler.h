/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_XEVENT_DETAIL_KEYBOARD_HANDLER_H
#define NICTA_VIBE_VIDEO_XEVENT_DETAIL_KEYBOARD_HANDLER_H

#include "video/handlers/detail/key_map.h"
#include "video/thread/multiton_model.h"

#include <image/window.h>
#include <thread/multiton/multiton.h>
#include <nicta_common/nicta_exception.h>

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

#include <algorithm>


namespace nicta {
namespace vibe {
namespace video {
namespace xevent {
namespace detail {

    template <char lit = '+'>
    struct keyboard_handler
    {
        // NOTE:  'lit' is the delimeter between key-names in `const char* key_sequence':

        // NOTE:  all calls to `wait_key' only have context if an openCV window has focus (and exists) ..
        //        ... hence if user code is only console based, all keyboard events will be ignored as `wait_key' only listens on "window-focus"

        // USER BEWARE:  multiple keypress detection comes at a greater cost of longer wait times on window to listen for the event!

        // USER BEWARE:  windows of *other* clients *may* _appear_ listen to events of other clients if system heavily loaded & switching b/w thread contexts


        bool operator()(const char* key_sequence,
                        boost::posix_time::time_duration td = boost::posix_time::milliseconds(10)
                       ) const throw (nicta::nicta_exception)
        {
            // use nth = 0 if *don't* want to wait on any key

            try
            {
                int key = nicta::vibe::video::xevent::detail::keystroke_assocMap(key_sequence);
                if (key == 0)
                {
                    //return nicta::vibe::image::display::wait_key(td, 1) >= 0;
                    int k = nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                                   , boost::mutex
                                                   >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                                                   < int
                                                                                                                   , boost::posix_time::time_duration
                                                                                                                   , unsigned short int
                                                                                                                   , boost::unique_lock
                                                                                                                   >(&nicta::vibe::image::display::wait_key,
                                                                                                                     td,
                                                                                                                     1);
                    return k >= 0;
                }
                if (key == -1)
                {
                    //return !nicta::vibe::image::display::wait_key(td, 0);
                    int k = nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                                   , boost::mutex
                                                   >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                                                   < int
                                                                                                                   , boost::posix_time::time_duration
                                                                                                                   , unsigned short int
                                                                                                                   , boost::unique_lock
                                                                                                                   >(&nicta::vibe::image::display::wait_key,
                                                                                                                     td,
                                                                                                                     0);
                    return !k;
                }
                unsigned short int nth = 1 + std::count(key_sequence, key_sequence + ::strlen(key_sequence), lit);
                //return nicta::vibe::image::display::wait_key(td, nth) == key;
                int k = nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                               , boost::mutex
                                               >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                                               < int
                                                                                                               , boost::posix_time::time_duration
                                                                                                               , unsigned short int
                                                                                                               , boost::unique_lock
                                                                                                               >(&nicta::vibe::image::display::wait_key,
                                                                                                                 td * nth,
                                                                                                                 nth);
                //return (k & 255) == key;
                return (k == key) || (k == key - 0x100000);    // (key with numlock) || (key with no numlock)
            }
            catch (const nicta::nicta_exception& e)
            {
                throw nicta::nicta_exception(e.descriptor(), "op()(key_sequence, time_duration)", "keyboard_handler");
            }
        }
    };

} } } } }  // namespace nicta::vibe::video::xevent::detail


#endif
