/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "thread/gate/gate.h"

#include "thread/condition/notify_model.h"
#include "thread/condition/rw_condition_wait.h"
#include "thread/guard/guard.h"
#include "thread/rw_locks/rw_locks.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/ref.hpp>

#include <iostream>



unsigned long int data_ = 0;

// total of only 1 gate, *all* with shared_mutex..
boost::shared_ptr<nicta::thread::gate<1, boost::shared_mutex> > G_0;


template <template <typename Lockable_> class LockDevice = boost::unique_lock>
struct Server
{
    typedef void result_type;


    Server()
    {
    }

    void operator()()
    {
        unsigned int i = 0;
        while (i < 10)
        {
            // gate is Write-initialized, so client will wait until Server writes first
            typename nicta::thread::write_lock::type glock = ::G_0->operator()().template operator()<nicta::thread::Write, LockDevice>();

            ++::data_;
            ++i;

            ::G_0->operator()().template operator()<nicta::thread::NotifyAll, LockDevice>(glock);
            ::sleep(1);
        }
    }
};


template <template <typename Lockable_> class LockDevice = boost::shared_lock>
struct Client
{
    typedef void result_type;


    void operator()()
    {
        unsigned int i = 0;
        while (i < 10)
        {
            // gate is Write-initialized, so client will wait until Server writes first
            typename nicta::thread::read_lock::type glock = ::G_0->operator()().template operator()<nicta::thread::Read, LockDevice>();

            std::cout << std::endl << "RW: " << ::data_;
            ++i;

            ::G_0->operator()().template operator()<nicta::thread::NotifyOne, LockDevice>(glock);
            ::sleep(1);
        }
    }
};




int main(int argc, char** argv)
{
    // we access the first gate (0-indexed), of N-gates (N = 1) - with 0th gate Write initialized..
    ::G_0 = nicta::thread::gate<1, boost::shared_mutex>::instance<0, nicta::thread::Write>();

    Server< > vs;
    Client< > cl;

    boost::thread t0(boost::ref(vs));
    boost::thread t1(boost::ref(cl));

    t1.join();

    std::cout << std::endl << "done." << std::endl;

    return 0;
}


