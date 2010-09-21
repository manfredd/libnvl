/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_THREAD_RW_CONDITION_WAIT_H
#define NICTA_THREAD_RW_CONDITION_WAIT_H

#include "thread/condition/notify_model.h"
#include "thread/guard/guard.h"

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/shared_mutex.hpp>


namespace nicta {
namespace thread {

    enum WaitFor
    {
        Read  =  0  ,
        Write =  1
    };


    template <typename Lockable>
    class rw_condition_wait : boost::noncopyable
    {
    public:

        template <template <typename Lockable_> class LockDevice>
        struct result_type
        {
            typedef nicta::thread::guard<Lockable, LockDevice> subtype;
            typedef boost::shared_ptr<nicta::thread::guard<Lockable, LockDevice> > type;
        };

        rw_condition_wait(nicta::thread::WaitFor init)
        : now_reading_(init == nicta::thread::Read)
        , now_writing_(init == nicta::thread::Write)
        , read_count_(0)
        , write_count_(0)
        {
            // Constructor

        }


        template < nicta::thread::WaitFor W
                 , template <typename Lockable_> class LockDevice
                 >
        typename rw_condition_wait<Lockable>::template result_type<LockDevice>::type
        operator()() throw ();


        template < nicta::thread::NotifyModel M
                 , template <typename Lockable_> class LockDevice
                 >
        bool
        operator()(typename nicta::thread::rw_condition_wait<Lockable>::template result_type<LockDevice>::type lock) throw ();


    private:

        bool reading_() const throw ();
        bool writing_() const throw ();


        bool now_reading_;
        bool now_writing_;

        Lockable mutex_;
        boost::condition_variable_any rcond_;
        boost::condition_variable_any wcond_;


        unsigned long int read_count_;
        boost::mutex read_count_mutex_;

        unsigned long int write_count_;
        boost::mutex write_count_mutex_;
    };




    template < >
    template < >
    nicta::thread::rw_condition_wait<boost::mutex>::result_type<boost::unique_lock>::type
    nicta::thread::rw_condition_wait<boost::mutex> :: 
    operator()< nicta::thread::Read
              , boost::unique_lock
              >() throw ()
    {
        // Exclusive reading

        nicta::thread::rw_condition_wait<boost::mutex>::result_type<boost::unique_lock>::type
             sp_g(new nicta::thread::rw_condition_wait<boost::mutex>::result_type<boost::unique_lock>::subtype(this->mutex_));

        this->rcond_.wait(sp_g->operator()(), !boost::bind(&nicta::thread::rw_condition_wait<boost::mutex>::writing_, this));
        this->now_reading_ = true;

        ++this->read_count_;

        return sp_g;
    }
    template < >
    template < >
    nicta::thread::rw_condition_wait<boost::shared_mutex>::result_type<boost::unique_lock>::type
    nicta::thread::rw_condition_wait<boost::shared_mutex> :: 
    operator()< nicta::thread::Read
              , boost::unique_lock
              >() throw ()
    {
        // Exclusive reading

        nicta::thread::rw_condition_wait<boost::shared_mutex>::result_type<boost::unique_lock>::type
             sp_g(new nicta::thread::rw_condition_wait<boost::shared_mutex>::result_type<boost::unique_lock>::subtype(this->mutex_));

        this->rcond_.wait(sp_g->operator()(), !boost::bind(&nicta::thread::rw_condition_wait<boost::shared_mutex>::writing_, this));
        this->now_reading_= true;

        ++this->read_count_;

        return sp_g;
    }
    template < >
    template < >
    nicta::thread::rw_condition_wait<boost::recursive_mutex>::result_type<boost::unique_lock>::type
    nicta::thread::rw_condition_wait<boost::recursive_mutex> :: 
    operator()< nicta::thread::Read
              , boost::unique_lock
              >() throw ()
    {
        // Exclusive reading

        nicta::thread::rw_condition_wait<boost::recursive_mutex>::result_type<boost::unique_lock>::type
             sp_g(new nicta::thread::rw_condition_wait<boost::recursive_mutex>::result_type<boost::unique_lock>::subtype(this->mutex_));

        this->rcond_.wait(sp_g->operator()(), !boost::bind(&nicta::thread::rw_condition_wait<boost::recursive_mutex>::writing_, this));
        this->now_reading_= true;

        ++this->read_count_;

        return sp_g;
    }

    // ----

