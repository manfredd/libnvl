/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_VIBE_VIDEO_VENDOR_PGR_H
#define NICTA_VIBE_VIDEO_VENDOR_PGR_H

# ifdef NICTA_PGR


#include <flycapture/Camera.h>
#include <flycapture/Error.h>
#include <flycapture/FlyCapture2Defs.h>
#include <flycapture/Image.h>
#include <flycapture/Utilities.h>

#include <nicta_common/nicta_exception.h>
#include <static_association/static_association.h>

#include <iostream>
#include <string>


namespace nicta {
namespace vibe {
namespace video {
namespace pgr {

    class pgr
    {
    public:

        static
        void
        create_camera_capture(FlyCapture2::Camera& flycapture,
                              FlyCapture2::PGRGuid& guid
                             ) throw (nicta::nicta_exception)
        {
            // Ready camera for capture


            //Debug meta-data
            //nicta::vibe::video::pgr::pgr::dump_buildinfo();


            FlyCapture2::Error fe = flycapture.Connect(&guid);
            nicta::vibe::video::pgr::pgr::check_pgr(fe, "Connect");


            //Debug meta-data
            //nicta::vibe::video::pgr::pgr::dump_caminfo(flycapture);


            // Query for Format7 support
            bool supported = false;
            FlyCapture2::Format7Info f7_info;
            f7_info.mode = FlyCapture2::MODE_3;
            fe = flycapture.GetFormat7Info(&f7_info, &supported);
            nicta::vibe::video::pgr::pgr::check_pgr(fe, "GetFormat7Info");
            if (!supported)
            {
                // No Format_7, MODE_3 support for this camera..

                // throw nicta::nicta_exception("PGR Format_7, MODE_3 not supported");
            }
            else
            {
                // Camera has Format_7, MODE_3 support..

                //Debug meta-data
                //nicta::vibe::video::pgr::pgr::dump_f7info(f7_info);


                // Set-up Format7 meta-data
                FlyCapture2::Format7ImageSettings f7_settings;
                f7_settings.mode    = FlyCapture2::MODE_3;
                f7_settings.offsetX = 0;
                f7_settings.offsetY = 0;
                f7_settings.width   = f7_info.maxWidth;
                f7_settings.height  = f7_info.maxHeight;

                FlyCapture2::CameraInfo cam_info;
                fe = flycapture.GetCameraInfo(&cam_info);
                nicta::vibe::video::pgr::pgr::check_pgr(fe, "GetCameraInfo");

                if (cam_info.isColorCamera)
                {
                    f7_settings.pixelFormat = FlyCapture2::PIXEL_FORMAT_RAW16;
                }
                else
                {
                    f7_settings.pixelFormat = FlyCapture2::PIXEL_FORMAT_MONO16;
                }

                // Validate Format7 settings
                bool valid = false;
                FlyCapture2::Format7PacketInfo f7_pinfo;
                fe = flycapture.ValidateFormat7Settings(&f7_settings, &valid, &f7_pinfo);
                nicta::vibe::video::pgr::pgr::check_pgr(fe, "ValidateFormat7Settings");
                if (!valid)
                {
                    throw nicta::nicta_exception("PGR Format_7 settings invalid");
                }

                // Load camera with Format7 settings
                fe = flycapture.SetFormat7Configuration(&f7_settings, f7_pinfo.recommendedBytesPerPacket);     // 4096
                //fe = flycapture.SetFormat7Configuration(&f7_settings, f7_pinfo.maxBytesPerPacket);           // 4096
                //fe = flycapture.SetFormat7Configuration(&f7_settings, f7_pinfo.unitBytesPerPacket);          //  128
                nicta::vibe::video::pgr::pgr::check_pgr(fe, "SetFormat7Configuration");
            }


            fe = flycapture.StartCapture();
            nicta::vibe::video::pgr::pgr::check_pgr(fe, "StartCapture");
        }


        static
        bool
        query_frame(FlyCapture2::Camera& flycapture,
                    FlyCapture2::Image& image
                   ) throw (nicta::nicta_exception)
        {
            // Pull image off PGR camera buffer

            FlyCapture2::Error fe = flycapture.RetrieveBuffer(&image);
            return fe == FlyCapture2::PGRERROR_OK;
        }


