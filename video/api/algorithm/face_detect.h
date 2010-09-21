/////////////////////////////////////////////////////////////////////////////////////////////
//
//  Code adpated by Manfred Doudar, NICTA Ltd, from:
//
//    Learning OpenCV - Computer Vision with the OpenCV Library, pp 511-512
//    By Gary Bradski & Adrian Kaehler, Copyright 2008 O'Reilly Media Inc.
//    September 2008, First Edition
//    ISBN: 978-0-596-51613-0
//
//    [ Also found here:  http://opencv.willowrange.com/wiki/FaceDetection ]
//
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_ALGORITHM_FACE_DETECT_H
#define NICTA_VIBE_VIDEO_ALGORITHM_FACE_DETECT_H

#include "video/handlers/window_tag.h"
#include "video/thread/multiton_model.h"

#include <image/image.h>
#include <image/image_depth.h>
#include <image/image_pixel.h>
#include <image/models/interpln_model.h>
#include <image/models/window_model.h>
#include <thread/multiton/multiton.h>
#include <nicta_common/nicta_exception.h>
#include <nicta_common/quantity.h>

#include <opencv/cv.h>
#include <opencv/cvtypes.h>
#include <opencv/cxcore.h>
#include <opencv/cxtypes.h>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

#include <iostream>
#include <set>
#include <string>
#include <utility>


namespace nicta {
namespace vibe {
namespace video {
namespace algorithm {

    class face_detect
    {
    public:

        ~face_detect() throw ();

        face_detect(const char* face_model,
                    nicta::quantity::pixel_width<unsigned int> width =
                         nicta::quantity::pixel_width<unsigned int> (8),
                    nicta::quantity::pixel_height<unsigned int> height =
                         nicta::quantity::pixel_height<unsigned int> (8)
                   ) throw (nicta::nicta_exception);

        template < typename Depth
                 , template <typename> class Pixel
                 , typename Depth_
                 , template <typename> class Pixel_
                 , template <typename, template <typename> class> class C_
                 >
        bool operator()(boost::shared_ptr<nicta::vibe::image::image<Depth, Pixel> > frame,
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
                       ) const throw ();

        unsigned int detected() const throw ();


    protected:

        template < typename T
                 , template <typename> class U
                 >
        boost::shared_ptr<nicta::vibe::image::image<T, U> >
        viola_face_detector_(nicta::vibe::image::image<T, U>& frame) const throw ();


    private:

        static ::CvScalar colors_[];

        unsigned int pixel_width_;
        unsigned int pixel_height_;

        ::CvMemStorage* storage_;
        ::CvHaarClassifierCascade* cascade_;

        mutable unsigned int detected_;
    };

} } } }  // namespace nicta::vibe::video::algorithm


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


/* initialize static member */
::CvScalar
nicta::vibe::video::algorithm::face_detect :: 
colors_[] = { { {   0,   0, 255 } },
              { {   0, 128, 255 } },
              { {   0, 255, 255 } },
              { {   0, 255,   0 } },
              { { 255, 128,   0 } },
              { { 255, 255,   0 } },
              { { 255,   0,   0 } },
              { { 255,   0, 255 } },
              { {   0,   0, 128 } },
              { {   0, 128, 128 } },
              { { 128, 255, 255 } },
              { {   0, 128,   0 } },
              { { 255, 128, 128 } },
              { { 128, 128,   0 } },
              { { 128,   0,   0 } },
              { { 255, 128, 255 } } };



nicta::vibe::video::algorithm::face_detect :: 
~face_detect() throw ()
{
    // Destructor

    ::cvReleaseHaarClassifierCascade(&this->cascade_);
    ::cvReleaseMemStorage(&this->storage_);
}


nicta::vibe::video::algorithm::face_detect :: 
face_detect(const char* face_model,
            nicta::quantity::pixel_width<unsigned int> width,
            nicta::quantity::pixel_height<unsigned int> height
           ) throw (nicta::nicta_exception)
: pixel_width_(width.operator()<unsigned int>())
, pixel_height_(height.operator()<unsigned int>())
, storage_(::cvCreateMemStorage(0))
, cascade_(static_cast< ::CvHaarClassifierCascade* >(::cvLoad(face_model, 0, 0, 0)))
, detected_(0)
{
    // Constructor - taking xml file input of training data

    if (!this->cascade_)
    {
        throw nicta::nicta_exception("failed to load face model", "face_detect", "face_detect");
    }
}


template < typename T
         , template <typename> class U
         , typename T_
         , template <typename> class U_
         , template <typename, template <typename> class> class C_
         >
