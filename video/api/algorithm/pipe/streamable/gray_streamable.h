/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_ALGORITHM_PIPE_STREAMABLE_GRAY_STREAMABLE_H
#define NICTA_VIBE_VIDEO_ALGORITHM_PIPE_STREAMABLE_GRAY_STREAMABLE_H

#include <image/image.h>
#include <image/image_depth.h>
#include <image/image_pixel.h>
#include <image/models/flip_model.h>
#include <nicta_common/nicta_exception.h>
#include <video/algorithm/pipe/display_tag.h>
#include <video/algorithm/pipe/visitable.h>
#include <video/algorithm/play_stream.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <set>
#include <string>


namespace nicta {
namespace vibe {
namespace video {
namespace algorithm {
namespace pipe {
namespace streamable {

    template < typename Depth = nicta::vibe::image::depth_8u
             , template <typename> class Pixel = nicta::vibe::image::bgr_p
             >
    class gray_streamable : public  nicta::vibe::video::algorithm::pipe::visitable<nicta::vibe::video::algorithm::pipe::streamable::gray_streamable>
                          , private boost::noncopyable
    {
    public:

        typedef nicta::vibe::video::algorithm::pipe::streamable::gray_streamable<Depth, Pixel> base_type;
        typedef boost::shared_ptr<nicta::vibe::image::image<Depth, nicta::vibe::image::gray_p> > result_type;


        template< typename Depth_
                , template <typename> class Pixel_
                , template <typename, template <typename> class> class C_
                >
        typename nicta::vibe::video::algorithm::pipe::streamable::gray_streamable< Depth
                                                                                 , Pixel
                                                                                 >::base_type::result_type
        operator()(boost::shared_ptr<const nicta::vibe::image::image<Depth, Pixel> > source,
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
                  ) const throw (nicta::nicta_exception);

        template< typename Depth_
                , template <typename> class Pixel_
                , template <typename, template <typename> class> class C_
                >
        bool
        operator()(boost::shared_ptr<const nicta::vibe::image::image<Depth, Pixel> > source,
                   boost::posix_time::ptime timestamp,
                   unsigned long int frame_index,
                   unsigned int payload_index,
                   unsigned int payload,
                   const std::string& source_id,
                   const std::string& host_id,
                   unsigned int host_connections,
                   std::set<std::string>& window_tags,
                   void (C_<Depth_, Pixel_>::*shutdown)() const throw (),
                   const C_<Depth_, Pixel_>* const host,
                   nicta::vibe::video::algorithm::pipe::display_tag&& tag
                  ) const throw (nicta::nicta_exception);


        // keep boost::variant quiet
        template< typename F_
                , typename C_
                >
        typename nicta::vibe::video::algorithm::pipe::streamable::gray_streamable< Depth
                                                                                 , Pixel
                                                                                 >::base_type::result_type
        operator()(F_&& source,
                   boost::posix_time::ptime timestamp,
                   unsigned long int frame_index,
                   unsigned int payload_index,
                   unsigned int payload,
                   const std::string& source_id,
                   const std::string& host_id,
                   unsigned int host_connections,
                   std::set<std::string>& window_tags,
                   void (C_::*shutdown)() const throw (),
                   const C_* const host
                  ) const throw (nicta::nicta_exception);

        // keep boost::variant quiet
        template< typename F_
                , typename C_
                >
        bool
        operator()(F_&& source,
                   boost::posix_time::ptime timestamp,
                   unsigned long int frame_index,
                   unsigned int payload_index,
                   unsigned int payload,
                   const std::string& source_id,
                   const std::string& host_id,
                   unsigned int host_connections,
                   std::set<std::string>& window_tags,
                   void (C_::*shutdown)() const throw (),
                   const C_* const host,
                   nicta::vibe::video::algorithm::pipe::display_tag&& tag
                  ) const throw (nicta::nicta_exception);
    };

} } } } } }  // namespace nicta::vibe::video::algorithm::pipe::streamable


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename T
         , template <typename> class U
         >
template< typename T_
        , template <typename> class U_
        , template <typename, template <typename> class> class C_
        >
typename nicta::vibe::video::algorithm::pipe::streamable::gray_streamable<T, U>::base_type::result_type
nicta::vibe::video::algorithm::pipe::streamable::gray_streamable<T, U> :: 
operator()(boost::shared_ptr<const nicta::vibe::image::image<T, U> > source,
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
          ) const throw (nicta::nicta_exception)
{
    // Create, Call, Return:  grayscale image

    return source->template convert<nicta::vibe::image::gray_p>();
}


template < typename T
         , template <typename> class U
         >
template< typename T_
        , template <typename> class U_
        , template <typename, template <typename> class> class C_
        >
bool
nicta::vibe::video::algorithm::pipe::streamable::gray_streamable<T, U> :: 
operator()(boost::shared_ptr<const nicta::vibe::image::image<T, U> > source,
           boost::posix_time::ptime timestamp,
           unsigned long int frame_index,
           unsigned int payload_index,
           unsigned int payload,
           const std::string& source_id,
           const std::string& host_id,
           unsigned int host_connections,
           std::set<std::string>& window_tags,
           void (C_<T_, U_>::*shutdown)() const throw (),
           const C_<T_, U_>* const host,
           nicta::vibe::video::algorithm::pipe::display_tag&& tag
          ) const throw (nicta::nicta_exception)
{
    // Create, Call, and Display:  grayscale image

    typename nicta::vibe::video::algorithm::pipe::streamable::gray_streamable<T, U>::base_type::result_type grayscale = this->operator()(source,
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
    return nicta::vibe::video::algorithm::play_stream<nicta::vibe::image::y_axis>().operator()(grayscale,
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


template < typename T
         , template <typename> class U
         >
template< typename F_
        , typename C_
        >
typename nicta::vibe::video::algorithm::pipe::streamable::gray_streamable<T, U>::base_type::result_type
nicta::vibe::video::algorithm::pipe::streamable::gray_streamable<T, U> ::
operator()(F_&& source,
           boost::posix_time::ptime timestamp,
           unsigned long int frame_index,
           unsigned int payload_index,
           unsigned int payload,
           const std::string& source_id,
           const std::string& host_id,
           unsigned int host_connections,
           std::set<std::string>& window_tags,
           void (C_::*shutdown)() const throw (),
           const C_* const host
          ) const throw (nicta::nicta_exception)
{
    // Keeps boost::variant quiet..
    // Create, Call, Return:  grayscale image

    return source->template convert<nicta::vibe::image::gray_p>();
}


template < typename T
         , template <typename> class U
         >
template< typename F_
        , typename C_
        >
bool
nicta::vibe::video::algorithm::pipe::streamable::gray_streamable<T, U> ::
operator()(F_&& source,
           boost::posix_time::ptime timestamp,
           unsigned long int frame_index,
           unsigned int payload_index,
           unsigned int payload,
           const std::string& source_id,
           const std::string& host_id,
           unsigned int host_connections,
           std::set<std::string>& window_tags,
           void (C_::*shutdown)() const throw (),
           const C_* const host,
           nicta::vibe::video::algorithm::pipe::display_tag&& tag
          ) const throw (nicta::nicta_exception)
{
    // Keeps boost::variant quiet..
    // Create, Call, and Display:  grayscale image

    typename nicta::vibe::video::algorithm::pipe::streamable::gray_streamable<T, U>::base_type::result_type grayscale = this->operator()(source,
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
    return nicta::vibe::video::algorithm::play_stream<nicta::vibe::image::y_axis>().operator()(grayscale,
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


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