        static
        double
        query_frame_rate(FlyCapture2::Camera& flycapture) throw (nicta::nicta_exception)
        {
            // Query camera frame rate

            FlyCapture2::Property property;
            property.type = FlyCapture2::FRAME_RATE;
            FlyCapture2::Error fe = flycapture.GetProperty(&property);
            nicta::vibe::video::pgr::pgr::check_pgr(fe, "GetProperty");
            return static_cast<double> (property.absValue);
        }


        static
        unsigned int
        query_register(FlyCapture2::Camera& flycapture,
                       unsigned int address
                      ) throw (nicta::nicta_exception)
        {
            // Query camera register - (address is hexadecimal)

            unsigned int value = 0;
            FlyCapture2::Error fe = flycapture.ReadRegister(address, &value);
            nicta::vibe::video::pgr::pgr::check_pgr(fe, "ReadRegister");
            return value;
        }


        static
        void
        check_pgr(FlyCapture2::Error& e, const char* s) throw (nicta::nicta_exception)
        {
            // Check for, and report PGR errors

            if (e != FlyCapture2::PGRERROR_OK)
            {
                throw nicta::nicta_exception(std::string(nicta::vibe::video::pgr::pgr::pgr_errorMap(e.GetType())), s, "FlyCapture2");
            }
        }


        static
        void
        dump_buildinfo() throw ()
        {
            // Dump library build info

            FlyCapture2::FC2Version f2v;
            FlyCapture2::Utilities::GetLibraryVersion(&f2v);

            std::cout << std::endl << "FlyCapture2 library version:  " << f2v.major << "." << f2v.minor << "." << f2v.type << "." << f2v.build
                      << std::endl << std::endl;
        }


        static
        void
        dump_caminfo(FlyCapture2::Camera& flycapture) throw (nicta::nicta_exception)
        {
            // Dump camera meta-data

            FlyCapture2::CameraInfo c_info;
            FlyCapture2::Error fe = flycapture.GetCameraInfo(&c_info);
            nicta::vibe::video::pgr::pgr::check_pgr(fe, "GetCameraInfo");

            std::cout << std::endl << "Serial Number:  " << c_info.serialNumber
                      << std::endl << "Camera Model :  " << c_info.modelName
                      << std::endl << "Camera Vendor:  " << c_info.vendorName
                      << std::endl << "Sensor       :  " << c_info.sensorInfo
                      << std::endl << "Resolution   :  " << c_info.sensorResolution
                      << std::endl << "Firmware Ver :  " << c_info.firmwareVersion
                      << std::endl << "Firmware Bld :  " << c_info.firmwareBuildTime
                      << std::endl << std::endl;
        }


        static
        void
        dump_f7info(FlyCapture2::Format7Info& f7_info) throw (nicta::nicta_exception)
        {
            // Dump format7 camera capabilties

            std::cout << std::endl << "Max image pixels (width, height)  :  " << f7_info.maxWidth        << " , " << f7_info.maxHeight
                      << std::endl << "Image unit size  (horiz, vertical):  " << f7_info.imageHStepSize  << " , " << f7_info.imageVStepSize
                      << std::endl << "Offset unit size (horiz, vertical):  " << f7_info.offsetHStepSize << " , " << f7_info.offsetVStepSize
                      << std::endl;
            fprintf(stdout,           "Pix fmt bitfield                  :  0x%08x\n\n", f7_info.pixelFormatBitField);
        }


    private:

        static
        nicta::association<int, const char *> pgr_errorArr[];

        static
        nicta::static_association<int, const char *> pgr_errorMap;
    };

} } } } // namespace nicta::vibe::video::pgr


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


/* Initialize static data members */

