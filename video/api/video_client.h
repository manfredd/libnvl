/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_CLIENT_H
#define NICTA_VIBE_VIDEO_CLIENT_H


#include "video/algorithm/canonical_identity.h"
#include "video/algorithm/deinterlace_identity.h"
#include "video/controller.h"
#include "video/frame.h"
#include "video/image_deleter.h"
#include "video/policy.h"
#include "video/video_server.h"
#include "video/handlers/window_tag.h"
#include "video/handlers/detail/keyboard_handler.h"
#include "video/macros/video_client.h"
#include "video/models/stream_model.h"
#include "video/thread/multiton_model.h"

#include <image/image.h>
#include <image/image_depth.h>
#include <image/image_pixel.h>
#include <image/window.h>
#include <nicta_common/detail/max_arity.h>
#include <nicta_common/nicta_exception.h>
#include <nicta_common/quantity.h>
#include <nicta_common/stringutil.h>
#include <nicta_common/util.h>
#include <thread/guard/guard.h>
#include <thread/multiton/multiton.h>
#include <thread/rw_locks/rw_locks.h>

#include <opencv/cxcore.h>
#include <opencv/cxtypes.h>

#include <boost/array.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/static_assert.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <list>
#include <map>
#include <set>
#include <string>


namespace nicta {
namespace vibe {
namespace video {

    /* Forward Declaration */
    template < typename Depth
             , template <typename _> class Pixel
             >
    class controller;


    template < typename Depth = nicta::vibe::image::depth_8u
             , template <typename _> class Pixel = nicta::vibe::image::bgr_p
             >
    class video_client
    {
    public:

        typedef void result_type;
        typedef nicta::vibe::video::policy<Depth, Pixel> policy_t;
        typedef nicta::vibe::video::video_server<Depth, Pixel> server_t;


        video_client(server_t* server,
                     const char* id,
                     nicta::quantity::step<unsigned long int> nth =
                          nicta::quantity::step<unsigned long int> (1),
                     nicta::quantity::signed_frame_buffer_capacity <long int> bufsize =
                          nicta::quantity::signed_frame_buffer_capacity<long int> (0),
                     const char* event_key = "SILENT_KEYS",
                     nicta::quantity::fraction<double> sleep_period =
                          nicta::quantity::fraction<double> (0)
                    ) throw ();



#define OPERATOR(z, n, _)                                                                                                                         \
                                                                                                                                                  \
        video_client(nicta::vibe::video::controller<Depth, Pixel>* mediator,                                                                      \
                     BOOST_PP_ENUM_PARAMS(n, server_t* server),                                                                                   \
                     const char* id,                                                                                                              \
                     nicta::quantity::step<unsigned long int> nth =                                                                               \
                          nicta::quantity::step<unsigned long int> (1),                                                                           \
                     nicta::quantity::signed_frame_buffer_capacity <long int> bufsize =                                                           \
                          nicta::quantity::signed_frame_buffer_capacity<long int> (0),                                                            \
                     const char* event_key = "SILENT_KEYS",                                                                                       \
                     nicta::quantity::fraction<double> sleep_period =                                                                             \
                          nicta::quantity::fraction<double> (0.99)                                                                                \
                    ) throw (nicta::nicta_exception)                                                                                              \
        : id_(id)                                                                                                                                 \
        , server_(0)                                                                                                                              \
        , controller_(mediator)                                                                                                                   \
        , eof_(false)                                                                                                                             \
        , frame_rate_(nth.operator()< >())                                                                                                        \
        , bufsize_(bufsize.operator()< >())                                                                                                       \
        , event_key_(event_key)                                                                                                                   \
        , sleep_fraction_(std::fabs(sleep_period.operator()< >()))                                                                                \
        , time_(boost::posix_time::not_a_date_time)                                                                                               \
        {                                                                                                                                         \
            /* Constructor */                                                                                                                     \
            /* NOTE:  will use *same* buffer-size for data from all servers, so interface remains clean */                                        \
                                                                                                                                                  \
            if (this->id_.empty())                                                                                                                \
            {                                                                                                                                     \
                this->id_ = nicta::stringutil::atomic_to_string(this);                                                                            \
            }                                                                                                                                     \
            this->controller_->register_client_online(this, BOOST_PP_ENUM_PARAMS(n, server), nth);                                                \
            BOOST_PP_REPEAT(n, INIT_BUFFER, server)                                                                                               \
            BOOST_PP_REPEAT(n, INIT_INIT_MAP, server)                                                                                             \
            BOOST_PP_REPEAT(n, INIT_EXPIRY_MAP, server)                                                                                           \
            BOOST_PP_REPEAT(n, INIT_STARTTIME_MAP, server)                                                                                        \
        }                                                                                                                                         \
                                                                                                                                                  \
                                                                                                                                                  \
        video_client(nicta::vibe::video::controller<Depth, Pixel>* mediator,                                                                      \
                     BOOST_PP_ENUM_PARAMS(n, policy_t& policy),                                                                                   \
                     const char* id,                                                                                                              \
                     const char* event_key = "SILENT_KEYS",                                                                                       \
                     nicta::quantity::signed_frame_buffer_capacity <long int> bufsize =                                                           \
                          nicta::quantity::signed_frame_buffer_capacity<long int> (0),                                                            \
                     nicta::quantity::fraction<double> sleep_period =                                                                             \
                          nicta::quantity::fraction<double> (0.99)                                                                                \
                    ) throw (nicta::nicta_exception)                                                                                              \
        : id_(id)                                                                                                                                 \
        , server_(0)                                                                                                                              \
        , controller_(mediator)                                                                                                                   \
        , eof_(false)                                                                                                                             \
        , frame_rate_(0)                                                                                                                          \
        , bufsize_(bufsize.operator()< >())                                                                                                       \
        , event_key_(event_key)                                                                                                                   \
        , sleep_fraction_(std::fabs(sleep_period.operator()< >()))                                                                                \
        , time_(boost::posix_time::not_a_date_time)                                                                                               \
        {                                                                                                                                         \
            /* Constructor */                                                                                                                     \
            /* NOTE:  will use *same* buffer-size for data from all servers, so interface remains clean */                                        \
                                                                                                                                                  \
            BOOST_MPL_ASSERT_MSG(0, CTOR_DISABLED__HETROGENEOUS_FRAME_RATES_NOT_YET_SUPPORTED, (nicta::vibe::video::video_client<Depth, Pixel>)); \
                                                                                                                                                  \
                                                                                                                                                  \
            if (this->id_.empty())                                                                                                                \
            {                                                                                                                                     \
                this->id_ = nicta::stringutil::atomic_to_string(this);                                                                            \
            }                                                                                                                                     \
            this->controller_->register_client_online(this, BOOST_PP_ENUM_PARAMS(n, policy));                                                     \
            BOOST_PP_REPEAT(n, INIT_BUFFER_, policy)                                                                                              \
            BOOST_PP_REPEAT(n, INIT_INIT_MAP_, policy)                                                                                            \
            BOOST_PP_REPEAT(n, INIT_EXPIRY_MAP_, policy)                                                                                          \
            BOOST_PP_REPEAT(n, INIT_STARTTIME_MAP_, policy)                                                                                       \
        }
/**/

#define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
#define BOOST_PP_LOCAL_LIMITS (1, NICTA_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
#undef OPERATOR

/* unload "macros/video_client.h" */
#undef INIT_STARTTIME_MAP_
#undef INIT_STARTTIME_MAP
#undef INIT_EXPIRY_MAP_
#undef INIT_EXPIRY_MAP
#undef INIT_BUFFER_
#undef INIT_BUFFER
#undef NICTA_VIBE_MACROS_VIDEO_CLIENT_H


        // ===

        template <typename Functor>
        result_type operator()(Functor& ftor) const;

        template < typename Functor
                 , typename ServerFunctor
                 >
        result_type operator()(Functor& ftor, ServerFunctor& sftor) const;

        template < typename Functor
                 , unsigned int Payload
                 , template <unsigned int Payload_, typename Depth_, template <typename _Depth> class _Pixel> class ServerDeinterlace
                 >
        result_type operator()(Functor& ftor,
                               ServerDeinterlace<Payload, Depth, Pixel>& dftor) const;

        template < typename Functor
                 , typename ServerFunctor
                 , unsigned int Payload
                 , template <unsigned int Payload_, typename Depth_, template <typename _Depth> class _Pixel> class ServerDeinterlace
                 >
        result_type operator()(Functor& ftor,
                               ServerFunctor& sftor,
                               ServerDeinterlace<Payload, Depth, Pixel>& dftor) const;

        // ---

        template <typename Functor>
        result_type operator()(boost::posix_time::ptime start, Functor& ftor) const;

        template < typename Functor
                 , typename ServerFunctor
                 >
        result_type operator()(boost::posix_time::ptime start,
                               Functor& ftor,
                               ServerFunctor& sftor) const;

        template < typename Functor
                 , unsigned int Payload
                 , template <unsigned int Payload_, typename Depth_, template <typename _Depth> class _Pixel> class ServerDeinterlace
                 >
        result_type operator()(boost::posix_time::ptime start,
                               Functor& ftor,
                               ServerDeinterlace<Payload, Depth, Pixel>& dftor) const;

        template < typename Functor
                 , typename ServerFunctor
                 , unsigned int Payload
                 , template <unsigned int Payload_, typename Depth_, template <typename _Depth> class _Pixel> class ServerDeinterlace
                 >
        result_type operator()(boost::posix_time::ptime start,
                               Functor& ftor,
                               ServerFunctor& sftor,
                               ServerDeinterlace<Payload, Depth, Pixel>& dftor) const;

        // ---

        template <typename Functor>
        result_type operator()(boost::posix_time::ptime start,
                               boost::posix_time::time_duration incr,
                               Functor& ftor) const;

        template < typename Functor
                 , typename ServerFunctor
                 >
        result_type operator()(boost::posix_time::ptime start,
                               boost::posix_time::time_duration incr,
                               Functor& ftor,
                               ServerFunctor& sftor) const;

        template < typename Functor
                 , unsigned int Payload
                 , template <unsigned int Payload_, typename Depth_, template <typename _Depth> class _Pixel> class ServerDeinterlace
                 >
        result_type operator()(boost::posix_time::ptime start,
                               boost::posix_time::time_duration incr,
                               Functor& ftor,
                               ServerDeinterlace<Payload, Depth, Pixel>& dftor) const;

        template < typename Functor
                 , typename ServerFunctor
                 , unsigned int Payload
                 , template <unsigned int Payload_, typename Depth_, template <typename _Depth> class _Pixel> class ServerDeinterlace
                 >
        result_type operator()(boost::posix_time::ptime start,
                               boost::posix_time::time_duration incr,
                               Functor& ftor,
                               ServerFunctor& sftor,
                               ServerDeinterlace<Payload, Depth, Pixel>& dftor) const;

        // ===

        template <typename Functor>
        result_type operator()(const nicta::vibe::video::video_server<Depth, Pixel>& server,
                               boost::posix_time::ptime start,
                               boost::posix_time::time_duration incr,
                               Functor& ftor
                              ) const;


