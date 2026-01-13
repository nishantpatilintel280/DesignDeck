//-----------------------------------------------------------------------------------------------------------
//
// INTEL CONFIDENTIAL
// Copyright (C) 2000-2024 Intel Corporation
// This software and the related documents are Intel copyrighted materials,
// and your use of them is governed by the express license under which they were provided to you ("License").
// Unless the License provides otherwise, you may not use, modify, copy, publish,
// distribute, disclose or transmit this software or the related documents without
// Intel's prior written permission. This software and the related documents are provided
// as is, with no express or implied warranties, other than those that are expressly stated in the License.
//
//-----------------------------------------------------------------------------------------------------------

/**
 *
 * @file  DisplayIDStructs.h
 * @brief This file contains all data structures used from Display ID spec.
 *
 */

#pragma once

 //-----------------------------------------------------------------------------
 //
 // Generic includes
 //
 //-----------------------------------------------------------------------------

 //-----------------------------------------------------------------------------
 //
 // CONSTANTs / ENUMs
 //
 //-----------------------------------------------------------------------------
#define DATA_BLOCK_HEADER_SIZE 3
#define BLOCK_SIZE 2
#define BLOCK_DATA_START 3
#define BLOCK_TAG 0
#define MANDATORY_FIELDS_SIZE 5
#define MAX_SECTION_DATA_SIZE 251
#define DISPLAY_SECTION_SIZE 256
#define MAX_DATA_BLOCK_TAGS 20
#define SECTION_START_HEADER 4
#define DID_1_3_DOT_CLOCK_SCALE 10000
#define DID_1_3_TIMING_CODE_SIZE_IN_BYTES 1
#define DISPLAY_ID_VER_1_3 0x12
#define DISPLAYID_TYPE7_PIXELRATE_MULTIPLIER 7425 // YCbCr420 Minimum Pixel Rate Requirement = 74.25MP/s × field value

typedef enum _DISPLAYID_BLOCK_TAG
{
    DID_DATA_BLOCK_PRODUCT_ID = 0,
    DID_DATA_BLOCK_DISPLAY_PARAMS,
    DID_DATA_BLOCK_COLOR_CHARACTERISTICS,
    DID_DATA_BLOCK_DETAILED_TIMING_1,
    DID_DATA_BLOCK_DETAILED_TIMING_2,
    DID_DATA_BLOCK_SHORT_TIMING_3,
    DID_DATA_BLOCK_DMTID_CODE_TIMING_4,
    DID_DATA_BLOCK_VESA_TIMING_STD,
    DID_DATA_BLOCK_CEA_TIMING_STD,
    DID_DATA_BLOCK_VIDEO_RANGE_LIMITS,
    DID_DATA_BLOCK_PRODUCT_SERIAL_NUMBER,
    DID_DATA_BLOCK_ASCII_STRING,
    DID_DATA_BLOCK_DISPLAY_DEVICE_DATA,
    DID_DATA_BLOCK_INTERFACE_POWER_SEQ,
    DID_DATA_BLOCK_TRANSFER_CHARACTERISTICS,
    DID_DATA_BLOCK_DISPLAY_INTERFACE,
    DID_DATA_BLOCK_STEREO_DISPLAY_INTERFACE,
    DID_DATA_BLOCK_SHORT_DESCRIPTOR_TIMING_5,
    DID_DATA_BLOCK_TILED_TOPOLOGY,
    DID_DATA_BLOCK_DETAILED_DESCRIPTOR_TIMING_6,
    DID_DATA_BLOCK_VENDOR_SPECIFIC = 0x7F,
    DID_DATA_BLOCK_RESERVED = 0x80,
    DID_DATA_BLOCK_CEA_SPECIFIC = 0x81
} DISPLAYID_BLOCK_TAG;

typedef enum _DISPLAYID_TIMING_TYPE
{
    DISPLAYID_DTD_TIMING_I = 1,
    DISPLAYID_DTD_TIMING_II,
    DISPLAYID_SHORT_TIMING_III,
    DISPLAYID_DMTID_TIMING_IV,
} DISPLAYID_TIMING_TYPE;

typedef enum _DISPLAYID_PRODUCT_ID
{
    DID_PID_EXTENSION = 0,         // 00    -    Extension section
    DID_PID_TEST_STRUCTURE,        // 01    -    Test structure
    DID_PID_DISPLAY_PANEL,         // 02    -    Display panel or other transducer, LCD or PDP module
    DID_PID_STANDALONE_DEVICE,     // 03    -    Standalone display device
    DID_PID_TV_RECIEVER,           // 04    -    Television receiver
    DID_PID_REPEATER,              // 05    -    Repeater or translator
    DID_PID_DIRECT_DRIVER_MONITOR, // 06    -    Direct drive monitor
    DID_PID_RESERVED,              // 07 or above - Reserved
} DISPLAYID_PRODUCT_ID;

#pragma pack(1)
//-----------------------------------------------------------------------------
//
//  Display ID section Header
//  Size: 4 bytes
//
//-----------------------------------------------------------------------------
typedef struct _DISPLAYID_HEADER
{
    union
    {
        DDU32 Value;
        struct
        {
            DDU8 RevisionAndVersion; // 1st byte - Display ID version and revision ( > 0)
            DDU8 BytesInSection;     // 2nd byte - Bytes in section ( <= 251)
            DDU8 ProductID;          // 3rd byte - Product ID of type DISPLAYID_PRODUCT_ID, will be 0 for extension blocks
            DDU8 ExtensionCount;     // 4th byte - Number of extensions , will be 0 for extension blocks
        };
    };
} DISPLAYID_HEADER;

typedef struct _DISPLAYID_EDID_EXTN_HEADER
{
    DDU8 TagId; // Tag ID -- This value will always be 0x70 for DID as EDID extension
    DISPLAYID_HEADER DisplayIdHeader;
} DISPLAYID_EDID_EXTN_HEADER;

typedef struct _DISPLAYID_BLOCK
{
    DDU8 BlockID; // Data Block Identification
    union
    {
        DDU8 RevAndOtherData; // Data Block Revision and Other Data
        struct
        {
            DDU8 Revision : 3; // Revision
            DDU8 Data : 5;     // Other Block specific Data
        };
        struct
        {
            // used for Type X timings
            DDU8 BlockRevision : 3; // Revision
            DDU8 UNIQUENAME(Rsvd) : 1;
            DDU8 PayloadBytesPerDescriptor : 3; // Number of Payload Bytes (M) in a Timing Descriptor
            DDU8 UNIQUENAME(Rsvd) : 1;
        };
    };
    DDU8 NumberOfBytes; // Number of Payload Bytes 0 to 248
} DISPLAYID_BLOCK;

//-----------------------------------------------------------------------------
//
// Product ID data block
// Tag ID: 0x00 - DID_DATA_BLOCK_PRODUCT_ID
// Size - 12 mandatory bytes + Variable size block for Product ID string
//
//-----------------------------------------------------------------------------
typedef struct _DISPLAYID_PNP_OUI_ID
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    union
    {
        DDU8 VendorProductID[12]; // Vendor / Product identification

        struct
        {
            // With Didplay ID2.0 Manufacturer ID is not PlugnPlay ID (PnP ID)
            // But IEEE Organizationally Unique Identification (OUI ID)
            DDU8 ManufacturerID[3]; // Bytes 3, 4 and 5 : Vendor/Manufacturer ID
            DDU8 ProductID[2];      // Bytes 6 and 7 : Product ID
            DDU8 SerialNumber[4];   // Bytes 8 to 11 : Serial numbers
            DDU8 WeekOfManufacture; // Byte 12 : Week of manufacture
            DDU8 YearOfManufacture; // Byte 13 : Year of manufacture
            DDU8 SizeOfProductID;   // Byte 14 : size of product ID string
        };
    };

    // Variable size Product ID string
    DDU8 ProductIDString[1]; // Defined to get the pointer to Product ID string
} DISPLAYID_PNP_OUI_ID;

