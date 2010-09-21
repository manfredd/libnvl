/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_IMAGE_IMAGE_H
#define NICTA_VIBE_IMAGE_IMAGE_H

#include "image/image_depth.h"
#include "image/image_pixel.h"
#include "image/algorithm/identity_translation.h"
#include "image/detail/conversion_map.h"
#include "image/detail/iplimage_deleter.h"
#include "image/models/colorcode_model.h"
#include "image/models/destination_model.h"
#include "image/models/flip_model.h"
#include "image/models/interpln_model.h"
#include "image/models/origin_model.h"
#include "image/models/read_model.h"
#include "image/models/window_model.h"
#include "image/models/write_model.h"
#include "image/native_type.h"
#include "image/splice_type.h"

#include <fwd/as_image.h>
#include <fwd/as_matrix.h>
#include <image_header/image_header.h>
#include <nicta_common/detail/max_arity.h>
#include <nicta_common/nicta_exception.h>
#include <nicta_common/macros/conditional_comma.h>
#include <nicta_common/macros/shielded_if.h>
#include <nicta_common/quantity.h>
#include <nicta_common/stringutil.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_same.hpp>

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/cxtypes.h>
#include <opencv/highgui.h>

#include <string>
#include <stdexcept>
#include <utility>


namespace nicta {
namespace vibe {
namespace image {

    template < typename Depth = nicta::vibe::image::depth_8u
             , template <typename _> class Pixel = nicta::vibe::image::bgr_p
             >
    class image
    {
    public:

        static const int depth;
        static const int components;
        static const bool bayer;
        static const bool monochrome;
        static const bool is_integral;
        static const char* pix_t;


        ~image() throw ();


        explicit image(const char* filename) throw (nicta::nicta_exception);


#define OPERATOR(z, n, _)                                                                                                                         \
                                                                                                                                                  \
        template < typename Functor                                                                                                               \
                 NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename R))                                                               \
                 >                                                                                                                                \
        image(const char* filename,                                                                                                               \
              const Functor ftor                                                                                                                  \
              NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, R, a))                                                                 \
             ) throw (nicta::nicta_exception);
        /**/

#define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
#define BOOST_PP_LOCAL_LIMITS (0, NICTA_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
#undef OPERATOR


        explicit image(const ::IplImage* rhs) throw (nicta::nicta_exception);

        image(nicta::quantity::width<unsigned int> width,
              nicta::quantity::height<unsigned int> height
             ) throw (nicta::nicta_exception);

        image(nicta::quantity::width<unsigned int> width,
              nicta::quantity::height<unsigned int> height,
              typename Depth::type value
             ) throw (nicta::nicta_exception);

        image(nicta::quantity::width<unsigned int> width,
              nicta::quantity::height<unsigned int> height,
              const Pixel<Depth>& pixel
             ) throw (nicta::nicta_exception);


        explicit image(const nicta::vibe::image::image<Depth, Pixel>& rhs) throw ();

        template <template <typename _> class Pixel_>
        explicit image(const nicta::vibe::image::image<Depth, Pixel_>& rhs) throw (nicta::nicta_exception);


#define OPERATOR(z, n, _)                                                                                                                         \
                                                                                                                                                  \
        template < typename Depth_                                                                                                                \
                 , typename Functor                                                                                                               \
                 NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename R))                                                               \
                 >                                                                                                                                \
        image(const nicta::vibe::image::image<Depth_, Pixel>& rhs,                                                                                \
              const Functor ftor                                                                                                                  \
              NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, R, a))                                                                 \
             ) throw (nicta::nicta_exception);                                                                                                    \
                                                                                                                                                  \
                                                                                                                                                  \
        template < typename Depth_                                                                                                                \
                 , template <typename _> class Pixel_                                                                                             \
                 , typename Functor                                                                                                               \
                 NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename R))                                                               \
                 >                                                                                                                                \
        image(const nicta::vibe::image::image<Depth_, Pixel_>& rhs,                                                                               \
              const Functor ftor                                                                                                                  \
              NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, R, a))                                                                 \
             ) throw (nicta::nicta_exception);
        /**/

#define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
#define BOOST_PP_LOCAL_LIMITS (0, NICTA_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
#undef OPERATOR



        image(const nicta::vibe::image::image<Depth, nicta::vibe::image::gray_p>& channel1,
              const nicta::vibe::image::image<Depth, nicta::vibe::image::gray_p>& channel2,
              const nicta::vibe::image::image<Depth, nicta::vibe::image::gray_p>& channel3
             ) throw (nicta::nicta_exception);

        image(const nicta::vibe::image::image<Depth, nicta::vibe::image::gray_p>& channel1,
              const nicta::vibe::image::image<Depth, nicta::vibe::image::gray_p>& channel2,
              const nicta::vibe::image::image<Depth, nicta::vibe::image::gray_p>& channel3,
              const nicta::vibe::image::image<Depth, nicta::vibe::image::gray_p>& alpha_channel
             ) throw (nicta::nicta_exception);


        boost::shared_ptr<typename nicta::vibe::image::splice_type<Depth, Pixel>::type>
        splice() const throw ();


        image<Depth, Pixel>& operator= (const nicta::vibe::image::image<Depth, Pixel>& rhs) throw ();

        template <template <typename _> class Pixel_>
        image<Depth, Pixel>& operator= (const nicta::vibe::image::image<Depth, Pixel_>& rhs) throw (nicta::nicta_exception);


        bool operator!= (const nicta::vibe::image::image<Depth, Pixel>& rhs) const throw ();
        bool operator== (const nicta::vibe::image::image<Depth, Pixel>& rhs) const throw ();


        // pixel setter
        Pixel<Depth>*
        operator[](unsigned int idx);

        // pixel getter
        const Pixel<Depth>* const
        operator[](unsigned int idx) const;


        // pixel setter:  bounds checked variant
        typename Depth::type
        operator()(unsigned int row,
                   unsigned int column,
                   unsigned int plane
                  ) throw (nicta::nicta_exception);

        // pixel getter:  bounds checked variant
        const typename Depth::type
        operator()(unsigned int row,
                   unsigned int column,
                   unsigned int plane
                  ) const throw (nicta::nicta_exception);



        template <template <typename _> class Pixel_>
        boost::shared_ptr<image<Depth, Pixel_> > convert() const throw (nicta::nicta_exception);


#define OPERATOR(z, n, _)                                                                                                                         \
                                                                                                                                                  \
        template < typename Depth_                                                                                                                \
                 , typename Functor                                                                                                               \
                 NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename R))                                                               \
                 >                                                                                                                                \
        boost::shared_ptr<image<Depth_, Pixel> > convert(BOOST_PP_ENUM_BINARY_PARAMS(n, R, a)) const;                                             \
                                                                                                                                                  \
                                                                                                                                                  \
        template < typename Depth_                                                                                                                \
                 , template <typename _> class Pixel_                                                                                             \
                 , typename Functor                                                                                                               \
                 NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename R))                                                               \
                 >                                                                                                                                \
        boost::shared_ptr<image<Depth_, Pixel_> > convert(BOOST_PP_ENUM_BINARY_PARAMS(n, R, a)) const;                                    
        /**/

#define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
#define BOOST_PP_LOCAL_LIMITS (0, NICTA_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
#undef OPERATOR


        boost::shared_ptr<image<Depth, Pixel> > select(nicta::quantity::x_origin<unsigned int> x_origin,
                                                       nicta::quantity::y_origin<unsigned int> y_origin,
                                                       nicta::quantity::x_absolute_displacement<unsigned int> x_displacement =
                                                              nicta::quantity::x_absolute_displacement<unsigned int> (0),
                                                       nicta::quantity::y_absolute_displacement<unsigned int> y_displacement =
                                                              nicta::quantity::y_absolute_displacement<unsigned int> (0)
                                                      ) const throw (nicta::nicta_exception);


        template <nicta::vibe::image::FlipModel M>
        image<Depth, Pixel> &
        flip(const nicta::vibe::image::destination_model<nicta::vibe::image::inplace> model) throw (nicta::nicta_exception);

        template <nicta::vibe::image::FlipModel M>
        boost::shared_ptr<image<Depth, Pixel> >
        flip(const nicta::vibe::image::destination_model<nicta::vibe::image::copy> model) const throw (nicta::nicta_exception);


        template <nicta::vibe::image::InterpolationModel M>
        void scale(image<Depth, Pixel>& dest) const throw ();

        template <nicta::vibe::image::InterpolationModel M>
        boost::shared_ptr<image<Depth, Pixel> >
        scale(nicta::quantity::width<unsigned int> width,
              nicta::quantity::height<unsigned int> height) const throw ();


        nicta::vibe::image::OriginModel origin() const throw ();


        template <nicta::vibe::image::WriteModel M = nicta::vibe::image::asImage>
        void save_as(const char* filename) const throw (nicta::nicta_exception);


        template <typename nicta::vibe::image::Native N = nicta::vibe::image::Handle>
        typename nicta::vibe::image::native_type<N>::type &
        native_handle() throw ();

        template <typename nicta::vibe::image::Native N = nicta::vibe::image::Handle>
        const typename nicta::vibe::image::native_type<N>::type &
        native_handle() const throw ();


        nicta::quantity::width<unsigned int> width() const throw ();
        nicta::quantity::height<unsigned int> height() const throw ();


        nicta::quantity::x_origin<unsigned int> ROI_x_origin() const throw ();
        nicta::quantity::y_origin<unsigned int> ROI_y_origin() const throw ();

        nicta::quantity::width<unsigned int> ROI_width() const throw ();
        nicta::quantity::height<unsigned int> ROI_height() const throw ();

        bool have_ROI() const throw ();
        void clear_ROI() throw ();
        void set_ROI(nicta::quantity::x_origin<unsigned int> x_origin,
                     nicta::quantity::y_origin<unsigned int> y_origin,
                     nicta::quantity::width<unsigned int> width,
                     nicta::quantity::height<unsigned int> height
                     ) throw (nicta::nicta_exception);
        ::CvRect get_ROI() const throw ();
        ::CvSize get_size() const throw ();

        bool have_COI() const throw ();
        void clear_COI()throw ();
        void set_COI(typename Pixel<Depth>::channel::coi c) throw ();
        typename Pixel<Depth>::channel::coi get_COI() const throw ();


        bool show(nicta::vibe::image::WindowModel m = nicta::vibe::image::autosize) const throw ();

        bool show(const char* id,
                  nicta::vibe::image::WindowModel m = nicta::vibe::image::autosize
                 ) const throw ();

        bool show(const char* id,
                  nicta::quantity::width<unsigned int> width,
                  nicta::quantity::height<unsigned int> height,
                  nicta::vibe::image::WindowModel m = nicta::vibe::image::autosize
                 ) const throw ();

        bool show(const char* id,
                  nicta::quantity::x_absolute_displacement<unsigned int> x_pos,
                  nicta::quantity::y_absolute_displacement<unsigned int> y_pos,
                  nicta::vibe::image::WindowModel m = nicta::vibe::image::autosize
                 ) const throw ();

        bool show(const char* id,
                  nicta::quantity::width<unsigned int> width,
                  nicta::quantity::height<unsigned int> height,
                  nicta::quantity::x_absolute_displacement<unsigned int> x_pos,
                  nicta::quantity::y_absolute_displacement<unsigned int> y_pos,
                  nicta::vibe::image::WindowModel m = nicta::vibe::image::autosize
                 ) const throw ();

        bool show(const char* id,
                  nicta::quantity::x_absolute_displacement<unsigned int> x_pos,
                  nicta::quantity::y_absolute_displacement<unsigned int> y_pos,
                  nicta::quantity::width<unsigned int> width,
                  nicta::quantity::height<unsigned int> height,
                  nicta::vibe::image::WindowModel m = nicta::vibe::image::autosize
                 ) const throw ();



        bool timed_show(boost::posix_time::time_duration td,
                        nicta::vibe::image::WindowModel m = nicta::vibe::image::autosize
                       ) const throw ();

        bool timed_show(const char* id,
                        boost::posix_time::time_duration td,
                        nicta::vibe::image::WindowModel m = nicta::vibe::image::autosize
                       ) const throw ();

        bool timed_show(const char* id,
                        boost::posix_time::time_duration td,
                        nicta::quantity::width<unsigned int> width,
                        nicta::quantity::height<unsigned int> height,
                        nicta::vibe::image::WindowModel m = nicta::vibe::image::autosize
                       ) const throw ();

        bool timed_show(const char* id,
                        boost::posix_time::time_duration td,
                        nicta::quantity::x_absolute_displacement<unsigned int> x_pos,
                        nicta::quantity::y_absolute_displacement<unsigned int> y_pos,
                        nicta::vibe::image::WindowModel m = nicta::vibe::image::autosize
                       ) const throw ();

        bool timed_show(const char* id,
                        boost::posix_time::time_duration td,
                        nicta::quantity::width<unsigned int> width,
                        nicta::quantity::height<unsigned int> height,
                        nicta::quantity::x_absolute_displacement<unsigned int> x_pos,
                        nicta::quantity::y_absolute_displacement<unsigned int> y_pos,
                        nicta::vibe::image::WindowModel m = nicta::vibe::image::autosize
                       ) const throw ();

        bool timed_show(const char* id,
                        boost::posix_time::time_duration td,
                        nicta::quantity::x_absolute_displacement<unsigned int> x_pos,
                        nicta::quantity::y_absolute_displacement<unsigned int> y_pos,
                        nicta::quantity::width<unsigned int> width,
                        nicta::quantity::height<unsigned int> height,
                        nicta::vibe::image::WindowModel m = nicta::vibe::image::autosize
                       ) const throw ();




        template < typename Depth_
                 , template <typename _> class Pixel_
                 >
        friend
        nicta::vibe::image::image<Depth_, Pixel_>& assign(nicta::vibe::image::image<Depth_, Pixel_>& lhs,
                                                          const nicta::vibe::image::image<Depth_, Pixel_>& rhs
                                                         ) throw ();

        template < typename Depth_
                 , template <typename _> class Pixel_
                 , template <typename _> class Pixel__
                 >
        friend
        nicta::vibe::image::image<Depth_, Pixel_>& assign(nicta::vibe::image::image<Depth_, Pixel_>& lhs,
                                                          const nicta::vibe::image::image<Depth_, Pixel__>& rhs
                                                         ) throw (nicta::nicta_exception);

#define OPERATOR(z, n, _)                                                                                                                         \
                                                                                                                                                  \
        template < typename Functor                                                                                                               \
                 , typename Depth_                                                                                                                \
                 , template <typename _> class Pixel_                                                                                             \
                 , typename Depth__                                                                                                               \
                 NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename R) )                                                              \
                 >                                                                                                                                \
        friend                                                                                                                                    \
        nicta::vibe::image::image<Depth_, Pixel_>& assign(nicta::vibe::image::image<Depth_, Pixel_>& lhs,                                         \
                                                          const nicta::vibe::image::image<Depth__, Pixel_>& rhs                                   \
                                                          NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, R, a))                     \
                                                         );                                                                                       \
                                                                                                                                                  \
        template < typename Functor                                                                                                               \
                 , typename Depth_                                                                                                                \
                 , template <typename _> class Pixel_                                                                                             \
                 , typename Depth__                                                                                                               \
                 , template <typename _> class Pixel__                                                                                            \
                 NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename R))                                                               \
                 >                                                                                                                                \
        friend                                                                                                                                    \
        nicta::vibe::image::image<Depth_, Pixel_>& assign(nicta::vibe::image::image<Depth_, Pixel_>& lhs,                                         \
                                                          const nicta::vibe::image::image<Depth__, Pixel__>& rhs                                  \
                                                          NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, R, a))                     \
                                                         ) throw (nicta::nicta_exception);
        /**/

#define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
#define BOOST_PP_LOCAL_LIMITS (0, NICTA_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
#undef OPERATOR




        template < typename Depth_
                 , template <typename _> class Pixel_
                 >
        friend
        boost::shared_ptr< nicta::vibe::image::image<Depth_, Pixel_> >
        as_image(const nicta::ocv::matrix::matrix< typename Depth_::type
                                                 , 2
                                                 , Pixel_<Depth_>::components
                                                 , 2
                                                 >& matrix) throw (std::domain_error);

        template < typename T
                 , template <typename _> class Pixel_
                 >
        friend
        boost::shared_ptr< nicta::ocv::matrix::matrix< T
                                                     , 2
                                                     , Pixel_<typename nicta::vibe::image::depth<T>::type>::components
                                                     , 2
                                                     >
                          >
        as_matrix(const nicta::vibe::image::image<typename nicta::vibe::image::depth<T>::type, Pixel_>& image) throw ();


    private:

        image() throw ();


        ::IplImage* image_;

        bool init_hdr_;
        ::IplImage header_;

        boost::tuples::tuple< ::IplImage&        // header_
                            , ::IplImage*&       // image_
                            > tuple_;


        // pair<origin, width | height>
        std::pair<unsigned int, unsigned int> xroi_;
        std::pair<unsigned int, unsigned int> yroi_;
    };


// >->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->-


    template < typename T
             , template <typename T_> class U
             >
    nicta::vibe::image::image<T, U>&
    assign(nicta::vibe::image::image<T, U>& lhs,
           const nicta::vibe::image::image<T, U>& rhs
          ) throw()
    {
        return lhs.nicta::vibe::image::image<T, U>::operator= (rhs);
    }


    template < typename T
             , template <typename T_> class U
             , template <typename T_> class V
             >
    nicta::vibe::image::image<T, U>&
    assign(nicta::vibe::image::image<T, U>& lhs,
           const nicta::vibe::image::image<T, V>& rhs
          ) throw(nicta::nicta_exception)
    {
        return lhs.nicta::vibe::image::image<T, U>::operator= (rhs);
    }


