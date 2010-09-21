/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_CONTROLLER_H
#define NICTA_VIBE_VIDEO_CONTROLLER_H

#include "video/policy.h"
#include "video/video_client.h"
#include "video/video_server.h"
#include "video/macros/controller.h"
#include "video/models/stream_model.h"
#include "video/thread/gate_model.h"

#include <image/image_depth.h>
#include <image/image_pixel.h>
#include <nicta_common/detail/max_arity.h>
#include <nicta_common/nicta_exception.h>
#include <nicta_common/quantity.h>
#include <nicta_common/util.h>
#include <thread/condition/condition_wait.h>
#include <thread/condition/notify_model.h>
#include <thread/gate/gate.h>
#include <thread/guard/guard.h>
#include <thread/rw_locks/rw_locks.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/tuple/tuple.hpp>

#include <list>
#include <map>
#include <string>


namespace nicta {
namespace vibe {
namespace video {

    /* Forward Declaration */
    template < typename Depth
             , template <typename _> class Pixel
             >
    class video_server;

    /* Forward Declaration */
    template < typename Depth
             , template <typename _> class Pixel
             >
    class video_client;


    template < typename Depth = nicta::vibe::image::depth_8u
             , template <typename _> class Pixel = nicta::vibe::image::bgr_p
             >
    class controller
    {
    public:

        /* IMPORTANT!!  for now, all both servers & clients connected to the *same* controller *must* run at the *same* fps! */


        typedef void result_type;
        typedef nicta::vibe::video::policy<Depth, Pixel> policy_t;
        typedef nicta::vibe::video::video_server<Depth, Pixel> server_t;


        controller(nicta::quantity::frame_rate_per_second<unsigned int> fps =
                        nicta::quantity::frame_rate_per_second<unsigned int> (0)
                  ) throw ();

        void register_server_online(const std::string& id) throw (nicta::nicta_exception);


#define OPERATOR(z, n, _)                                                                                                                         \
                                                                                                                                                  \
        void register_client_online(nicta::vibe::video::video_client<Depth, Pixel>* client,                                                       \
                                    BOOST_PP_ENUM_PARAMS(n, server_t* server),                                                                    \
                                    nicta::quantity::step<unsigned long int> nth =                                                                \
                                         nicta::quantity::step<unsigned long int> (1)                                                             \
                                    ) throw (nicta::nicta_exception)                                                                              \
        {                                                                                                                                         \
            /* Insert a table record associating server(s) & clients linked by a data policy */                                                   \
            /* Once client is threaded, client will initiate comms with controller.. */                                                           \
                                                                                                                                                  \
            nicta::thread::guard<boost::mutex, boost::unique_lock> lock(this->table_mutex_);                                                      \
            BOOST_PP_REPEAT(n, INSERT_CTRL_TABLE_RECORD, server)                                                                                  \
        }                                                                                                                                         \
                                                                                                                                                  \
                                                                                                                                                  \
        void register_client_online(nicta::vibe::video::video_client<Depth, Pixel>* client,                                                       \
                                    BOOST_PP_ENUM_PARAMS(n, policy_t& policy)                                                                     \
                                   ) throw (nicta::nicta_exception)                                                                               \
        {                                                                                                                                         \
            /* Insert a table record associating server(s) & clients linked by a data policy */                                                   \
            /* Once client is threaded, client will initiate comms with controller.. */                                                           \
                                                                                                                                                  \
            nicta::thread::guard<boost::mutex, boost::unique_lock> lock(this->table_mutex_);                                                      \
            BOOST_PP_REPEAT(n, INSERT_CTRL_TABLE_RECORD_, policy)                                                                                 \
        }
/**/

#define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
#define BOOST_PP_LOCAL_LIMITS (1, NICTA_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
#undef OPERATOR

/* unload "macros/controller.h" */
#undef INSERT_CTRL_TABLE_RECORD_
#undef INSERT_CTRL_TABLE_RECORD
#undef NICTA_VIBE_MACROS_CONTROLLER_H


        result_type operator()() throw (nicta::nicta_exception);

        double fps() const throw ();
        const boost::posix_time::time_duration& period() const throw ();

        unsigned int server_links(const std::string& server_id) const throw ();
        unsigned int client_links(const std::string& client_id) const throw ();
        void unlink(const nicta::vibe::video::video_client<Depth, Pixel>* const client, const std::string& server_id) throw ();

