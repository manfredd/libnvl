/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_ALGORITHM_PIPE_VISITABLE_H
#define NICTA_VIBE_VIDEO_ALGORITHM_PIPE_VISITABLE_H

#include "video/algorithm/pipe/display_tag.h"

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/or.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#include <set>
#include <string>
#include <utility>


namespace nicta {
namespace vibe {
namespace video {
namespace algorithm {
namespace pipe {

    // Streamables to inherit from visitable..
    template < template< typename Depth_
                       , template <typename> class Pixel_
                       > class Derived
             >
    struct visitable
    {
        template < typename Depth
                 , template <typename> class Pixel
                 , typename R
                 , typename U
                 , typename P
                 , typename C
                 >
        typename boost::disable_if< boost::mpl::or_< boost::is_same< R
                                                                   , typename nicta::vibe::video::algorithm::pipe::display_tag::type
                                                                   >
                                                   , boost::mpl::not_< boost::is_same< R
                                                                                     , typename Derived<Depth, Pixel>::base_type::result_type
                                                                                     >
                                                                     >
                                                   >
                                  , R
                                  >::type
        operator()(U& data,
                   boost::posix_time::ptime timestamp,
                   unsigned long int frame_index,
                   P& payload_index,
                   unsigned int payload,
                   const std::string& source_id,
                   const std::string& host_id,
                   unsigned int host_connections,
                   std::set<std::string>& window_tags,
                   void (C::*shutdown)() const throw (),
                   const C* const host
                  ) const
        {
            // Dispatch to underlying type..

            return static_cast<const Derived<Depth, Pixel> *> (this)->operator()(std::move(data),
                                                                                 timestamp,
                                                                                 frame_index,
                                                                                 payload_index,
                                                                                 payload,
                                                                                 source_id,
                                                                                 host_id,
                                                                                 host_connections,
                                                                                 window_tags,
                                                                                 shutdown,
                                                                                 host);
        }
        template < typename Depth
                 , template <typename> class Pixel
                 , typename R
                 , typename U
                 , typename P
                 , typename C
                 >
        typename boost::enable_if< boost::is_same< R
                                                 , typename nicta::vibe::video::algorithm::pipe::display_tag::type
                                                 >
                                 , R
                                 >::type
        operator()(U& data,
                   boost::posix_time::ptime timestamp,
                   unsigned long int frame_index,
                   P& payload_index,
                   unsigned int payload,
                   const std::string& source_id,
                   const std::string& host_id,
                   unsigned int host_connections,
                   std::set<std::string>& window_tags,
                   void (C::*shutdown)() const throw (),
                   const C* const host
                  ) const
        {
            // Dispatch to underlying type.. (and display)

            return static_cast<const Derived<Depth, Pixel> *> (this)->operator()(std::move(data),
                                                                                 timestamp,
                                                                                 frame_index,
                                                                                 payload_index,
                                                                                 payload,
                                                                                 source_id,
                                                                                 host_id,
                                                                                 host_connections,
                                                                                 window_tags,
                                                                                 shutdown,
                                                                                 host,
                                                                                 nicta::vibe::video::algorithm::pipe::display_tag());
        }


        template < typename Depth
                 , template <typename> class Pixel
                 , typename R
                 , typename U
                 , typename P
                 , typename C
                 >
        typename boost::disable_if< boost::mpl::or_< boost::is_same< R
                                                                   , typename nicta::vibe::video::algorithm::pipe::display_tag::type
                                                                   >
                                                   , boost::mpl::not_< boost::is_same< R
                                                                                     , typename Derived<Depth, Pixel>::base_type::result_type
                                                                                     >
                                                                     >
                                                   >
                                  , R
                                  >::type
        operator()(const U& data,
                   boost::posix_time::ptime timestamp,
                   unsigned long int frame_index,
                   P& payload_index,
                   unsigned int payload,
                   const std::string& source_id,
                   const std::string& host_id,
                   unsigned int host_connections,
                   std::set<std::string>& window_tags,
                   void (C::*shutdown)() const throw (),
                   const C* const host
                  ) const
        {
            // Dispatch to underlying type..

            return static_cast<const Derived<Depth, Pixel> *> (this)->operator()(std::move(data),
                                                                                 timestamp,
                                                                                 frame_index,
                                                                                 payload_index,
                                                                                 payload,
                                                                                 source_id,
                                                                                 host_id,
                                                                                 host_connections,
                                                                                 window_tags,
                                                                                 shutdown,
                                                                                 host);
        }
        template < typename Depth
                 , template <typename> class Pixel
                 , typename R
                 , typename U
                 , typename P
                 , typename C
                 >
        typename boost::enable_if< boost::is_same< R
                                                 , typename nicta::vibe::video::algorithm::pipe::display_tag::type
                                                 >
                                 , R
                                 >::type
        operator()(const U& data,
                   boost::posix_time::ptime timestamp,
                   unsigned long int frame_index,
                   P& payload_index,
                   unsigned int payload,
                   const std::string& source_id,
                   const std::string& host_id,
                   unsigned int host_connections,
                   std::set<std::string>& window_tags,
                   void (C::*shutdown)() const throw (),
                   const C* const host
                  ) const
        {
            // Dispatch to underlying type.. (and display)

            return static_cast<const Derived<Depth, Pixel> *> (this)->operator()(std::move(data),
                                                                                 timestamp,
                                                                                 frame_index,
                                                                                 payload_index,
                                                                                 payload,
                                                                                 source_id,
                                                                                 host_id,
                                                                                 host_connections,
                                                                                 window_tags,
                                                                                 shutdown,
                                                                                 host,
                                                                                 nicta::vibe::video::algorithm::pipe::display_tag());
        }


