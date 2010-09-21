/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "ocv_converters/as_image.h"
#include "ocv_converters/as_matrix.h"

#include <image/image.h>
#include <image/image_depth.h>
#include <image/image_pixel.h>
#include <image/window.h>
#include <nicta_common/nicta_exception.h>
#include <nicta_common/quantity.h>
#include <ocv_matrix/ocv_matrix.h>

#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>



using namespace nicta;
using namespace nicta::vibe::image;
using namespace nicta::ocv::matrix;
using namespace nicta::vibe::image::display;


int main(int argc, char** argv)
{
    try
    {
        image< > img0("file1.jpeg");                              // open bgr_8 image file, and display
        img0.show("img0");

        boost::shared_ptr<matrix<unsigned char, 2, 3, 2> > MTX = nicta::ocv::convert::as_matrix<unsigned char, bgr_p>(img0);
        unsigned int rows = MTX->rows();
        unsigned int cols = MTX->columns();
        unsigned int channels = MTX->channels();
        std::cout << std::endl << std::endl << "[mtx-rows, mtx-cols, mtx-channels] = " << rows << " x " << cols << " : " << channels;

        boost::shared_ptr<image< > > IMG = nicta::ocv::convert::as_image<depth_8u, bgr_p>(*MTX);
        unsigned int height = IMG->height().operator()< >();
        unsigned int width  = IMG->width().operator()< >();
        unsigned int components = IMG->components;
        std::cout << std::endl << "[img-height, img-width, img-components] = " << height << " x " << width << " : " << components<< std::endl << std::endl;

        IMG->show("IMG->MTX->IMG");
        wait_key(boost::posix_time::seconds(0));                  // show image, and wait for any keypress
    }
    catch (const nicta_exception& e)
    {
        std::cerr << e.nice_what();
        exit(1);
    }

    return 0;
}