//-----------------------------------------------------------------------------
//
//  Display Parameters Data Block
//  Tag ID: 0x01 - DID_DATA_BLOCK_DISPLAY_PARAMS
//  Size - 12 byte Block
//
//-----------------------------------------------------------------------------

typedef struct _DISPLAYID_DISPLAY_PARAMETERS
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    DDU16 HImageSize;  // 2 bytes of H. Image size
    DDU16 VImageSize;  // 2 bytes of V. Image size
    DDU16 HPixelCount; // 2 bytes of H. Pixel count
    DDU16 VPixelCount; // 2 bytes of V. Pixel count
    union
    {
        DDU8 SupportFlag; // 1 bit flag
        struct
        {
            // 0 1 2 3 4 5 6 7
            DDU8 DeInterlacing : 1;    //               1 De-interlacing
            DDU8 SupportsAI : 1;       //             1   Reserved at 0
            DDU8 FixedPixelFormat : 1; //           1     Fixed pixel format
            DDU8 FixedTiming : 1;      //         1       Fixed timing
            DDU8 PowerManagement : 1;  //       1         Power management
            DDU8 AudioInpOverride : 1; //     1           Audio input override
            DDU8 SeperateAudioInp : 1; //   1             Separate audio inputs provided
            DDU8 AudioSupported : 1;   // 1               Audio support on video interface
        };
    };

    DDU8 TransferChar; // Transfer Charateristic Gamma, 0xFF here means no Gamma. Gamma 2.2 will be 0x78
    // Gamma value = (TransferChar / 100) + 1 (Range 1.0 to 3.54)
    DDU8 AspectRatio; // Aspect ratio
    // AR = (AspectRatio / 100) + 1 (Range 1.0 to 3.55)
    union
    {
        DDU8 ColorBitDepth; // Color bit depth
        struct
        {
            DDU8 DynRangeBpc : 1;       //  Dynamic range, bits/color, overall, minus one (range 1 → 16)
            DDU8 DynRangeNativeBpc : 1; //  Dynamic range, bits/color, “native” (per above), minus one (range 1 → 16)
        };
    };
} DISPLAYID_DISPLAY_PARAMETERS;

//-----------------------------------------------------------------------------
//
//  Color Characteristics Block
//  Tag ID: 0x02 - DID_DATA_BLOCK_COLOR_CHARACTERISTICS
//  Size - Variable size block; ((# of WhitePoints + # of primaries) * 3) + 1
//
//-----------------------------------------------------------------------------
typedef struct _COLOR_CHAR_REVID
{
    union
    {
        DDU8 RevAndOtherData; // Data Block Revision and Other Data
        struct
        {
            DDU8 Revision : 3;       // Revision
            DDU8 TransferCharID : 4; // 0 - not associated to a particular Transfer Characteristic block,
            // else needs to be associated with right Transfer Characteristic block (Block ID: 0xE)
            DDU8 TranferCharInfo : 1; // Color info. represented in terms of 0 - 1931 CIE (x,y) coordinates, 1 - 1976 CIE (u’v’) coordinates
        };
    };
} COLOR_CHAR_REVID;

typedef struct _DID_CHROMATICITY_DATA
{
    DDU8 ColorxOruLowBits; // Color x or u’ value or standard color space identifier Low bits

    union
    {
        DDU8 ChromaticityValue;
        struct
        {
            DDU8 ColorxOruHighBits : 4; // Color x or u’ value or standard color space identifier high bits
            DDU8 ColoryOrvLowBits : 4;  // Color y or v’ value or standard color space identifier Low bits
        };
    };

    DDU8 ColoryOrvHighBits; // Color y or v’ value or standard color space identifier High bits
} DID_CHROMATICITY_DATA;

typedef struct _COLOR_CHARS_INFO
{
    DDU8 NumOfWhitePoints : 4; // No. of white points (Nw)
    DDU8 NumOfPrimaries : 3;   // No. of primaries (Np)
    DDU8 ColorPlane : 1;       // Color plane color; 1 - display employs temporal color, 0 - display employs spatial color
} COLOR_CHARS_INFO;

#define DID_NUM_PRIMARY_WHITE_POINTS 4

typedef struct _DISPLAYID_COLOR_CHARS
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    COLOR_CHARS_INFO ColorCharsInfo; // Color characteristic information

    // Multiple instance of  Chromaticity Data Block can be present
    DID_CHROMATICITY_DATA ChromaData[DID_NUM_PRIMARY_WHITE_POINTS];
} DISPLAYID_COLOR_CHARS;

//-----------------------------------------------------------------------------
//
//  Video Timing Modes -
//      Tag ID: 0x03 - DID_DATA_BLOCK_DETAILED_TIMING_1
//      Tag ID: 0x04 - DID_DATA_BLOCK_DETAILED_TIMING_2
//      Tag ID: 0x05 - DID_DATA_BLOCK_SHORT_TIMING_3
//      Tag ID: 0x06 - DID_DATA_BLOCK_DMTID_CODE_TIMING_4
//      Tag ID: 0x11 - DID_DATA_BLOCK_SHORT_DESCRIPTOR_TIMING_5
//      Tag ID: 0x13 - DID_DATA_BLOCK_DETAILED_DESCRIPTOR_TIMING_6
//      Tag ID: 0x07 - DID_DATA_BLOCK_VESA_TIMING_STD
//      Tag ID: 0x08 - DID_DATA_BLOCK_CEA_TIMING_STD
//
//-----------------------------------------------------------------------------

typedef enum _FRAME_SCAN_TYPE
{
    SCAN_TYPE_PROGRESSIVE = 0,
    SCAN_TYPE_INTERLACED,
} FRAME_SCAN_TYPE;

typedef enum _S3D_SUPPORT
{
    S3D_MONOSCOPIC = 0,
    S3D_STEREO_ONLY,
    S3D_MONO_OR_STEREO,
    S3D_RSVD
} S3D_SUPPORT;