#define OPERATOR(z, n, _)                                                                                                                         \
                                                                                                                                                  \
    template < typename Functor                                                                                                                   \
             , typename T                                                                                                                         \
             , template <typename T_> class U                                                                                                     \
             , typename V                                                                                                                         \
             NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename R))                                                                   \
             >                                                                                                                                    \
    nicta::vibe::image::image<T, U>&                                                                                                              \
    assign(nicta::vibe::image::image<T, U>& lhs,                                                                                                  \
           const nicta::vibe::image::image<V, U>& rhs                                                                                             \
           NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, R, a))                                                                    \
          )                                                                                                                                       \
    {                                                                                                                                             \
        /* Assignment Op - color depth (V->T) conversion on rhs (dump lhs.image_) */                                                              \
        /* usage:  nicta::vibe::image::assign<nicta::vibe::image::algorithm::identity_transformation> (img1, img2); */                            \
                                                                                                                                                  \
        if (!lhs.init_hdr_)                                                                                                                       \
        {                                                                                                                                         \
            ::cvReleaseImage(&lhs.image_);                                                                                                        \
        }                                                                                                                                         \
        else                                                                                                                                      \
        {                                                                                                                                         \
            lhs.init_hdr_ = !lhs.init_hdr_;                                                                                                       \
        }                                                                                                                                         \
                                                                                                                                                  \
        lhs.image_ = ::cvCreateImage(::cvSize(rhs.image_->width, rhs.image_->height), T::value, U<T>::components);                                \
                                                                                                                                                  \
        for (int i = 0; i != lhs.image_->height; ++i)                                                                                             \
        {                                                                                                                                         \
            for (int j = 0; j != lhs.image_->width; ++j)                                                                                          \
            {                                                                                                                                     \
                for (int member = 0; member != U<V>::components; ++member)                                                                        \
                {                                                                                                                                 \
                    /* works without packing/alignment issues b/c all types are the same */                                                       \
                                                                                                                                                  \
                    *(reinterpret_cast<typename T::type *> (&lhs[i][j]) + member) =                                                               \
                           Functor().template operator()<typename T::type> (* (reinterpret_cast<typename V::type const *> (&rhs[i][j]) + member)  \
                                                                           NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));            \
                }                                                                                                                                 \
            }                                                                                                                                     \
        }                                                                                                                                         \
        lhs.xroi_ = std::make_pair<unsigned int, unsigned int> (0, lhs.image_->width);                                                            \
        lhs.yroi_ = std::make_pair<unsigned int, unsigned int> (0, lhs.image_->height);                                                           \
                                                                                                                                                  \
        return lhs;                                                                                                                               \
    }                                                                                                                                             \
                                                                                                                                                  \
                                                                                                                                                  \
    template < typename Functor                                                                                                                   \
             , typename T                                                                                                                         \
             , template <typename T_> class U                                                                                                     \
             , typename V                                                                                                                         \
             , template <typename V_> class W                                                                                                     \
             NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename R))                                                                   \
             >                                                                                                                                    \
    nicta::vibe::image::image<T, U>&                                                                                                              \
    assign(nicta::vibe::image::image<T, U>& lhs,                                                                                                  \
           const nicta::vibe::image::image<V, W>& rhs                                                                                             \
           NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, R, a))                                                                    \
          ) throw (nicta::nicta_exception)                                                                                                        \
    {                                                                                                                                             \
        /* Assignment Op - color -depth (V->T) & -space (W->U) conversion on rhs (dump lhs.image_) */                                             \
        /* usage:  nicta::vibe::image::assign<nicta::vibe::image::algorithm::identity_transformation> (img1, img2); */                            \
                                                                                                                                                  \
        if (rhs.image_->roi)                                                                                                                      \
        {                                                                                                                                         \
            throw nicta::nicta_exception("Cannot assign to lvalue image object from rvalue having defined ROI", "assign");                        \
        }                                                                                                                                         \
                                                                                                                                                  \
                                                                                                                                                  \
        if (!lhs.init_hdr_)                                                                                                                       \
        {                                                                                                                                         \
            ::cvReleaseImage(&lhs.image_);                                                                                                        \
        }                                                                                                                                         \
        else                                                                                                                                      \
        {                                                                                                                                         \
            lhs.init_hdr_ = !lhs.init_hdr_;                                                                                                       \
        }                                                                                                                                         \
                                                                                                                                                  \
        /* get the CV_SRC2DST identifier & convert to new color space (W->U), (depth unchanged) */                                                \
        int id = nicta::vibe::image::detail::conversion_map<W<V>::encoding, U<V>::encoding>::type::value;                                         \
                                                                                                                                                  \
        ::IplImage* temp = ::cvCreateImage(::cvSize(rhs.image_->width, rhs.image_->height), V::value, U<V>::components);                          \
                                                                                                                                                  \
        ::cvCvtColor(rhs.image_, temp, id);                                                                                                       \
                                                                                                                                                  \
        lhs.image_ = ::cvCreateImage(::cvSize(rhs.image_->width, rhs.image_->height), T::value, U<T>::components);                                \
                                                                                                                                                  \
        /* convert to new color depth (V->T) */                                                                                                   \
        for (int i = 0; i != lhs.image_->height; ++i)                                                                                             \
        {                                                                                                                                         \
            for (int j = 0; j != lhs.image_->width; ++j)                                                                                          \
            {                                                                                                                                     \
                for (int member = 0; member != U<T>::components; ++member)                                                                        \
                {                                                                                                                                 \
                    /* works without packing/alignment issues b/c all types are the same */                                                       \
                                                                                                                                                  \
                    *(reinterpret_cast<typename T::type *> (&lhs[i][j]) + member) =                                                               \
                           Functor().template operator()<typename T::type>                                                                        \
                               (* (reinterpret_cast<typename V::type const *> (temp->imageData + i * temp->widthStep + j) + member)               \
                               NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                                        \
                }                                                                                                                                 \
            }                                                                                                                                     \
        }                                                                                                                                         \
        ::cvReleaseImage(&temp);                                                                                                                  \
        lhs.xroi_ = std::make_pair<unsigned int, unsigned int> (0, lhs.image_->width);                                                            \
        lhs.yroi_ = std::make_pair<unsigned int, unsigned int> (0, lhs.image_->height);                                                           \
                                                                                                                                                  \
        return lhs;                                                                                                                               \
    }
    /**/

#define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
#define BOOST_PP_LOCAL_LIMITS (0, NICTA_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
#undef OPERATOR


} } } // namespace nicta::vibe::image


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


/* Initialize static data members */
template < typename T
         , template <typename T_> class U
         >
const int
nicta::vibe::image::image<T, U> :: 
depth(static_cast<int> (T::depth));


template < typename T
         , template <typename T_> class U
         >
const int
nicta::vibe::image::image<T, U> :: 
components(static_cast<int> (U<T>::components));


template < typename T
         , template <typename T_> class U
         >
const bool
nicta::vibe::image::image<T, U> :: 
bayer(U<T>::components == 1 && static_cast<int> (U<T>::encoding) != static_cast<int> (nicta::vibe::image::GRAY));


template < typename T
         , template <typename T_> class U
         >
const bool
nicta::vibe::image::image<T, U> :: 
monochrome(U<T>::components == 1 && static_cast<int> (U<T>::encoding) == static_cast<int> (nicta::vibe::image::GRAY));


template < typename T
         , template <typename T_> class U
         >
const bool
nicta::vibe::image::image<T, U> :: 
is_integral(T::depth != IPL_DEPTH_32F && T::depth != IPL_DEPTH_64F);


template < typename T
         , template <typename T_> class U
         >
const char*
nicta::vibe::image::image<T, U> :: 
pix_t(U<T>::pix_t);


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::image::image<T, U> :: 
~image() throw ()
{
    // Destructor

    if (!this->init_hdr_)
    {
        ::cvReleaseImage(&this->image_);
    }
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::image::image<T, U> :: 
image(const char* filename) throw (nicta::nicta_exception)
: image_(0)
, init_hdr_(false)
, tuple_(header_, image_)
, xroi_(std::make_pair<unsigned int, unsigned int> (0, 0))
, yroi_(std::make_pair<unsigned int, unsigned int> (0, 0))
{
    // Constructor
    // Supported image formats: BMP, DIB, JPEG, JPG, JPE, PNG, PBM, PGM, PPM, SR, RAS, TIFF, TIF
    // WARNING:  we cannot make check to see if color space is same (img files don't encode that info) ..user beware!
    //
    // usage:  nicta::vibe::image::image<nicta::vibe::image::depth_8u, nicta::vibe::image::rgb_p> img(filename);
    //

    nicta::vibe::header::image_header hdr(filename);
    int hdr_depth = hdr.depth();
    bool hdr_color = hdr.is_color();

    switch (U<T>::encoding)  // color-space
    {
        case nicta::vibe::image::GRAY :
        {
            if (hdr_color)
            {
                throw nicta::nicta_exception("Sought implicit conversion of file to multi-channel color-type", "image<T, U>", "image<T, U>");
            }
            break;
        }

        default:  // color
        {
            if (!hdr_color)
            {
                throw nicta::nicta_exception("Sought implicit conversion of file to 1-channel grayscale", "image<T, U>", "image<T, U>");
            }
            break;
        }
    }


    if (hdr_depth != T::depth)
    {
        throw nicta::nicta_exception("Sought implicit conversion of file to alternate bit-depth", "image<T, U>", "image<T, U>");
    }


    // no implicit conversion attempted ... now choose the right model, and load file into this->image_

    if (hdr_depth == 8)
    {
        if (!hdr_color)
        {
            this->image_ = ::cvLoadImage(filename, nicta::vibe::image::read_model<nicta::vibe::image::Gray_8>::value);
        }
        else if (static_cast<int> (U<T>::encoding) == static_cast<int> (nicta::vibe::image::BGR))  // hdr_color == true
        {
            this->image_ = ::cvLoadImage(filename, nicta::vibe::image::read_model<nicta::vibe::image::Color_8>::value);
        }
        else  // hdr_color == true, encoding = color, !BGR  [U is whatever user chooses, color, but not bgr_p]
        {
            this->image_ = ::cvLoadImage(filename, nicta::vibe::image::read_model<nicta::vibe::image::File_n>::value);
        }
    }
    else
    {
        if (!hdr_color)
        {
            this->image_ = ::cvLoadImage(filename, nicta::vibe::image::read_model<nicta::vibe::image::Gray_n>::value);
        }
        else if (static_cast<int> (U<T>::encoding) == static_cast<int> (nicta::vibe::image::BGR))  // hdr_color == true
        {
            this->image_ = ::cvLoadImage(filename, nicta::vibe::image::read_model<nicta::vibe::image::Color_n>::value);
        }
        else  // hdr_color == true, encoding = color, !BGR  [U is whatever user chooses, color, but not bgr_p]
        {
            this->image_ = ::cvLoadImage(filename, nicta::vibe::image::read_model<nicta::vibe::image::File_n>::value);
        }
    }


    if (!this->image_)
    {
        throw nicta::nicta_exception("Failed to load image", "image<T, U>", "image<T, U>");
    }
    this->xroi_.second = this->image_->width;
    this->yroi_.second = this->image_->height;


    // check color depths match
    if (this->image_->depth != static_cast<int> (T::value))
    {
        throw nicta::nicta_exception("Color-depth mismatch", "image<T, U>", "image<T, U>");
    }

    // ck number of color channels equal
    if (U<T>::components != this->image_->nChannels)
    {
        throw nicta::nicta_exception("Color-channel count mismatch", "image<T, U>", "image<T, U>");
    }

    // cannot ck if color space is same :-(
}



#define OPERATOR(z, n, _)                                                                                                                         \
                                                                                                                                                  \
template < typename T                                                                                                                             \
         , template <typename T_> class U                                                                                                         \
         >                                                                                                                                        \
template < typename Functor                                                                                                                       \
         NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename R))                                                                       \
         >                                                                                                                                        \
nicta::vibe::image::image<T, U> ::                                                                                                                \
image(const char* filename,                                                                                                                       \
      const Functor ftor                                                                                                                          \
      NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, R, a))                                                                         \
     ) throw (nicta::nicta_exception)                                                                                                             \