    template < >
    template < >
    nicta::thread::rw_condition_wait<boost::shared_mutex>::result_type<boost::shared_lock>::type
    nicta::thread::rw_condition_wait<boost::shared_mutex> :: 
    operator()< nicta::thread::Read
              , boost::shared_lock
              >() throw ()
    {
        // Shared reading

        nicta::thread::rw_condition_wait<boost::shared_mutex>::result_type<boost::shared_lock>::type
             sp_g(new nicta::thread::rw_condition_wait<boost::shared_mutex>::result_type<boost::shared_lock>::subtype(this->mutex_));

        this->rcond_.wait(sp_g->operator()(), !boost::bind(&nicta::thread::rw_condition_wait<boost::shared_mutex>::writing_, this));
        this->now_reading_= true;

        nicta::thread::guard<boost::mutex, boost::unique_lock> g(this->read_count_mutex_);
        ++this->read_count_;
        g.unlock();

        return sp_g;
    }


    // ====


    template < >
    template < >
    nicta::thread::rw_condition_wait<boost::mutex>::result_type<boost::unique_lock>::type
    nicta::thread::rw_condition_wait<boost::mutex> :: 
    operator()< nicta::thread::Write
              , boost::unique_lock
              >() throw ()
    {
        // Exclusive write

        nicta::thread::rw_condition_wait<boost::mutex>::result_type<boost::unique_lock>::type
             sp_g(new nicta::thread::rw_condition_wait<boost::mutex>::result_type<boost::unique_lock>::subtype(this->mutex_));

        this->wcond_.wait(sp_g->operator()(), !boost::bind(&nicta::thread::rw_condition_wait<boost::mutex>::reading_, this));
        this->now_writing_= true;

        ++this->write_count_;

        return sp_g;
    }
    template < >
    template < >
    nicta::thread::rw_condition_wait<boost::shared_mutex>::result_type<boost::unique_lock>::type
    nicta::thread::rw_condition_wait<boost::shared_mutex> :: 
    operator()< nicta::thread::Write
              , boost::unique_lock
              >() throw ()
    {
        // Exclusive write

        nicta::thread::rw_condition_wait<boost::shared_mutex>::result_type<boost::unique_lock>::type
             sp_g(new nicta::thread::rw_condition_wait<boost::shared_mutex>::result_type<boost::unique_lock>::subtype(this->mutex_));

        this->wcond_.wait(sp_g->operator()(), !boost::bind(&nicta::thread::rw_condition_wait<boost::shared_mutex>::reading_, this));
        this->now_writing_= true;

        ++this->write_count_;

        return sp_g;
    }
    template < >
    template < >
    nicta::thread::rw_condition_wait<boost::recursive_mutex>::result_type<boost::unique_lock>::type
    nicta::thread::rw_condition_wait<boost::recursive_mutex> :: 
    operator()< nicta::thread::Write
              , boost::unique_lock
              >() throw ()
    {
        // Exclusive write

        nicta::thread::rw_condition_wait<boost::recursive_mutex>::result_type<boost::unique_lock>::type
             sp_g(new nicta::thread::rw_condition_wait<boost::recursive_mutex>::result_type<boost::unique_lock>::subtype(this->mutex_));

        this->wcond_.wait(sp_g->operator()(), !boost::bind(&nicta::thread::rw_condition_wait<boost::recursive_mutex>::reading_, this));
        this->now_writing_= true;

        ++this->write_count_;

        return sp_g;
    }

    // ----

    template < >
    template < >
    nicta::thread::rw_condition_wait<boost::shared_mutex>::result_type<boost::shared_lock>::type
    nicta::thread::rw_condition_wait<boost::shared_mutex> :: 
    operator()< nicta::thread::Write
              , boost::shared_lock
              >() throw ()
    {
        // Dangerous (USE WITH CARE)!  Shared write - allow more than one writer for (potentially) same resource
        // ... though will usu. use when writing to different buffers, AND wanting to signal when *all* buffers have been written to (as same cvar is used)

        nicta::thread::rw_condition_wait<boost::shared_mutex>::result_type<boost::shared_lock>::type
             sp_g(new nicta::thread::rw_condition_wait<boost::shared_mutex>::result_type<boost::shared_lock>::subtype(this->mutex_));

        this->wcond_.wait(sp_g->operator()(), !boost::bind(&nicta::thread::rw_condition_wait<boost::shared_mutex>::reading_, this));
        this->now_writing_= true;

        nicta::thread::guard<boost::mutex, boost::unique_lock> g(this->write_count_mutex_);
        ++this->write_count_;
        g.unlock();

        return sp_g;
    }


    // ****


