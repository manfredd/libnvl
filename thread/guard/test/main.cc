/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "thread/guard/guard.h"

#include <boost/thread/locks.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/ref.hpp>

#include <iostream>



unsigned int data_ = 0;
boost::shared_mutex mutex_;


class W
{
public:

    typedef void result_type;


    void operator()()
    {
        long unsigned int i = 0;
        while (i < 10)
        {
            ::sleep(1);
            nicta::thread::guard<boost::shared_mutex, boost::unique_lock> lock(::mutex_);
            ++::data_;
            ++i;
        }
    }
};


class R
{
public:

    typedef void result_type;


    void operator()()
    {
        int i = 0;
        while (i < 10)
        {
            ::sleep(1);
            nicta::thread::guard<boost::shared_mutex, boost::shared_lock> lock(::mutex_);
            std::cout << std::endl << ::data_ << std::endl;
            ++i;
        }
    }
};




int main(int argc, char** argv)
{
    R r;
    W w;

    boost::thread t1(boost::ref(r));
    boost::thread t2(boost::ref(w));

    t1.join();
    t2.join();

    std::cout << std::endl << "done." << std::endl;

    return 0;
}


