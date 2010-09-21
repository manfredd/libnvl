/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_THREAD_GATE_H
#define NICTA_THREAD_GATE_H


#include "thread/condition/notify_model.h"             // <--- bring this in as a convenience for files that #incl this rw_gate.h
#include "thread/condition/rw_condition_wait.h"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <vector>

namespace nicta {
namespace thread {

    template < unsigned int I = 1
             , typename Lockable = boost::shared_mutex
             >
    class gate : boost::noncopyable
    {
    public:

        template < unsigned int Instance
                 , nicta::thread::WaitFor Init = nicta::thread::Read
                 >
        static boost::shared_ptr<nicta::thread::gate<I, Lockable> > instance() throw ();

        nicta::thread::rw_condition_wait<Lockable>& operator()() throw ();


    protected:

        gate(nicta::thread::WaitFor init) throw ();


    private:

        // no static storage, so will have a unique rw_gate_ for *each* gate instance_[i = 1->I]
        nicta::thread::rw_condition_wait<Lockable> rw_gate_;
        static std::vector<boost::shared_ptr<nicta::thread::gate<I, Lockable> > > instance_;



        BOOST_STATIC_ASSERT (I > 0);
    };

} }  // namespace nicta::thread


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


/* initialize static member */
template < unsigned int I
         , typename Lockable
         >
std::vector<boost::shared_ptr<nicta::thread::gate<I, Lockable> > >
nicta::thread::gate< I
                   , Lockable
                   > :: 
instance_(I, boost::shared_ptr<nicta::thread::gate<I, Lockable> > (static_cast<nicta::thread::gate<I, Lockable> *> (0)));



template < unsigned int I
         , typename Lockable
         >
nicta::thread::gate< I
                   , Lockable
                   > :: 
gate(nicta::thread::WaitFor init) throw ()
: rw_gate_(init)
{
    // Constructor
}


template < unsigned int I
         , typename Lockable
         >
template < unsigned int Instance
         , nicta::thread::WaitFor Init
         >
boost::shared_ptr<nicta::thread::gate<I, Lockable> >
nicta::thread::gate< I
                   , Lockable
                   > :: 
instance() throw ()
{
    // Initialize and return the singleton

    BOOST_STATIC_ASSERT (Instance < I);

    if (nicta::thread::gate<I, Lockable>::instance_[Instance] ==
        boost::shared_ptr<nicta::thread::gate<I, Lockable> > (static_cast<nicta::thread::gate<I, Lockable> *> (0)))
    {
        nicta::thread::gate<I, Lockable>::instance_[Instance] =
        boost::shared_ptr<nicta::thread::gate<I, Lockable> > (new nicta::thread::gate<I, Lockable>(Init));
    }
    return nicta::thread::gate<I, Lockable>::instance_[Instance];
}


template < unsigned int I
         , typename Lockable
         >
nicta::thread::rw_condition_wait<Lockable> &
nicta::thread::gate< I
                   , Lockable
                   > :: 
operator()() throw ()
{
    // Return the gate object

    return this->rw_gate_;
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
