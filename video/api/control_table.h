/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_CONTROL_TABLE_H
#define NICTA_VIBE_VIDEO_CONTROL_TABLE_H


#include <image/image_depth.h>
#include <image/image_pixel.h>

#include <boost/tuple/tuple.hpp>

#include <list>

namespace nicta {
namespace vibe {
namespace video {

    /* Forward Declarations */

    template < typename Depth
             , template <typename _> class Pixel
             >
    class video_server;

    template < typename Depth
             , template <typename _> class Pixel
             >
    class video_client;



    enum TableRecord
    {
        Server       = 0 ,
        Client       = 1 ,
        FrameRate    = 2 ,
        FrameCounter = 3
    };

    template < typename Depth = nicta::vibe::image::depth_8u
             , template <typename _> class Pixel = nicta::vibe::image::bgr_p
             >
    struct table_record
    {
        // tuple< *server, *client, nth, ctr % nth >
        typedef typename boost::tuples::tuple< nicta::vibe::video::video_server<Depth, Pixel> *
                                             , nicta::vibe::video::video_client<Depth, Pixel> *
                                             , unsigned long int
                                             , unsigned long int
                                             > type;

    };


    template < typename Depth = nicta::vibe::image::depth_8u
             , template <typename _> class Pixel = nicta::vibe::image::bgr_p
             >
    struct control_table
    {
        typedef typename std::list<typename nicta::vibe::video::table_record<Depth, Pixel>::type> type;
    };


} } }  // namespace nicta::vibe::video


#endif
