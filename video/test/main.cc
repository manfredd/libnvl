/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "video/algorithm/canonical_identity.h"
#include "video/algorithm/deinterlace_identity.h"
#include "video/algorithm/face_detect.h"
#include "video/algorithm/play_stream.h"
#include "video/algorithm/pipe/pipe_stream.h"
#include "video/algorithm/pipe/streamable/gray_streamable.h"
#include "video/algorithm/pipe/visitable.h"
#include "video/algorithm/pipe/visitor.h"
#include "video/algorithm/record_stream.h"
#include "video/controller.h"
#include "video/models/camera_device.h"
#include "video/video_client.h"
#include "video/video_server.h"

#include <image/models/flip_model.h>

#include <nicta_common/nicta_exception.h>
#include <nicta_common/quantity.h>
#include <nicta_common/util.h>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/ref.hpp>

#include <iostream>
#include <string>
#include <utility>
#include <vector>


int main(int argc, char** argv)
{
    try
    {
        // ---- FILE WRITING -----------------------------------------

        std::cout << std::endl << "Writing a video file.." << std::endl;


        // instantiate controllers, servers, and clients ..
        nicta::vibe::video::controller< > cm_w;
        nicta::vibe::video::video_server< > vs_w(&cm_w, "video.avi", "vs_w", boost::posix_time::time_duration());
        nicta::vibe::video::video_client< > clw(&cm_w,
                                                &vs_w,
                                                "clientw",
                                                nicta::quantity::step<unsigned long int>(1),
                                                nicta::quantity::signed_frame_buffer_capacity<long int>(0), // unlimited buffer size (drop no frames)
                                                "SILENT_KEYS");          // take every frame from each server ; no keyboard termination


        //nicta::vibe::video::algorithm::canonical_identity ftor;                     // <--- can pass an algorithm to do work at server end too ..
        //nicta::vibe::video::algorithm::deinterlace_identity dftor;                  // <--- can pass an algorithm to do work at server end too ..
        //boost::thread vsw_thd(boost::bind( &nicta::vibe::video::video_server< >::operator()< nicta::vibe::video::algorithm::canonical_identity
        //                                                                                   , 1U
        //                                                                                   , nicta::vibe::video::algorithm::deinterlace_identity
        //                                                                                   >
        //                                  , boost::ref(vs_w)
        //                                  , boost::ref(ftor)
        //                                  , boost::ref(dftor)));


        boost::thread csw_thd(boost::ref(cm_w));
        boost::thread vsw_thd(boost::ref(vs_w));


        // algorithm:  writes frames
        nicta::vibe::video::algorithm::record_stream< > recorder("output.avi",
                                                                 "DIVX",
                                                                 nicta::quantity::frame_rate_per_second<unsigned int>(25));



        // thread client, consuming frames 3 seconds from now, and 0sec (aka all frames) thereafter [ >= ]
        boost::posix_time::ptime tw = boost::posix_time::microsec_clock::local_time();
        boost::thread clw_thd(boost::bind(&nicta::vibe::video::video_client< >::operator()<nicta::vibe::video::algorithm::record_stream< > >,
                                          boost::cref(clw),
                                          boost::cref(vs_w),                          // <--- redundant master-server b/c clw only runs off 1 server anyway
                                          tw + boost::posix_time::seconds(3),
                                          boost::posix_time::seconds(0),
                                          boost::ref(recorder)));


        // wait for client & controller to complete ..
        clw_thd.join();
        csw_thd.join();
        recorder.~record_stream< >();        // must release resource held by stream (just a peculiarity of ffmpeg..
                                             // .. else reading from output.avi below becomes problematic)
                                             // NOTE:  it is *not* a good idea to call dtor explicitly!


        // ---- NON-THREADED VARIANT OF FILE WRITING -----------------------------------
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        //
        //nicta::vibe::video::video_server< > s_vsw("video.avi", "s_vsw", boost::posix_time::time_duration());
        //nicta::vibe::video::video_client< > s_clw(&s_vsw,
        //                                          "s_clientw",
        //                                          nicta::quantity::step<unsigned long int>(1),
        //                                          nicta::quantity::signed_frame_buffer_capacity<long int>(0), // unlimited buffer size (drop no frames)
        //                                          "SILENT_KEYS");          // take every frame from each server ; no keyboard termination
        //
        //nicta::vibe::video::algorithm::record_stream< > s_recorder("output.avi",
        //                                                           "DIVX",
        //                                                            nicta::quantity::frame_rate_per_second<unsigned int>(25));
        //
        //boost::posix_time::ptime s_tw = boost::posix_time::microsec_clock::local_time();
        //s_clw(s_tw + boost::posix_time::seconds(3),
        //      boost::posix_time::seconds(0),
        //      s_recorder);
        //
        //s_recorder.~record_stream< >();
        //
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.



        // ---- FILE READING -----------------------------------------

        std::cout << std::endl << "Reading several video files.." << std::endl;



        // instantiate controllers, servers, and clients ..
        nicta::vibe::video::controller< > cm1;
        nicta::vibe::video::video_server< > vs1(&cm1, "video.avi", "vs1", boost::posix_time::time_duration());
        nicta::vibe::video::video_server< > vs2(&cm1, "output.avi", "vs2", boost::posix_time::time_duration());
        nicta::vibe::video::video_client< > cl1(&cm1,
                                                &vs1, &vs2,
                                                "client1",
                                                nicta::quantity::step<unsigned long int>(5),
                                                nicta::quantity::signed_frame_buffer_capacity<long int>(0), // unlimited buffer size (drop no frames)
                                                "SILENT_KEYS");          // take every 5th frame from each server ; no keyboard termination
        nicta::vibe::video::video_client< > cl2(&cm1,
                                                &vs1,
                                                "client2",
                                                nicta::quantity::step<unsigned long int>(1),
                                                nicta::quantity::signed_frame_buffer_capacity<long int>(0), // unlimited buffer size (drop no frames)
                                                "SILENT_KEYS");          // take every frame from server ; no keyboard termination


        // instantiate controllers, servers, and clients ..
        nicta::vibe::video::controller< > cm2;
        nicta::vibe::video::video_server< > vs3(&cm2, "video.avi", "vs3", boost::posix_time::time_duration());
        nicta::vibe::video::video_client< > cl3(&cm2,
                                                &vs3,
                                                "client3",
                                                nicta::quantity::step<unsigned long int>(1),
                                                nicta::quantity::signed_frame_buffer_capacity<long int>(0), // unlimited buffer size (drop no frames)
                                                "ESC");                  // take every frame from server ; terminate client on `Esc' keypress


        // thread servers and controllers ..
        boost::thread cs1_thd(boost::ref(cm1));
        boost::thread cs2_thd(boost::ref(cm2));
        boost::thread vs1_thd(boost::ref(vs1));
        boost::thread vs2_thd(boost::ref(vs2));
        boost::thread vs3_thd(boost::ref(vs3));


        // algorithm:  displays frames
        nicta::vibe::video::algorithm::play_stream< > ftor;


        // thread client, consuming all incoming frames
        boost::thread cl1_thd(boost::bind(&nicta::vibe::video::video_client< >::operator()<nicta::vibe::video::algorithm::play_stream< > >,
                                          boost::cref(cl1),
                                          boost::ref(ftor)));



        // thread client, consuming frames 2 seconds from now, and 800ms thereafter [ >= ]
        // ..grabing images across all connected servers, with timestamps sync'd on server vs1
        boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
        boost::thread cl2_thd(boost::bind(&nicta::vibe::video::video_client< >::operator()<nicta::vibe::video::algorithm::play_stream< > >,
                                          boost::cref(cl2),
                                          boost::cref(vs1),                           // <--- redundant master-server b/c cl2 only runs off 1 server anyway
                                          t + boost::posix_time::seconds(2),
                                          boost::posix_time::milliseconds(800),
                                          boost::ref(ftor)));



        // thread client, consuming frames 5 seconds from now, and 0sec (aka all frames) thereafter [ >= ]
        // ..grabing images across all connected servers, with timestamps sync'd on server vs3
        boost::thread cl3_thd(boost::bind(&nicta::vibe::video::video_client< >::operator()<nicta::vibe::video::algorithm::play_stream< > >,
                                          boost::cref(cl3),
                                          boost::cref(vs3),                           // <--- redundant master-server b/c cl3 only runs off 1 server anyway
                                          t + boost::posix_time::seconds(5),
                                          boost::posix_time::seconds(0),
                                          boost::ref(ftor)));



        // wait for clients & controller to complete ..
        cl3_thd.join();
        cl2_thd.join();
        cl1_thd.join();
        cs2_thd.join();
        cs1_thd.join();



        // ---- NON-THREADED VARIANT OF FILE READING -----------------------------------
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        //
        nicta::vibe::video::video_server< > s_vs("video.avi", "s_vs", boost::posix_time::time_duration());
        nicta::vibe::video::video_client< > s_cl(&s_vs,
                                                "s_client3",
                                                nicta::quantity::step<unsigned long int>(1),
                                                nicta::quantity::signed_frame_buffer_capacity<long int>(0), // unlimited buffer size (drop no frames)
                                                "ESC");                  // take every frame from server ; terminate client on `Esc' keypress
        s_cl(ftor);
        //
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.



        // ---- IMAGE-FILE READING -----------------------------------

        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        //
        //std::cout << std::endl << "Reading several image files.." << std::endl;
        //
        //
        //// instantiate controllers, servers, and clients ..
        //nicta::vibe::video::controller< > control;
        //nicta::vibe::video::video_server< > server(&control,
        //                                           "/import/apu/projects/1/VISTA/vibe/usr/local/share/scratch/manfred/face_data",
        //                                           "*.JPG, *.png, *.jpeg, *jpg, *.JPG",    // <--- will show *.JPG files twice .. a feature *not* a bug!
        //                                           "server",
        //                                           nicta::quantity::frame_rate_per_second<unsigned int>(1));
        //nicta::vibe::video::video_client< > client(&control,
        //                                           &server,
        //                                           "client",
        //                                           nicta::quantity::step<unsigned long int>(1),
        //                                           nicta::quantity::signed_frame_buffer_capacity<long int>(0), // unlimited buffer size (drop no frames)
        //                                           "SILENT_KEYS");
        //
        //
        //boost::thread control_thd(boost::ref(control));
        //boost::thread server_thd(boost::ref(server));
        //
        //
        //// algorithm:  displays frames
        //nicta::vibe::video::algorithm::play_stream< >  iftor;
        //
        //
        //// thread client, consuming all incoming frames
        //boost::thread client_thd(boost::bind(&nicta::vibe::video::video_client< >::operator()<nicta::vibe::video::algorithm::play_stream< > >,
        //                                     boost::cref(client),
        //                                     boost::ref(iftor)));
        //
        //client_thd.join();
        //control_thd.join();
        //
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        // ---- NON-THREADED VARIANT OF IMAGE-FILE READING
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        //
        //nicta::vibe::video::video_server< > s_server("/import/apu/projects/1/VISTA/vibe/usr/local/share/scratch/manfred/face_data",
        //                                             "*.JPG, *.png, *.jpeg, *jpg, *.JPG",    // <--- will show *.JPG files twice .. a feature *not* a bug!
        //                                             "s_server",
        //                                             nicta::quantity::frame_rate_per_second<unsigned int>(1));
        //nicta::vibe::video::video_client< > s_client(&s_server,
        //                                             "s_client",
        //                                             nicta::quantity::step<unsigned long int>(1),
        //                                             nicta::quantity::signed_frame_buffer_capacity<long int>(0),   // unlimited buffer size (drop no frames)
        //                                             "SILENT_KEYS");
        //
        //nicta::vibe::video::algorithm::play_stream< >  iftor;
        //s_client(iftor);
        //
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.





        // ---- LIVE CAMERA FEED -------------------------------------

        std::cout << std::endl << "Live camera feed.." << std::endl;


        // the camera..
        nicta::vibe::video::camera_device< > camera;

        // instantiate controllers, servers, and clients ..
        nicta::vibe::video::controller< > cmx;
        nicta::vibe::video::video_server< > vsx(&cmx, camera, "vsx");
        nicta::vibe::video::video_client< > clx(&cmx,
                                                &vsx,
                                                "client_X",
                                                nicta::quantity::step<unsigned long int>(1),
                                                nicta::quantity::signed_frame_buffer_capacity<long int>(0), // unlimited buffer size (drop no frames)
                                                "ENTER");                // take every frame from server ; terminate client on `Enter' keypress


        // thread servers and controllers ..
        boost::thread csx_thd(boost::ref(cmx));
        boost::thread vsx_thd(boost::ref(vsx));


        // algorithm:  displays frames
        //nicta::vibe::video::algorithm::play_stream<nicta::vibe::image::y_axis>  ftorx;


        // algorithm:  detects faces
        std::string filename;
        const char* cascade = "haarcascade_frontalface_alt.xml";
        nicta::util::deduced_file("HAAR_CASCADE_PATH", cascade, filename);
        nicta::vibe::video::algorithm::face_detect ftorx(filename.c_str());


        // thread client, consuming all incoming frames

        //boost::thread clx_thd(boost::bind(&nicta::vibe::video::video_client< >::operator()<nicta::vibe::video::algorithm::play_stream<nicta::vibe::image::y_axis> >,
        //                                  boost::cref(clx),
        //                                  boost::ref(ftorx)));

        boost::thread clx_thd(boost::bind(&nicta::vibe::video::video_client< >::operator()<nicta::vibe::video::algorithm::face_detect>,
                                          boost::cref(clx),
                                          boost::ref(ftorx)));

        clx_thd.join();
        csx_thd.join();


        // ---- NON-THREADED VARIANT OF LIVE CAMERA FEED -----------------------------------
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        //
        //nicta::vibe::video::camera_device< > s_camera;
        //
        //nicta::vibe::video::video_server< > s_vsx(s_camera, "s_vsx");
        //nicta::vibe::video::video_client< > s_clx(&s_vsx,
        //                                          "s_client_X",
        //                                          nicta::quantity::step<unsigned long int>(1),
        //                                          nicta::quantity::signed_frame_buffer_capacity<long int>(0),   // unlimited buffer size (drop no frames)
        //                                          "ENTER");                // take every frame from server ; terminate client on `Enter' keypress
        //std::string s_filename;
        //const char* s_cascade = "haarcascade_frontalface_alt.xml";
        //nicta::util::deduced_file("HAAR_CASCADE_PATH", s_cascade, s_filename);
        //nicta::vibe::video::algorithm::face_detect s_ftorx(s_filename.c_str());
        //
        //s_clx(s_ftorx);
        //
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.




        // ---- GRAY-STREAMABLE CAMERA FEED --------------------------------
        //
        //std::cout << std::endl << "Gray-streamable camera feed.." << std::endl;
        //
        //
        //std::vector< boost::variant< nicta::vibe::video::algorithm::pipe::visitable<nicta::vibe::video::algorithm::pipe::streamable::gray_streamable> *
        //                           >
        //           > algo_stream;
        //
        //nicta::vibe::video::algorithm::pipe::streamable::gray_streamable< > g_stream;
        //algo_stream.push_back(&g_stream);
        //
        //nicta::vibe::video::algorithm::pipe::pipe_stream1< nicta::vibe::image::depth_8u
        //                                                 , nicta::vibe::image::bgr_p
        //                                                 , nicta::vibe::algorithm::pipe::display_tag::type
        //                                                 , nicta::vibe::video::algorithm::pipe::visitable< nicta::vibe::video::algorithm::pipe::streamable::gray_streamable >
        //                                                 , nicta::vibe::video::algorithm::pipe::visitor
        //                                                 , std::vector
        //                                                 > pipe(std::move(algo_stream));
        //
        //nicta::vibe::video::camera_device< > g_camera;
        //nicta::vibe::video::video_server< > g_server(g_camera, "g_server");
        //nicta::vibe::video::video_client< > g_client(&g_server,
        //                                             "g_client",
        //                                             nicta::quantity::step<unsigned long int> (1),
        //                                             nicta::quantity::signed_frame_buffer_capacity<long int> (0),
        //                                             "ENTER");
        //g_client(pipe);
    }
    catch (const nicta::nicta_exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    return 0;
}