: image_(0)                                                                                                                                       \
, init_hdr_(false)                                                                                                                                \
, tuple_(header_, image_)                                                                                                                         \
, xroi_(std::make_pair<unsigned int, unsigned int> (0, 0))                                                                                        \
, yroi_(std::make_pair<unsigned int, unsigned int> (0, 0))                                                                                        \
{                                                                                                                                                 \
    /* Constructor                                                                                                                      */        \
    /* Supported image formats: BMP, DIB, JPEG, JPG, JPE, PNG, PBM, PGM, PPM, SR, RAS, TIFF, TIF                                        */        \
    /* WARNING:  we cannot make check to see if color space is same (img files don't encode that info) ..user beware!                   */        \
    /*                                                                                                                                  */        \
    /* usage:  nicta::vibe::image::image<nicta::vibe::image::depth_8u, nicta::vibe::image::rgb_p>                                       */        \
    /*              img(filename, nicta::vibe::image::algorithm::identity_translation());                                               */        \
    /*                                                                                                                                  */        \
                                                                                                                                                  \
    nicta::vibe::header::image_header hdr(filename);                                                                                              \
    int hdr_depth     = hdr.depth();                                                                                                              \
    bool hdr_color    = hdr.is_color();                                                                                                           \
    bool hdr_integral = hdr.is_integral();                                                                                                        \
                                                                                                                                                  \
    if (!hdr_color)                                                                                                                               \
    {                                                                                                                                             \
        /* <depth, gray_p> */                                                                                                                     \
                                                                                                                                                  \
        if (static_cast<int> (U<T>::encoding) == static_cast<int> (nicta::vibe::image::GRAY) && hdr_depth == T::depth)                            \
        {                                                                                                                                         \
            if (hdr_depth == 8)                                                                                                                   \
            {                                                                                                                                     \
                this->image_ = ::cvLoadImage(filename, nicta::vibe::image::read_model<nicta::vibe::image::Gray_8>::value);                        \
            }                                                                                                                                     \
            else                                                                                                                                  \
            {                                                                                                                                     \
                this->image_ = ::cvLoadImage(filename, nicta::vibe::image::read_model<nicta::vibe::image::Gray_n>::value);                        \
            }                                                                                                                                     \
        }                                                                                                                                         \
        else                                                                                                                                      \
        {                                                                                                                                         \
            /* <depth, gray_p> */                                                                                                                 \
                                                                                                                                                  \
            ::IplImage* img = 0;                                                                                                                  \
            if (hdr_depth == 8)                                                                                                                   \
            {                                                                                                                                     \
                img = ::cvLoadImage(filename, nicta::vibe::image::read_model<nicta::vibe::image::Gray_8>::value);                                 \
            }                                                                                                                                     \
            else                                                                                                                                  \
            {                                                                                                                                     \
                img = ::cvLoadImage(filename, nicta::vibe::image::read_model<nicta::vibe::image::Gray_n>::value);                                 \
            }                                                                                                                                     \
                                                                                                                                                  \
            if (static_cast<int> (U<T>::encoding) != static_cast<int> (nicta::vibe::image::GRAY))                                                 \
            {                                                                                                                                     \
                /* do color space conversion [gray_p->U] */                                                                                       \
                                                                                                                                                  \
                if (hdr_depth == T::depth)                                                                                                        \
                {                                                                                                                                 \
                    /* no depth conversion necessary */                                                                                           \
                                                                                                                                                  \
                    /* get the CV_SRC2DST identifier & convert to new color space (gray_p->U), (depth unchanged) */                               \
                    int id = nicta::vibe::image::detail::conversion_map< nicta::vibe::image::gray_p<T>::encoding                                  \
                                                                       , U<T>::encoding                                                           \
                                                                       >::type::value;                                                            \
                                                                                                                                                  \
                    this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);             \
                    ::cvCvtColor(img, this->image_, id);  /* U<depth_8> */                                                                        \
                                                                                                                                                  \
                    ::cvReleaseImage(&img);                                                                                                       \
                }                                                                                                                                 \
                else                                                                                                                              \
                {                                                                                                                                 \
                    /* do color-space & depth conversion */                                                                                       \
                                                                                                                                                  \
                    switch (hdr_depth)                                                                                                            \
                    {                                                                                                                             \
                        case 8 :                                                                                                                  \
                        {                                                                                                                         \
                            /* get the CV_SRC2DST identifier & convert to new color space (gray_p->U), (depth unchanged) */                           \
                            int id = nicta::vibe::image::detail::conversion_map< nicta::vibe::image::gray_p<nicta::vibe::image::depth_8u>::encoding   \
                                                                               , U<nicta::vibe::image::depth_8u>::encoding                            \
                                                                               >::type::value;                                                        \
                                                                                                                                                  \
                            ::IplImage* temp = ::cvCreateImage( ::cvSize(img->width, img->height)                                                 \
                                                              , static_cast<int> (nicta::vibe::image::depth_8u::value)                            \
                                                              , U<nicta::vibe::image::depth_8u>::components);                                     \
                                                                                                                                                  \
                            ::cvCvtColor(img, temp, id);                                                                                          \
                            ::cvReleaseImage(&img);                                                                                               \
                                                                                                                                                  \
                                                                                                                                                  \
                            this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);     \
                                                                                                                                                  \
                            /* convert to new color depth (depth->T) */                                                                           \
                            for (int i = 0; i != this->image_->height; ++i)                                                                       \
                            {                                                                                                                     \
                                for (int j = 0; j != this->image_->width; ++j)                                                                    \
                                {                                                                                                                 \
                                    for (int member = 0; member != U<T>::components; ++member)                                                    \
                                    {                                                                                                             \
                                        /* works without packing/alignment issues b/c all types are the same */                                   \
                                                                                                                                                  \
                                        *(reinterpret_cast<typename T::type *>                                                                    \
                                              (this->image_->imageData + i * this->image_->widthStep + j) + member) =                             \
                                        ftor.template operator()<typename T::type>                                                                \
                                            (* (reinterpret_cast<typename nicta::vibe::image::depth_8u::type const *>                             \
                                                    (temp->imageData + i * temp->widthStep + j) + member)                                         \
                                            NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                           \
                                    }                                                                                                             \
                                }                                                                                                                 \
                            }                                                                                                                     \
                            ::cvReleaseImage(&temp);                                                                                              \
                                                                                                                                                  \
                            break;                                                                                                                \
                        }                                                                                                                         \
                                                                                                                                                  \
                        case 16 :                                                                                                                 \
                        {                                                                                                                         \
                            /* get the CV_SRC2DST identifier & convert to new color space (gray_p->U), (depth unchanged) */                            \
                            int id = nicta::vibe::image::detail::conversion_map< nicta::vibe::image::gray_p<nicta::vibe::image::depth_16u>::encoding   \
                                                                               , U<nicta::vibe::image::depth_16u>::encoding                            \
                                                                               >::type::value;                                                         \
                                                                                                                                                  \
                            ::IplImage* temp = ::cvCreateImage( ::cvSize(img->width, img->height)                                                 \
                                                              , static_cast<int> (nicta::vibe::image::depth_16u::value)                           \
                                                              , U<nicta::vibe::image::depth_16u>::components);                                    \
                                                                                                                                                  \
                            ::cvCvtColor(img, temp, id);                                                                                          \
                            ::cvReleaseImage(&img);                                                                                               \
                                                                                                                                                  \
                                                                                                                                                  \
                            this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);     \
                                                                                                                                                  \
                            /* convert to new color depth (depth->T) */                                                                           \
                            for (int i = 0; i != this->image_->height; ++i)                                                                       \
                            {                                                                                                                     \
                                for (int j = 0; j != this->image_->width; ++j)                                                                    \
                                {                                                                                                                 \
                                    for (int member = 0; member != U<T>::components; ++member)                                                    \
                                    {                                                                                                             \
                                        /* works without packing/alignment issues b/c all types are the same */                                   \
                                                                                                                                                  \
                                        *(reinterpret_cast<typename T::type *>                                                                    \
                                              (this->image_->imageData + i * this->image_->widthStep + j) + member) =                             \
                                        ftor.template operator()<typename T::type>                                                                \
                                            (* (reinterpret_cast<typename nicta::vibe::image::depth_16u::type const *>                            \
                                                    (temp->imageData + i * temp->widthStep + j) + member)                                         \
                                            NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                           \
                                    }                                                                                                             \
                                }                                                                                                                 \
                            }                                                                                                                     \
                            ::cvReleaseImage(&temp);                                                                                              \
                                                                                                                                                  \
                            break;                                                                                                                \
                        }                                                                                                                         \
                                                                                                                                                  \
                        case 32 :                                                                                                                 \
                        {                                                                                                                         \
                            if (hdr_integral)                                                                                                     \
                            {                                                                                                                     \
                                /* get the CV_SRC2DST identifier & convert to new color space (gray_p->U), (depth unchanged) */                            \
                                int id = nicta::vibe::image::detail::conversion_map< nicta::vibe::image::gray_p<nicta::vibe::image::depth_32s>::encoding   \
                                                                                   , U<nicta::vibe::image::depth_32s>::encoding                            \
                                                                                   >::type::value;                                                         \
                                                                                                                                                  \
                                ::IplImage* temp = ::cvCreateImage( ::cvSize(img->width, img->height)                                             \
                                                                  , static_cast<int> (nicta::vibe::image::depth_32s::value)                       \
                                                                  , U<nicta::vibe::image::depth_32s>::components);                                \
                                                                                                                                                  \
                                ::cvCvtColor(img, temp, id);                                                                                      \
                                ::cvReleaseImage(&img);                                                                                           \
                                                                                                                                                  \
                                                                                                                                                  \
                                this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components); \
                                                                                                                                                  \
                                /* convert to new color depth (depth->T) */                                                                       \
                                for (int i = 0; i != this->image_->height; ++i)                                                                   \
                                {                                                                                                                 \
                                    for (int j = 0; j != this->image_->width; ++j)                                                                \
                                    {                                                                                                             \
                                        for (int member = 0; member != U<T>::components; ++member)                                                \
                                        {                                                                                                         \
                                            /* works without packing/alignment issues b/c all types are the same */                               \
                                                                                                                                                  \
                                            *(reinterpret_cast<typename T::type *>                                                                \
                                                  (this->image_->imageData + i * this->image_->widthStep + j) + member) =                         \
                                            ftor.template operator()<typename T::type>                                                            \
                                                (* (reinterpret_cast<typename nicta::vibe::image::depth_32s::type const *>                        \
                                                        (temp->imageData + i * temp->widthStep + j) + member)                                     \
                                                NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                       \
                                        }                                                                                                         \
                                    }                                                                                                             \
                                }                                                                                                                 \
                                ::cvReleaseImage(&temp);                                                                                          \
                            }                                                                                                                     \
                            else                                                                                                                  \
                            {                                                                                                                     \
                                /* get the CV_SRC2DST identifier & convert to new color space (gray_p->U), (depth unchanged) */                            \
                                int id = nicta::vibe::image::detail::conversion_map< nicta::vibe::image::gray_p<nicta::vibe::image::depth_32f>::encoding   \
                                                                                   , U<nicta::vibe::image::depth_32f>::encoding                            \
                                                                                   >::type::value;                                                         \
                                                                                                                                                  \
                                ::IplImage* temp = ::cvCreateImage( ::cvSize(img->width, img->height)                                             \
                                                                  , static_cast<int> (nicta::vibe::image::depth_32f::value)                       \
                                                                  , U<nicta::vibe::image::depth_32f>::components);                                \
                                                                                                                                                  \
                                ::cvCvtColor(img, temp, id);                                                                                      \
                                ::cvReleaseImage(&img);                                                                                           \
                                                                                                                                                  \
                                                                                                                                                  \
                                this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components); \
                                                                                                                                                  \
                                /* convert to new color depth (depth->T) */                                                                       \
                                for (int i = 0; i != this->image_->height; ++i)                                                                   \
                                {                                                                                                                 \
                                    for (int j = 0; j != this->image_->width; ++j)                                                                \
                                    {                                                                                                             \
                                        for (int member = 0; member != U<T>::components; ++member)                                                \
                                        {                                                                                                         \
                                            /* works without packing/alignment issues b/c all types are the same */                               \
                                                                                                                                                  \
                                            *(reinterpret_cast<typename T::type *>                                                                \
                                                  (this->image_->imageData + i * this->image_->widthStep + j) + member) =                         \
                                            ftor.template operator()<typename T::type>                                                            \
                                                (* (reinterpret_cast<typename nicta::vibe::image::depth_32f::type const *>                        \
                                                        (temp->imageData + i * temp->widthStep + j) + member)                                     \
                                                NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                       \
                                        }                                                                                                         \
                                    }                                                                                                             \
                                }                                                                                                                 \
                                ::cvReleaseImage(&temp);                                                                                          \
                            }                                                                                                                     \
                                                                                                                                                  \
                            break;                                                                                                                \
                        }                                                                                                                         \
                                                                                                                                                  \
                        case 64 :                                                                                                                 \
                        {                                                                                                                         \
                            /* get the CV_SRC2DST identifier & convert to new color space (gray_p->U), (depth unchanged) */                            \
                            int id = nicta::vibe::image::detail::conversion_map< nicta::vibe::image::gray_p<nicta::vibe::image::depth_64f>::encoding   \
                                                                               , U<nicta::vibe::image::depth_64f>::encoding                            \
                                                                               >::type::value;                                                         \
                                                                                                                                                  \
                            ::IplImage* temp = ::cvCreateImage( ::cvSize(img->width, img->height)                                                 \
                                                              , static_cast<int> (nicta::vibe::image::depth_64f::value)                           \
                                                              , U<nicta::vibe::image::depth_64f>::components);                                    \
                                                                                                                                                  \
                            ::cvCvtColor(img, temp, id);                                                                                          \
                            ::cvReleaseImage(&img);                                                                                               \
                                                                                                                                                  \
                                                                                                                                                  \
                            this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);     \
                                                                                                                                                  \
                            /* convert to new color depth (depth->T) */                                                                           \
                            for (int i = 0; i != this->image_->height; ++i)                                                                       \
                            {                                                                                                                     \
                                for (int j = 0; j != this->image_->width; ++j)                                                                    \
                                {                                                                                                                 \
                                    for (int member = 0; member != U<T>::components; ++member)                                                    \
                                    {                                                                                                             \
                                        /* works without packing/alignment issues b/c all types are the same */                                   \
                                                                                                                                                  \
                                        *(reinterpret_cast<typename T::type *>                                                                    \
                                              (this->image_->imageData + i * this->image_->widthStep + j) + member) =                             \
                                        ftor.template operator()<typename T::type>                                                                \
                                            (* (reinterpret_cast<typename nicta::vibe::image::depth_64f::type const *>                            \
                                                    (temp->imageData + i * temp->widthStep + j) + member)                                         \
                                            NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                           \
                                    }                                                                                                             \
                                }                                                                                                                 \
                            }                                                                                                                     \
                            ::cvReleaseImage(&temp);                                                                                              \
                                                                                                                                                  \
                            break;                                                                                                                \
                        }                                                                                                                         \
                    }                                                                                                                             \
                }                                                                                                                                 \
            }                                                                                                                                     \
            else                                                                                                                                  \
            {                                                                                                                                     \
                /* do depth conversion */                                                                                                         \
                                                                                                                                                  \
                switch (hdr_depth)                                                                                                                \
                {                                                                                                                                 \
                    case 8 :                                                                                                                      \
                    {                                                                                                                             \
                        this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);         \
                                                                                                                                                  \
                        /* convert to new color depth (depth->T) */                                                                               \
                        for (int i = 0; i != this->image_->height; ++i)                                                                           \
                        {                                                                                                                         \
                            for (int j = 0; j != this->image_->width; ++j)                                                                        \
                            {                                                                                                                     \
                                for (int member = 0; member != U<T>::components; ++member)                                                        \
                                {                                                                                                                 \
                                    /* works without packing/alignment issues b/c all types are the same */                                       \
                                                                                                                                                  \
                                    *(reinterpret_cast<typename T::type *>                                                                        \
                                          (this->image_->imageData + i * this->image_->widthStep + j) + member) =                                 \
                                    ftor.template operator()<typename T::type>                                                                    \
                                        (* (reinterpret_cast<typename nicta::vibe::image::depth_8u::type const *>                                 \
                                                (img->imageData + i * img->widthStep + j) + member)                                               \
                                        NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                               \
                                }                                                                                                                 \
                            }                                                                                                                     \
                        }                                                                                                                         \
                        ::cvReleaseImage(&img);                                                                                                   \
                                                                                                                                                  \
                        break;                                                                                                                    \
                    }                                                                                                                             \
                                                                                                                                                  \
                    case 16 :                                                                                                                     \
                    {                                                                                                                             \
                        this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);         \
                                                                                                                                                  \
                        /* convert to new color depth (depth->T) */                                                                               \
                        for (int i = 0; i != this->image_->height; ++i)                                                                           \
                        {                                                                                                                         \
                            for (int j = 0; j != this->image_->width; ++j)                                                                        \
                            {                                                                                                                     \
                                for (int member = 0; member != U<T>::components; ++member)                                                        \
                                {                                                                                                                 \
                                    /* works without packing/alignment issues b/c all types are the same */                                       \
                                                                                                                                                  \
                                    *(reinterpret_cast<typename T::type *>                                                                        \
                                          (this->image_->imageData + i * this->image_->widthStep + j) + member) =                                 \
                                    ftor.template operator()<typename T::type>                                                                    \
                                        (* (reinterpret_cast<typename nicta::vibe::image::depth_16u::type const *>                                \
                                                (img->imageData + i * img->widthStep + j) + member)                                               \
                                        NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                               \
                                }                                                                                                                 \
                            }                                                                                                                     \
                        }                                                                                                                         \
                        ::cvReleaseImage(&img);                                                                                                   \
                                                                                                                                                  \
                        break;                                                                                                                    \
                    }                                                                                                                             \
                                                                                                                                                  \
                    case 32 :                                                                                                                     \
                    {                                                                                                                             \
                        if (hdr_integral)                                                                                                         \
                        {                                                                                                                         \
                            this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);     \
                                                                                                                                                  \
                            /* convert to new color depth (depth->T) */                                                                           \
                            for (int i = 0; i != this->image_->height; ++i)                                                                       \
                            {                                                                                                                     \
                                for (int j = 0; j != this->image_->width; ++j)                                                                    \
                                {                                                                                                                 \
                                    for (int member = 0; member != U<T>::components; ++member)                                                    \
                                    {                                                                                                             \
                                        /* works without packing/alignment issues b/c all types are the same */                                   \
                                                                                                                                                  \
                                        *(reinterpret_cast<typename T::type *>                                                                    \
                                              (this->image_->imageData + i * this->image_->widthStep + j) + member) =                             \
                                        ftor.template operator()<typename T::type>                                                                \
                                            (* (reinterpret_cast<typename nicta::vibe::image::depth_32s::type const *>                            \
                                                    (img->imageData + i * img->widthStep + j) + member)                                           \
                                            NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                           \
                                    }                                                                                                             \
                                }                                                                                                                 \
                            }                                                                                                                     \
                            ::cvReleaseImage(&img);                                                                                               \
                        }                                                                                                                         \
                        else                                                                                                                      \
                        {                                                                                                                         \
                            this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);     \
                                                                                                                                                  \
                            /* convert to new color depth (depth->T) */                                                                           \
                            for (int i = 0; i != this->image_->height; ++i)                                                                       \
                            {                                                                                                                     \
                                for (int j = 0; j != this->image_->width; ++j)                                                                    \
                                {                                                                                                                 \
                                    for (int member = 0; member != U<T>::components; ++member)                                                    \
                                    {                                                                                                             \
                                        /* works without packing/alignment issues b/c all types are the same */                                   \
                                                                                                                                                  \
                                        *(reinterpret_cast<typename T::type *>                                                                    \
                                              (this->image_->imageData + i * this->image_->widthStep + j) + member) =                             \
                                        ftor.template operator()<typename T::type>                                                                \
                                            (* (reinterpret_cast<typename nicta::vibe::image::depth_32f::type const *>                            \
                                                    (img->imageData + i * img->widthStep + j) + member)                                           \
                                            NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                           \
                                    }                                                                                                             \
                                }                                                                                                                 \
                            }                                                                                                                     \
                            ::cvReleaseImage(&img);                                                                                               \
                        }                                                                                                                         \
                                                                                                                                                  \
                        break;                                                                                                                    \
                    }                                                                                                                             \
                                                                                                                                                  \
                    case 64 :                                                                                                                     \
                    {                                                                                                                             \
                        this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);         \
                                                                                                                                                  \
                        /* convert to new color depth (depth->T) */                                                                               \
                        for (int i = 0; i != this->image_->height; ++i)                                                                           \
                        {                                                                                                                         \
                            for (int j = 0; j != this->image_->width; ++j)                                                                        \
                            {                                                                                                                     \
                                for (int member = 0; member != U<T>::components; ++member)                                                        \
                                {                                                                                                                 \
                                    /* works without packing/alignment issues b/c all types are the same */                                       \
                                                                                                                                                  \
                                    *(reinterpret_cast<typename T::type *>                                                                        \
                                          (this->image_->imageData + i * this->image_->widthStep + j) + member) =                                 \
                                    ftor.template operator()<typename T::type>                                                                    \
                                        (* (reinterpret_cast<typename nicta::vibe::image::depth_64f::type const *>                                \
                                                (img->imageData + i * img->widthStep + j) + member)                                               \
                                        NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                               \
                                }                                                                                                                 \
                            }                                                                                                                     \
                        }                                                                                                                         \
                        ::cvReleaseImage(&img);                                                                                                   \
                                                                                                                                                  \
                        break;                                                                                                                    \
                    }                                                                                                                             \
                }                                                                                                                                 \
            }                                                                                                                                     \
        }                                                                                                                                         \
    }                                                                                                                                             \
    else if (static_cast<int> (U<T>::encoding) == static_cast<int> (nicta::vibe::image::BGR))  /* hdr_color == true */                            \
    {                                                                                                                                             \
        /* <depth, bgr_p> */                                                                                                                      \
                                                                                                                                                  \
        if (hdr_depth == T::depth)                                                                                                                \
        {                                                                                                                                         \
            if (hdr_depth == 8)                                                                                                                   \
            {                                                                                                                                     \
                this->image_ = ::cvLoadImage(filename, nicta::vibe::image::read_model<nicta::vibe::image::Color_8>::value);                       \
            }                                                                                                                                     \
            else                                                                                                                                  \
            {                                                                                                                                     \
                this->image_ = ::cvLoadImage(filename, nicta::vibe::image::read_model<nicta::vibe::image::Color_n>::value);                       \
            }                                                                                                                                     \
        }                                                                                                                                         \
        else                                                                                                                                      \
        {                                                                                                                                         \
            /* do depth conversion */                                                                                                             \
                                                                                                                                                  \
            ::IplImage* img = 0;                                                                                                                  \
            if (hdr_depth == 8)                                                                                                                   \
            {                                                                                                                                     \
                img = ::cvLoadImage(filename, nicta::vibe::image::read_model<nicta::vibe::image::Color_8>::value);                                \
            }                                                                                                                                     \
            else                                                                                                                                  \
            {                                                                                                                                     \
                img = ::cvLoadImage(filename, nicta::vibe::image::read_model<nicta::vibe::image::Color_n>::value);                                \
            }                                                                                                                                     \
            if (!img)                                                                                                                             \
            {                                                                                                                                     \
                throw nicta::nicta_exception("Failed to load image", "image<T, U>", "image<T, U>");                                               \
            }                                                                                                                                     \
                                                                                                                                                  \
            switch (hdr_depth)                                                                                                                    \
            {                                                                                                                                     \
                case 8 :                                                                                                                          \
                {                                                                                                                                 \
                    this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);             \
                                                                                                                                                  \
                    /* convert to new color depth (depth->T) */                                                                                   \
                    for (int i = 0; i != this->image_->height; ++i)                                                                               \
                    {                                                                                                                             \
                        for (int j = 0; j != this->image_->width; ++j)                                                                            \
                        {                                                                                                                         \
                            for (int member = 0; member != U<T>::components; ++member)                                                            \
                            {                                                                                                                     \
                                /* works without packing/alignment issues b/c all types are the same */                                           \
                                                                                                                                                  \
                                *(reinterpret_cast<typename T::type *>                                                                            \
                                      (this->image_->imageData + i * this->image_->widthStep + j) + member) =                                     \
                                ftor.template operator()<typename T::type>                                                                        \
                                    (* (reinterpret_cast<typename nicta::vibe::image::depth_8u::type const *>                                     \
                                            (img->imageData + i * img->widthStep + j) + member)                                                   \
                                    NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                                   \
                            }                                                                                                                     \
                        }                                                                                                                         \
                    }                                                                                                                             \
                    ::cvReleaseImage(&img);                                                                                                       \
                                                                                                                                                  \
                    break;                                                                                                                        \
                }                                                                                                                                 \
                                                                                                                                                  \
                case 16 :                                                                                                                         \
                {                                                                                                                                 \
                    this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);             \
                                                                                                                                                  \
                    /* convert to new color depth (depth->T) */                                                                                   \
                    for (int i = 0; i != this->image_->height; ++i)                                                                               \
                    {                                                                                                                             \
                        for (int j = 0; j != this->image_->width; ++j)                                                                            \
                        {                                                                                                                         \
                            for (int member = 0; member != U<T>::components; ++member)                                                            \
                            {                                                                                                                     \
                                /* works without packing/alignment issues b/c all types are the same */                                           \
                                                                                                                                                  \
                                *(reinterpret_cast<typename T::type *>                                                                            \
                                      (this->image_->imageData + i * this->image_->widthStep + j) + member) =                                     \
                                ftor.template operator()<typename T::type>                                                                        \
                                    (* (reinterpret_cast<typename nicta::vibe::image::depth_16u::type const *>                                    \
                                            (img->imageData + i * img->widthStep + j) + member)                                                   \
                                    NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                                   \
                            }                                                                                                                     \
                        }                                                                                                                         \
                    }                                                                                                                             \
                    ::cvReleaseImage(&img);                                                                                                       \
                                                                                                                                                  \
                    break;                                                                                                                        \
                }                                                                                                                                 \
                                                                                                                                                  \
                case 32 :                                                                                                                         \
                {                                                                                                                                 \
                    if (hdr_integral)                                                                                                             \
                    {                                                                                                                             \
                        this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);         \
                                                                                                                                                  \
                        /* convert to new color depth (depth->T) */                                                                               \
                        for (int i = 0; i != this->image_->height; ++i)                                                                           \
                        {                                                                                                                         \
                            for (int j = 0; j != this->image_->width; ++j)                                                                        \
                            {                                                                                                                     \
                                for (int member = 0; member != U<T>::components; ++member)                                                        \
                                {                                                                                                                 \
                                    /* works without packing/alignment issues b/c all types are the same */                                       \
                                                                                                                                                  \
                                    *(reinterpret_cast<typename T::type *>                                                                        \
                                          (this->image_->imageData + i * this->image_->widthStep + j) + member) =                                 \
                                    ftor.template operator()<typename T::type>                                                                    \
                                        (* (reinterpret_cast<typename nicta::vibe::image::depth_32s::type const *>                                \
                                                (img->imageData + i * img->widthStep + j) + member)                                               \
                                        NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                               \
                                }                                                                                                                 \
                            }                                                                                                                     \
                        }                                                                                                                         \
                        ::cvReleaseImage(&img);                                                                                                   \
                    }                                                                                                                             \
                    else                                                                                                                          \
                    {                                                                                                                             \
                        this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);         \
                                                                                                                                                  \
                        /* convert to new color depth (depth->T) */                                                                               \
                        for (int i = 0; i != this->image_->height; ++i)                                                                           \
                        {                                                                                                                         \
                            for (int j = 0; j != this->image_->width; ++j)                                                                        \
                            {                                                                                                                     \
                                for (int member = 0; member != U<T>::components; ++member)                                                        \
                                {                                                                                                                 \
                                    /* works without packing/alignment issues b/c all types are the same */                                       \
                                                                                                                                                  \
                                    *(reinterpret_cast<typename T::type *>                                                                        \
                                          (this->image_->imageData + i * this->image_->widthStep + j) + member) =                                 \
                                    ftor.template operator()<typename T::type>                                                                    \
                                        (* (reinterpret_cast<typename nicta::vibe::image::depth_32f::type const *>                                \
                                                (img->imageData + i * img->widthStep + j) + member)                                               \
                                        NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                               \
                                }                                                                                                                 \
                            }                                                                                                                     \
                        }                                                                                                                         \
                        ::cvReleaseImage(&img);                                                                                                   \
                    }                                                                                                                             \
                                                                                                                                                  \
                    break;                                                                                                                        \
                }                                                                                                                                 \
                                                                                                                                                  \
                case 64 :                                                                                                                         \
                {                                                                                                                                 \
                    this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);             \
                                                                                                                                                  \
                    /* convert to new color depth (depth->T) */                                                                                   \
                    for (int i = 0; i != this->image_->height; ++i)                                                                               \
                    {                                                                                                                             \
                        for (int j = 0; j != this->image_->width; ++j)                                                                            \
                        {                                                                                                                         \
                            for (int member = 0; member != U<T>::components; ++member)                                                            \
                            {                                                                                                                     \
                                /* works without packing/alignment issues b/c all types are the same */                                           \
                                                                                                                                                  \
                                *(reinterpret_cast<typename T::type *>                                                                            \
                                      (this->image_->imageData + i * this->image_->widthStep + j) + member) =                                     \
                                ftor.template operator()<typename T::type>                                                                        \
                                    (* (reinterpret_cast<typename nicta::vibe::image::depth_64f::type const *>                                    \
                                            (img->imageData + i * img->widthStep + j) + member)                                                   \
                                    NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                                   \
                            }                                                                                                                     \
                        }                                                                                                                         \
                    }                                                                                                                             \
                    ::cvReleaseImage(&img);                                                                                                       \
                                                                                                                                                  \
                    break;                                                                                                                        \
                }                                                                                                                                 \
            }                                                                                                                                     \
        }                                                                                                                                         \
    }                                                                                                                                             \
    else  /* hdr_color == true, encoding = color, !BGR  [U is whatever user chooses, color, but not bgr_p] */                                     \
    {                                                                                                                                             \
        /* <depth, U> */                                                                                                                          \
                                                                                                                                                  \
        if (hdr_depth == T::depth)                                                                                                                \
        {                                                                                                                                         \
            if (hdr_depth == 8)                                                                                                                   \
            {                                                                                                                                     \
                this->image_ = ::cvLoadImage(filename, nicta::vibe::image::read_model<nicta::vibe::image::File_8>::value);                        \
            }                                                                                                                                     \
            else                                                                                                                                  \
            {                                                                                                                                     \
                this->image_ = ::cvLoadImage(filename, nicta::vibe::image::read_model<nicta::vibe::image::File_n>::value);                        \
            }                                                                                                                                     \
        }                                                                                                                                         \
        else                                                                                                                                      \
        {                                                                                                                                         \
            /* do depth conversion */                                                                                                             \
                                                                                                                                                  \
            ::IplImage* img = 0;                                                                                                                  \
            if (hdr_depth == 8)                                                                                                                   \
            {                                                                                                                                     \
                img = ::cvLoadImage(filename, nicta::vibe::image::read_model<nicta::vibe::image::File_8>::value);                                 \
            }                                                                                                                                     \
            else                                                                                                                                  \
            {                                                                                                                                     \
                img = ::cvLoadImage(filename, nicta::vibe::image::read_model<nicta::vibe::image::File_n>::value);                                 \
            }                                                                                                                                     \
            if (!img)                                                                                                                             \
            {                                                                                                                                     \
                throw nicta::nicta_exception("Failed to load image", "image<T, U>", "image<T, U>");                                               \
            }                                                                                                                                     \
                                                                                                                                                  \
            switch (hdr_depth)                                                                                                                    \
            {                                                                                                                                     \
                case 8 :                                                                                                                          \
                {                                                                                                                                 \
                    this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);             \
                                                                                                                                                  \
                    /* convert to new color depth (depth->T) */                                                                                   \
                    for (int i = 0; i != this->image_->height; ++i)                                                                               \
                    {                                                                                                                             \
                        for (int j = 0; j != this->image_->width; ++j)                                                                            \
                        {                                                                                                                         \
                            for (int member = 0; member != U<T>::components; ++member)                                                            \
                            {                                                                                                                     \
                                /* works without packing/alignment issues b/c all types are the same */                                           \
                                                                                                                                                  \
                                *(reinterpret_cast<typename T::type *>                                                                            \
                                      (this->image_->imageData + i * this->image_->widthStep + j) + member) =                                     \
                                ftor.template operator()<typename T::type>                                                                        \
                                    (* (reinterpret_cast<typename nicta::vibe::image::depth_8u::type const *>                                     \
                                            (img->imageData + i * img->widthStep + j) + member)                                                   \
                                    NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                                   \
                            }                                                                                                                     \
                        }                                                                                                                         \
                    }                                                                                                                             \
                    ::cvReleaseImage(&img);                                                                                                       \
                                                                                                                                                  \
                    break;                                                                                                                        \
                }                                                                                                                                 \
                                                                                                                                                  \
                case 16 :                                                                                                                         \
                {                                                                                                                                 \
                    this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);             \
                                                                                                                                                  \
                    /* convert to new color depth (depth->T) */                                                                                   \
                    for (int i = 0; i != this->image_->height; ++i)                                                                               \
                    {                                                                                                                             \
                        for (int j = 0; j != this->image_->width; ++j)                                                                            \
                        {                                                                                                                         \
                            for (int member = 0; member != U<T>::components; ++member)                                                            \
                            {                                                                                                                     \
                                /* works without packing/alignment issues b/c all types are the same */                                           \
                                                                                                                                                  \
                                *(reinterpret_cast<typename T::type *>                                                                            \
                                      (this->image_->imageData + i * this->image_->widthStep + j) + member) =                                     \
                                ftor.template operator()<typename T::type>                                                                        \
                                    (* (reinterpret_cast<typename nicta::vibe::image::depth_16u::type const *>                                    \
                                            (img->imageData + i * img->widthStep + j) + member)                                                   \
                                    NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                                   \
                            }                                                                                                                     \
                        }                                                                                                                         \
                    }                                                                                                                             \
                    ::cvReleaseImage(&img);                                                                                                       \
                                                                                                                                                  \
                    break;                                                                                                                        \
                }                                                                                                                                 \
                                                                                                                                                  \
                case 32 :                                                                                                                         \
                {                                                                                                                                 \
                    if (hdr_integral)                                                                                                             \
                    {                                                                                                                             \
                        this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);         \
                                                                                                                                                  \
                        /* convert to new color depth (depth->T) */                                                                               \
                        for (int i = 0; i != this->image_->height; ++i)                                                                           \
                        {                                                                                                                         \
                            for (int j = 0; j != this->image_->width; ++j)                                                                        \
                            {                                                                                                                     \
                                for (int member = 0; member != U<T>::components; ++member)                                                        \
                                {                                                                                                                 \
                                    /* works without packing/alignment issues b/c all types are the same */                                       \
                                                                                                                                                  \
                                    *(reinterpret_cast<typename T::type *>                                                                        \
                                          (this->image_->imageData + i * this->image_->widthStep + j) + member) =                                 \
                                    ftor.template operator()<typename T::type>                                                                    \
                                        (* (reinterpret_cast<typename nicta::vibe::image::depth_32s::type const *>                                \
                                                (img->imageData + i * img->widthStep + j) + member)                                               \
                                        NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                               \
                                }                                                                                                                 \
                            }                                                                                                                     \
                        }                                                                                                                         \
                        ::cvReleaseImage(&img);                                                                                                   \
                    }                                                                                                                             \
                    else                                                                                                                          \
                    {                                                                                                                             \
                        this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);         \
                                                                                                                                                  \
                        /* convert to new color depth (depth->T) */                                                                               \
                        for (int i = 0; i != this->image_->height; ++i)                                                                           \
                        {                                                                                                                         \
                            for (int j = 0; j != this->image_->width; ++j)                                                                        \
                            {                                                                                                                     \
                                for (int member = 0; member != U<T>::components; ++member)                                                        \
                                {                                                                                                                 \
                                    /* works without packing/alignment issues b/c all types are the same */                                       \
                                                                                                                                                  \
                                    *(reinterpret_cast<typename T::type *>                                                                        \
                                          (this->image_->imageData + i * this->image_->widthStep + j) + member) =                                 \
                                    ftor.template operator()<typename T::type>                                                                    \
                                        (* (reinterpret_cast<typename nicta::vibe::image::depth_32f::type const *>                                \
                                                (img->imageData + i * img->widthStep + j) + member)                                               \
                                        NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                               \
                                }                                                                                                                 \
                            }                                                                                                                     \
                        }                                                                                                                         \
                        ::cvReleaseImage(&img);                                                                                                   \
                    }                                                                                                                             \
                                                                                                                                                  \
                    break;                                                                                                                        \
                }                                                                                                                                 \
                                                                                                                                                  \
                case 64 :                                                                                                                         \
                {                                                                                                                                 \
                    this->image_ = ::cvCreateImage(::cvSize(img->width, img->height), static_cast<int> (T::value), U<T>::components);             \
                                                                                                                                                  \
                    /* convert to new color depth (depth->T) */                                                                                   \
                    for (int i = 0; i != this->image_->height; ++i)                                                                               \
                    {                                                                                                                             \
                        for (int j = 0; j != this->image_->width; ++j)                                                                            \
                        {                                                                                                                         \
                            for (int member = 0; member != U<T>::components; ++member)                                                            \
                            {                                                                                                                     \
                                /* works without packing/alignment issues b/c all types are the same */                                           \
                                                                                                                                                  \
                                *(reinterpret_cast<typename T::type *>                                                                            \
                                      (this->image_->imageData + i * this->image_->widthStep + j) + member) =                                     \
                                ftor.template operator()<typename T::type>                                                                        \
                                    (* (reinterpret_cast<typename nicta::vibe::image::depth_64f::type const *>                                    \
                                            (img->imageData + i * img->widthStep + j) + member)                                                   \
                                    NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                                   \
                            }                                                                                                                     \
                        }                                                                                                                         \
                    }                                                                                                                             \
                    ::cvReleaseImage(&img);                                                                                                       \
                                                                                                                                                  \
                    break;                                                                                                                        \
                }                                                                                                                                 \
            }                                                                                                                                     \
        }                                                                                                                                         \
    }                                                                                                                                             \
                                                                                                                                                  \
                                                                                                                                                  \
    if (!this->image_)                                                                                                                            \
    {                                                                                                                                             \
        throw nicta::nicta_exception("Failed to load image", "image<T, U>", "image<T, U>");                                                       \
    }                                                                                                                                             \
    this->xroi_.second = this->image_->width;                                                                                                     \
    this->yroi_.second = this->image_->height;                                                                                                    \
                                                                                                                                                  \
                                                                                                                                                  \
    /* check color depths match */                                                                                                                \
    if (this->image_->depth != static_cast<int> (T::value))                                                                                       \
    {                                                                                                                                             \
        throw nicta::nicta_exception("Color-depth mismatch", "image<T, U>", "image<T, U>");                                                       \
    }                                                                                                                                             \
                                                                                                                                                  \
    /* ck number of color channels equal */                                                                                                       \
    if (U<T>::components != this->image_->nChannels)                                                                                              \
    {                                                                                                                                             \
        throw nicta::nicta_exception("Color-channel count mismatch", "image<T, U>", "image<T, U>");                                               \
    }                                                                                                                                             \
                                                                                                                                                  \
    /* cannot ck if color space is same :-( */                                                                                                    \
}
/**/