        // ===

        template < unsigned int N
                 , typename Functor
                 >
        result_type operator()(Functor& ftor) const;

        template < unsigned int N
                 , typename Functor
                 , typename ServerFunctor
                 >
        result_type operator()(Functor& ftor,
                               ServerFunctor& sftor) const;

        template < unsigned int N
                 , typename Functor
                 , unsigned int Payload
                 , template <unsigned int Payload_, typename Depth_, template <typename _Depth> class _Pixel> class ServerDeinterlace
                 >
        result_type operator()(Functor& ftor,
                               ServerDeinterlace<Payload, Depth, Pixel>& dftor) const;

        template < unsigned int N
                 , typename Functor
                 , typename ServerFunctor
                 , unsigned int Payload
                 , template <unsigned int Payload_, typename Depth_, template <typename _Depth> class _Pixel> class ServerDeinterlace
                 >
        result_type operator()(Functor& ftor,
                               ServerFunctor& sftor,
                               ServerDeinterlace<Payload, Depth, Pixel>& dftor) const;

        // ---

        template < unsigned int N
                 , typename Functor
                 >
        result_type operator()(boost::posix_time::ptime start, Functor& ftor) const;

        template < unsigned int N
                 , typename Functor
                 , typename ServerFunctor
                 >
        result_type operator()(boost::posix_time::ptime start,
                               Functor& ftor,
                               ServerFunctor& sftor) const;

        template < unsigned int N
                 , typename Functor
                 , unsigned int Payload
                 , template <unsigned int Payload_, typename Depth_, template <typename _Depth> class _Pixel> class ServerDeinterlace
                 >
        result_type operator()(boost::posix_time::ptime start,
                               Functor& ftor,
                               ServerDeinterlace<Payload, Depth, Pixel>& dftor) const;

        template < unsigned int N
                 , typename Functor
                 , typename ServerFunctor
                 , unsigned int Payload
                 , template <unsigned int Payload_, typename Depth_, template <typename _Depth> class _Pixel> class ServerDeinterlace
                 >
        result_type operator()(boost::posix_time::ptime start,
                               Functor& ftor,
                               ServerFunctor& sftor,
                               ServerDeinterlace<Payload, Depth, Pixel>& dftor) const;

        // ---

        template < unsigned int N
                 , typename Functor
                 >
        result_type operator()(boost::posix_time::ptime start,
                               boost::posix_time::time_duration incr,
                               Functor& ftor) const;

        template < unsigned int N
                 , typename Functor
                 , typename ServerFunctor
                 >
        result_type operator()(boost::posix_time::ptime start,
                               boost::posix_time::time_duration incr,
                               Functor& ftor,
                               ServerFunctor& sftor) const;

        template < unsigned int N
                 , typename Functor
                 , unsigned int Payload
                 , template <unsigned int Payload_, typename Depth_, template <typename _Depth> class _Pixel> class ServerDeinterlace
                 >
        result_type operator()(boost::posix_time::ptime start,
                               boost::posix_time::time_duration incr,
                               Functor& ftor,
                               ServerDeinterlace<Payload, Depth, Pixel>& dftor) const;

        template < unsigned int N
                 , typename Functor
                 , typename ServerFunctor
                 , unsigned int Payload
                 , template <unsigned int Payload_, typename Depth_, template <typename _Depth> class _Pixel> class ServerDeinterlace
                 >
        result_type operator()(boost::posix_time::ptime start,
                               boost::posix_time::time_duration incr,
                               Functor& ftor,
                               ServerFunctor& sftor,
                               ServerDeinterlace<Payload, Depth, Pixel>& dftor) const;

        // ===

        template < unsigned int N
                 , typename Functor
                 >
        result_type operator()(const nicta::vibe::video::video_server<Depth, Pixel>& server,
                               boost::posix_time::ptime start,
                               boost::posix_time::time_duration incr,
                               Functor& ftor
                              ) const;

        // ---


        void shutdown() const throw ();
        void rx(const typename nicta::vibe::video::sp_iplframe::type* packet,
                const std::string& id,
                unsigned int payload
               ) const throw ();

        bool online() const throw ();
        bool offline() const throw ();
        const std::string& id() const throw ();

        unsigned long int frame_rate() const throw ();

        unsigned long int stream_length() const throw ();
        unsigned long int stream_length(const std::string& server_id) const throw (nicta::nicta_exception);

        nicta::vibe::video::StreamModel stream_type(const std::string& server_id) const throw (nicta::nicta_exception);


    private:

        static const char* CTOR_DISABLED__HETROGENEOUS_FRAME_RATES_NOT_YET_SUPPORTED;


        template < typename Depth_ = nicta::vibe::image::depth_8u
                 , template <typename _> class Pixel_ = nicta::vibe::image::bgr_p
                 >
        struct predicate_
        {
            bool operator()(const typename nicta::vibe::video::sp_frame<Depth_, Pixel_>::type& p1, const boost::posix_time::ptime& p2) const
            {
                return p1->template get<nicta::vibe::video::TimeStamp>() < p2;
            }
        };

        std::set<std::string>::size_type initialized_() const throw ();
        bool initialized_(const std::string& server_id) const throw ();
        void set_initialized_(const std::string& server_id) const throw ();

        bool buffered_() const throw ();

        void janitor_(std::set<std::string>& terminated) const throw ();
        void janitor_(const std::map<std::string, std::set<std::string> >& terminated) const throw ();


        std::string id_;
        nicta::vibe::video::video_server<Depth, Pixel>* server_;
        nicta::vibe::video::controller<Depth, Pixel>* controller_;

        mutable bool eof_;
        mutable boost::mutex eof_mutex_;

        typedef typename nicta::vibe::video::sp_frame<Depth, Pixel>::type frame_t;
        typedef std::list<typename nicta::vibe::video::video_client<Depth, Pixel>::frame_t> frame_sequence_t;
        mutable std::map<std::string, typename nicta::vibe::video::video_client<Depth, Pixel>::frame_sequence_t> buffer_;

        mutable std::map<bool, std::set<std::string> > init_;

        mutable std::map<std::string, boost::posix_time::ptime> rx_timestamp_;
        mutable std::map<std::string, bool> expired_;

        mutable std::map<std::string, bool> start_time_matched_;

        unsigned long int frame_rate_;
        long int bufsize_;

        const char* event_key_;

        double sleep_fraction_;

        mutable boost::posix_time::ptime time_;
    };

} } } // namespace nicta::vibe::video


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


/* initialize static member */
template < typename T
         , template <typename T_> class U
         >
const char *
nicta::vibe::video::video_client<T, U> ::
CTOR_DISABLED__HETROGENEOUS_FRAME_RATES_NOT_YET_SUPPORTED = 0;



template < typename T
         , template <typename T_> class U
         >
nicta::vibe::video::video_client<T, U> ::
video_client(nicta::vibe::video::video_server<T, U>* server,
             const char* id,
             nicta::quantity::step<unsigned long int> nth,
             nicta::quantity::signed_frame_buffer_capacity <long int> bufsize,
             const char* event_key,
             nicta::quantity::fraction<double> sleep_period
            ) throw ()
: id_(id)
, server_(server)
, controller_(0)
, eof_(false)
, frame_rate_(nth.operator()< >())
, bufsize_(bufsize.operator()< >())
, event_key_(event_key)
, sleep_fraction_(std::fabs(sleep_period.operator()< >()))
, time_(boost::posix_time::not_a_date_time)
{
    // Constructor
    // NOTE:  thread-unaware variant..

    if (this->id_.empty())
    {
        this->id_ = nicta::stringutil::atomic_to_string(this);
    }
    std::string sid(this->server_->id());
    this->buffer_[sid];
    this->init_[false].insert(sid);
    this->expired_[sid]            = false;
    this->start_time_matched_[sid] = false;
}


template < typename T
         , template <typename T_> class U
         >
