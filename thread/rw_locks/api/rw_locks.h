/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_THREAD_RW_LOCKS_H
#define NICTA_THREAD_RW_LOCKS_H


#include "thread/condition/rw_condition_wait.h"

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>

namespace nicta {
namespace thread {

    struct read_lock
    {
        typedef nicta::thread::rw_condition_wait<boost::shared_mutex>::result_type<boost::shared_lock>::type type;
    };

    struct write_lock
    {
        typedef nicta::thread::rw_condition_wait<boost::shared_mutex>::result_type<boost::unique_lock>::type type;
    };

} }  // namespace nicta::thread




#endif