typedef struct _DID_TIMINGINFO_1
{
    DDU8 PixelClock[3]; // 3 bytes of pixel clock data
    union
    {
        DDU8 TimingOption; // Timing option
        struct
        {
            DDU8 AspectRatio : 4;     // Aspect ratio ; Consists of Index [0->6] into --> DidAspectRatioTable
            DDU8 FrameScanType : 1;   // Scan type 0 - Progressive Scan frame, 1 - Interlaced Scan frame of type FRAME_SCAN_TYPE
            DDU8 Stereo3DSupport : 2; // 3D Stereo support of type S3D_SUPPORT
            DDU8 Preferedflag : 1;    // 1 - This Timing info is the Prefered detailed timing
        };
    };

    DDU16 HActive; // H. Active Image pixels minus 1 [1 → 65,536 Lines]
    DDU16 HBlank;  // H. Blank pixels minus 1 [1 → 65,536 Lines]

    union
    {
        DDU16 HSyncOffsetPolarity; // H. sync Offset
        struct
        {
            DDU16 HSyncOffset : 15;  // H. sync Offset
            DDU16 HSyncPolarity : 1; // H Sync Polarity 0 - -ve, 1 - +ve
        };
    };
    DDU16 HSyncWidth; // H. sync Width minus 1 [1 → 65,536 Lines]
    DDU16 VActive;    // V. Active Image lines minus 1 [1 → 65,536 Lines]
    DDU16 VBlank;     // V. Blank lines minus 1 [1 → 65,536 Lines]

    union
    {
        DDU16 VSyncOffsetPolarity; // V. sync Offset
        struct
        {
            DDU16 VSyncOffset : 15;  // V. sync Offset
            DDU16 VSyncPolarity : 1; // V Sync Polarity 0 - -ve, 1 - +ve
        };
    };

    DDU16 VSyncWidth; // V. sync Width
} DID_TIMINGINFO_1;

typedef struct _DISPLAYID_DTD_TIMING_1
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    // Multiple instance of Timing Info 1 Data Block can be present
    DID_TIMINGINFO_1 DidTimingInfo1; // Defined to get the pointer to first instance of Timing Info 1 Data Block
} DISPLAYID_DTD_TIMING_1;

//-----------------------------------------------------------------------------
//
//  Video Timing Modes - Detailed Timing "Type II timing" data block
//  Tag ID: 0x04 - DID_DATA_BLOCK_DETAILED_TIMING_2
//  Size - Variable size block -- 11 bytes per timing * N # of Timings
//
//-----------------------------------------------------------------------------
typedef struct _DID_TIMINGINFO_2
{
    DDU8 PixelClock[3]; // 3 bytes of pixel clock data

    union
    {
        DDU8 TimingOption; // Timing option
        struct
        {
            DDU8 UNIQUENAME(Rsvd) : 2; // Reserved
            DDU8 VSyncPolarity : 1;    // V Sync Polarity 0 - -ve, 1 - +ve
            DDU8 HSyncPolarity : 1;    // H Sync Polarity 0 - -ve, 1 - +ve
            DDU8 FrameScanType : 1;    // Scan type 0 - Progressive Scan frame, 1 - Interlaced Scan frame of type FRAME_SCAN_TYPE
            DDU8 Stereo3DSupport : 2;  // 3D Stereo support of type S3D_SUPPORT
            DDU8 Preferedflag : 1;     // 1 - This Timing info is the Prefered detailed timing
        };
    };

    union
    {
        DDU16 HActiveBlank; // 2 bytes H. Active Image pixels/H. Blank pixels
        struct
        {
            DDU16 HActive : 9; // H. Active Image pixels, (Pixels/8) - 1 [1 → 1024 Chars]
            DDU16 HBlank : 7;  // H. Blank pixels, (Pixels/8) - 1 [1 → 128 Chars]
        };
    };

    union
    {
        DDU8 HSyncOffsetWidth; // 1 bytes H. Sync offset/H. Sync width
        struct
        {
            DDU8 HSyncOffset : 4; // H. sync Offset, (Pixels/8) - 1
            DDU8 HSyncWidth : 4;  // H. sync Width, (Pixels/8) - 1
        };
    };

    union
    {
        DDU16 VerticalActiveImage; // 2 bytes V. Active Image lines
        struct
        {
            DDU16 VActive : 12;         // V. active lines
            DDU16 UNIQUENAME(Rsvd) : 4; // Reserved
        };
    };

    DDU8 VBlank; // V. Blank lines

    union
    {
        DDU8 VSyncOffsetWidth; // V. Sync offset/V. Sync width
        struct
        {
            DDU8 VSyncOffset : 4; // V. Sync offset
            DDU8 VSyncWidth : 4;  // V. Sync width
        };
    };
} DID_TIMINGINFO_2;

typedef struct _DISPLAYID_DTD_TIMING_2
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    // Multiple instance of Timing Info 2 Data Block can be present
    DID_TIMINGINFO_2 DidTimingInfo2; // Defined to get the pointer to first instance of Timing Info 2 Data Block
} DISPLAYID_DTD_TIMING_2;

//-----------------------------------------------------------------------------
//
//  Video Timing Modes - Short Timing "Type III Timing" data block
//  Tag ID: 0x05 - DID_DATA_BLOCK_SHORT_TIMING_3
//  Size - Variable size block -- 3 bytes per timing * N # of Timings
//
//-----------------------------------------------------------------------------
typedef struct _DID_TIMINGINFO_3
{
    union
    {
        DDU8 TimingOptions; // Timing option

        struct
        {
            DDU32 AspectRatio : 4;   // Consists of array of ASPECT_RATIO --> DidAspectRatioTable
            DDU32 TimingFormula : 3; // Timing Formula/Algorithm 0 - CVT standard Blanking, 1 - CVT reduced blanking
            DDU32 Preferedflag : 1;  // 1 - This Timing info is the Prefered detailed timing
        };
    };

    DDU8 HActive; // H.Active Image pixels, (Pixels/8) - 1 [1 → 256 Chars]
    union
    {
        DDU8 FrameTransferTypeAndRate; // 1 byte having info of Transfer Rate/Scan type
        struct
        {
            DDU8 TransferRate : 7;  // Frame/Field Refresh Rate [1 → 128HZ]
            DDU8 FrameScanType : 1; // Scan type 0 - Progressive Scan frame, 1 - Interlaced Scan frame of type FRAME_SCAN_TYPE
        };
    };
} DID_TIMINGINFO_3;

typedef struct _DISPLAYID_SHORT_TIMING_3
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    // Multiple instance of Timing Info 3 Data Block can be present
    DID_TIMINGINFO_3 DidTimingInfo3; // Defined to get the pointer to first instance of Timing Info 3 Data Block
} DISPLAYID_SHORT_TIMING_3;

//-----------------------------------------------------------------------------
//
//  Video Timing Modes - DMT Timing Code "Type IV Timing" data block
//  Tag ID: 0x06 - DID_DATA_BLOCK_DMTID_CODE_TIMING_4 -- 1 byte timing block
//
//-----------------------------------------------------------------------------
typedef enum _TIMING_CODE_TYPE
{
    TIMING_CODE_DMT = 0,
    TIMING_CODE_CEA_VIC,
    TIMING_CODE_HDMI_VIC,
    TIMING_CODE_RSVD
} TIMING_CODE_TYPE;

typedef struct _SHORT_TIMING_4_REVID
{
    union
    {
        DDU8 RevAndOtherData; // Data Block Revision and Other Data
        struct
        {
            DDU8 Revision : 3;         // Revision
            DDU8 UNIQUENAME(Rsvd) : 3; // Reserved
            DDU8 TimingCode : 2;       // Timing Code Type of type TIMING_CODE_TYPE
        };
    };
} SHORT_TIMING_4_REVID;

typedef struct _DISPLAYID_DMTID_CODE_TIMING_4
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    // Multiple instance of Timing Info 4 Data Block can be present
    DDU8 DidTimingInfo4; // Defined to get the pointer to first instance of Timing Info 4 Data Block
} DISPLAYID_DMTID_CODE_TIMING_4;

