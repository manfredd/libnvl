/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_MACROS_CONTROLLER_H
#define NICTA_VIBE_VIDEO_MACROS_CONTROLLER_H


#include "video/policy.h"
#include "video/control_table.h"

#include <nicta_common/nicta_exception.h>

#include <string>



#define INSERT_CTRL_TABLE_RECORD(z, n, server)                                                                                                         \
                                                                                                                                                       \
            if (this->connected_clients_.find(server ## n->id()) != this->connected_clients_.end())                                                    \
            {                                                                                                                                          \
                throw nicta::nicta_exception("sought mulitiple registration of server to client", "register_client_online", "controller<T, U>");       \
            }                                                                                                                                          \
                                                                                                                                                       \
            this->control_table_.push_back(typename nicta::vibe::video::table_record<Depth, Pixel>::type (server ## n,                                 \
                                                                                                   client,                                             \
                                                                                                   (nth.operator()< >() ? nth.operator()< >() : 1UL),  \
                                                                                                   0));                                                \
                                                                                                                                                       \
            this->connected_clients_[client->id()] += 1;
/**/


#define INSERT_CTRL_TABLE_RECORD_(z, n, policy)                                                                                                        \
                                                                                                                                                       \
            if (this->connected_clients_.find(policy ## n.server()->id()) != this->connected_clients_.end())                                           \
            {                                                                                                                                          \
                throw nicta::nicta_exception("sought mulitiple registration of server to client", "register_client_online", "controller<T, U>");       \
            }                                                                                                                                          \
                                                                                                                                                       \
            this->control_table_.push_back(typename nicta::vibe::video::table_record<Depth, Pixel>::type (policy ## n.server(),                        \
                                                                                                   client,                                             \
                                                                                                   (policy ## n.nth() ? policy ## n.nth() : 1UL),      \
                                                                                                   0));                                                \
                                                                                                                                                       \
            this->connected_clients_[client->id()] += 1;
/**/
                                                  


#endif
