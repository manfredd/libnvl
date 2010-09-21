/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_MACROS_VIDEO_CLIENT_H
#define NICTA_VIBE_VIDEO_MACROS_VIDEO_CLIENT_H


#include "video/policy.h"
#include "video/video_client.h"



#define INIT_BUFFER(z, n, server) this->buffer_[server ## n->id()];
/**/

#define INIT_BUFFER_(z, n, policy) this->buffer_[policy ## n.id()];
/**/



#define INIT_INIT_MAP(z, n, server) this->init_[false].insert(server ## n->id());
/**/

#define INIT_INIT_MAP_(z, n, policy) this->init_[false].insert(policy ## n.id());
/**/



#define INIT_EXPIRY_MAP(z, n, server) this->expired_[server ## n->id()] = false;
/**/

#define INIT_EXPIRY_MAP_(z, n, policy) this->expired_[policy ## n.id()] = false;
/**/



#define INIT_STARTTIME_MAP(z, n, server) this->start_time_matched_[server ## n->id()] = false;
/**/

#define INIT_STARTTIME_MAP_(z, n, policy) this->start_time_matched_[policy ## n.id()] = false;
/**/



#endif
