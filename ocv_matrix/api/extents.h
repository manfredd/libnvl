/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_EXTENTS_H
#define NICTA_OCV_MATRIX_EXTENTS_H

#include "ocv_matrix/macros/init_extents.h"

#include <opencv/cxtypes.h>

#include <boost/mpl/assert.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/utility/enable_if.hpp>

#include <algorithm>
#include <numeric>
#include <vector>
#include <stdexcept>


namespace nicta {
namespace ocv {
namespace matrix {

    template <unsigned int D>
    class extents
    {
    public:

        typedef int value_type; 
        typedef std::vector<int>::size_type size_type;

        extents() throw ()
        : data_(0)
        {
        }


#define OPERATOR(z, n, _)                                                         \
                                                                                  \
        BOOST_PP_IF(BOOST_PP_NOT(n), explicit, )                                  \
        extents(BOOST_PP_ENUM_PARAMS(BOOST_PP_INC(n), unsigned int e)) throw ()   \
        : data_(D)                                                                \
        {                                                                         \
            /* Used for matrix shaping */                                         \
                                                                                  \
            BOOST_STATIC_ASSERT(( D == BOOST_PP_INC(n) ));                        \
                                                                                  \
            BOOST_PP_REPEAT(BOOST_PP_INC(n), INIT_EXTENTS, e)                     \
        }
/**/

#define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
#define BOOST_PP_LOCAL_LIMITS (0, CV_MAX_DIM - 1)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
#undef OPERATOR

/* unload "macros/init_extents.h" */
#undef INIT_EXTENTS


        extents(unsigned int* data, unsigned int size) throw (std::length_error, std::bad_exception)
        : data_(D)
        {
            if (size != D)
            {
                throw std::length_error("extent size mismatch");
            }
            for (unsigned int i = 0; i != size; ++i)
            {
                this->data_[i] = data[i];
            }
        }

        template <typename T>
        explicit extents(std::vector<T>& e, typename boost::enable_if<boost::is_integral<T> >::type* dummy = 0) throw (std::length_error)
        : data_(D)
        {
            if (e.size() != D)
            {
                throw std::length_error("extent size mismatch");
            }
            for (unsigned int i = 0; i != e.size(); ++i)
            {
                this->data_[i] = static_cast<int> (e[i]);
            }
        }


        bool operator==(const extents<D>& rhs) const throw ()
        {
            std::vector<int>::const_iterator it = std::search(this->data_.begin(),
                                                              this->data_.end(),
                                                              rhs.data_.begin(),
                                                              rhs.data_.end());
            return it == this->data_.begin();
        }

        bool operator!=(const extents<D>& rhs) const throw ()
        {
            return !this->nicta::ocv::matrix::extents<D>::operator==(rhs);
        }

        extents<D>& operator=(unsigned int e) throw (std::length_error)
        {
            return this->extents<D>::operator,(e);
        }

        extents<D>& operator,(unsigned int e) throw (std::length_error)
        {
            if (this->data_.size() >= D)
            {
                throw std::length_error("extent size-limit exceeded");
            }
            this->data_.push_back(e);
            return *this;
        }

        unsigned int operator[](unsigned int i) const throw (std::out_of_range)
        {
            if (i >= this->data_.size())
            {
                throw std::out_of_range("extent access out of range");
            }
            return static_cast<unsigned int> (this->data_[i]);
        }


        typename nicta::ocv::matrix::extents<D>::value_type* operator()() throw (std::length_error)
        {
            if (this->data_.size() != D)
            {
                throw std::length_error("incomplete extent structure");
            }
            return this->data_.data();
        }


        const typename nicta::ocv::matrix::extents<D>::value_type* operator()() const throw (std::length_error)
        {
            if (this->data_.size() != D)
            {
                throw std::length_error("incomplete extent structure");
            }
            return this->data_.data();
        }

        typename nicta::ocv::matrix::extents<D>::size_type size() const throw ()
        {
            // Number of extents (should equal D **when** extent object has been populated)

            return this->data_.size();
        }

        unsigned int length() const throw ()
        {
            // Sum across all extents

            return std::accumulate(this->data_.begin(), this->data_.end(), 0U);
        }

        unsigned long int product() const throw ()
        {
            // Product across all orthogonal extents

            return std::accumulate(this->data_.begin(), this->data_.end(), 1UL, std::multiplies<unsigned long int>());
        }


    private:

        std::vector<int> data_;



        BOOST_STATIC_ASSERT (D > 0);
        BOOST_STATIC_ASSERT (D <= CV_MAX_DIM);
    };

} } } // namespace nicta::ocv::matrix


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