//-----------------------------------------------------------------------------
//
//  Video Timing Modes - Short descriptor "Type V Timing" data block
//  Tag ID: 0x11 - DID_DATA_BLOCK_SHORT_DESCRIPTOR_TIMING_5
//  Size - Variable size block -- 7 bytes per timing * N # of Timings
//
//  @todo: VSDI-22878
//  This is not yet implemented as there is no display indicating this in display ID as of now
//  Will need to implement at a later point of time if required
//
//-----------------------------------------------------------------------------
typedef enum _TIMING_FORMULA_5
{
    VESA_CVT_REDUCED_BLANKING_V2,
    VESA_CVT_CUSTOM_REDUCED_BLANKING,
} TIMING_FORMULA_5;

typedef struct _DID_TIMINGINFO_5
{
    union
    {
        DDU8 TimingOptions; // 1 byte Timing option

        struct
        {
            DDU32 TimingFormula : 2;    // Timing Formula/Algorithm of type TIMING_FORMULA_5
            DDU32 UNIQUENAME(Rsvd) : 2; // Reserved
            DDU32 RRSupported : 1;      // Refresh Rate supported; 0 - REFRESH RATE*(1000/1001) supported, 1 - not supported
            DDU32 Stereo3DSupport : 2;  // 3D Stereo support of type S3D_SUPPORT
            DDU32 Preferedflag : 1;     // 1 - This Timing info is the Prefered detailed timing
        };
    };

    DDU16 HActive;     // H. Active Image pixels
    DDU16 VActive;     // V. Active Image lines
    DDU16 RefreshRate; // Refresh Rate 1 → 256 Hz
} DID_TIMINGINFO_5;

typedef struct _DISPLAYID_SHORT_TIMING_5
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    // Multiple instance of Timing Info 5 Data Block can be present
    DID_TIMINGINFO_5 DIDTiming5; // Defined to get the pointer to first instance of Timing Info 5 Data Block
} DISPLAYID_SHORT_TIMING_5;

//-----------------------------------------------------------------------------
//
//  Video Timing Modes - Detailed descriptor "Type VI Timing" data block
//  Tag ID: 0x13 - DID_DATA_BLOCK_DETAILED_DESCRIPTOR_TIMING_6
//  Size - Variable size block -- 0x14 or 0x17 bytes per timing block * N # of Timings
//
//  @todo: VSDI-22878
//  This is not yet implemented as there is no display indicating this in display ID as of now
//  Will need to implement at a later point of time if required
//
//-----------------------------------------------------------------------------
typedef struct _DID_TIMINGINFO_6_TYPE1
{
    DDU16 PixelClockLowBits; // 2 bytes of pixel clock bits 15:0

    union
    {
        DDU8 PixelClkHighAndTimingOptions; // 1 bytes pixel clock and 1 byte Timing option
        struct
        {
            DDU8 PixelClock : 6;      // 6 higher order bits of pixel clock
            DDU8 AspectAndSzInfo : 1; // 0 - Aspect & Size Information not included (only 0x14 bytes), 1 - Included (0x17 bytes)
            DDU8 Preferedflag : 1;    // 1 - This Timing info is the Prefered detailed timing
        };
    };

    union
    {
        DDU16 HActiveAndPolarity; // H. Active Image pixels and Timing Flags
        struct
        {
            DDU16 HActive : 14;         // H. Active Image pixels
            DDU16 UNIQUENAME(Rsvd) : 1; // Reserved
            DDU16 HSyncPolarity : 1;    // H Sync Polarity 0 - -ve, 1 - +ve
        };
    };

    union
    {
        DDU16 VActiveAndPolarity; // V. Active Image pixels and Timing Flags
        struct
        {
            DDU16 VActive : 14;         // V. Active Image pixels
            DDU16 UNIQUENAME(Rsvd) : 1; // Reserved
            DDU16 VSyncPolarity : 1;    // V Sync Polarity 0 - -ve, 1 - +ve
        };
    };

    DDU8 HBlankLowBits;  // H. Blank pixels [Low bits 7 → 0]
    DDU8 HOffsetLowBits; // H. Offset (Horizontal Front Porch) [Low bits 7 → 0]

    union
    {
        DDU8 HBlankOffsetHighBits; // H. sync Offset
        struct
        {
            DDU8 HBlankHighBits : 4;  // H. sync Offset
            DDU8 HOffsetHighBits : 4; // H Sync Polarity 0 - -ve, 1 - +ve
        };
    };
    DDU8 HSyncWidth; // H. sync Width
    DDU8 VBlank;     // V. Blank lines
    DDU8 VOffset;    // V. Offset (Vertical Front Porch)
    union
    {
        DDU8 VSyncAndTimingFlags; // V. sync Offset
        struct
        {
            DDU8 VSyncWidth : 4;       // V. sync Offset
            DDU8 UNIQUENAME(Rsvd) : 5; // Reserved
            DDU8 Stereo3DSupport : 2;  // 3D Stereo support of type S3D_SUPPORT
            DDU8 FrameScanType : 1;    // Scan type 0 - Progressive Scan frame, 1 - Interlaced Scan frame
        };
    };
} DID_TIMINGINFO_6_TYPE1;

typedef struct _DID_TIMINGINFO_6_TYPE2
{
    DID_TIMINGINFO_6_TYPE1 Type1CommonTiming6;

    DDU8 AspectMultiplier;      // Aspect Ratio = Aspect Multiplier x 3 / 256 0 → 2.99
    DDU8 VImageBaseSizeLowBits; // Vertical Image Size = Vertical Image Base Size x Size Multiplier [Low bits 7 → 0]
    union
    {
        DDU8 VImageBaseSizeHighBitsAndSzMult; // Vertical Image Base Size (High Bits) & Size Multiplier
        struct
        {
            DDU8 VImageBaseSizeHighBits; // Vertical Image Base Size [High bits 11 → 8]
            DDU8 SizeMultiplier;         // Size multiplier
        };
    };
} DID_TIMINGINFO_6_TYPE2;

typedef struct _DISPLAYID_DETAILED_TIMING_6
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    // Multiple instance of Timing Info 6 Data Block can be present
    DID_TIMINGINFO_6_TYPE2 DIDTiming6; // Defined to get the pointer to first instance of Timing Info 6 Data Block
} DISPLAYID_DETAILED_TIMING_6;

//-----------------------------------------------------------------------------
//
//  Video Timing Modes - VESA Standard Timing data block
//  Tag ID: 0x07 - DID_DATA_BLOCK_VESA_TIMING_STD --
//  Size - 10 byte timing block
//
//-----------------------------------------------------------------------------
#define VESA_STD_TIMING_BLOCK_LENGTH 10