        bool online(const std::string& server_id) const throw ();
        unsigned long int frame_rate(const nicta::vibe::video::video_client<Depth, Pixel>* const client, const std::string& server_id) const throw ();

        unsigned long int stream_length(const std::string& server_id) const throw (nicta::nicta_exception);
        unsigned long int stream_length(const nicta::vibe::video::video_client<Depth, Pixel>* const client) const throw ();

        nicta::vibe::video::StreamModel stream_type(const std::string& server_id) const throw (nicta::nicta_exception);


    private:

        bool online_() const throw ();
        void fps_check_() const throw (nicta::nicta_exception);


        double fps_;
        boost::posix_time::time_duration period_;

        boost::mutex server_mutex_;
        mutable boost::mutex table_mutex_;

        // map<id, cnxns>
        std::map<std::string, unsigned int> connected_servers_;
        std::map<std::string, unsigned int> connected_clients_;
        typename nicta::vibe::video::control_table<Depth, Pixel>::type control_table_;
    };

} } } // namespace nicta::vibe::video


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::video::controller<T, U> :: 
controller(nicta::quantity::frame_rate_per_second<unsigned int> fps) throw ()
: fps_(fps.operator()<double>())
, period_(nicta::util::fps_period(fps.operator()<double>()))
{
    // Constructor
}


template < typename T
         , template <typename T_> class U
         >
void
nicta::vibe::video::controller<T, U> :: 
register_server_online(const std::string& id) throw (nicta::nicta_exception)
{
    // Server registers itself that it is now streaming data, and keeping track of connections held

    nicta::thread::guard<boost::mutex, boost::unique_lock> lock(this->server_mutex_);
    if (this->connected_servers_.find(id) != this->connected_servers_.end())
    {
        throw nicta::nicta_exception("sought multiple registration of server id", "register_server_online", "controller<T, U>");
    }
    this->connected_servers_[id] += 1;
}


template < typename T
         , template <typename T_> class U
         >
void
nicta::vibe::video::controller<T, U> :: 
operator()() throw (nicta::nicta_exception)
{
    // At camera rate (or other - not yet supported), incr table_record counters and push data to client(s)

    if (!this->fps_)  // will be set by first server that initiates contact (if fps_ in ctor == 0) .. conseq all servers per ctrl.obj *must* have fps same
    {
        // get and then set this->fps_ via first server entry in table:
        nicta::vibe::video::video_server<T, U>* server = this->control_table_.begin()->template get<nicta::vibe::video::Server>();
        server->fps_waiter().template operator()< bool
                                                , nicta::vibe::video::video_server<T, U>
                                                , boost::unique_lock
                                                > (&nicta::vibe::video::video_server<T, U>::fps_available, server);
    
        this->fps_    = server->fps();
        this->period_ = nicta::util::fps_period(this->fps_);
    }
    this->fps_check_();                         // now check all servers in table are at same fps too (.. might throw!)
    nicta::util::sleep(this->period_);


    do
    {
        nicta::thread::guard<boost::mutex, boost::unique_lock> lock(this->table_mutex_);
        typename nicta::thread::write_lock::type glock =
                 nicta::thread::gate< >::template instance<nicta::vibe::video::thread::gate_model::shared>()->operator()().template operator()
                                                                                                                              < nicta::thread::Write
                                                                                                                              , boost::unique_lock
                                                                                                                              >();

        for (typename nicta::vibe::video::control_table<T, U>::type::iterator it = this->control_table_.begin(); it != this->control_table_.end(); ++it)
        {
            if (1) //!it->template get<nicta::vibe::video::FrameCounter>())
            {
                // get data from servers, push to clients

                nicta::vibe::video::video_server<T, U>* server = it->template get<nicta::vibe::video::Server>();
                nicta::vibe::video::video_client<T, U>* client = it->template get<nicta::vibe::video::Client>();

                // moved client->online test inside so that server can system stays alive regardless of client state
                if (server->online()) // && client->online())
                {
                    // this condition (below) is Ok for cameras too b/c:  FrameIndex == 0 (always)  :-)
                    if (client->online() && (server->held_frame_index() % it->template get<nicta::vibe::video::FrameRate>() == 0))
                    {
                        client->rx(server->tx(), server->id(), server->payload());
                    }
                }
                else
                {
                    this->control_table_.erase(it--);

                    nicta::thread::guard<boost::mutex, boost::unique_lock> s_lock(this->server_mutex_);
                    std::map<std::string, unsigned int>::const_iterator s_pos = this->connected_servers_.find(server->id());
                    if (s_pos != this->connected_servers_.end())
                    {
                        this->connected_servers_[server->id()] -= 1;
                        if (!this->connected_servers_[server->id()])
                        {
                            server->shutdown();
                            this->connected_servers_.erase(server->id());
                        }
                    }

                    std::map<std::string, unsigned int>::const_iterator c_pos = this->connected_clients_.find(client->id());
                    if (c_pos != this->connected_clients_.end())
                    {
                        this->connected_clients_[client->id()] -= 1;
                        if (!this->connected_clients_[client->id()])
                        {
                            this->connected_clients_.erase(client->id());
                        }
                    }

                    continue;
                }
            }
            /* policy.ctr = ++policy.ctr % policy.nth ... this serves no purpose anymore!*/
            it->template get<nicta::vibe::video::FrameCounter>() = ++it->template get<nicta::vibe::video::FrameCounter>() %
                                                                     it->template get<nicta::vibe::video::FrameRate>();
        }
        nicta::thread::gate< >::template instance<nicta::vibe::video::thread::gate_model::shared>()->operator()().template operator()
                                                                                                                     < nicta::thread::NotifyAll
                                                                                                                     , boost::unique_lock
                                                                                                                     >(glock);
        lock.unlock();

        // wait till next available frame
        nicta::util::sleep(this->period_);
    }
    while (this->online_());
}


