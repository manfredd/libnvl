/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_PGR_CAMERA_DEVICE_H
#define NICTA_VIBE_VIDEO_PGR_CAMERA_DEVICE_H

# ifdef NICTA_PGR


#include "video/vendor/pgr/pgr.h"

#include <flycapture/BusManager.h>
#include <flycapture/Camera.h>
#include <flycapture/Error.h>
#include <flycapture/FlyCapture2Defs.h>

#include <image/image_pixel.h>
#include <image/detail/conversion_assocmap.h>
#include <nicta_common/nicta_exception.h>
#include <nicta_common/quantity.h>
#include <static_association/static_association.h>

#include <cstring>
#include <string>


namespace nicta {
namespace vibe {
namespace video {
namespace pgr {

    template <unsigned short int CameraIndex = 0>
    class camera_device
    {
    public:

        enum
        {
            index = static_cast<int> (CameraIndex)                    // index = nth PGR camera
        };


        ~camera_device() throw ();

        camera_device() throw (nicta::nicta_exception);
        camera_device(nicta::quantity::frame_rate_per_second<unsigned int> fps) throw (nicta::nicta_exception);

        double frame_rate() const throw ();
        const char* bayer_tiling() const throw ();


    private:

        static
        nicta::association<unsigned int, const char *> bayer_tileArr[];

        static
        nicta::static_association<unsigned int, const char *> bayer_tileMap;


        static
        const char* bayer_tiling_(FlyCapture2::Camera& stream) throw ();


        FlyCapture2::Camera& stream_() throw ();


        FlyCapture2::BusManager bm_;
        FlyCapture2::PGRGuid guid_;
        FlyCapture2::Camera stream_capture_;

        double fps_;
        const volatile char* tiling_;


        template < typename T
                 , template <typename _> class U
                 >
        friend
        class video_server;
    };

} } } } // namespace nicta::vibe::video::pgr


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


/* Initialize static data members */

// [ NOTE:  for futher information, see method  bayer_tiling_(Camera&)  -  below.. ]
//
template <unsigned short int I>
nicta::association<unsigned int, const char *>
nicta::vibe::video::pgr::camera_device<I> :: bayer_tileArr[] = { { 0x59595959 , nicta::vibe::image::gray_p< >::pix_t    },    // YYYY           // Gray
                                                                 { 0x47425247 , nicta::vibe::image::BayerGR_p< >::pix_t },    // GBRG           // BayerGR
                                                                 { 0x47424752 , nicta::vibe::image::BayerGR_p< >::pix_t },    // GBGR
                                                                 { 0x47524247 , nicta::vibe::image::BayerGB_p< >::pix_t },    // GRBG           // BayerGB
                                                                 { 0x47524742 , nicta::vibe::image::BayerGB_p< >::pix_t },    // GRGB
                                                                 { 0x42474752 , nicta::vibe::image::BayerBG_p< >::pix_t },    // BGGR           // BayerBG
                                                                 { 0x52474247 , nicta::vibe::image::BayerBG_p< >::pix_t },    // RGBG
                                                                 { 0x52474742 , nicta::vibe::image::BayerRG_p< >::pix_t },    // RGGB           // BayerRG
                                                                 { 0x42475247 , nicta::vibe::image::BayerRG_p< >::pix_t } };  // BGRG


template <unsigned short int I>
nicta::static_association<unsigned int, const char *>
nicta::vibe::video::pgr::camera_device<I> :: bayer_tileMap
             (nicta::vibe::video::pgr::camera_device<I>::bayer_tileArr,
              sizeof(nicta::vibe::video::pgr::camera_device<I>::bayer_tileArr) / sizeof(nicta::vibe::video::pgr::camera_device<I>::bayer_tileArr[0]));



template <unsigned short int I>
nicta::vibe::video::pgr::camera_device<I> :: 
~camera_device() throw ()
{
    // Destructor

    this->stream_capture_.StopCapture();
    this->stream_capture_.Disconnect();
}


template <unsigned short int I>
nicta::vibe::video::pgr::camera_device<I> :: 
camera_device() throw (nicta::nicta_exception)
: fps_(0)
, tiling_(0)
{
    // Constructor

    unsigned int count = 0;
    FlyCapture2::Error fe = this->bm_.GetNumOfCameras(&count);
    nicta::vibe::video::pgr::pgr::check_pgr(fe, "GetNumOfCameras");

    // camera_index: [0, count)
    if (!count || static_cast<unsigned int> (I) >= count)
    {
        throw nicta::nicta_exception("Index out of bounds:  sought to connect to non-extant PGR camera");
    }

    fe = this->bm_.GetCameraFromIndex(static_cast<unsigned int> (I), &this->guid_);
    nicta::vibe::video::pgr::pgr::check_pgr(fe, "GetCameraFromIndex");

    nicta::vibe::video::pgr::pgr::create_camera_capture(this->stream_capture_, this->guid_);

    this->fps_    = nicta::vibe::video::pgr::pgr::query_frame_rate(this->stream_capture_);
    this->tiling_ = nicta::vibe::video::pgr::camera_device<I>::bayer_tiling_(this->stream_capture_);
}