typedef struct _DID_VESA_STD_TIMING_INFO
{
    union
    {
        DDU8 TimingsModes[VESA_STD_TIMING_BLOCK_LENGTH];
        struct
        {
            // Byte 1
            DDU8 Supports640x350_85 : 1;
            DDU8 Supports640x400_85 : 1;
            DDU8 Supports720x400_85 : 1;
            DDU8 Supports640x480_60 : 1;
            DDU8 Supports640x480_72 : 1;
            DDU8 Supports640x480_75 : 1;
            DDU8 Supports640x480_85 : 1;
            DDU8 Supports800x600_56 : 1;

            // Byte 2
            DDU8 Supports800x600_60 : 1;
            DDU8 Supports800x600_72 : 1;
            DDU8 Supports800x600_75 : 1;
            DDU8 Supports800x600_85 : 1;
            DDU8 Supports800x600_120RB : 1;
            DDU8 Supports848x480_60 : 1;
            DDU8 Supports1024x768_43INT : 1;
            DDU8 Supports1024x768_60 : 1;

            // Byte 3
            DDU8 Supports1024x768_70 : 1;
            DDU8 Supports1024x768_75 : 1;
            DDU8 Supports1024x768_85 : 1;
            DDU8 Supports1024x768_120RB : 1;
            DDU8 Supports1152x864_75 : 1;
            DDU8 Supports1280x768_60RB : 1;
            DDU8 Supports1280x768_60 : 1;
            DDU8 Supports1280x768_75 : 1;

            // Byte 4
            DDU8 Supports1280x768_85 : 1;
            DDU8 Supports1280x768_120RB : 1;
            DDU8 Supports1280x800_60RB : 1;
            DDU8 Supports1280x800_60 : 1;
            DDU8 Supports1280x800_75 : 1;
            DDU8 Supports1280x800_85 : 1;
            DDU8 Supports1280x800_120RB : 1;
            DDU8 Supports1280x960_60 : 1;

            // Byte 5
            DDU8 Supports1280x960_85 : 1;
            DDU8 Supports1280x960_120RB : 1;
            DDU8 Supports1280x1024_60 : 1;
            DDU8 Supports1280x1024_75 : 1;
            DDU8 Supports1280x1024_85 : 1;
            DDU8 Supports1280x1024_120RB : 1;
            DDU8 Supports1360x768_60 : 1;
            DDU8 Supports1360x768_120RB : 1;

            // Byte 6
            DDU8 Supports1400x1050_60RB : 1;
            DDU8 Supports1400x1050_60 : 1;
            DDU8 Supports1400x1050_75 : 1;
            DDU8 Supports1400x1050_85 : 1;
            DDU8 Supports1400x1050_120RB : 1;
            DDU8 Supports1440x900_60RB : 1;
            DDU8 Supports1440x900_60 : 1;
            DDU8 Supports1440x900_75 : 1;

            // Byte 7
            DDU8 Supports1400x900_85 : 1;
            DDU8 Supports1440x900_120RB : 1;
            DDU8 Supports1600x1200_60 : 1;
            DDU8 Supports1600x1200_65 : 1;
            DDU8 Supports1600x1200_70 : 1;
            DDU8 Supports1600x1200_75 : 1;
            DDU8 Supports1600x1200_85 : 1;
            DDU8 Supports1600x1200_120RB : 1;

            // Byte 8
            DDU8 Supports1680x1050_60RB : 1;
            DDU8 Supports1680x1050_60 : 1;
            DDU8 Supports1680x1050_75 : 1;
            DDU8 Supports1680x1050_85 : 1;
            DDU8 Supports1680x1050_120RB : 1;
            DDU8 Supports1792x1344_60 : 1;
            DDU8 Supports1792x1344_75 : 1;
            DDU8 Supports1792x1344_120RB : 1;

            // Byte 9
            DDU8 Supports1856x1392_60 : 1;
            DDU8 Supports1856x1392_75 : 1;
            DDU8 Supports1856x1392_120RB : 1;
            DDU8 Supports1920x1200_60RB : 1;
            DDU8 Supports1920x1200_60 : 1;
            DDU8 Supports1920x1200_75 : 1;
            DDU8 Supports1920x1200_85 : 1;
            DDU8 Supports1920x1200_120RB : 1;

            // Byte 10
            DDU8 Supports1920x1440_60 : 1;
            DDU8 Supports1920x1440_75 : 1;
            DDU8 Supports1920x1440_120RB : 1;
            DDU8 Supports2560x1600_60RB : 1;
            DDU8 Supports2560x1600_60 : 1;
            DDU8 Supports2560x1600_75 : 1;
            DDU8 Supports2560x1600_85 : 1;
            DDU8 Supports2560x1600_120RB : 1;
        };
    };
} DID_VESA_STD_TIMING_INFO;

typedef struct _DISPLAYID_VESA_TIMING_STD
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    // Multiple instances of Vesa Std Timing Info can be present
    DID_VESA_STD_TIMING_INFO VesaStdTimingInfo; // Defined to get the pointer to first instance of Vesa Standard Timing Info Data Block
} DISPLAYID_VESA_TIMING_STD;

//-----------------------------------------------------------------------------
//
//  Video Timing Modes - VESA CEA Timing Standard data block
//  Tag ID: 0x08 - DID_DATA_BLOCK_CEA_TIMING_STD
//  Size - 8 byte timing block
//
//-----------------------------------------------------------------------------
#define DID_CEA_TIMING_STD_SIZE 8

typedef struct _DID_CEA_TIMING_INFO
{
    union
    {
        DDU8 TimingsModes[DID_CEA_TIMING_STD_SIZE];
        struct
        {
            // Byte 1
            DDU8 Supports640x400_60_4_3 : 1;
            DDU8 Supports720x480_60_4_3 : 1;
            DDU8 Supports720x480_60_16_9 : 1;
            DDU8 Supports1280x720_60_16_9 : 1;
            DDU8 Supports1920x1080i_60_16_9 : 1;
            DDU8 Supports720_1440_x480i_60_4_3 : 1;
            DDU8 Supports720_1440_x480i_60_16_9 : 1;
            DDU8 Supports720_1440_x240p_60_4_3 : 1;

            // Byte 2
            DDU8 Supports720_1440_x240_60_16_9 : 1;
            DDU8 Supports2880x480i_60_4_3 : 1;
            DDU8 Supports2880x480i_60_16_9 : 1;
            DDU8 Supports2880x240p_60_4_3 : 1;
            DDU8 Supports2880x240p_60_16_9 : 1;
            DDU8 Supports1440x480p_60_4_3 : 1;
            DDU8 Supports1440x480p_60_16_9 : 1;
            DDU8 Supports1920x1080p_60_16_9 : 1;

            // Byte 3
            DDU8 Supports720x576p_50_4_3 : 1;
            DDU8 Supports720x576p_50_16_9 : 1;
            DDU8 Supports1280x720_50_16_9 : 1;
            DDU8 Supports1920x1080i_50_16_9 : 1;
            DDU8 Supports720_1440_x576i_50_4_3 : 1;
            DDU8 Supports720_1440_x576i_50_16_9 : 1;
            DDU8 Supports720_1440_x288p_50_4_3 : 1;
            DDU8 Supports720_1440_x288p_50_16_9 : 1;

            // Byte 4
            DDU8 Supports2880x576i_50_4_3 : 1;
            DDU8 Supports2880x576i_50_16_9 : 1;
            DDU8 Supports2880x288p_50_4_3 : 1;
            DDU8 Supports2880x288p_50_16_9 : 1;
            DDU8 Supports1440x576p_50_4_3 : 1;
            DDU8 Supports1440x576p_50_16_9 : 1;
            DDU8 Supports1920x1080p_50_16_9 : 1;
            DDU8 Supports1920x1080p_24_16_9 : 1;

            // Byte 5
            DDU8 Supports1920x1080p_25_16_9 : 1;
            DDU8 Supports1920x1080p_30_16_9 : 1;
            DDU8 Supports2880x480p_60_4_3 : 1;
            DDU8 Supports2880x480p_60_16_9 : 1;
            DDU8 Supports2880x576p_50_4_3 : 1;
            DDU8 Supports2880x576p_50_16_9 : 1;
            DDU8 Supports1920x1080i_50_16_9d : 1;
            DDU8 Supports1920x1080p_100_4_3 : 1;

            // Byte 6
            DDU8 Supports1280x720p_100_16_9 : 1;
            DDU8 Supports720x576p_100_4_3 : 1;
            DDU8 Supports720x576p_100_16_9 : 1;
            DDU8 Supports720_1440_x576i_100_4_ : 1;
            DDU8 Supports720_1440_x576i_100_16_9 : 1;
            DDU8 Supports1920x1080i_120_16_9 : 1;
            DDU8 Supports1280x720p_120_16_9 : 1;
            DDU8 Supports720x480p_120_4_3 : 1;

            // Byte 7
            DDU8 Supports720x480p_120_16_9 : 1;
            DDU8 Supports720_1440_x480i_120_4_3 : 1;
            DDU8 Supports720_1440_x480i_120_16_9 : 1;
            DDU8 Supports720x576p_200_4_3 : 1;
            DDU8 Supports720x576p_200_16_9 : 1;
            DDU8 Supports720_1440_x576i_200_4_3 : 1;
            DDU8 Supports720_1440_x576i_200_16_9 : 1;
            DDU8 Supports720x480p_240_4_3 : 1;

            // Byte 8
            DDU8 Supports720x480p_240_16_9 : 1;
            DDU8 Supports720_1440_x480i_240_4_3 : 1;
            DDU8 Supports720_1440_x480i_240_16_9 : 1;
            DDU8 Supports1280x720p_24_16_9 : 1;
            DDU8 Supports1280x720p_25_16_9 : 1;
            DDU8 Supports1280x720p_30_16_9 : 1;
            DDU8 Supports1920x1080p_120_16_9 : 1;
            DDU8 Supports1920x1080p_100_16_9 : 1;
        };
    };
} DID_CEA_TIMING_INFO;