bool
nicta::vibe::video::algorithm::face_detect :: 
operator()(boost::shared_ptr<nicta::vibe::image::image<T, U> > frame,
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
          ) const throw ()
{
    // Detect face in image, and show
    // NOTE:  return value is just a convenience, and is an implementation detail that should be ignored by the common user
    //        .. power-user:  can use return value to kill connection of this client with specific source
    // NOTE:  client | shutdown arguments can be used to conditionally terminate the client entirely

    boost::shared_ptr<nicta::vibe::image::image<T, U> > face = this->viola_face_detector_(*frame);

    std::string tag;
    nicta::vibe::video::display::window_tag()(host_id, source_id, tag);
    bool ret1 = nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                       , boost::mutex
                                       >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                                              < bool
                                                                                                              , nicta::vibe::image::image<T, U>
                                                                                                              , const char *
                                                                                                              , nicta::vibe::image::WindowModel
                                                                                                              , boost::unique_lock
                                                                                                              >(&nicta::vibe::image::image<T, U>::show,
                                                                                                                *frame,
                                                                                                                tag.c_str(),
                                                                                                                nicta::vibe::image::autosize);

    std::string ftag(tag + " - Detected Face(s)");
    window_tags.insert(ftag);

    bool ret2 = nicta::thread::multiton< nicta::vibe::video::thread::multiton_model::size
                                       , boost::mutex
                                       >::instance<nicta::vibe::video::thread::multiton_model::client>()->operator()
                                                                                              < bool
                                                                                              , nicta::vibe::image::image<T, U>
                                                                                              , const char *
                                                                                              , nicta::quantity::width<unsigned int>
                                                                                              , nicta::quantity::height<unsigned int>
                                                                                              , nicta::quantity::x_absolute_displacement<unsigned int>
                                                                                              , nicta::quantity::y_absolute_displacement<unsigned int>
                                                                                              , nicta::vibe::image::WindowModel
                                                                                              , boost::unique_lock
                                                                                              >(&nicta::vibe::image::image<T, U>::show,
                                                                                                *face,
                                                                                                ftag.c_str(),
                                                                                                nicta::quantity::width<unsigned int>(480),
                                                                                                nicta::quantity::height<unsigned int>(480),
                                                                                                nicta::quantity::x_absolute_displacement<unsigned int> (800),
                                                                                                nicta::quantity::y_absolute_displacement<unsigned int> (54),
                                                                                                nicta::vibe::image::autosize);

    return ret1 && ret2;
}


unsigned int
nicta::vibe::video::algorithm::face_detect :: 
detected() const throw ()
{
    // Return count of faces detected

    return this->detected_;
}


template < typename T
         , template <typename T_> class U
         >
boost::shared_ptr<nicta::vibe::image::image<T, U> >
nicta::vibe::video::algorithm::face_detect :: 
viola_face_detector_(nicta::vibe::image::image<T, U>& frame) const throw ()
{
    // Detect face in image - using Paul Viola's AdaBoost cascaded framework

    nicta::vibe::image::image<T, U> frame_copy(frame);

    boost::shared_ptr< nicta::vibe::image::image< nicta::vibe::image::depth_8u
                                                , nicta::vibe::image::gray_p
                                                >
                     > gray = frame.template convert<nicta::vibe::image::gray_p>();

    double scale = 3;
    nicta::vibe::image::image<nicta::vibe::image::depth_8u, nicta::vibe::image::gray_p>
           tiny(nicta::quantity::width<unsigned int>(frame.width().operator()< >() / scale),
                nicta::quantity::height<unsigned int>(frame.height().operator()< >() / scale));

    gray->scale<nicta::vibe::image::Bilinear>(tiny);

    ::cvEqualizeHist(tiny.template native_handle< >(), tiny.native_handle());

    ::cvClearMemStorage(this->storage_);
    ::CvSize pixel = ::cvSize(this->pixel_width_, this->pixel_height_);
    ::CvSeq* faces = ::cvHaarDetectObjects(tiny.template native_handle< >(), this->cascade_, this->storage_, 1.25, 2, CV_HAAR_DO_CANNY_PRUNING, pixel);


    int largest = 0;
    std::pair<int, int> point1(0, 0);
    std::pair<int, int> point2(0, 0);

    int count = faces ? faces->total : 0;
    for (int i = 0; i != count; ++i)
    {
        ::CvRect* rect = reinterpret_cast< ::CvRect * >(::cvGetSeqElem(faces, i));

        // find dimensions of face, and scale
        ::CvPoint p1;
        p1.x = static_cast<int> (rect->x * scale);
        p1.y = static_cast<int> (rect->y * scale);

        ::CvPoint p2;
        p2.x = static_cast<int> ((rect->x + rect->width) * scale);
        p2.y = static_cast<int> ((rect->y + rect->height) * scale);

        // draw the rectangle in the input image
        ::cvRectangle(frame.template native_handle< >(), p1, p2, this->colors_[(i + 1) % 16], 2, 8, 0);

        // find the largest face
        if (largest < rect->width)
        {
            largest = rect->width;
            point1  = std::make_pair<int, int>(p1.x, p1.y);
            point2  = std::make_pair<int, int>(p2.x, p2.y);
        }
    }
    this->detected_ = static_cast<unsigned int> (count);

    // show the closest face
    unsigned int x = point1.first  - 10U;
    unsigned int y = point1.second - 10U;
    if (x < 1U) x = 1U;
    if (y < 1U) y = 1U;


    unsigned int delta = count ? 15U : 11U;
    frame_copy.set_ROI(nicta::quantity::x_origin<unsigned int>(x),
                       nicta::quantity::y_origin<unsigned int>(y),
                       nicta::quantity::width<unsigned int>(point2.first   - point1.first + delta),
                       nicta::quantity::height<unsigned int>(point2.second - point1.second + delta));

    boost::shared_ptr<nicta::vibe::image::image<T, U> > s_ptr(new nicta::vibe::image::image<T, U> (frame_copy));
    frame_copy.clear_ROI();

    return s_ptr;
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
