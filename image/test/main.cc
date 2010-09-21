/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "image/image.h"
#include "image/algorithm/identity_translation.h"
#include "image/algorithm/unsigned_translation.h"
#include "image/models/destination_model.h"
#include "image/models/flip_model.h"
#include "image/models/interpln_model.h"
#include "image/models/window_model.h"
#include "image/models/write_model.h"
#include "image/native_type.h"
#include "image/scribe.h"
#include "image/splice_type.h"
#include "image/window.h"
#include "image_header/image_header.h"
#include "nicta_common/nicta_exception.h"
#include "nicta_common/quantity.h"

#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>



using namespace nicta;
using namespace nicta::quantity;
using namespace nicta::vibe::image;
using namespace nicta::vibe::image::algorithm;
using namespace nicta::vibe::image::display;
using namespace nicta::vibe::header;


int main(int argc, char** argv)
{
    try
    {
        const image< > img(width<unsigned int> (200), height<unsigned int> (300), 255);
        //
        // alternatively:
        // const image<depth<unsigned char>::type, bgr_p> img(width<unsigned int> (200), height<unsigned int> (300), 255);
        //


        if (img.width() != img.ROI_width())
        {
            throw nicta::nicta_exception("img.width() != img.ROI_width()", "main.cc");
        }
        if (img.height() != img.ROI_height())
        {
            throw nicta::nicta_exception("img.height() != img.ROI_height()", "main.cc");
        }

        img.show("initialized",
                 x_absolute_displacement<unsigned int> (30),
                 y_absolute_displacement<unsigned int> (60));

        // open bgr_8 image file & display
        const char* fyle1 = "file1.jpeg";
        image< > img1 (fyle1);

        scribe scribble(it_simplex);
        scribble(img1, "Hello World!");

        img1.show("img1",                                            // won't 'show' until either do a timed_show, or wait_key
                  x_absolute_displacement<unsigned int> (800),
                  y_absolute_displacement<unsigned int> (200));

        image< > n_img(img1.native_handle< >());
        n_img.show("Native Copy");

        // (dangerous!) cast to opencv type & get meta-data
        ::IplImage* const * rp_img1 = reinterpret_cast< ::IplImage* const * > (&img1);

        std::cout << std::endl << "Dimensions [img1]: HxW = " << img1.height() << "  " << img1.width() << std::endl;
        std::cout << std::endl << "Dimensions [rp_img1]: HxW = " << (*rp_img1)->height << "  " << (*rp_img1)->width << std::endl;


        // copy raw IplImage type
        const image< > img0 (*rp_img1);


        boost::shared_ptr<nicta::vibe::image::splice_type< >::type> spl = img1.splice();
        spl->get<bgr_p<depth_8u>::channel::B>()->show("SPLICE_BLUE");
        spl->get<bgr_p<depth_8u>::channel::G>()->show("SPLICE_GREEN");
        spl->get<bgr_p<depth_8u>::channel::R>()->show("SPLICE_RED");

        image< > imgMERGE(*spl->get<bgr_p<depth_8u>::channel::B>(),
                          *spl->get<bgr_p<depth_8u>::channel::G>(),
                          *spl->get<bgr_p<depth_8u>::channel::R>());
        imgMERGE.show("MERGED BGR");


        img0.timed_show("img0",                                      // show img0 and wait for 5000 ms - will also show all 'queued' images
                        boost::posix_time::seconds(5),
                        x_absolute_displacement<unsigned int> (444),
                        y_absolute_displacement<unsigned int> (777));



        // cast (color conversion):  bgr_8u -> gray_8u
        const boost::shared_ptr<const image<depth_8u, gray_p> > grayImg1 = img1.convert<gray_p>();
        grayImg1->show();

        // (dangerous!) cast to opencv type & get meta-data
        ::IplImage* const * rp_grayImg1 = reinterpret_cast< ::IplImage * const * > (&*grayImg1);
        std::cout << std::endl << "Dimensions [grayImg1]: HxW = " << grayImg1->height() << "  " << grayImg1->width() << std::endl;
        std::cout << std::endl << "Dimensions [rp_grayImg1]: HxW = " << (*rp_grayImg1)->height << "  " << (*rp_grayImg1)->width << std::endl;


        // cast (depth conversion):  bgr_8u -> bgr_16u ; applying identity_translation on depth
        boost::shared_ptr<const image<depth_16u, bgr_p> > bgrImg2 = img1.convert<depth_16u, identity_translation>();
        bgrImg2->show();


        // cast (depth conversion):  bgr_8u -> bgr_16u ; applying unsigned_translation on depth
        boost::shared_ptr<const image<depth_16u, bgr_p> > bgrImg3 = img1.convert<depth_16u, unsigned_translation>();
        bgrImg3->show();


        // logical comparisons
        // std::cout << std::endl << "IMAGES EQUAL? [img1,bgrImg2]:  " << (img1 == *bgrImg2) << std::endl
        // std::cout << std::endl << "IMAGES EQUAL? [img1,bgrImg3]:  " << (img1 == *bgrImg3) << std::endl
        //std::cout << std::endl << "IMAGES EQUAL? [img1,grayImg1]: " << (img1 == *grayImg1) << std::endl;
        std::cout << std::endl << "IMAGES EQUAL? [img1,img0]: " << (img1 == img0) << std::endl;



        // cast (color & depth conversion):  bgr_8u -> gray_16u ; applying unsigned_translation on depth
        boost::shared_ptr<const image<depth_16u, gray_p> > grayImg4 = img1.convert<depth_16u, gray_p, unsigned_translation>();
        grayImg4->show();


        // save as bgr_8u color image
        grayImg1->save_as<asColor>("file3.jpg");


        // open bgr_8u image file, applying identity_translation on depth
        //   (ie. saying will allow implicit conversions here, though none is done here)
        const image< > img_grayImg1 ("file3.jpg", identity_translation());
        img_grayImg1.show();



        // save to file
        img_grayImg1.save_as< >("file4.jpg");


        // query file header
        image_header hd("file4.jpg");
        std::cout << std::endl << "File METADATA:  [depth] = " << hd.depth() << 
                                                 " [color] = " << hd.is_color() << 
                                                 " [integral_type] = " << hd.is_integral() << std::endl;

        // query image class metadata
        std::cout << std::endl << "image<depth_8u, bgr_p> METADATA:  [depth] = " << image< >::depth <<
                                                                   " [color] = " << !image< >::monochrome <<
                                                                   " [integral_type] = " << image< >::is_integral << std::endl;



        // pixel access
        std::cout << std::endl << "BGR output for corner 3x3 pixels [img1], at origin(0, 0)";
        for (int i = 0; i != 3; ++i)
        {
            std::cout << std::endl << "Row [" << i << "]: " << std::endl;
            for (int j = 0; j != 3; ++j)
            {
                int b = static_cast<int> (img1[i][j].b);
                int g = static_cast<int> (img1[i][j].g);
                int r = static_cast<int> (img1[i][j].r);
                std::cout <<  "Col " << j << ": (" << b << ", " << g << ", " << r << ")" << std::endl;
                std::cout <<  "Col " << j << ": (" << static_cast<int> (img1[i][j](0)) << ", "
                                                   << static_cast<int> (img1[i][j](1)) << ", "
                                                   << static_cast<int> (img1[i][j](2)) << ")" << std::endl;
            }
            std::cout << std::endl;
        }
        img1.set_ROI(nicta::quantity::x_origin<unsigned int> (329),
                     nicta::quantity::y_origin<unsigned int> (98),
                     nicta::quantity::width<unsigned int> (3),
                     nicta::quantity::height<unsigned int> (3));
        std::cout << std::endl << "BGR output for 3x3 pixels in [img1], at: ROI-origin(329, 98)";
        for (int i = 0; i != 3; ++i)
        {
            std::cout << std::endl << "Row [" << i << "]: " << std::endl;
            for (int j = 0; j != 3; ++j)
            {
                int b = static_cast<int> (img1(i,j,0));
                int g = static_cast<int> (img1(i,j,1));
                int r = static_cast<int> (img1(i,j,2));
                std::cout <<  "Col " << j << ": (" << b << ", " << g << ", " << r << ")" << std::endl;
            }
            std::cout << std::endl;
        }
        img1.clear_ROI();



        std::cout << std::endl << "IMAGES EQUAL [img1,img_grayImg1]: " << (img1 == img_grayImg1) << std::endl;


        // make image assignment
        //img1 = img_grayImg1;                        // this is Ok
        assign(img1, img_grayImg1).show();            // this is also Ok


        std::cout << std::endl << "IMAGES EQUAL [img1,img_grayImg1]: " << (img1 == img_grayImg1) << std::endl;




        // won't be equal b/c rhs is truly in range depth_16u (did unsigned_translation earlier),
        //   and we are shrinking it to depth_8u -it's not going to fit

        // std::cout << std::endl << "IMAGES EQUAL [img1,bgrImg3]: " << (img1 == *bgrImg3) << std::endl;

        assign<identity_translation>(img1, *bgrImg3).show();

        // std::cout << std::endl << "IMAGES EQUAL [img1,bgrImg3]: " << (img1 == *bgrImg3) << std::endl;



        // std::cout << std::endl << "IMAGES EQUAL [img1,bgrImg3]: " << (img1 == *grayImg4) << std::endl;

        assign<identity_translation>(img1, *grayImg4).show();

        // std::cout << std::endl << "IMAGES EQUAL [img1,bgrImg3]: " << (img1 == *grayImg4) << std::endl << std::endl;




        // copy-like, and use unsigned_translation for any depth conversions

        // 2/3 of image will be blacked out (obviously!) .. gray_p -> bgr_p (what do you expect!)
        image< > imgX1(*grayImg4, unsigned_translation());
        imgX1.show();

        // 2/3 of image will be blacked out (obviously!) .. gray_p -> bgr_p (what do you expect!)
        image< > imgX2(*bgrImg2, unsigned_translation());
        imgX2.show();

        // 2/3 of image will be blacked out (obviously!) .. gray_p -> bgr_p (what do you expect!)
        image< > imgX3(*grayImg1, unsigned_translation());
        imgX3.show();

        // something more sane:  bgr_p -> gray_p ... all good!
        image<depth_8u, gray_p> sane(img1, unsigned_translation());
        sane.show("sane");


        // select sub-region & display
        image< > (fyle1).select(x_origin<unsigned int> (90),
                                y_origin<unsigned int> (100))->show();


        // select sub-region & display
        image< > imgF(fyle1);
        boost::shared_ptr<image< > > sp = imgF.select(x_origin<unsigned int> (100),
                                                      y_origin<unsigned int> (110));
        sp->show("NO FLIP", nicta::vibe::image::resize);

        // copies with flip
        sp->flip<x_axis> (destination_model<copy> ())->show("FLIPPED-Xaxis");
        sp->flip<y_axis> (destination_model<copy> ())->show("FLIPPED-Yaxis");
        sp->flip<xy_axis> (destination_model<copy> ())->show("FLIPPED-XYaxis");

        // inplace with flip
        sp->flip<xy_axis> (destination_model<inplace> ()).show("FLIPPED-XYaxis-INPLACE");


        // rescale image
        sp->scale<Bicubic> (width<unsigned int> (500), height<unsigned int> (500))->show("SCALED UP");
        sp->scale<Bilinear> (width<unsigned int> (200), height<unsigned int> (300))->show("SCALED DOWN");


        // freeze all shown images & wait for any keypress
        wait_key(boost::posix_time::seconds(0));
    }
    catch (const nicta_exception& e)
    {
        std::cerr << e.nice_what();
        exit(1);
    }

    return 0;
}
