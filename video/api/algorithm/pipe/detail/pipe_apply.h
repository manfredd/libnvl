/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_ALGORITHM_PIPE_DETAIL_PIPE_APPLY_H
#define NICTA_VIBE_VIDEO_ALGORITHM_PIPE_DETAIL_PIPE_APPLY_H

#include "video/algorithm/pipe/macros/check_for_void_return_.h"

#include <nicta_common/detail/max_arity.h>

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/variant.hpp>
#include <boost/variant/variant_fwd.hpp>

#include <set>
#include <string>
#include <utility>


namespace nicta {
namespace vibe {
namespace video {
namespace algorithm {
namespace pipe {

#define OPERATOR(z, n, _)                                                                                                                               \
                                                                                                                                                        \
    template < typename Initial_t                                                                                                                       \
             , BOOST_PP_ENUM_PARAMS(n, typename R)                                                                                                      \
             , template < typename Depth_                                                                                                               \
                        , template <typename> class Pixel_                                                                                              \
                        , typename R_                                                                                                                   \
                        , typename P_                                                                                                                   \
                        , typename C_                                                                                                                   \
                        > class Visitor                                                                                                                 \
             >                                                                                                                                          \
    class pipe_apply ## n ## _                                                                                                                          \
    {                                                                                                                                                   \
    public:                                                                                                                                             \
                                                                                                                                                        \
        typedef void result_type;                                                                                                                       \
                                                                                                                                                        \
                                                                                                                                                        \
        pipe_apply ## n ## _(Initial_t&& data, Initial_t* = 0) throw ()                                                                                 \
        : data_(std::move(data))                                                                                                                        \
        {                                                                                                                                               \
            /* Constructor */                                                                                                                           \
        }                                                                                                                                               \
                                                                                                                                                        \
                                                                                                                                                        \
        pipe_apply ## n ## _(Initial_t data) throw ()                                                                                                   \
        : data_(std::move(data))                                                                                                                        \
        {                                                                                                                                               \
            /* Constructor */                                                                                                                           \
        }                                                                                                                                               \
                                                                                                                                                        \
                                                                                                                                                        \
        template < typename Depth                                                                                                                       \
                 , template <typename> class Pixel                                                                                                      \
                 , typename R                                                                                                                           \
                 , typename A                                                                                                                           \
                 , typename P                                                                                                                           \
                 , typename C                                                                                                                           \
                 >                                                                                                                                      \
        nicta::vibe::video::algorithm::pipe::pipe_apply ## n ## _< Initial_t                                                                            \
                                                                 , BOOST_PP_ENUM_PARAMS(n, R)                                                           \
                                                                 , Visitor                                                                              \
                                                                 >::result_type                                                                         \
        operator()(const A* algorithm,                                                                                                                  \
                   boost::posix_time::ptime timestamp,                                                                                                  \
                   unsigned long int frame_index,                                                                                                       \
                   P& payload_index,                                                                                                                    \
                   unsigned int payload,                                                                                                                \
                   const std::string& source_id,                                                                                                        \
                   const std::string& host_id,                                                                                                          \
                   unsigned int host_connections,                                                                                                       \
                   std::set<std::string>& window_tags,                                                                                                  \
                   void (C::*shutdown)() const throw (),                                                                                                \
                   const C* const host                                                                                                                  \
                  )                                                                                                                                     \
        {                                                                                                                                               \
            /* Do work:  returning result -  this->data_ = algorithm(this->data_);  for next algorithm to pick up as input */                           \
                                                                                                                                                        \
            this->data_ = boost::apply_visitor(Visitor<Depth, Pixel, R, P, C>(timestamp,                                                                \
                                                                              frame_index,                                                              \
                                                                              payload_index,                                                            \
                                                                              payload,                                                                  \
                                                                              source_id,                                                                \
                                                                              host_id,                                                                  \
                                                                              host_connections,                                                         \
                                                                              window_tags,                                                              \
                                                                              shutdown,                                                                 \
                                                                              host),                                                                    \
                                               *algorithm,                                                                                              \
                                               this->data_);                                                                                            \
        }                                                                                                                                               \
                                                                                                                                                        \
                                                                                                                                                        \
        BOOST_PP_CAT(R, BOOST_PP_DEC(n))                                                                                                                \
        operator()() const throw ()                                                                                                                     \
        {                                                                                                                                               \
            /* Return result of last algorithm call - this should **always** return a bool! (..so as to interoperate with video-algorithms) */          \
                                                                                                                                                        \
            BOOST_STATIC_ASSERT(( boost::is_same< BOOST_PP_CAT(R, BOOST_PP_DEC(n)), bool >::value ));                                                   \
                                                                                                                                                        \
            return std::move(boost::get<BOOST_PP_CAT(R, BOOST_PP_DEC(n))>(this->data_));                                                                \
        }                                                                                                                                               \
                                                                                                                                                        \
                                                                                                                                                        \
    private:                                                                                                                                            \
                                                                                                                                                        \
        boost::variant<Initial_t, BOOST_PP_ENUM_PARAMS(n, R)> data_;                                                                                    \
                                                                                                                                                        \
                                                                                                                                                        \
        BOOST_PP_REPEAT(BOOST_PP_DEC(n), CHECK_FOR_VOID_RETURN_, R)                                                                                     \
        BOOST_STATIC_ASSERT(( NICTA_MAX_ARITY < BOOST_VARIANT_LIMIT_TYPES ));                                                                           \
    };
/**/


#define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
#define BOOST_PP_LOCAL_LIMITS (1, NICTA_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
#undef OPERATOR

#undef CHECK_FOR_VOID_RETURN_

} } } } }  // namespace nicta::vibe::video::algorithm::pipe


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
