/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_FRAME_H
#define NICTA_VIBE_VIDEO_FRAME_H


#include <image/image_depth.h>
#include <image/image_pixel.h>

#include <opencv/cxtypes.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>


namespace nicta {
namespace vibe {

namespace image {

    /* forward declaration */
    template < typename Depth
             , template <typename _> class Pixel>
    class image;

} // namespace nicta::vibe::image


namespace video {

    enum FrameRecord
    {
        Image        = 0 ,
        Payload      = 1 ,
        PayloadIndex = 2 ,
        FrameIndex   = 3 ,
        TimeStamp    = 4 
    };


    struct iplframe
    {
        // tuple< img, payload, p-idx, f-idx, timestamp >
        typedef boost::tuples::tuple< ::IplImage *                                          // image
                                    , unsigned int                                          // payload
                                    , unsigned int                                          // payload-index
                                    , unsigned long int                                     // frame-index
                                    , boost::posix_time::ptime                              // timestamp
                                    > type;
    };


    template < typename Depth = nicta::vibe::image::depth_8u
             , template <typename _> class Pixel = nicta::vibe::image::bgr_p
             >
    struct frame
    {
        // tuple< img, payload, p-idx, f-idx, timestamp >
        typedef boost::tuples::tuple< nicta::vibe::image::image<Depth, Pixel> *             // image
                                    , unsigned int                                          // payload
                                    , unsigned int                                          // payload-index
                                    , unsigned long int                                     // frame-index
                                    , boost::posix_time::ptime                              // timestamp
                                    > type;
    };


    struct sp_iplframe
    {
        // tuple< img, payload, p-idx, f-idx, timestamp >
        typedef boost::shared_ptr<nicta::vibe::video::iplframe::type> type;
    };


    template < typename Depth = nicta::vibe::image::depth_8u
             , template <typename _> class Pixel = nicta::vibe::image::bgr_p
             >
    struct sp_frame
    {
        // tuple< img, payload, p-idx, f-idx, timestamp >
        typedef boost::shared_ptr<typename nicta::vibe::video::frame<Depth, Pixel>::type> type;
    };

} } }  // namespace nicta::vibe::video


#endif
