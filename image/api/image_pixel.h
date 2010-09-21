/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_IMAGE_PIXEL_H
#define NICTA_VIBE_IMAGE_IMAGE_PIXEL_H

#include "image/image_depth.h"
#include "image/models/colorcode_model.h"


namespace nicta {
namespace vibe {
namespace image {

    template < typename Depth
             , template <typename _> class Pixel
             >
    struct pixel
    {
        typename Depth::type&
        operator()(unsigned int idx)
        {
            // works without packing/alignment issues b/c all types are the same
            return *(reinterpret_cast<typename Depth::type *> (static_cast<Pixel<Depth> *> (this)) + idx);
        }

        typename Depth::type
        operator()(unsigned int idx) const
        {
            // works without packing/alignment issues b/c all types are the same
            return *(reinterpret_cast<const typename Depth::type *> (static_cast<const Pixel<Depth> *> (this)) + idx);
        }


    protected:

        pixel() { }
    };

    template <typename Depth = nicta::vibe::image::depth_8u>
    struct gray_p : pixel<Depth, gray_p>
    {
        typedef Depth depth;
        typedef typename Depth::type type;

        typename Depth::type pixel;

        static const int components = 1;
        static const char* pix_t;


        struct channel
        {
            enum coi
            {
                Gray = 0 
            };
        };

        enum
        {
            encoding = nicta::vibe::image::GRAY
        };
    };
    template <typename Depth>
    const char* gray_p<Depth>::pix_t("GRAY");


    template <typename Depth = nicta::vibe::image::depth_8u>
    struct rgb_p : pixel<Depth, rgb_p>
    {
        typedef Depth depth;
        typedef typename Depth::type type;

        typename Depth::type r;
        typename Depth::type g;
        typename Depth::type b;

        static const int components = 3;
        static const char* pix_t;


        struct channel
        {
            enum coi
            {
                R   =  0 ,
                G   =  1 ,
                B   =  2 ,
                RGB = -1 
            };
        };

        enum
        {
            encoding = nicta::vibe::image::RGB
        };
    };
    template <typename Depth>
    const char* rgb_p<Depth>::pix_t("RGB");


    template <typename Depth = nicta::vibe::image::depth_8u>
    struct bgr_p : pixel<Depth, bgr_p>
    {
        typedef Depth depth;
        typedef typename Depth::type type;

        typename Depth::type b;
        typename Depth::type g;
        typename Depth::type r;

        static const int components = 3;
        static const char* pix_t;


        struct channel
        {
            enum coi
            {
                B   =  0 ,
                G   =  1 ,
                R   =  2 ,
                BGR = -1 
            };
        };

        enum
        {
            encoding = nicta::vibe::image::BGR
        };
    };
    template <typename Depth>
    const char* bgr_p<Depth>::pix_t("BGR");


    template <typename Depth = nicta::vibe::image::depth_8u>
    struct rgba_p : pixel<Depth, rgba_p>
    {
        typedef Depth depth;
        typedef typename Depth::type type;

        typename Depth::type r;
        typename Depth::type g;
        typename Depth::type b;
        typename Depth::type alpha;

        static const int components = 4;
        static const char* pix_t;


        struct channel
        {
            enum coi
            {
                R    =  0 ,
                G    =  1 ,
                B    =  2 ,
                A    =  3 ,
                RGBA = -1 
            };
        };

        enum
        {
            encoding = nicta::vibe::image::RGBA
        };
    };
    template <typename Depth>
    const char* rgba_p<Depth>::pix_t("RGBA");


    template <typename Depth = nicta::vibe::image::depth_8u>
    struct bgra_p : pixel<Depth, bgra_p>
    {
        typedef Depth depth;
        typedef typename Depth::type type;

        typename Depth::type b;
        typename Depth::type g;
        typename Depth::type r;
        typename Depth::type alpha;

        static const int components = 4;
        static const char* pix_t;


        struct channel
        {
            enum coi
            {
                B    =  0 ,
                R    =  1 ,
                G    =  2 ,
                A    =  3 ,
                BGRA = -1 
            };
        };

        enum
        {
            encoding = nicta::vibe::image::BGRA
        };
    };
    template <typename Depth>
    const char* bgra_p<Depth>::pix_t("BGRA");


    template <typename Depth = nicta::vibe::image::depth_8u>
    struct xyz_p : pixel<Depth, xyz_p>
    {
        typedef Depth depth;
        typedef typename Depth::type type;

        typename Depth::type x;
        typename Depth::type y;
        typename Depth::type z;

        static const int components = 3;
        static const char* pix_t;


        struct channel
        {
            enum coi
            {
                X   =  0 ,
                Y   =  1 ,
                Z   =  2 ,
                XYZ = -1 
            };
        };

        enum
        {
            encoding = nicta::vibe::image::XYZ
        };
    };
    template <typename Depth>
    const char* xyz_p<Depth>::pix_t("XYZ");


    template <typename Depth = nicta::vibe::image::depth_8u>
    struct YCrCb_p : pixel<Depth, YCrCb_p>
    {
        typedef Depth depth;
        typedef typename Depth::type type;

