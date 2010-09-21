/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_DETAIL_RECIPROCAL_H
#define NICTA_OCV_MATRIX_DETAIL_RECIPROCAL_H

#include <cmath>
#include <stdexcept>


namespace nicta {
namespace ocv {
namespace matrix {
namespace detail {

    class reciprocal
    {
    public:

        explicit reciprocal(double multiplier = 1.0) throw ()
        : m_(multiplier)
        {
        }

        template <typename T>
        void operator()(T& operand) const throw (std::domain_error)
        {
            // modifies in-place

            if (!operand)
            {
                throw std::domain_error("division by zero");
            }
            operand = static_cast<T> (this->m_ / static_cast<double> (operand));
        }

        template <typename T>
        void operator()(T& lhs, const T& rhs) const throw (std::domain_error)
        {
            if (!rhs)
            {
                throw std::domain_error("division by zero");
            }
            lhs = static_cast<T> (this->m_ / static_cast<double> (rhs));
        }


    private:

        double m_;
    };

} } } } // namespace nicta::ocv::matrix::detail



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
