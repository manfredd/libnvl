/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_DETAIL_EXPONENTIAL_H
#define NICTA_OCV_MATRIX_DETAIL_EXPONENTIAL_H

#include <cmath>
#include <stdexcept>


namespace nicta {
namespace ocv {
namespace matrix {
namespace detail {

    struct exponential
    {
        template <typename T>
        void operator()(T& operand) const throw ()
        {
            // modifies in-place

            operand = static_cast<T> (std::exp(static_cast<double> (operand)));
        }

        template <typename T>
        void operator()(T& lhs, const T& rhs) const throw ()
        {
            lhs = static_cast<T> (std::exp(static_cast<double> (rhs)));
        }
    };

} } } } // namespace nicta::ocv::matrix::detail



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
