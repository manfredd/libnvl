/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_DISPLAY_SCRIBE_H
#define NICTA_VIBE_IMAGE_DISPLAY_SCRIBE_H

#include "image/image.h"
#include "image/native_type.h"

#include <nicta_common/quantity.h>

#include <opencv/cxcore.h>
#include <opencv/cxtypes.h>

#include <boost/noncopyable.hpp>


namespace nicta {
namespace vibe {
namespace image {
namespace display {

    enum LineType
    {
        Bresenham_4 = 4     ,
        Bresenham_8 = 8     ,
        AntiAliased = CV_AA     // Guassian Filtering used
    };

    enum HersheyFont
    {
        simplex           = CV_FONT_HERSHEY_SIMPLEX                         ,
        plain             = CV_FONT_HERSHEY_PLAIN                           ,
        duplex            = CV_FONT_HERSHEY_DUPLEX                          ,
        complex           = CV_FONT_HERSHEY_COMPLEX                         ,
        triplex           = CV_FONT_HERSHEY_TRIPLEX                         ,
        complex_small     = CV_FONT_HERSHEY_COMPLEX_SMALL                   ,
        script_simplex    = CV_FONT_HERSHEY_SCRIPT_SIMPLEX                  ,
        script_complex    = CV_FONT_HERSHEY_SCRIPT_COMPLEX                  ,

        it_simplex        = CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC        ,
        it_plain          = CV_FONT_HERSHEY_PLAIN | CV_FONT_ITALIC          ,
        it_duplex         = CV_FONT_HERSHEY_DUPLEX | CV_FONT_ITALIC         ,
        it_complex        = CV_FONT_HERSHEY_COMPLEX | CV_FONT_ITALIC        ,
        it_triplex        = CV_FONT_HERSHEY_TRIPLEX | CV_FONT_ITALIC        ,
        it_complex_small  = CV_FONT_HERSHEY_COMPLEX_SMALL | CV_FONT_ITALIC  ,
        it_script_simplex = CV_FONT_HERSHEY_SCRIPT_SIMPLEX | CV_FONT_ITALIC ,
        it_script_complex = CV_FONT_HERSHEY_SCRIPT_COMPLEX | CV_FONT_ITALIC 
    };


    class scribe : boost::noncopyable
    {
    public:

        explicit scribe(HersheyFont font_type,
                        double font_height    = 0.5,
                        double font_width     = 0.5,
                        unsigned int boldness = 1,
                        LineType ln_type      = nicta::vibe::image::display::AntiAliased) throw ();


        template < typename Depth
                 , template <typename _> class Pixel
                 >
        void operator()(nicta::vibe::image::image<Depth, Pixel>& image,
                        const char* text,
                        nicta::quantity::x_absolute_displacement<unsigned int> text_offset_x =
                               nicta::quantity::x_absolute_displacement<unsigned int> (10),
                        nicta::quantity::y_absolute_displacement<unsigned int> text_offset_y =
                               nicta::quantity::y_absolute_displacement<unsigned int> (10),
                        ::CvScalar color = CV_RGB(0, 255, 0)) const throw ();


    private:

        ::CvFont font_;
    };

} } } }  // namespace nicta::vibe::image::display


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


nicta::vibe::image::display::scribe :: 
scribe(nicta::vibe::image::display::HersheyFont font_type,
       double font_height,
       double font_width,
       unsigned int boldness,
       nicta::vibe::image::display::LineType ln_type) throw ()
{
    // Constructor - initialize the font

    double shear = 0;
    ::cvInitFont(&this->font_, static_cast<int> (font_type), font_height, font_width, shear, boldness, ln_type);
}


template < typename T
         , template <typename T_> class U
         >
void
nicta::vibe::image::display::scribe :: 
operator()(nicta::vibe::image::image<T, U>& image,
           const char* text,
           nicta::quantity::x_absolute_displacement<unsigned int> text_offset_x,
           nicta::quantity::y_absolute_displacement<unsigned int> text_offset_y,
           ::CvScalar color) const throw ()
{
    // Draw text onto image
    // NOTE:  origins mark bottom left corner of text
    // NOTE:  origins are relative to BOTTOM-left-corner of image to ease correct placement of text!!
    // NOTE:  if image is for example gray_scale, then a color text cannot appear, however a gray-scaled representation will

    ::CvPoint text_origin = ::cvPoint(text_offset_x.operator()<int>(), image.height().operator()<int>() - text_offset_y.operator()<int>());
    ::cvPutText(static_cast< ::CvArr *> (image.template native_handle< >()), text, text_origin, &this->font_, color);
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