typedef struct _DISPLAYID_CEA_TIMING_STD
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    // Multiple instances of CEA Timing Info can be present
    DID_CEA_TIMING_INFO CeaTimingInfo; // Defined to get the pointer to first instance of CEA Timing Info Data Block
} DISPLAYID_CEA_TIMING_STD;

//-----------------------------------------------------------------------------
//
//  Video Timing Range Limits
//  Tag ID: 0x09 - DID_DATA_BLOCK_VIDEO_RANGE_LIMITS
//  Size - 15 byte block
//
//-----------------------------------------------------------------------------
typedef struct _DISPLAYID_VIDEO_RANGE
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    DDU8 MinimumPixelClock[3];      // Minimum pixel in 10,000 Hz units
    DDU8 MaximumPixelClock[3];      // Maximum pixel in 10,000 Hz units
    DDU8 MinimumHorizontalFreq;     // Minimum H. frequency
    DDU16 MaximumHorizontalFreq;    // Maximum H. frequency
    DDU8 MinimumHorizontalBlanking; // Minimum H. blanking
    DDU8 MinimumVerticalFreq;       // Minimum V. refresh rate
    DDU8 MaximumVerticalFreq;       // Maximum V. refresh rate
    DDU16 MinimumVerticalBlanking;  // Minimum V. blanking
    union
    {
        DDU8 VideoTimingSupportFlag; // Video timing support
        struct
        {
            DDU8 UNIQUENAME(Rsvd) : 4; // Reserved
            DDU8 DiscreteFreqDisplayDevice : 1;
            DDU8 VesaCvtReducedBlankingSupported : 1;
            DDU8 VesaCvtStandardBlankingSupported : 1;
            DDU8 FrameScanType : 1; // Scan type 0 - Progressive Scan frame, 1 - Interlaced Scan frame
        };
    };
} DISPLAYID_VIDEO_RANGE;

//-----------------------------------------------------------------------------
//
//  Product Serial Number Block
//  Tag ID: 0x0A - DID_DATA_BLOCK_PRODUCT_SERIAL_NUMBER
//  Size - Variable size block
//
//-----------------------------------------------------------------------------
typedef struct _DISPLAYID_PRODUCT_SERIAL_NUMBER
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    // Multiple ASCII characters of Product Serial number can be present
    DDU8 ProductSerialNumber; // Defined to get the pointer to first ASCII byte of the product serial number
} DISPLAYID_PRODUCT_SERIAL_NUMBER;

//-----------------------------------------------------------------------------
//
//  General purpose ASCII string
//  Tag ID: 0x0B - DID_DATA_BLOCK_ASCII_STRING
//  Size - Variable size block
//
//-----------------------------------------------------------------------------
typedef struct _DISPLAYID_ASCII_STRING
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    // Multiple General purpose ASCII characters can be present
    DDU8 ProductSerialNumber; // Defined to get the pointer to first ASCII byte of the General purpose ASCII string
} DISPLAYID_ASCII_STRING;

//-----------------------------------------------------------------------------
//
//  Display Device Data Block
//  Tag ID: 0x0C - DID_DATA_BLOCK_DISPLAY_DEVICE_DATA
//  Size - 13 byte Data block
//
//-----------------------------------------------------------------------------
typedef struct _DISPLAYID_DISPLAY_DEVICE_DATA
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    DDU8 DisplayDeviceTechnology; // Display device technologies(LCD, CRT, etc)
    DDU8 DeviceOperatingMode;     // Upper 4 bits provides primary modes
    // and lower 4 bits additional info
    union
    {
        DDU32 DeviceNativePixelFormat; // Number of pixels in H. and V. directions.
        struct
        {
            DDU32 HPixelCount : 16; // Physical pixels in Horizontal direction (Max 65,536)
            DDU32 VPixelCount : 16; // Physical pixels in Vertical direction (Max 65,536)
        };
    };
    DDU8 PhysicalAspectRatio; // Physical aspect ratio
    // AR = (Long-axis image area size)/(Short-axis image area size)
    DDU8 PhysicalOrientation; // Default Orientation & Rotation Capability; Scan Direction
    DDU8 SubPixelInfo;        // Sub pixel physical layout/relative size of features within each pixel
    DDU16 HAndVPixelPitch;    // Distance from the geometric center of any given dot/pixel its nearest adjacent one
    DDU8 ColorBitDepth;       // Color bit depth of the display device minus one (range 1 → 65,536)
    DDU8 ResponseTime;        // Response time of the display device
} DISPLAYID_DISPLAY_DEVICE_DATA;

