/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_QUANTITY_H
#define NICTA_QUANTITY_H

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/utility/enable_if.hpp>

#include <iostream>
#include <stdexcept>


namespace nicta {
namespace quantity {

    template <typename T>
    class quantity_base
    {
    public:

        typedef T type;


        explicit quantity_base(T value) throw ()
        : quantity_(value)
        { }


        template <typename RetType = T>
        typename boost::enable_if< typename boost::is_convertible<T, RetType>::type
                                 , RetType
                                 >::type
        operator()() const throw ()
        {
            // Canonical get-functor

            return static_cast<RetType> (this->quantity_);
        }

        template <typename U>
        typename boost::enable_if< typename boost::is_convertible<U, T>::type
                                 , void
                                 >::type
        operator()(U value) throw ()
        {
            // Canonical set-functor

            this->quantity_ = static_cast<T> (value);
        }


        bool operator<(const quantity_base<T>& rhs) const throw ();
        bool operator>(const quantity_base<T>& rhs) const throw ();
        bool operator<=(const quantity_base<T>& rhs) const throw ();
        bool operator>=(const quantity_base<T>& rhs) const throw ();
        bool operator==(const quantity_base<T>& rhs) const throw ();
        bool operator!=(const quantity_base<T>& rhs) const throw ();


        friend
        typename boost::enable_if_c< boost::is_arithmetic<T>::value
                                   , std::ostream&
                                   >::type
        operator<<(std::ostream& stream, nicta::quantity::quantity_base<T>& qb) throw ()
        {
            // Ouptut Streamable Friend

            stream << qb.quantity_;
            return stream;
        }

        friend
        typename boost::enable_if_c< boost::is_arithmetic<T>::value
                                   , std::ostream&
                                   >::type
        operator<<(std::ostream& stream, const nicta::quantity::quantity_base<T>& qb) throw ()
        {
            // Ouptut Streamable Friend - const version

            stream << qb.quantity_;
            return stream;
        }


    private:

        T quantity_;
    };



    template < typename T
             , typename Enable = void
             >
    struct x_origin
    { };

