/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "imageheader_test.h"
#include "image_header/image_header.h"
#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION(ImageHeaderTest);



void ImageHeaderTest :: setUp()
{
}


void ImageHeaderTest :: tearDown()
{
}


void ImageHeaderTest :: imageHeader()
{
    nicta::vibe::header::image_header ih("file.jpeg");

    CPPUNIT_ASSERT_EQUAL(ih.depth(), 8);
    CPPUNIT_ASSERT_EQUAL(ih.width(), 500);
    CPPUNIT_ASSERT_EQUAL(ih.height(), 332);
    CPPUNIT_ASSERT(ih.is_color() == true);
    CPPUNIT_ASSERT(ih.is_integral() == true);
    CPPUNIT_ASSERT(std::string(ih.filename()) == std::string("file.jpeg"));
}

