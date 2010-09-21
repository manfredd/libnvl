/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_HEADER_IMAGE_HEADER_H
#define NICTA_VIBE_HEADER_IMAGE_HEADER_H

#include <nicta_common/nicta_exception.h>


namespace nicta {
namespace vibe {
namespace header {

    class image_header
    {
    public:

        explicit image_header(const char* filename) throw (nicta::nicta_exception);

        int depth() const throw ();
        int width() const throw ();
        int height() const throw ();
        bool is_color() const throw ();
        bool is_integral() const throw ();

        const char* filename() const throw ();


    private:

        const char* filename_;

        int depth_;              // color bit depth (usu 8, and for known img-types - unsigned => 8u)
        int width_;              // image width
        int height_;             // image height
        bool is_color_;          // is color image
        bool is_integral_;       // is image saved as floating_point or integer type
    };

} } } // namespace nicta::vibe::header

#endif
