/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_ALGORITHM_CLICK_STREAM_H
#define NICTA_VIBE_VIDEO_ALGORITHM_CLICK_STREAM_H

#include "video/handlers/detail/algorithm/mouse_coordinates.h"
#include "video/handlers/detail/mouse_callback.h"
#include "video/handlers/window_tag.h"
#include "video/thread/multiton_model.h"

#include <image/image.h>
#include <image/models/window_model.h>
#include <thread/multiton/multiton.h>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace nicta {
namespace vibe {
namespace video {
namespace algorithm {

    template < typename V
             , template <typename V_, typename A_> class Ctr = std::vector
             >
    struct click_stream
    {
    public:

        click_stream(Ctr<V, std::allocator<V> >& ctr) throw ();

        template < typename Depth
                 , template <typename> class Pixel
                 , typename Depth_
                 , template <typename> class Pixel_
                 , template <typename, template <typename> class> class C_
                 >
        bool operator()(const boost::shared_ptr<nicta::vibe::image::image<Depth, Pixel> > frame,
                        boost::posix_time::ptime timestamp,
                        unsigned long int frame_index,
                        unsigned int payload_index,
                        unsigned int payload,
                        const std::string& source_id,
                        const std::string& host_id,
                        unsigned int host_connections,
                        std::set<std::string>& window_tags,
                        void (C_<Depth_, Pixel_>::*shutdown)() const throw (),
                        const C_<Depth_, Pixel_>* const host
                       ) const throw ();


    private:

        Ctr<V, std::allocator<V> >& ctr_;
    };

} } } }  // namespace nicta::vibe::video::algorithm


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename V
         , template <typename V_, typename A_> class Ctr
         >
nicta::vibe::video::algorithm::click_stream<V, Ctr> :: 
click_stream(Ctr<V, std::allocator<V> >& ctr) throw ()
: ctr_(ctr)
{
    // Constructor
}


template < typename V
         , template <typename V_, typename A_> class Ctr
         >
template < typename T
         , template <typename> class U
         , typename T_
         , template <typename> class U_
         , template <typename, template <typename> class> class C_
         >
bool
nicta::vibe::video::algorithm::click_stream<V, Ctr> :: 
operator()(const boost::shared_ptr<nicta::vibe::image::image<T, U> > frame,
           boost::posix_time::ptime timestamp,
           unsigned long int frame_index,
           unsigned int payload_index,
           unsigned int payload,
           const std::string& source_id,
           const std::string& host_id,
           unsigned int host_connections,
           std::set<std::string>& window_tags,
           void (C_<T_, U_>::*shutdown)() const throw (),
           const C_<T_, U_>* const host
          ) const throw ()
{
    // Show frame at sought (x,y) coordinates, relative to bottom-left
    // NOTE:  return value is just a convenience, and is an implementation detail that should be ignored by the common user
    //        .. power-user:  can use return value to kill connection of this client with specific source
    // NOTE:  client | shutdown arguments can be used to conditionally terminate the client entirely

    /*
    std::cout << std::endl << "** [" << source_id << "] ** "
                           << frame_index << "  "
                           << timestamp << std::endl;
    */

    std::string tag;
    nicta::vibe::video::display::window_tag()(host_id, source_id, tag);
    window_tags.insert(tag);

    nicta::vibe::video::xevent::detail::mouse_callback<nicta::vibe::video::xevent::detail::algorithm::mouse_coordinates<V, Ctr> >::callback(tag.c_str(), &this->ctr_);

    //boost::bind<void> (shutdown, host)();                  // <--- can use this to conditionally shutdown the client on return of function op()(.)

    //return frame->show(tag.c_str());                       // return value here used for detecting keyboard events
    bool ret = nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                      , boost::mutex
                                      >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()< bool
                                                                                                                   , nicta::vibe::image::image<T, U>
                                                                                                                   , const char *
                                                                                                                   , nicta::vibe::image::WindowModel
                                                                                                                   , boost::unique_lock
                                                                                                                   >(&nicta::vibe::image::image<T, U>::show,
                                                                                                                     *frame,
                                                                                                                     tag.c_str().
                                                                                                                     nicta::vibe::image::autosize);
    std::cin.get();                                        // wait for key-press..

    return ret;
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
