/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "thread/condition/condition_wait.h"

#include <boost/thread/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/ref.hpp>

#include <iostream>


template <typename Lockable = boost::shared_mutex>
class Server
{
public:

    typedef void result_type;

    void operator()()
    {
        ::sleep(10);

        this->cw_(true);                                   // sets predicate to true
        this->cw_.template operator()<nicta::thread::NotifyAll>();        // notifies all waiting cvars; cvars will check once signaled if predicate is true

        this->cw_(false);                         // predicate set to false - cvars won't have time to see that it was true before.. so they will wait again
		::sleep(3);

        this->cw_.template operator()<nicta::thread::NotifyAll>(true);    // sets predicate to true & notifies all waiting cvars (in one operation)
    }

    nicta::thread::condition_wait<Lockable>& waiter() throw ()
    {
        return this->cw_;
    }

    bool have_data() const
    {
        //return this->have_data_;
        return this->cw_();
    }


private:

    nicta::thread::condition_wait<Lockable> cw_;
};


template <typename Lockable = boost::shared_mutex>
class Client
{
public:

    typedef void result_type;

    Client(Server<Lockable>* vs)
    : vs_(vs)
    { }

    void operator()(const char* s)
    {
        int i = 0;
        vs_->waiter().template operator()<bool, Server<Lockable>, boost::shared_lock>(&Server<Lockable>::have_data, this->vs_);

        while (i < 20)
        {
            ::sleep(1);
            std::cout << std::endl << "[" << s << "]" << i;
            ++i;
        }
    }


private:

    Server<Lockable>* vs_;
};





int main(int argc, char** argv)
{
    Server<boost::shared_mutex> vs;
    Client<boost::shared_mutex> c1(&vs);
    Client<boost::shared_mutex> c2(&vs);

    boost::thread t0(boost::ref(vs));

    // Example of mutiple threads waiting (blocking wait) on a condition to return true
    boost::thread t1(boost::ref(c1), "thread_1");
    boost::thread t2(boost::ref(c2), "thread_2");

    t1.join();
    t2.join();

    std::cout << std::endl << "done." << std::endl;

    return 0;
}


