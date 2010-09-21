/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_ALGORITHM_PIPE_CHECK_FOR_VOID_RETURN_H
#define NICTA_VIBE_VIDEO_ALGORITHM_PIPE_CHECK_FOR_VOID_RETURN_H

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_void.hpp>



#define CHECK_FOR_VOID_RETURN(z, n, R)                                                                                                                  \
    BOOST_STATIC_ASSERT(( !boost::template is_void< R ## n >::value ));
/**/


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
