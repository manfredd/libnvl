/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
//
// NOTE:  this version made available when color space info is only available at runtime
//
/////////////////////////////////////////////////////////////////////////////////////////////



#ifndef NICTA_VIBE_IMAGE_DETAIL_CONVERSION_ASSOCMAP_H
#define NICTA_VIBE_IMAGE_DETAIL_CONVERSION_ASSOCMAP_H

#include "static_association/static_association.h"

#include <opencv/cv.h>

#include <string>


namespace nicta {
namespace vibe {
namespace image {
namespace detail {

    // association<conversion_string_id, opencv_enum_id>
    static
    nicta::association<std::string, int> conversion_assocArray[] = { { std::string("RGB->GRAY")    , CV_RGB2GRAY    },
                                                                     { std::string("BGR->GRAY")    , CV_BGR2GRAY    },
                                                                     { std::string("GRAY->RGB")    , CV_GRAY2RGB    },
                                                                     { std::string("GRAY->BGR")    , CV_GRAY2BGR    },
                                                                     { std::string("RGBA->GRAY")   , CV_RGBA2GRAY   },
                                                                     { std::string("BGRA->GRAY")   , CV_BGRA2GRAY   },
                                                                     { std::string("GRAY->RGBA")   , CV_GRAY2RGBA   },
                                                                     { std::string("GRAY->BGRA")   , CV_GRAY2BGRA   },
                                                                     { std::string("RGB->XYZ")     , CV_RGB2XYZ     },
                                                                     { std::string("BGR->XYZ")     , CV_BGR2XYZ     },
                                                                     { std::string("XYZ->RGB")     , CV_XYZ2RGB     },
                                                                     { std::string("XYZ->BGR")     , CV_XYZ2BGR     },
                                                                     { std::string("RGB->YCrCb")   , CV_RGB2YCrCb   },
                                                                     { std::string("BGR->YCrCb")   , CV_BGR2YCrCb   },
                                                                     { std::string("YCrCb->RGB")   , CV_YCrCb2RGB   },
                                                                     { std::string("YCrCb->BGR")   , CV_YCrCb2BGR   },
                                                                     { std::string("RGB->HSV")     , CV_RGB2HSV     },
                                                                     { std::string("BGR->HSV")     , CV_BGR2HSV     },
                                                                     { std::string("HSV->RGB")     , CV_HSV2RGB     },
                                                                     { std::string("HSV->BGR")     , CV_HSV2BGR     },
                                                                     { std::string("RGB->HLS")     , CV_RGB2HLS     },
                                                                     { std::string("BGR->HLS")     , CV_BGR2HLS     },
                                                                     { std::string("HLS->RGB")     , CV_HLS2RGB     },
                                                                     { std::string("HLS->BGR")     , CV_HLS2BGR     },
                                                                     { std::string("RGB->Lab")     , CV_RGB2Lab     },
                                                                     { std::string("BGR->Lab")     , CV_BGR2Lab     },
                                                                     { std::string("Lab->RGB")     , CV_Lab2RGB     },
                                                                     { std::string("Lab->BGR")     , CV_Lab2BGR     },
                                                                     { std::string("RGB->Luv")     , CV_RGB2Luv     },
                                                                     { std::string("BGR->Luv")     , CV_BGR2Luv     },
                                                                     { std::string("Luv->RGB")     , CV_Luv2RGB     },
                                                                     { std::string("Luv->BGR")     , CV_Luv2BGR     },
                                                                     { std::string("BayerBG->RGB") , CV_BayerBG2RGB },
                                                                     { std::string("BayerGB->RBG") , CV_BayerGB2RGB },
                                                                     { std::string("BayerRG->RBG") , CV_BayerRG2RGB },
                                                                     { std::string("BayerGR->RBG") , CV_BayerGR2RGB },
                                                                     { std::string("BayerBG->BGR") , CV_BayerBG2BGR },
                                                                     { std::string("BayerGB->BGR") , CV_BayerGB2BGR },
                                                                     { std::string("BayerRG->BGR") , CV_BayerRG2BGR },
                                                                     { std::string("BayerGR->BGR") , CV_BayerGR2BGR } };


    static
    nicta::static_association<std::string, int> conversion_assocMap
         (nicta::vibe::image::detail::conversion_assocArray,
		  sizeof(nicta::vibe::image::detail::conversion_assocArray) / sizeof(nicta::vibe::image::detail::conversion_assocArray[0]));

} } } } // namespace nicta::vibe::image::detail


#endif
