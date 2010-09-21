/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_ALGORITHM_PLAY_STREAM_H
#define NICTA_VIBE_VIDEO_ALGORITHM_PLAY_STREAM_H

#include "video/handlers/window_tag.h"
#include "video/thread/multiton_model.h"

#include <image/image.h>
#include <image/models/flip_model.h>
#include <image/models/destination_model.h>
#include <image/models/window_model.h>
#include <nicta_common/macros/compiler.h>
#include <nicta_common/stringutil.h>
#include <thread/multiton/multiton.h>

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

#include <iostream>
#include <set>
#include <string>


namespace nicta {
namespace vibe {
namespace video {
namespace algorithm {

    template <nicta::vibe::image::FlipModel M = nicta::vibe::image::identity_axis>                // to mirror, use:  nicta::vibe::image::y_axis
    struct play_stream
    {
    public:

        play_stream(nicta::vibe::image::WindowModel m = nicta::vibe::image::autosize) throw ();


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


#if defined(x86_64) && GCC_VERSION == 40402                      /* bug in GCC-4.4.2 on 64-bit machines */
        template < unsigned long int N
#else
        template < unsigned int N
#endif
                 , typename Depth
                 , template <typename> class Pixel
                 , typename Depth_
                 , template <typename> class Pixel_
                 , template <typename, template <typename> class> class C_
                 >
        bool operator()(const boost::array<boost::shared_ptr<nicta::vibe::image::image<Depth, Pixel> >, N>& frame,
                        boost::posix_time::ptime timestamp,
                        unsigned long int frame_index,
                        boost::array<unsigned int, N>& payload_index,
                        unsigned int payload,
                        const std::string& source_id,
                        const std::string& host_id,
                        unsigned int host_connections,
                        std::set<std::string>& window_tags,
                        void (C_<Depth_, Pixel_>::*shutdown)() const throw (),
                        const C_<Depth_, Pixel_>* const host
                       ) const throw ();


    private:

        nicta::vibe::image::WindowModel window_model_;
    };

} } } }  // namespace nicta::vibe::video::algorithm


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template <nicta::vibe::image::FlipModel M>
nicta::vibe::video::algorithm::play_stream<M> ::
play_stream(nicta::vibe::image::WindowModel m) throw ()
: window_model_(m)
{
    // Constructor
}


template <nicta::vibe::image::FlipModel M>
template < typename T
         , template <typename> class U
         , typename T_
         , template <typename> class U_
         , template <typename, template <typename> class> class C_
         >
bool
nicta::vibe::video::algorithm::play_stream<M> :: 
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

    //boost::bind<void> (shutdown, host)();                  // <--- can use this to conditionally shutdown the client on return of function op()(.)


    nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                           , boost::mutex
                           >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                                 < nicta::vibe::image::image<T, U> &
                                                                                                 , nicta::vibe::image::image<T, U>
                                                                                                 , const nicta::vibe::image::destination_model<nicta::vibe::image::inplace>
                                                                                                 , boost::unique_lock
                                                                                                 >(&nicta::vibe::image::image<T, U>::template flip<M>,
                                                                                                   *frame,
                                                                                                   nicta::vibe::image::destination_model<nicta::vibe::image::inplace>());

    //return frame->show(tag.c_str());                       // return value here used for detecting keyboard events
    return nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                  , boost::mutex
                                  >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()< bool
                                                                                                               , nicta::vibe::image::image<T, U>
                                                                                                               , const char *
                                                                                                               , nicta::vibe::image::WindowModel
                                                                                                               , boost::unique_lock
                                                                                                               >(&nicta::vibe::image::image<T, U>::show,
                                                                                                                 *frame,
                                                                                                                 tag.c_str(),
                                                                                                                 this->window_model_);
}


template <nicta::vibe::image::FlipModel M>
#if defined(x86_64) && GCC_VERSION == 40402                      /* bug in GCC-4.4.2 on 64-bit machines */
template < unsigned long int N
#else
template < unsigned int N
#endif
         , typename T
         , template <typename> class U
         , typename T_
         , template <typename> class U_
         , template <typename, template <typename> class> class C_
         >
bool
nicta::vibe::video::algorithm::play_stream<M> :: 
operator()(const boost::array<boost::shared_ptr<nicta::vibe::image::image<T, U> >, N>& frame,
           boost::posix_time::ptime timestamp,
           unsigned long int frame_index,
           boost::array<unsigned int, N>& payload_index,
           unsigned int payload,
           const std::string& source_id,
           const std::string& host_id,
           unsigned int host_connections,
           std::set<std::string>& window_tags,
           void (C_<T_, U_>::*shutdown)() const throw (),
           const C_<T_, U_>* const host
          ) const throw ()
{
    // Show frames at sought (x,y) coordinates, relative to bottom-left
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

    //boost::bind<void> (shutdown, host)();                  // <--- can use this to conditionally shutdown the client on return of function op()(.)

    bool ret = true;
    unsigned long int idx = 0;

    //return frame->show(tag_n.c_str());                       // return value here used for detecting keyboard events
    for (typename boost::array<boost::shared_ptr<nicta::vibe::image::image<T, U> >, N>::const_iterator it = frame.begin(); it != frame.end(); ++it)
    {
        std::string tag_n(tag + std::string(" - Index ") + nicta::stringutil::atomic_to_string(idx++));
        window_tags.insert(tag_n);

        nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                               , boost::mutex
                               >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                                     < nicta::vibe::image::image<T, U> &
                                                                                                     , nicta::vibe::image::image<T, U>
                                                                                                     , const nicta::vibe::image::destination_model<nicta::vibe::image::inplace>
                                                                                                     , boost::unique_lock
                                                                                                     >(&nicta::vibe::image::image<T, U>::template flip<M>,
                                                                                                       **it,
                                                                                                       nicta::vibe::image::destination_model<nicta::vibe::image::inplace>());

        ret &= nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                      , boost::mutex
                                      >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()< bool
                                                                                                                   , nicta::vibe::image::image<T, U>
                                                                                                                   , const char *
                                                                                                                   , nicta::vibe::image::WindowModel
                                                                                                                   , boost::unique_lock
                                                                                                                   >(&nicta::vibe::image::image<T, U>::show,
                                                                                                                     **it,
                                                                                                                     tag_n.c_str(),
                                                                                                                     this->window_model_);
    }
    return ret;
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
