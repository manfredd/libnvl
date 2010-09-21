/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_DETAIL_POWER_H
#define NICTA_OCV_MATRIX_DETAIL_POWER_H

#include <cmath>
#include <stdexcept>


namespace nicta {
namespace ocv {
namespace matrix {
namespace detail {

    class power
    {
    public:

        explicit power(double exponent) throw ()
        : e_(exponent)
        {
        }

        template <typename T>
        void operator()(T& base) const throw (std::domain_error)
        {
            // modifies in place

            if (!base && !this->e_)
            {
                throw std::domain_error("pow(0, 0) undefined");
            }
            if (base < 0 && std::fmod(this->e_, 1.0))
            {
                throw std::domain_error("pow(-ve, non-integer) is complex valued");
            }
            base = static_cast<T> (std::pow(static_cast<double> (base), this->e_));
        }

        template <typename T>
        void operator()(T& lhs, const T& rhs) const throw (std::domain_error)
        {
            if (!rhs && !this->e_)
            {
                throw std::domain_error("pow(0, 0) undefined");
            }
            if (rhs < 0 && std::fmod(this->e_, 1.0))
            {
                throw std::domain_error("pow(-ve, non-integer) is complex valued");
            }
            lhs = static_cast<T> (std::pow(static_cast<double> (rhs), this->e_));
        }


    private:

        double e_;
    };

} } } } // namespace nicta::ocv::matrix::detail



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
