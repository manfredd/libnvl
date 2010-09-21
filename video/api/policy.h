/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_POLICY_H
#define NICTA_VIBE_VIDEO_POLICY_H


#include "video/video_server.h"

#include <image/image_depth.h>
#include <image/image_pixel.h>
#include <nicta_common/quantity.h>

#include <string>

namespace nicta {
namespace vibe {
namespace video {

    template < typename Depth = nicta::vibe::image::depth_8u
             , template <typename _> class Pixel = nicta::vibe::image::bgr_p
             >
    class policy
    {
    public:

        policy(nicta::vibe::video::video_server<Depth, Pixel>* server,
               nicta::quantity::step<unsigned long int> nth = 
                    nicta::quantity::step<unsigned long int>(1)
              ) throw ();

        unsigned long int nth() const throw ();
        const std::string& id() const throw ();
        nicta::vibe::video::video_server<Depth, Pixel>* server() const throw ();


    private:

        unsigned long int nth_;
        nicta::vibe::video::video_server<Depth, Pixel>* server_;
    };

} } } // namespace nicta::vibe::video


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::video::policy<T, U> :: 
policy(nicta::vibe::video::video_server<T, U>* server,
       nicta::quantity::step<unsigned long int> nth
      ) throw ()
: nth_(nth.operator()< >() ? nth.operator()< >() : 1)
, server_(server)
{
    // Canonical Constructor
}


template < typename T
         , template <typename T_> class U
         >
unsigned long int
nicta::vibe::video::policy<T, U> :: 
nth() const throw ()
{
    // Return policy frame

    return this->nth_;
}


template < typename T
         , template <typename T_> class U
         >
const std::string &
nicta::vibe::video::policy<T, U> :: 
id() const throw ()
{
    // Return identifier associated with this policy's server

    return this->server_->id();
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::video::video_server<T, U> *
nicta::vibe::video::policy<T, U> :: 
server() const throw ()
{
    // Return server associated with this policy

    return this->server_;
}


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


#endif
