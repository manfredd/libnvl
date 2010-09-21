/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_SERVER_H
#define NICTA_VIBE_VIDEO_SERVER_H

#include "video/controller.h"
#include "video/frame.h"
#include "video/iplimage_deleter.h"
#include "video/algorithm/canonical_identity.h"
#include "video/algorithm/deinterlace_identity.h"
#include "video/algorithm/detail/fourcc.h"
#include "video/models/camera_device.h"
#include "video/models/driver_type.h"
#include "video/models/stream_model.h"
#include "video/thread/gate_model.h"
#include "video/thread/multiton_model.h"
#ifdef NICTA_PGR
# include "video/vendor/pgr/pgr.h"
# include "video/vendor/pgr/camera_device.h"
#endif

#include <avi_header/avi_header.h>
#include <image/image.h>
#include <image/image_depth.h>
#include <image/image_pixel.h>
#include <nicta_common/nicta_exception.h>
#include <nicta_common/null_deleter.h>
#include <nicta_common/quantity.h>
#include <nicta_common/stringutil.h>
#include <nicta_common/util.h>
#include <thread/condition/condition_wait.h>
#include <thread/condition/notify_model.h>
#include <thread/gate/gate.h>
#include <thread/guard/guard.h>
#include <thread/multiton/multiton.h>
#include <thread/rw_locks/rw_locks.h>

#ifdef NICTA_PGR
# include <flycapture/Camera.h>
# include <flycapture/Image.h>
#endif

#include <opencv/cxcore.h>
#include <opencv/cxtypes.h>
#include <opencv/highgui.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/array.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/type_traits/is_same.hpp>

#include <exception>
#include <list>
#include <string>

namespace nicta {
namespace vibe {
namespace video {

    /* Forward Declaration */
    template < typename Depth
             , template <typename _> class Pixel
             >
    class controller;


    template < typename Depth = nicta::vibe::image::depth_8u
             , template <typename _> class Pixel = nicta::vibe::image::bgr_p
             >
    class video_server
    {
    public:

        typedef void result_type;


        ~video_server() throw ();


        video_server(const char* path,
                     const char* imagefiles,
                     const char* id,
                     nicta::quantity::frame_rate_per_second<unsigned int> fps
                    ) throw (nicta::nicta_exception);

        video_server(nicta::vibe::video::controller<Depth, Pixel>* mediator,
                     const char* path,
                     const char* imagefiles,
                     const char* id,
                     nicta::quantity::frame_rate_per_second<unsigned int> fps
                    ) throw (nicta::nicta_exception);


        video_server(const char* videofile,
                     const char* id,
                     boost::posix_time::time_duration start
                    ) throw ();

        video_server(nicta::vibe::video::controller<Depth, Pixel>* mediator,
                     const char* videofile,
                     const char* id,
                     boost::posix_time::time_duration start
                    ) throw ();


        video_server(const char* videofile,
                     const char* id,
                     nicta::quantity::frames<unsigned long int> start
                    ) throw ();

        video_server(nicta::vibe::video::controller<Depth, Pixel>* mediator,
                     const char* videofile,
                     const char* id,
                     nicta::quantity::frames<unsigned long int> start
                    ) throw ();


        video_server(const char* videofile,
                     const char* id,
                     nicta::quantity::ratio<double> start
                    ) throw (nicta::nicta_exception);

        video_server(nicta::vibe::video::controller<Depth, Pixel>* mediator,
                     const char* videofile,
                     const char* id,
                     nicta::quantity::ratio<double> start
                    ) throw (nicta::nicta_exception);



        template < typename nicta::vibe::video::DriverType driver
                 , unsigned short int camera_index
                 >
        video_server(nicta::vibe::video::camera_device<driver, camera_index> camera,
                     const char* id,
                     nicta::quantity::frame_rate_per_second<unsigned int> fps =
                            nicta::quantity::frame_rate_per_second<unsigned int> (0),
                     const char* fourcc_codec = "",
                     nicta::quantity::width<unsigned int> width =
                            nicta::quantity::width<unsigned int> (0),
                     nicta::quantity::height<unsigned int> height =
                            nicta::quantity::height<unsigned int> (0)
                    ) throw (nicta::nicta_exception);


        template < typename nicta::vibe::video::DriverType driver
                 , unsigned short int camera_index
                 >
        video_server(nicta::vibe::video::controller<Depth, Pixel>* mediator,
                     nicta::vibe::video::camera_device<driver, camera_index> camera,
                     const char* id,
                     const char* fourcc_codec = "",
                     nicta::quantity::width<unsigned int> width =
                            nicta::quantity::width<unsigned int> (0),
                     nicta::quantity::height<unsigned int> height =
                            nicta::quantity::height<unsigned int> (0)
                    ) throw (nicta::nicta_exception);


#ifdef NICTA_PGR
        // pgr stereo camera
        template <unsigned short int camera_index>
        video_server(nicta::vibe::video::pgr::camera_device<camera_index>& camera,
                     const char* id
                    ) throw (nicta::nicta_exception);

        template <unsigned short int camera_index>
        video_server(nicta::vibe::video::controller<Depth, Pixel>* mediator,
                     nicta::vibe::video::pgr::camera_device<camera_index>& camera,
                     const char* id
                    ) throw (nicta::nicta_exception);
#endif


        result_type operator()() throw (nicta::nicta_exception, std::bad_exception);

        template <typename Functor>
        result_type operator()(Functor& ftor) throw (nicta::nicta_exception, std::bad_exception);

        template < unsigned int Payload
                 , template <unsigned int Payload_, typename D_, template <typename _D> class P_> class Deinterlace
                 >
        result_type operator()(Deinterlace<Payload, Depth, Pixel>& dftor) throw (nicta::nicta_exception, std::bad_exception);

        template < typename Functor
                 , unsigned int Payload
                 , template <unsigned int Payload_, typename D_, template <typename _D> class P_> class Deinterlace
                 >
        result_type operator()(Functor& ftor, Deinterlace<Payload, Depth, Pixel>& dftor) throw (nicta::nicta_exception, std::bad_exception);


        void shutdown() throw ();
        unsigned long int held_frame_index() const throw ();
        const typename nicta::vibe::video::sp_iplframe::type* tx() const throw ();

        bool online() const throw ();
        bool offline() const throw ();
        const std::string& id() const throw ();

        unsigned int payload() const throw ();

        double fps() const throw ();
        bool fps_available() const throw ();
        nicta::thread::condition_wait<boost::mutex>& fps_waiter() throw ();

        unsigned long int stream_length() const throw ();
        nicta::vibe::video::StreamModel stream_type() const throw ();


    private:

        unsigned int stream_position_() const throw ();


        std::string id_;
        unsigned int payload_;

        bool stream_init_;
        bool stream_eof_;
        mutable boost::shared_mutex eof_mutex_;

