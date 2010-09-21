/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_IMAGE_DEPTH_H
#define NICTA_VIBE_IMAGE_IMAGE_DEPTH_H

#include <boost/mpl/at.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>

#include <opencv/cxtypes.h>


namespace nicta {
namespace vibe {
namespace image {

    template <typename T, long int depth>
    struct depth_t
    {
        typedef T type;

        /*
        enum
        {
            value = depth
        };
        */

        static const long int value;
    };
    template <typename T, long int depth>
    const long int nicta::vibe::image::depth_t<T, depth>::value = depth;


    struct depth_8u : public depth_t <unsigned char, IPL_DEPTH_8U>
    {
        enum
        {
            depth = 8
        };
    };


    struct depth_8s : public depth_t <signed char, IPL_DEPTH_8S>
    {
        enum
        {
            depth = 8
        };
    };


    struct depth_16u : public depth_t <unsigned short, IPL_DEPTH_16U>
    {
        enum
        {
            depth = 16
        };
    };


    struct depth_16s : public depth_t <signed short, IPL_DEPTH_16S>
    {
        enum
        {
            depth = 16
        };
    };


    struct depth_32s : public depth_t <int, IPL_DEPTH_32S>
    {
        enum
        {
            depth = 32
        };
    };


    struct depth_32f : public depth_t <float, IPL_DEPTH_32F>
    {
        enum
        {
            depth = 32
        };
    };


    struct depth_64f : public depth_t <double, IPL_DEPTH_64F>
    {
        enum
        {
            depth = 64
        };
    };



    template <typename T>
    struct depth
    {
        typedef boost::mpl::map< boost::mpl::pair<unsigned char, nicta::vibe::image::depth_8u>
                               , boost::mpl::pair<signed char, nicta::vibe::image::depth_8s>
                               , boost::mpl::pair<unsigned short, nicta::vibe::image::depth_16u>
                               , boost::mpl::pair<signed short, nicta::vibe::image::depth_16s>
                               , boost::mpl::pair<int, nicta::vibe::image::depth_32s>
                               , boost::mpl::pair<float, nicta::vibe::image::depth_32f>
                               , boost::mpl::pair<double, nicta::vibe::image::depth_64f>
                               > m_type;

        typedef typename boost::mpl::at<m_type, T>::type type;
    };

} } } // namespace nicta::vibe::image


#endif
