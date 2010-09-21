/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_ALGORITHM_DEINTERLACE_POINTGRAY_H
#define NICTA_VIBE_VIDEO_ALGORITHM_DEINTERLACE_POINTGRAY_H

# ifdef NICTA_PGR


#include "video/algorithm/deinterlace_identity.h"
#include "video/vendor/pgr/pgr.h"

#include <image/detail/conversion_assocmap.h>
#include <image/detail/iplimage_deleter.h>
#include <image/image_depth.h>
#include <image/image_pixel.h>
#include <nicta_common/nicta_exception.h>

#include <flycapture/Error.h>
#include <flycapture/Image.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/cxtypes.h>

#include <boost/array.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/int.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <string>

namespace nicta {
namespace vibe {
namespace video {
namespace algorithm {

    /* this generic definition accommodates n-camera Pointgray setups.. by adding op()()'s as appropriate! */
    template < unsigned int Payload
             , typename Depth = nicta::vibe::image::depth_8u
             , template <typename _> class Pixel = nicta::vibe::image::bgr_p
             >
    class deinterlace_pointgray : public nicta::vibe::video::algorithm::deinterlace_identity<Payload, Depth, Pixel>
    {
    public:

        deinterlace_pointgray(const char* bayer_tiling) throw ();


        typename boost::enable_if< boost::mpl::equal_to< boost::mpl::int_<Payload>
                                                       , boost::mpl::int_<2>
                                                       >
                                 , nicta::vibe::video::algorithm::deinterlace_pointgray<Payload, Depth, Pixel> &
                                 >::type
        operator()(FlyCapture2::Image* frame) throw ();


        template <typename T>
        nicta::vibe::video::algorithm::deinterlace_identity<Payload, Depth, Pixel> &
        operator()(T* frame) throw (nicta::nicta_exception)
        {
            return this->nicta::vibe::video::algorithm::deinterlace_identity<Payload, Depth, Pixel>::template operator()<T>(frame);
        }


    private:

        const std::string bayer_tiling_;
    };

} } } }  // namespace nicta::vibe::video::algorithm


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < unsigned int Payload
         , typename T
         , template <typename _> class U
         >
nicta::vibe::video::algorithm::deinterlace_pointgray<Payload, T, U> ::
deinterlace_pointgray(const char* bayer_tiling) throw ()
: bayer_tiling_(bayer_tiling)
{
    // Constructor
}


template < unsigned int Payload
         , typename T
         , template <typename _> class U
         >
typename boost::enable_if< boost::mpl::equal_to< boost::mpl::int_<Payload>
                                               , boost::mpl::int_<2>
                                               >
                         , nicta::vibe::video::algorithm::deinterlace_pointgray<Payload, T, U> &
                         >::type
nicta::vibe::video::algorithm::deinterlace_pointgray<Payload, T, U> :: 
operator()(FlyCapture2::Image* frame) throw ()
{
    // Deinterlace PointGray (Bumblebee) stereo images - THESE ARE BAYER IMAGES!!
    //
    // NOTE:  input image is 16-bit byte-inteleaved: upper byte is the Right image ; lower byte is the Left image
    // NOTE:  Little-endian systems byte-order is reversed! (ie. upper byte is at the end, and lower-byte is at the front)
    // NOTE:  obviously, width & height of input frame, left, right images are the same!, only the bit-depth varies!
    // NOTE:  we *MUST* push_back  [ boost::shared_ptr< ::IplImage > ] types!

    BOOST_STATIC_ASSERT((boost::is_same<T, nicta::vibe::image::depth_8u>::type::value));

    FlyCapture2::Image frame_copy;
    FlyCapture2::Error fe = frame_copy.DeepCopy(frame);
    nicta::vibe::video::pgr::pgr::check_pgr(fe, "DeepCopy");

    unsigned int cols     = frame_copy.GetCols();
    unsigned int rows     = frame_copy.GetRows();
    //unsigned int bpp      = frame_copy.GetBitsPerPixel();        // bpp / 8 == Payload (2U)
    unsigned char* iframe = frame_copy.GetData();


    boost::shared_ptr< ::IplImage > bayer_left( ::cvCreateImage(::cvSize(cols, rows), static_cast<int> (T::value), 1)
                                              , nicta::vibe::image::detail::iplimage_deleter());

    boost::shared_ptr< ::IplImage > bayer_right( ::cvCreateImage(::cvSize(cols, rows), static_cast<int> (T::value), 1)
                                               , nicta::vibe::image::detail::iplimage_deleter());

    unsigned int idx = 0;
    for (unsigned int i = 0; i != rows; ++i)
    {
        for (unsigned int j = 0; j != cols; ++j)
        {
            // lower 8 bytes:  (the camera's left)
            reinterpret_cast<nicta::vibe::image::depth_8u::type *> (bayer_left->imageData + i * bayer_left->widthStep)[j] =
                                                                                                               static_cast<nicta::vibe::image::depth_8u::type> (iframe[idx++]);

            // upper 8 bytes:  (the camera's right)
            reinterpret_cast<nicta::vibe::image::depth_8u::type *> (bayer_right->imageData + i * bayer_right->widthStep)[j] =
                                                                                                               static_cast<nicta::vibe::image::depth_8u::type> (iframe[idx++]);
        }
    }


    // we need to convert on color space of the image pairs..
    std::string tile(this->bayer_tiling_);

    boost::shared_ptr< ::IplImage > left_3( ::cvCreateImage(::cvSize(cols, rows), static_cast<int> (T::value), 3)
                                          , nicta::vibe::image::detail::iplimage_deleter());

    boost::shared_ptr< ::IplImage > right_3( ::cvCreateImage(::cvSize(cols, rows), static_cast<int> (T::value), 3)
                                           , nicta::vibe::image::detail::iplimage_deleter());

    int id = nicta::vibe::image::detail::conversion_assocMap(tile + std::string("->BGR"));                            // there are limited conversions from Bayer types, pick one!
    ::cvCvtColor(&*bayer_left, &*left_3, id);
    ::cvCvtColor(&*bayer_right, &*right_3, id);


    if (std::string(U<T>::pix_t) == "BGR")
    {
        // no futher conversions necessary

        this->nicta::vibe::video::algorithm::deinterlace_identity<2U, T, U>::push_back(left_3);
        this->nicta::vibe::video::algorithm::deinterlace_identity<2U, T, U>::push_back(right_3);
    }
    else
    {
        // convert from BGR to sought color space (..possibly even monochrome!)

        boost::shared_ptr< ::IplImage > left( ::cvCreateImage(::cvSize(cols, rows), static_cast<int> (T::value), static_cast<int> (U<T>::components))
                                            , nicta::vibe::image::detail::iplimage_deleter());

        boost::shared_ptr< ::IplImage > right( ::cvCreateImage(::cvSize(cols, rows), static_cast<int> (T::value), static_cast<int> (U<T>::components))
                                             , nicta::vibe::image::detail::iplimage_deleter());

        int id = nicta::vibe::image::detail::conversion_assocMap(std::string("BGR->") + std::string(U<T>::pix_t));    // ..now convert from BGR to sought color space
        ::cvCvtColor(&*left_3, &*left, id);
        ::cvCvtColor(&*right_3, &*right, id);

        this->nicta::vibe::video::algorithm::deinterlace_identity<2U, T, U>::push_back(left);
        this->nicta::vibe::video::algorithm::deinterlace_identity<2U, T, U>::push_back(right);
    }
    return *this;
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


# endif  /* NICTA_PGR */

#endif