template < typename T
         , template <typename T_> class U
         >
double
nicta::vibe::video::controller<T, U> :: 
fps() const throw ()
{
    // Return the 1/period at which we attempt to poll servers

    return this->fps_;
}


template < typename T
         , template <typename T_> class U
         >
const boost::posix_time::time_duration &
nicta::vibe::video::controller<T, U> :: 
period() const throw ()
{
    // Return the period at which we attempt to poll servers

    return this->period_;
}


template < typename T
         , template <typename T_> class U
         >
unsigned int
nicta::vibe::video::controller<T, U> :: 
server_links(const std::string& server_id) const throw ()
{
    // Return count of clients a server is connected to

    std::map<std::string, unsigned int>::const_iterator pos = this->connected_servers_.find(server_id);
    if (pos != this->connected_servers_.end())
    {
        return *pos;
    }
    return 0;
}


template < typename T
         , template <typename T_> class U
         >
unsigned int
nicta::vibe::video::controller<T, U> :: 
client_links(const std::string& client_id) const throw ()
{
    // Return count of servers a client is connected to

    std::map<std::string, unsigned int>::const_iterator it = this->connected_clients_.find(client_id);
    if (it != this->connected_clients_.end())
    {
        return it->second;
    }
    return 0;
}


template < typename T
         , template <typename T_> class U
         >
