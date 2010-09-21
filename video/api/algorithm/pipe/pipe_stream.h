/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_ALGORITHM_PIPE_PIPE_STREAM_H
#define NICTA_VIBE_VIDEO_ALGORITHM_PIPE_PIPE_STREAM_H

#include "video/algorithm/pipe/detail/pipe_apply.h"
#include "video/algorithm/pipe/macros/check_for_void_return.h"
#include "video/algorithm/pipe/macros/visit.h"
#include "video/algorithm/pipe/macros/visit_n.h"

#include <nicta_common/detail/max_arity.h>
#include <nicta_common/macros/alternating_repeat.h>
#include <nicta_common/macros/compiler.h>

#include <boost/array.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/variant.hpp>
#include <boost/variant/variant_fwd.hpp>

#include <set>
#include <string>
#include <utility>
#include <vector>


namespace nicta {
namespace vibe {

namespace image {

    /* forward declaration */
    template < typename Depth
             , template <typename _> class Pixel
             >
    class image;

} // namespace nicta::vibe::image


namespace video {
namespace algorithm {
namespace pipe {

#if defined(x86_64) && GCC_VERSION == 40402                 /* special handling needed, owing to bug in GCC-4.4.2 on 64-bit machines */ 


# define OPERATOR(z, n, _)                                                                                                                                         \
                                                                                                                                                                   \
    template < NICTA_ALTERNATING_REPEAT_0(n, (typename Depth)                                             /* the incoming depth-type of streamable   */            \
                                             (template <typename> class Pixel)                            /* the incoming pixel-type of streamable   */            \
                                             (typename R)                                                 /* the algo return value R_i               */            \
                                             (typename Visitable))                                        /* the streamable (algo)                   */            \
             , template < typename D_                                                                                                                              \
                        , template <typename> class Px_                                                                                                            \
                        , typename R_                                                                                                                              \
                        , typename P_                                                                                                                              \
                        , typename C_                                                                                                                              \
                        > class Visitor                                                                   /* visitor having return value R_i         */            \
             , template < typename V_                                                                                                                              \
                        , typename A_ = std::allocator<V_>                                                                                                         \
                        > class Ctr = std::vector                                                         /* type of ctr holding streamables (algos) */            \
             >                                                                                                                                                     \
    class pipe_stream ## n                                                                                                                                         \
    {                                                                                                                                                              \
    public:                                                                                                                                                        \
                                                                                                                                                                   \
        pipe_stream ## n (Ctr<boost::variant<BOOST_PP_ENUM_BINARY_PARAMS(n, Visitable, * BOOST_PP_INTERCEPT)> >&& ctr) throw ()                                    \
        : ctr_(std::move(ctr))                                                                                                                                     \
        {                                                                                                                                                          \
            /* Constructor */                                                                                                                                      \
        }                                                                                                                                                          \
                                                                                                                                                                   \
                                                                                                                                                                   \
        template < typename Depth_                                                                                                                                 \
                 , template <typename> class Pixel_                                                                                                                \
                 , template <typename, template <typename> class> class C                                                                                          \
                 >                                                                                                                                                 \
        bool operator()(const boost::shared_ptr<nicta::vibe::image::image<Depth_, Pixel_> > frame,                                                                 \
                        boost::posix_time::ptime timestamp,                                                                                                        \
                        unsigned long int frame_index,                                                                                                             \
                        unsigned int payload_index,                                                                                                                \
                        unsigned int payload,                                                                                                                      \
                        const std::string& source_id,                                                                                                              \
                        const std::string& host_id,                                                                                                                \
                        unsigned int host_connections,                                                                                                             \
                        std::set<std::string>& window_tags,                                                                                                        \
                        void (C<Depth_, Pixel_>::*shutdown)() const throw (),                                                                                      \
                        const C<Depth_, Pixel_>* const host                                                                                                        \
                       ) const                                                                                                                                     \
        {                                                                                                                                                          \
            /* Do work: feed output of algo_i into algo_(i+1); with algo_0 taking 'frame' as it's input                 */                                         \
            /* NOTE:  the last algorithm **should** return a 'bool' ..accomodates last algo displaying window on screen */                                         \
                                                                                                                                                                   \
            nicta::vibe::video::algorithm::pipe::pipe_apply ## n ## _< boost::shared_ptr<nicta::vibe::image::image<Depth_, Pixel_> >                               \
                                                                     , BOOST_PP_ENUM_PARAMS(n, R)                                                                  \
                                                                     , Visitor                                                                                     \
                                                                     > ftor(std::move(frame));                                                                     \
            BOOST_PP_REPEAT(n, VISIT, n)                                                                                                                           \
            return ftor.operator()();                                                                                                                              \
        }                                                                                                                                                          \
                                                                                                                                                                   \
                                                                                                                                                                   \
                                                                                                                                                                   \
        template < unsigned long int N                      /* bug in GCC-4.4.2 on 64-bit machines - doesn't recognize unsigned int correctly */                   \
                 , typename Depth_                                                                                                                                 \
                 , template <typename> class Pixel_                                                                                                                \
                 , template <typename, template <typename> class> class C                                                                                          \
                 >                                                                                                                                                 \
        bool operator()(const boost::array<boost::shared_ptr<nicta::vibe::image::image<Depth_, Pixel_> >, N>& frame,                                               \
                        boost::posix_time::ptime timestamp,                                                                                                        \
                        unsigned long int frame_index,                                                                                                             \
                        boost::array<unsigned int, N>& payload_index,                                                                                              \
                        unsigned int payload,                                                                                                                      \
                        const std::string& source_id,                                                                                                              \
                        const std::string& host_id,                                                                                                                \
                        unsigned int host_connections,                                                                                                             \
                        std::set<std::string>& window_tags,                                                                                                        \
                        void (C<Depth_, Pixel_>::*shutdown)() const throw (),                                                                                      \
                        const C<Depth_, Pixel_>* const host                                                                                                        \
                       ) const                                                                                                                                     \
        {                                                                                                                                                          \
            /* Do work: feed output of algo_i into algo_(i+1); with algo_0 taking 'frame' as it's input                 */                                         \
            /* NOTE:  the last algorithm **should** return a 'bool' ..accomodates last algo displaying window on screen */                                         \
                                                                                                                                                                   \
            nicta::vibe::video::algorithm::pipe::pipe_apply ## n ## _< boost::array<boost::shared_ptr<nicta::vibe::image::image<Depth_, Pixel_> >, N>              \
                                                                     , BOOST_PP_ENUM_PARAMS(n, R)                                                                  \
                                                                     , Visitor                                                                                     \
                                                                     > ftor(std::move(frame));                                                                     \
            BOOST_PP_REPEAT(n, VISIT_N, n)                                                                                                                         \
            return ftor.operator()();                                                                                                                              \
        }                                                                                                                                                          \
                                                                                                                                                                   \
                                                                                                                                                                   \
    private:                                                                                                                                                       \
                                                                                                                                                                   \
        pipe_stream ## n () = delete;                                                                                                                              \
                                                                                                                                                                   \
        Ctr<boost::variant<BOOST_PP_ENUM_BINARY_PARAMS(n, Visitable, * BOOST_PP_INTERCEPT)> > ctr_;                                                                \
                                                                                                                                                                   \
                                                                                                                                                                   \
        BOOST_PP_REPEAT(n, CHECK_FOR_VOID_RETURN, R)                                                                                                               \
        BOOST_STATIC_ASSERT(( NICTA_MAX_ARITY < BOOST_VARIANT_LIMIT_TYPES ));                                                                                      \
        BOOST_STATIC_ASSERT(( boost::is_same< BOOST_PP_CAT(R, BOOST_PP_DEC(n)), bool >::value ));                                                                  \
    }; 
/**/


# define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
# define BOOST_PP_LOCAL_LIMITS (1, NICTA_MAX_ARITY)
# include BOOST_PP_LOCAL_ITERATE()
# undef BOOST_PP_LOCAL_MACRO
# undef BOOST_PP_LOCAL_LIMITS
# undef OPERATOR


#else


# define OPERATOR(z, n, _)                                                                                                                                         \
                                                                                                                                                                   \
    template < NICTA_ALTERNATING_REPEAT_0(n, (typename Depth)                                             /* the incoming depth-type of streamable   */            \
                                             (template <typename> class Pixel)                            /* the incoming pixel-type of streamable   */            \
                                             (typename R)                                                 /* the algo return value R_i               */            \
                                             (typename Visitable))                                        /* the streamable (algo)                   */            \
             , template < typename D_                                                                                                                              \
                        , template <typename> class Px_                                                                                                            \
                        , typename R_                                                                                                                              \
                        , typename P_                                                                                                                              \
                        , typename C_                                                                                                                              \
                        > class Visitor                                                                   /* visitor having return value R_i         */            \
             , template < typename V_                                                                                                                              \
                        , typename A_ = std::allocator<V_>                                                                                                         \
                        > class Ctr = std::vector                                                         /* type of ctr holding streamables (algos) */            \
             >                                                                                                                                                     \
    class pipe_stream ## n                                                                                                                                         \
    {                                                                                                                                                              \
    public:                                                                                                                                                        \
                                                                                                                                                                   \
        pipe_stream ## n (Ctr<boost::variant<BOOST_PP_ENUM_BINARY_PARAMS(n, Visitable, * BOOST_PP_INTERCEPT)> >&& ctr) throw ()                                    \
        : ctr_(std::move(ctr))                                                                                                                                     \
        {                                                                                                                                                          \
            /* Constructor */                                                                                                                                      \
        }                                                                                                                                                          \
                                                                                                                                                                   \
                                                                                                                                                                   \
        template < typename Depth_                                                                                                                                 \
                 , template <typename> class Pixel_                                                                                                                \
                 , template <typename, template <typename> class> class C                                                                                          \
                 >                                                                                                                                                 \
        bool operator()(const boost::shared_ptr<nicta::vibe::image::image<Depth_, Pixel_> > frame,                                                                 \
                        boost::posix_time::ptime timestamp,                                                                                                        \
                        unsigned long int frame_index,                                                                                                             \
                        unsigned int payload_index,                                                                                                                \
                        unsigned int payload,                                                                                                                      \
                        const std::string& source_id,                                                                                                              \
                        const std::string& host_id,                                                                                                                \
                        unsigned int host_connections,                                                                                                             \
                        std::set<std::string>& window_tags,                                                                                                        \
                        void (C<Depth_, Pixel_>::*shutdown)() const throw (),                                                                                      \
                        const C<Depth_, Pixel_>* const host                                                                                                        \
                       ) const                                                                                                                                     \
        {                                                                                                                                                          \
            /* Do work: feed output of algo_i into algo_(i+1); with algo_0 taking 'frame' as it's input                 */                                         \
            /* NOTE:  the last algorithm **should** return a 'bool' ..accomodates last algo displaying window on screen */                                         \
                                                                                                                                                                   \
            nicta::vibe::video::algorithm::pipe::pipe_apply ## n ## _< boost::shared_ptr<nicta::vibe::image::image<Depth_, Pixel_> >                               \
                                                                     , BOOST_PP_ENUM_PARAMS(n, R)                                                                  \
                                                                     , Visitor                                                                                     \
                                                                     > ftor(std::move(frame));                                                                     \
            BOOST_PP_REPEAT(n, VISIT, n)                                                                                                                           \
            return ftor.operator()();                                                                                                                              \
        }                                                                                                                                                          \
                                                                                                                                                                   \
                                                                                                                                                                   \
        template < unsigned int N                                                                                                                                  \
                 , typename Depth_                                                                                                                                 \
                 , template <typename> class Pixel_                                                                                                                \
                 , template <typename, template <typename> class> class C                                                                                          \
                 >                                                                                                                                                 \
        bool operator()(const boost::array<boost::shared_ptr<nicta::vibe::image::image<Depth_, Pixel_> >, N>& frame,                                               \
                        boost::posix_time::ptime timestamp,                                                                                                        \
                        unsigned long int frame_index,                                                                                                             \
                        boost::array<unsigned int, N>& payload_index,                                                                                              \
                        unsigned int payload,                                                                                                                      \
                        const std::string& source_id,                                                                                                              \
                        const std::string& host_id,                                                                                                                \
                        unsigned int host_connections,                                                                                                             \
                        std::set<std::string>& window_tags,                                                                                                        \
                        void (C<Depth_, Pixel_>::*shutdown)() const throw (),                                                                                      \
                        const C<Depth_, Pixel_>* const host                                                                                                        \
                       ) const                                                                                                                                     \
        {                                                                                                                                                          \
            /* Do work: feed output of algo_i into algo_(i+1); with algo_0 taking 'frame' as it's input                 */                                         \
            /* NOTE:  the last algorithm **should** return a 'bool' ..accomodates last algo displaying window on screen */                                         \
                                                                                                                                                                   \
            nicta::vibe::video::algorithm::pipe::pipe_apply ## n ## _< boost::array<boost::shared_ptr<nicta::vibe::image::image<Depth_, Pixel_> >, N>              \
                                                                     , BOOST_PP_ENUM_PARAMS(n, R)                                                                  \
                                                                     , Visitor                                                                                     \
                                                                     > ftor(std::move(frame));                                                                     \
            BOOST_PP_REPEAT(n, VISIT_N, n)                                                                                                                         \
            return ftor.operator()();                                                                                                                              \
        }                                                                                                                                                          \
                                                                                                                                                                   \
                                                                                                                                                                   \
    private:                                                                                                                                                       \
                                                                                                                                                                   \
        pipe_stream ## n () = delete;                                                                                                                              \
                                                                                                                                                                   \
        Ctr<boost::variant<BOOST_PP_ENUM_BINARY_PARAMS(n, Visitable, * BOOST_PP_INTERCEPT)> > ctr_;                                                                \
                                                                                                                                                                   \
                                                                                                                                                                   \
        BOOST_PP_REPEAT(n, CHECK_FOR_VOID_RETURN, R)                                                                                                               \
        BOOST_STATIC_ASSERT(( NICTA_MAX_ARITY < BOOST_VARIANT_LIMIT_TYPES ));                                                                                      \
        BOOST_STATIC_ASSERT(( boost::is_same< BOOST_PP_CAT(R, BOOST_PP_DEC(n)), bool >::value ));                                                                  \
    };
/**/


# define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
# define BOOST_PP_LOCAL_LIMITS (1, NICTA_MAX_ARITY)
# include BOOST_PP_LOCAL_ITERATE()
# undef BOOST_PP_LOCAL_MACRO
# undef BOOST_PP_LOCAL_LIMITS
# undef OPERATOR


#endif


#undef CHECK_FOR_VOID_RETURN
#undef VISIT_N
#undef VISIT

} } } } }  // namespace nicta::vibe::video::algorithm::pipe


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
