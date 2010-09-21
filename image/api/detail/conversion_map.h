/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_DETAIL_CONVERSION_MAP_H
#define NICTA_VIBE_IMAGE_DETAIL_CONVERSION_MAP_H

#include "image/models/colorcode_model.h"

#include <opencv/cv.h>

#include <boost/mpl/at.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/void.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>


namespace nicta {
namespace vibe {
namespace image {
namespace detail {

    template <int From, int To>
    struct conversion_map
    {
        struct void_                       // returned when conversion From -> To is the identity (ie. From == To) - we SHOULD NOT convert if this is returned!!
        {
            typedef void_ type;

            enum
            {
                value = -1 
            };
        };

        struct error_                      // returned when conversion From -> To does not exist - the static_assert below guards us from this eventuality
        {
            typedef error_ type;

            enum
            {
                value = -2 
            };
        };


        typedef boost::mpl::map< boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::RGB>, boost::mpl::int_<nicta::vibe::image::GRAY> >
                                                 , boost::mpl::int_<CV_RGB2GRAY> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::BGR>, boost::mpl::int_<nicta::vibe::image::GRAY> >
                                                 , boost::mpl::int_<CV_BGR2GRAY> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::GRAY>, boost::mpl::int_<nicta::vibe::image::RGB> >
                                                 , boost::mpl::int_<CV_GRAY2RGB> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::GRAY>, boost::mpl::int_<nicta::vibe::image::BGR> >
                                                 , boost::mpl::int_<CV_GRAY2BGR> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::RGBA>, boost::mpl::int_<nicta::vibe::image::GRAY> >
                                                 , boost::mpl::int_<CV_RGBA2GRAY> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::BGRA>, boost::mpl::int_<nicta::vibe::image::GRAY> >
                                                 , boost::mpl::int_<CV_BGRA2GRAY> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::GRAY>, boost::mpl::int_<nicta::vibe::image::RGBA> >
                                                 , boost::mpl::int_<CV_GRAY2RGBA> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::GRAY>, boost::mpl::int_<nicta::vibe::image::BGRA> >
                                                 , boost::mpl::int_<CV_GRAY2BGRA> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::RGB>, boost::mpl::int_<nicta::vibe::image::XYZ> >
                                                 , boost::mpl::int_<CV_RGB2XYZ> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::BGR>, boost::mpl::int_<nicta::vibe::image::XYZ> >
                                                 , boost::mpl::int_<CV_BGR2XYZ> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::XYZ>, boost::mpl::int_<nicta::vibe::image::RGB> >
                                                 , boost::mpl::int_<CV_XYZ2RGB> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::XYZ>, boost::mpl::int_<nicta::vibe::image::BGR> >
                                                 , boost::mpl::int_<CV_XYZ2BGR> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::RGB>, boost::mpl::int_<nicta::vibe::image::YCrCb> >
                                                 , boost::mpl::int_<CV_RGB2YCrCb> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::BGR>, boost::mpl::int_<nicta::vibe::image::YCrCb> >
                                                 , boost::mpl::int_<CV_BGR2YCrCb> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::YCrCb>, boost::mpl::int_<nicta::vibe::image::RGB> >
                                                 , boost::mpl::int_<CV_YCrCb2RGB> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::YCrCb>, boost::mpl::int_<nicta::vibe::image::BGR> >
                                                 , boost::mpl::int_<CV_YCrCb2BGR> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::RGB>, boost::mpl::int_<nicta::vibe::image::HSV> >
                                                 , boost::mpl::int_<CV_RGB2HSV> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::BGR>, boost::mpl::int_<nicta::vibe::image::HSV> >
                                                 , boost::mpl::int_<CV_BGR2HSV> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::HSV>, boost::mpl::int_<nicta::vibe::image::RGB> >
                                                 , boost::mpl::int_<CV_HSV2RGB> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::HSV>, boost::mpl::int_<nicta::vibe::image::BGR> >
                                                 , boost::mpl::int_<CV_HSV2BGR> >
                               >::type c_type1;