void
nicta::vibe::video::controller<T, U> :: 
unlink(const nicta::vibe::video::video_client<T, U>* const client, const std::string& server_id) throw ()
{
    // Delete table record that associates server & client

    for (typename nicta::vibe::video::control_table<T, U>::type::iterator it = this->control_table_.begin(); it != this->control_table_.end(); ++it)
    {
        if (it->template get<nicta::vibe::video::Client>() == client && it->template get<nicta::vibe::video::Server>()->id() == server_id)
        {
            nicta::thread::guard<boost::mutex, boost::unique_lock> lock(this->table_mutex_);
            this->control_table_.erase(it);

            std::map<std::string, unsigned int>::const_iterator s_pos = this->connected_servers_.find(server_id);
            if (s_pos != this->connected_servers_.end())
            {
                this->connected_servers_[server_id] -= 1;
            }
            std::map<std::string, unsigned int>::const_iterator c_pos = this->connected_clients_.find(client->id());
            if (c_pos != this->connected_clients_.end())
            {
                this->connected_clients_[client->id()] -= 1;
            }

            break;
        }
    }
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::video::controller<T, U> :: 
online(const std::string& server_id) const throw ()
{
    // Query if server is available

    for (typename nicta::vibe::video::control_table<T, U>::type::const_iterator it = this->control_table_.begin(); it != this->control_table_.end(); ++it)
    {
        if (it->template get<nicta::vibe::video::Server>()->id() == server_id)
        {
            return it->template get<nicta::vibe::video::Server>()->online();
        }
    }
    return false;
}


template < typename T
         , template <typename T_> class U
         >
unsigned long int
nicta::vibe::video::controller<T, U> :: 
frame_rate(const nicta::vibe::video::video_client<T, U>* const client, const std::string& server_id) const throw ()
{
    // Get frame_rate at which client wants data from server

    for (typename nicta::vibe::video::control_table<T, U>::type::const_iterator it = this->control_table_.begin(); it != this->control_table_.end(); ++it)
    {
        if (it->template get<nicta::vibe::video::Client>() == client && it->template get<nicta::vibe::video::Server>()->id() == server_id)
        {
            nicta::thread::guard<boost::mutex, boost::unique_lock> lock(this->table_mutex_);
            return it->template get<nicta::vibe::video::FrameRate>();
        }
    }
    return 0;
}


template < typename T
         , template <typename T_> class U
         >
unsigned long int
nicta::vibe::video::controller<T, U> :: 
stream_length(const std::string& server_id) const throw (nicta::nicta_exception)
{
    // Get total number of frames available on server stream *at* connection-time
    // NOTE:  will **always** return 1 for camera connections!  ...only makes sense for file-connections!!

    for (typename nicta::vibe::video::control_table<T, U>::type::const_iterator it = this->control_table_.begin(); it != this->control_table_.end(); ++it)
    {
        if (it->template get<nicta::vibe::video::Server>()->id() == server_id)
        {
            return it->template get<nicta::vibe::video::Server>()->stream_length();
        }
    }
    throw nicta::nicta_exception("no such server", "stream_length", "controller<T, U>");
    return 0;
}


template < typename T
         , template <typename T_> class U
         >
unsigned long int
nicta::vibe::video::controller<T, U> :: 
stream_length(const nicta::vibe::video::video_client<T, U>* const client) const throw ()
{
    // Get total number of frames available for *all* server streams associated with 'client' *at* connection-time
    // NOTE:  will **always** return 1 for camera connections!  ...only makes sense for file-connections!!

    unsigned long int count = 0;
    for (typename nicta::vibe::video::control_table<T, U>::type::const_iterator it = this->control_table_.begin(); it != this->control_table_.end(); ++it)
    {
        if (it->template get<nicta::vibe::video::Client>() == client)
        {
            count += it->template get<nicta::vibe::video::Server>()->stream_length();
        }
    }
    return count;
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::video::StreamModel
nicta::vibe::video::controller<T, U> :: 
stream_type(const std::string& server_id) const throw (nicta::nicta_exception)
{
    // Return the source-stream type:  Image(dir of img files) ; File(avi files) ; Camera(camera feed)

    for (typename nicta::vibe::video::control_table<T, U>::type::const_iterator it = this->control_table_.begin(); it != this->control_table_.end(); ++it)
    {
        if (it->template get<nicta::vibe::video::Server>()->id() == server_id)
        {
            return it->template get<nicta::vibe::video::Server>()->stream_type();
        }
    }
    throw nicta::nicta_exception("no such server", "stream_length", "controller<T, U>");
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::video::controller<T, U> :: 
online_() const throw ()
{
    // Return if true if control table contains any entries
    // NOTE:  this only has context within scope of op()(.) & will only be up-to-date *after* a call to op()(.) !!
    // .. an independent would consist of iterating through all registered servers/clients & querying them directly!

    nicta::thread::guard<boost::mutex, boost::unique_lock> lock(this->table_mutex_);
    return !!this->control_table_.size();
}


template < typename T
         , template <typename T_> class U
         >
void
nicta::vibe::video::controller<T, U> :: 
fps_check_() const throw (nicta::nicta_exception)
{
    // Used to check that all server entries in this->control_table_ run at same fps rate (and as stipulated on controller too)
    // ... since right now, hetrogeneous server frame rates for controller are not (yet) supported

    for (typename nicta::vibe::video::control_table<T, U>::type::const_iterator it = this->control_table_.begin(); it != this->control_table_.end(); ++it)
    {
        nicta::vibe::video::video_server<T, U>* server = it->template get<nicta::vibe::video::Server>();
        server->fps_waiter().template operator()< bool
                                                , nicta::vibe::video::video_server<T, U>
                                                , boost::unique_lock
                                                > (&nicta::vibe::video::video_server<T, U>::fps_available, server);
    
        if (this->fps_ != server->fps())
        {
            throw nicta::nicta_exception("no support for hetrogeneous server frame rates for controller at present", "op()()", "controller<T, U>");
        }
    }
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
