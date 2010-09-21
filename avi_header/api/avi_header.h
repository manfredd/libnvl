/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_HEADER_AVI_HEADER_H
#define NICTA_VIBE_HEADER_AVI_HEADER_H

#include <nicta_common/nicta_exception.h>


namespace nicta {
namespace vibe {
namespace header {

    // NOTE: class only reads information about the *first* stream *only* for:
    //  data_type ; fourcc_codec ; scale ; rate ; start_time ; length ; sample_size !!

    // Reference:  http://pvdtools.sourceforge.net/aviformat.txt
    // Reference:  http://www.ymer.org/amir/2007/06/04/getting-the-number-of-frames-of-an-avi-video-file-for-opencv
    // Reference:  http://msdn.microsoft.com/en-us/ms779631(VS.85).aspx ; then goto:  AVI RIFF File Reference -> AVIMAINHEADER
    // NOTE:  OpenCV's property_id:  CV_CAP_PROP_FRAME_COUNT always returns 0 on Linux ... hence buggy

    class avi_header
    {
    public:

        explicit avi_header(const char* filename) throw (nicta::nicta_exception);

        const char* data_type() const throw (nicta::nicta_exception);
        const char* fourcc_codec() const throw (nicta::nicta_exception);

        int msec_frame_delay() const throw ();
        int max_data_rate() const throw ();
        int frame_count() const throw ();
        int initial_frame() const throw ();
        int stream_count() const throw ();
        int optimal_buffer_size() const throw ();
        int frame_width() const throw ();
        int frame_height() const throw ();

        int scale() const throw (nicta::nicta_exception);
        int rate() const throw (nicta::nicta_exception);
        int start_time() const throw (nicta::nicta_exception);
        int length() const throw (nicta::nicta_exception);
        int sample_size() const throw (nicta::nicta_exception);

        bool interleaved() const throw ();
        bool optimized_video_capture() const throw ();
        bool copyrighted_data() const throw ();

        const char* filename() const throw ();


    private:

        int numeric_(const char* buffer) const throw ();


        const char* filename_;

        int msec_frame_delay_;                      // number of microseconds between frames
        int max_data_rate_;                         // approximated data rate of the file - nbr of bytes/sec system must be able to handle
        bool interleaved_;                          // specifies if file is interleaved
        bool optimized_video_capture_;              // specifies if file is optimized for video capture
        bool copyrighted_data_;                     // specifies if file contains copyrighted data 
        int frame_count_;                           // specifies number of frames in file
        int initial_frame_;                         // specifies initial frame for interleaved file
        int stream_count_;                          // count of streams present in AVI file (eg. video & sound = 2 streams)
        int optimal_buffer_size_;                   // suggest buffer size for reading file chunks (or entire record if interleaved)
        int frame_width_;                           // frame width in pixels
        int frame_height_;                          // frame height in pixels
        char data_type_[5];                         // type of data for this stream segment:  'auds' (audio), 'mids' (midi), 'txts' (text), 'vids' (video)
        char fourcc_codec_[5];                      // FourCC codec, see:  www.fourcc.org
        int scale_;                                 // used with rate_ to specifiy the time scale this stream will use; FPS = rate/scale
        int rate_;                                  // see scale_
        int start_time_;                            // specifies starting time for the stream (usu 0)
        int length_;                                // length of stream in units specified by rate_ & scale_
        int sample_size_;                           // sample size, (=0 if varies)
    };

} } } // namespace nicta::vibe::header

#endif
