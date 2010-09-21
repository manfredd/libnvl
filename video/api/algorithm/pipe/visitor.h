/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_ALGORITHM_PIPE_VISITOR_H
#define NICTA_VIBE_VIDEO_ALGORITHM_PIPE_VISITOR_H

#include "video/algorithm/pipe/visitable.h"

#include <boost/date_time/posix_time/ptime.hpp>

#include <set>
#include <string>
#include <utility>


namespace nicta {
namespace vibe {
namespace video {
namespace algorithm {
namespace pipe {

    template < typename Depth
             , template <typename> class Pixel
             , typename R
             , typename P
             , typename C
             >
    class visitor : public boost::static_visitor<R>
    {
    public:

        visitor(boost::posix_time::ptime timestamp,
                unsigned long int frame_index,
                P& payload_index,
                unsigned int payload,
                const std::string& source_id,
                const std::string& host_id,
                unsigned int host_connections,
                std::set<std::string>& window_tags,
                void (C::*shutdown)() const throw (),
                const C* const host
               )
        : timestamp_(timestamp)
        , frame_index_(frame_index)
        , payload_index_(payload_index)
        , payload_(payload)
        , source_id_(source_id)
        , host_id_(host_id)
        , host_connections_(host_connections)
        , window_tags_(window_tags)
        , shutdown_(shutdown)
        , host_(host)
        {
            // Constructor
        }


        template < template< typename Depth_
                           , template <typename> class Pixel_
                           > class S
                 , typename U
                 >
        R operator()(nicta::vibe::video::algorithm::pipe::visitable<S>* algorithm, U& data) const
        {
            return algorithm->template operator()<Depth, Pixel, R, U, P, C>(std::forward<U>(data),
                                                                            this->timestamp_,
                                                                            this->frame_index_,
                                                                            this->payload_index_,
                                                                            this->payload_,
                                                                            this->source_id_,
                                                                            this->host_id_,
                                                                            this->host_connections_,
                                                                            this->window_tags_,
                                                                            this->shutdown_,
                                                                            this->host_);
        }

        template < template< typename Depth_
                           , template <typename> class Pixel_
                           > class S
                 , typename U
                 >
        R operator()(const nicta::vibe::video::algorithm::pipe::visitable<S>& algorithm, U& data) const
        {
            return algorithm.template operator()<Depth, Pixel, R, U, P, C>(std::forward<U>(data),
                                                                           this->timestamp_,
                                                                           this->frame_index_,
                                                                           this->payload_index_,
                                                                           this->payload_,
                                                                           this->source_id_,
                                                                           this->host_id_,
                                                                           this->host_connections_,
                                                                           this->window_tags_,
                                                                           this->shutdown_,
                                                                           this->host_);
        }

        template < template< typename Depth_
                           , template <typename> class Pixel_
                           > class S
                 , typename U
                 >
        R operator()(const nicta::vibe::video::algorithm::pipe::visitable<S>* algorithm, U&& data, U* = 0) const
        {
            return algorithm->template operator()<Depth, Pixel, R, U, P, C>(std::forward<U>(data),
                                                                            this->timestamp_,
                                                                            this->frame_index_,
                                                                            this->payload_index_,
                                                                            this->payload_,
                                                                            this->source_id_,
                                                                            this->host_id_,
                                                                            this->host_connections_,
                                                                            this->window_tags_,
                                                                            this->shutdown_,
                                                                            this->host_);
        }

        template < template< typename Depth_
                           , template <typename> class Pixel_
                           > class S
                 , typename U
                 >
        R operator()(const nicta::vibe::video::algorithm::pipe::visitable<S>& algorithm, U&& data, U* = 0) const
        {
            return algorithm.template operator()<Depth, Pixel, R, U, P, C>(std::forward<U>(data),
                                                                           this->timestamp_,
                                                                           this->frame_index_,
                                                                           this->payload_index_,
                                                                           this->payload_,
                                                                           this->source_id_,
                                                                           this->host_id_,
                                                                           this->host_connections_,
                                                                           this->window_tags_,
                                                                           this->shutdown_,
                                                                           this->host_);
        }

        template < typename A
                 , typename U
                 >
        R operator()(A* algorithm, U& data) const
        {
            return algorithm->template operator()<Depth, Pixel, R, U, P, C>(std::move(data),
                                                                            this->timestamp_,
                                                                            this->frame_index_,
                                                                            this->payload_index_,
                                                                            this->payload_,
                                                                            this->source_id_,
                                                                            this->host_id_,
                                                                            this->host_connections_,
                                                                            this->window_tags_,
                                                                            this->shutdown_,
                                                                            this->host_);
        }

        template < typename A
                 , typename U
                 >
        R operator()(A& algorithm, U& data) const
        {
            return algorithm.template operator()<Depth, Pixel, R, U, P, C>(std::move(data),
                                                                           this->timestamp_,
                                                                           this->frame_index_,
                                                                           this->payload_index_,
                                                                           this->payload_,
                                                                           this->source_id_,
                                                                           this->host_id_,
                                                                           this->host_connections_,
                                                                           this->window_tags_,
                                                                           this->shutdown_,
                                                                           this->host_);
        }


    private:

        boost::posix_time::ptime timestamp_;
        unsigned long int frame_index_;
        P& payload_index_;
        unsigned int payload_;
        const std::string& source_id_;
        const std::string& host_id_;
        unsigned int host_connections_;
        std::set<std::string>& window_tags_;
        void (C::*shutdown_)() const throw ();
        const C* const host_;
    };

} } } } }  // namespace nicta::vibe::video::algorithm::pipe


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