#define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
#define BOOST_PP_LOCAL_LIMITS (0, NICTA_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
#undef OPERATOR



template < typename T
         , template <typename T_> class U
         >
nicta::vibe::image::image<T, U> :: 
image(const ::IplImage* rhs) throw (nicta::nicta_exception)
: image_(0)
, init_hdr_(false)
, tuple_(header_, image_)
, xroi_(std::make_pair<unsigned int> (static_cast<unsigned int> (::cvGetImageROI(rhs).x), static_cast<unsigned int> (rhs->width)))
, yroi_(std::make_pair<unsigned int> (static_cast<unsigned int> (::cvGetImageROI(rhs).y), static_cast<unsigned int> (rhs->height)))
{
    // Constructor - Implicit conversions are forbidden with this constructor !!
    // WARNING:  we cannot make check to see if color space is same (img files don't encode that info) ..user beware!
    // WARNING:  we cannot make definitive check to see if 'rhs' is grayscale or a bayer image ..user beware!
    //
    // NOTE:  if rhs is color, will assume color is U
    // NOTE:  copies the rhs ROI !
    //

    if (!rhs)
    {
        throw nicta::nicta_exception("Null-pointed file object passed in", "image<T, U>", "image<T, U>");
    }

    long int img_depth = rhs->depth;
    //bool img_color = (rhs->nChannels > 1);  ..it could be a bayer-image, there is no definitive way to check it is color or not!

    switch (U<T>::encoding)  // color-space
    {
        case nicta::vibe::image::GRAY :
        {
            //if (img_color)  ..it could be a bayer-image, there is no definitive way to check it is color or not!
            //{
            //    throw nicta::nicta_exception("Sought implicit conversion of file to multi-channel color-type", "image<T, U>", "image<T, U>");
            //}
            break;
        }

        default:  // color
        {
            //if (!img_color)  ..it could be a bayer-image, there is no definitive way to check it is color or not!
            //{
            //    throw nicta::nicta_exception("Sought implicit conversion of file to 1-channel grayscale", "image<T, U>", "image<T, U>");
            //}
            break;
        }
    }

    // check color depths match
    if (img_depth != T::value)
    {
        throw nicta::nicta_exception("Sought implicit conversion of file to alternate bit-depth", "image<T, U>", "image<T, U>");
    }

    // ck number of color channels equal
    if (U<T>::components != rhs->nChannels)
    {
        throw nicta::nicta_exception("Color-channel count mismatch", "image<T, U>", "image<T, U>");
    }

    // cannot ck if color space is same :-(  ...so color is taken to be whatever U is.


    this->image_ = ::cvCloneImage(rhs);
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::image::image<T, U> :: 
image(nicta::quantity::width<unsigned int> width,
      nicta::quantity::height<unsigned int> height
     ) throw (nicta::nicta_exception)
: image_(::cvCreateImage(::cvSize(width.operator()<int>(), height.operator()<int>()), static_cast<int> (T::value), U<T>::components))
, init_hdr_(false)
, tuple_(header_, image_)
, xroi_(std::make_pair<unsigned int> (0, width.template operator()< >()))
, yroi_(std::make_pair<unsigned int> (0, height.template operator()< >()))
{
    // Constructor - creates uninitialized space for image having (width x height) extents

    if (!width.operator()< >() || !height.operator()< >())
    {
        throw nicta::nicta_exception("Bad Argument Value - image width or height zero initialized", "image<T, U>", "image<T, U>");
    }
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::image::image<T, U> :: 
image(nicta::quantity::width<unsigned int> width,
      nicta::quantity::height<unsigned int> height,
      typename T::type value
     ) throw (nicta::nicta_exception)
: image_(::cvCreateImage(::cvSize(width.operator()<int>(), height.operator()<int>()), static_cast<int> (T::value), U<T>::components))
, init_hdr_(false)
, tuple_(header_, image_)
, xroi_(std::make_pair<unsigned int> (0, width.template operator()< >()))
, yroi_(std::make_pair<unsigned int> (0, height.template operator()< >()))
{
    // Constructor - creates space for image having (width x height) extents, and initializing all values to `value'

    if (!width.operator()< >() || !height.operator()< >())
    {
        throw nicta::nicta_exception("Bad Argument Value - image width or height zero initialized", "image<T, U>", "image<T, U>");
    }

    int Width(width.operator()<int>());
    int Height(height.operator()<int>());
    for (int i = 0; i != Height; ++i)
    {
        for (int j = 0; j != Width; ++j)
        {
            // access each member of struct U & cast
            for (int member = 0; member != U<T>::components; ++member)
            {
                // works without packing/alignment issues b/c all types are the same

                *(reinterpret_cast<typename T::type *> (&(* this)[i][j]) + member) = value;
            }
        }
    }
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::image::image<T, U> :: 
image(nicta::quantity::width<unsigned int> width,
      nicta::quantity::height<unsigned int> height,
      const U<T>& pixel
     ) throw (nicta::nicta_exception)
: image_(::cvCreateImage(::cvSize(width.operator()<int>(), height.operator()<int>()), static_cast<int> (T::value), U<T>::components))
, init_hdr_(false)
, tuple_(header_, image_)
, xroi_(std::make_pair<unsigned int> (0, width.template operator()< >()))
, yroi_(std::make_pair<unsigned int> (0, height.template operator()< >()))
{
    // Constructor - creates space for image having (width x height) extents, and initializing all values to `pixel'

    if (!width.operator()< >() || !height.operator()< >())
    {
        throw nicta::nicta_exception("Bad Argument Value - image width or height zero initialized", "image<T, U>", "image<T, U>");
    }

    int Width(width.operator()<int>());
    int Height(height.operator()<int>());
    for (int i = 0; i != Height; ++i)
    {
        for (int j = 0; j != Width; ++j)
        {
            // access each member of struct U & cast
            for (int member = 0; member != U<T>::components; ++member)
            {
                // works without packing/alignment issues b/c all types are the same

                *(reinterpret_cast<typename T::type *> (&(* this)[i][j]) + member) = pixel(member);
            }
        }
    }
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::image::image<T, U> :: 
image(const nicta::vibe::image::image<T, U>& rhs) throw ()
: image_(::cvCloneImage(rhs.image_))
, init_hdr_(false)
, tuple_(header_, image_)
, xroi_(rhs.xroi_)
, yroi_(rhs.yroi_)
{
    // Copy Constructor
    // NOTE:  copies the rhs ROI !
}


template < typename T
         , template <typename T_> class U
         >
template <template <typename T_> class V>
nicta::vibe::image::image<T, U> :: 
image(const nicta::vibe::image::image<T, V>& rhs) throw (nicta::nicta_exception)
: image_(::cvCreateImage( ::cvSize((*reinterpret_cast< ::IplImage* const * > (&rhs))->width,
                                   (*reinterpret_cast< ::IplImage* const * > (&rhs))->height)
                        , static_cast<int> (T::value), U<T>::components))
, init_hdr_(false)
, tuple_(header_, image_)
, xroi_(std::make_pair<unsigned int> (0, static_cast<unsigned int> (image_->width)))
, yroi_(std::make_pair<unsigned int> (0, static_cast<unsigned int> (image_->height)))
{
    // Copy-LIKE Constructor - color space (V->U) conversion

    if ((*reinterpret_cast< ::IplImage* const * > (&rhs))->roi)
    {
        throw nicta::nicta_exception("Cannot construct image object from rvalue having defined ROI", "image<T, U>", "image<T, U>");
    }

    // get the CV_SRC2DST identifier
    int id = nicta::vibe::image::detail::conversion_map<V<T>::encoding, U<T>::encoding>::type::value;

    ::cvCvtColor(*reinterpret_cast< ::IplImage* const * > (&rhs), this->image_, id);
}



#define OPERATOR(z, n, _)                                                                                                                         \
                                                                                                                                                  \
template < typename T                                                                                                                             \
         , template <typename T_> class U                                                                                                         \
         >                                                                                                                                        \
template < typename V                                                                                                                             \
         , typename Functor                                                                                                                       \
         NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename R))                                                                       \
         >                                                                                                                                        \
nicta::vibe::image::image<T, U> ::                                                                                                                \
image(const nicta::vibe::image::image<V, U>& rhs,                                                                                                 \
      const Functor ftor                                                                                                                          \
      NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, R, a))                                                                         \
     ) throw (nicta::nicta_exception)                                                                                                             \
