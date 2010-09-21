/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "sound_test.h"
#include "nicta_common/sound.h"

CPPUNIT_TEST_SUITE_REGISTRATION(SoundTest);


using namespace nicta;


void SoundTest :: setUp()
{
}


void SoundTest :: tearDown()
{
}


void SoundTest :: playSoundFile()
{
    sound::play_sound_file("sound.wav");
}


void SoundTest :: Tts()
{
    sound::tts("sound.txt");
    sound::tts("'Type your text here.'");
    sound::tts("\"No! do not type your text here.\"");
}