        typedef boost::mpl::map< boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::RGB>, boost::mpl::int_<nicta::vibe::image::HLS> >
                                                 , boost::mpl::int_<CV_RGB2HLS> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::BGR>, boost::mpl::int_<nicta::vibe::image::HLS> >
                                                 , boost::mpl::int_<CV_BGR2HLS> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::HLS>, boost::mpl::int_<nicta::vibe::image::RGB> >
                                                 , boost::mpl::int_<CV_HLS2RGB> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::HLS>, boost::mpl::int_<nicta::vibe::image::BGR> >
                                                 , boost::mpl::int_<CV_HLS2BGR> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::RGB>, boost::mpl::int_<nicta::vibe::image::Lab> >
                                                 , boost::mpl::int_<CV_RGB2Lab> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::BGR>, boost::mpl::int_<nicta::vibe::image::Lab> >
                                                 , boost::mpl::int_<CV_BGR2Lab> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::Lab>, boost::mpl::int_<nicta::vibe::image::RGB> >
                                                 , boost::mpl::int_<CV_Lab2RGB> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::Lab>, boost::mpl::int_<nicta::vibe::image::BGR> >
                                                 , boost::mpl::int_<CV_Lab2BGR> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::RGB>, boost::mpl::int_<nicta::vibe::image::Luv> >
                                                 , boost::mpl::int_<CV_RGB2Luv> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::BGR>, boost::mpl::int_<nicta::vibe::image::Luv> >
                                                 , boost::mpl::int_<CV_BGR2Luv> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::Luv>, boost::mpl::int_<nicta::vibe::image::RGB> >
                                                 , boost::mpl::int_<CV_Luv2RGB> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::Luv>, boost::mpl::int_<nicta::vibe::image::BGR> >
                                                 , boost::mpl::int_<CV_Luv2BGR> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::BayerBG>, boost::mpl::int_<nicta::vibe::image::RGB> >
                                                 , boost::mpl::int_<CV_BayerBG2RGB> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::BayerGB>, boost::mpl::int_<nicta::vibe::image::RGB> >
                                                 , boost::mpl::int_<CV_BayerGB2RGB> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::BayerRG>, boost::mpl::int_<nicta::vibe::image::RGB> >
                                                 , boost::mpl::int_<CV_BayerRG2RGB> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::BayerGR>, boost::mpl::int_<nicta::vibe::image::RGB> >
                                                 , boost::mpl::int_<CV_BayerGR2RGB> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::BayerBG>, boost::mpl::int_<nicta::vibe::image::BGR> >
                                                 , boost::mpl::int_<CV_BayerBG2BGR> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::BayerGB>, boost::mpl::int_<nicta::vibe::image::BGR> >
                                                 , boost::mpl::int_<CV_BayerGB2BGR> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::BayerRG>, boost::mpl::int_<nicta::vibe::image::BGR> >
                                                 , boost::mpl::int_<CV_BayerRG2BGR> >
                               , boost::mpl::pair< boost::mpl::pair<boost::mpl::int_<nicta::vibe::image::BayerGR>, boost::mpl::int_<nicta::vibe::image::BGR> >
                                                 , boost::mpl::int_<CV_BayerGR2BGR> >
                               > c_type2;


        typedef typename boost::mpl::at< typename nicta::vibe::image::detail::conversion_map<From, To>::c_type1
                                       , boost::mpl::pair<boost::mpl::int_<From>, boost::mpl::int_<To> >
                                       >::type type1;

        typedef typename boost::mpl::at< typename nicta::vibe::image::detail::conversion_map<From, To>::c_type2
                                       , boost::mpl::pair<boost::mpl::int_<From>, boost::mpl::int_<To> >
                                       >::type type2;




        // the static_assert below guards us against no match ; however if [From == To], we return conversion_map<From, To>::void_ - we should NOT convert when [From == To] !!
        // (..chosen not to static_assert when [From == To]: rather in code we just clone another instance of the image, however we DO NO convert with cvCvtColor or similar!!)

        typedef
        typename boost::mpl::eval_if< typename boost::mpl::has_key< typename nicta::vibe::image::detail::conversion_map<From, To>::c_type1
                                                                  , boost::mpl::pair<boost::mpl::int_<From>, boost::mpl::int_<To> >
                                                                  >::type
                                    , typename nicta::vibe::image::detail::conversion_map<From, To>::type1
                                    , typename boost::mpl::eval_if< typename boost::mpl::has_key< typename nicta::vibe::image::detail::conversion_map<From, To>::c_type2
                                                                                                , boost::mpl::pair<boost::mpl::int_<From>, boost::mpl::int_<To> >
                                                                                                >::type
                                                                  , typename nicta::vibe::image::detail::conversion_map<From, To>::type2
                                                                  , typename boost::mpl::eval_if
                                                                               < boost::is_same<boost::mpl::int_<From>, boost::mpl::int_<To> >
                                                                               , boost::mpl::identity<typename nicta::vibe::image::detail::conversion_map<From, To>::void_::type>
                                                                               , boost::mpl::identity<typename nicta::vibe::image::detail::conversion_map<From, To>::error_::type>
                                                                               >::type
                                                                  >::type
                                    >::type type;



        BOOST_STATIC_ASSERT((static_cast<int> (nicta::vibe::image::detail::conversion_map<From, To>::type::value) !=
                             static_cast<int> ( nicta::vibe::image::detail::conversion_map<From, To>::error_::value)));
    };

} } } } // namespace nicta::vibe::image::detail


#endif
