/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_ALGORITHM_RECORD_STREAM_H
#define NICTA_VIBE_VIDEO_ALGORITHM_RECORD_STREAM_H

#include "video/algorithm/detail/fourcc.h"
#include "video/thread/multiton_model.h"

#include <image/image.h>
#include <image/image_depth.h>
#include <image/image_pixel.h>
#include <image/models/colorcode_model.h>
#include <thread/multiton/multiton.h>
#include <nicta_common/nicta_exception.h>
#include <nicta_common/quantity.h>

#include <opencv/highgui.h>

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

#include <set>
#include <string>


namespace nicta {
namespace vibe {
namespace video {
namespace algorithm {

    template < typename Depth = nicta::vibe::image::depth_8u
             , template <typename> class Pixel = nicta::vibe::image::bgr_p
             >
    class record_stream : boost::noncopyable
    {
    public:

        ~record_stream() throw ();

        record_stream(const char* filename,
                      const char* fourcc_codec,
                      nicta::quantity::frame_rate_per_second<unsigned int> fps,
                      nicta::quantity::width<unsigned int> width =
                           nicta::quantity::width<unsigned int>(0),
                      nicta::quantity::height<unsigned int> height =
                           nicta::quantity::height<unsigned int>(0)
                     ) throw (nicta::nicta_exception);


        template < typename Depth_
                 , template <typename> class Pixel_
                 , template <typename, template <typename> class> class C_
                 >
        bool operator()(const boost::shared_ptr<nicta::vibe::image::image<Depth, Pixel> > frame,
                        boost::posix_time::ptime timestamp,
                        unsigned long int frame_index,
                        unsigned int payload_index,
                        unsigned int payload,
                        const std::string& source_id,
                        const std::string& host_id,
                        unsigned int host_connections,
                        std::set<std::string>& window_tags,
                        void (C_<Depth_, Pixel_>::*shutdown)() const throw (),
                        const C_<Depth_, Pixel_>* const host
                       ) throw (nicta::nicta_exception);


    private:

        template < typename Depth_
                 , template <typename> class Pixel_
                 , template <typename T, template <typename > class> class C_
                 >
        bool op_(const nicta::vibe::image::image<Depth, Pixel>& frame,
                 boost::posix_time::ptime timestamp,
                 unsigned long int frame_index,
                 unsigned int payload_index,
                 unsigned int payload,
                 const std::string& source_id,
                 const std::string& host_id,
                 unsigned int host_connections,
                 std::set<std::string>& window_tags,
                 void (C_<Depth_, Pixel_>::*shutdown)() const throw (),
                 const C_<Depth_, Pixel_>* const host
                ) throw (nicta::nicta_exception);


        ::CvVideoWriter* ostream_;

        const char* filename_;
        int codec_;
        double fps_;
        mutable int width_;
        mutable int height_;
    };

} } } } // namespace nicta::vibe::video::algorithm



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename T
         , template <typename> class U
         >
nicta::vibe::video::algorithm::record_stream<T, U> :: 
~record_stream() throw ()
{
    // Destructor

    if (this->ostream_)
    {
        ::cvReleaseVideoWriter(&this->ostream_);
        this->ostream_ = 0;
    }
}


template < typename T
         , template <typename> class U
         >
nicta::vibe::video::algorithm::record_stream<T, U> :: 
record_stream(const char* filename,
             const char* fourcc_codec,
             nicta::quantity::frame_rate_per_second<unsigned int> fps,
             nicta::quantity::width<unsigned int> width,
             nicta::quantity::height<unsigned int> height
            ) throw (nicta::nicta_exception)
try
: ostream_(0)
, filename_(filename)
, codec_(nicta::vibe::video::algorithm::detail::fourcc_assocMap(fourcc_codec))
, fps_(fps.operator()<double>())
, width_(width.operator()<int>())
, height_(height.operator()<int>())
{
    // Constructor

    if (!this->fps_)
    {
        throw nicta::nicta_exception("Invalid quantity (0) for stream frame rate", "record_stream<T, U>", "record_stream<T, U>");
    }

    if (this->width_ && this->height_)
    {
        int is_color   = !!(static_cast<int> (U<T>::encoding) != nicta::vibe::image::color_code_model<nicta::vibe::image::GRAY>::value);
        this->ostream_ = ::cvCreateVideoWriter(this->filename_,
                                               this->codec_,
                                               this->fps_,
                                               ::cvSize(this->width_, this->height_),
                                               is_color);
    }
}
catch (const nicta::nicta_exception& e)
{
    if (nicta::vibe::video::algorithm::detail::fourcc_assocMap.has_key(fourcc_codec))
    {
        throw nicta::nicta_exception(e.descriptor(), "record_stream<T, U>", "record_stream<T, U>");
    }
    else
    {
        throw nicta::nicta_exception("Unrecognized FourCC codec", "record_stream<T, U>", "record_stream<T, U>");
    }
}