        bool imagecapture_;
        std::list<std::string> imagefiles_;
        std::list<std::string>::size_type imagefiles_count_;

        bool filecapture_;
        ::CvCapture* stream_capture_;
#ifdef NICTA_PGR
        FlyCapture2::Camera* pgr_stream_capture_;
#else
        bool pgr_stream_capture_;                                          // a hack that minimizes number of ifdefs we need
#endif

        short unsigned int read_ptr_;
        typename nicta::vibe::video::sp_iplframe::type* buffer_;
        typename nicta::vibe::video::sp_iplframe::type* mt_buffer_[2];     // [2]:  read_ptr will read at one index, and while we write into the other!

        nicta::vibe::video::controller<Depth, Pixel>* controller_;

        double fps_;
        nicta::thread::condition_wait<boost::mutex> fps_waiter_;

        unsigned long int stream_length_;
        nicta::vibe::video::StreamModel stream_type_;
    };

} } } // namespace nicta::vibe::video


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::video::video_server<T, U> :: 
~video_server() throw ()
{
    // Destructor

    if (this->stream_capture_)
    {
        ::cvReleaseCapture(&this->stream_capture_);
        this->stream_capture_ = 0;
    }

    if (!this->controller_)
    {
        // for thread-unaware state..
        delete[] this->buffer_;
        this->buffer_ = 0;
    }
    else
    {
        // for a thread-aware state..
        delete[] this->mt_buffer_[1];
        delete[] this->mt_buffer_[0];
        this->mt_buffer_[1] = 0;
        this->mt_buffer_[0] = 0;
    }
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::video::video_server<T, U> :: 
video_server(const char* path,
             const char* imagefiles,
             const char* id,
             nicta::quantity::frame_rate_per_second<unsigned int> fps
            ) throw (nicta::nicta_exception)
: id_(id)
, payload_(0)
, stream_init_(false)
, stream_eof_(false)
, imagecapture_(true)
, imagefiles_count_(0)
, filecapture_(false)
, stream_capture_(0)
, pgr_stream_capture_(0)
, read_ptr_(1)
, buffer_(0)
, controller_(0)
, fps_(fps.operator()<double>())
, fps_waiter_(false)
, stream_length_(0)
, stream_type_(nicta::vibe::video::Directory)
{
    // Constructor - for reading image files from a directory

    this->mt_buffer_[0] = 0;
    this->mt_buffer_[1] = 0;

    if (!fps.operator()< >())
    {
        throw nicta::nicta_exception("Sought to stream directory of images at zero fps", "video_server", "video_server");
    }

    if (this->id_.empty())
    {
        this->id_ = nicta::stringutil::atomic_to_string(this);
    }

    std::vector<std::string> vec;
    boost::algorithm::split(vec, imagefiles, boost::algorithm::is_any_of(" ,;"));

    for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it)
    {
        boost::filesystem::path p(path, boost::filesystem::portable_directory_name);
        p /= *it;

        std::vector<std::string> files;
        std::string cmd("ls " + p.native_file_string());
        nicta::util::command_capture(cmd.c_str(), files);

        for (std::vector<std::string>::iterator jt = files.begin(); jt != files.end(); ++jt)
        {
            boost::filesystem::path p(*jt, boost::filesystem::portable_file_name);
            if (boost::filesystem::is_regular_file(p))
            {
                this->imagefiles_.insert(this->imagefiles_.end(), p.native_file_string());
            }
        }
    }
    if (this->imagefiles_.empty())
    {
        throw nicta::nicta_exception("No such file or directory", "video_server", "video_server");
    }
    this->imagefiles_count_ = this->imagefiles_.size();
    this->stream_length_    = static_cast<unsigned long int> (this->imagefiles_count_);
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::video::video_server<T, U> :: 
video_server(nicta::vibe::video::controller<T, U>* mediator,
             const char* path,
             const char* imagefiles,
             const char* id,
             nicta::quantity::frame_rate_per_second<unsigned int> fps
            ) throw (nicta::nicta_exception)
: id_(id)
, payload_(0)
, stream_init_(false)
, stream_eof_(false)
, imagecapture_(true)
, imagefiles_count_(0)
, filecapture_(false)
, stream_capture_(0)
, pgr_stream_capture_(0)
, read_ptr_(1)
, buffer_(0)
, controller_(mediator)
, fps_(fps.operator()<double>())
, fps_waiter_(false)
, stream_length_(0)
, stream_type_(nicta::vibe::video::Directory)
{
    // Constructor - for reading image files from a directory
    // NOTE:  thread-aware variant..

    this->mt_buffer_[0] = 0;
    this->mt_buffer_[1] = 0;

    if (!fps.operator()< >())
    {
        throw nicta::nicta_exception("Sought to stream directory of images at zero fps", "video_server", "video_server");
    }

    if (this->id_.empty())
    {
        this->id_ = nicta::stringutil::atomic_to_string(this);
    }

    std::vector<std::string> vec;
    boost::algorithm::split(vec, imagefiles, boost::algorithm::is_any_of(" ,;"));

    for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it)
    {
        boost::filesystem::path p(path, boost::filesystem::portable_directory_name);
        p /= *it;

        std::vector<std::string> files;
        std::string cmd("ls " + p.native_file_string());
        nicta::util::command_capture(cmd.c_str(), files);

        for (std::vector<std::string>::iterator jt = files.begin(); jt != files.end(); ++jt)
        {
            boost::filesystem::path p(*jt, boost::filesystem::portable_file_name);
            if (boost::filesystem::is_regular_file(p))
            {
                this->imagefiles_.insert(this->imagefiles_.end(), p.native_file_string());
            }
        }
    }
    if (this->imagefiles_.empty())
    {
        throw nicta::nicta_exception("No such file or directory", "video_server", "video_server");
    }
    this->imagefiles_count_ = this->imagefiles_.size();
    this->stream_length_    = static_cast<unsigned long int> (this->imagefiles_count_);
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::video::video_server<T, U> :: 
video_server(const char* videofile,
             const char* id,
             boost::posix_time::time_duration start
            ) throw ()
: id_(id)
, payload_(0)
, stream_init_(false)
, stream_eof_(false)
, imagecapture_(false)
, imagefiles_count_(0)
, filecapture_(true)
, stream_capture_(::cvCaptureFromFile(videofile))
, pgr_stream_capture_(0)
, read_ptr_(1)
, buffer_(0)
, controller_(0)
, fps_(0)
, fps_waiter_(false)
, stream_length_(static_cast<unsigned long int> (nicta::vibe::header::avi_header(videofile).frame_count()))
, stream_type_(nicta::vibe::video::File)
{
    // Constructor - for reading AVI files
    // `start':  position from the file beginning to initiate reading

    this->mt_buffer_[0] = 0;
    this->mt_buffer_[1] = 0;

    if (this->id_.empty())
    {
        this->id_ = nicta::stringutil::atomic_to_string(this);
    }

    double t = static_cast<double> (start.total_seconds());

#ifdef BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG
    t += static_cast<double> (start.fractional_seconds()) / 1e9;
#else
    t += static_cast<double> (start.fractional_seconds()) / 1e6;
#endif

    if (t)
    {
        ::cvSetCaptureProperty(this->stream_capture_, CV_CAP_PROP_POS_MSEC, t * 1000);
    }
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::video::video_server<T, U> :: 
video_server(nicta::vibe::video::controller<T, U>* mediator,
             const char* videofile,
             const char* id,
             boost::posix_time::time_duration start
            ) throw ()
: id_(id)
, payload_(0)
, stream_init_(false)
, stream_eof_(false)
, imagecapture_(false)
, imagefiles_count_(0)
, filecapture_(true)
, stream_capture_(::cvCaptureFromFile(videofile))
, pgr_stream_capture_(0)
, read_ptr_(1)
, buffer_(0)
, controller_(mediator)
, fps_(0)
, fps_waiter_(false)
, stream_length_(static_cast<unsigned long int> (nicta::vibe::header::avi_header(videofile).frame_count()))
, stream_type_(nicta::vibe::video::File)
{
    // Constructor - for reading AVI files
    // `start':  position from the file beginning to initiate reading
    // NOTE:  thread-aware variant..

    this->mt_buffer_[0] = 0;
    this->mt_buffer_[1] = 0;

    if (this->id_.empty())
    {
        this->id_ = nicta::stringutil::atomic_to_string(this);
    }

    double t = static_cast<double> (start.total_seconds());

#ifdef BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG
    t += static_cast<double> (start.fractional_seconds()) / 1e9;
#else
    t += static_cast<double> (start.fractional_seconds()) / 1e6;
#endif

    if (t)
    {
        ::cvSetCaptureProperty(this->stream_capture_, CV_CAP_PROP_POS_MSEC, t * 1000);
    }
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::video::video_server<T, U> ::
video_server(const char* videofile,
             const char* id,
             nicta::quantity::frames<unsigned long int> start
            ) throw ()
: id_(id)
, payload_(0)
, stream_init_(false)
, stream_eof_(false)
, imagecapture_(false)
, imagefiles_count_(0)
, filecapture_(true)
, stream_capture_(::cvCaptureFromFile(videofile))
, pgr_stream_capture_(0)
, read_ptr_(1)
, buffer_(0)
, controller_(0)
, fps_(0)
, fps_waiter_(false)
, stream_length_(static_cast<unsigned long int> (nicta::vibe::header::avi_header(videofile).frame_count()))
, stream_type_(nicta::vibe::video::File)
{
    // Constructor - for reading AVI files

    this->mt_buffer_[0] = 0;
    this->mt_buffer_[1] = 0;

    if (this->id_.empty())
    {
        this->id_ = nicta::stringutil::atomic_to_string(this);
    }

    if (start.operator()< >())
    {
        ::cvSetCaptureProperty(this->stream_capture_, CV_CAP_PROP_POS_FRAMES, start.operator()<double>());
    }
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::video::video_server<T, U> ::
video_server(nicta::vibe::video::controller<T, U>* mediator,
             const char* videofile,
             const char* id,
             nicta::quantity::frames<unsigned long int> start
            ) throw ()
: id_(id)
, payload_(0)
, stream_init_(false)
, stream_eof_(false)
, imagecapture_(false)
, imagefiles_count_(0)
, filecapture_(true)
, stream_capture_(::cvCaptureFromFile(videofile))
, pgr_stream_capture_(0)
, read_ptr_(1)
, buffer_(0)
, controller_(mediator)
, fps_(0)
, fps_waiter_(false)
, stream_length_(static_cast<unsigned long int> (nicta::vibe::header::avi_header(videofile).frame_count()))
, stream_type_(nicta::vibe::video::File)
{
    // Constructor - for reading AVI files
    // NOTE:  thread-aware variant..

    this->mt_buffer_[0] = 0;
    this->mt_buffer_[1] = 0;

    if (this->id_.empty())
    {
        this->id_ = nicta::stringutil::atomic_to_string(this);
    }

    if (start.operator()< >())
    {
        ::cvSetCaptureProperty(this->stream_capture_, CV_CAP_PROP_POS_FRAMES, start.operator()<double>());
    }
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::video::video_server<T, U> ::
video_server(const char* videofile,
             const char* id,
             nicta::quantity::ratio<double> start
            ) throw (nicta::nicta_exception)
: id_(id)
, payload_(0)
, stream_init_(false)
, stream_eof_(false)
, imagecapture_(false)
, imagefiles_count_(0)
, filecapture_(true)
, stream_capture_(::cvCaptureFromFile(videofile))
, pgr_stream_capture_(0)
, read_ptr_(1)
, buffer_(0)
, controller_(0)
, fps_(0)
, fps_waiter_(false)
, stream_length_(static_cast<unsigned long int> (nicta::vibe::header::avi_header(videofile).frame_count()))
, stream_type_(nicta::vibe::video::File)
{
    // Constructor - for reading AVI files

    this->mt_buffer_[0] = 0;
    this->mt_buffer_[1] = 0;

    if (this->id_.empty())
    {
        this->id_ = nicta::stringutil::atomic_to_string(this);
    }

    if (start.operator()< >() < 0 || start.operator()< >() > 1)
    {
        throw nicta::nicta_exception("ratio to file start must be in range [0, 1]", "video_server<T, U>", "video_server<T, U>");
    }
    ::cvSetCaptureProperty(this->stream_capture_, CV_CAP_PROP_POS_AVI_RATIO, start.operator()<double>());
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::video::video_server<T, U> ::
video_server(nicta::vibe::video::controller<T, U>* mediator,
             const char* videofile,
             const char* id,
             nicta::quantity::ratio<double> start
            ) throw (nicta::nicta_exception)
: id_(id)
, payload_(0)
, stream_init_(false)
, stream_eof_(false)
, imagecapture_(false)
, imagefiles_count_(0)
, filecapture_(true)
, stream_capture_(::cvCaptureFromFile(videofile))
, pgr_stream_capture_(0)
, read_ptr_(1)
, buffer_(0)
, controller_(mediator)
, fps_(0)
, fps_waiter_(false)
, stream_length_(static_cast<unsigned long int> (nicta::vibe::header::avi_header(videofile).frame_count()))
, stream_type_(nicta::vibe::video::File)
{
    // Constructor - for reading AVI files
    // NOTE:  thread-aware variant..

    this->mt_buffer_[0] = 0;
    this->mt_buffer_[1] = 0;

    if (this->id_.empty())
    {
        this->id_ = nicta::stringutil::atomic_to_string(this);
    }

    if (start.operator()< >() < 0 || start.operator()< >() > 1)
    {
        throw nicta::nicta_exception("ratio to file start must be in range [0, 1]", "video_server<T, U>", "video_server<T, U>");
    }
    ::cvSetCaptureProperty(this->stream_capture_, CV_CAP_PROP_POS_AVI_RATIO, start.operator()<double>());
}


template < typename T
         , template <typename T_> class U
         >
template < typename nicta::vibe::video::DriverType driver
         , unsigned short int camera_index
         >
nicta::vibe::video::video_server<T, U> :: 
video_server(nicta::vibe::video::camera_device<driver, camera_index> camera,
             const char* id,
             nicta::quantity::frame_rate_per_second<unsigned int> fps,
             const char* fourcc_codec,
             nicta::quantity::width<unsigned int> width,
             nicta::quantity::height<unsigned int> height
            ) throw (nicta::nicta_exception)
: id_(id)
, payload_(0)
, stream_init_(false)
, stream_eof_(false)
, imagecapture_(false)
, imagefiles_count_(0)
, filecapture_(false)
, stream_capture_(::cvCreateCameraCapture(static_cast<int> (camera.index)))
, pgr_stream_capture_(0)
, read_ptr_(1)
, buffer_(0)
, controller_(0)
, fps_(fps.template operator()<double>())
, fps_waiter_(false)
, stream_length_(1)
, stream_type_(nicta::vibe::video::Camera)
{
    // Constructor - for reading Video streams
    // NOTE:  if fps is 0, the camera device will be queried for its FPS
    // NOTE:  on some machines, the V4L2 driver will error when trying to query CV_CAP_PROP_FPS - 
    //        ...in such cases, we need to set the FPS manually (as provided for by this ctor) !!
    // NOTE:  for thread-aware setups (**next** ctor), we can choose to set the fps via controller/mediator

    this->mt_buffer_[0] = 0;
    this->mt_buffer_[1] = 0;

    if (!this->stream_capture_)
    {
        std::string msg  = "\n";
                    msg += "  >> Character device driver /dev/video1394/<port> is listening on another channel.\n";
                    msg += "  >> Your operating system could not initialize the device.\n";
                    msg += "  >> Please try again later.\n";

        throw nicta::nicta_exception(msg, "video_server", "video_server");
    }

    if (this->id_.empty())
    {
        this->id_ = nicta::stringutil::atomic_to_string(this);
    }

    if (width.operator()< >())
    {
        ::cvSetCaptureProperty(this->stream_capture_, CV_CAP_PROP_FRAME_WIDTH, width.operator()<double>());
    }
    if (height.operator()< >())
    {
        ::cvSetCaptureProperty(this->stream_capture_, CV_CAP_PROP_FRAME_HEIGHT, height.operator()<double>());
    }
    if (std::string(fourcc_codec).size() && nicta::vibe::video::algorithm::detail::fourcc_assocMap.has_key(fourcc_codec))
    {
        ::cvSetCaptureProperty(this->stream_capture_, CV_CAP_PROP_FOURCC, nicta::vibe::video::algorithm::detail::fourcc_assocMap(fourcc_codec));
    }
}


template < typename T
         , template <typename T_> class U
         >
template < typename nicta::vibe::video::DriverType driver
         , unsigned short int camera_index
         >
nicta::vibe::video::video_server<T, U> :: 
video_server(nicta::vibe::video::controller<T, U>* mediator,
             nicta::vibe::video::camera_device<driver, camera_index> camera,
             const char* id,
             const char* fourcc_codec,
             nicta::quantity::width<unsigned int> width,
             nicta::quantity::height<unsigned int> height
            ) throw (nicta::nicta_exception)
: id_(id)
, payload_(0)
, stream_init_(false)
, stream_eof_(false)
, imagecapture_(false)
, imagefiles_count_(0)
, filecapture_(false)
, stream_capture_(::cvCreateCameraCapture(static_cast<int> (camera.index)))
, pgr_stream_capture_(0)
, read_ptr_(1)
, buffer_(0)
, controller_(mediator)
, fps_(mediator->fps())
, fps_waiter_(false)
, stream_length_(1)
, stream_type_(nicta::vibe::video::Camera)
{
    // Constructor - for reading Video streams
    // NOTE:  thread-aware variant..

    this->mt_buffer_[0] = 0;
    this->mt_buffer_[1] = 0;

    if (!this->stream_capture_)
    {
        std::string msg  = "\n";
                    msg += "  >> Character device driver /dev/video1394/<port> is listening on another channel.\n";
                    msg += "  >> Your operating system could not initialize the device.\n";
                    msg += "  >> Please try again later.\n";

        throw nicta::nicta_exception(msg, "video_server", "video_server");
    }

    if (this->id_.empty())
    {
        this->id_ = nicta::stringutil::atomic_to_string(this);
    }

    if (width.operator()< >())
    {
        ::cvSetCaptureProperty(this->stream_capture_, CV_CAP_PROP_FRAME_WIDTH, width.operator()<double>());
    }
    if (height.operator()< >())
    {
        ::cvSetCaptureProperty(this->stream_capture_, CV_CAP_PROP_FRAME_HEIGHT, height.operator()<double>());
    }
    if (this->controller_->fps())
    {
        ::cvSetCaptureProperty(this->stream_capture_, CV_CAP_PROP_FPS, this->controller_->fps());
    }
    if (std::string(fourcc_codec).size() && nicta::vibe::video::algorithm::detail::fourcc_assocMap.has_key(fourcc_codec))
    {
        ::cvSetCaptureProperty(this->stream_capture_, CV_CAP_PROP_FOURCC, nicta::vibe::video::algorithm::detail::fourcc_assocMap(fourcc_codec));
    }
}



#ifdef NICTA_PGR

template < typename T
         , template <typename T_> class U
         >
template <unsigned short int camera_index>
nicta::vibe::video::video_server<T, U> :: 
video_server(nicta::vibe::video::pgr::camera_device<camera_index>& camera,
             const char* id
            ) throw (nicta::nicta_exception)
: id_(id)
, payload_(0)
, stream_init_(false)
, stream_eof_(false)
, imagecapture_(false)
, imagefiles_count_(0)
, filecapture_(false)
, stream_capture_(0)
, pgr_stream_capture_(&camera.stream_())
, read_ptr_(1)
, buffer_(0)
, controller_(0)
, fps_(camera.frame_rate())
, fps_waiter_(false)
, stream_length_(1)
, stream_type_(nicta::vibe::video::Camera)
{
    // Constructor - for connecting to a PGR ***stereo*** camera

    this->mt_buffer_[0] = 0;
    this->mt_buffer_[1] = 0;

    if (this->id_.empty())
    {
        this->id_ = nicta::stringutil::atomic_to_string(this);
    }
}


template < typename T
         , template <typename T_> class U
         >
template <unsigned short int camera_index>
nicta::vibe::video::video_server<T, U> :: 
video_server(nicta::vibe::video::controller<T, U>* mediator,
             nicta::vibe::video::pgr::camera_device<camera_index>& camera,
             const char* id
            ) throw (nicta::nicta_exception)
: id_(id)
, payload_(0)
, stream_init_(false)
, stream_eof_(false)
, imagecapture_(false)
, imagefiles_count_(0)
, filecapture_(false)
, stream_capture_(0)
, pgr_stream_capture_(&camera.stream_())
, read_ptr_(1)
, buffer_(0)
, controller_(mediator)
, fps_(mediator->fps())
, fps_waiter_(false)
, stream_length_(1)
, stream_type_(nicta::vibe::video::Camera)
{
    // Constructor - for connecting to a PGR ***stereo*** camera
    // NOTE:  thread-aware variant..

    this->mt_buffer_[0] = 0;
    this->mt_buffer_[1] = 0;

    if (this->id_.empty())
    {
        this->id_ = nicta::stringutil::atomic_to_string(this);
    }
}

#endif



template < typename T
         , template <typename T_> class U
         >
typename nicta::vibe::video::video_server<T, U>::result_type
nicta::vibe::video::video_server<T, U> :: 
operator()() throw (nicta::nicta_exception, std::bad_exception)
{
    // Start video capture from device, waiting td-seconds before streaming into this->cbuf_
    // ... applying the identity functor - which doesn't alter the incoming image
    // ... and applying identity deinterlacing functor - which doesn't alter the incoming image

    nicta::vibe::video::algorithm::canonical_identity ftor;
    nicta::vibe::video::algorithm::deinterlace_identity<1U, T, U> dftor;
    this->operator()< nicta::vibe::video::algorithm::canonical_identity
                    , 1U
                    , nicta::vibe::video::algorithm::deinterlace_identity
                    >(ftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template <typename Functor>
typename nicta::vibe::video::video_server<T, U>::result_type
nicta::vibe::video::video_server<T, U> :: 
operator()(Functor& ftor) throw (nicta::nicta_exception, std::bad_exception)
{
    // Start video capture from device, waiting td-seconds before streaming into this->cbuf_
    // ... applying user-defined server-side 'work' functor to source data
    // ... and applying identity deinterlacing functor - which doesn't alter the incoming image

    nicta::vibe::video::algorithm::deinterlace_identity<1U, T, U> dftor;
    this->operator()< Functor
                    , 1U
                    , nicta::vibe::video::algorithm::deinterlace_identity
                    >(ftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < unsigned int Payload
         , template <unsigned int Payload_, typename D_, template <typename _D> class P_> class Deinterlace
         >
typename nicta::vibe::video::video_server<T, U>::result_type
nicta::vibe::video::video_server<T, U> :: 
operator()(Deinterlace<Payload, T, U>& dftor) throw (nicta::nicta_exception, std::bad_exception)
{
    // Start video capture from device, waiting td-seconds before streaming into this->cbuf_
    // ... applying the identity functor - which doesn't alter the incoming image
    // ... and applying user-defined deinterlacing functor to source data

    BOOST_STATIC_ASSERT(( Payload > 0 ));

    nicta::vibe::video::algorithm::canonical_identity ftor;
    this->operator()< nicta::vibe::video::algorithm::canonical_identity
                    , Payload
                    , Deinterlace
                    >(ftor, dftor);
}


template < typename T
         , template <typename T_> class U
         >
template < typename Functor
         , unsigned int Payload
         , template <unsigned int Payload_, typename D_, template <typename _D> class P_> class Deinterlace
         >
typename nicta::vibe::video::video_server<T, U>::result_type
nicta::vibe::video::video_server<T, U> :: 
operator()(Functor& ftor, Deinterlace<Payload, T, U>& dftor) throw (nicta::nicta_exception, std::bad_exception)
{
    // Start video capture from device, waiting td-seconds before streaming into this->cbuf_


    BOOST_STATIC_ASSERT(( Payload > 0 ));

    if (!this->payload_)
    {
        this->payload_ = Payload;
        if (!this->controller_)
        {
            // for thread-unaware state..
            this->buffer_ = new typename nicta::vibe::video::sp_iplframe::type[this->payload_];
        }
        else
        {
            // for a thread-aware state..
            this->mt_buffer_[0] = new typename nicta::vibe::video::sp_iplframe::type[this->payload_];
            this->mt_buffer_[1] = new typename nicta::vibe::video::sp_iplframe::type[this->payload_];
        }
    }

    if (!this->controller_)
    {
        // for thread-unaware state..

        ::IplImage* frame = 0;
        bool flycap = false;
#ifdef NICTA_PGR
        FlyCapture2::Image flyimage;
#endif
        nicta::vibe::image::image<T, U>* img = 0;
        if (this->stream_capture_ || this->imagecapture_ || this->pgr_stream_capture_)
        {
            if (this->stream_capture_)
            {
                // NOTE:  should *never* delete the ptr 'frame' - will go when dtor called on this->stream_capture_

                frame = ::cvQueryFrame(this->stream_capture_);
#ifdef NICTA_3CH_GRAYSCALE_CAMERAS
                if (U<T>::components == 1 && frame->nChannels == 3)
                {
                    // Crappy hardware:  it's a grayscale image, but it comes in from the camera as 3-channel grayscale.. convert into 1-channel grayscale
                    // -OR-
                    // [bonus *feature* NOT to be documented!]:  it's a color image, and we indicated that we want a grayscale stream.. convert into 1-channel grayscale

                    nicta::vibe::image::image<T, nicta::vibe::image::bgr_p> cimg(frame);
                    img   = new nicta::vibe::image::image<T, U> (*cimg.template convert<U>());
                    frame = img->template native_handle< >();
                }
#endif
            }
            else if (this->pgr_stream_capture_)
            {
#ifdef NICTA_PGR
                flycap = nicta::vibe::video::pgr::pgr::query_frame(*this->pgr_stream_capture_, flyimage);
#endif
            }
            else // this->imagecapture_ == true
            {
                // NOTE:  ptr 'img' *must* be deleted! (not 'frame' however, ..obviously.. since it is managed by 'img'!)

                if (this->imagefiles_.size())
                {
                    img   = new nicta::vibe::image::image<T, U>(this->imagefiles_.begin()->c_str());
                    frame = img->template native_handle< >();
                    this->imagefiles_.erase(this->imagefiles_.begin());
                }
            }

            if (frame || flycap)
            {
                if (!this->fps_)
                {
                    if (!this->pgr_stream_capture_)
                    {
                        this->fps_ = ::cvGetCaptureProperty(this->stream_capture_, CV_CAP_PROP_FPS);
                    }
                    else
                    {
#ifdef NICTA_PGR
                        this->fps_ = nicta::vibe::video::pgr::pgr::query_frame_rate(*this->pgr_stream_capture_);
#endif
                    }
                }
                boost::posix_time::time_duration period = nicta::util::fps_period(this->fps_);


                this->stream_init_ = true;                       // flag that we have begun stream capture
                if (this->online())
                {
                    if (!this->pgr_stream_capture_)
                    {
                        ::IplImage* frame_copy = ::cvCloneImage(frame);    // may seg-fault here when kill window sometimes (eg, half-way through copy..)

                        // for cases where either:  this->imagecapture_ == true || (U<T>::components == 1 && frame->nChannls == 3) ..
                        delete img;
                        img = 0;

                        // cvCopy automatically flips BL-origin grab to a TL-origin image => no need for cvFlip :-)
                        //::cvCopy(frame, frame_copy, 0);

                        boost::array<boost::shared_ptr< ::IplImage >, Payload>& data =
                             dftor(frame_copy).template nicta::vibe::video::algorithm::deinterlace_identity<Payload, T, U>::operator()();

                        boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
                        for (unsigned int pidx = 0; pidx != Payload; ++pidx)
                        {
                            // Payload:  number of interlaced images in source  [ always =1, for non-interlaced sources ]
                            // pidx:  zero-based index of de-interlaced image   [ always =0, for non-interlaced sources ]
                            // stream_position:  zero-based index of nth-frame received from source
                            // now:  timestamp

                            // apply functor to frame_copy, and push_back
                            if (!boost::is_same<Deinterlace<Payload, T, U>, nicta::vibe::video::algorithm::deinterlace_identity<Payload, T, U> >::value)
                            {
                                this->buffer_[pidx] = typename nicta::vibe::video::sp_iplframe::type
                                                                                   (new typename nicta::vibe::video::iplframe::type(&*ftor(&*data[pidx]),
                                                                                                                                    Payload,
                                                                                                                                    pidx,
                                                                                                                                    this->stream_position_(),
                                                                                                                                    now),
                                                                                    nicta::null_deleter());
                            }
                            else
                            {
                                this->buffer_[pidx] = typename nicta::vibe::video::sp_iplframe::type
                                                                                   (new typename nicta::vibe::video::iplframe::type(&*ftor(&*data[pidx]),
                                                                                                                                    Payload,
                                                                                                                                    pidx,
                                                                                                                                    this->stream_position_(),
                                                                                                                                    now),
                                                                                    nicta::vibe::video::iplimage_deleter());
                            }
                        }
                        if (!boost::is_same<Deinterlace<Payload, T, U>, nicta::vibe::video::algorithm::deinterlace_identity<Payload, T, U> >::value)
                        {
                            ::cvReleaseImage(&frame_copy);
                            frame_copy = 0;
                        }


                        // wait till next available frame
                        //
                        //nicta::util::sleep(period);
                        //
                    }
                    else // this->pgr_stream_capture_ != 0
                    {
#ifdef NICTA_PGR
                        boost::array<boost::shared_ptr< ::IplImage >, Payload>& data =
                             dftor(&flyimage).template nicta::vibe::video::algorithm::deinterlace_identity<Payload, T, U>::operator()();

                        boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
                        for (unsigned int pidx = 0; pidx != Payload; ++pidx)
                        {
                            // Payload:  number of interlaced images in source  [ always =1, for non-interlaced sources ]
                            // pidx:  zero-based index of de-interlaced image   [ always =0, for non-interlaced sources ]
                            // stream_position:  zero-based index of nth-frame received from source
                            // now:  timestamp

                            // apply functor to flyimage, and push_back
                            this->buffer_[pidx] = typename nicta::vibe::video::sp_iplframe::type
                                                                               (new typename nicta::vibe::video::iplframe::type(&*ftor(&*data[pidx]),
                                                                                                                                Payload,
                                                                                                                                pidx,
                                                                                                                                this->stream_position_(),
                                                                                                                                now),
                                                                                nicta::null_deleter());
                        }
#endif
                    }
                }
            }
            else
            {
                this->stream_eof_ = true;
            }
        }
        return;
    }



    // for a thread-aware state..

    ::IplImage* frame = 0;
    bool flycap = false;
#ifdef NICTA_PGR
    FlyCapture2::Image flyimage;
#endif
    nicta::vibe::image::image<T, U>* img = 0;
    if (this->stream_capture_ || this->imagecapture_ || this->pgr_stream_capture_)
    {
        if (this->stream_capture_)
        {
            // NOTE:  should *never* delete the ptr 'frame' - will go when dtor called on this->stream_capture_

            //::IplImage* frame = ::cvQueryFrame(this->stream_capture_);
            frame = nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                           , boost::mutex
                                           >::instance<nicta::vibe::video::thread::multiton_model::server>()->operator()< ::IplImage *
                                                                                                                        , ::CvCapture *
                                                                                                                        , boost::unique_lock
                                                                                                                        >(&::cvQueryFrame,
                                                                                                                          this->stream_capture_);
#ifdef NICTA_3CH_GRAYSCALE_CAMERAS
            if (U<T>::components == 1 && frame->nChannels == 3)
            {
                // Crappy hardware:  it's a grayscale image, but it comes in from the camera as 3-channel grayscale.. convert into 1-channel grayscale
                // -OR-
                // [bonus *feature* NOT to be documented!]:  it's a color image, and we indicated that we want a grayscale stream.. convert into 1-channel grayscale

                nicta::vibe::image::image<T, nicta::vibe::image::bgr_p> cimg(frame);
                img   = new nicta::vibe::image::image<T, U> (*cimg.template convert<U>());
                frame = img->template native_handle< >();
            }
#endif
        }
        else if (this->pgr_stream_capture_)
        {
#ifdef NICTA_PGR
            flycap = nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                            , boost::mutex
                                            >::instance<nicta::vibe::video::thread::multiton_model::server>()->operator()< bool
                                                                                                                         , FlyCapture2::Camera &
                                                                                                                         , FlyCapture2::Image &
                                                                                                                         , boost::unique_lock
                                                                                                                         >(&nicta::vibe::video::pgr::pgr::query_frame,
                                                                                                                           *this->pgr_stream_capture_,
                                                                                                                           flyimage);
#endif
        }
        else // this->imagecapture_ == true
        {
            // NOTE:  ptr 'img' *must* be deleted! (not 'frame' however, ..obviously.. since it is managed by 'img'!)

            img = nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                         , boost::mutex
                                         >::instance<nicta::vibe::video::thread::multiton_model::server>()->operator()< nicta::vibe::image::image<T, U>
                                                                                                                      , const char *
                                                                                                                      , boost::unique_lock
                                                                                                                      >(this->imagefiles_.begin()->c_str());
            frame = img->template native_handle< >();
            this->imagefiles_.erase(this->imagefiles_.begin());
        }

        if (frame || flycap)
        {
            if (!this->fps_ || this->imagecapture_)
            {
                if (!this->fps_)
                {
                    if (!this->pgr_stream_capture_)
                    {
                        this->fps_ = ::cvGetCaptureProperty(this->stream_capture_, CV_CAP_PROP_FPS);
                    }
                    else
                    {
#ifdef NICTA_PGR
                        this->fps_ = nicta::vibe::video::pgr::pgr::query_frame_rate(*this->pgr_stream_capture_);
#endif
                    }
                }
                this->fps_waiter().template operator()<nicta::thread::NotifyOne>(true);            // signal controller
            }
            boost::posix_time::time_duration period = nicta::util::fps_period(this->fps_);


            bool flag = false;
            this->stream_init_ = true;                       // flag that we have begun stream capture
            while (this->online())
            {
                if (!this->pgr_stream_capture_)
                {
                    ::IplImage* frame_copy = ::cvCloneImage(frame);    // may seg-fault here when kill window sometimes (eg, half-way through copy..)

                    // for cases where either:  this->imagecapture_ == true || (U<T>::components == 1 && frame->nChannls == 3) ..
                    // call the destructor
                    nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                           , boost::mutex
                                           >::instance<nicta::vibe::video::thread::multiton_model::server>()->operator()< nicta::vibe::image::image<T, U>
                                                                                                                        , boost::unique_lock
                                                                                                                        >(img);
                    img = 0;

                    // cvCopy automatically flips BL-origin grab to a TL-origin image => no need for cvFlip :-)
                    //::cvCopy(frame, frame_copy, 0);

                    // it may be a "read-lock" & "shared", but there will only ever be one table we connect to .. so it works!
                    // .. table on the other hand will acquire the gate as a unique write-lock so can have chance to read from servers prior to any updates.
                    typename nicta::thread::read_lock::type glock =
                             nicta::thread::gate< >::template instance<nicta::vibe::video::thread::gate_model::shared>()->operator()().template operator()
                                                                                                                                          < nicta::thread::Read
                                                                                                                                          , boost::shared_lock
                                                                                                                                          >();

                    // apply functor to frame_copy, and push_back
                    boost::array<boost::shared_ptr< ::IplImage >, Payload>& data =
                         dftor(frame_copy).template nicta::vibe::video::algorithm::deinterlace_identity<Payload, T, U>::operator()();

                    unsigned int idx = (this->read_ptr_ + 1) % 2;
                    boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
                    for (unsigned int pidx = 0; pidx != Payload; ++pidx)
                    {
                        // Payload:  number of interlaced images in source  [ always =1, for non-interlaced sources ]
                        // pidx:  zero-based index of de-interlaced image   [ always =0, for non-interlaced sources ]
                        // stream_position:  zero-based index of nth-frame received from source
                        // now:  timestamp

                        if (!boost::is_same<Deinterlace<Payload, T, U>, nicta::vibe::video::algorithm::deinterlace_identity<Payload, T, U> >::value)
                        {
                            this->mt_buffer_[idx][pidx] = typename nicta::vibe::video::sp_iplframe::type
                                                                                       (new typename nicta::vibe::video::iplframe::type(&*ftor(&*data[pidx]),
                                                                                                                                        Payload,
                                                                                                                                        pidx,
                                                                                                                                        this->stream_position_(),
                                                                                                                                        now),
                                                                                        nicta::null_deleter());
                        }
                        else
                        {
                            this->mt_buffer_[idx][pidx] = typename nicta::vibe::video::sp_iplframe::type
                                                                                       (new typename nicta::vibe::video::iplframe::type(&*ftor(&*data[pidx]),
                                                                                                                                        Payload,
                                                                                                                                        pidx,
                                                                                                                                        this->stream_position_(),
                                                                                                                                        now),
                                                                                        nicta::vibe::video::iplimage_deleter());
                        }
                    }
                    this->read_ptr_ = idx;
                    if (!boost::is_same<Deinterlace<Payload, T, U>, nicta::vibe::video::algorithm::deinterlace_identity<Payload, T, U> >::value)
                    {
                        ::cvReleaseImage(&frame_copy);
                        frame_copy = 0;
                    }


                    nicta::thread::gate< >::template instance<nicta::vibe::video::thread::gate_model::shared>()->operator()().template operator()
                                                                                                                                 < nicta::thread::NotifyOne
                                                                                                                                 , boost::shared_lock
                                                                                                                                 >(glock);
                }
                else // this->pgr_stream_capture_ != 0
                {
#ifdef NICTA_PGR
                    // it may be a "read-lock" & "shared", but there will only ever be one table we connect to .. so it works!
                    // .. table on the other hand will acquire the gate as a unique write-lock so can have chance to read from servers prior to any updates.
                    typename nicta::thread::read_lock::type glock =
                             nicta::thread::gate< >::template instance<nicta::vibe::video::thread::gate_model::shared>()->operator()().template operator()
                                                                                                                                          < nicta::thread::Read
                                                                                                                                          , boost::shared_lock
                                                                                                                                          >();

                    // apply functor to frame_copy, and push_back
                    boost::array<boost::shared_ptr< ::IplImage >, Payload>& data =
                         dftor(&flyimage).template nicta::vibe::video::algorithm::deinterlace_identity<Payload, T, U>::operator()();

                    unsigned int idx = (this->read_ptr_ + 1) % 2;
                    boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
                    for (unsigned int pidx = 0; pidx != Payload; ++pidx)
                    {
                        // Payload:  number of interlaced images in source  [ always =1, for non-interlaced sources ]
                        // pidx:  zero-based index of de-interlaced image   [ always =0, for non-interlaced sources ]
                        // stream_position:  zero-based index of nth-frame received from source
                        // now:  timestamp

                        this->mt_buffer_[idx][pidx] = typename nicta::vibe::video::sp_iplframe::type
                                                                                   (new typename nicta::vibe::video::iplframe::type(&*ftor(&*data[pidx]),
                                                                                                                                    Payload,
                                                                                                                                    pidx,
                                                                                                                                    this->stream_position_(),
                                                                                                                                    now),
                                                                                    nicta::null_deleter());
                    }
                    this->read_ptr_ = idx;


                    nicta::thread::gate< >::template instance<nicta::vibe::video::thread::gate_model::shared>()->operator()().template operator()
                                                                                                                                 < nicta::thread::NotifyOne
                                                                                                                                 , boost::shared_lock
                                                                                                                                 >(glock);
#endif
                }


                // NOTE:  when frame-grabbing, openCV returns image with BL-origin, and on copy flips it automatically to TL-origin
                if (this->stream_capture_)
                {
                    // NOTE:  should *never* delete the ptr 'frame' - will go when dtor called on this->stream_capture_

                    //frame = ::cvQueryFrame(this->stream_capture_);
                    frame = nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                                   , boost::mutex
                                                   >::instance<nicta::vibe::video::thread::multiton_model::server>()->operator()< ::IplImage *
                                                                                                                                , ::CvCapture *
                                                                                                                                , boost::unique_lock
                                                                                                                                >(&::cvQueryFrame,
                                                                                                                                  this->stream_capture_);
                }
                else if (this->pgr_stream_capture_)
                {
#ifdef NICTA_PGR
                    flycap = nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                                    , boost::mutex
                                                    >::instance<nicta::vibe::video::thread::multiton_model::server>()->operator()< bool
                                                                                                                                 , FlyCapture2::Camera &
                                                                                                                                 , FlyCapture2::Image &
                                                                                                                                 , boost::unique_lock
                                                                                                                                 >(&nicta::vibe::video::pgr::pgr::query_frame,
                                                                                                                                   *this->pgr_stream_capture_,
                                                                                                                                   flyimage);
#endif
                }
                else // this->imagecapture_ == true
                {
                    if (this->imagefiles_.size())
                    {
                        // NOTE:  ptr 'img' *must* be deleted! (not 'frame' however, ..obviously.. since it is managed by 'img'!)

                        img = nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                                 , boost::mutex
                                                 >::instance<nicta::vibe::video::thread::multiton_model::server>()->operator()
                                                                                                                      < nicta::vibe::image::image<T, U>
                                                                                                                      , const char *
                                                                                                                      , boost::unique_lock
                                                                                                                      >(this->imagefiles_.begin()->c_str());
                        frame = img->template native_handle< >();
                        this->imagefiles_.erase(this->imagefiles_.begin());
                    }
                    else
                    {
                        frame = 0;
                    }
                }

                if (!frame && !flycap)
                {
                    nicta::thread::guard<boost::shared_mutex, boost::unique_lock> lock(this->eof_mutex_);

                    this->stream_eof_ = true;             // flag that stream capture has finished
                    break;
                }

                if (!flag)
                {
                    this->controller_->register_server_online(this->id());
                    flag = true;
                }
                

                // wait till next available frame
                nicta::util::sleep(period);
            }
        }
    }
}


template < typename T
         , template <typename T_> class U
         >
void
nicta::vibe::video::video_server<T, U> :: 
shutdown() throw ()
{
    // Used by Controller to ask Server to shutdown
    // NOTE:  for a threaded video_server..

    nicta::thread::guard<boost::shared_mutex, boost::shared_lock> lock(this->eof_mutex_);
    this->stream_eof_ = true;
}


template < typename T
         , template <typename T_> class U
         >
unsigned long int
nicta::vibe::video::video_server<T, U> :: 
held_frame_index() const throw ()
{
    // Return the index of the frame currently held by this->read_ptr_
    // NOTE:  will **always** return 0 for camera connections!  ...only makes sense for file-connections!!

    if (!this->controller_)
    {
        return this->buffer_[0]->template get<nicta::vibe::video::FrameIndex>();
    }
    return this->mt_buffer_[this->read_ptr_][0]->template get<nicta::vibe::video::FrameIndex>();
}


template < typename T
         , template <typename T_> class U
         >
const typename nicta::vibe::video::sp_iplframe::type *
nicta::vibe::video::video_server<T, U> :: 
tx() const throw ()
{
    // Transmit a frame packet

    if (!this->controller_)
    {
        return this->buffer_;
    }
    return this->mt_buffer_[this->read_ptr_];
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::video::video_server<T, U> :: 
online() const throw ()
{
    // Return True if stream having been initialized, is still streaming data

    nicta::thread::guard<boost::shared_mutex, boost::shared_lock> lock(this->eof_mutex_);

    static bool result = true;
    if (!!(this->stream_init_ && this->stream_eof_) && result)
    {
        // essentially want to pick up one last frame after eof set... so we can pick up last frame via controller
        // .. will need to test if controller on different sleep time than server, if continues to work ????

        result = false;
        return true;
    }

    return !(this->stream_init_ && this->stream_eof_);
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::video::video_server<T, U> :: 
offline() const throw ()
{
    // Return True if stream having been initialized, has now closed

    return !this->online();
}


template < typename T
         , template <typename T_> class U
         >
const std::string &
nicta::vibe::video::video_server<T, U> :: 
id() const throw ()
{
    // Return the string identifier associated with this video server

    return this->id_;
}


template < typename T
         , template <typename T_> class U
         >
unsigned int
nicta::vibe::video::video_server<T, U> :: 
payload() const throw ()
{
    // Return number of interleaved images we receive from source
    // NOTE:  we don't know this until first call of op()(), where use indicates with a template-parameter how many to expect (default = 1)

    return this->payload_;
}


template < typename T
         , template <typename T_> class U
         >
double
nicta::vibe::video::video_server<T, U> :: 
fps() const throw ()
{
    // Return fps of this server
    // NOTE: if fps == 0 at controller, then we only know this quantity *after* initiating the server's thread!

    return this->fps_;
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::video::video_server<T, U> :: 
fps_available() const throw ()
{
    // Return the waiter state
    // NOTE:  for a threaded video_server..

    return this->fps_waiter_();
}


template < typename T
         , template <typename T_> class U
         >
nicta::thread::condition_wait<boost::mutex> &
nicta::vibe::video::video_server<T, U> :: 
fps_waiter() throw ()
{
    // Return waiter object through which we query fps data
    // NOTE:  for a threaded video_server..

    return this->fps_waiter_;
}


template < typename T
         , template <typename T_> class U
         >
unsigned long int
nicta::vibe::video::video_server<T, U> :: 
stream_length() const throw ()
{
    // Get total number of frames available in the stream *at* connection-time
    // NOTE:  will **always** return 1 for camera connections!  ...only makes sense for file-connections!!

    return this->stream_length_;
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::video::StreamModel
nicta::vibe::video::video_server<T, U> :: 
stream_type() const throw ()
{
    // Return the source-stream type:  Image(dir of img files) ; File(avi files) ; Camera(camera feed)

    return this->stream_type_;
}


template < typename T
         , template <typename T_> class U
         >
unsigned int
nicta::vibe::video::video_server<T, U> :: 
stream_position_() const throw ()
{
    // Get the ZERO-based frame index to be yielded *after* the last call to ::cvQueryFrame [= cvGrabFrame + cvRetrieveFrame]
    // ...hence, the zero-based index to the frame in the stream
    // NOTE:  will **always** return 0 for camera connections!  ...only makes sense for file-connections!!

    if (this->pgr_stream_capture_)
    {
#ifdef NICTA_PGR
        return 0;
#endif
    }
    if (this->imagecapture_)
    {
        return this->imagefiles_count_ - this->imagefiles_.size();
    }
    return ::cvGetCaptureProperty(this->stream_capture_, CV_CAP_PROP_POS_FRAMES);
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
