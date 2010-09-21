/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_STATIC_ASSOCIATION_H
#define NICTA_STATIC_ASSOCIATION_H


#include "nicta_common/nicta_exception.h"

#include <map>

namespace nicta {

    template < typename key_type
             , typename value_type
             >
    struct association
    {
        key_type key;
        value_type value;
    };


    template < typename key_type
             , typename value_type
             >
    class static_association : private std::map<key_type, value_type>    // An associative object which can't be modified during its lifetime
    {
    public:

        static_association(const association<key_type, value_type>* array, size_t array_size) throw()
        {
            // Constructor

            for (size_t i = 0; i != array_size; ++i)
            {
                this->std::map<key_type, value_type>::insert(std::make_pair(array[i].key, array[i].value));
            }
        }


        bool has_key(key_type key) throw()
        {
            return this->std::map<key_type, value_type>::find(key) != this->std::map<key_type, value_type>::end();
        }


        value_type operator()(key_type key) throw (nicta::nicta_exception)
        {
            // Return the value associated with the key

            typename std::map<key_type, value_type>::iterator it = this->std::map<key_type, value_type>::find(key);
            if (it != this->std::map<key_type, value_type>::end())
            {
                return it->second;
            }

            throw nicta::nicta_exception("No known map key", "get", "static_association");
        }


        size_t size() throw ()
        {
            // Return the number of associative pairs

            return this->std::map<key_type, value_type>::size();
        }
    };

} // namespace nicta


#endif

