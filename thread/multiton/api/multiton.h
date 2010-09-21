/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_THREAD_MULTITON_H
#define NICTA_THREAD_MULTITON_H

#include <nicta_common/detail/max_arity.h>
#include <nicta_common/macros/conditional_comma.h>

#include <boost/noncopyable.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

#include <vector>

namespace nicta {
namespace thread {

    enum Array
    {
        block
    };

    template <nicta::thread::Array T = nicta::thread::block>
    struct array
    {
        enum
        {
            alloc = T
        };
    };


    template < unsigned int I = 1
             , typename Lockable = boost::mutex
             >
    class multiton : boost::noncopyable
    {
    public:

        template <unsigned int Instance>
        static boost::shared_ptr<nicta::thread::multiton<I, Lockable> > instance() throw ();

        template < typename C
                 , template<typename Lockable_> class LockDevice
                 >
        void operator()(C* c) throw ();


        template < typename C
                 , typename Block = nicta::thread::array<nicta::thread::block>
                 , template<typename Lockable_> class LockDevice
                 >
        void operator()(C* c) throw ();


        template < typename C
                 , typename size_type
                 , typename Block = nicta::thread::array<nicta::thread::block>
                 , template<typename Lockable_> class LockDevice
                 >
        C* operator()(size_type n);


        template < typename C
                 , typename P
                 , typename offset_type
                 , typename size_type
                 , typename Block = nicta::thread::array<nicta::thread::block>
                 , template<typename Lockable_> class LockDevice
                 >
        C* operator()(P pool, offset_type m, size_type n);


#define OPERATOR(z, n, _)                                                                                                                         \
                                                                                                                                                  \
        template < typename C                                                                                                                     \
                 NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                               \
                 , template<typename Lockable_> class LockDevice                                                                                  \
                 >                                                                                                                                \
        C* operator()(BOOST_PP_ENUM_BINARY_PARAMS(n, T, a));                                                                                      \
                                                                                                                                                  \
                                                                                                                                                  \
        template < typename R                                                                                                                     \
                 , typename C                                                                                                                     \
                 NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                               \
                 , template<typename Lockable_> class LockDevice                                                                                  \
                 >                                                                                                                                \
        R operator()(R (C::*mem_fun_ref)(BOOST_PP_ENUM_PARAMS(n, T)),                                                                             \
                     C& c                                                                                                                         \
                     NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a)));                                                        \
                                                                                                                                                  \
                                                                                                                                                  \
        template < typename R                                                                                                                     \
                 , typename C                                                                                                                     \
                 NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                               \
                 , template<typename Lockable_> class LockDevice                                                                                  \
                 >                                                                                                                                \
        R operator()(R (C::*mem_fun)(BOOST_PP_ENUM_PARAMS(n, T)),                                                                                 \
                     C* c                                                                                                                         \
                     NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a)));                                                        \
                                                                                                                                                  \
                                                                                                                                                  \
        template < typename R                                                                                                                     \
                 , typename C                                                                                                                     \
                 NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                               \
                 , template<typename Lockable_> class LockDevice                                                                                  \
                 >                                                                                                                                \
        R operator()(R (C::*const_mem_fun_ref)(BOOST_PP_ENUM_PARAMS(n, T)) const,                                                                 \
                     const C& c                                                                                                                   \
                     NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a))) const;                                                  \
                                                                                                                                                  \
                                                                                                                                                  \
        template < typename R                                                                                                                     \
                 , typename C                                                                                                                     \
                 NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                               \
                 , template<typename Lockable_> class LockDevice                                                                                  \
                 >                                                                                                                                \
        R operator()(R (C::*const_mem_fun)(BOOST_PP_ENUM_PARAMS(n, T)) const,                                                                     \
                     const C* c                                                                                                                   \
                     NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a))) const;                                                  \
                                                                                                                                                  \
                                                                                                                                                  \
        template < typename R                                                                                                                     \
                 NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                               \
                 , template<typename Lockable_> class LockDevice                                                                                  \
                 >                                                                                                                                \
        R operator()(R (*ptr_fun)(BOOST_PP_ENUM_PARAMS(n, T))                                                                                     \
                     NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a)));
        /**/

#define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
#define BOOST_PP_LOCAL_LIMITS (0, NICTA_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
#undef OPERATOR


    private:

        template <template<typename Lockable_> class Lock = boost::unique_lock>
        class guard_ : boost::noncopyable
        {
        public:

            ~guard_() throw ()
            {
                if (this->lock_.owns_lock())
                {
                    this->lock_.unlock();
                }
            }

            explicit guard_(Lockable& mutex) throw ()
            : lock_(mutex)
            {
            }


        private:

            Lock<Lockable> lock_;
        };


        multiton<I, Lockable>() throw ();


        // no static storage, so will have a unique mutex for *each* multiton instance_[i = 1->I]
        mutable Lockable mutex_;
        static std::vector<boost::shared_ptr<nicta::thread::multiton<I, Lockable> > > instance_;



        BOOST_STATIC_ASSERT (I > 0);
    };

} } // namespace nicta::thread



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


