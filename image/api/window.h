/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_DISPLAY_WINDOW_H
#define NICTA_VIBE_IMAGE_DISPLAY_WINDOW_H

#include <nicta_common/quantity.h>

#include <opencv/highgui.h>

#include <boost/date_time/posix_time/posix_time.hpp>


namespace nicta {
namespace vibe {
namespace image {
namespace display {

    void* window_handle(const char* id) throw ();

    void move_window(const char* id,
                     nicta::quantity::x_absolute_displacement<unsigned int> x_pos,
                     nicta::quantity::y_absolute_displacement<unsigned int> y_pos
                    ) throw ();


    void resize_window(const char* id,
                       nicta::quantity::width<unsigned int> pix_width,
                       nicta::quantity::height<unsigned int> pix_height
                      ) throw ();


    void destroy_window(const char* id) throw ();

    void destroy_all_windows() throw ();


    int wait_key(boost::posix_time::time_duration td) throw ();
    int wait_key(boost::posix_time::time_duration td, unsigned short int nth) throw ();

} } } }  // namespace nicta::vibe::image::display


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


namespace nicta {
namespace vibe {
namespace image {
namespace display {

inline
void *
window_handle(const char* id) throw ()
{
    // Return handle for window aliased by 'id'
    // NOTE:  returns '0' if window does not exist

    return ::cvGetWindowHandle(id);
}


inline
void
move_window(const char* id,
            nicta::quantity::x_absolute_displacement<unsigned int> x_pos,
            nicta::quantity::y_absolute_displacement<unsigned int> y_pos
           ) throw ()
{
    // Move window given by handle 'id' to position [x_pos, y_pos] in pixels, relative to UL corner

    ::cvMoveWindow(id, x_pos.operator()<int>(), y_pos.operator()<int>());
}


inline
void
resize_window(const char* id,
              nicta::quantity::width<unsigned int> pix_width,
              nicta::quantity::height<unsigned int> pix_height
             ) throw ()
{
    // Resize window - given by handle 'id' to pixel -height, and -width

    ::cvResizeWindow(id, pix_height.operator()<int>(), pix_width.operator()<int>());
}


inline
void
destroy_window(const char* id) throw ()
{
    // Destroy a window - given by handle 'id'

    ::cvDestroyWindow(id);
}


inline
void
destroy_all_windows() throw ()
{
    // Destroy all pop'd windows

    ::cvDestroyAllWindows();
}


inline
int
wait_key(boost::posix_time::time_duration td) throw ()
{
    // Wait for any keypress for upto time duration 'td'
    // RETURN:  key-code
    //
    // NOTE:  will show any queued images on screen too
    // NOTE:  if msec == 0; we wait indefinitely until a keypress is registered
    //

    return ::cvWaitKey(static_cast<int> (td.total_milliseconds()));
}


inline
int
wait_key(boost::posix_time::time_duration td,
         unsigned short int nth
        ) throw ()
{
    // Wait for nth keypress for *upto* time duration 'td' on **each** key, but return no key-stroke if total time is > td
    // RETURN:  key-code
    //
    // NOTE:  will show any queued images on screen too
    // NOTE:  if msec == 0; we wait indefinitely until a keypress is registered
    //

    int key = -1;
    unsigned long int count = 0;
    boost::posix_time::ptime timeout = boost::posix_time::microsec_clock::local_time() + td;
    while (boost::posix_time::microsec_clock::local_time() <= timeout && count != static_cast<unsigned long int> (nth))
    {
        key = ::cvWaitKey(static_cast<int> (td.total_milliseconds()));
        ++count;
    }
    if (!count || count != static_cast<unsigned long int> (nth))
    {
        // return value indicates no key was pressed (exactly what we want if key-count < nth or if nth == 0)

        ::cvWaitKey(static_cast<int> (td.total_milliseconds()));
        return -1;
    }
    return key;    // return keycode
}

} } } }  // namespace nicta::vibe::image::display


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
