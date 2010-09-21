/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "image_header/image_header.h"

#include <nicta_common/nicta_exception.h>

#include <opencv/highgui.h>
#include <opencv/grfmt_base.h>
#include <opencv/grfmt_bmp.h>
#include <opencv/grfmt_exr.h>
#include <opencv/grfmt_jpeg.h>
#include <opencv/grfmt_jpeg2000.h>
#include <opencv/grfmt_png.h>
#include <opencv/grfmt_pxm.h>
#include <opencv/grfmt_sunras.h>
#include <opencv/grfmt_tiff.h>
#include <opencv/loadsave.h>

#include <iostream>
#include <string>


nicta::vibe::header::image_header :: 
image_header(const char* filename) throw (nicta::nicta_exception)
: filename_(filename)
{
    // Return image file header info

    // loadsave.cpp
    ::CvImageFilters imgf;
    ::GrFmtReader* grfmt_rdr = imgf.FindReader(filename);
    if (!grfmt_rdr)
    {
        throw nicta::nicta_exception("Failed to open file: " + std::string(filename), "image_header", "image_header");
    }

    //grfmt_base.h, etc
    if (!grfmt_rdr->ReadHeader())
    {
        throw nicta::nicta_exception("Failed to read image file header", "image_header", "image_header");
    }

    this->depth_       = grfmt_rdr->GetDepth();
    this->width_       = grfmt_rdr->GetWidth();
    this->height_      = grfmt_rdr->GetHeight();
    this->is_color_    = grfmt_rdr->IsColor();
    this->is_integral_ = !grfmt_rdr->IsFloat();
}


int
nicta::vibe::header::image_header :: 
depth() const throw ()
{
    // Return image color depth (usu. 8)

    return this->depth_;
}


int
nicta::vibe::header::image_header :: 
width() const throw ()
{
    // Return image width

    return this->width_;
}


int
nicta::vibe::header::image_header :: 
height() const throw ()
{
    // Return image height

    return this->height_;
}


bool
nicta::vibe::header::image_header :: 
is_color() const throw ()
{
    // Return true if image is a color image

    return this->is_color_;
}


bool
nicta::vibe::header::image_header :: 
is_integral() const throw ()
{
    // Return true if image data is a Non-floating-pt data type

    return this->is_integral_;
}


const char*
nicta::vibe::header::image_header :: 
filename() const throw ()
{
    // Return filename associated with image_header

    return this->filename_;
}

