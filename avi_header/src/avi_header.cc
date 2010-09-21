/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


// NOTE: class only reads information about the *first* stream *only* for:
//  data_type_ ; fourcc_codec_ ; scale_ ; rate_ ; start_time_ ; length_ ; sample_size_ !!

// Reference:  http://pvdtools.sourceforge.net/aviformat.txt
// Reference:  http://www.ymer.org/amir/2007/06/04/getting-the-number-of-frames-of-an-avi-video-file-for-opencv
// Reference:  http://msdn.microsoft.com/en-us/ms779631(VS.85).aspx ; then goto:  AVI RIFF File Reference -> AVIMAINHEADER
// NOTE:  OpenCV's property_id:  CV_CAP_PROP_FRAME_COUNT always returns 0 on Linux ... hence buggy



#include "avi_header/avi_header.h"

#include <nicta_common/nicta_exception.h>

#include <cmath>
#include <cstring>
#include <fstream>


nicta::vibe::header::avi_header :: 
avi_header(const char* filename) throw (nicta::nicta_exception)
: filename_(filename)
{
    // Constructor - Return AVI file header info

    std::ifstream f(filename, std::ios::in | std::ios::binary);
    if (f.fail())
    {
        throw nicta::nicta_exception("failed to open file", "nicta::vibe::avi_header");
    }


    char buffer[4];

    f.seekg(0x20, std::ios::beg);                       // 0x20
    f.read(buffer, 4);
    this->msec_frame_delay_ = this->numeric_(buffer);

    f.seekg(0x0, std::ios::cur);                        // 0x24
    f.read(buffer, 4);
    this->max_data_rate_ = this->numeric_(buffer);

    f.seekg(0x4, std::ios::cur);                        // 0x2C
    f.read(buffer, 4);
    this->interleaved_             = (this->numeric_(buffer) >>  8) & 1;
    this->optimized_video_capture_ = (this->numeric_(buffer) >> 16) & 1;
    this->copyrighted_data_        = (this->numeric_(buffer) >> 17) & 1;

    f.seekg(0x0, std::ios::cur);                        // 0x30
    f.read(buffer, 4);
    this->frame_count_ = this->numeric_(buffer);

    f.seekg(0x0, std::ios::cur);                        // 0x34
    f.read(buffer, 4);
    this->initial_frame_ = this->numeric_(buffer);

    f.seekg(0x0, std::ios::cur);                        // 0x38
    f.read(buffer, 4);
    this->stream_count_ = this->numeric_(buffer);

    f.seekg(0x0, std::ios::cur);                        // 0x3C
    f.read(buffer, 4);
    this->optimal_buffer_size_ = this->numeric_(buffer);

    f.seekg(0x0, std::ios::cur);                        // 0x40
    f.read(buffer, 4);
    this->frame_width_ = this->numeric_(buffer);

    f.seekg(0x0, std::ios::cur);                        // 0x44
    f.read(buffer, 4);
    this->frame_height_ = this->numeric_(buffer);

    f.seekg(0x24, std::ios::cur);                       // 0x6C
    f.read(buffer, 4);
    strcpy(this->data_type_, buffer);
    this->data_type_[4] = 0;

    f.seekg(0x0, std::ios::cur);                        // 0x70
    f.read(buffer, 4);
    strcpy(this->fourcc_codec_, buffer);
    this->fourcc_codec_[4] = 0;

    f.seekg(0xC, std::ios::cur);                        // 0x80
    f.read(buffer, 4);
    this->scale_ = this->numeric_(buffer);

    f.seekg(0x0, std::ios::cur);                        // 0x84
    f.read(buffer, 4);
    this->rate_ = this->numeric_(buffer);

    f.seekg(0x0, std::ios::cur);                        // 0x88
    f.read(buffer, 4);
    this->start_time_ = this->numeric_(buffer);

    f.seekg(0x0, std::ios::cur);                        // 0x8C
    f.read(buffer, 4);
    this->length_ = this->numeric_(buffer);

    f.seekg(0x8, std::ios::cur);                        // 0x98
    f.read(buffer, 4);
    this->sample_size_ = this->numeric_(buffer);

    f.close();
}


