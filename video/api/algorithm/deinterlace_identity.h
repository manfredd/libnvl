/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_ALGORITHM_DEINTERLACE_IDENTITY_H
#define NICTA_VIBE_VIDEO_ALGORITHM_DEINTERLACE_IDENTITY_H

#include <image/image_depth.h>
#include <image/image_pixel.h>
#include <nicta_common/nicta_exception.h>
#include <nicta_common/null_deleter.h>

#include <opencv/cxtypes.h>

#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>


namespace nicta {
namespace vibe {
namespace video {
namespace algorithm {

    /* users should inherit from this and define op() */
    template < unsigned int N
             , typename Depth = nicta::vibe::image::depth_8u
             , template <typename _> class Pixel = nicta::vibe::image::bgr_p
             >
    class deinterlace_identity
    {
    public:

        enum
        {
            Payload = static_cast<int> (N) 
        };


        deinterlace_identity() throw ()
        : idx_(0)
        {
            // Constructor
        }


        void
        push_back(const boost::shared_ptr< ::IplImage >& image) throw ()
        {
            // IMPORTANT:  image-frames Users `push_back' **MUST** be managed shared.ptrs (ie. **can't ever have `null_deleter()' shared.ptr memory policy) !!!

            this->data_[this->idx_++ % N] = image;
        }


        boost::array<boost::shared_ptr< ::IplImage >, N> &
        operator()() throw ()
        {
            return this->data_;
        }


        template <typename T>
        nicta::vibe::video::algorithm::deinterlace_identity<N, Depth, Pixel> &
        operator()(T* frame) throw (nicta::nicta_exception)
        {
            // NOTE:  users should override this via derived class with a *specific* type-instance of T, otherwise, call this method to throw an exception!
            //
            // IMPORTANT:  image-frames Users `push_back' **MUST** be managed shared.ptrs (ie. **can't ever have `null_deleter()' shared.ptr memory policy) !!!

            throw nicta::nicta_exception("undefined frame type", "operator()<T>(T*)", "deinterlace_identity");
            return *this;
        }


    private:

        unsigned int idx_;
        boost::array<boost::shared_ptr< ::IplImage >, N> data_;


        BOOST_STATIC_ASSERT (N > 0);
    };



    template < typename Depth
             , template <typename _> class Pixel
             >
    class deinterlace_identity<1, Depth, Pixel>
    {
    public:

        enum
        {
            Payload = 1 
        };


        boost::array<boost::shared_ptr< ::IplImage >, 1> &
        operator()() throw ()
        {
            return this->data_;
        }


        nicta::vibe::video::algorithm::deinterlace_identity<1, Depth, Pixel> &
        operator()(::IplImage* frame) throw ()
        {
            // A do-nothing SERVER-SIDE functor ..
            //
            // IMPORTANT:  image-frames Users `push_back' **MUST** be managed shared.ptrs (ie. **can't ever have `null_deleter()' shared.ptr memory policy) !!!

            this->data_[0] = boost::shared_ptr< ::IplImage >(frame, nicta::null_deleter());
            return *this;
        }


        template <typename T>
        nicta::vibe::video::algorithm::deinterlace_identity<1, Depth, Pixel> &
        operator()(T* frame) throw (nicta::nicta_exception)
        {
            // NOTE:  users should override this via derived class with a *specific* type-instance of T, otherwise, call this method to throw an exception!
            //
            // IMPORTANT:  image-frames Users `push_back' **MUST** be managed shared.ptrs (ie. **can't ever have `null_deleter()' shared.ptr memory policy) !!!

            throw nicta::nicta_exception("undefined frame type", "operator()<T>(T*)", "deinterlace_identity");
            return *this;
        }


    private:

        boost::array<boost::shared_ptr< ::IplImage >, 1> data_;
    };

} } } }  // namespace nicta::vibe::video::algorithm


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
