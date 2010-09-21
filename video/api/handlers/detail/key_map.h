/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License,  Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_XEVENT_DETAIL_KEY_MAP_H
#define NICTA_VIBE_VIDEO_XEVENT_DETAIL_KEY_MAP_H

#include <static_association/static_association.h>


namespace nicta {
namespace vibe {
namespace video {
namespace xevent {
namespace detail {

    // USER BEWARE:  multiple keypress detection comes at a greater cost of longer wait times on window to listen for the event!

    // USER BEWARE:  windows of *other* clients *may* _appear_ listen to events of other clients if system heavily loaded & switching b/w thread contexts

    // NOTE:  if we do (keyval & 255) we will not be able to detect multiple key-presses,  so we don't apply the mask


    nicta::association<const char*, int> keystroke_assocArray[] = { { "ANY_KEY"          ,  0x0      },
                                                                    { "SILENT_KEYS"      , -0x1      },
                                                                    { "ALT_L"            ,  0x10FFE9 },
                                                                    { "ALT_R"            ,  0x10FFEA },
                                                                    { "ALT+BACKSPC"      ,  0x18FF08 },
                                                                    { "ALT+END"          ,  0x18FF57 },
                                                                    { "ALT+ENTER"        ,  0x18000A },
                                                                    { "ALT+ESC"          ,  0x18001B },
                                                                    { "BACKSPC"          ,  0x10FF08 },
                                                                    { "BREAK"            ,  0x10FF13 },
                                                                    { "CTRL_L"           ,  0x10FFE3 },
                                                                    { "CTRL_R"           ,  0x10FFE4 },
                                                                    { "CTRL+BACKSPC"     ,  0x14FF08 },
                                                                    { "CTRL+C"           ,  0x140063 },
                                                                    { "CTRL+D"           ,  0x140064 },
                                                                    { "CTRL+END"         ,  0x14FF57 },
                                                                    { "CTRL+ENTER"       ,  0x14000A },
                                                                    { "CTRL+ESC"         ,  0x14001B },
                                                                    { "CTRL+TAB"         ,  0x140009 },
                                                                    { "CTRL+Q"           ,  0x140071 },
                                                                    { "CTRL+X"           ,  0x140078 },
                                                                    { "CTRL+Z"           ,  0x14007A },
                                                                    { "CTRL+ALT+BREAK"   ,  0x1CFF6B },
                                                                    { "CTRL+ALT+DELETE"  ,  0x1CFFFF },
                                                                    { "CTRL+ALT+END"     ,  0x1CFF57 },
                                                                    { "CTRL+ALT+PRNTSCR" ,  0x1CFF15 },
                                                                    { "CTRL+ESC+BREAK"   ,  0x14FF6B },
                                                                    { "CTRL+ESC+END"     ,  0x14FF57 },
                                                                    { "CTRL+SHIFT+ESC"   ,  0x15001B },
                                                                    { "DELETE"           ,  0x10FFFF },
                                                                    { "END"              ,  0x10FF57 },
                                                                    { "ENTER"            ,  0x10000A },
                                                                    { "ENTER_L"          ,  0x10000A },
                                                                    { "ENTER_R"          ,  0x10FF8D },
                                                                    { "ESC"              ,  0x10001B },
                                                                    { "HOME"             ,  0x10FF50 },
                                                                    { "INSERT"           ,  0x10FF63 },
                                                                    { "NUMERIC_ENTER"    ,  0x10FF8D },
                                                                    { "PAGE_DOWN"        ,  0x10FF56 },
                                                                    { "PAGE_UP"          ,  0x10FF55 },
                                                                    { "SCROLL_LOCK"      ,  0x10FF14 },
                                                                    { "SHIFT_L"          ,  0x10FFE1 },
                                                                    { "SHIFT_R"          ,  0x10FFE2 },
                                                                    { "SPACE"            ,  0x100020 },
                                                                    { "SHIFT+BACKSPC"    ,  0x11FF08 },
                                                                    { "SHIFT+ENTER"      ,  0x11000A },
                                                                    { "SHIFT+END"        ,  0x11FF57 },
                                                                    { "SHIFT+ESC"        ,  0x11001B },
                                                                    { "SHIFT+Z"          ,  0x11005A },
                                                                    { "SHIFT+ESC+BREAK"  ,  0x11FF13 },
                                                                    { "SHIFT+ESC+END"    ,  0x11001B },
                                                                    { "TAB"              ,  0x100009 } };


    nicta::static_association<const char*,  int> keystroke_assocMap
         (nicta::vibe::video::xevent::detail::keystroke_assocArray,
          sizeof(nicta::vibe::video::xevent::detail::keystroke_assocArray) / sizeof(nicta::vibe::video::xevent::detail::keystroke_assocArray[0]));

} } } } } // namespace nicta::vibe::video::xevent::detail


#endif





//////////////////////////////////////
// GET KEY MAPPINGS WITH CODE BELOW:
//////////////////////////////////////
//
// #include <iostream>
// #include <opencv/cv.h>
// #include <opencv/highgui.h>
//
// int main(int argc, char** argv)
// {
//     ::cvNamedWindow("win");
//     while (1)
//     {
//         int key =  ::cvWaitKey(0);
//         if (key != -1)
//         {
//             std::cout << std::endl << "KEYVAL = " << key << std::endl;
//         }
//     }
//     ::cvDestroyWindow("win");
//
//     return 0;
// }
//
//////////////////////////////////////