const char*
nicta::vibe::header::avi_header :: 
data_type() const throw (nicta::nicta_exception)
{
    // Data-type of the *FIRST* data-stream in file
    // Either of:  'auds' (audio) ; 'mids' (midi) ; 'txts' (text) ; 'vids' (video)
    // fccType @ 0x6C ;   http://pvdtools.sourceforge.net/aviformat.txt

    return this->data_type_;
}


const char*
nicta::vibe::header::avi_header :: 
fourcc_codec() const throw (nicta::nicta_exception)
{
    // Usually a four-letter code specifiying the the stream encoding, of the *FIRST* data-stream in file
    // NOTE:  *may* have trailing rubbish for some *rare* codec ids that are 2 or 3 characters long
    // Reference:  http://www.fourcc.org/codecs.php
    // fccHandler @ 0x70 ;   http://pvdtools.sourceforge.net/aviformat.txt

    return this->fourcc_codec_;
}


int
nicta::vibe::header::avi_header :: 
msec_frame_delay() const throw ()
{
    // Specifies the number of microseconds between frames -
    // Indicates the overall timing for the file
    // dwMicroSecPerFrame @ 0x20 ;   http://pvdtools.sourceforge.net/aviformat.txt

    return this->msec_frame_delay_;
}


int
nicta::vibe::header::avi_header :: 
max_data_rate() const throw ()
{
    // Specifies the approximate maximum data rate of the file -
    // Indicates the number of bytes per second the system must handle to present an AVI sequence
    // dwMaxBytesPerSec @ 0x24 ;   http://pvdtools.sourceforge.net/aviformat.txt

    return this->max_data_rate_;
}


int
nicta::vibe::header::avi_header :: 
frame_count() const throw ()
{
    // Total number of frames
    // dwTotalFrames @ 0x34 ;   http://pvdtools.sourceforge.net/aviformat.txt

    return this->frame_count_;
}


int
nicta::vibe::header::avi_header :: 
initial_frame() const throw ()
{
    // Specifies the initial frame for interleaved files (non-interleaved files should specify zero)
    // dwInitialFrames @ 0x34 ;   http://pvdtools.sourceforge.net/aviformat.txt

    return this->initial_frame_;
}


int
nicta::vibe::header::avi_header :: 
stream_count() const throw ()
{
    // Specifies the number of streams in the file
    // Eg:  a file with audio & video has 2 streams.
    // dwStreams @ 0x38

    return this->stream_count_;
}


int
nicta::vibe::header::avi_header :: 
optimal_buffer_size() const throw ()
{
    // Specifies the suggested buffer size for reading the file (in bytes)
    // Size will generally be large enough to contain the largest chunk in the file
    // For an interleaved file, the buffer size should be large enough to read an entire record, and not just a chunk
    // dwSuggestedBufferSize @ 0x3C ;   http://pvdtools.sourceforge.net/aviformat.txt

    return this->optimal_buffer_size_;
}


int
nicta::vibe::header::avi_header :: 
frame_width() const throw ()
{
    // Specifies the width of an AVI file (in pixels)
    // dwWidth @ 0x40 ;   http://pvdtools.sourceforge.net/aviformat.txt

    return this->frame_width_;
}


int
nicta::vibe::header::avi_header :: 
frame_height() const throw ()
{
    // Specifies the height of an AVI file (in pixels)
    // dwHeight @ 0x44 ;   http://pvdtools.sourceforge.net/aviformat.txt

    return this->frame_height_;
}


int
nicta::vibe::header::avi_header :: 
scale() const throw (nicta::nicta_exception)
{
    // Used with rate() below to specify the time scale that this stream will use, of the *FIRST* data-stream in file
    // NOTE:  divide  rate() / scale() to get Frames Per Second (FPS)
    // dwScale @ 0x80 ;   http://pvdtools.sourceforge.net/aviformat.txt

    if (this->stream_count_ != 1)
    {
        // Error because we only ever read info about the first stream in file ... (will prevent user errors this way)

        throw nicta::nicta_exception("Multi-stream AVI files not supported", "nicta::vibe::avi_header");
    }
    return this->scale_;
}