    template < >
    template < >
    bool
    nicta::thread::rw_condition_wait<boost::mutex> :: 
    operator()< nicta::thread::NotifyOne
              , boost::unique_lock
              >(nicta::thread::rw_condition_wait<boost::mutex>::result_type<boost::unique_lock>::type lock) throw ()
    {
        // Release & Notify

        if (this->now_reading_)
        {
            nicta::thread::guard<boost::mutex, boost::unique_lock> g(this->read_count_mutex_);
            --this->read_count_;
            g.unlock();

            if (!this->read_count_)
            {
                this->now_reading_ = false;
                lock->unlock();
                this->wcond_.notify_one();

                return true;
            }
            else
            {
                lock->unlock();
            }
        }
        else
        {
            nicta::thread::guard<boost::mutex, boost::unique_lock> g(this->write_count_mutex_);
            --this->write_count_;
            g.unlock();

            if (!this->write_count_)
            {
                this->now_writing_ = false;
                lock->unlock();
                this->rcond_.notify_one();

                return true;
            }
            else
            {
                lock->unlock();
            }
        }
        return false;
    }
    template < >
    template < >
    bool
    nicta::thread::rw_condition_wait<boost::shared_mutex> :: 
    operator()< nicta::thread::NotifyOne
              , boost::unique_lock
              >(nicta::thread::rw_condition_wait<boost::shared_mutex>::result_type<boost::unique_lock>::type lock) throw ()
    {
        // Release & Notify

        if (this->now_reading_)
        {
            nicta::thread::guard<boost::mutex, boost::unique_lock> g(this->read_count_mutex_);
            --this->read_count_;
            g.unlock();

            if (!this->read_count_)
            {
                this->now_reading_ = false;
                lock->unlock();
                this->wcond_.notify_one();

                return true;
            }
            else
            {
                lock->unlock();
            }
        }
        else
        {
            nicta::thread::guard<boost::mutex, boost::unique_lock> g(this->write_count_mutex_);
            --this->write_count_;
            g.unlock();

            if (!this->write_count_)
            {
                this->now_writing_ = false;
                lock->unlock();
                this->rcond_.notify_one();

                return true;
            }
            else
            {
                lock->unlock();
            }
        }
        return false;
    }
    template < >
    template < >
    bool
    nicta::thread::rw_condition_wait<boost::recursive_mutex> :: 
    operator()< nicta::thread::NotifyOne
              , boost::unique_lock
              >(nicta::thread::rw_condition_wait<boost::recursive_mutex>::result_type<boost::unique_lock>::type lock) throw ()
    {
        // Release & Notify

        if (this->now_reading_)
        {
            nicta::thread::guard<boost::mutex, boost::unique_lock> g(this->read_count_mutex_);
            --this->read_count_;
            g.unlock();

            if (!this->read_count_)
            {
                this->now_reading_ = false;
                lock->unlock();
                this->wcond_.notify_one();

                return true;
            }
            else
            {
                lock->unlock();
            }
        }
        else
        {
            nicta::thread::guard<boost::mutex, boost::unique_lock> g(this->write_count_mutex_);
            --this->write_count_;
            g.unlock();

            if (!this->write_count_)
            {
                this->now_writing_ = false;
                lock->unlock();
                this->rcond_.notify_one();

                return true;
            }
            else
            {
                lock->unlock();
            }
        }
        return false;
    }

    // ----

    template < >
    template < >
    bool
    nicta::thread::rw_condition_wait<boost::shared_mutex> :: 
    operator()< nicta::thread::NotifyOne
              , boost::shared_lock
              >(nicta::thread::rw_condition_wait<boost::shared_mutex>::result_type<boost::shared_lock>::type lock) throw ()
    {
        // Release & Notify

        if (this->now_reading_)
        {
            nicta::thread::guard<boost::mutex, boost::unique_lock> g(this->read_count_mutex_);
            --this->read_count_;
            g.unlock();

            if (!this->read_count_)
            {
                this->now_reading_ = false;
                lock->unlock();
                this->wcond_.notify_one();

                return true;
            }
            else
            {
                lock->unlock();
            }
        }
        else
        {
            nicta::thread::guard<boost::mutex, boost::unique_lock> g(this->write_count_mutex_);
            --this->write_count_;
            g.unlock();

            if (!this->write_count_)
            {
                this->now_writing_ = false;
                lock->unlock();
                this->rcond_.notify_one();

                return true;
            }
            else
            {
                lock->unlock();
            }
        }
        return false;
    }

    // ====