: image_(::cvCreateImage(::cvSize((*reinterpret_cast< ::IplImage* const * > (&rhs))->width,                                                       \
                                  (*reinterpret_cast< ::IplImage* const * > (&rhs))->height),                                                     \
                         static_cast<int> (T::value), U<T>::components))                                                                          \
, init_hdr_(false)                                                                                                                                \
, tuple_(header_, image_)                                                                                                                         \
, xroi_(std::make_pair<unsigned int> (0, static_cast<unsigned int> (image_->width)))                                                              \
, yroi_(std::make_pair<unsigned int> (0, static_cast<unsigned int> (image_->height)))                                                             \
{                                                                                                                                                 \
    /* Copy-LIKE Constructor - color depth (V->T) conversion */                                                                                   \
                                                                                                                                                  \
    if ((*reinterpret_cast< ::IplImage* const * > (&rhs))->roi)                                                                                   \
    {                                                                                                                                             \
        throw nicta::nicta_exception("Cannot construct image object from rvalue having defined ROI", "image<T, U>", "image<T, U>");               \
    }                                                                                                                                             \
                                                                                                                                                  \
    for (int i = 0; i != this->image_->height; ++i)                                                                                               \
    {                                                                                                                                             \
        for (int j = 0; j != this->image_->width; ++j)                                                                                            \
        {                                                                                                                                         \
            for (int member = 0; member != U<V>::components; ++member)                                                                            \
            {                                                                                                                                     \
                /* works without packing/alignment issues b/c all types are the same */                                                           \
                                                                                                                                                  \
                *(reinterpret_cast<typename T::type *> (this->image_->imageData + i * this->image_->widthStep + j) + member) =                    \
                       ftor.template operator()<typename T::type> (* (reinterpret_cast<typename V::type const *> (&rhs[i][j]) + member)           \
                                                                  NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                     \
            }                                                                                                                                     \
        }                                                                                                                                         \
    }                                                                                                                                             \
}                                                                                                                                                 \
                                                                                                                                                  \
                                                                                                                                                  \
template < typename T                                                                                                                             \
         , template <typename T_> class U                                                                                                         \
         >                                                                                                                                        \
template < typename V                                                                                                                             \
         , template <typename V_> class W                                                                                                         \
         , typename Functor                                                                                                                       \
         NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename R))                                                                       \
         >                                                                                                                                        \
nicta::vibe::image::image<T, U> ::                                                                                                                \
image(const nicta::vibe::image::image<V, W>& rhs,                                                                                                 \
      const Functor ftor                                                                                                                          \
      NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_BINARY_PARAMS(n, R, a))                                                                         \
     ) throw (nicta::nicta_exception)                                                                                                             \
: image_(::cvCreateImage(::cvSize((*reinterpret_cast< ::IplImage* const * > (&rhs))->width,                                                       \
                                  (*reinterpret_cast< ::IplImage* const * > (&rhs))->height),                                                     \
                         static_cast<int> (T::value), U<T>::components))                                                                          \