nicta::association<int, const char *>
nicta::vibe::video::pgr::pgr :: pgr_errorArr[] =
       { { static_cast<int> (FlyCapture2::PGRERROR_UNDEFINED)                    , "PGRERROR_UNDEFINED - Undefined"                                                 },
         { static_cast<int> (FlyCapture2::PGRERROR_OK)                           , "PGRERROR_OK - Function returned with no errors"                                 },
         { static_cast<int> (FlyCapture2::PGRERROR_FAILED)                       , "PGRERROR_FAILED - General failure"                                              },
         { static_cast<int> (FlyCapture2::PGRERROR_NOT_IMPLEMENTED)              , "PGRERROR_NOT_IMPLEMENTED - Function has not been implemented"                   },
         { static_cast<int> (FlyCapture2::PGRERROR_FAILED_BUS_MASTER_CONNECTION) , "PGRERROR_FAILED_BUS_MASTER_CONNECTION - Could not connect to Bus Master"        },
         { static_cast<int> (FlyCapture2::PGRERROR_NOT_CONNECTED)                , "PGRERROR_NOT_CONNECTED - Camera has not been connected"                         },
         { static_cast<int> (FlyCapture2::PGRERROR_INIT_FAILED)                  , "PGRERROR_INIT_FAILED - Initialization fail"                                     },
         { static_cast<int> (FlyCapture2::PGRERROR_NOT_INTITIALIZED)             , "PGRERROR_NOT_INTITIALIZED - Camera has not been initialized"                    },
         { static_cast<int> (FlyCapture2::PGRERROR_INVALID_PARAMETER)            , "PGRERROR_INVALID_PARAMETER - Invalid parameter passed to function"              },
         { static_cast<int> (FlyCapture2::PGRERROR_INVALID_SETTINGS)             , "PGRERROR_INVALID_SETTINGS - Setting set to camera is invalid"                   },
         { static_cast<int> (FlyCapture2::PGRERROR_INVALID_BUS_MANAGER)          , "PGRERROR_INVALID_BUS_MANAGER - Invalid Bus Manager object"                      },
         { static_cast<int> (FlyCapture2::PGRERROR_MEMORY_ALLOCATION_FAILED)     , "PGRERROR_MEMORY_ALLOCATION_FAILED - Could not allocate memory"                  },
         { static_cast<int> (FlyCapture2::PGRERROR_LOW_LEVEL_FAILURE)            , "PGRERROR_LOW_LEVEL_FAILURE - Low level error"                                   },
         { static_cast<int> (FlyCapture2::PGRERROR_NOT_FOUND)                    , "PGRERROR_NOT_FOUND - Device not found"                                          },
         { static_cast<int> (FlyCapture2::PGRERROR_FAILED_GUID)                  , "PGRERROR_FAILED_GUID - GUID failure"                                            },
         { static_cast<int> (FlyCapture2::PGRERROR_INVALID_PACKET_SIZE)          , "PGRERROR_INVALID_PACKET_SIZE - Packet size set to camera is invalid"            },
         { static_cast<int> (FlyCapture2::PGRERROR_INVALID_MODE)                 , "PGRERROR_INVALID_MODE - Invalid mode has been passed to function"               },
         { static_cast<int> (FlyCapture2::PGRERROR_NOT_IN_FORMAT7)               , "PGRERROR_NOT_IN_FORMAT7 - Error due to not being in Format7"                    },
         { static_cast<int> (FlyCapture2::PGRERROR_NOT_SUPPORTED)                , "PGRERROR_NOT_SUPPORTED - This feature is unsupporte"                            },
         { static_cast<int> (FlyCapture2::PGRERROR_TIMEOUT)                      , "PGRERROR_TIMEOUT - Timeout error"                                               },
         { static_cast<int> (FlyCapture2::PGRERROR_BUS_MASTER_FAILED)            , "PGRERROR_BUS_MASTER_FAILED - Bus Master Failure"                                },
         { static_cast<int> (FlyCapture2::PGRERROR_INVALID_GENERATION)           , "PGRERROR_INVALID_GENERATION - Generation Count Mismatch"                        },
         { static_cast<int> (FlyCapture2::PGRERROR_LUT_FAILED)                   , "PGRERROR_LUT_FAILED - Look Up Table failure"                                    },
         { static_cast<int> (FlyCapture2::PGRERROR_IIDC_FAILED)                  , "PGRERROR_IIDC_FAILED - IIDC failure"                                            },
         { static_cast<int> (FlyCapture2::PGRERROR_STROBE_FAILED)                , "PGRERROR_STROBE_FAILED - Strobe failure"                                        },
         { static_cast<int> (FlyCapture2::PGRERROR_TRIGGER_FAILED)               , "PGRERROR_TRIGGER_FAILED - Trigger failure"                                      },
         { static_cast<int> (FlyCapture2::PGRERROR_PROPERTY_FAILED)              , "PGRERROR_PROPERTY_FAILED - Property failure"                                    },
         { static_cast<int> (FlyCapture2::PGRERROR_PROPERTY_NOT_PRESENT)         , "PGRERROR_PROPERTY_NOT_PRESENT - Property is not present"                        },
         { static_cast<int> (FlyCapture2::PGRERROR_REGISTER_FAILED)              , "PGRERROR_REGISTER_FAILED - Register access failed"                              },
         { static_cast<int> (FlyCapture2::PGRERROR_READ_REGISTER_FAILED)         , "PGRERROR_READ_REGISTER_FAILED - Register read failed"                           },
         { static_cast<int> (FlyCapture2::PGRERROR_WRITE_REGISTER_FAILED)        , "PGRERROR_WRITE_REGISTER_FAILED - Register write failed"                         },
         { static_cast<int> (FlyCapture2::PGRERROR_ISOCH_FAILED)                 , "PGRERROR_ISOCH_FAILED - Isochronous failure"                                    },
         { static_cast<int> (FlyCapture2::PGRERROR_ISOCH_ALREADY_STARTED)        , "PGRERROR_ISOCH_ALREADY_STARTED - Isochronous transfer has already been started" },
         { static_cast<int> (FlyCapture2::PGRERROR_ISOCH_NOT_STARTED)            , "PGRERROR_ISOCH_NOT_STARTED - Isochronous transfer has not been started"         },
         { static_cast<int> (FlyCapture2::PGRERROR_ISOCH_START_FAILED)           , "PGRERROR_ISOCH_START_FAILED - Isochronous start failed"                         },
         { static_cast<int> (FlyCapture2::PGRERROR_ISOCH_RETRIEVE_BUFFER_FAILED) , "PGRERROR_ISOCH_RETRIEVE_BUFFER_FAILED - Isochronous retrieve buffer failed"     },
         { static_cast<int> (FlyCapture2::PGRERROR_ISOCH_STOP_FAILED)            , "PGRERROR_ISOCH_STOP_FAILED - Isochronous stop failed"                           },
         { static_cast<int> (FlyCapture2::PGRERROR_ISOCH_SYNC_FAILED)            , "PGRERROR_ISOCH_SYNC_FAILED - Isochronous image synchronization failed"          },
         { static_cast<int> (FlyCapture2::PGRERROR_ISOCH_BANDWIDTH_EXCEEDED)     , "PGRERROR_ISOCH_BANDWIDTH_EXCEEDED - Isochronous bandwidth exceeded"             },
         { static_cast<int> (FlyCapture2::PGRERROR_IMAGE_CONVERSION_FAILED)      , "PGRERROR_IMAGE_CONVERSION_FAILED - Image conversion failed"                     },
         { static_cast<int> (FlyCapture2::PGRERROR_IMAGE_LIBRARY_FAILURE)        , "PGRERROR_IMAGE_LIBRARY_FAILURE - Image library failure"                         },
         { static_cast<int> (FlyCapture2::PGRERROR_BUFFER_TOO_SMALL)             , "PGRERROR_BUFFER_TOO_SMALL - Buffer is too small"                                },
         { static_cast<int> (FlyCapture2::PGRERROR_FORCE_32BITS)                 , "FULL_32BIT_VALUE"                                                               } };


nicta::static_association<int, const char *>
nicta::vibe::video::pgr::pgr :: pgr_errorMap
             (nicta::vibe::video::pgr::pgr::pgr_errorArr,
              sizeof(nicta::vibe::video::pgr::pgr::pgr_errorArr) / sizeof(nicta::vibe::video::pgr::pgr::pgr_errorArr[0]));


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


# endif  /* NICTA_PGR */

#endif