    template <typename T>
    struct x_origin < T
                    , typename boost::enable_if<boost::is_arithmetic<T> >::type
                    > : public quantity_base<T>
    {
        x_origin(T x_coord = 0) throw ()
        : quantity_base<T> (x_coord)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct y_origin
    { };

    template <typename T>
    struct y_origin < T
                    , typename boost::enable_if<boost::is_arithmetic<T> >::type
                    > : public quantity_base<T>
    {
        y_origin(T y_coord = 0) throw ()
        : quantity_base<T> (y_coord)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct x_coordinate
    { };

    template <typename T>
    struct x_coordinate < T
                        , typename boost::enable_if<boost::is_arithmetic<T> >::type
                        > : public x_origin<T>
    {
        x_coordinate(T x_coord = 0) throw ()
        : x_origin<T> (x_coord)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct y_coordinate
    { };

    template <typename T>
    struct y_coordinate < T
                        , typename boost::enable_if<boost::is_arithmetic<T> >::type
                        > : public y_origin<T>
    {
        y_coordinate(T y_coord = 0) throw ()
        : y_origin<T> (y_coord)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct x_center
    { };

    template <typename T>
    struct x_center < T
                    , typename boost::enable_if<boost::is_integral<T> >::type
                    > : public quantity_base<T>
    {
        x_center(T x_coord = 0) throw ()
        : quantity_base<T> (x_coord)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct y_center
    { };

    template <typename T>
    struct y_center < T
                    , typename boost::enable_if<boost::is_integral<T> >::type
                    > : public quantity_base<T>
    {
        y_center(T y_coord = 0) throw ()
        : quantity_base<T> (y_coord)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct length
    { };

    template <typename T>
    struct length < T
                  , typename boost::enable_if<boost::is_arithmetic<T> >::type
                  > : public quantity_base<T>
    {
        length(T len = 0) throw ()
        : quantity_base<T> (len)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct width
    { };

    template <typename T>
    struct width < T
                 , typename boost::enable_if<boost::is_unsigned<T> >::type
                 > : public length<T>
    {
        width(T len = 0) throw ()
        : length<T> (len)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct height
    { };

    template <typename T>
    struct height < T
                  , typename boost::enable_if<boost::is_unsigned<T> >::type
                  > : public length<T>
    {
        height(T len = 0) throw ()
        : length<T> (len)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct pixel_width
    { };

    template <typename T>
    struct pixel_width < T
                       , typename boost::enable_if<boost::is_unsigned<T> >::type
                       > : public length<T>
    {
        pixel_width(T len = 1) throw ()
        : length<T> (len)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct pixel_height
    { };

    template <typename T>
    struct pixel_height < T
                        , typename boost::enable_if<boost::is_unsigned<T> >::type
                        > : public length<T>
    {
        pixel_height(T len = 1) throw ()
        : length<T> (len)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct x_pixel_coordinate
    { };

    template <typename T>
    struct x_pixel_coordinate < T
                              , typename boost::enable_if<boost::is_integral<T> >::type
                              > : public quantity_base<T>
    {
        x_pixel_coordinate(T x_pix = 0) throw ()
        : quantity_base<T> (x_pix)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct y_pixel_coordinate
    { };

    template <typename T>
    struct y_pixel_coordinate < T
                              , typename boost::enable_if<boost::is_integral<T> >::type
                              > : public quantity_base<T>
    {
        y_pixel_coordinate(T y_pix = 0) throw ()
        : quantity_base<T> (y_pix)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct pixel_component
    { };

    template <typename T>
    struct pixel_component < T
                           , typename boost::enable_if<boost::is_unsigned<T> >::type
                           > : public quantity_base<T>
    {
        pixel_component(T plane = 0) throw ()
        : quantity_base<T> (plane)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct pixel_intensity
    { };

    template <typename T>
    struct pixel_intensity < T
                           , typename boost::enable_if<boost::is_arithmetic<T> >::type
                           > : public quantity_base<T>
    {
        pixel_intensity(T value = 0) throw ()
        : quantity_base<T> (value)
        { }
    };




    template < typename T
             , typename Enable = void
             >
    struct x_relative_displacement
    { };

    template <typename T>
    struct x_relative_displacement < T
                                   , typename boost::enable_if<boost::is_signed<T> >::type
                                   > : public length<T>
    {
        x_relative_displacement(T disp = 0) throw ()
        : length<T> (disp)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct y_relative_displacement
    { };

    template <typename T>
    struct y_relative_displacement < T
                                   , typename boost::enable_if<boost::is_signed<T> >::type
                                   > : public length<T>
    {
        y_relative_displacement(T disp = 0) throw ()
        : length<T> (disp)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct x_absolute_displacement
    { };

    template <typename T>
    struct x_absolute_displacement < T
                                   , typename boost::enable_if<boost::is_unsigned<T> >::type
                                   > : public length<T>
    {
        x_absolute_displacement(T disp = 0) throw ()
        : length<T> (disp)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct y_absolute_displacement
    { };

    template <typename T>
    struct y_absolute_displacement < T
                                   , typename boost::enable_if<boost::is_unsigned<T> >::type
                                   > : public length<T>
    {
        y_absolute_displacement(T disp = 0) throw ()
        : length<T> (disp)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct count
    { };

    template <typename T>
    struct count < T
                 , typename boost::enable_if<boost::is_integral<T> >::type
                 > : public quantity_base<T>
    {
        count(T n = 0) throw ()
        : quantity_base<T> (n)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct period
    { };

    template <typename T>
    struct period < T
                  , typename boost::enable_if<boost::is_unsigned<T> >::type
                  > : public quantity_base<T>
    {
        period(T n = 0) throw ()
        : quantity_base<T> (n)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct step
    { };

    template <typename T>
    struct step < T
                , typename boost::enable_if<boost::is_integral<T> >::type
                > : public quantity_base<T>
    {
        step(T n = 1) throw ()
        : quantity_base<T> (n)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct increment
    { };

    template <typename T>
    struct increment < T
                     , typename boost::enable_if<boost::is_arithmetic<T> >::type
                     > : public quantity_base<T>
    {
        increment(T incr = 0) throw ()
        : quantity_base<T> (incr)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct ratio
    { };

    template <typename T>
    struct ratio < T
                 , typename boost::enable_if<boost::is_arithmetic<T> >::type
                 > : public quantity_base<T>
    {
        explicit ratio(T s) throw ()
        : quantity_base<T> (s)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct fraction
    { };

    template <typename T>
    struct fraction < T
                 , typename boost::enable_if<boost::is_floating_point<T> >::type
                 > : public quantity_base<T>
    {
        explicit fraction(T s) throw ()
        : quantity_base<T> (s)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct scale_factor
    { };

    template <typename T>
    struct scale_factor < T
                        , typename boost::enable_if<boost::is_arithmetic<T> >::type
                        > : public quantity_base<T>
    {
        explicit scale_factor(T s) throw ()
        : quantity_base<T> (s)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct x_order
    { };

    template <typename T>
    struct x_order < T
                   , typename boost::enable_if<boost::is_unsigned<T> >::type
                   > : public quantity_base<T>
    {
        x_order(T xo = 0) throw ()
        : quantity_base<T> (xo)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct y_order
    { };

    template <typename T>
    struct y_order < T
                   , typename boost::enable_if<boost::is_unsigned<T> >::type
                   > : public quantity_base<T>
    {
        y_order(T yo = 0) throw ()
        : quantity_base<T> (yo)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct order
    { };

    template <typename T>
    struct order < T
                 , typename boost::enable_if<boost::is_unsigned<T> >::type
                 > : public quantity_base<T>
    {
        order(T o) throw ()
        : quantity_base<T> (o)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct gradient
    { };

    template <typename T>
    struct gradient < T
                    , typename boost::enable_if<boost::is_arithmetic<T> >::type
                    > : public quantity_base<T>
    {
        gradient(T m = 1) throw ()
        : quantity_base<T> (m)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct exponent
    { };

    template <typename T>
    struct exponent < T
                    , typename boost::enable_if<boost::is_arithmetic<T> >::type
                    > : public quantity_base<T>
    {
        exponent(T e = 1) throw ()
        : quantity_base<T> (e)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct additive_constant
    { };

    template <typename T>
    struct additive_constant < T
                             , typename boost::enable_if<boost::is_arithmetic<T> >::type
                             > : public quantity_base<T>
    {
        additive_constant(T c = 0) throw ()
        : quantity_base<T> (c)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct lower_bound
    { };

    template <typename T>
    struct lower_bound < T
                       , typename boost::enable_if<boost::is_arithmetic<T> >::type
                       > : public quantity_base<T>
    {
        lower_bound(T lb = 0) throw ()
        : quantity_base<T> (lb)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct upper_bound
    { };

    template <typename T>
    struct upper_bound < T
                       , typename boost::enable_if<boost::is_arithmetic<T> >::type
                       > : public quantity_base<T>
    {
        upper_bound(T ub = 0) throw ()
        : quantity_base<T> (ub)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct lower_threshold
    { };

    template <typename T>
    struct lower_threshold < T
                           , typename boost::enable_if<boost::is_arithmetic<T> >::type
                           > : public quantity_base<T>
    {
        lower_threshold(T lt = 0) throw ()
        : quantity_base<T> (lt)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct upper_threshold
    { };

    template <typename T>
    struct upper_threshold < T
                           , typename boost::enable_if<boost::is_arithmetic<T> >::type
                           > : public quantity_base<T>
    {
        upper_threshold(T ut = 0) throw ()
        : quantity_base<T> (ut)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct threshold
    { };

    template <typename T>
    struct threshold < T
                     , typename boost::enable_if<boost::is_arithmetic<T> >::type
                     > : public quantity_base<T>
    {
        threshold(T t = 0) throw ()
        : quantity_base<T> (t)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct frame_rate_per_second
    { };

    template <typename T>
    struct frame_rate_per_second < T
                                 , typename boost::enable_if<boost::is_unsigned<T> >::type
                                 > : public quantity_base<T>
    {
        frame_rate_per_second(T fps = 0) throw ()
        : quantity_base<T> (fps)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct frame_buffer_capacity
    { };

    template <typename T>
    struct frame_buffer_capacity < T
                                 , typename boost::enable_if<boost::is_unsigned<T> >::type
                                 > : public quantity_base<T>
    {
        explicit frame_buffer_capacity(T capacity) throw ()
        : quantity_base<T> (capacity)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct signed_frame_buffer_capacity
    { };

    template <typename T>
    struct signed_frame_buffer_capacity < T
                                        , typename boost::enable_if<boost::is_integral<T> >::type
                                        > : public quantity_base<T>
    {
        explicit signed_frame_buffer_capacity(T capacity) throw ()
        : quantity_base<T> (capacity)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct frames
    { };

    template <typename T>
    struct frames < T
                  , typename boost::enable_if<boost::is_unsigned<T> >::type
                  > : public quantity_base<T>
    {
        explicit frames(T n) throw ()
        : quantity_base<T> (n)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct aperature_size
    { };

    template <typename T>
    struct aperature_size < T
                          , typename boost::enable_if<boost::is_unsigned<T> >::type
                          > : public quantity_base<T>
    {
        explicit aperature_size(T n) throw ()
        : quantity_base<T> (n)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct parameterization
    { };

    template <typename T>
    struct parameterization < T
                            , typename boost::enable_if<boost::is_arithmetic<T> >::type
                            > : public quantity_base<T>
    {
        explicit parameterization(T s) throw ()
        : quantity_base<T> (s)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct bandwidth
    { };

    template <typename T>
    struct bandwidth < T
                     , typename boost::enable_if<boost::is_arithmetic<T> >::type
                     > : public quantity_base<T>
    {
        bandwidth(T s = 1) throw ()
        : quantity_base<T> (s)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct std_deviations
    { };

    template <typename T>
    struct std_deviations < T
                         , typename boost::enable_if<boost::is_arithmetic<T> >::type
                         > : public quantity_base<T>
    {
        std_deviations(T s = 3) throw ()
        : quantity_base<T> (s)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct gamma_correction
    { };

    template <typename T>
    struct gamma_correction < T
                            , typename boost::enable_if<boost::is_floating_point<T> >::type
                            > : public quantity_base<T>
    {
        gamma_correction(T g = T(1)) throw (std::domain_error)
        : quantity_base<T> (g)
        {
            if (g < 0)
            {
                throw std::domain_error("gamma_correction undefined on the negative Reals");
            }
        }

        template <typename RetType = T>
        typename boost::enable_if< typename boost::is_convertible<T, RetType>::type
                                 , RetType
                                 >::type
        operator()() const throw ()
        {
            // Canonical get-functor

            return this->quantity_base<T>::template operator()<RetType>();
        }

        template <typename U>
        typename boost::enable_if< typename boost::is_convertible<U, T>::type
                                 , void
                                 >::type
        operator()(U value) throw (std::domain_error)
        {
            // Canonical set-functor

            if (value < 0)
            {
                throw std::domain_error("gamma_correction undefined on the negative Reals");
            }
            this->quantity_base<T>::operator()(value);
        }
    };



    template < typename T
             , typename Enable = void
             >
    struct gain_control
    { };

    template <typename T>
    struct gain_control < T
                        , typename boost::enable_if<boost::is_floating_point<T> >::type
                        > : public quantity_base<T>
    {
        gain_control(T a = T(1)) throw (std::domain_error)
        : quantity_base<T> (a)
        {
            if (a < 0)
            {
                throw std::domain_error("gain_control undefined on the negative Reals");
            }
        }

        template <typename RetType = T>
        typename boost::enable_if< typename boost::is_convertible<T, RetType>::type
                                 , RetType
                                 >::type
        operator()() const throw ()
        {
            // Canonical get-functor

            return this->quantity_base<T>::template operator()<RetType>();
        }

        template <typename U>
        typename boost::enable_if< typename boost::is_convertible<U, T>::type
                                 , void
                                 >::type
        operator()(U value) throw (std::domain_error)
        {
            // Canonical set-functor

            if (value < 0)
            {
                throw std::domain_error("gain_control undefined on the negative Reals");
            }
            this->quantity_base<T>::operator()(value);
        }
    };



    template < typename T
             , typename Enable = void
             >
    struct kernel_size
    { };

    template <typename T>
    struct kernel_size < T
                       , typename boost::enable_if<boost::is_unsigned<T> >::type
                       > : public quantity_base<T>
    {
        explicit kernel_size(T size) throw ()
        : quantity_base<T> (size)
        { }
    };



    template < typename T
             , typename Enable = void
             >
    struct display_source
    { };

    template <typename T>
    struct display_source < T
                          , typename boost::enable_if<boost::is_same<T, bool> >::type
                          > : public quantity_base<T>
    {
        explicit display_source(T disp) throw ()
        : quantity_base<T> (disp)
        { }
    };

} } // namespace nicta::quantity




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template <typename T>
inline
bool
nicta::quantity::quantity_base<T> ::
operator<(const nicta::quantity::quantity_base<T>& rhs) const throw ()
{
    // Logical Comparitor op<

    return this->quantity_ < rhs.quantity_;
}


template <typename T>
inline
bool
nicta::quantity::quantity_base<T> ::
operator>(const nicta::quantity::quantity_base<T>& rhs) const throw ()
{
    // Logical Comparitor op>

    return (rhs < *this);
}


template <typename T>
inline
bool
nicta::quantity::quantity_base<T> ::
operator<=(const nicta::quantity::quantity_base<T>& rhs) const throw ()
{
    // Logical Comparitor op<=

    return !(rhs < *this);
}


template <typename T>
inline
bool
nicta::quantity::quantity_base<T> ::
operator>=(const nicta::quantity::quantity_base<T>& rhs) const throw ()
{
    // Logical Comparitor op>=

    return !(*this < rhs);
}


template <typename T>
inline
bool
nicta::quantity::quantity_base<T> ::
operator==(const nicta::quantity::quantity_base<T>& rhs) const throw ()
{
    // Logical Comparitor ==

    return !(*this < rhs) && !(rhs < *this);
}


template <typename T>
inline
bool
nicta::quantity::quantity_base<T> ::
operator!=(const nicta::quantity::quantity_base<T>& rhs) const throw ()
{
    // Logical Comparitor !=

    return !(*this == rhs);
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
