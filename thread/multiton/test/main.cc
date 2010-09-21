/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "thread/multiton/multiton.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/ref.hpp>

#include <iostream>


template <unsigned int I>
class M
{
public:

    M() { }

    typedef void result_type;


    int foo(int i) const
    {
        std::cout << std::endl << "int foo(" << i << ")" << std::endl;
        return 0;
    }

    result_type operator()(int i) const
    {
        for (int j = 0; j != 1000; ++j)
        {
            // placement-new example..

            double*  pool = new double[100 * 200];
            double** z    = new double* [100];
            for (unsigned int k = 0; k != 100; ++k)
            {
                z[k] = nicta::thread::multiton<I, boost::shared_mutex>::template instance<0>()->template operator()< double
                                                                                                                   , double *
                                                                                                                   , unsigned int
                                                                                                                   , size_t
                                                                                                                   , nicta::thread::array< >
                                                                                                                   , boost::unique_lock
                                                                                                                   >(pool, k, 100);
            }
            nicta::thread::multiton<I, boost::shared_mutex>::template instance<0>()->template operator() < double *
                                                                                                         , nicta::thread::array< >
                                                                                                         , boost::unique_lock
                                                                                                         >(z);
            nicta::thread::multiton<I, boost::shared_mutex>::template instance<0>()->template operator() < double
                                                                                                         , nicta::thread::array< >
                                                                                                         , boost::unique_lock
                                                                                                         >(pool);


            // case i = {0, 1} are synchronized and will never clash (they share the same mutex)
            // I/O with i = 2 *may* clash with i = {0, 1} (they don't share same mutex), so I/O may be interrupted (broken lines *may* result)

            if (i == 0)
            {
                // read-write lock:  [Write Locking]
                nicta::thread::multiton<I, boost::shared_mutex>::template instance<0>()->template operator()< int
                                                                                                            , M<I>
                                                                                                            , int
                                                                                                            , boost::unique_lock
                                                                                                            >(&M<I>::foo, this, 0);
            }
            else if (i == 1)
            {
                // read-write lock:  [Read Locking]
                nicta::thread::multiton<I, boost::shared_mutex>::template instance<0>()->template operator()< int
                                                                                                            , M<I>
                                                                                                            , int
                                                                                                            , boost::shared_lock
                                                                                                            >(&M<I>::foo, this, 1);
            }
            else
            {
                // [Exclusive Locking]
                nicta::thread::multiton<I, boost::mutex>::template instance<1>()->template operator()< int
                                                                                                     , M<I>
                                                                                                     , int
                                                                                                     , boost::unique_lock
                                                                                                     >(&M<I>::foo, this, 2);
            }
        }
    }
};


int main(int argc, char** argv)
{
    const M<2> m;
    boost::thread t0(boost::cref(m), 0);
    boost::thread t1(boost::cref(m), 1);
    boost::thread t2(boost::cref(m), 2);

    t0.join();
    t1.join();
    t2.join();

    return 0;
}


