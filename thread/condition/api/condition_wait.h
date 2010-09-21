/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_THREAD_CONDITION_WAIT_H
#define NICTA_THREAD_CONDITION_WAIT_H

#include "thread/condition/notify_model.h"
#include "thread/guard/guard.h"

#include <nicta_common/detail/max_arity.h>
#include <nicta_common/macros/conditional_comma.h>

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/ref.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/shared_mutex.hpp>


namespace nicta {
namespace thread {

    template <typename Lockable>
    class condition_wait : boost::noncopyable
    {
    public:

        condition_wait(bool state = false);

        template <nicta::thread::NotifyModel M>
        void operator()() throw ();

        // getters & setters of state_ variable
        bool operator()() const throw ();
        void operator()(bool state) throw ();

        // sets a state and notifies
        template <nicta::thread::NotifyModel M>
        void operator()(bool state) throw ();


#define OPERATOR(z, n, _)                                                                                                                         \
                                                                                                                                                  \
        template < typename R                                                                                                                     \
                 , typename C                                                                                                                     \
                 NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                               \
                 , template<typename Lockable_> class LockDevice                                                                                  \
                 >                                                                                                                                \
        void operator()(R (C::*mem_fun_ref)(BOOST_PP_ENUM_PARAMS(n, T)),                                                                          \
                        C& c                                                                                                                      \
                        NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a)));                                                     \
                                                                                                                                                  \
                                                                                                                                                  \
        template < typename R                                                                                                                     \
                 , typename C                                                                                                                     \
                 NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                               \
                 , template<typename Lockable_> class LockDevice                                                                                  \
                 >                                                                                                                                \
        void operator()(R (C::*mem_fun)(BOOST_PP_ENUM_PARAMS(n, T)),                                                                              \
                        C* c                                                                                                                      \
                        NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a)));                                                     \
                                                                                                                                                  \
                                                                                                                                                  \
        template < typename R                                                                                                                     \
                 , typename C                                                                                                                     \
                 NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                               \
                 , template<typename Lockable_> class LockDevice                                                                                  \
                 >                                                                                                                                \
        void operator()(R (C::*const_mem_fun_ref)(BOOST_PP_ENUM_PARAMS(n, T)) const,                                                              \
                        const C& c                                                                                                                \
                        NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a))) const;                                               \
                                                                                                                                                  \
                                                                                                                                                  \
        template < typename R                                                                                                                     \
                 , typename C                                                                                                                     \
                 NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                               \
                 , template<typename Lockable_> class LockDevice                                                                                  \
                 >                                                                                                                                \
        void operator()(R (C::*const_mem_fun)(BOOST_PP_ENUM_PARAMS(n, T)) const,                                                                  \
                        const C* c                                                                                                                \
                        NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a))) const;                                               \
                                                                                                                                                  \
                                                                                                                                                  \
        template < typename R                                                                                                                     \
                 NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                               \
                 , template<typename Lockable_> class LockDevice                                                                                  \
                 >                                                                                                                                \
        void operator()(R (*ptr_fun)(BOOST_PP_ENUM_PARAMS(n, T))                                                                                  \
                        NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a)));
        /**/

#define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
#define BOOST_PP_LOCAL_LIMITS (0, NICTA_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
#undef OPERATOR


    private:

        bool state_;

        mutable Lockable mutex_;
        mutable boost::condition_variable_any condition_;

        //mutable boost::mutex state_mutex_;
        mutable boost::shared_mutex state_mutex_;
    };



    template < >
    template < >
    void
    condition_wait<boost::mutex> :: 
    operator()<nicta::thread::NotifyOne>() throw ()
    {
        // Notify just one waiter

        this->condition_.notify_one();
    }
    template < >
    template < >
    void
    condition_wait<boost::shared_mutex> :: 
    operator()<nicta::thread::NotifyOne>() throw ()
    {
        // Notify just one waiter

        this->condition_.notify_one();
    }
    template < >
    template < >
    void
    condition_wait<boost::recursive_mutex> :: 
    operator()<nicta::thread::NotifyOne>() throw ()
    {
        // Notify just one waiter

        this->condition_.notify_one();
    }

    template < >
    template < >
    void
    condition_wait<boost::mutex> :: 
    operator()<nicta::thread::NotifyAll>() throw ()
    {
        // Notify all waiters

        this->condition_.notify_all();
    }
    template < >
    template < >
    void
    condition_wait<boost::shared_mutex> :: 
    operator()<nicta::thread::NotifyAll>() throw ()
    {
        // Notify all waiters

        this->condition_.notify_all();
    }
    template < >
    template < >
    void
    condition_wait<boost::recursive_mutex> :: 
    operator()<nicta::thread::NotifyAll>() throw ()
    {
        // Notify all waiters

        this->condition_.notify_all();
    }

} } // namespace nicta::thread



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template <typename Lockable>
nicta::thread::condition_wait<Lockable> :: 
condition_wait(bool state)
: state_(state)
{
    // Constructor

}


template <typename Lockable>
bool
nicta::thread::condition_wait<Lockable> :: 
operator()() const throw ()
{
    // Return state variable

    nicta::thread::guard<boost::shared_mutex, boost::shared_lock> g(this->state_mutex_);
    return this->state_;
}


template <typename Lockable>
void
nicta::thread::condition_wait<Lockable> :: 
operator()(bool state) throw ()
{
    // Set state variable

    nicta::thread::guard<boost::shared_mutex, boost::unique_lock> g(this->state_mutex_);
    this->state_ = state;
}


