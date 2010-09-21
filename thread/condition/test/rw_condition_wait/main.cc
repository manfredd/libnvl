/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "thread/condition/rw_condition_wait.h"
#include "thread/guard/guard.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/ref.hpp>

#include <iostream>



unsigned long int data_ = 0;


template <typename Lockable = boost::shared_mutex>
class Server
{
public:

    typedef void result_type;

    Server()
    : rw_cw_(nicta::thread::Write)    // lock it for write initialization (ie.  server to start first, client will wait on the first notify)
    {
    }


    void operator()()
    {
        unsigned int i = 0;
        while (i < 10)
        {
            typename nicta::thread::rw_condition_wait<Lockable>::template result_type<boost::unique_lock>::type lock =
                 this->rw_cw_.template operator()<nicta::thread::Write, boost::unique_lock>();

            ++::data_;
            ++i;

            this->rw_cw_.template operator()<nicta::thread::NotifyAll, boost::unique_lock>(lock);
            ::sleep(1);
        }
    }


    nicta::thread::rw_condition_wait<Lockable>& waiter() throw ()
    {
        return this->rw_cw_;
    }


private:

    nicta::thread::rw_condition_wait<Lockable> rw_cw_;
};


template <typename Lockable = boost::shared_mutex>
class Client
{
public:

    typedef void result_type;

    Client(Server<Lockable>* vs)
    : vs_(vs)
    { }

    void operator()()
    {
        unsigned int i = 0;
        while (i < 10)
        {
            typename nicta::thread::rw_condition_wait<Lockable>::template result_type<boost::shared_lock>::type lock =
                 this->vs_->waiter().template operator()<nicta::thread::Read, boost::shared_lock>();

            std::cout << std::endl << "RW: " << ::data_;
            ++i;

            this->vs_->waiter().template operator()<nicta::thread::NotifyOne, boost::shared_lock>(lock);
            ::sleep(1);
        }
    }


private:

    Server<Lockable>* vs_;
};





int main(int argc, char** argv)
{
    Server<boost::shared_mutex> vs;
    Client<boost::shared_mutex> c(&vs);

    boost::thread t0(boost::ref(vs));
    boost::thread t1(boost::ref(c));

    t1.join();

    std::cout << std::endl << "done." << std::endl;

    return 0;
}