        typename Depth::type Y;
        typename Depth::type Cr;
        typename Depth::type Cb;

        static const int components = 3;
        static const char* pix_t;


        struct channel
        {
            enum coi
            {
                Y     =  0 ,
                Cr    =  1 ,
                Cb    =  2 ,
                YCrCb = -1 
            };
        };

        enum
        {
            encoding = nicta::vibe::image::YCrCb
        };
    };
    template <typename Depth>
    const char* YCrCb_p<Depth>::pix_t("YCrCb");


    template <typename Depth = nicta::vibe::image::depth_8u>
    struct HSV_p : pixel<Depth, HSV_p>
    {
        typedef Depth depth;
        typedef typename Depth::type type;

        typename Depth::type H;
        typename Depth::type S;
        typename Depth::type V;

        static const int components = 3;
        static const char* pix_t;


        struct channel
        {
            enum coi
            {
                H   =  0 ,
                S   =  1 ,
                V   =  2 ,
                HSV = -1 
            };
        };

        enum
        {
            encoding = nicta::vibe::image::HSV
        };
    };
    template <typename Depth>
    const char* HSV_p<Depth>::pix_t("HSV");


    template <typename Depth = nicta::vibe::image::depth_8u>
    struct HLS_p : pixel<Depth, HLS_p>
    {
        typedef Depth depth;
        typedef typename Depth::type type;

        typename Depth::type H;
        typename Depth::type L;
        typename Depth::type S;

        static const int components = 3;
        static const char* pix_t;


        struct channel
        {
            enum coi
            {
                H   =  0 ,
                L   =  1 ,
                S   =  2 ,
                HLS = -1 
            };
        };

        enum
        {
            encoding = nicta::vibe::image::HLS
        };
    };
    template <typename Depth>
    const char* HLS_p<Depth>::pix_t("HLS");


    template <typename Depth = nicta::vibe::image::depth_8u>
    struct Lab_p : pixel<Depth, Lab_p>
    {
        typedef Depth depth;
        typedef typename Depth::type type;

        typename Depth::type L;
        typename Depth::type a;
        typename Depth::type b;

        static const int components = 3;
        static const char* pix_t;


        struct channel
        {
            enum coi
            {
                L   =  0 ,
                a   =  1 ,
                b   =  2 ,
                Lab = -1 
            };
        };

        enum
        {
            encoding = nicta::vibe::image::Lab
        };
    };
    template <typename Depth>
    const char* Lab_p<Depth>::pix_t("Lab");


    template <typename Depth = nicta::vibe::image::depth_8u>
    struct Luv_p : pixel<Depth, Luv_p>
    {
        typedef Depth depth;
        typedef typename Depth::type type;

        typename Depth::type L;
        typename Depth::type u;
        typename Depth::type v;

        static const int components = 3;
        static const char* pix_t;


        struct channel
        {
            enum coi
            {
                L   =  0 ,
                u   =  1 ,
                v   =  2 ,
                Luv = -1 
            };
        };

        enum
        {
            encoding = nicta::vibe::image::Luv
        };
    };
    template <typename Depth>
    const char* Luv_p<Depth>::pix_t("Luv");



    template <typename Depth>
    struct Bayer_p : pixel<Depth, Bayer_p>
    {
        typedef Depth depth;
        typedef typename Depth::type type;

        typename Depth::type pixel;

        // NOTE: each bayer pixel is one channel-type, but a bayer image has 3-channels!
        static const int components = 1;


        struct channel
        {
            enum coi
            {
                Bayer = 0 
            };
        };


    protected:

        Bayer_p() { }
    };


    template <typename Depth = nicta::vibe::image::depth_8u>
    struct BayerBG_p : Bayer_p<Depth>
    {
        static const char* pix_t;

        enum
        {
            encoding = nicta::vibe::image::BayerBG
        };
    };
    template <typename Depth>
    const char* BayerBG_p<Depth>::pix_t("BayerBG");


    template <typename Depth = nicta::vibe::image::depth_8u>
    struct BayerGB_p : Bayer_p<Depth>
    {
        static const char* pix_t;

        enum
        {
            encoding = nicta::vibe::image::BayerGB
        };
    };
    template <typename Depth>
    const char* BayerGB_p<Depth>::pix_t("BayerGB");


    template <typename Depth = nicta::vibe::image::depth_8u>
    struct BayerRG_p : Bayer_p<Depth>
    {
        static const char* pix_t;

        enum
        {
            encoding = nicta::vibe::image::BayerRG
        };
    };
    template <typename Depth>
    const char* BayerRG_p<Depth>::pix_t("BayerRG");


    template <typename Depth = nicta::vibe::image::depth_8u>
    struct BayerGR_p : Bayer_p<Depth>
    {
        static const char* pix_t;

        enum
        {
            encoding = nicta::vibe::image::BayerGR
        };
    };
    template <typename Depth>
    const char* BayerGR_p<Depth>::pix_t("BayerGR");

} } } // namespace nicta::vibe::image


#endif