template <typename Lockable>
template <nicta::thread::NotifyModel M>
void
nicta::thread::condition_wait<Lockable> :: 
operator()(bool state) throw ()
{
    // Set state variable

    nicta::thread::guard<boost::shared_mutex, boost::unique_lock> g(this->state_mutex_);
    this->state_ = state;
    this->operator()<M>();
}



#define OPERATOR(z, n, _)                                                                                                                         \
                                                                                                                                                  \
template <typename Lockable>                                                                                                                      \
template < typename R                                                                                                                             \
         , typename C                                                                                                                             \
         NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                                       \
         , template<typename Lockable_> class LockDevice                                                                                          \
         >                                                                                                                                        \
void                                                                                                                                              \
nicta::thread::condition_wait<Lockable> ::                                                                                                        \
operator()(R (C::*mem_fun_ref)(BOOST_PP_ENUM_PARAMS(n, T)),                                                                                       \
           C& c                                                                                                                                   \
           NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a)))                                                                   \
{                                                                                                                                                 \
    /* Member function call through class instance:  R C::mem_fun_ref(args) */                                                                    \
                                                                                                                                                  \
    nicta::thread::guard<Lockable, LockDevice> g(this->mutex_);                                                                                   \
    this->condition_.wait(g(), boost::bind(mem_fun_ref, boost::ref(c) NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a))));                \
}                                                                                                                                                 \
                                                                                                                                                  \
                                                                                                                                                  \
template <typename Lockable>                                                                                                                      \
template < typename R                                                                                                                             \
         , typename C                                                                                                                             \
         NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                                       \
         , template<typename Lockable_> class LockDevice                                                                                          \
         >                                                                                                                                        \
void                                                                                                                                              \
nicta::thread::condition_wait<Lockable> ::                                                                                                        \
operator()(R (C::*mem_fun)(BOOST_PP_ENUM_PARAMS(n, T)),                                                                                           \
           C* c                                                                                                                                   \
           NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a)))                                                                   \
{                                                                                                                                                 \
    /* Member function call through ptr to class instance:  R (*C::mem_fun)(args) */                                                              \
                                                                                                                                                  \
    nicta::thread::guard<Lockable, LockDevice> g(this->mutex_);                                                                                   \
    this->condition_.wait(g(), boost::bind(mem_fun, boost::ref(c) NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a))));                    \
}                                                                                                                                                 \
                                                                                                                                                  \
                                                                                                                                                  \
template <typename Lockable>                                                                                                                      \
template < typename R                                                                                                                             \
         , typename C                                                                                                                             \
         NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                                       \
         , template<typename Lockable_> class LockDevice                                                                                          \
         >                                                                                                                                        \
void                                                                                                                                              \
nicta::thread::condition_wait<Lockable> ::                                                                                                        \
operator()(R (C::*const_mem_fun_ref)(BOOST_PP_ENUM_PARAMS(n, T)) const,                                                                           \
           const C& c                                                                                                                             \
           NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a))) const                                                             \
{                                                                                                                                                 \
    /* Const member function call through class instance:  R C::mem_fun_ref(args) const */                                                        \
                                                                                                                                                  \
    nicta::thread::guard<Lockable, LockDevice> g(this->mutex_);                                                                                   \
    this->condition_.wait(g(), boost::bind(const_mem_fun_ref, boost::cref(c) NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a))));         \
}                                                                                                                                                 \
                                                                                                                                                  \
                                                                                                                                                  \
template <typename Lockable>                                                                                                                      \
template < typename R                                                                                                                             \
         , typename C                                                                                                                             \
         NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                                       \
         , template<typename Lockable_> class LockDevice                                                                                          \
         >                                                                                                                                        \
void                                                                                                                                              \
nicta::thread::condition_wait<Lockable> ::                                                                                                        \
operator()(R (C::*const_mem_fun)(BOOST_PP_ENUM_PARAMS(n, T)) const,                                                                               \
           const C* c                                                                                                                             \
           NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a))) const                                                             \
{                                                                                                                                                 \
    /* Const member function call through ptr to class instance:  R (*C::mem_fun)(args) const */                                                  \
                                                                                                                                                  \
    nicta::thread::guard<Lockable, LockDevice> g(this->mutex_);                                                                                   \
    this->condition_.wait(g(), boost::bind(const_mem_fun, boost::cref(c) NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a))));             \
}                                                                                                                                                 \
                                                                                                                                                  \
                                                                                                                                                  \
template <typename Lockable>                                                                                                                      \
template < typename R                                                                                                                             \
         NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                                       \
         , template<typename Lockable_> class LockDevice                                                                                          \
         >                                                                                                                                        \
void                                                                                                                                              \
nicta::thread::condition_wait<Lockable> ::                                                                                                        \
operator()(R (*ptr_fun)(BOOST_PP_ENUM_PARAMS(n, T))                                                                                               \
           NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a)))                                                                   \
{                                                                                                                                                 \
    /* Free-function call:  R ptr_fun(args) */                                                                                                    \
                                                                                                                                                  \
    nicta::thread::guard<Lockable, LockDevice> g(this->mutex_);                                                                                   \
    this->condition_.wait(g(), boost::bind(ptr_fun NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a))));                                   \
}
/**/

#define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
#define BOOST_PP_LOCAL_LIMITS (0, NICTA_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
#undef OPERATOR



// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