/* initialize static memeber */
template < unsigned int I
         , typename Lockable
         >
std::vector<boost::shared_ptr<nicta::thread::multiton<I, Lockable> > >
nicta::thread::multiton< I
                       , Lockable
                       > :: 
instance_(I, boost::shared_ptr<nicta::thread::multiton<I, Lockable> > (static_cast<nicta::thread::multiton<I, Lockable> *> (0)));



template < unsigned int I
         , typename Lockable
         >
nicta::thread::multiton< I
                       , Lockable
                       > :: 
multiton() throw ()
{
    // Constructor

}


template < unsigned int I
         , typename Lockable
         >
template <unsigned int Instance>
boost::shared_ptr<nicta::thread::multiton<I, Lockable> >
nicta::thread::multiton< I
                       , Lockable
                       > :: 
instance() throw ()
{
    // Return the singleton instance

    BOOST_STATIC_ASSERT (Instance < I);

    if (nicta::thread::multiton<I, Lockable>::instance_[Instance] ==
        boost::shared_ptr<nicta::thread::multiton<I, Lockable> > (static_cast<nicta::thread::multiton<I, Lockable> *> (0)))
    {
        nicta::thread::multiton<I, Lockable>::instance_[Instance] =
                                            boost::shared_ptr<nicta::thread::multiton<I, Lockable> > (new nicta::thread::multiton<I, Lockable>());
    }
    return nicta::thread::multiton<I, Lockable>::instance_[Instance];
}


template < unsigned int I
         , typename Lockable
         >
template < typename C
         , template <typename _> class LockDevice
         >
void
nicta::thread::multiton< I 
                       , Lockable
                       > :: 
operator()(C* c) throw ()
{
    // Delete call

    typename nicta::thread::multiton<I, Lockable>::template guard_<LockDevice> g(this->mutex_);
    delete c;
}


template < unsigned int I
         , typename Lockable
         >
template < typename C
         , typename Block
         , template <typename _> class LockDevice
         >
void
nicta::thread::multiton< I 
                       , Lockable
                       > :: 
operator()(C* c) throw ()
{
    // Delete call for  new C[n]  ojbects

    typename nicta::thread::multiton<I, Lockable>::template guard_<LockDevice> g(this->mutex_);
    delete[] c;
}


template < unsigned int I
         , typename Lockable
         >
template < typename C
         , typename size_type
         , typename Block
         , template <typename _> class LockDevice
         >
C*
nicta::thread::multiton< I 
                       , Lockable
                       > :: 
operator()(size_type n)
{
    // Constructor call of  new C[n]  types

    typename nicta::thread::multiton<I, Lockable>::template guard_<LockDevice> g(this->mutex_);
    return new C[n];
}


template < unsigned int I
         , typename Lockable
         >
template < typename C
         , typename P
         , typename offset_type
         , typename size_type
         , typename Block
         , template <typename _> class LockDevice
         >
C*
nicta::thread::multiton< I 
                       , Lockable
                       > :: 
operator()(P pool, offset_type m, size_type n)
{
    // Constructor call with placement-new:  new (pool + m * n) C[n]  types

    typename nicta::thread::multiton<I, Lockable>::template guard_<LockDevice> g(this->mutex_);
    return new (pool + m * n) C[n];
}



#define OPERATOR(z, n, _)                                                                                                                         \
                                                                                                                                                  \
template < unsigned int I                                                                                                                         \
         , typename Lockable                                                                                                                      \
         >                                                                                                                                        \
template < typename C                                                                                                                             \
         NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                                       \
         , template <typename _> class LockDevice                                                                                                 \
         >                                                                                                                                        \
C*                                                                                                                                                \
nicta::thread::multiton< I                                                                                                                        \
                       , Lockable                                                                                                                 \
                       > ::                                                                                                                       \
operator()(BOOST_PP_ENUM_BINARY_PARAMS(n, T, a))                                                                                                  \
{                                                                                                                                                 \
    /* Constructor call:  C(args) */                                                                                                              \
                                                                                                                                                  \
    typename nicta::thread::multiton<I, Lockable>::template guard_<LockDevice> g(this->mutex_);                                                   \
    return new C(BOOST_PP_ENUM_PARAMS(n, a));                                                                                                     \
}                                                                                                                                                 \
                                                                                                                                                  \
                                                                                                                                                  \
template < unsigned int I                                                                                                                         \
         , typename Lockable                                                                                                                      \
         >                                                                                                                                        \
template < typename R                                                                                                                             \
         , typename C                                                                                                                             \
         NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                                       \
         , template <typename _> class LockDevice                                                                                                 \
         >                                                                                                                                        \
R                                                                                                                                                 \
nicta::thread::multiton< I                                                                                                                        \
                       , Lockable                                                                                                                 \
                       > ::                                                                                                                       \