    template < >
    template < >
    bool
    nicta::thread::rw_condition_wait<boost::mutex> :: 
    operator()< nicta::thread::NotifyAll
              , boost::unique_lock
              >(nicta::thread::rw_condition_wait<boost::mutex>::result_type<boost::unique_lock>::type lock) throw ()
    {
        // Release & Notify

        if (this->now_reading_)
        {
            nicta::thread::guard<boost::mutex, boost::unique_lock> g(this->read_count_mutex_);
            --this->read_count_;
            g.unlock();

            if (!this->read_count_)
            {
                this->now_reading_ = false;
                lock->unlock();
                this->wcond_.notify_all();

                return true;
            }
            else
            {
                lock->unlock();
            }
        }
        else
        {
            nicta::thread::guard<boost::mutex, boost::unique_lock> g(this->write_count_mutex_);
            --this->write_count_;
            g.unlock();

            if (!this->write_count_)
            {
                this->now_writing_ = false;
                lock->unlock();
                this->rcond_.notify_all();

                return true;
            }
            else
            {
                lock->unlock();
            }
        }
        return false;
    }
    template < >
    template < >
    bool
    nicta::thread::rw_condition_wait<boost::shared_mutex> :: 
    operator()< nicta::thread::NotifyAll
              , boost::unique_lock
              >(nicta::thread::rw_condition_wait<boost::shared_mutex>::result_type<boost::unique_lock>::type lock) throw ()
    {
        // Release & Notify

        if (this->now_reading_)
        {
            nicta::thread::guard<boost::mutex, boost::unique_lock> g(this->read_count_mutex_);
            --this->read_count_;
            g.unlock();

            if (!this->read_count_)
            {
                this->now_reading_ = false;
                lock->unlock();
                this->wcond_.notify_all();

                return true;
            }
            else
            {
                lock->unlock();
            }
        }
        else
        {
            nicta::thread::guard<boost::mutex, boost::unique_lock> g(this->write_count_mutex_);
            --this->write_count_;
            g.unlock();

            if (!this->write_count_)
            {
                this->now_writing_ = false;
                lock->unlock();
                this->rcond_.notify_all();

                return true;
            }
            else
            {
                lock->unlock();
            }
        }
        return false;
    }
    template < >
    template < >
    bool
    nicta::thread::rw_condition_wait<boost::recursive_mutex> :: 
    operator()< nicta::thread::NotifyAll
              , boost::unique_lock
              >(nicta::thread::rw_condition_wait<boost::recursive_mutex>::result_type<boost::unique_lock>::type lock) throw ()
    {
        // Release & Notify

        if (this->now_reading_)
        {
            nicta::thread::guard<boost::mutex, boost::unique_lock> g(this->read_count_mutex_);
            --this->read_count_;
            g.unlock();

            if (!this->read_count_)
            {
                this->now_reading_ = false;
                lock->unlock();
                this->wcond_.notify_all();

                return true;
            }
            else
            {
                lock->unlock();
            }
        }
        else
        {
            nicta::thread::guard<boost::mutex, boost::unique_lock> g(this->write_count_mutex_);
            --this->write_count_;
            g.unlock();

            if (!this->write_count_)
            {
                this->now_writing_ = false;
                lock->unlock();
                this->rcond_.notify_all();

                return true;
            }
            else
            {
                lock->unlock();
            }
        }
        return false;
    }

    // ----

    template < >
    template < >
    bool
    nicta::thread::rw_condition_wait<boost::shared_mutex> :: 
    operator()< nicta::thread::NotifyAll
              , boost::shared_lock
              >(nicta::thread::rw_condition_wait<boost::shared_mutex>::result_type<boost::shared_lock>::type lock) throw ()
    {
        // Release & Notify

        if (this->now_reading_)
        {
            nicta::thread::guard<boost::mutex, boost::unique_lock> g(this->read_count_mutex_);
            --this->read_count_;
            g.unlock();

            if (!this->read_count_)
            {
                this->now_reading_ = false;
                lock->unlock();
                this->wcond_.notify_all();

                return true;
            }
            else
            {
                lock->unlock();
            }
        }
        else
        {
            nicta::thread::guard<boost::mutex, boost::unique_lock> g(this->write_count_mutex_);
            --this->write_count_;
            g.unlock();

            if (!this->write_count_)
            {
                this->now_writing_ = false;
                lock->unlock();
                this->rcond_.notify_all();

                return true;
            }
            else
            {
                lock->unlock();
            }
        }
        return false;
    }

} } // namespace nicta::thread



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template <typename Lockable>
bool
nicta::thread::rw_condition_wait<Lockable> :: 
reading_() const throw ()
{
    // Return state variable

    return this->now_reading_;
}


template <typename Lockable>
bool
nicta::thread::rw_condition_wait<Lockable> :: 
writing_() const throw ()
{
    // Return state variable

    return this->now_writing_;
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