//-----------------------------------------------------------------------------
//
//  Interface Power sequencing Data Block
//  Tag ID: 0x0D - DID_DATA_BLOCK_INTERFACE_POWER_SEQ
//  Size - 6 byte Data block
//
//       ----------------------
//    T1/      ............     \
// _____/  T2  /            \  T3 \__T4_
//.........../ T5 _____  T6 \...........
// _______________/     \________________
//
//-----------------------------------------------------------------------------
typedef struct _DISPLAYID_INTERFACE_POWER_SEQUENCING
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    union
    {
        DDU8 PowerSeqT1;
        struct
        {
            DDU8 PowerSeqT1Max : 4; // T1Max = (2 * PowerSeqT1Max) ms (Max 30ms)
            DDU8 PowerSeqT1Min : 4; // T1Min = (PowerSeqT1Min / 10) ms (Max 1.5ms)
        };
    };
    union
    {
        DDU8 PowerSeqT2;
        struct
        {
            DDU8 PowerSeqT2Max : 6;    // T2Max = (2 * PowerSeqT2Max) ms (Max 126ms)
            DDU8 UNIQUENAME(Rsvd) : 2; // Reserved;  T2 Min is assumed to be 0
        };
    };
    union
    {
        DDU8 PowerSeqT3;
        struct
        {
            DDU8 PowerSeqT3Max : 6;    // T3Max =  = (2 * PowerSeqT2Max) ms (Max 126ms)
            DDU8 UNIQUENAME(Rsvd) : 2; // Reserved ; T3 Min is assumed to be 0
        };
    };
    union
    {
        DDU8 PowerSeqT4;
        struct
        {
            DDU8 PowerSeqT4Min : 7;    // T4Min = (PowerSeqT4Min * 10) ms (Max 1270ms)
            DDU8 UNIQUENAME(Rsvd) : 1; // Reserved
        };
    };
    union
    {
        DDU8 PowerSeqT5;
        struct
        {
            DDU8 PowerSeqT5Min : 6;    // T5Min = (PowerSeqT5Min * 10) ms (Max 63)
            DDU8 UNIQUENAME(Rsvd) : 2; // Reserved
        };
    };
    union
    {
        DDU8 PowerSeqT6;
        struct
        {
            DDU8 PowerSeqT6Min : 6;    // T6Min = (PowerSeqT6Min * 10) ms (Max 630ms)
            DDU8 UNIQUENAME(Rsvd) : 2; // Reserved
        };
    };
} DISPLAYID_INTERFACE_POWER_SEQUENCING;

//-----------------------------------------------------------------------------
//
//  Transfer Characteristics Data Block
//  Tag ID: 0x0E - DID_DATA_BLOCK_TRANSFER_CHARACTERISTICS
//  Size - Variable size block
//
//-----------------------------------------------------------------------------
typedef struct _TRANSFER_CHAR_REVID
{
    union
    {
        DDU8 RevAndOtherData; // Data Block Revision and Other Data
        struct
        {
            DDU8 Revision : 3;         // Revision
            DDU8 UNIQUENAME(Rsvd) : 1; // Reserved
            DDU8 TransferCharID : 4;   // 0 - not associated to a particular Transfer Characteristic block,
            // else needs to be associated with right Transfer Characteristic block (Block ID: 0xE)
        };
    };
} TRANSFER_CHAR_REVID;

typedef struct _LUMINANCE_SAMPLES
{
    DDU8 NumberOfSamples;  // Number of samples present. If "UsesFourParameterModel" is set, this value should be 5
    DDU8* pLuminanceValue; // Pointer to the samples. If "UsesFourParameterModel" is set, this will be a pointer to 4 bytes of data
} LUMINANCE_SAMPLES;

typedef struct _DISPLAYID_TRANSFER_CHARACTERISTICS
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    union
    {
        DDU8 TransferCharInfo;
        struct
        {
            DDU8 UNIQUENAME(Rsvd) : 5;
            DDU8 UsesFourParameterModel : 1;
            DDU8 PrimaryColorCurves : 1;
            DDU8 WhiteColorCurves : 1;
        };
    };

    // Multiple Luminance Samples can be  in the block
    LUMINANCE_SAMPLES LuminanceSamples; // Defined to get the pointer to first Luminance Sample
} DISPLAYID_TRANSFER_CHARACTERISTICS;

//-----------------------------------------------------------------------------
//
//  Display Interface Data Block
//  Tag ID: 0x0F - DID_DATA_BLOCK_DISPLAY_INTERFACE
//  Size - 10 byte Data block
//
//-----------------------------------------------------------------------------
typedef enum _DID_INTERFACE_TYPE
{
    INTERFACE_TYPE_ANALOG = 0,
    INTERFACE_TYPE_LVDS,
    INTERFACE_TYPE_TMDS_GENERIC,
    INTERFACE_TYPE_RSDS_GENERIC,
    INTERFACE_TYPE_DVID,
    INTERFACE_TYPE_DVII_ANALOG,
    INTERFACE_TYPE_DVII_DIGITAL,
    INTERFACE_TYPE_HDMI_A,
    INTERFACE_TYPE_HDMI_B,
    INTERFACE_TYPE_MDDI,
    INTERFACE_TYPE_DISPLAYPORT,
    INTERFACE_TYPE_PROPRIETORY_DIGITAL_INTERFACE,
} DID_INTERFACE_TYPE;

typedef enum _DID_CONTENT_PROTECTION_TYPE
{
    CONTENT_PROTECTION_NONE = 0,
    CONTENT_PROTECTION_HDCP,
    CONTENT_PROTECTION_DTCP,
    CONTENT_PROTECTION_DPCP,
} DID_CONTENT_PROTECTION_TYPE;

typedef enum _DID_SPREAD_SUPPORTED_TYPE
{
    SPREAD_SUPPORTED_NONE = 0,
    SPREAD_SUPPORTED_DOWN_SPREAD,
    SPREAD_SUPPORTED_CENTER_SPREAD,
} SPREAD_SUPPORTED_TYPE;

typedef struct _DISPLAYID_DISPLAY_INTERFACE
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    union // Byte 0
    {
        DDU8 InterfaceTypeNumberOfLinks; // Interface Type and # of links
        struct
        {
            DDU8 NumberOfLinks : 4; // Num links (e.g. NumLinks for Dual channel DVI = 2. For DP it can tell about # of data lines)
            DDU8 InterfaceType : 4; // Interface type of type DID_INTERFACE_TYPE
        };
    };
    union // Byte 1
    {
        DDU8 InterfaceStandVerReleaseNum; // Interface Standard Version and Release Number
        struct
        {
            DDU8 InterfaceStandardRevision : 4; // Interface Standard Revision
            DDU8 InterfaceStandardVersion : 4;  // Interface Standard Version
        };
    };

    // Bytes 2, 3, 4
    DDU8 ColorBitDepthRGBEncoding;      // Supported Interface Color Depth for RGBEncoding of type DD_BPC_SUPPORTED mask
    DDU8 ColorBitDepthYCbCr444encoding; // Supported Interface Color Depth for YCbCr444 Encoding of type DD_BPC_SUPPORTED mask
    DDU8 ColorBitDepthYCbCr422encoding; // Supported Interface Color Depth for YCbCr422 Encoding of type DD_BPC_SUPPORTED mask

    union // Byte 5
    {
        DDU8 SupportedContentProtection;
        struct
        {
            DDU8 ContentProtectionSupport : 3; // Supported Content Protection of type DID_CONTENT_PROTECTION_TYPE
            DDU8 UNIQUENAME(Rsvd) : 5;         // Reserved
        };
    };

    union // Byte 6
    {
        DDU8 ContentProtectionVerionRevision;

        struct
        {
            DDU8 ContentProtectionRevision : 4; // Content Protection Revision
            DDU8 ContentProtectionVersion : 4;  // Content Protection Version
        };
    };

    union // Byte 7
    {
        DDU8 SpreadSpectrumInformation;
        struct
        {
            DDU8 SpreadPercentage : 4;      // Spread Percentage, Spread Spectrum Percentage = (Bits[3:0]/10)% (Range is from 0 to 1.5%)
            DDU8 UNIQUENAME(Rsvd) : 2;      // Reserved
            DDU8 TypeOfSpreadSupported : 2; // Type of Spread Supported, of type SPREAD_SUPPORTED_TYPE
        };
    };

    DDU8 VoltageSupportColorMapping; // Byte 8   [LVDS(Generic) specific Interface, i.e. when InterfaceType == INTERFACE_TYPE_LVDS]
    DDU8 TimingSignalSettings;       // Byte 9   [LVDS(Generic) specific Interface, i.e. when InterfaceType == INTERFACE_TYPE_LVDS]
} DISPLAYID_DISPLAY_INTERFACE;