template <typename Functor>
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(Functor& ftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Step through each this->buffer_[id] sequentially
    // Get frames from video_server and apply ftor
    // NOTE:  need this as boost::bind does not acknowledge default arguments
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..

    nicta::vibe::video::algorithm::canonical_identity sftor;
    nicta::vibe::video::algorithm::deinterlace_identity<1U, T, U> dftor;
    this->template operator()< Functor
                             , nicta::vibe::video::algorithm::canonical_identity
                             , 1U
                             , nicta::vibe::video::algorithm::deinterlace_identity
                             >(boost::posix_time::ptime(), boost::posix_time::time_duration(0, 0, 0, 0), ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < typename Functor
         , typename ServerFunctor
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(Functor& ftor, ServerFunctor& sftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Step through each this->buffer_[id] sequentially
    // Get frames from video_server and apply ftor
    // NOTE:  need this as boost::bind does not acknowledge default arguments
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  sftor only meaningful for non-threaded contexts!

    nicta::vibe::video::algorithm::deinterlace_identity<1U, T, U> dftor;
    this->template operator()< Functor
                             , ServerFunctor
                             , 1U
                             , nicta::vibe::video::algorithm::deinterlace_identity
                             >(boost::posix_time::ptime(), boost::posix_time::time_duration(0, 0, 0, 0), ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < typename Functor
         , unsigned int Payload
         , template <unsigned int Payload_, typename T_, template <typename _T> class U_> class ServerDeinterlace
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(Functor& ftor,
           ServerDeinterlace<Payload, T, U>& dftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Step through each this->buffer_[id] sequentially
    // Get frames from video_server and apply ftor
    // NOTE:  need this as boost::bind does not acknowledge default arguments
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  dftor only meaningful for non-threaded contexts!

    BOOST_STATIC_ASSERT(( Payload > 0 ));

    nicta::vibe::video::algorithm::canonical_identity sftor;
    this->template operator()< Functor
                             , nicta::vibe::video::algorithm::canonical_identity
                             , Payload
                             , ServerDeinterlace
                             >(boost::posix_time::ptime(), boost::posix_time::time_duration(0, 0, 0, 0), ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < typename Functor
         , typename ServerFunctor
         , unsigned int Payload
         , template <unsigned int Payload_, typename T_, template <typename _T> class U_> class ServerDeinterlace
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(Functor& ftor,
           ServerFunctor& sftor,
           ServerDeinterlace<Payload, T, U>& dftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Step through each this->buffer_[id] sequentially
    // Get frames from video_server and apply ftor
    // NOTE:  need this as boost::bind does not acknowledge default arguments
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  sftor, dftor only meaningful for non-threaded contexts!

    BOOST_STATIC_ASSERT(( Payload > 0 ));

    this->template operator()< Functor
                             , ServerFunctor
                             , Payload
                             , ServerDeinterlace
                             >(boost::posix_time::ptime(), boost::posix_time::time_duration(0, 0, 0, 0), ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template <typename Functor>
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(boost::posix_time::ptime start, Functor& ftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Search for start_time `start' at each this->buffer_[id], and step through each this->buffer_[id] sequentially thereafter
    // Get frames from video_server and apply ftor
    // NOTE:  need this as boost::bind does not acknowledge default arguments
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..

    nicta::vibe::video::algorithm::canonical_identity sftor;
    nicta::vibe::video::algorithm::deinterlace_identity<1U, T, U> dftor;
    this->template operator()< Functor
                             , nicta::vibe::video::algorithm::canonical_identity
                             , 1U
                             , nicta::vibe::video::algorithm::deinterlace_identity
                             >(start, boost::posix_time::time_duration(0, 0, 0, 0), ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < typename Functor
         , typename ServerFunctor
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(boost::posix_time::ptime start,
           Functor& ftor,
           ServerFunctor& sftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Search for start_time `start' at each this->buffer_[id], and step through each this->buffer_[id] sequentially thereafter
    // Get frames from video_server and apply ftor
    // NOTE:  need this as boost::bind does not acknowledge default arguments
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  sftor only meaningful for non-threaded contexts!

    nicta::vibe::video::algorithm::deinterlace_identity<1U, T, U> dftor;
    this->template operator()< Functor
                             , ServerFunctor
                             , 1U
                             , nicta::vibe::video::algorithm::deinterlace_identity
                             >(start, boost::posix_time::time_duration(0, 0, 0, 0), ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < typename Functor
         , unsigned int Payload
         , template <unsigned int Payload_, typename T_, template <typename _T> class U_> class ServerDeinterlace
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(boost::posix_time::ptime start,
           Functor& ftor,
           ServerDeinterlace<Payload, T, U>& dftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Search for start_time `start' at each this->buffer_[id], and step through each this->buffer_[id] sequentially thereafter
    // Get frames from video_server and apply ftor
    // NOTE:  need this as boost::bind does not acknowledge default arguments
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  dftor only meaningful for non-threaded contexts!

    BOOST_STATIC_ASSERT(( Payload > 0 ));

    nicta::vibe::video::algorithm::canonical_identity sftor;
    this->template operator()< Functor
                             , nicta::vibe::video::algorithm::canonical_identity
                             , Payload
                             , ServerDeinterlace
                             >(start, boost::posix_time::time_duration(0, 0, 0, 0), ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < typename Functor
         , typename ServerFunctor
         , unsigned int Payload
         , template <unsigned int Payload_, typename T_, template <typename _T> class U_> class ServerDeinterlace
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(boost::posix_time::ptime start,
           Functor& ftor,
           ServerFunctor& sftor,
           ServerDeinterlace<Payload, T, U>& dftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Search for start_time `start' at each this->buffer_[id], and step through each this->buffer_[id] sequentially thereafter
    // Get frames from video_server and apply ftor
    // NOTE:  need this as boost::bind does not acknowledge default arguments
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  sftor, dftor only meaningful for non-threaded contexts!

    BOOST_STATIC_ASSERT(( Payload > 0 ));

    this->template operator()< Functor
                             , ServerFunctor
                             , Payload
                             , ServerDeinterlace
                             >(start, boost::posix_time::time_duration(0, 0, 0, 0), ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template <typename Functor>
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(boost::posix_time::ptime start,
           boost::posix_time::time_duration incr,
           Functor& ftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Search for start_time `start' at each this->buffer_[id], and step through with  start + incr * N  thereafter
    // Get frames from video_server and apply ftor
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..

    nicta::vibe::video::algorithm::canonical_identity sftor;
    nicta::vibe::video::algorithm::deinterlace_identity<1U, T, U> dftor;
    this->template operator()< Functor
                             , nicta::vibe::video::algorithm::canonical_identity
                             , 1U
                             , nicta::vibe::video::algorithm::deinterlace_identity
                             >(start, incr, ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < typename Functor
         , typename ServerFunctor
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(boost::posix_time::ptime start,
           boost::posix_time::time_duration incr,
           Functor& ftor,
           ServerFunctor& sftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Search for start_time `start' at each this->buffer_[id], and step through with  start + incr * N  thereafter
    // Get frames from video_server and apply ftor
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  sftor only meaningful for non-threaded contexts!

    nicta::vibe::video::algorithm::deinterlace_identity<1U, T, U> dftor;
    this->template operator()< Functor
                             , ServerFunctor
                             , 1U
                             , nicta::vibe::video::algorithm::deinterlace_identity
                             >(start, incr, ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < typename Functor
         , unsigned int Payload
         , template <unsigned int Payload_, typename T_, template <typename _T> class U_> class ServerDeinterlace
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(boost::posix_time::ptime start,
           boost::posix_time::time_duration incr,
           Functor& ftor,
           ServerDeinterlace<Payload, T, U>& dftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Search for start_time `start' at each this->buffer_[id], and step through with  start + incr * N  thereafter
    // Get frames from video_server and apply ftor
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  dftor only meaningful for non-threaded contexts!

    BOOST_STATIC_ASSERT(( Payload > 0 ));

    nicta::vibe::video::algorithm::canonical_identity sftor;
    this->template operator()< Functor
                             , nicta::vibe::video::algorithm::canonical_identity
                             , Payload
                             , ServerDeinterlace
                             >(start, incr, ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < typename Functor
         , typename ServerFunctor
         , unsigned int Payload
         , template <unsigned int Payload_, typename T_, template <typename _T> class U_> class ServerDeinterlace
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(boost::posix_time::ptime start,
           boost::posix_time::time_duration incr,
           Functor& ftor,
           ServerFunctor& sftor,
           ServerDeinterlace<Payload, T, U>& dftor) const
{
    // Function-call operator - convenience method for initiating thread (or non-thread)
    // Search for start_time `start' at each this->buffer_[id], and step through with  start + incr * N  thereafter
    // Get frames from video_server and apply ftor
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  sftor, dftor only meaningful for non-threaded contexts!

    BOOST_STATIC_ASSERT(( Payload > 0 ));

    std::map<std::string, std::set<std::string> > secondary_terminated;
    while (!this->initialized_() || (this->buffered_() && this->online()))
    {
        if (!this->controller_)
        {
            // for thread-unaware contexts..

            if (!this->server_->online()) break;
            this->server_->template operator()<ServerFunctor, Payload, ServerDeinterlace>(sftor, dftor);
            if (this->server_->held_frame_index() % this->frame_rate() != 0) continue;
            this->rx(this->server_->tx(), this->server_->id(), this->server_->payload());
        }

        bool increment = false;
        std::set<std::string> terminated;
        for (typename std::map<std::string, typename nicta::vibe::video::video_client<T, U>::frame_sequence_t>::iterator it = this->buffer_.begin();
             it != this->buffer_.end();
             ++it)
        {
            if ((!this->initialized_(it->first) && !it->second.size()) || terminated.find(it->first) != terminated.end())
            {
                continue;
            }

            if (!start.is_not_a_date_time())
            {
                // find a time >= start
                typename std::list<typename nicta::vibe::video::video_client<T, U>::frame_t>::iterator pos =
                                                            std::lower_bound(it->second.begin(),
                                                                             it->second.end(),
                                                                             start,
                                                                             typename nicta::vibe::video::video_client<T, U>::template predicate_<T, U>());

                if ((!this->initialized_(it->first) || this->start_time_matched_[it->first]) && pos == it->second.end())
                {
                    boost::posix_time::time_duration period = this->controller_ ? this->controller_->period()
                                                                                : nicta::util::fps_period(this->server_->fps());
                    if (boost::posix_time::microsec_clock::local_time() - it->second.back()->template get<nicta::vibe::video::TimeStamp>() >
                        incr + period * 3)
                    {
                        // no more data ..and last held through has previously been processed - this will empty the container for the associated server

                        it->second.erase(it->second.begin(), pos);
                        terminated.insert(it->first);
                    }
                    else if (it->second.size() > 1)
                    {
                        // we don't want what's come in so far, ..throw it away

                        it->second.erase(it->second.begin(), --pos);
                    }
                    continue;
                }
                it->second.erase(it->second.begin(), pos);
                this->start_time_matched_[it->first] = true;
                if (!increment)
                {
                    start     += incr;
                    increment  = true;
                }
            }


            bool flag = false;
            boost::posix_time::ptime time = (*it->second.begin())->template get<nicta::vibe::video::TimeStamp>();
            if (this->time_.is_not_a_date_time())
            {
                flag = true;
                this->time_ = time;

                // we use this->time_ to ensure we don't pass the the same image as the one prior ...
            }

            bool handle = true;
            if (flag || time != this->time_)
            {
                flag = false;
                this->time_ = time;

                // if (this->initialized_(it->first) == true && handle == false):  user has closed window [X]
                unsigned int host_connection_count = this->controller_ ? this->controller_->client_links(this->id_) : 1U;
                boost::shared_ptr<nicta::vibe::image::image<T, U> >
                     frame_copy(new nicta::vibe::image::image<T, U>(*(*it->second.begin())->template get<nicta::vibe::video::Image>()));
                handle = ftor(frame_copy,                                                     // for user algorithm safety, a copy *is* be passed through!!
                              (*it->second.begin())->template get<nicta::vibe::video::TimeStamp>(),
                              (*it->second.begin())->template get<nicta::vibe::video::FrameIndex>(),
                              (*it->second.begin())->template get<nicta::vibe::video::PayloadIndex>(),
                              (*it->second.begin())->template get<nicta::vibe::video::Payload>(),
                              it->first,
                              this->id_,
                              host_connection_count,
                              secondary_terminated[it->first],
                              &nicta::vibe::video::template video_client<T, U>::shutdown,
                              const_cast<const nicta::vibe::video::video_client<T, U> * const> (this));
            }

            /*
            std::cout << std::endl << "** [" << this->id_ << " : " << it->first.c_str() << "] ** "
                                   << (*it->second.begin())->template get<nicta::vibe::video::FrameIndex>() << "  "
                                   << (*it->second.begin())->template get<nicta::vibe::video::PayloadIndex>() << "  "
                                   << (*it->second.begin())->template get<nicta::vibe::video::Payload>() << "  "
                                   << (*it->second.begin())->template get<nicta::vibe::video::TimeStamp>() << "  "
                                   << it->second.size();
            */


            if (it->second.size() > 1)
            {
                // we can remove the if'arg and just keep code below, but if we are at different period than server, we don't want to throw last valid frame

                it->second.erase(it->second.begin(), ++it->second.begin());
            }


            if (this->controller_)
            {
                // for thread-aware contexts..

                // logic so as not to abort if we are on the penultimate frame - so that we can show last frame before exiting..
                if (this->initialized_(it->first) && it->second.size() == (*it->second.begin())->template get<nicta::vibe::video::Payload>())
                {
                    boost::posix_time::time_duration td = (this->controller_->period() * (2 + 1)) * this->controller_->frame_rate(this, it->first);
                    if (boost::posix_time::microsec_clock::local_time() - this->rx_timestamp_.find(it->first)->second > td)
                    {
                        if (this->expired_[it->first])
                        {
                            terminated.insert(it->first);
                            this->expired_.erase(it->first);
                        }
                        this->expired_[it->first] = true;
                    }
                }
            }

            // user signalled death
            if (this->initialized_(it->first) && !handle)
            {
                // user has closed window [X]
                terminated.insert(it->first);
            }
            else if (this->offline() || nicta::vibe::video::xevent::detail::keyboard_handler< >()(this->event_key_))
            {
                // user sent signal via ftor *or* keyboard signal to kill this client
                // NOTE:  this condition (hence *all* keyboard keystrokes) will be ignored if user this is a *console-only* application!

                this->janitor_(terminated);
                break;
            }

            this->set_initialized_(it->first);
        }

        // clean-up
        bool erasure = false;
        for (typename std::set<std::string>::iterator jt = terminated.begin(); jt != terminated.end(); ++jt)
        {
            erasure = true;
            this->buffer_.erase(*jt);
            if (this->controller_)
            {
                // for thread-aware contexts..

                this->controller_->unlink(this, *jt);
            }

            std::string tag;
            nicta::vibe::video::display::window_tag()(this->id_, *jt, tag);
            if (nicta::vibe::image::display::window_handle(tag.c_str()))
            {
                nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                       , boost::mutex
                                       >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                               < void
                                                                                               , const char *
                                                                                               , boost::unique_lock
                                                                                               >(&nicta::vibe::image::display::destroy_window, tag.c_str());

            }
            for (typename std::set<std::string>::iterator kt = secondary_terminated[*jt].begin(); kt != secondary_terminated[*jt].end(); ++kt)
            {
                if (nicta::vibe::image::display::window_handle((*kt).c_str()))
                {
                    nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                           , boost::mutex
                                           >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                             < void
                                                                                             , const char *
                                                                                             , boost::unique_lock
                                                                                             >(&nicta::vibe::image::display::destroy_window, (*kt).c_str());
                }
            }
        }
        if (erasure && this->buffer_.empty())
        {
            this->shutdown();
            break;
        }

        // hold for next temporal window
        if (this->controller_)
        {
            // for thread-aware contexts..

            nicta::util::sleep(this->controller_->period() * this->sleep_fraction_);
        }
        else
        {
            // for thread-unaware contexts..

            nicta::util::sleep(nicta::util::fps_period(this->server_->fps()) * this->sleep_fraction_);
        }
    }
    this->janitor_(secondary_terminated);

    //std::cout << std::endl << "DEAD THREAD " << std::endl;
}


template < typename T
         , template <typename T_> class U
         >
template <typename Functor>
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(const nicta::vibe::video::video_server<T, U>& server,
           boost::posix_time::ptime start,
           boost::posix_time::time_duration incr,
           Functor& ftor
          ) const
{
    // Function-call operator - convenience method for initiating thread
    // UNFORTUNATELY, boost::bind can not distinguish overloads in this case, so user has to be explicit [b/c of the const char*] :-(
    // 1-arg op(server_id):
    //       Use timestamps on this->buffer_[server_id], and use for all buffers                                   == [ op()(server, NADT, td(0), ftor) ]
    // 2-arg op(server_id, start):     
    //       Use timestamps on this->buffer_[server_id], starting at time `start', and use for all buffers         == [ op(server, start, td(0), ftor)  ]
    // 3-arg op()(server_id, start, incr):
    //       Use timestamps on this->buffer_[server_id], starting at time `start', and use for all buffers
    //       .. and stepping through this->buffer_[server_id] with  start + incr * N  thereafter                   == [ op(server, start, td, ftor)     ]
    // Get frames from video_server and apply ftor
    // NOTE:  this function is *ONLY* meant for thread -aware contexts.. !!

    bool nadt = start.is_not_a_date_time();
    const char* server_id = server.id().c_str();
    std::map<std::string, std::set<std::string> > secondary_terminated;
    while (!this->initialized_() || (this->buffered_() && this->online()))
    {
        if (!this->controller_->online(server_id) && this->buffer_[server_id].empty())
        {
            break;
        }

        if (!this->buffer_[server_id].size())
        {
            for (typename std::map<std::string, typename nicta::vibe::video::video_client<T, U>::frame_sequence_t>::iterator it = this->buffer_.begin();
                 it != this->buffer_.end();
                 ++it)
            {
                it->second.clear();
            }

            // hold for next temporal window
            nicta::util::sleep(this->controller_->period());
            continue;
        }

        if (nadt)
        {
            // start came in uninitialized..

            typename nicta::vibe::video::video_client<T, U>::frame_sequence_t::const_iterator pos = this->buffer_[server_id].begin();
            if (pos == this->buffer_[server_id].end())
            {
                break;
            }
            start = (*pos)->template get<nicta::vibe::video::TimeStamp>();
        }
        else
        {
            // start came in initialized..

            typename nicta::vibe::video::video_client<T, U>::frame_sequence_t& li = this->buffer_[server_id];
            if (li.begin() == li.end())
            {
                break;
            }

            // find a time >= start
            typename std::list<typename nicta::vibe::video::video_client<T, U>::frame_t>::iterator pos =
                                                           std::lower_bound(li.begin(),
                                                                            li.end(),
                                                                            start,
                                                                            typename nicta::vibe::video::video_client<T, U>::template predicate_<T, U>());

            if ((!this->initialized_(server_id) || this->start_time_matched_[server_id]) && pos == li.end())
            {
                if (boost::posix_time::microsec_clock::local_time() - li.back()->template get<nicta::vibe::video::TimeStamp>() >
                    incr + this->controller_->period() * 3)
                {
                    // no more data ..and last held through has previously been processed - this will empty the container for the associated server
                    //li.erase(li.begin(), pos);

                    // goto shutdown ...
                    typename std::map<std::string, typename nicta::vibe::video::video_client<T, U>::frame_sequence_t>::iterator it;
                    for (it = this->buffer_.begin(); it != this->buffer_.end(); ++it)
                    {
                        this->controller_->unlink(this, it->first);

                        std::string tag;
                        nicta::vibe::video::display::window_tag()(this->id_, it->first, tag);
                        if (nicta::vibe::image::display::window_handle(tag.c_str()))
                        {
                            nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                                   , boost::mutex
                                                   >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                               < void
                                                                                               , const char *
                                                                                               , boost::unique_lock
                                                                                               >(&nicta::vibe::image::display::destroy_window, tag.c_str());
                        }
                        for (typename std::set<std::string>::iterator jt = secondary_terminated[it->first].begin();
                             jt != secondary_terminated[it->first].end();
                             ++jt)
                        {
                            if (nicta::vibe::image::display::window_handle((*jt).c_str()))
                            {
                                nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                                       , boost::mutex
                                                       >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                            < void
                                                                                            , const char *
                                                                                            , boost::unique_lock
                                                                                            >(&nicta::vibe::image::display::destroy_window, (*jt).c_str());
                            }
                        }
                    }
                    this->buffer_.clear();
                    this->shutdown();
                }
                else if (li.size() > 1)
                {
                    // we don't want what's come in so far, ..throw it away
                    li.erase(li.begin(), --pos);

                    // ..and blow away *all* of everyone else's data
                    typename std::map<std::string, typename nicta::vibe::video::video_client<T, U>::frame_sequence_t>::iterator it;
                    for (it = this->buffer_.begin(); it != this->buffer_.end(); ++it)
                    {
                        if (it->first != std::string(server_id))
                        {
                            it->second.clear();
                        }
                    }
                }

                // hold for next temporal window
                nicta::util::sleep(this->controller_->period() * this->sleep_fraction_);
                continue;
            }
        }


        // From here down, code is same as for op()(start, incr):  with the *exception* of if-block at [@ @ @]


        bool increment = false;
        std::set<std::string> terminated;
        for (typename std::map<std::string, typename nicta::vibe::video::video_client<T, U>::frame_sequence_t>::iterator it = this->buffer_.begin();
             it != this->buffer_.end();
             ++it)
        {
            if ((!this->initialized_(it->first) && !it->second.size()) || terminated.find(it->first) != terminated.end())
            {
                continue;
            }

            if (!start.is_not_a_date_time())
            {
                // find a time >= start
                typename std::list<typename nicta::vibe::video::video_client<T, U>::frame_t>::iterator pos =
                                                            std::lower_bound(it->second.begin(),
                                                                             it->second.end(),
                                                                             start,
                                                                             typename nicta::vibe::video::video_client<T, U>::template predicate_<T, U>());

                if ((!this->initialized_(it->first) || this->start_time_matched_[it->first]) && pos == it->second.end())
                {
                    if (boost::posix_time::microsec_clock::local_time() - it->second.back()->template get<nicta::vibe::video::TimeStamp>() >
                        incr + this->controller_->period() * 3)
                    {
                        // no more data ..and last held through has previously been processed - this will empty the container for the associated server

                        it->second.erase(it->second.begin(), pos);
                        terminated.insert(it->first);

                        // [@ @ @]:  goto shutdown ..
                        if (it->first == server_id)
                        {
                            this->janitor_(terminated);
                            break;
                        }
                    }
                    else if (it->second.size() > 1)
                    {
                        // we don't want what's come in so far, ..throw it away

                        it->second.erase(it->second.begin(), --pos);
                    }
                    continue;
                }
                it->second.erase(it->second.begin(), pos);
                this->start_time_matched_[it->first] = true;
                if (!increment)
                {
                    start     += incr;
                    increment  = true;
                }
            }


            bool flag = false;
            boost::posix_time::ptime time = (*it->second.begin())->template get<nicta::vibe::video::TimeStamp>();
            if (this->time_.is_not_a_date_time())
            {
                flag = true;
                this->time_ = time;

                // we use this->time_ to ensure we don't pass the the same image as the one prior ...
            }

            bool handle = true;
            if (flag || time != this->time_)
            {
                flag = false;
                this->time_ = time;

                // if (this->initialized_(it->first) == true && handle == false):  user has closed window [X]
                boost::shared_ptr<nicta::vibe::image::image<T, U> >
                     frame_copy(new nicta::vibe::image::image<T, U>(*(*it->second.begin())->template get<nicta::vibe::video::Image>()));
                handle = ftor(frame_copy,                                                     // for user algorithm safety, a copy *is* be passed through!!
                              (*it->second.begin())->template get<nicta::vibe::video::TimeStamp>(),
                              (*it->second.begin())->template get<nicta::vibe::video::FrameIndex>(),
                              (*it->second.begin())->template get<nicta::vibe::video::PayloadIndex>(),
                              (*it->second.begin())->template get<nicta::vibe::video::Payload>(),
                              it->first,
                              this->id_,
                              this->controller_->client_links(this->id_),
                              secondary_terminated[it->first],
                              &nicta::vibe::video::template video_client<T, U>::shutdown,
                              const_cast<const nicta::vibe::video::video_client<T, U> * const> (this));
            }

            /*
            std::cout << std::endl << "** [" << this->id_ << " : " << it->first.c_str() << "] ** "
                                   << (*it->second.begin())->template get<nicta::vibe::video::FrameIndex>() << "  "
                                   << (*it->second.begin())->template get<nicta::vibe::video::PayloadIndex>() << "  "
                                   << (*it->second.begin())->template get<nicta::vibe::video::Payload>() << "  "
                                   << (*it->second.begin())->template get<nicta::vibe::video::TimeStamp>() << "  "
                                   << it->second.size();
            */


            if (it->second.size() > 1)
            {
                // we can remove the if'arg and just keep code below, but if we are at different period than server, we don't want to throw last valid frame

                it->second.erase(it->second.begin(), ++it->second.begin());
            }


            // logic so as not to abort if we are on the penultimate frame - so that we can show last frame before exiting..
            if (this->initialized_(it->first) && it->second.size() == (*it->second.begin())->template get<nicta::vibe::video::Payload>())
            {
                boost::posix_time::time_duration td = (this->controller_->period() * (2 + 1)) * this->controller_->frame_rate(this, it->first);
                if (boost::posix_time::microsec_clock::local_time() - this->rx_timestamp_.find(it->first)->second > td)
                {
                    if (this->expired_[it->first])
                    {
                        terminated.insert(it->first);
                        this->expired_.erase(it->first);

                        // [@ @ @]:  goto shutdown ..
                        if (it->first == server_id)
                        {
                            this->janitor_(terminated);
                            break;
                        }
                    }
                    this->expired_[it->first] = true;
                }
            }

            // user signalled death
            if (this->initialized_(it->first) && !handle)
            {
                // user has closed window [X]
                terminated.insert(it->first);

                // [@ @ @]:  goto shutdown ..
                if (it->first == server_id)
                {
                    this->janitor_(terminated);
                    break;
                }
            }
            else if (this->offline() || nicta::vibe::video::xevent::detail::keyboard_handler< >()(this->event_key_))
            {
                // user sent signal via ftor *or* keyboard signal to kill this client
                // NOTE:  this condition (hence *all* keyboard keystrokes) will be ignored if user this is a *console-only* application!

                this->janitor_(terminated);
                break;
            }

            this->set_initialized_(it->first);
        }

        // clean-up
        bool erasure = false;
        for (typename std::set<std::string>::iterator jt = terminated.begin(); jt != terminated.end(); ++jt)
        {
            erasure = true;
            this->buffer_.erase(*jt);
            this->controller_->unlink(this, *jt);

            std::string tag;
            nicta::vibe::video::display::window_tag()(this->id_, *jt, tag);
            if (nicta::vibe::image::display::window_handle(tag.c_str()))
            {
                nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                       , boost::mutex
                                       >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                               < void
                                                                                               , const char *
                                                                                               , boost::unique_lock
                                                                                               >(&nicta::vibe::image::display::destroy_window, tag.c_str());
            }
            for (typename std::set<std::string>::iterator kt = secondary_terminated[*jt].begin(); kt != secondary_terminated[*jt].end(); ++kt)
            {
                if (nicta::vibe::image::display::window_handle((*kt).c_str()))
                {
                    nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                           , boost::mutex
                                           >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                            < void
                                                                                            , const char *
                                                                                            , boost::unique_lock
                                                                                            >(&nicta::vibe::image::display::destroy_window, (*kt).c_str());
                }
            }
        }
        if (erasure && this->buffer_.empty())
        {
            this->shutdown();
            break;
        }

        // hold for next temporal window
        nicta::util::sleep(this->controller_->period() * this->sleep_fraction_);
    }
    this->janitor_(secondary_terminated);

    //std::cout << std::endl << "DEAD THREAD " << std::endl;
}


template < typename T
         , template <typename T_> class U
         >
template < unsigned int N
         , typename Functor
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(Functor& ftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Step through each this->buffer_[id] sequentially
    // Get frames from video_server and apply ftor
    // NOTE:  need this as boost::bind does not acknowledge default arguments
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  this variant sends through N-frames to client functor

    nicta::vibe::video::algorithm::canonical_identity sftor;
    nicta::vibe::video::algorithm::deinterlace_identity<1U, T, U> dftor;
    this->template operator()< N
                             , Functor
                             , nicta::vibe::video::algorithm::canonical_identity
                             , 1U
                             , nicta::vibe::video::algorithm::deinterlace_identity
                             >(boost::posix_time::ptime(), boost::posix_time::time_duration(0, 0, 0, 0), ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < unsigned int N
         , typename Functor
         , typename ServerFunctor
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(Functor& ftor,
           ServerFunctor& sftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Step through each this->buffer_[id] sequentially
    // Get frames from video_server and apply ftor
    // NOTE:  need this as boost::bind does not acknowledge default arguments
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  this variant sends through N-frames to client functor
    // NOTE:  sftor only meaningful for non-threaded contexts!

    nicta::vibe::video::algorithm::deinterlace_identity<1U, T, U> dftor;
    this->template operator()< N
                             , Functor
                             , ServerFunctor
                             , 1U
                             , nicta::vibe::video::algorithm::deinterlace_identity
                             >(boost::posix_time::ptime(), boost::posix_time::time_duration(0, 0, 0, 0), ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < unsigned int N
         , typename Functor
         , unsigned int Payload
         , template <unsigned int Payload_, typename T_, template <typename _T> class U_> class ServerDeinterlace
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(Functor& ftor,
           ServerDeinterlace<Payload, T, U>& dftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Step through each this->buffer_[id] sequentially
    // Get frames from video_server and apply ftor
    // NOTE:  need this as boost::bind does not acknowledge default arguments
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  this variant sends through N-frames to client functor
    // NOTE:  dftor only meaningful for non-threaded contexts!

    BOOST_STATIC_ASSERT(( Payload > 0 ));

    nicta::vibe::video::algorithm::canonical_identity sftor;
    this->template operator()< N
                             , Functor
                             , nicta::vibe::video::algorithm::canonical_identity
                             , Payload
                             , ServerDeinterlace
                             >(boost::posix_time::ptime(), boost::posix_time::time_duration(0, 0, 0, 0), ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < unsigned int N
         , typename Functor
         , typename ServerFunctor
         , unsigned int Payload
         , template <unsigned int Payload_, typename T_, template <typename _T> class U_> class ServerDeinterlace
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(Functor& ftor,
           ServerFunctor& sftor,
           ServerDeinterlace<Payload, T, U>& dftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Step through each this->buffer_[id] sequentially
    // Get frames from video_server and apply ftor
    // NOTE:  need this as boost::bind does not acknowledge default arguments
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  this variant sends through N-frames to client functor
    // NOTE:  sftor, dftor only meaningful for non-threaded contexts!

    BOOST_STATIC_ASSERT(( Payload > 0 ));

    this->template operator()< N
                             , Functor
                             , ServerFunctor
                             , Payload
                             , ServerDeinterlace
                             >(boost::posix_time::ptime(), boost::posix_time::time_duration(0, 0, 0, 0), ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < unsigned int N
         , typename Functor
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(boost::posix_time::ptime start, Functor& ftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Search for start_time `start' at each this->buffer_[id], and step through each this->buffer_[id] sequentially thereafter
    // Get frames from video_server and apply ftor
    // NOTE:  need this as boost::bind does not acknowledge default arguments
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  this variant sends through N-frames to client functor

    nicta::vibe::video::algorithm::canonical_identity sftor;
    nicta::vibe::video::algorithm::deinterlace_identity<1U, T, U> dftor;
    this->template operator()< N
                             , Functor
                             , nicta::vibe::video::algorithm::canonical_identity
                             , 1U
                             , nicta::vibe::video::algorithm::deinterlace_identity
                             >(start, boost::posix_time::time_duration(0, 0, 0, 0), ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < unsigned int N
         , typename Functor
         , typename ServerFunctor
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(boost::posix_time::ptime start,
           Functor& ftor,
           ServerFunctor& sftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Search for start_time `start' at each this->buffer_[id], and step through each this->buffer_[id] sequentially thereafter
    // Get frames from video_server and apply ftor
    // NOTE:  need this as boost::bind does not acknowledge default arguments
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  this variant sends through N-frames to client functor
    // NOTE:  sftor only meaningful for non-threaded contexts!

    nicta::vibe::video::algorithm::deinterlace_identity<1U, T, U> dftor;
    this->template operator()< N
                             , Functor
                             , ServerFunctor
                             , 1U
                             , nicta::vibe::video::algorithm::deinterlace_identity
                             >(start, boost::posix_time::time_duration(0, 0, 0, 0), ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < unsigned int N
         , typename Functor
         , unsigned int Payload
         , template <unsigned int Payload_, typename T_, template <typename _T> class U_> class ServerDeinterlace
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(boost::posix_time::ptime start,
           Functor& ftor,
           ServerDeinterlace<Payload, T, U>& dftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Search for start_time `start' at each this->buffer_[id], and step through each this->buffer_[id] sequentially thereafter
    // Get frames from video_server and apply ftor
    // NOTE:  need this as boost::bind does not acknowledge default arguments
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  this variant sends through N-frames to client functor
    // NOTE:  dftor only meaningful for non-threaded contexts!

    BOOST_STATIC_ASSERT(( Payload > 0 ));

    nicta::vibe::video::algorithm::canonical_identity sftor;
    this->template operator()< N
                             , Functor
                             , nicta::vibe::video::algorithm::canonical_identity
                             , Payload
                             , ServerDeinterlace
                             >(start, boost::posix_time::time_duration(0, 0, 0, 0), ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < unsigned int N
         , typename Functor
         , typename ServerFunctor
         , unsigned int Payload
         , template <unsigned int Payload_, typename T_, template <typename _T> class U_> class ServerDeinterlace
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(boost::posix_time::ptime start,
           Functor& ftor,
           ServerFunctor& sftor,
           ServerDeinterlace<Payload, T, U>& dftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Search for start_time `start' at each this->buffer_[id], and step through each this->buffer_[id] sequentially thereafter
    // Get frames from video_server and apply ftor
    // NOTE:  need this as boost::bind does not acknowledge default arguments
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  this variant sends through N-frames to client functor
    // NOTE:  sftor, dftor only meaningful for non-threaded contexts!

    BOOST_STATIC_ASSERT(( Payload > 0 ));

    this->template operator()< N
                             , Functor
                             , ServerFunctor
                             , Payload
                             , ServerDeinterlace
                             >(start, boost::posix_time::time_duration(0, 0, 0, 0), ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < unsigned int N
         , typename Functor
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(boost::posix_time::ptime start,
           boost::posix_time::time_duration incr,
           Functor& ftor) const
{
    // Function-call operator - convenience method for initiating thread
    // Search for start_time `start' at each this->buffer_[id], and step through with  start + incr * N  thereafter
    // Get frames from video_server and apply ftor
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  this variant sends through N-frames to client functor

    nicta::vibe::video::algorithm::canonical_identity sftor;
    nicta::vibe::video::algorithm::deinterlace_identity<1U, T, U> dftor;
    this->template operator()< N
                             , Functor
                             , nicta::vibe::video::algorithm::canonical_identity
                             , 1U
                             , nicta::vibe::video::algorithm::deinterlace_identity
                             >(start, incr, ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < unsigned int N
         , typename Functor
         , typename ServerFunctor
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(boost::posix_time::ptime start,
           boost::posix_time::time_duration incr,
           Functor& ftor,
           ServerFunctor& sftor) const
{
    // Function-call operator - convenience methed for non-threaded contexts
    // Search for start_time `start' at each this->buffer_[id], and step through with  start + incr * N  thereafter
    // Get frames from video_server and apply ftor
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  this variant sends through N-frames to client functor
    // NOTE:  sftor only meaningful for non-threaded contexts!

    nicta::vibe::video::algorithm::deinterlace_identity<1U, T, U> dftor;
    this->template operator()< N
                             , Functor
                             , ServerFunctor
                             , 1U
                             , nicta::vibe::video::algorithm::deinterlace_identity
                             >(start, incr, ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < unsigned int N
         , typename Functor
         , unsigned int Payload
         , template <unsigned int Payload_, typename T_, template <typename _T> class U_> class ServerDeinterlace
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(boost::posix_time::ptime start,
           boost::posix_time::time_duration incr,
           Functor& ftor,
           ServerDeinterlace<Payload, T, U>& dftor) const
{
    // Function-call operator - convenience method for non-threaded contexts
    // Search for start_time `start' at each this->buffer_[id], and step through with  start + incr * N  thereafter
    // Get frames from video_server and apply ftor
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  this variant sends through N-frames to client functor
    // NOTE:  dftor only meaningful for non-threaded contexts!

    BOOST_STATIC_ASSERT(( Payload > 0 ));

    nicta::vibe::video::algorithm::canonical_identity sftor;
    this->template operator()< N
                             , Functor
                             , nicta::vibe::video::algorithm::canonical_identity
                             , Payload
                             , ServerDeinterlace
                             >(start, incr, ftor, sftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < unsigned int N
         , typename Functor
         , typename ServerFunctor
         , unsigned int Payload
         , template <unsigned int Payload_, typename T_, template <typename _T> class U_> class ServerDeinterlace
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(boost::posix_time::ptime start,
           boost::posix_time::time_duration incr,
           Functor& ftor,
           ServerFunctor& sftor,
           ServerDeinterlace<Payload, T, U>& dftor) const
{
    // Function-call operator - convenience method for initiating thread (or non-thread)
    // Search for start_time `start' at each this->buffer_[id], and step through with  start + incr * N  thereafter
    // Get frames from video_server and apply ftor
    // NOTE:  this function works dually for both thread and non-thread -aware contexts..
    // NOTE:  this variant sends through N-frames to client functor
    // NOTE:  sftor, dftor only meaningful for non-threaded contexts!

    BOOST_MPL_ASSERT(( boost::mpl::bool_<N != 0> ));
    BOOST_STATIC_ASSERT(( Payload > 0 ));

    std::map<std::string, std::set<std::string> > secondary_terminated;
    while (!this->initialized_() || (this->buffered_() && this->online()))
    {
        if (!this->controller_)
        {
            // for thread-unaware contexts..

            if (!this->server_->online()) break;
            this->server_->template operator()<ServerFunctor, Payload, ServerDeinterlace>(sftor, dftor);
            if (this->server_->held_frame_index() % this->frame_rate() != 0) continue;
            this->rx(this->server_->tx(), this->server_->id(), this->server_->payload());
        }

        bool increment = false;
        std::set<std::string> terminated;
        for (typename std::map<std::string, typename nicta::vibe::video::video_client<T, U>::frame_sequence_t>::iterator it = this->buffer_.begin();
             it != this->buffer_.end();
             ++it)
        {
            if ((!this->initialized_(it->first) && !it->second.size()) || terminated.find(it->first) != terminated.end())
            {
                continue;
            }

            // unfortunately, a runtime error check:
            if (!this->initialized_())
            {
                if (N > (*it->second.begin())->template get<nicta::vibe::video::Payload>())
                {
                    std::cerr << std::endl << " ********* ERROR:  template parameter N exceeds server payload size *********" << std::endl;
                    this->shutdown();
                }
            }

            if (!start.is_not_a_date_time())
            {
                // find a time >= start
                typename std::list<typename nicta::vibe::video::video_client<T, U>::frame_t>::iterator pos =
                                                            std::lower_bound(it->second.begin(),
                                                                             it->second.end(),
                                                                             start,
                                                                             typename nicta::vibe::video::video_client<T, U>::template predicate_<T, U>());

                if ((!this->initialized_(it->first) || this->start_time_matched_[it->first]) && pos == it->second.end())
                {
                    boost::posix_time::time_duration period = this->controller_ ? this->controller_->period()
                                                                                : nicta::util::fps_period(this->server_->fps());
                    if (boost::posix_time::microsec_clock::local_time() - it->second.back()->template get<nicta::vibe::video::TimeStamp>() >
                        incr + period * 3)
                    {
                        // no more data ..and last held through has previously been processed - this will empty the container for the associated server

                        it->second.erase(it->second.begin(), pos);
                        terminated.insert(it->first);
                    }
                    else if (it->second.size() > 1)
                    {
                        // we don't want what's come in so far, ..throw it away

                        it->second.erase(it->second.begin(), --pos);
                    }
                    continue;
                }
                it->second.erase(it->second.begin(), pos);
                this->start_time_matched_[it->first] = true;
                if (!increment)
                {
                    start     += incr;
                    increment  = true;
                }
            }


            bool flag = false;
            boost::posix_time::ptime time = (*it->second.begin())->template get<nicta::vibe::video::TimeStamp>();
            if (this->time_.is_not_a_date_time())
            {
                flag = true;
                this->time_ = time;

                // we use this->time_ to ensure we don't pass the the same image as the one prior ...
            }

            bool handle = true;
            if (flag || time != this->time_)
            {
                flag = false;
                this->time_ = time;

                // if (this->initialized_(it->first) == true && handle == false):  user has closed window [X]
                unsigned int n = 0;
                boost::array<unsigned int, N> payload_indices;
                boost::array<boost::shared_ptr<nicta::vibe::image::image<T, U> >, N> frames;
                for (typename nicta::vibe::video::video_client<T, U>::frame_sequence_t::iterator iter = it->second.begin();
                     iter != it->second.end() && n != N;
                     ++iter, ++n)
                {
                    frames[n] = boost::shared_ptr<nicta::vibe::image::image<T, U> >
                                     (new nicta::vibe::image::image<T, U>(*(*iter)->template get<nicta::vibe::video::Image>()));
                    payload_indices[n] = (*iter)->template get<nicta::vibe::video::PayloadIndex>();
                }
                unsigned int host_connection_count = this->controller_ ? this->controller_->client_links(this->id_) : 1U;
                handle = ftor(frames,                                                         // for user algorithm safety, a copy *is* be passed through!!
                              (*it->second.begin())->template get<nicta::vibe::video::TimeStamp>(),
                              (*it->second.begin())->template get<nicta::vibe::video::FrameIndex>(),
                              payload_indices,
                              (*it->second.begin())->template get<nicta::vibe::video::Payload>(),
                              it->first,
                              this->id_,
                              host_connection_count,
                              secondary_terminated[it->first],
                              &nicta::vibe::video::template video_client<T, U>::shutdown,
                              const_cast<const nicta::vibe::video::video_client<T, U> * const> (this));
            }

            /*
            std::cout << std::endl << "** [" << this->id_ << " : " << it->first.c_str() << "] ** "
                                   << (*it->second.begin())->template get<nicta::vibe::video::FrameIndex>() << "  "
                                   << (*it->second.begin())->template get<nicta::vibe::video::PayloadIndex>() << "  "
                                   << (*it->second.begin())->template get<nicta::vibe::video::Payload>() << "  "
                                   << (*it->second.begin())->template get<nicta::vibe::video::TimeStamp>() << "  "
                                   << it->second.size();
            */


            if (it->second.size() > N)
            {
                // we can remove the if'arg and just keep code below, but if we are at different period than server, we don't want to throw last valid frame

                typename nicta::vibe::video::video_client<T, U>::frame_sequence_t::iterator iter = it->second.begin();
                std::advance(iter, N);
                it->second.erase(it->second.begin(), iter);
            }


            if (this->controller_)
            {
                // for thread-aware contexts..

                // logic so as not to abort if we are on the penultimate frame - so that we can show last frame before exiting..
                if (this->initialized_(it->first) && it->second.size() == (*it->second.begin())->template get<nicta::vibe::video::Payload>())
                {
                    boost::posix_time::time_duration td = (this->controller_->period() * (2 + 1)) * this->controller_->frame_rate(this, it->first);
                    if (boost::posix_time::microsec_clock::local_time() - this->rx_timestamp_.find(it->first)->second > td)
                    {
                        if (this->expired_[it->first])
                        {
                            terminated.insert(it->first);
                            this->expired_.erase(it->first);
                        }
                        this->expired_[it->first] = true;
                    }
                }
            }

            // user signalled death
            if (this->initialized_(it->first) && !handle)
            {
                // user has closed window [X]
                terminated.insert(it->first);
            }
            else if (this->offline() || nicta::vibe::video::xevent::detail::keyboard_handler< >()(this->event_key_))
            {
                // user sent signal via ftor *or* keyboard signal to kill this client
                // NOTE:  this condition (hence *all* keyboard keystrokes) will be ignored if user this is a *console-only* application!

                this->janitor_(terminated);
                break;
            }

            this->set_initialized_(it->first);
        }

        // clean-up
        bool erasure = false;
        for (typename std::set<std::string>::iterator jt = terminated.begin(); jt != terminated.end(); ++jt)
        {
            erasure = true;
            this->buffer_.erase(*jt);
            if (this->controller_)
            {
                // for thread-aware contexts..

                this->controller_->unlink(this, *jt);
            }

            std::string tag;
            nicta::vibe::video::display::window_tag()(this->id_, *jt, tag);
            if (nicta::vibe::image::display::window_handle(tag.c_str()))
            {
                nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                       , boost::mutex
                                       >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                               < void
                                                                                               , const char *
                                                                                               , boost::unique_lock
                                                                                               >(&nicta::vibe::image::display::destroy_window, tag.c_str());

            }
            for (typename std::set<std::string>::iterator kt = secondary_terminated[*jt].begin(); kt != secondary_terminated[*jt].end(); ++kt)
            {
                if (nicta::vibe::image::display::window_handle((*kt).c_str()))
                {
                    nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                           , boost::mutex
                                           >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                             < void
                                                                                             , const char *
                                                                                             , boost::unique_lock
                                                                                             >(&nicta::vibe::image::display::destroy_window, (*kt).c_str());
                }
            }
        }
        if (erasure && this->buffer_.empty())
        {
            this->shutdown();
            break;
        }

        // hold for next temporal window
        if (this->controller_)
        {
            // for thread-aware contexts..

            nicta::util::sleep(this->controller_->period() * this->sleep_fraction_);
        }
        else
        {
            // for thread-unaware contexts..

            nicta::util::sleep(nicta::util::fps_period(this->server_->fps()) * this->sleep_fraction_);
        }
    }
    this->janitor_(secondary_terminated);

    //std::cout << std::endl << "DEAD THREAD " << std::endl;
}


template < typename T
         , template <typename T_> class U
         >
template < unsigned int N
         , typename Functor
         >
typename nicta::vibe::video::video_client<T, U>::result_type
nicta::vibe::video::video_client<T, U> ::
operator()(const nicta::vibe::video::video_server<T, U>& server,
           boost::posix_time::ptime start,
           boost::posix_time::time_duration incr,
           Functor& ftor
          ) const
{
    // Function-call operator - convenience method for initiating thread
    // UNFORTUNATELY, boost::bind can not distinguish overloads in this case, so user has to be explicit [b/c of the const char*] :-(
    // 1-arg op(server_id):
    //       Use timestamps on this->buffer_[server_id], and use for all buffers                                   == [ op()(server, NADT, td(0), ftor) ]
    // 2-arg op(server_id, start):     
    //       Use timestamps on this->buffer_[server_id], starting at time `start', and use for all buffers         == [ op(server, start, td(0), ftor)  ]
    // 3-arg op()(server_id, start, incr):
    //       Use timestamps on this->buffer_[server_id], starting at time `start', and use for all buffers
    //       .. and stepping through this->buffer_[server_id] with  start + incr * N  thereafter                   == [ op(server, start, td, ftor)     ]
    // Get frames from video_server and apply ftor
    // NOTE:  this function is *ONLY* meant for thread -aware contexts.. !!
    // NOTE:  this variant sends through N-frames to client functor

    BOOST_MPL_ASSERT(( boost::mpl::bool_<N != 0> ));

    bool nadt = start.is_not_a_date_time();
    const char* server_id = server.id().c_str();
    std::map<std::string, std::set<std::string> > secondary_terminated;
    while (!this->initialized_() || (this->buffered_() && this->online()))
    {
        if (!this->controller_->online(server_id) && this->buffer_[server_id].empty())
        {
            break;
        }

        if (!this->buffer_[server_id].size())
        {
            for (typename std::map<std::string, typename nicta::vibe::video::video_client<T, U>::frame_sequence_t>::iterator it = this->buffer_.begin();
                 it != this->buffer_.end();
                 ++it)
            {
                it->second.clear();
            }

            // hold for next temporal window
            nicta::util::sleep(this->controller_->period());
            continue;
        }

        if (nadt)
        {
            // start came in uninitialized..

            typename nicta::vibe::video::video_client<T, U>::frame_sequence_t::const_iterator pos = this->buffer_[server_id].begin();
            if (pos == this->buffer_[server_id].end())
            {
                break;
            }
            start = (*pos)->template get<nicta::vibe::video::TimeStamp>();
        }
        else
        {
            // start came in initialized..

            typename nicta::vibe::video::video_client<T, U>::frame_sequence_t& li = this->buffer_[server_id];
            if (li.begin() == li.end())
            {
                break;
            }

            // find a time >= start
            typename std::list<typename nicta::vibe::video::video_client<T, U>::frame_t>::iterator pos =
                                                           std::lower_bound(li.begin(),
                                                                            li.end(),
                                                                            start,
                                                                            typename nicta::vibe::video::video_client<T, U>::template predicate_<T, U>());

            if ((!this->initialized_(server_id) || this->start_time_matched_[server_id]) && pos == li.end())
            {
                if (boost::posix_time::microsec_clock::local_time() - li.back()->template get<nicta::vibe::video::TimeStamp>() >
                    incr + this->controller_->period() * 3)
                {
                    // no more data ..and last held through has previously been processed - this will empty the container for the associated server
                    //li.erase(li.begin(), pos);

                    // goto shutdown ...
                    typename std::map<std::string, typename nicta::vibe::video::video_client<T, U>::frame_sequence_t>::iterator it;
                    for (it = this->buffer_.begin(); it != this->buffer_.end(); ++it)
                    {
                        this->controller_->unlink(this, it->first);

                        std::string tag;
                        nicta::vibe::video::display::window_tag()(this->id_, it->first, tag);
                        if (nicta::vibe::image::display::window_handle(tag.c_str()))
                        {
                            nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                                   , boost::mutex
                                                   >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                               < void
                                                                                               , const char *
                                                                                               , boost::unique_lock
                                                                                               >(&nicta::vibe::image::display::destroy_window, tag.c_str());
                        }
                        for (typename std::set<std::string>::iterator jt = secondary_terminated[it->first].begin();
                             jt != secondary_terminated[it->first].end();
                             ++jt)
                        {
                            if (nicta::vibe::image::display::window_handle((*jt).c_str()))
                            {
                                nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                                       , boost::mutex
                                                       >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                            < void
                                                                                            , const char *
                                                                                            , boost::unique_lock
                                                                                            >(&nicta::vibe::image::display::destroy_window, (*jt).c_str());
                            }
                        }
                    }
                    this->buffer_.clear();
                    this->shutdown();
                }
                else if (li.size() > 1)
                {
                    // we don't want what's come in so far, ..throw it away
                    li.erase(li.begin(), --pos);

                    // ..and blow away *all* of everyone else's data
                    typename std::map<std::string, typename nicta::vibe::video::video_client<T, U>::frame_sequence_t>::iterator it;
                    for (it = this->buffer_.begin(); it != this->buffer_.end(); ++it)
                    {
                        if (it->first != std::string(server_id))
                        {
                            it->second.clear();
                        }
                    }
                }

                // hold for next temporal window
                nicta::util::sleep(this->controller_->period() * this->sleep_fraction_);
                continue;
            }
        }


        // From here down, code is same as for op()(start, incr):  with the *exception* of if-block at [@ @ @]


        bool increment = false;
        std::set<std::string> terminated;
        for (typename std::map<std::string, typename nicta::vibe::video::video_client<T, U>::frame_sequence_t>::iterator it = this->buffer_.begin();
             it != this->buffer_.end();
             ++it)
        {
            if ((!this->initialized_(it->first) && !it->second.size()) || terminated.find(it->first) != terminated.end())
            {
                continue;
            }

            // unfortunately, a runtime error check:
            if (!this->initialized_())
            {
                if (N > (*it->second.begin())->template get<nicta::vibe::video::Payload>())
                {
                    std::cerr << std::endl << " ********* ERROR:  template parameter N exceeds server payload size *********" << std::endl;
                    this->shutdown();
                }
            }

            if (!start.is_not_a_date_time())
            {
                // find a time >= start
                typename std::list<typename nicta::vibe::video::video_client<T, U>::frame_t>::iterator pos =
                                                            std::lower_bound(it->second.begin(),
                                                                             it->second.end(),
                                                                             start,
                                                                             typename nicta::vibe::video::video_client<T, U>::template predicate_<T, U>());

                if ((!this->initialized_(it->first) || this->start_time_matched_[it->first]) && pos == it->second.end())
                {
                    if (boost::posix_time::microsec_clock::local_time() - it->second.back()->template get<nicta::vibe::video::TimeStamp>() >
                        incr + this->controller_->period() * 3)
                    {
                        // no more data ..and last held through has previously been processed - this will empty the container for the associated server

                        it->second.erase(it->second.begin(), pos);
                        terminated.insert(it->first);

                        // [@ @ @]:  goto shutdown ..
                        if (it->first == server_id)
                        {
                            this->janitor_(terminated);
                            break;
                        }
                    }
                    else if (it->second.size() > 1)
                    {
                        // we don't want what's come in so far, ..throw it away

                        it->second.erase(it->second.begin(), --pos);
                    }
                    continue;
                }
                it->second.erase(it->second.begin(), pos);
                this->start_time_matched_[it->first] = true;
                if (!increment)
                {
                    start     += incr;
                    increment  = true;
                }
            }


            bool flag = false;
            boost::posix_time::ptime time = (*it->second.begin())->template get<nicta::vibe::video::TimeStamp>();
            if (this->time_.is_not_a_date_time())
            {
                flag = true;
                this->time_ = time;

                // we use this->time_ to ensure we don't pass the the same image as the one prior ...
            }

            bool handle = true;
            if (flag || time != this->time_)
            {
                flag = false;
                this->time_ = time;

                // if (this->initialized_(it->first) == true && handle == false):  user has closed window [X]
                unsigned int n = 0;
                boost::array<unsigned int, N> payload_indices;
                boost::array<boost::shared_ptr<nicta::vibe::image::image<T, U> >, N> frames;
                for (typename nicta::vibe::video::video_client<T, U>::frame_sequence_t::iterator iter = it->second.begin();
                     iter != it->second.end() && n != N;
                     ++iter, ++n)
                {
                    frames[n] = boost::shared_ptr<nicta::vibe::image::image<T, U> >
                                     (new nicta::vibe::image::image<T, U>(*(*iter)->template get<nicta::vibe::video::Image>()));
                    payload_indices[n] = (*iter)->template get<nicta::vibe::video::PayloadIndex>();
                }
                handle = ftor(frames,                                                         // for user algorithm safety, a copy *is* be passed through!!
                              (*it->second.begin())->template get<nicta::vibe::video::TimeStamp>(),
                              (*it->second.begin())->template get<nicta::vibe::video::FrameIndex>(),
                              payload_indices,
                              (*it->second.begin())->template get<nicta::vibe::video::Payload>(),
                              it->first,
                              this->id_,
                              this->controller_->client_links(this->id_),
                              secondary_terminated[it->first],
                              &nicta::vibe::video::template video_client<T, U>::shutdown,
                              const_cast<const nicta::vibe::video::video_client<T, U> * const> (this));
            }

            /*
            std::cout << std::endl << "** [" << this->id_ << " : " << it->first.c_str() << "] ** "
                                   << (*it->second.begin())->template get<nicta::vibe::video::FrameIndex>() << "  "
                                   << (*it->second.begin())->template get<nicta::vibe::video::PayloadIndex>() << "  "
                                   << (*it->second.begin())->template get<nicta::vibe::video::Payload>() << "  "
                                   << (*it->second.begin())->template get<nicta::vibe::video::TimeStamp>() << "  "
                                   << it->second.size();
            */


            if (it->second.size() > N)
            {
                // we can remove the if'arg and just keep code below, but if we are at different period than server, we don't want to throw last valid frame

                typename nicta::vibe::video::video_client<T, U>::frame_sequence_t::iterator iter = it->second.begin();
                std::advance(iter, N);
                it->second.erase(it->second.begin(), iter);
            }


            // logic so as not to abort if we are on the penultimate frame - so that we can show last frame before exiting..
            if (this->initialized_(it->first) && it->second.size() == (*it->second.begin())->template get<nicta::vibe::video::Payload>())
            {
                boost::posix_time::time_duration td = (this->controller_->period() * (2 + 1)) * this->controller_->frame_rate(this, it->first);
                if (boost::posix_time::microsec_clock::local_time() - this->rx_timestamp_.find(it->first)->second > td)
                {
                    if (this->expired_[it->first])
                    {
                        terminated.insert(it->first);
                        this->expired_.erase(it->first);

                        // [@ @ @]:  goto shutdown ..
                        if (it->first == server_id)
                        {
                            this->janitor_(terminated);
                            break;
                        }
                    }
                    this->expired_[it->first] = true;
                }
            }

            // user signalled death
            if (this->initialized_(it->first) && !handle)
            {
                // user has closed window [X]
                terminated.insert(it->first);

                // [@ @ @]:  goto shutdown ..
                if (it->first == server_id)
                {
                    this->janitor_(terminated);
                    break;
                }
            }
            else if (this->offline() || nicta::vibe::video::xevent::detail::keyboard_handler< >()(this->event_key_))
            {
                // user sent signal via ftor *or* keyboard signal to kill this client
                // NOTE:  this condition (hence *all* keyboard keystrokes) will be ignored if user this is a *console-only* application!

                this->janitor_(terminated);
                break;
            }

            this->set_initialized_(it->first);
        }

        // clean-up
        bool erasure = false;
        for (typename std::set<std::string>::iterator jt = terminated.begin(); jt != terminated.end(); ++jt)
        {
            erasure = true;
            this->buffer_.erase(*jt);
            this->controller_->unlink(this, *jt);

            std::string tag;
            nicta::vibe::video::display::window_tag()(this->id_, *jt, tag);
            if (nicta::vibe::image::display::window_handle(tag.c_str()))
            {
                nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                       , boost::mutex
                                       >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                               < void
                                                                                               , const char *
                                                                                               , boost::unique_lock
                                                                                               >(&nicta::vibe::image::display::destroy_window, tag.c_str());
            }
            for (typename std::set<std::string>::iterator kt = secondary_terminated[*jt].begin(); kt != secondary_terminated[*jt].end(); ++kt)
            {
                if (nicta::vibe::image::display::window_handle((*kt).c_str()))
                {
                    nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                           , boost::mutex
                                           >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                            < void
                                                                                            , const char *
                                                                                            , boost::unique_lock
                                                                                            >(&nicta::vibe::image::display::destroy_window, (*kt).c_str());
                }
            }
        }
        if (erasure && this->buffer_.empty())
        {
            this->shutdown();
            break;
        }

        // hold for next temporal window
        nicta::util::sleep(this->controller_->period() * this->sleep_fraction_);
    }
    this->janitor_(secondary_terminated);

    //std::cout << std::endl << "DEAD THREAD " << std::endl;
}


template < typename T
         , template <typename T_> class U
         >
void
nicta::vibe::video::video_client<T, U> :: 
shutdown() const throw ()
{
    nicta::thread::guard<boost::mutex, boost::unique_lock> lock(this->eof_mutex_);
    this->eof_ = true;
}


template < typename T
         , template <typename T_> class U
         >
void
nicta::vibe::video::video_client<T, U> :: 
rx(const typename nicta::vibe::video::sp_iplframe::type* packet,
   const std::string& id,
   unsigned int payload
  ) const throw ()
{
    // Receive frame data packet from server with 'id' via controller thread

    typename nicta::vibe::video::video_client<T, U>::frame_sequence_t::size_type size = this->buffer_[id].size();
    if (size &&
        this->buffer_[id].back()->template get<nicta::vibe::video::TimeStamp>() == packet[0]->template get<nicta::vibe::video::TimeStamp>())
    {
        // forbid duplicate frames
        // NOTE:  use TimeStamp (not FrameIndex) in condition above b/c cameras will always have FrameIndex == 0

        return;
    }

    if (this->bufsize_ > 0 && static_cast<long int> (size) > this->bufsize_)
    {
        // drop [upto] (oldest) frame-payload (and accept new one) if buffer is larger than threshold:  this->bufsize_ && this->bufsize_ != 0

        // IMPORTANT NOTE: client algorithm **MUST** make a copy of image when it gets it! else may be invalidated if the users' algo is too slow
        //                 ..because frame is passed by reference, and here it is pop'd off if buffer size gets larger than threshold!!
        // ..On behalf of the client, we *DO* make a copy before dispatching to the user-defined algorithm [..safety achieved outweighs the copy overhead!]


        typename nicta::vibe::video::video_client<T, U>::frame_sequence_t::iterator iter = this->buffer_[id].begin();
        if (static_cast<unsigned int> (size) >= payload)
        {
            std::advance(iter, payload);
        }
        else
        {
            std::advance(iter, size);
        }
        this->buffer_[id].erase(this->buffer_[id].begin(), iter);
    }

    bool flag = true;
    if (this->bufsize_ < 0)
    {
        if (static_cast<long int> (size) == std::labs(this->bufsize_))
        {
            // consume frames in this->buffer_ first, before add any more

            flag = false;
        }
        else if (!size)
        {
            // Ok to add more frames now (until reach bufsize_ again) ..

            flag = true;
        }
    }

    if (flag)
    {
        for (unsigned int i = 0; i != payload; ++i)
        {
            // std::map<std::string, std::list<nicta::vibe::video::sp_frame<Depth, Pixel> > >
            this->buffer_[id].push_back(typename nicta::vibe::video::sp_frame<T, U>::type
                  (new typename nicta::vibe::video::frame<T, U>::type(new nicta::vibe::image::image<T, U>(packet[i]->template get<nicta::vibe::video::Image>()),
                                                                      packet[i]->template get<nicta::vibe::video::Payload>(),
                                                                      packet[i]->template get<nicta::vibe::video::PayloadIndex>(),
                                                                      packet[i]->template get<nicta::vibe::video::FrameIndex>(),
                                                                      packet[i]->template get<nicta::vibe::video::TimeStamp>()),
                   typename nicta::vibe::video::image_deleter<T, U>()));
        }
        this->rx_timestamp_[id] = boost::posix_time::microsec_clock::local_time();
    }
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::video::video_client<T, U> :: 
online() const throw ()
{
    nicta::thread::guard<boost::mutex, boost::unique_lock> lock(this->eof_mutex_);
    return !this->eof_;
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::video::video_client<T, U> :: 
offline() const throw ()
{
    return !this->online();
}


template < typename T
         , template <typename T_> class U
         >
const std::string &
nicta::vibe::video::video_client<T, U> :: 
id() const throw ()
{
    return this->id_;
}


template < typename T
         , template <typename T_> class U
         >
unsigned long int
nicta::vibe::video::video_client<T, U> :: 
frame_rate() const throw ()
{
    return this->frame_rate_;
}


template < typename T
         , template <typename T_> class U
         >
unsigned long int
nicta::vibe::video::video_client<T, U> :: 
stream_length() const throw ()
{
    // Get total number of frames available for *all* server streams associated with 'client' *at* connection-time
    // NOTE:  will **always** return 1 for camera connections!  ...only makes sense for file-connections!!

    if (this->controller_)
    {
        // thread-aware state
        return this->controller_->stream_length(this);
    }
    return this->server_->stream_length();
}


template < typename T
         , template <typename T_> class U
         >
unsigned long int
nicta::vibe::video::video_client<T, U> :: 
stream_length(const std::string& server_id) const throw (nicta::nicta_exception)
{
    // Get total number of frames available on server stream *at* connection-time
    // NOTE:  will **always** return 1 for camera connections!  ...only makes sense for file-connections!!

    if (this->controller_)
    {
        // thread-aware state
        return this->controller_->stream_length(server_id);
    }
    if (this->server_->id() != server_id)
    {
        throw nicta::nicta_exception("no such server", "stream_length", "video_client");
    }
    return this->server_->stream_length();
}



template < typename T
         , template <typename T_> class U
         >
nicta::vibe::video::StreamModel
nicta::vibe::video::video_client<T, U> ::
stream_type(const std::string& server_id) const throw (nicta::nicta_exception)
{
    // Return the source-stream type:  Image(dir of img files) ; File(avi files) ; Camera(camera feed)

    if (this->controller_)
    {
        // thread-aware state
        return this->controller_->stream_type(server_id);
    }
    if (this->server_->id() != server_id)
    {
        throw nicta::nicta_exception("no such server", "stream_length", "video_client");
    }
    return this->server_->stream_type();
}


template < typename T
         , template <typename T_> class U
         >
std::set<std::string>::size_type
nicta::vibe::video::video_client<T, U> ::
initialized_() const throw ()
{
    // Return True if there are no longer *any* `non-initialized' members

    return !this->init_[false].size();
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::video::video_client<T, U> ::
initialized_(const std::string& server_id) const throw ()
{
    // Return True if server_id is amongst the `initialized' members

    return this->init_[true].find(server_id) != this->init_[true].end();
}


template < typename T
         , template <typename T_> class U
         >
void
nicta::vibe::video::video_client<T, U> ::
set_initialized_(const std::string& server_id) const throw ()
{
    // Mark server_id as an initialized member, and remove it from the uninitialized set

    std::set<std::string>::iterator pos = this->init_[false].find(server_id);
    if (pos != this->init_[false].end())
    {
        this->init_[false].erase(pos);
    }
    this->init_[true].insert(server_id);
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::video::video_client<T, U> :: 
buffered_() const throw ()
{
    // Query if buffers still contain any data - so that if servers go offline, we still continue to process held data

    bool result = false;
    for (typename std::map<std::string, typename nicta::vibe::video::video_client<T, U>::frame_sequence_t>::iterator it = this->buffer_.begin();
         it != this->buffer_.end();
         ++it)
    {
        result |= !!it->second.size();
    }
    return result;
}


template < typename T
         , template <typename T_> class U
         >
void
nicta::vibe::video::video_client<T, U> :: 
janitor_(std::set<std::string>& terminated) const throw ()
{
    // Ready client for death ..

    for (typename std::map<std::string, typename nicta::vibe::video::video_client<T, U>::frame_sequence_t>::iterator it = this->buffer_.begin();
         it != this->buffer_.end();
         ++it)
    {
         terminated.insert(it->first);
    }
}


template < typename T
         , template <typename T_> class U
         >
void
nicta::vibe::video::video_client<T, U> :: 
janitor_(const std::map<std::string, std::set<std::string> >& terminated) const throw ()
{
    // Does cleanup duties for rogue deaths

    for (std::set<std::string>::const_iterator it = this->init_[true].begin(); it != this->init_[true].end(); ++it)
    { 
        if (this->controller_)
        {
            // for thread-aware contexts..

            this->controller_->unlink(this, *it);
        }

        std::string tag;
        nicta::vibe::video::display::window_tag()(this->id_, *it, tag);
        if (nicta::vibe::image::display::window_handle(tag.c_str()))
        {
            //nicta::vibe::image::display::destroy_window(tag.c_str());
            nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                   , boost::mutex
                                   >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                               < void
                                                                                               , const char *
                                                                                               , boost::unique_lock
                                                                                               >(&nicta::vibe::image::display::destroy_window, tag.c_str());
        }
    }
    for (std::map<std::string, std::set<std::string> >::const_iterator it = terminated.begin(); it != terminated.end(); ++it)
    {
        for (std::set<std::string>::const_iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
        {
            if (nicta::vibe::image::display::window_handle((*jt).c_str()))
            {
                //nicta::vibe::image::display::destroy_window((*jt).c_str());
                nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                       , boost::mutex
                                       >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                            < void
                                                                                            , const char *
                                                                                            , boost::unique_lock
                                                                                            >(&nicta::vibe::image::display::destroy_window, (*jt).c_str());
            }
        }
    }
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