int
nicta::vibe::header::avi_header :: 
rate() const throw (nicta::nicta_exception)
{
    // Used with scale() above to specify the time scale that this stream will use, of the *FIRST* data-stream in file
    // Divide:  rate() / scale() to get Frames Per Second (FPS) [for a video stream]
    // dwRate @ 0x84 ;   http://pvdtools.sourceforge.net/aviformat.txt

    if (this->stream_count_ != 1)
    {
        // Error because we only ever read info about the first stream in file ... (will prevent user errors this way)

        throw nicta::nicta_exception("Multi-stream AVI files not supported", "nicta::vibe::avi_header");
    }
    return this->rate_;
}


int
nicta::vibe::header::avi_header :: 
start_time() const throw (nicta::nicta_exception)
{
    // Specifies the starting time of the *FIRST* data-stream in file, (usually zero)
    // dwStart @ 0x88 ;   http://pvdtools.sourceforge.net/aviformat.txt

    if (this->stream_count_ != 1)
    {
        // Error because we only ever read info about the first stream in file ... (will prevent user errors this way)

        throw nicta::nicta_exception("Multi-stream AVI files not supported", "nicta::vibe::avi_header");
    }
    return this->start_time_;
}


int
nicta::vibe::header::avi_header :: 
length() const throw (nicta::nicta_exception)
{
    // Length of the *FIRST* data-stream in file, [in units specified by rate() and scale()]
    // dwLength @ 0x8C ;   http://pvdtools.sourceforge.net/aviformat.txt

    if (this->stream_count_ != 1)
    {
        // Error because we only ever read info about the first stream in file ... (will prevent user errors this way)

        throw nicta::nicta_exception("Multi-stream AVI files not supported", "nicta::vibe::avi_header");
    }
    return this->length_;
}


int
nicta::vibe::header::avi_header :: 
sample_size() const throw (nicta::nicta_exception)
{
    // Sample size of the *FIRST* data-stream in file, (zero if it varies)
    // dwSampleSize @ 0x98 ;   http://pvdtools.sourceforge.net/aviformat.txt

    if (this->stream_count_ != 1)
    {
        // Error because we only ever read info about the first stream in file ... (will prevent user errors this way)

        throw nicta::nicta_exception("Multi-stream AVI files not supported", "nicta::vibe::avi_header");
    }
    return this->sample_size_;
}


bool
nicta::vibe::header::avi_header :: 
interleaved() const throw ()
{
    // Returns True if file is interleaved
    // dwFlags @ 0x2C ; bit 8 ;   http://pvdtools.sourceforge.net/aviformat.txt

    return this->interleaved_;
}


bool
nicta::vibe::header::avi_header :: 
optimized_video_capture() const throw ()
{
    // Returns True if file is optimized for video capture
    // dwFlags @ 0x2C ; bit 16 ;   http://pvdtools.sourceforge.net/aviformat.txt

    return this->optimized_video_capture_;
}


bool
nicta::vibe::header::avi_header :: 
copyrighted_data() const throw ()
{
    // Returns True if file contains copyrighted data
    // dwFlags @ 0x2C ; bit 17 ;   http://pvdtools.sourceforge.net/aviformat.txt

    return this->copyrighted_data_;
}


const char*
nicta::vibe::header::avi_header :: 
filename() const throw ()
{
    // Returns input filename that was queried

    return this->filename_;
}


int
nicta::vibe::header::avi_header :: 
numeric_(const char* buffer) const throw ()
{
    // Helper Function:  Decodes reads into numeric values

    return static_cast<int> (static_cast<unsigned char> (buffer[0]) +
                             static_cast<unsigned char> (buffer[1]) * 256 +
                             static_cast<unsigned char> (buffer[2]) * std::pow(256.0, 2.0) +
                             static_cast<unsigned char> (buffer[3]) * std::pow(256.0, 3.0));
}

