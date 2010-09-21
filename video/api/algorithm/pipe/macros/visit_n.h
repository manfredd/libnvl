/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_ALGORITHM_PIPE_MACROS_VISIT_N_H
#define NICTA_VIBE_VIDEO_ALGORITHM_PIPE_MACROS_VISIT_N_H

#include <boost/array.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/variant.hpp>
#include <boost/variant/variant_fwd.hpp>



#define VISIT_N(z, n, k)                                                                                                                                \
    ftor.template operator()< Depth ## n                                                                                                                \
                            , Pixel ## n                                                                                                                \
                            , R ## n                                                                                                                    \
                            , boost::variant<BOOST_PP_ENUM_BINARY_PARAMS(k, Visitable, * BOOST_PP_INTERCEPT)>                                           \
                            , boost::array<unsigned int, N>                                                                                             \
                            , C<Depth_, Pixel_>                                                                                                         \
                            >(&this->ctr_[n],                                                                              /* the algorithm  */         \
                              timestamp,                                                                                                                \
                              frame_index,                                                                                                              \
                              payload_index,                                                                                                            \
                              payload,                                                                                                                  \
                              source_id,                                                                                                                \
                              host_id,                                                                                                                  \
                              host_connections,                                                                                                         \
                              window_tags,                                                                                                              \
                              shutdown,                                                                                                                 \
                              host);
/**/


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