operator()(R (C::*mem_fun_ref)(BOOST_PP_ENUM_PARAMS(n, T)),                                                                                       \
           C& c                                                                                                                                   \
           NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a)))                                                                   \
{                                                                                                                                                 \
    /* Member function call through class instance:  R C::mem_fun_ref(args) */                                                                    \
                                                                                                                                                  \
    typename nicta::thread::multiton<I, Lockable>::template guard_<LockDevice> g(this->mutex_);                                                   \
    return (c.*mem_fun_ref)(BOOST_PP_ENUM_PARAMS(n, a));                                                                                          \
}                                                                                                                                                 \
                                                                                                                                                  \
                                                                                                                                                  \
template < unsigned int I                                                                                                                         \
         , typename Lockable                                                                                                                      \
         >                                                                                                                                        \
template < typename R                                                                                                                             \
         , typename C                                                                                                                             \
         NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                                       \
         , template <typename _> class LockDevice                                                                                                 \
         >                                                                                                                                        \
R                                                                                                                                                 \
nicta::thread::multiton< I                                                                                                                        \
                       , Lockable                                                                                                                 \
                       > ::                                                                                                                       \
operator()(R (C::*mem_fun)(BOOST_PP_ENUM_PARAMS(n, T)),                                                                                           \
           C* c                                                                                                                                   \
           NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a)))                                                                   \
{                                                                                                                                                 \
    /* Member function call through ptr to class instance:  R (*C::mem_fun)(args) */                                                              \
                                                                                                                                                  \
    typename nicta::thread::multiton<I, Lockable>::template guard_<LockDevice> g(this->mutex_);                                                   \
    return (c->*mem_fun)(BOOST_PP_ENUM_PARAMS(n, a));                                                                                             \
}                                                                                                                                                 \
                                                                                                                                                  \
                                                                                                                                                  \
template < unsigned int I                                                                                                                         \
         , typename Lockable                                                                                                                      \
         >                                                                                                                                        \
template < typename R                                                                                                                             \
         , typename C                                                                                                                             \
         NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                                       \
         , template <typename _> class LockDevice                                                                                                 \
         >                                                                                                                                        \
R                                                                                                                                                 \
nicta::thread::multiton< I                                                                                                                        \
                       , Lockable                                                                                                                 \
                       > ::                                                                                                                       \
operator()(R (C::*const_mem_fun_ref)(BOOST_PP_ENUM_PARAMS(n, T)) const,                                                                           \
           const C& c                                                                                                                             \
           NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a))) const                                                             \
{                                                                                                                                                 \
    /* Const member function call through class instance:  R C::mem_fun_ref(args) const */                                                        \
                                                                                                                                                  \
    typename nicta::thread::multiton<I, Lockable>::template guard_<LockDevice> g(this->mutex_);                                                   \
    return (c.*const_mem_fun_ref)(BOOST_PP_ENUM_PARAMS(n, a));                                                                                    \
}                                                                                                                                                 \
                                                                                                                                                  \
                                                                                                                                                  \
template < unsigned int I                                                                                                                         \
         , typename Lockable                                                                                                                      \
         >                                                                                                                                        \
template < typename R                                                                                                                             \
         , typename C                                                                                                                             \
         NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                                       \
         , template <typename _> class LockDevice                                                                                                 \
         >                                                                                                                                        \
R                                                                                                                                                 \
nicta::thread::multiton< I                                                                                                                        \
                       , Lockable                                                                                                                 \
                       > ::                                                                                                                       \
operator()(R (C::*const_mem_fun)(BOOST_PP_ENUM_PARAMS(n, T)) const,                                                                               \
           const C* c                                                                                                                             \
           NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a))) const                                                             \
{                                                                                                                                                 \
    /* Const member function call through ptr to class instance:  R (*C::mem_fun)(args) const */                                                  \
                                                                                                                                                  \
    typename nicta::thread::multiton<I, Lockable>::template guard_<LockDevice> g(this->mutex_);                                                   \
    return (c->*const_mem_fun)(BOOST_PP_ENUM_PARAMS(n, a));                                                                                       \
}                                                                                                                                                 \
                                                                                                                                                  \
                                                                                                                                                  \
template < unsigned int I                                                                                                                         \
         , typename Lockable                                                                                                                      \
         >                                                                                                                                        \
template < typename R                                                                                                                             \
         NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename T))                                                                       \
         , template <typename _> class LockDevice                                                                                                 \
         >                                                                                                                                        \
R                                                                                                                                                 \
nicta::thread::multiton< I                                                                                                                        \
                       , Lockable                                                                                                                 \
                       > ::                                                                                                                       \
operator()(R (*ptr_fun)(BOOST_PP_ENUM_PARAMS(n, T))                                                                                               \
           NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, T, a)))                                                                   \
{                                                                                                                                                 \
    /* Free-function call:  R ptr_fun(args) */                                                                                                    \
                                                                                                                                                  \
    typename nicta::thread::multiton<I, Lockable>::template guard_<LockDevice> g(this->mutex_);                                                   \
    return ptr_fun(BOOST_PP_ENUM_PARAMS(n, a));                                                                                                   \
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
