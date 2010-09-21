/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License,  Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_XEVENT_DETAIL_MOUSE_CALLBACK_H
#define NICTA_VIBE_VIDEO_XEVENT_DETAIL_MOUSE_CALLBACK_H


#include "video/thread/multiton_model.h"

#include <thread/multiton/multiton.h>

#include <opencv/highgui.h>

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

namespace nicta {
namespace vibe {
namespace video {
namespace xevent {
namespace detail {

    // can use these enumerations to detect if 'event' (1st param) of the mouse-callback was any of these listened-to events
    enum MouseAction
    {
        mouse_move         = CV_EVENT_MOUSEMOVE     ,
        button_up          = CV_EVENT_MBUTTONUP     ,
        button_down        = CV_EVENT_MBUTTONDOWN   ,
        double_click       = CV_EVENT_MBUTTONDBLCLK ,
        left_button_up     = CV_EVENT_LBUTTONUP     ,
        left_button_down   = CV_EVENT_LBUTTONDOWN   ,
        left_double_click  = CV_EVENT_LBUTTONDBLCLK ,
        right_button_up    = CV_EVENT_RBUTTONUP     ,
        right_button_down  = CV_EVENT_RBUTTONDOWN   ,
        right_double_click = CV_EVENT_RBUTTONDBLCLK 
    };


    // can do bitwise AND (&) of these enumerations with 'flag' (4th param) of the mouse-callback to detect if any one of these were pressed down:  (flag & (1 << enum))
    enum MouseFlag
    {
        alt          = CV_EVENT_FLAG_ALTKEY   ,
        ctrl         = CV_EVENT_FLAG_CTRLKEY  ,
        shift        = CV_EVENT_FLAG_SHIFTKEY ,
        button       = CV_EVENT_FLAG_MBUTTON  ,
        left_button  = CV_EVENT_FLAG_LBUTTON  ,
        right_button = CV_EVENT_FLAG_RBUTTON 
    };


    struct mouse_event
    {
        void operator()(int event,
                        int x_coordinate,
                        int y_coordinate,
                        int mouse_flags,
                        void* args
                       ) const throw ()
        {
            // A do-nothing mouse-callback handler - user is expected to inherit and override
            //
            // NOTES:
            //  *      event         - any of  nicta::vibe::video::xevent::detail::{ mouse_move, button_up, button_down, double_click,
            //                                                                       left_button_up, left_button_down, left_double_click,
            //                                                                       right_button_up, right_button_down, right_double_click }
            //
            //  *      x_coordinate  - the x-coord at site of mouse-action, relative to upper-right corner of image
            //
            //  *      y_coordinate  - the y-coord at site of mouse-action, relative to upper-right corner of image
            //
            //  *      mouse_flags   - can be used with any of  M=nicta::vibe::video::xevent::detail::{ alt, ctrl, shift, button, left_button, right_button } with bitwise-and
            //                         to determine if any of those listed {M} were down along with the mouse-action:  ie.  (mouse_flags & (1 << M_i))
            //
            //  *      void* args    - used to pass data/structure to manipulate (image or otherwise)
            //
            // ASIDE:
            //  *      to register a call-back, just do:  nicta::vibe::video::xevent::detail::mouse_callback<mouse_event'>::callback(window_name, args);
            //         .. where mouse_event' is class that inherits from mouse_event and overrides this function-call op()
        }
    };


    template <typename Functor = nicta::vibe::video::xevent::detail::mouse_event>
    class mouse_callback
    {
    public:

        static
        void callback(const char* window_name, void* args = 0) throw ()
        {
            // Need this extra level of indirection because ::cvSetMouseCallback expects an unmangled C-like function
            // NOTE:  named window having 'window_name' must be available (ie. on-screen)!

            //::cvSetMouseCallback(window_name, nicta::vibe::video::xevent::detail::mouse_callback<Functor>::mouse_callback_, args);
            nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                   , boost::mutex
                                   >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                                   < void
                                                                                                   , const char *
                                                                                                   , ::CvMouseCallback
                                                                                                   , void *
                                                                                                   , boost::unique_lock
                                                                                                   >(&::cvSetMouseCallback,
                                                                                                     window_name,
                                                                                                     nicta::vibe::video::xevent::detail::mouse_callback<Functor>::mouse_callback_,
                                                                                                     args);
        }


    private:

        static
        void mouse_callback_(int event,
                             int x_coordinate,
                             int y_coordinate,
                             int mouse_flags,
                             void* args) throw ()
        {
            // Call our mouse-callback handler 'Functor'

            Functor()(event, x_coordinate, y_coordinate, mouse_flags, args);

            //nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
            //                       , boost::mutex
            //                       >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()< void
            //                                                                                                    , Functor
            //                                                                                                    , int
            //                                                                                                    , int
            //                                                                                                    , int
            //                                                                                                    , int
            //                                                                                                    , void *
            //                                                                                                    , boost::unique_lock
            //                                                                                                    >(&Functor::operator(),
            //                                                                                                      Functor(),
            //                                                                                                      event,
            //                                                                                                      x_coordinate,
            //                                                                                                      y_coordinate,
            //                                                                                                      mouse_flags,
            //                                                                                                      args);
        }
    };

} } } } } // namespace nicta::vibe::video::xevent::detail


#endif