, init_hdr_(false)                                                                                                                                \
, tuple_(header_, image_)                                                                                                                         \
, xroi_(std::make_pair<unsigned int> (0, static_cast<unsigned int> (image_->width)))                                                              \
, yroi_(std::make_pair<unsigned int> (0, static_cast<unsigned int> (image_->height)))                                                             \
{                                                                                                                                                 \
    /* Copy-LIKE Constructor - color -depth (V->T) & -space (W->U) conversion */                                                                  \
                                                                                                                                                  \
    if ((*reinterpret_cast< ::IplImage* const * > (&rhs))->roi)                                                                                   \
    {                                                                                                                                             \
        throw nicta::nicta_exception("Cannot construct image object from rvalue having defined ROI", "image<T, U>", "image<T, U>");               \
    }                                                                                                                                             \
                                                                                                                                                  \
    /* get the CV_SRC2DST identifier & convert to new color space (W->U), (depth unchanged) */                                                    \
    int id = nicta::vibe::image::detail::conversion_map<W<V>::encoding, U<V>::encoding>::type::value;                                             \
                                                                                                                                                  \
    ::IplImage* temp = ::cvCreateImage(::cvSize((*reinterpret_cast< ::IplImage* const * > (&rhs))->width,                                         \
                                                (*reinterpret_cast< ::IplImage* const * > (&rhs))->height),                                       \
                                       static_cast<int> (V::value), U<V>::components);                                                            \
                                                                                                                                                  \
                                                                                                                                                  \
    /* NOTE: if W<V>::components < U<V>::components will convert color-space no problem, but remaining color-planes will be blacked-out */        \
    /*       ... will see this once attempt convert on color-depth under these conditions                                               */        \
    /* Eg.   bgr_8u -> gray_8u will, after a cvCvtColor call with 2 planes blotted out, and 1 plane containing the grayscale img        */        \
    /*       ... then on loop for color depth, (even if use U<V>::components instead of W<V>::components) .. will result in only 1/3    */        \
    /*       ... of a visible image .. as the other 2 planes now contain nothing!                                                       */        \
                                                                                                                                                  \
    ::cvCvtColor(*reinterpret_cast< ::IplImage* const * > (&rhs), temp, id);                                                                      \
                                                                                                                                                  \
    /* convert to new color depth (V->T) */                                                                                                       \
    for (int i = 0; i != this->image_->height; ++i)                                                                                               \
    {                                                                                                                                             \
        for (int j = 0; j != this->image_->width; ++j)                                                                                            \
        {                                                                                                                                         \
            /* note: if W<V>::components < U<V>::components ... see NOTE above */                                                                 \
            for (int member = 0; member != W<V>::components; ++member)                                                                            \
            {                                                                                                                                     \
                /* works without packing/alignment issues b/c all types are the same */                                                           \
                                                                                                                                                  \
                *(reinterpret_cast<typename T::type *> (this->image_->imageData + i * this->image_->widthStep + j) + member) =                    \
                       ftor.template operator()<typename T::type>                                                                                 \
                           (* (reinterpret_cast<typename V::type const *> (temp->imageData + i * temp->widthStep + j) + member)                   \
                           NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                                            \
            }                                                                                                                                     \
        }                                                                                                                                         \
    }                                                                                                                                             \
    ::cvReleaseImage(&temp);                                                                                                                      \
}
/**/

#define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
#define BOOST_PP_LOCAL_LIMITS (0, NICTA_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
#undef OPERATOR




template < typename T
         , template <typename T_> class U
         >
nicta::vibe::image::image<T, U> :: 
image(const nicta::vibe::image::image<T, nicta::vibe::image::gray_p>& channel1,
      const nicta::vibe::image::image<T, nicta::vibe::image::gray_p>& channel2,
      const nicta::vibe::image::image<T, nicta::vibe::image::gray_p>& channel3
     ) throw (nicta::nicta_exception)
: image_(::cvCreateImage( ::cvSize((*reinterpret_cast< ::IplImage* const * > (&channel1))->width,
                                   (*reinterpret_cast< ::IplImage* const * > (&channel1))->height)
                        , static_cast<int> (T::value), U<T>::components))
, init_hdr_(false)
, tuple_(header_, image_)
, xroi_(std::make_pair<unsigned int, unsigned int> (0, static_cast<unsigned int> (image_->width)))
, yroi_(std::make_pair<unsigned int, unsigned int> (0, static_cast<unsigned int> (image_->height)))
{
    // Copy-LIKE Constructor - creates image of 3 channels (by merging them together)

    BOOST_STATIC_ASSERT(( U<T>::components == 3 ));

    if (channel1.width() != channel2.width() ||
        channel1.width() != channel3.width() ||
        channel1.height() != channel2.height() ||
        channel1.height() != channel3.height())
    {
        ::cvReleaseImage(&this->image_);
        throw nicta::nicta_exception("3 input image planes must be equally dimenstioned", "image", "image");
    }
    ::cvMerge(channel1.template native_handle< >(),
              channel2.template native_handle< >(),
              channel3.template native_handle< >(),
              0,
              this->image_);
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::image::image<T, U> :: 
image(const nicta::vibe::image::image<T, nicta::vibe::image::gray_p>& channel1,
      const nicta::vibe::image::image<T, nicta::vibe::image::gray_p>& channel2,
      const nicta::vibe::image::image<T, nicta::vibe::image::gray_p>& channel3,
      const nicta::vibe::image::image<T, nicta::vibe::image::gray_p>& alpha_channel
     ) throw (nicta::nicta_exception)
: image_(::cvCreateImage( ::cvSize((*reinterpret_cast< ::IplImage* const * > (&channel1))->width,
                                   (*reinterpret_cast< ::IplImage* const * > (&channel1))->height)
                        , static_cast<int> (T::value), U<T>::components))
, init_hdr_(false)
, tuple_(header_, image_)
, xroi_(std::make_pair<unsigned int, unsigned int> (0, static_cast<unsigned int> (image_->width)))
, yroi_(std::make_pair<unsigned int, unsigned int> (0, static_cast<unsigned int> (image_->height)))
{
    // Copy-LIKE Constructor - creates image of 4 channels (by merging them together)

    BOOST_STATIC_ASSERT(( U<T>::components == 4 ));

    if (channel1.width() != channel2.width() ||
        channel1.width() != channel3.width() ||
        channel1.width() != alpha_channel.width() ||
        channel1.height() != channel2.height() ||
        channel1.height() != channel3.height() ||
        channel1.height() != alpha_channel.height())
    {
        ::cvReleaseImage(&this->image_);
        throw nicta::nicta_exception("4 input image planes must be equally dimenstioned", "image", "image");
    }
    ::cvMerge(channel1.template native_handle< >(),
              channel2.template native_handle< >(),
              channel3.template native_handle< >(),
              alpha_channel.template native_handle< >(),
              this->image_);
}


template < typename T
         , template <typename T_> class U
         >
boost::shared_ptr<typename nicta::vibe::image::splice_type<T, U>::type>
nicta::vibe::image::image<T, U> :: 
splice() const throw ()
{
    // Splice each channel
    // NOTE:  if have only one channel (gray_p, and BayerXX_p varieties), it makes no sense to do so - and compiler error will result

    boost::shared_ptr<typename nicta::vibe::image::splice_type<T, U>::type>  s_ptr(new typename nicta::vibe::image::splice_type<T, U>::type());

    s_ptr->template get<0>() =
      typename boost::tuples::element<0, typename nicta::vibe::image::splice_type<T, U>::type>::type
        (new typename boost::tuples::element< 0
                                            , typename nicta::vibe::image::splice_type<T, U>::type
                                            >::type::element_type(this->width(), this->height()));
    s_ptr->template get<1>() =
      typename boost::tuples::element<1, typename nicta::vibe::image::splice_type<T, U>::type>::type
        (new typename boost::tuples::element< 1
                                            , typename nicta::vibe::image::splice_type<T, U>::type
                                            >::type::element_type(this->width(), this->height()));
    s_ptr->template get<2>() =
      typename boost::tuples::element<2, typename nicta::vibe::image::splice_type<T, U>::type>::type
        (new typename boost::tuples::element< 2
                                            , typename nicta::vibe::image::splice_type<T, U>::type
                                            >::type::element_type(this->width(), this->height()));

    if (boost::tuples::length<typename nicta::vibe::image::splice_type<T, U>::type>::value == 4)
    {
        // there is an alpha-channel:

        s_ptr->template get<boost::tuples::length<typename nicta::vibe::image::splice_type<T, U>::type>::value - 1>() =
          typename boost::tuples::element< boost::tuples::length<typename nicta::vibe::image::splice_type<T, U>::type>::value - 1
                                         , typename nicta::vibe::image::splice_type<T, U>::type
                                         >::type
            (new typename boost::tuples::element< boost::tuples::length<typename nicta::vibe::image::splice_type<T, U>::type>::value - 1
                                                , typename nicta::vibe::image::splice_type<T, U>::type
                                                >::type::element_type(this->width(), this->height()));
    }

    ::cvSplit(this->image_,
              s_ptr->template get<0>()->template native_handle< >(),
              s_ptr->template get<1>()->template native_handle< >(),
              s_ptr->template get<2>()->template native_handle< >(),
              (boost::tuples::length<typename nicta::vibe::image::splice_type<T, U>::type>::value > 3) ?
              s_ptr->template get<boost::tuples::length<typename nicta::vibe::image::splice_type<T, U>::type>::value - 1>()->template native_handle< >() : 0);

    return s_ptr;
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::image::image<T, U> &
nicta::vibe::image::image<T, U> :: 
operator=(const nicta::vibe::image::image<T, U>& rhs) throw ()
{
    // Assignment Op

    if (this != &rhs)
    {
        if (!this->init_hdr_)
        {
            ::cvReleaseImage(&this->image_);
        }
        else
        {
            this->init_hdr_ = !this->init_hdr_;
        }
        this->image_ = ::cvCloneImage(rhs.image_);
        this->xroi_ = rhs.xroi_;
        this->yroi_ = rhs.yroi_;
    }
    return *this;
}


template < typename T
         , template <typename T_> class U
         >
template <template <typename T_> class V>
nicta::vibe::image::image<T, U> &
nicta::vibe::image::image<T, U> :: 
operator=(const nicta::vibe::image::image<T, V>& rhs) throw (nicta::nicta_exception)
{
    // Assignment Op - color space conversion (V->U) on rhs (dump this->image_)

    if (this != &rhs)
    {
        if (rhs.image_->roi)
        {
            throw nicta::nicta_exception("Cannot assign to lvalue image object from rvalue having defined ROI", "operator=", "image<T, U>");
        }

        if (!this->init_hdr_)
        {
            ::cvReleaseImage(&this->image_);
        }
        else
        {
            this->init_hdr_ = !this->init_hdr_;
        }
        this->image_ = ::cvCreateImage(::cvSize((*reinterpret_cast< ::IplImage* const * > (&rhs))->width,
                                                (*reinterpret_cast< ::IplImage* const * > (&rhs))->height),
                                       static_cast<int> (T::value), U<T>::components);

        // get the CV_SRC2DST identifier
        int id = nicta::vibe::image::detail::conversion_map<V<T>::encoding, U<T>::encoding>::type::value;

        ::cvCvtColor(*reinterpret_cast< ::IplImage* const * > (&rhs), this->image_, id);

        this->xroi_ = std::make_pair<unsigned int, unsigned int> (0, static_cast<unsigned int> (image_->width));
        this->yroi_ = std::make_pair<unsigned int, unsigned int> (0, static_cast<unsigned int> (image_->height));
    }
    return *this;
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::image::image<T, U> :: operator!= (const nicta::vibe::image::image<T, U>& rhs) const throw ()
{
    // Logical-inEquality Op
    // NOTE:  ignores ROI settings !

    BOOST_STATIC_ASSERT( boost::is_floating_point<T>::value == false );

    return !this->nicta::vibe::image::image<T, U>::operator== (rhs);
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::image::image<T, U> :: operator== (const nicta::vibe::image::image<T, U>& rhs) const throw ()
{
    // Logical-Equality Op
    // NOTE:  ignores ROI settings !

    BOOST_STATIC_ASSERT( boost::is_floating_point<T>::value == false );

    if (this->image_->nSize     == rhs.image_->nSize &&
        this->image_->nChannels == rhs.image_->nChannels &&
        /* this->image_->depth     == rhs.image_->depth && */
        this->image_->width     == rhs.image_->width &&
        this->image_->height    == rhs.image_->height &&
        /* this->image_->widthStep == rhs.image_->widthStep && */
        this->image_->imageSize == rhs.image_->imageSize)
    {
        return !memcmp(this->image_->imageData, rhs.image_->imageData, this->image_->imageSize);
    }
    return false;
}


template < typename T
         , template <typename T_> class U
         >
inline
U<T>*
nicta::vibe::image::image<T, U> :: 
operator[](unsigned int idx)
{
    // Access image row
    // WARNING:  this **IGNORES** ROI settings !!
    // 
    // Image<depth_8u, bgr_p> k(filename);
    // k.operator[i][j]
    //

    return reinterpret_cast<U<T> *> (this->image_->imageData + idx * this->image_->widthStep);
}


template < typename T
         , template <typename T_> class U
         >
inline
const U<T>* const
nicta::vibe::image::image<T, U> :: 
operator[](unsigned int idx) const
{
    // Access image row - const version
    // WARNING:  this **IGNORES** ROI settings !!
    // 
    // Image<depth_8u, bgr_p> k(filename);
    // k.operator[i][j]
    //

    return reinterpret_cast<const U<T> * const> (this->image_->imageData + idx * this->image_->widthStep);
}


template < typename T
         , template <typename T_> class U
         >
inline
typename T::type
nicta::vibe::image::image<T, U> :: 
operator()(unsigned int row,
           unsigned int column,
           unsigned int plane
          ) throw (nicta::nicta_exception)
{
    // Access image element:  bounds checked variant
    // WARNING:  this **DOES** account for ROI !!
    //
    // Image<depth_8u, bgr_p> k(filename)
    // k.operator()(2, 3, 0)
    //

    if (row    >= this->yroi_.second ||
        column >= this->xroi_.second ||
        plane  >= static_cast<unsigned int> (U<T>::components))
    {
        throw nicta::nicta_exception("Bad Argument Value - out of bounds error", "image<T, U>", "image<T, U>");
    }

    // works without packing/alignment issues b/c all types are the same
    return *(reinterpret_cast<typename T::type *> (&(* this)[this->yroi_.first + row][this->xroi_.first + column]) + plane);
}


template < typename T
         , template <typename T_> class U
         >
inline
const typename T::type
nicta::vibe::image::image<T, U> :: 
operator()(unsigned int row,
           unsigned int column,
           unsigned int plane
          ) const throw (nicta::nicta_exception)
{
    // Access image element - const version:  bounds checked variant
    // WARNING:  this **DOES** account for ROI !!
    //
    // Image<depth_8u, bgr_p> k(filename)
    // k.operator()(2, 3, 0)
    //

    if (row    >= this->yroi_.second ||
        column >= this->xroi_.second ||
        plane  >= static_cast<unsigned int> (U<T>::components))
    {
        throw nicta::nicta_exception("Bad Argument Value - out of bounds error", "image<T, U>", "image<T, U>");
    }

    // works without packing/alignment issues b/c all types are the same
    return *(reinterpret_cast<const typename T::type *> (&(* this)[this->yroi_.first + row][this->xroi_.first + column]) + plane);
}


template < typename T
         , template <typename T_> class U
         >
template <template <typename T_> class V>
boost::shared_ptr<nicta::vibe::image::image<T, V> > 
nicta::vibe::image::image<T, U> :: 
convert() const throw (nicta::nicta_exception)
{
    // Clone and map to sought color space (U->V)
    // usage:  image.as_color<nicta::vibe::image::gray_p> ();

    if (boost::is_same<U<T>, V<T> >::value)
    {
        /* Just clone, no conversions necessary */
        return boost::shared_ptr<nicta::vibe::image::image<T, V> > (new nicta::vibe::image::image<T, V> (this->image_));
    }

    if (this->image_->roi)
    {
        throw nicta::nicta_exception("Cannot map color-space whilst having defined ROI", "convert<Pixel>", "image<T, U>");
    }

    // get the CV_SRC2DST identifier
    int id = nicta::vibe::image::detail::conversion_map<U<T>::encoding, V<T>::encoding>::type::value;

    boost::shared_ptr< ::IplImage > dst( ::cvCreateImage( ::cvSize(this->image_->width, this->image_->height)
                                                        , static_cast<int> (T::value)
                                                        , V<T>::components)
                                       , nicta::vibe::image::detail::iplimage_deleter());

    ::cvCvtColor(this->image_, &*dst, id);
    return boost::shared_ptr<nicta::vibe::image::image<T, V> > (new nicta::vibe::image::image<T, V> (&*dst));
}



#define OPERATOR(z, n, _)                                                                                                                         \
                                                                                                                                                  \
template < typename T                                                                                                                             \
         , template <typename T_> class U                                                                                                         \
         >                                                                                                                                        \
template < typename V                                                                                                                             \
         , typename Functor                                                                                                                       \
         NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename R))                                                                       \
         >                                                                                                                                        \
boost::shared_ptr<nicta::vibe::image::image<V, U> >                                                                                               \
nicta::vibe::image::image<T, U> ::                                                                                                                \
convert(BOOST_PP_ENUM_BINARY_PARAMS(n, R, a)) const                                                                                               \
{                                                                                                                                                 \
    /* Clone and map to sought color depth (T->V) */                                                                                              \
    /* usage:  img1.as_depth<nicta::vibe::image::depth_16u, nicta::vibe::image::algorithm::identity_translation> (); */                           \
                                                                                                                                                  \
    if (boost::is_same<T, V>::value)                                                                                                              \
    {                                                                                                                                             \
        /* Just clone, no conversions necessary */                                                                                                \
        return boost::shared_ptr<nicta::vibe::image::image<V, U> > (new nicta::vibe::image::image<V, U> (this->image_));                          \
    }                                                                                                                                             \
                                                                                                                                                  \
    boost::shared_ptr<nicta::vibe::image::image<V, U> >                                                                                           \
        s_ptr(new nicta::vibe::image::image<V, U> (nicta::quantity::width<unsigned int>(this->image_->width),                                     \
                                                   nicta::quantity::height<unsigned int>(this->image_->height)));                                 \
                                                                                                                                                  \
    for (int i = 0; i != this->image_->height; ++i)                                                                                               \
    {                                                                                                                                             \
        for (int j = 0; j != this->image_->width; ++j)                                                                                            \
        {                                                                                                                                         \
            /* access each member of struct U & cast */                                                                                           \
            for (int member = 0; member != U<T>::components; ++member)                                                                            \
            {                                                                                                                                     \
                /* works without packing/alignment issues b/c all types are the same */                                                           \
                                                                                                                                                  \
                /* FIXME:  maybe do mpl stuff with the const int the cast below ...if *this is const.. then add the const...  */                  \
                                                                                                                                                  \
                *(reinterpret_cast<typename V::type *> (&(* s_ptr)[i][j]) + member) =                                                             \
                       Functor().template operator()<typename V::type> (* (reinterpret_cast<typename T::type const *> (&(* this)[i][j]) + member) \
                                                                       NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                \
            }                                                                                                                                     \
        }                                                                                                                                         \
    }                                                                                                                                             \
    return s_ptr;                                                                                                                                 \
}                                                                                                                                                 \
                                                                                                                                                  \
                                                                                                                                                  \
template < typename T                                                                                                                             \
         , template <typename T_> class U                                                                                                         \
         >                                                                                                                                        \
template < typename V                                                                                                                             \
         , template <typename V_> class W                                                                                                         \
         , typename Functor                                                                                                                       \
         NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, typename R))                                                                       \
         >                                                                                                                                        \
boost::shared_ptr<nicta::vibe::image::image<V, W> >                                                                                               \
nicta::vibe::image::image<T, U> ::                                                                                                                \
convert(BOOST_PP_ENUM_BINARY_PARAMS(n, R, a)) const                                                                                               \
{                                                                                                                                                 \
    /* Clone and map to sought - color -depth (V->T) & -space (W->U) conversion */                                                                \
    /* usage:  img.as_image<nicta::vibe::image::depth_8s, nicta::vibe::image::gray_p, nicta::vibe::image::algorithm::identity_translation> (); */ \
                                                                                                                                                  \
    /* ---------- */                                                                                                                              \
                                                                                                                                                  \
    if (boost::is_same<U<T>, W<V> >::value)                                                                                                       \
    {                                                                                                                                             \
        /* Just clone, no conversions necessary */                                                                                                \
        return boost::shared_ptr<nicta::vibe::image::image<V, W> > (new nicta::vibe::image::image<V, W> (this->image_));                          \
    }                                                                                                                                             \
                                                                                                                                                  \
    /* ---------- */                                                                                                                              \
                                                                                                                                                  \
    if (boost::is_same<U<T>, W<T> >::value)                                                                                                       \
    {                                                                                                                                             \
        /* Pixels (U,W) is same, convert Depth (T->V) */                                                                                          \
        /* return this->as_depth<V, Functor NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, R))>(); */                                      \
                                                                                                                                                  \
        boost::shared_ptr<nicta::vibe::image::image<V, W> >                                                                                       \
            s_ptr(new nicta::vibe::image::image<V, W> (nicta::quantity::width<unsigned int>(this->image_->width),                                 \
                                                       nicta::quantity::height<unsigned int>(this->image_->height)));                             \
                                                                                                                                                  \
        for (int i = 0; i != this->image_->height; ++i)                                                                                           \
        {                                                                                                                                         \
            for (int j = 0; j != this->image_->width; ++j)                                                                                        \
            {                                                                                                                                     \
                /* access each member of struct W & cast */                                                                                       \
                for (int member = 0; member != W<V>::components; ++member)                                                                        \
                {                                                                                                                                 \
                    /* works without packing/alignment issues b/c all types are the same */                                                       \
                                                                                                                                                  \
                    /* FIXME:  maybe do mpl stuff with the const int the cast below ...if *this is const.. then add the const...  */              \
                                                                                                                                                      \
                    *(reinterpret_cast<typename V::type *> (&(* s_ptr)[i][j]) + member) =                                                             \
                           Functor().template operator()<typename V::type> (* (reinterpret_cast<typename T::type const *> (&(* this)[i][j]) + member) \
                                                                           NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                \
                }                                                                                                                                     \
            }                                                                                                                                     \
        }                                                                                                                                         \
        return s_ptr;                                                                                                                             \
    }                                                                                                                                             \
                                                                                                                                                  \
    /* ---------- */                                                                                                                              \
                                                                                                                                                  \
    if (boost::is_same<T, V>::value)                                                                                                              \
    {                                                                                                                                             \
        /* Depth (T,V) is same, convert Pixels (U->W) */                                                                                          \
        /* return this->as_color<W>(); */                                                                                                         \
                                                                                                                                                  \
        if (this->image_->roi)                                                                                                                    \
        {                                                                                                                                         \
            throw nicta::nicta_exception("Cannot map color-space whilst having defined ROI", "convert<Depth, Pixel>", "image<T, U>");             \
        }                                                                                                                                         \
                                                                                                                                                  \
        /* get the CV_SRC2DST identifier */                                                                                                       \
        int id = nicta::vibe::image::detail::conversion_map<U<T>::encoding, W<V>::encoding>::type::value;                                         \
                                                                                                                                                  \
        boost::shared_ptr< ::IplImage > dst( ::cvCreateImage( ::cvSize(this->image_->width, this->image_->height)                                 \
                                                            , static_cast<int> (T::value)                                                         \
                                                            , W<T>::components)                                                                   \
                                           , nicta::vibe::image::detail::iplimage_deleter());                                                     \
                                                                                                                                                  \
        ::cvCvtColor(this->image_, &*dst, id);                                                                                                    \
        return boost::shared_ptr<nicta::vibe::image::image<V, W> > (new nicta::vibe::image::image<V, W> (&*dst));                                 \
    }                                                                                                                                             \
                                                                                                                                                  \
                                                                                                                                                  \
    /* ---------- */                                                                                                                              \
                                                                                                                                                  \
                                                                                                                                                  \
    if (this->image_->roi)                                                                                                                        \
    {                                                                                                                                             \
        throw nicta::nicta_exception("Cannot map color-space whilst having defined ROI", "convert<Depth, Pixel>", "image<T, U>");                 \
    }                                                                                                                                             \
                                                                                                                                                  \
    /* get the CV_SRC2DST identifier & convert to new color space (U->W), (depth unchanged) */                                                    \
    int id = nicta::vibe::image::detail::conversion_map<U<T>::encoding, W<T>::encoding>::type::value;                                             \
                                                                                                                                                  \
    ::IplImage* temp = ::cvCreateImage(::cvSize(this->image_->width, this->image_->height), static_cast<int> (T::value), W<T>::components);       \
    ::cvCvtColor(this->image_, temp, id);                                                                                                         \
                                                                                                                                                  \
                                                                                                                                                  \
    boost::shared_ptr<nicta::vibe::image::image<V, W> >                                                                                           \
        s_ptr(new nicta::vibe::image::image<V, W> (nicta::quantity::width<unsigned int>(this->image_->width),                                     \
                                                   nicta::quantity::height<unsigned int>(this->image_->height)));                                 \
                                                                                                                                                  \
    /* convert to new color depth (T->V) */                                                                                                       \
    for (int i = 0; i != this->image_->height; ++i)                                                                                               \
    {                                                                                                                                             \
        for (int j = 0; j != this->image_->width; ++j)                                                                                            \
        {                                                                                                                                         \
            /* access each member of struct W & cast */                                                                                           \
            for (int member = 0; member != W<V>::components; ++member)                                                                            \
            {                                                                                                                                     \
                /* works without packing/alignment issues b/c all types are the same */                                                           \
                                                                                                                                                  \
                *(reinterpret_cast<typename V::type *> (&(* s_ptr)[i][j]) + member) =                                                             \
                       Functor().template operator()<typename V::type>                                                                            \
                           (* (reinterpret_cast<typename T::type const *> (temp->imageData + i * temp->widthStep + j) + member)                   \
                           NICTA_CONDITIONAL_COMMA(_, n, BOOST_PP_ENUM_PARAMS(n, a)));                                                            \
            }                                                                                                                                     \
        }                                                                                                                                         \
    }                                                                                                                                             \
    ::cvReleaseImage(&temp);                                                                                                                      \
                                                                                                                                                  \
    return s_ptr;                                                                                                                                 \
}
/**/


#define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
#define BOOST_PP_LOCAL_LIMITS (0, NICTA_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
#undef OPERATOR



template < typename T
         , template <typename T_> class U
         >
boost::shared_ptr<nicta::vibe::image::image<T, U> >
nicta::vibe::image::image<T, U> :: 
select(nicta::quantity::x_origin<unsigned int> x_origin,
       nicta::quantity::y_origin<unsigned int> y_origin,
       nicta::quantity::x_absolute_displacement<unsigned int> x_displacement,
       nicta::quantity::y_absolute_displacement<unsigned int> y_displacement
      ) const throw (nicta::nicta_exception)
{
    // Cut out a region from this image
    //
    // NOTE:  if displacement == 0, take from origin, till extent of image boundary
    //

    if (x_origin.operator()<int>() >= this->image_->width)
    {
        throw nicta::nicta_exception("x_origin is beyond bounds of image width", "select", "image<T, U>");
    }
    if (y_origin.operator()<int>() >= this->image_->height)
    {
        throw nicta::nicta_exception("y_origin is beyond bounds of image height", "select", "image<T, U>");
    }

    int width  = x_displacement.operator()< >() ? x_displacement.operator()<int>() : this->image_->width  - x_origin.operator()<int>();
    int height = y_displacement.operator()< >() ? y_displacement.operator()<int>() : this->image_->height - y_origin.operator()<int>();

    boost::shared_ptr<nicta::vibe::image::image<T, U> >
        s_ptr(new nicta::vibe::image::image<T, U> (nicta::quantity::width<unsigned int>(width),
                                                   nicta::quantity::height<unsigned int>(height)));

    for (int i = 0; i != height; ++i)
    {
        for (int j = 0; j != width; ++j)
        {
            // access each member of struct U & cast
            for (int member = 0; member != U<T>::components; ++member)
            {
                // works without packing/alignment issues b/c all types are the same

                *(reinterpret_cast<typename T::type *> (&(* s_ptr)[i][j]) + member) =
                       *(reinterpret_cast<typename T::type const *> (&(* this)[y_origin.operator()< >() + i][x_origin.operator()< >() + j]) + member);
            }
        }
    }
    return s_ptr;
}


template < typename T
         , template <typename T_> class U
         >
template <nicta::vibe::image::FlipModel M>
nicta::vibe::image::image<T, U> &
nicta::vibe::image::image<T, U> :: 
flip(const nicta::vibe::image::destination_model<nicta::vibe::image::inplace> /*model*/) throw (nicta::nicta_exception)
{
    // Flip image orientation along either { x- ; y- ; x- & y- } axis **AND** specify output as in-place

    if (nicta::vibe::image::flip_model<M>::value != nicta::vibe::image::flip_model<nicta::vibe::image::identity_axis>::value)
    {
        if (this->image_->roi)
        {
            throw nicta::nicta_exception("Cannot flip image whilst having defined ROI", "flip", "image<T, U>");
        }

        ::cvFlip(this->image_, 0, nicta::vibe::image::flip_model<M>::value);
    }
    return *this;
}


template < typename T
         , template <typename T_> class U
         >
template <nicta::vibe::image::FlipModel M>
boost::shared_ptr<nicta::vibe::image::image<T, U> >
nicta::vibe::image::image<T, U> :: 
flip(const nicta::vibe::image::destination_model<nicta::vibe::image::copy> /*model*/) const throw (nicta::nicta_exception)
{
    // Flip image orientation along either x-, y-, or x- & y- axis **AND** specify output as copy

    if (this->image_->roi)
    {
        throw nicta::nicta_exception("Cannot flip image whilst having defined ROI", "flip", "image<T, U>");
    }

    if (nicta::vibe::image::flip_model<M>::value != nicta::vibe::image::flip_model<nicta::vibe::image::identity_axis>::value)
    {
        boost::shared_ptr<nicta::vibe::image::image<T, U> >
            s_ptr(new nicta::vibe::image::image<T, U> (nicta::quantity::width<unsigned int>(this->image_->width),
                                                       nicta::quantity::height<unsigned int>(this->image_->height)));

        ::cvFlip(this->image_, *reinterpret_cast< ::IplImage* const * > (&*s_ptr), nicta::vibe::image::flip_model<M>::value);
        return s_ptr;
    }
    return boost::shared_ptr<nicta::vibe::image::image<T, U> >(new nicta::vibe::image::image<T, U> (*this));
}


template < typename T
         , template <typename T_> class U
         >
template <nicta::vibe::image::InterpolationModel M>
void
nicta::vibe::image::image<T, U> :: 
scale(nicta::vibe::image::image<T, U>& dest) const throw ()
{
    // Interpolate this->image_ to size of dest, and output to dest

    ::cvResize(this->image_, dest.image_, nicta::vibe::image::interpolation_model<M>::value);
}


template < typename T
         , template <typename T_> class U
         >
template <nicta::vibe::image::InterpolationModel M>
boost::shared_ptr<nicta::vibe::image::image<T, U> >
nicta::vibe::image::image<T, U> :: 
scale(nicta::quantity::width<unsigned int> width,
      nicta::quantity::height<unsigned int> height) const throw ()
{
    // Interpolate this->image_ to user-specified size(widht, height)

    boost::shared_ptr<nicta::vibe::image::image<T, U> > s_ptr(new nicta::vibe::image::image<T, U> (width, height));
    ::cvResize(this->image_, s_ptr->image_, nicta::vibe::image::interpolation_model<M>::value);
    return s_ptr;
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::image::OriginModel
nicta::vibe::image::image<T, U> :: 
origin() const throw ()
{
    // Returns the origin sentinel indicating image origin
    // NOTE:  TopLeft is usually the default for OpenCV *except* on frame-"grabbing"

    return this->image_->origin == IPL_ORIGIN_BL
           ? nicta::vibe::image::BottomLeft
           : nicta::vibe::image::TopLeft;
}


template < typename T
         , template <typename T_> class U
         >
template <nicta::vibe::image::WriteModel M>
void
nicta::vibe::image::image<T, U> :: 
save_as(const char* filename) const throw (nicta::nicta_exception)
{
    // Save image as 8-bit gray, or 8-bit color (BGR)
    // ...can't be bothered doing class specializations just for this one ..(eh, oh well!)
    //
    // NOTE:  filetype saved as depends on extension
    // Supported image formats: BMP, DIB, JPEG, JPG, JPE, PNG, PBM, PGM, PPM, SR, RAS, TIFF, TIF
    //
    // usage:  img.save_as<nicta::vibe::image::asImage> (filename, nicta::vibe::image::write_model<nicta::vibe::image::asColor>());
    //


    bool depth_8bit(T::value == static_cast<long int> (IPL_DEPTH_8U) || T::value == static_cast<long int> (IPL_DEPTH_8S));
    bool color_gray(static_cast<int> (U<T>::encoding) == static_cast<int> (nicta::vibe::image::GRAY));
    bool color_bgr(static_cast<int> (U<T>::encoding) == static_cast<int> (nicta::vibe::image::BGR));

    switch (static_cast<int> (nicta::vibe::image::write_model<M>::value))
    {
        case nicta::vibe::image::asGray :
        {
            if (depth_8bit && color_gray)
            {
                int success = ::cvSaveImage(filename, this->image_);
                if (!success)
                {
                    throw nicta::nicta_exception("Could not save file:  " + std::string(filename), "save_as", "image");
                }
                return;
            }
            else if (!depth_8bit)
            {
                boost::shared_ptr<const nicta::vibe::image::image<nicta::vibe::image::depth_8u, nicta::vibe::image::gray_p> > sp = 
                       this->convert<nicta::vibe::image::depth_8u, nicta::vibe::image::gray_p, nicta::vibe::image::algorithm::identity_translation>();
                int success = ::cvSaveImage(filename, *reinterpret_cast< ::IplImage* const * > (&*sp));

                if (!success)
                {
                    throw nicta::nicta_exception("Could not save file:  " + std::string(filename), "save_as", "image");
                }
                return;
            }
            else if (depth_8bit)
            {
                boost::shared_ptr<const nicta::vibe::image::image<T, nicta::vibe::image::gray_p> > sp = this->convert<nicta::vibe::image::gray_p>();
                int success = ::cvSaveImage(filename, *reinterpret_cast< ::IplImage* const * > (&*sp));

                if (!success)
                {
                    throw nicta::nicta_exception("Could not save file:  " + std::string(filename), "save_as", "image");
                }
                return;
            }

            break;
        }


        case nicta::vibe::image::asColor :
        {
            if (depth_8bit && color_bgr)
            {
                int success = ::cvSaveImage(filename, this->image_);
                if (!success)
                {
                    throw nicta::nicta_exception("Could not save file:  " + std::string(filename), "save_as", "image");
                }
                return;
            }
            else if (!depth_8bit)
            {
                boost::shared_ptr<const nicta::vibe::image::image<nicta::vibe::image::depth_8u, nicta::vibe::image::bgr_p> > sp = 
                       this->convert<nicta::vibe::image::depth_8u, nicta::vibe::image::bgr_p, nicta::vibe::image::algorithm::identity_translation>();
                int success = ::cvSaveImage(filename, *reinterpret_cast< ::IplImage* const * > (&*sp));

                if (!success)
                {
                    throw nicta::nicta_exception("Could not save file:  " + std::string(filename), "save_as", "image");
                }
                return;
            }
            else if (depth_8bit)
            {
                boost::shared_ptr<const nicta::vibe::image::image<T, nicta::vibe::image::bgr_p> > sp = this->convert<nicta::vibe::image::bgr_p>();
                int success = ::cvSaveImage(filename, *reinterpret_cast< ::IplImage* const * > (&*sp));

                if (!success)
                {
                    throw nicta::nicta_exception("Could not save file:  " + std::string(filename), "save_as", "image");
                }
                return;
            }

            break;
        }


        case nicta::vibe::image::asImage :
        {
            if (depth_8bit && (color_gray || color_bgr))
            {
                int success = ::cvSaveImage(filename, this->image_);
                if (!success)
                {
                    throw nicta::nicta_exception("Could not save file:  " + std::string(filename), "save_as", "image");
                }
                return;
            }
            else if (!depth_8bit)
            {
                if (color_gray)
                {
                    boost::shared_ptr<const nicta::vibe::image::image<nicta::vibe::image::depth_8u, U> > sp =
                           this->convert<nicta::vibe::image::depth_8u, nicta::vibe::image::algorithm::identity_translation>();
                    int success = ::cvSaveImage(filename, *reinterpret_cast< ::IplImage* const * > (&*sp));

                    if (!success)
                    {
                        throw nicta::nicta_exception("Could not save file:  " + std::string(filename), "save_as", "image");
                    }
                    return;
                }
                else
                {
                    boost::shared_ptr<const nicta::vibe::image::image<nicta::vibe::image::depth_8u, nicta::vibe::image::bgr_p> > sp = 
                           this->convert<nicta::vibe::image::depth_8u, nicta::vibe::image::bgr_p, nicta::vibe::image::algorithm::identity_translation>();
                    int success = ::cvSaveImage(filename, *reinterpret_cast< ::IplImage* const * > (&*sp));

                    if (!success)
                    {
                        throw nicta::nicta_exception("Could not save file:  " + std::string(filename), "save_as", "image");
                    }
                    return;
                }
            }
            else // is 8bit, not gray, not bgr => is 8bit, color (non-bgr)
            {
                boost::shared_ptr<const nicta::vibe::image::image<T, nicta::vibe::image::bgr_p> > sp = this->convert<nicta::vibe::image::bgr_p>();
                int success = ::cvSaveImage(filename, *reinterpret_cast< ::IplImage* const * > (&*sp));

                if (!success)
                {
                    throw nicta::nicta_exception("Could not save file:  " + std::string(filename), "save_as", "image");
                }
            }

           break;
        }
    }
}


template < typename T
         , template <typename T_> class U
         >
template <typename nicta::vibe::image::Native N>
typename nicta::vibe::image::native_type<N>::type &
nicta::vibe::image::image<T, U> :: 
native_handle() throw ()
{
    // Return the native image type

    return this->tuple_.template get<static_cast<int> (N)>();
}


template < typename T
         , template <typename T_> class U
         >
template <typename nicta::vibe::image::Native N>
const typename nicta::vibe::image::native_type<N>::type &
nicta::vibe::image::image<T, U> :: 
native_handle() const throw ()
{
    // Return the native image type

    return this->tuple_.template get<static_cast<int> (N)>();
}


template < typename T
         , template <typename T_> class U
         >
nicta::quantity::width<unsigned int>
nicta::vibe::image::image<T, U> :: 
width() const throw ()
{
    // Get the image width

    return nicta::quantity::width<unsigned int>(static_cast<unsigned int> (this->image_->width));
}


template < typename T
         , template <typename T_> class U
         >
nicta::quantity::height<unsigned int>
nicta::vibe::image::image<T, U> :: 
height() const throw ()
{
    // Get the image height

    return nicta::quantity::height<unsigned int>(static_cast<unsigned int> (this->image_->height));
}


template < typename T
         , template <typename T_> class U
         >
nicta::quantity::x_origin<unsigned int>
nicta::vibe::image::image<T, U> :: 
ROI_x_origin() const throw ()
{
    // Get the ROI x-origin
    // NOTE:  will be '0' if no ROI is set
    // NOTE:  this is the same value as:  this->xroi_.first;

    return nicta::quantity::x_origin<unsigned int>(static_cast<unsigned int> (::cvGetImageROI(this->image_).x));
}


template < typename T
         , template <typename T_> class U
         >
nicta::quantity::y_origin<unsigned int>
nicta::vibe::image::image<T, U> :: 
ROI_y_origin() const throw ()
{
    // Get the ROI y-origin
    // NOTE:  will be '0' if no ROI is set
    // NOTE:  this is the same value as:  this->yroi_.first;

    return nicta::quantity::x_origin<unsigned int>(static_cast<unsigned int> (::cvGetImageROI(this->image_).y));
}


template < typename T
         , template <typename T_> class U
         >
nicta::quantity::width<unsigned int>
nicta::vibe::image::image<T, U> :: 
ROI_width() const throw ()
{
    // Get the ROI width
    // NOTE:  will be same as this->image_->width if no ROI is set
    // NOTE:  this is the same value as:  this->xroi_.second;

    return nicta::quantity::width<unsigned int>(static_cast<unsigned int> (::cvGetSize(this->image_).width));
}


template < typename T
         , template <typename T_> class U
         >
nicta::quantity::height<unsigned int>
nicta::vibe::image::image<T, U> :: 
ROI_height() const throw ()
{
    // Get the ROI height
    // NOTE:  will be same as this->image_->height if no ROI is set
    // NOTE:  this is the same value as:  this->yroi_.second;

    return nicta::quantity::height<unsigned int>(static_cast<unsigned int> (::cvGetSize(this->image_).height));
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::image::image<T, U> :: 
have_ROI() const throw ()
{
    // Query if underlying IplImage* image_ has ROI set

    return !(this->image_->roi == 0);
}


template < typename T
         , template <typename T_> class U
         >
void
nicta::vibe::image::image<T, U> :: 
clear_ROI() throw ()
{
    // Unset the ROI for this->image_

    ::cvResetImageROI(this->image_);
}


template < typename T
         , template <typename T_> class U
         >
void
nicta::vibe::image::image<T, U> :: 
set_ROI(nicta::quantity::x_origin<unsigned int> x_origin,
        nicta::quantity::y_origin<unsigned int> y_origin,
        nicta::quantity::width<unsigned int> width,
        nicta::quantity::height<unsigned int> height
       ) throw (nicta::nicta_exception)
{
    // Set the ROI for this->image_

    if ((x_origin.template operator()< >() + width.template operator()< >() > this->width().template operator()< >()) ||
        (y_origin.template operator()< >() + height.template operator()< >() > this->height().template operator()< >()))
    {
        throw nicta::nicta_exception("invalid ROI origin and/or dimensions", "set_ROI", "image<T, U>");
    }

    int rows = height.template operator()<int>();
    int cols = width.template operator()<int>();
    if (!rows) rows = 1;
    if (!cols) cols = 1;
    ::CvRect rect = ::cvRect(x_origin.template operator()<int>(),
                             y_origin.template operator()<int>(),
                             cols,
                             rows);
    ::cvSetImageROI(this->image_, rect);
    this->xroi_ = std::make_pair<unsigned int, unsigned int> (x_origin.template operator()< >(), static_cast<unsigned int> (cols));
    this->yroi_ = std::make_pair<unsigned int, unsigned int> (y_origin.template operator()< >(), static_cast<unsigned int> (rows));
}


template < typename T
         , template <typename T_> class U
         >
::CvRect
nicta::vibe::image::image<T, U> :: 
get_ROI() const throw()
{
    // Return the ROI for this->image_
    // NOTE:  if there is no ROI set, returned is CvRect[0, 0, this->image_->width, this->image_->height]
    // NOTE:  this information is also contained in:  { this->xroi_ ; this->yroi_ }  dually

    return ::cvGetImageROI(this->image_);
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::image::image<T, U> ::
have_COI() const throw ()
{
    // Query if underlying IplImage* image_ has COI set

    return this->image_->roi && this->image_->roi->coi;
}


template < typename T
         , template <typename T_> class U
         >
void
nicta::vibe::image::image<T, U> ::
clear_COI() throw ()
{
    // Unset the COI for this->image_ -- means *selecting* all channels

    ::cvSetImageCOI(this->image_, 0);
}


template < typename T
         , template <typename T_> class U
         >
void
nicta::vibe::image::image<T, U> ::
set_COI(typename U<T>::channel::coi c) throw ()
{
    // Set the COI for this->image_

    ::cvSetImageCOI(this->image_, static_cast<int> (c) + !(U<T>::components > 1));
}


template < typename T
         , template <typename T_> class U
         >
typename U<T>::channel::coi
nicta::vibe::image::image<T, U> ::
get_COI() const throw()
{
    // Return the COI for this->image_
    // NOTE:  if there is "no" COI set (ie. if all channels *are* set), returned is '0' (indicating that all channels *are* set)

    return static_cast<typename U<T>::channel::coi> (::cvGetImageCOI(this->image_) - !(U<T>::components > 1));
}


template < typename T
         , template <typename T_> class U
         >
::CvSize
nicta::vibe::image::image<T, U> :: 
get_size() const throw()
{
    // Return the size-pair (width, height) of this->image_
    // NOTE:  this **WILL** account for ROI settings !!

    return ::cvGetSize(this->image_);
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::image::image<T, U> ::
show(nicta::vibe::image::WindowModel m) const throw ()
{
    // Pop image in a window on screen, at default OS coordinates, and autosize
    // NOTE:  image will be 'queued' for display, but won't display until do either a timed_show, or wait_key

    bool handle = true;
    std::string id = nicta::stringutil::atomic_to_string(this);
    if (!::cvGetWindowHandle(id.c_str()))
    {
        handle = false;
        ::cvNamedWindow(id.c_str(), static_cast<int> (m));
    }
    ::cvShowImage(id.c_str(), this->image_);
    return handle;
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::image::image<T, U> ::
show(const char* id,
     nicta::vibe::image::WindowModel m
    ) const throw ()
{
    // Pop image in a window on screen, at default OS coordinates, and autosize
    // NOTE:  image will be 'queued' for display, but won't display until do either a timed_show, or wait_key

    if (std::string(id).empty())
    {
        return this->show();
    }

    bool handle = true;
    if (!::cvGetWindowHandle(id))
    {
        handle = false;
        ::cvNamedWindow(id, static_cast<int> (m));
    }
    ::cvShowImage(id, this->image_);
    return handle;
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::image::image<T, U> ::
show(const char* id,
     nicta::quantity::width<unsigned int> width,
     nicta::quantity::height<unsigned int> height,
     nicta::vibe::image::WindowModel m
    ) const throw ()
{
    // Pop image in a window on screen, at default OS coordinates, and resize (autosize iff width || height == 0)
    // NOTE:  image will be 'queued' for display, but won't display until do either a timed_show, or wait_key

    bool handle = true;
    if (std::string(id).empty())
    {
        std::string id = nicta::stringutil::atomic_to_string(this);
        if (!::cvGetWindowHandle(id.c_str()))
        {
            handle = false;
            if (width.operator()< >() || height.operator()< >())
            {
                ::cvNamedWindow(id.c_str(), static_cast<int> (nicta::vibe::image::resize));
                ::cvResizeWindow(id.c_str(), width.operator()<int>(), height.operator()<int>());
            }
            else
            {
                ::cvNamedWindow(id.c_str(), static_cast<int> (m));
            }
        }
        ::cvShowImage(id.c_str(), this->image_);
    }
    else
    {
        if (!::cvGetWindowHandle(id))
        {
            handle = false;
            if (width.operator()< >() || height.operator()< >())
            {
                ::cvNamedWindow(id, static_cast<int> (nicta::vibe::image::resize));
                ::cvResizeWindow(id, width.operator()<int>(), height.operator()<int>());
            }
            else
            {
                ::cvNamedWindow(id, static_cast<int> (m));
            }
        }
        ::cvShowImage(id, this->image_);
    }
    return handle;
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::image::image<T, U> ::
show(const char* id,
     nicta::quantity::x_absolute_displacement<unsigned int> x_pos,
     nicta::quantity::y_absolute_displacement<unsigned int> y_pos,
     nicta::vibe::image::WindowModel m
    ) const throw ()
{
    // Pop image in a window on screen, at pixel coordinates [offset from upper-left corner of screen]
    // NOTE:  image will be 'queued' for display, but won't display until do either a timed_show, or wait_key
    // NOTE:  we only move the window on first show!

    bool handle = true;
    if (std::string(id).empty())
    {
        std::string id = nicta::stringutil::atomic_to_string(this);
        if (!::cvGetWindowHandle(id.c_str()))
        {
            handle = false;
            ::cvNamedWindow(id.c_str(), static_cast<int> (m));
            ::cvMoveWindow(id.c_str(), x_pos.operator()<int>(), y_pos.operator()<int>());
        }
        ::cvShowImage(id.c_str(), this->image_);
    }
    else
    {
        if (!::cvGetWindowHandle(id))
        {
            handle = false;
            ::cvNamedWindow(id, static_cast<int> (m));
            ::cvMoveWindow(id, x_pos.operator()<int>(), y_pos.operator()<int>());
        }
        ::cvShowImage(id, this->image_);
    }
    return handle;
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::image::image<T, U> ::
show(const char* id,
     nicta::quantity::width<unsigned int> width,
     nicta::quantity::height<unsigned int> height,
     nicta::quantity::x_absolute_displacement<unsigned int> x_pos,
     nicta::quantity::y_absolute_displacement<unsigned int> y_pos,
     nicta::vibe::image::WindowModel m
    ) const throw ()
{
    // Pop image in a window on screen, at pixel coordinates [offset from upper-left corner of screen], and resize (autosize iff width || height == 0)
    // NOTE:  image will be 'queued' for display, but won't display until do either a timed_show, or wait_key
    // NOTE:  we only move the window on first show!

    bool handle = true;
    if (std::string(id).empty())
    {
        std::string id = nicta::stringutil::atomic_to_string(this);
        if (!::cvGetWindowHandle(id.c_str()))
        {
            handle = false;
            if (width.operator()< >() || height.operator()< >())
            {
                ::cvNamedWindow(id.c_str(), static_cast<int> (nicta::vibe::image::resize));
                ::cvResizeWindow(id.c_str(), width.operator()<int>(), height.operator()<int>());
            }
            else
            {
                ::cvNamedWindow(id.c_str(), static_cast<int> (m));
            }
            ::cvMoveWindow(id.c_str(), x_pos.operator()<int>(), y_pos.operator()<int>());
        }
        ::cvShowImage(id.c_str(), this->image_);
    }
    else
    {
        if (!::cvGetWindowHandle(id))
        {
            handle = false;
            if (width.operator()< >() || height.operator()< >())
            {
                ::cvNamedWindow(id, static_cast<int> (nicta::vibe::image::resize));
                ::cvResizeWindow(id, width.operator()<int>(), height.operator()<int>());
            }
            else
            {
                ::cvNamedWindow(id, static_cast<int> (m));
            }
            ::cvMoveWindow(id, x_pos.operator()<int>(), y_pos.operator()<int>());
        }
        ::cvShowImage(id, this->image_);
    }
    return handle;
}



template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::image::image<T, U> ::
show(const char* id,
     nicta::quantity::x_absolute_displacement<unsigned int> x_pos,
     nicta::quantity::y_absolute_displacement<unsigned int> y_pos,
     nicta::quantity::width<unsigned int> width,
     nicta::quantity::height<unsigned int> height,
     nicta::vibe::image::WindowModel m
    ) const throw ()
{
    // Pop image in a window on screen, at pixel coordinates [offset from upper-left corner of screen], and resize (autosize iff width || height == 0)
    // NOTE:  image will be 'queued' for display, but won't display until do either a timed_show, or wait_key
    // NOTE:  we only move the window on first show!

    return this->show(id, width, height, x_pos, y_pos, m);
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::image::image<T, U> ::
timed_show(boost::posix_time::time_duration td,
           nicta::vibe::image::WindowModel m
          ) const throw ()
{
    // Pop image in a window on screen, at default OS coordinates, with image up-time of 'td' time-duration
    // NOTE:  if 'td' == 0; image remains up indefinitely until a keypress event is registered

    bool handle = true;
    std::string id = nicta::stringutil::atomic_to_string(this);
    if (!::cvGetWindowHandle(id.c_str()))
    {
        handle = false;
        ::cvNamedWindow(id.c_str(), static_cast<int> (m));
    }
    ::cvShowImage(id.c_str(), this->image_);

    ::cvWaitKey(static_cast<int> (td.total_milliseconds()));
    ::cvDestroyWindow(id.c_str());
    return handle;
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::image::image<T, U> ::
timed_show(const char* id,
           boost::posix_time::time_duration td,
           nicta::vibe::image::WindowModel m
          ) const throw ()
{
    // Pop image in a window on screen, at default OS coordinates, with image up-time of 'td' time-duration
    // NOTE:  if 'td' == 0; image remains up indefinitely until a keypress event is registered

    if (std::string(id).empty())
    {
        return this->timed_show(td);
    }

    bool handle = true;
    if (!::cvGetWindowHandle(id))
    {
        handle = false;
        ::cvNamedWindow(id, static_cast<int> (m));
    }
    ::cvShowImage(id, this->image_);

    ::cvWaitKey(static_cast<int> (td.total_milliseconds()));
    ::cvDestroyWindow(id);
    return handle;
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::image::image<T, U> ::
timed_show(const char* id,
           boost::posix_time::time_duration td,
           nicta::quantity::width<unsigned int> width,
           nicta::quantity::height<unsigned int> height,
           nicta::vibe::image::WindowModel m
          ) const throw ()
{
    // Pop image in a window on screen, at default OS coordinates, with image up-time of 'td' time-duration, and resize (autosize iff width || height == 0)
    // NOTE:  if 'td' == 0; image remains up indefinitely until a keypress event is registered

    bool handle = true;
    if (std::string(id).empty())
    {
        std::string id = nicta::stringutil::atomic_to_string(this);
        if (!::cvGetWindowHandle(id.c_str()))
        {
            handle = false;
            if (width.operator()< >() || height.operator()< >())
            {
                ::cvNamedWindow(id.c_str(), static_cast<int> (nicta::vibe::image::resize));
                ::cvResizeWindow(id.c_str(), width.operator()<int>(), height.operator()<int>());
            }
            else
            {
                ::cvNamedWindow(id.c_str(), static_cast<int> (m));
            }
        }
        ::cvShowImage(id.c_str(), this->image_);

        ::cvWaitKey(static_cast<int> (td.total_milliseconds()));
        ::cvDestroyWindow(id.c_str());
    }
    else
    {
        if (!::cvGetWindowHandle(id))
        {
            handle = false;
            if (width.operator()< >() || height.operator()< >())
            {
                ::cvNamedWindow(id, static_cast<int> (nicta::vibe::image::resize));
                ::cvResizeWindow(id, width.operator()<int>(), height.operator()<int>());
            }
            else
            {
                ::cvNamedWindow(id, static_cast<int> (m));
            }
        }
        ::cvShowImage(id, this->image_);

        ::cvWaitKey(static_cast<int> (td.total_milliseconds()));
        ::cvDestroyWindow(id);
    }
    return handle;
}



template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::image::image<T, U> ::
timed_show(const char* id,
           boost::posix_time::time_duration td,
           nicta::quantity::x_absolute_displacement<unsigned int> x_pos,
           nicta::quantity::y_absolute_displacement<unsigned int> y_pos,
           nicta::vibe::image::WindowModel m
          ) const throw ()
{
    // Pop image in a window on screen, at pixel coordinates [offset from upper-left corner of screen], with image up-time of 'td' time-duration
    // NOTE:  if 'td' == 0; image remains up indefinitely until a keypress event is registered
    // NOTE:  we only move the window on first show!

    bool handle = true;
    if (std::string(id).empty())
    {
        std::string id = nicta::stringutil::atomic_to_string(this);
        if (!::cvGetWindowHandle(id.c_str()))
        {
            handle = false;
            ::cvNamedWindow(id.c_str(), static_cast<int> (m));
            ::cvMoveWindow(id.c_str(), x_pos.operator()<int>(), y_pos.operator()<int>());
        }
        ::cvShowImage(id.c_str(), this->image_);

        ::cvWaitKey(static_cast<int> (td.total_milliseconds()));
        ::cvDestroyWindow(id.c_str());
    }
    else
    {
        if (!::cvGetWindowHandle(id))
        {
            handle = false;
            ::cvNamedWindow(id, static_cast<int> (m));
            ::cvMoveWindow(id, x_pos.operator()<int>(), y_pos.operator()<int>());
        }
        ::cvShowImage(id, this->image_);

        ::cvWaitKey(static_cast<int> (td.total_milliseconds()));
        ::cvDestroyWindow(id);
    }
    return handle;
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::image::image<T, U> ::
timed_show(const char* id,
           boost::posix_time::time_duration td,
           nicta::quantity::width<unsigned int> width,
           nicta::quantity::height<unsigned int> height,
           nicta::quantity::x_absolute_displacement<unsigned int> x_pos,
           nicta::quantity::y_absolute_displacement<unsigned int> y_pos,
           nicta::vibe::image::WindowModel m
          ) const throw ()
{
    // Pop image in a window on screen, at pixel coordinates [offset from upper-left corner of screen], with image up-time of 'td' time-duration
    // ..., and resize (autosize iff width || height == 0)
    // NOTE:  if 'td' == 0; image remains up indefinitely until a keypress event is registered
    // NOTE:  we only move the window on first show!

    bool handle = true;
    if (std::string(id).empty())
    {
        std::string id = nicta::stringutil::atomic_to_string(this);
        if (!::cvGetWindowHandle(id.c_str()))
        {
            handle = false;
            if (width.operator()< >() || height.operator()< >())
            {
                ::cvNamedWindow(id.c_str(), static_cast<int> (nicta::vibe::image::resize));
                ::cvResizeWindow(id.c_str(), width.operator()<int>(), height.operator()<int>());
            }
            else
            {
                ::cvNamedWindow(id.c_str(), static_cast<int> (m));
            }
            ::cvMoveWindow(id.c_str(), x_pos.operator()<int>(), y_pos.operator()<int>());
        }
        ::cvShowImage(id.c_str(), this->image_);

        ::cvWaitKey(static_cast<int> (td.total_milliseconds()));
        ::cvDestroyWindow(id.c_str());
    }
    else
    {
        if (!::cvGetWindowHandle(id))
        {
            handle = false;
            if (width.operator()< >() || height.operator()< >())
            {
                ::cvNamedWindow(id, static_cast<int> (nicta::vibe::image::resize));
                ::cvResizeWindow(id, width.operator()<int>(), height.operator()<int>());
            }
            else
            {
                ::cvNamedWindow(id, static_cast<int> (m));
            }
            ::cvMoveWindow(id, x_pos.operator()<int>(), y_pos.operator()<int>());
        }
        ::cvShowImage(id, this->image_);

        ::cvWaitKey(static_cast<int> (td.total_milliseconds()));
        ::cvDestroyWindow(id);
    }
    return handle;
}


template < typename T
         , template <typename T_> class U
         >
bool
nicta::vibe::image::image<T, U> ::
timed_show(const char* id,
           boost::posix_time::time_duration td,
           nicta::quantity::x_absolute_displacement<unsigned int> x_pos,
           nicta::quantity::y_absolute_displacement<unsigned int> y_pos,
           nicta::quantity::width<unsigned int> width,
           nicta::quantity::height<unsigned int> height,
           nicta::vibe::image::WindowModel m
          ) const throw ()
{
    // Pop image in a window on screen, at pixel coordinates [offset from upper-left corner of screen], with image up-time of 'td' time-duration
    // ..., and resize (autosize iff width || height == 0)
    // NOTE:  if 'td' == 0; image remains up indefinitely until a keypress event is registered
    // NOTE:  we only move the window on first show!

    return this->timed_show(id, td, width, height, x_pos, y_pos, m);
}


template < typename T
         , template <typename T_> class U
         >
nicta::vibe::image::image<T, U> ::
image() throw ()
: image_(0)
, init_hdr_(true)
, tuple_(header_, image_)
, xroi_(std::make_pair<unsigned int, unsigned int> (0, 0))
, yroi_(std::make_pair<unsigned int, unsigned int> (0, 0))
{
    // Constructor (protected)
    // NOTE:  principally used for access to an img-header
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
