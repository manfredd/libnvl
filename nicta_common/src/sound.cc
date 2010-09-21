/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "nicta_common/nicta_exception.h"
#include "nicta_common/sound.h"
#include "nicta_common/util.h"

#include <boost/assign/std/set.hpp>

#include <cstdlib>
#include <exception>
#include <set>
#include <string>
#include <vector>


void
nicta::sound :: 
play_sound_file(const char* file) throw (nicta::nicta_exception, std::bad_exception)
{
    // Plays any of a number of supported sound files through the system sound setup

    using namespace boost::assign;

    std::set<std::string> file_types;
    file_types += "8svx", "aiff", "alsa", "au", "avr", "cdr", "cvs", "dat", "gsm", "hcom", "maud", "mp3", "nul",
                  "ogg", "prc", "sf", "sph", "smp", "snd", "sndt", "txw", "vms", "voc", "vox", "wav", "wvp",
                  "ub", "sb", "uw", "sw", "ul", "al", "lu", "la", "sl";


    std::string ss(file);
    std::string::size_type pos = ss.find_last_of('.');
    if (pos != std::string::npos)
    {
        std::string extn = ss.substr(++pos, std::string::npos);
        if (file_types.find(extn) != file_types.end())
        {
            // file has supported extn
            std::string pstr("play " + ss + " >& /dev/null");
            int retval = ::system(pstr.c_str());
            if (retval == -1)
            {
                throw nicta::nicta_exception("command failure", "play_sound_file", "nicta::sound");
            }
        }
        else
        {
            throw nicta::nicta_exception("unsupported file extension", "play_sound_file", "nicta::sound");
        }
    }
    else
    {
        throw nicta::nicta_exception("could not query file type", "play_sound_file", "nicta::sound");
    }
}


void
nicta::sound :: 
tts(const char* arg) throw (nicta::nicta_exception, std::bad_exception)
{
    // Converts text-to-speech and plays it:  'arg' is either a text file, or string we want to speak

    std::string cmd("file ");
    cmd += std::string(arg);
    std::vector<std::string> cmd_out;
    nicta::util::command_capture(cmd.c_str(), cmd_out);

    std::string::size_type pos = cmd_out[0].rfind("ASCII text");
    std::string speak("flite ");
    if (pos != std::string::npos)
    {
        // it's a text-file
        speak += "-f ";
    }
    else
    {
        // it's a character string
        speak += "-t ";
    }
    speak += std::string(arg);
    int retval = ::system(speak.c_str());
    if (retval == -1)
    {
        throw nicta::nicta_exception("command failure", "tts", "nicta::sound");
    }
}