        template < typename Depth
                 , template <typename> class Pixel
                 , typename R
                 , typename U
                 , typename P
                 , typename C
                 >
        typename boost::disable_if< boost::mpl::or_< boost::is_same< R
                                                                   , typename nicta::vibe::video::algorithm::pipe::display_tag::type
                                                                   >
                                                   , boost::mpl::not_< boost::is_same< R
                                                                                     , typename Derived<Depth, Pixel>::base_type::result_type
                                                                                     >
                                                                     >
                                                   >
                                  , R
                                  >::type
        operator()(U&& data,
                   boost::posix_time::ptime timestamp,
                   unsigned long int frame_index,
                   P& payload_index,
                   unsigned int payload,
                   const std::string& source_id,
                   const std::string& host_id,
                   unsigned int host_connections,
                   std::set<std::string>& window_tags,
                   void (C::*shutdown)() const throw (),
                   const C* const host,
                   U* = 0
                  ) const
        {
            // Dispatch to underlying type..

            return static_cast<const Derived<Depth, Pixel> *> (this)->operator()(std::forward<U>(data),
                                                                                 timestamp,
                                                                                 frame_index,
                                                                                 payload_index,
                                                                                 payload,
                                                                                 source_id,
                                                                                 host_id,
                                                                                 host_connections,
                                                                                 window_tags,
                                                                                 shutdown,
                                                                                 host);
        }
        template < typename Depth
                 , template <typename> class Pixel
                 , typename R
                 , typename U
                 , typename P
                 , typename C
                 >
        typename boost::enable_if< boost::is_same< R
                                                 , typename nicta::vibe::video::algorithm::pipe::display_tag::type
                                                 >
                                 , R
                                 >::type
        operator()(U&& data,
                   boost::posix_time::ptime timestamp,
                   unsigned long int frame_index,
                   P& payload_index,
                   unsigned int payload,
                   const std::string& source_id,
                   const std::string& host_id,
                   unsigned int host_connections,
                   std::set<std::string>& window_tags,
                   void (C::*shutdown)() const throw (),
                   const C* const host,
                   U* = 0
                  ) const
        {
            // Dispatch to underlying type.. (and display)

            return static_cast<const Derived<Depth, Pixel> *> (this)->operator()(std::forward<U>(data),
                                                                                 timestamp,
                                                                                 frame_index,
                                                                                 payload_index,
                                                                                 payload,
                                                                                 source_id,
                                                                                 host_id,
                                                                                 host_connections,
                                                                                 window_tags,
                                                                                 shutdown,
                                                                                 host,
                                                                                 nicta::vibe::video::algorithm::pipe::display_tag());
        }


        // keep boost::variant quiet
        template < typename Depth
                 , template <typename> class Pixel
                 , typename R
                 , typename U
                 , typename P
                 , typename C
                 >
        R
        operator()(bool /*data*/,
                   boost::posix_time::ptime /*timestamp*/,
                   unsigned long int frame_index,
                   P& payload_index,
                   unsigned int payload,
                   const std::string& source_id,
                   const std::string& host_id,
                   unsigned int host_connections,
                   std::set<std::string>& window_tags,
                   void (C::*shutdown)() const throw (),
                   const C* const host
                  ) const
        {
            // Keeps boost::variant quiet..

            return R();
        }
    };

} } } } }  // namespace nicta::vibe::video::algorithm::pipe


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