template < typename T
         , template <typename> class U
         >
template < typename T_
         , template <typename> class U_
         , template <typename, template <typename> class> class C_
         >
bool
nicta::vibe::video::algorithm::record_stream<T, U> :: 
operator()(const boost::shared_ptr<nicta::vibe::image::image<T, U> > frame,
           boost::posix_time::ptime timestamp,
           unsigned long int frame_index,
           unsigned int payload_index,
           unsigned int payload,
           const std::string& source_id,
           const std::string& host_id,
           unsigned int host_connections,
           std::set<std::string>& window_tags,
           void (C_<T_, U_>::*shutdown)() const throw (),
           const C_<T_, U_>* const host
          ) throw (nicta::nicta_exception)
{
    // This is a thin wrapper over op_()(.) to avoid two multiton calls.. [ one for each of:  ::cvCreateVideoWriter ; ::cvWriteFrame ]

    // Writes frame to the output stream
    // NOTE:  if ROI is set, then ROI will be written to output stream instead!
    // NOTE:  return value is just a convenience, and is an implementation detail that should be ignored by the common user
    //        .. power-user:  can use return value to kill connection of this client with specific source
    // NOTE:  client | shutdown arguments can be used to conditionally terminate the client entirely

    // return this->op_(*frame, timestamp, frame_index, source_id, host_id, host_connections, shutdown, host);
    return nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                  , boost::mutex
                                  >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                               < bool
                                                                               , nicta::vibe::video::algorithm::record_stream<T, U>
                                                                               , const nicta::vibe::image::image<T, U> &
                                                                               , boost::posix_time::ptime
                                                                               , unsigned long int
                                                                               , unsigned int
                                                                               , unsigned int
                                                                               , const std::string &
                                                                               , const std::string &
                                                                               , unsigned int
                                                                               , std::set<std::string> &
                                                                               , void (C_<T_, U_>::*)() const throw ()
                                                                               , const C_<T_, U_>* const
                                                                               , boost::unique_lock
                                                                               >(&nicta::vibe::video::algorithm::record_stream<T, U>::template op_<T_, U_, C_>, this,
                                                                                 *frame,
                                                                                 timestamp,
                                                                                 frame_index,
                                                                                 payload_index,
                                                                                 payload,
                                                                                 source_id,
                                                                                 host_id,
                                                                                 host_connections,
                                                                                 window_tags,
                                                                                 shutdown,
                                                                                 host);
}


template < typename T
         , template <typename> class U
         >
template < typename T_
         , template <typename> class U_
         , template <typename, template <typename> class> class C_
         >
bool
nicta::vibe::video::algorithm::record_stream<T, U> :: 
op_(const nicta::vibe::image::image<T, U>& frame,
    boost::posix_time::ptime timestamp,
    unsigned long int frame_index,
    unsigned int payload_index,
    unsigned int payload,
    const std::string& source_id,
    const std::string& host_id,
    unsigned int host_connections,
    std::set<std::string>& window_tags,
    void (C_<T_, U_>::*shutdown)() const throw (),
    const C_<T_, U_>* const host
   ) throw (nicta::nicta_exception)
{
    // Writes frame to the output stream
    // NOTE:  if ROI is set, then ROI will be written to output stream instead!
    // NOTE:  return value is just a convenience, and is an implementation detail that should be ignored by the common user
    //        .. power-user:  can use return value to kill connection of this client with specific source
    // NOTE:  client | shutdown arguments can be used to conditionally terminate the client entirely

    if (!this->ostream_)
    {
        // init this->ostream_ if previously uninitialized, using ROI_width & ROI_height of first incoming frame to init the output stream..

        if (!this->width_) this->width_   = frame.ROI_width().operator()<int>();
        if (!this->height_) this->height_ = frame.ROI_height().operator()<int>();
        int is_color   = !!(static_cast<int> (U<T>::encoding) != nicta::vibe::image::color_code_model<nicta::vibe::image::GRAY>::value);
        this->ostream_ = ::cvCreateVideoWriter(this->filename_,
                                               this->codec_,
                                               this->fps_,
                                               ::cvSize(this->width_, this->height_),
                                               is_color);

        if (!this->ostream_)
        {
            throw nicta::nicta_exception("Failed to create instance of CvVideoWriter", "record_stream<T, U>", "record_stream<T, U>");
        }
    }

    if (frame.ROI_width().operator()<int>() == this->width_ && frame.ROI_height().operator()<int>() == this->height_)
    {
        ::cvWriteFrame(this->ostream_, frame.template native_handle< >());
        return true;
    }
    //boost::bind<void> (shutdown, host)();                      // <--- can use this to conditionally shutdown the client on return of function op()(.)
    throw nicta::nicta_exception("Size mismatch. Anticipated frame-size and actual frame ROI size vary.", "op()(image<T, U>)", "record_stream<T, U>");
    return false;
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
