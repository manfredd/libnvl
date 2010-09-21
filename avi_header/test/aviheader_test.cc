/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "aviheader_test.h"
#include "avi_header/avi_header.h"
#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION(AVIHeaderTest);



void AVIHeaderTest :: setUp()
{
}


void AVIHeaderTest :: tearDown()
{
}


void AVIHeaderTest :: aviHeader()
{
    nicta::vibe::header::avi_header vh("video.avi");

    CPPUNIT_ASSERT_EQUAL(vh.msec_frame_delay(), 40000);
    CPPUNIT_ASSERT_EQUAL(vh.max_data_rate(), 25000);
    CPPUNIT_ASSERT(vh.interleaved() == true);
    CPPUNIT_ASSERT(vh.optimized_video_capture() == false);
    CPPUNIT_ASSERT(vh.copyrighted_data() == false);
    CPPUNIT_ASSERT_EQUAL(vh.frame_count(), 501);
    CPPUNIT_ASSERT_EQUAL(vh.initial_frame(), 0);
    CPPUNIT_ASSERT_EQUAL(vh.stream_count(), 1);
    CPPUNIT_ASSERT_EQUAL(vh.optimal_buffer_size(), 1024 * 1024);
    CPPUNIT_ASSERT_EQUAL(vh.frame_width(), 640);
    CPPUNIT_ASSERT_EQUAL(vh.frame_height(), 480);
    CPPUNIT_ASSERT(std::string(vh.data_type()) == std::string("vids"));
    CPPUNIT_ASSERT(std::string(vh.fourcc_codec()) == std::string("DIVX"));
    CPPUNIT_ASSERT_EQUAL(vh.scale(), 1);
    CPPUNIT_ASSERT_EQUAL(vh.rate(), 25);
    CPPUNIT_ASSERT_EQUAL(vh.start_time(), 0);
    CPPUNIT_ASSERT_EQUAL(vh.length(), 501);
    CPPUNIT_ASSERT_EQUAL(vh.sample_size(), 921600);

    CPPUNIT_ASSERT(std::string(vh.filename()) == std::string("video.avi"));
}

