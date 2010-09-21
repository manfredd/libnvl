/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_SPLICE_TYPE_H
#define NICTA_VIBE_IMAGE_SPLICE_TYPE_H

#include "image/image_depth.h"
#include "image/image_pixel.h"

#include <boost/mpl/at.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>


namespace nicta {
namespace vibe {
namespace image {

    /* forward declaration */
    template < typename T
             , template <typename T_> class U
             >
    class image;


    /* helper meta-function */
    template < typename Depth = nicta::vibe::image::depth_8u
             , template <typename _> class Pixel = nicta::vibe::image::bgr_p
             >
    struct splice_type
    {
        typedef boost::tuples::tuple< boost::shared_ptr<nicta::vibe::image::image<Depth, nicta::vibe::image::gray_p> >
                                    , boost::shared_ptr<nicta::vibe::image::image<Depth, nicta::vibe::image::gray_p> > 
                                    , boost::shared_ptr<nicta::vibe::image::image<Depth, nicta::vibe::image::gray_p> > 
                                    >
                c3_type;


        typedef boost::tuples::tuple< boost::shared_ptr<nicta::vibe::image::image<Depth, nicta::vibe::image::gray_p> >
                                    , boost::shared_ptr<nicta::vibe::image::image<Depth, nicta::vibe::image::gray_p> > 
                                    , boost::shared_ptr<nicta::vibe::image::image<Depth, nicta::vibe::image::gray_p> > 
                                    , boost::shared_ptr<nicta::vibe::image::image<Depth, nicta::vibe::image::gray_p> > 
                                    >
                c4_type;


        typedef boost::mpl::map< boost::mpl::pair<nicta::vibe::image::rgb_p<Depth>, typename nicta::vibe::image::splice_type<Depth, Pixel>::c3_type>
                               , boost::mpl::pair<nicta::vibe::image::bgr_p<Depth>, typename nicta::vibe::image::splice_type<Depth, Pixel>::c3_type>
                               , boost::mpl::pair<nicta::vibe::image::rgba_p<Depth>, typename nicta::vibe::image::splice_type<Depth, Pixel>::c4_type>
                               , boost::mpl::pair<nicta::vibe::image::bgra_p<Depth>, typename nicta::vibe::image::splice_type<Depth, Pixel>::c4_type>
                               , boost::mpl::pair<nicta::vibe::image::xyz_p<Depth>, typename nicta::vibe::image::splice_type<Depth, Pixel>::c3_type>
                               , boost::mpl::pair<nicta::vibe::image::YCrCb_p<Depth>, typename nicta::vibe::image::splice_type<Depth, Pixel>::c3_type>
                               , boost::mpl::pair<nicta::vibe::image::HSV_p<Depth>, typename nicta::vibe::image::splice_type<Depth, Pixel>::c3_type>
                               , boost::mpl::pair<nicta::vibe::image::HLS_p<Depth>, typename nicta::vibe::image::splice_type<Depth, Pixel>::c3_type>
                               , boost::mpl::pair<nicta::vibe::image::Luv_p<Depth>, typename nicta::vibe::image::splice_type<Depth, Pixel>::c3_type>
                               >
                m_type;


        typedef typename boost::mpl::at< typename nicta::vibe::image::splice_type<Depth, Pixel>::m_type, Pixel<Depth> >::type
                type;
    };

} } } // namespace nicta::vibe::image


#endif