//-----------------------------------------------------------------------------
//
//  Stereo Display Interface Data Block
//  Tag ID: 0x10 - DID_DATA_BLOCK_STEREO_DISPLAY_INTERFACE
//  Size - Variable size block
//
//  S3D is planned to be de-featured going forward, so there is no plan to implement this
//
//-----------------------------------------------------------------------------
typedef struct _DISPLAYID_STEREO_DISPLAY_INTERFACE
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    // Stereo Interface Playload
    DDU8 Data; // Yet to be defined
} DISPLAYID_STEREO_DISPLAY_INTERFACE;

//-----------------------------------------------------------------------------
//
//  Tiled Display Topology Data Block
//  Tag ID: 0x12 - DID_DATA_BLOCK_TILED_TOPOLOGY
//  Size - 22 byte Data Block
//
//-----------------------------------------------------------------------------
#define SIZE_OF_TILED_DISPLAY_BLOCK 22     // Full block size
#define MIN_SIZE_OF_TILED_DISPLAY_BLOCK 13 // Block without Pnp ID details

typedef enum _DID_TILED_DISPLAY_SCALING_TYPE
{
    SCALING_TYPE_NONE = 0,
    SCALING_TYPE_NO_STRETCH,
    SCALING_TYPE_STRETCH_ENTIRE_DISPLAY,
    SCALING_TYPE_CLONE_OTHER_DISPLAYS,
    SCALING_TYPE_MAX,
} DID_TILED_DISPLAY_SCALING_TYPE;

typedef struct _DISPLAYID_TILED_DISPLAY_TOPOLOGY
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    union // Byte 0
    {
        DDU8 TiledDisplayTileCapsInfo;
        struct
        {
            DDU8 TiledDisplayBehavior : 3; // Behavior of this tile when it is the only tile receiving an image from source,
            // of type DID_TILED_DISPLAY_SCALING_TYPE
// For a tile display consisting of multiple physical display enclosures, this bit must be 1
            DDU8 TiledDisplayExtBehavior : 2; // Behavior when more than 1 tile and less than total number of tiles are driven by the source
            // Note: When the total number of tiles is 2, then this field (Bits 4:3) becomes “don’t care”
            DDU8 UNIQUENAME(Rsvd) : 1; // Reserved
            // 1 = Tile Bezel Information descriptor present; [0xE-0xB] and Pixel Multiplier[0xA] are non 0
            DDU8 TileBezelInfoPresent : 1; // 0 = Tile Bezel Information descriptor not present [0xE-0xB] and Pixel Multiplier[0xA] == 0
            DDU8 TileDisplayEnclosure : 1; // 0 = Display has multiple physical display enclosures, 1 = Single physical display enclosure
        };
    };

    union // Byte 1
    {
        DDU8 TiledDisplayTopology;
        struct
        {
            DDU8 NumVTilesLowBits : 4; // Total Number of Vertical Tiles (Low Bits [3:0]) minus 1 [1 → 64]
            DDU8 NumHTilesLowBits : 4; // Total Number of Horizontal Tiles (Low Bits [3:0]) minus 1 [1 → 64]
        };
    };

    union // Byte 2
    {
        DDU8 TiledDisplayLocation;
        struct
        {
            DDU8 VTileLocationLowBits : 4; // Vertical Tile Location (Low Bits [3:0]) minus 1 [1 → 64]
            DDU8 HTileLocationLowBits : 4; // Horizontal Tile Location (Low Bits [3:0]) minus 1 [1 → 64]
        };
    };

    union // Byte 3
    {
        DDU8 TiledDisplayTopologyLocationHighBits;
        struct
        {
            DDU8 VTileLocationHighBits : 2; // Vertical Tile Location (High Bits [5:4])
            DDU8 HTileLocationHighBits : 2; // Horizontal Tile Location (High Bits [5:4])
            DDU8 NumVTilesHighBits : 2;     // Total Number of Vertical Tiles (High Bits [5:4])
            DDU8 NumHTilesHighBits : 2;     // Total Number of Horizontal Tiles (High Bits [5:4])
        };
    };

    // Bytes 4, 5, 6, 7 -- Tile Size

    DDU16 TiledDisplayHorSize; // Tiled display Horizontal size at native resolution minus 1 [1 → 65,536 Lines]
    DDU16 TiledDisplayVerSize; // Tiled display Vertical size at native resolution minus 1 [1 → 65,536 Lines]

    // Byte 8 Tile Pixel Multiplier
    DDU8 PixelMultilier; // Pixel multipler

    union // Bytes 9, 10, 11, 12 -- Tile Bezel Information
    {
        DDU32 TileBezelInformation;
        struct
        {
            DDU8 TopBezelsize;    // Top Bezel in pixels = (Pixel Multiplier x Top Bezel Size x 0.1)
            DDU8 BottomBezelsize; // Bottom Bezel in pixels = (Pixel Multiplier x Bottom Bezel Size x 0.1)
            DDU8 RightBezelsize;  // Right Bezel in pixels = (Pixel Multiplier x Right Bezel Size x 0.1)
            DDU8 LeftBezelsize;   // Left Bezel in pixels = (Pixel Multiplier x Left Bezel Size x 0.1)
        };
    };

    // Bytes 13 to 22 -- Tiled Display Topology ID Descriptor

    DDU8 ManufacturerID[3]; // Tiled Display Vendor ID
    DDU8 ProductID[2];      // Tiled Display Product Code
    DDU8 SerialNumber[4];   // Tiled Display Serial Number
} DISPLAYID_TILED_DISPLAY_TOPOLOGY;

//-----------------------------------------------------------------------------
//
//  CEA Data Block within a DisplayID Data Block
//  Tag ID: 0x81 - DID_DATA_BLOCK_CEA_SPECIFIC
//  Size - Variable size block
//
//-----------------------------------------------------------------------------
typedef struct _DID_CEA_BLOCK
{
    union
    {
        DDU8 CeaBlockTagCodeLength;
        struct
        {
            DDU8 CeaBlocLength : 5;   // CEA Block Tag Code
            DDU8 CeaBlockTagCode : 3; // CEA block length
        };
    };

    // Pointer to CEA block variable length data
    DDU8 Data; // Defined to get the pointer to CEA block variable length data
} DID_CEA_BLOCK;

typedef struct _DISPLAYID_CEA_EXTN_BLOCK
{
    DISPLAYID_BLOCK BlockHeader; // Data Block header

    // Multiple CEA blocks can be present within Display ID block
    DID_CEA_BLOCK CeaDataBlock; // Defined to get the pointer to first CEA data block
} DISPLAYID_CEA_EXTN_BLOCK;

#pragma pack()
