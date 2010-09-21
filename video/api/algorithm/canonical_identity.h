/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_ALGORITHM_CANONICAL_IDENTITY_H
#define NICTA_VIBE_VIDEO_ALGORITHM_CANONICAL_IDENTITY_H

#include <image/image.h>
#include <nicta_common/null_deleter.h>

#include <opencv/cxtypes.h>

#include <boost/array.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/shared_ptr.hpp>

#include <set>
#include <string>


namespace nicta {
namespace vibe {
namespace video {
namespace algorithm {

    struct canonical_identity
    {
        boost::shared_ptr< ::IplImage > operator()(::IplImage* frame) const throw ()
        {
            // A do-nothing SERVER-SIDE functor ..
            //
            // IMPORTANT:  for Server-side algorithms, User **MUST** modify argument `frame' _in-place_ **AND** return it (`frame') as shared.ptr with a `null_deleter()' !!!

            return boost::shared_ptr< ::IplImage >(frame, nicta::null_deleter());
        }


        template < typename Depth
                 , template <typename> class Pixel
                 , typename Depth_
                 , template <typename> class Pixel_
                 , template <typename, template <typename> class> class C_
                 >
        bool operator()(boost::shared_ptr<nicta::vibe::image::image<Depth, Pixel> > frame,
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
                       ) const throw ()
        {
            // A do-nothing CLIENT-SIDE functor ..

            return true;
        }


        template < unsigned int N
                 , typename Depth
                 , template <typename> class Pixel
                 , typename Depth_
                 , template <typename> class Pixel_
                 , template <typename, template <typename> class> class C_
                 >
        bool operator()(boost::array<boost::shared_ptr<nicta::vibe::image::image<Depth, Pixel> >, N>& frame,
                        boost::posix_time::ptime timestamp,
                        unsigned long int frame_index,
                        boost::array<unsigned int, N>& payload_indices,
                        unsigned int payload,
                        const std::string& source_id,
                        const std::string& host_id,
                        unsigned int host_connections,
                        std::set<std::string>& window_tags,
                        void (C_<Depth_, Pixel_>::*shutdown)() const throw (),
                        const C_<Depth_, Pixel_>* const host
                       ) const throw ()
        {
            // A do-nothing CLIENT-SIDE "Payload-N"-functor .. [getting N-frames from source]
            // NOTE:  [N > 0] && [N <= server-Payload] ==> all connected servers must have same sized Payload!

            return true;
        }
    };

} } } }  // namespace nicta::vibe::video::algorithm


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
