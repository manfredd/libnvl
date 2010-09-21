/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_NATIVE_TYPE_H
#define NICTA_OCV_MATRIX_NATIVE_TYPE_H

#include <opencv/cxtypes.h>

#include <boost/mpl/at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>


namespace nicta {
namespace ocv {
namespace matrix {

    enum Native
    {
        Header  = 0 ,        // ::CvMat&  | ::CvMatND&
        Handle  = 1 ,        // ::CvMat*& | ::CvMatND*&
        Pointer = 2          // unsigned char*&
    };



    /* helper metafunctions */

    template < unsigned int U
             , typename nicta::ocv::matrix::Native N = nicta::ocv::matrix::Handle
             >
    struct native_type
    {
        typedef boost::mpl::map< boost::mpl::pair<boost::mpl::int_<nicta::ocv::matrix::Header>, ::CvMatND >
                               , boost::mpl::pair<boost::mpl::int_<nicta::ocv::matrix::Handle>, ::CvMatND *>
                               , boost::mpl::pair<boost::mpl::int_<nicta::ocv::matrix::Pointer>, unsigned char *>
                               > n_type;

        typedef typename boost::mpl::at<typename nicta::ocv::matrix::native_type<U, N>::n_type, boost::mpl::int_<N> >::type type;
    };

    template <typename nicta::ocv::matrix::Native N>
    struct native_type <2, N>
    {
        typedef boost::mpl::map< boost::mpl::pair<boost::mpl::int_<nicta::ocv::matrix::Header>, ::CvMat >
                               , boost::mpl::pair<boost::mpl::int_<nicta::ocv::matrix::Handle>, ::CvMat *>
                               , boost::mpl::pair<boost::mpl::int_<nicta::ocv::matrix::Pointer>, unsigned char *>
                               > n_type;

        typedef typename boost::mpl::at<typename nicta::ocv::matrix::native_type<2, N>::n_type, boost::mpl::int_<N> >::type type;
    };

    template <typename nicta::ocv::matrix::Native N>
    struct native_type <1, N>
    {
        typedef boost::mpl::map< boost::mpl::pair<boost::mpl::int_<nicta::ocv::matrix::Header>, ::CvMat >
                               , boost::mpl::pair<boost::mpl::int_<nicta::ocv::matrix::Handle>, ::CvMat *>
                               , boost::mpl::pair<boost::mpl::int_<nicta::ocv::matrix::Pointer>, unsigned char *>
                               > n_type;

        typedef typename boost::mpl::at<typename nicta::ocv::matrix::native_type<1, N>::n_type, boost::mpl::int_<N> >::type type;
    };

} } } // namespace nicta::ocv::matrix


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