template <unsigned short int I>
nicta::vibe::video::pgr::camera_device<I> ::
camera_device(nicta::quantity::frame_rate_per_second<unsigned int> fps) throw (nicta::nicta_exception)
: fps_(0)
, tiling_(0)
{
    // Constructor

    unsigned int count = 0;
    FlyCapture2::Error fe = this->bm_.GetNumOfCameras(&count);
    nicta::vibe::video::pgr::pgr::check_pgr(fe, "GetNumOfCameras");

    // camera_index: [0, count)
    if (!count || static_cast<unsigned int> (I) >= count)
    {
        throw nicta::nicta_exception("Index out of bounds:  sought to connect to non-extant PGR camera");
    }

    fe = this->bm_.GetCameraFromIndex(static_cast<unsigned int> (I), &this->guid_);
    nicta::vibe::video::pgr::pgr::check_pgr(fe, "GetCameraFromIndex");

    nicta::vibe::video::pgr::pgr::create_camera_capture(this->stream_capture_, this->guid_);
    if (fps.operator()<double>() != 0)
    {
        FlyCapture2::Property property;
        property.type           = FlyCapture2::FRAME_RATE;     // property to be configured is frame-rate
        property.absControl     = true;                        // if want to set anything manually, always use absoute values
        property.autoManualMode = false;                       // we are doing a manual setting, so automatic-mode = false
        property.onOff          = true;                        // if set to false, the frame rate won't be used and acquisition speed will be depdt on shutter & packet size soley
        property.absValue = static_cast<float>(this->fps_);    // set to another fps

        FlyCapture2::Error fe = this->stream_capture_.SetProperty(&property);
        nicta::vibe::video::pgr::pgr::check_pgr(fe, "SetProperty");
    }
    this->fps_    = nicta::vibe::video::pgr::pgr::query_frame_rate(this->stream_capture_);
    this->tiling_ = nicta::vibe::video::pgr::camera_device<I>::bayer_tiling_(this->stream_capture_);
}


template <unsigned short int I>
double
nicta::vibe::video::pgr::camera_device<I> :: 
frame_rate() const throw ()
{
    // Return camera frame-rate

    return this->fps_;
}


template <unsigned short int I>
const char *
nicta::vibe::video::pgr::camera_device<I> :: 
bayer_tiling() const throw ()
{
    // Return the Bayer-tiling associated with this camera context:  { "GRAY" ; "BayerBG" ; "BayerGB" ; "BayerRG" ; "BayerGR" }

    std::string::size_type len = std::strlen(const_cast<const char*>(this->tiling_));
    std::string s(len, 0);
    for (std::string::size_type i = 0; i != len; ++i)
    {
        s[i] = this->tiling_[i];
    }
    return s.c_str();
}


template <unsigned short int I>
const char *
nicta::vibe::video::pgr::camera_device<I> :: 
bayer_tiling_(FlyCapture2::Camera& stream) throw ()
{
    // Return the Bayer-tiling associated with this camera context:  { "GRAY" ; "BayerBG" ; "BayerGB" ; "BayerRG" ; "BayerGR" }
    // p54:  PGR Digital Camera Register Reference.pdf
    // Blue(B): 0x42 ; Green(G): 0x47 ; Red(R): 0x52 ; Monochrome(Y) 0x59
    // Querying the Bayer-tiling we get output:  0xC1C2C3C4...  example:  0x52474742 is RGGB  ;  0x59595959 is YYYY (monochrome)
    // NOTE:  register address 0x1040 is address on all PGR cameras to query the BAYER_TILE_MAPPING register

    unsigned int bayer_tile_register = 0x1040;    // BAYER_TILE_MAPPING
    unsigned int bayer_tile = nicta::vibe::video::pgr::pgr::query_register(stream, bayer_tile_register);

    /*
    unsigned int C1 = (bayer_tile >> 24) & 0xFF;
    unsigned int C2 = (bayer_tile >> 16) & 0xFF;
    unsigned int C3 = (bayer_tile >> 8) & 0xFF;
    unsigned int C4 = (bayer_tile) & 0xFF;
    */

    return nicta::vibe::video::pgr::camera_device<I>::bayer_tileMap(bayer_tile);
}


template <unsigned short int I>
FlyCapture2::Camera &
nicta::vibe::video::pgr::camera_device<I> :: 
stream_() throw ()
{
    // Return reference to this camera stream

    return this->stream_capture_;
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


# endif  /* NICTA_PGR */

#endif
