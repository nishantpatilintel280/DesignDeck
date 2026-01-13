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
 * @file  EDIDStructs.h
 * @brief This file contains all data structures used from EDID spec.
 *
 */

#pragma once

 //-----------------------------------------------------------------------------
 //
 // Generic CONSTANTS/Includes
 //
 //-----------------------------------------------------------------------------

#define BYTE_LENGTH 0x08

//-----------------------------------------------------------------------------
//
// CONSTANTs as per EDID spec
//
//-----------------------------------------------------------------------------

// Macros for EDID Revision and Version
#define EDID_HEADER_VERSION 0x00
#define EDID_VERSION_1_2 0x12
#define EDID_VERSION_1_3 0x13
#define EDID_VERSION_1_4 0x14

#define MAX_EDID_EXTENSIONS 255 // Max EDID blocks minus Block 0, as per 1.4 spec
#define NUM_STANDARD_TIMING 8

// Maximum supported EDID extentions - if required increment in future
// Notes: In that case make sure you run PreFast to check for stack overflow
// Preferred stack threshold size is 1024 bytes

#define EDID14_MAX_MONITOR_DESCRIPTORS 0x03

#define EST_TIMINGS_III_BLOCK_DATA_LENGTH 0x06

#define MONITOR_NAME_LENGTH 13

#define EDID_CHROMA_BYTES 10

#define EDID_MAX_SUPPORTED_PIXEL_CLOCK_HZ 655350000 ///< Edid Max Supported Pixel Clock in Hz
#define EDID_MAX_SUPPORTED_HACTIVE 4095             ///< Edid Max Supported Horizontal Active Pixels
#define EDID_MAX_SUPPORTED_HBLANK 4095              ///< Edid Max Supported Horizontal Blanking Pixel
#define EDID_MAX_SUPPORTED_HSYNC 1023               ///< Edid Max Supported Horizontal Sync Pulse Width
#define EDID_MAX_SUPPORTED_HFRONTPORCH 1023         ///< Edid Max Supported Horizontal Front Porch
#define EDID_MAX_SUPPORTED_VACTIVE 4095             ///< Edid Max Supported Vertical Active Lines
#define EDID_MAX_SUPPORTED_VBLANK 4095              ///< Edid Max Supported Vertical Blanking Lines
#define EDID_MAX_SUPPORTED_VSYNC 63                 ///< Edid Max Supported Vertical Sync Pulse Width
#define EDID_MAX_SUPPORTED_VFRONTPORCH 63           ///< Edid Max Supported Vertical Front Porch

//-----------------------------------------------------------------------------
//
//  EDID Structure definitions -- START
//
//-----------------------------------------------------------------------------
#pragma pack(1)

#define EDID_HEADER_SIZE 8

// EDID Header: 8 bytes
// Bytes 0 - 7: EDID Header
static const DDU8 BaseEdidHeader[EDID_HEADER_SIZE] = { 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00 };

// Vendor and Product Identification: 10 bytes
typedef union _EDID_PNPPID
{
    DDU8 VendorProductID[10]; // Vendor / Product identification
    struct
    {
        DDU8 ManufacturerID[2]; // Bytes 8, 9: Manufacturer ID
        DDU8 ProductID[2];      // Bytes 10, 11: Product ID
        DDU8 SerialNumber[4];   // Bytes 12 - 15: Serial numbers
        DDU8 WeekOfManufacture; // Byte 16: Week of manufacture
        DDU8 YearOfManufacture; // Byte 17: Year of manufacture
    };
} EDID_PNPPID;

typedef enum _VIDEO_INTERFACE_SUPPORTED
{
    VIDEO_INTERFACE_UNDEFINED = 0,
    VIDEO_INTERFACE_DVI,
    VIDEO_INTERFACE_HDMI_A,
    VIDEO_INTERFACE_HDMI_B,
    VIDEO_INTERFACE_MDDI,
    VIDEO_INTERFACE_DISPLAYPORT,
} VIDEO_INTERFACE_SUPPORTED;

typedef enum _EDID_COLOR_ENCODING
{
    EDID_COLOR_RGB_444_ONLY = 0,
    EDID_COLOR_RGB_444_YCBCR_422,
    EDID_COLOR_RGB_444_YCBCR_444_422,
    EDID_COLOR_RGB_444 = 0,
} EDID_COLOR_ENCODING;

// Basic display parameters & features: 5 bytes
typedef struct _EDID_BASIC_DISPLAY_PARAMS
{
    union
    {
        DDU8 VideoInput; // Byte 20: Video input definition

        struct
        {                          // (EDID 1.3 definition)
            DDU8 SyncInput : 4;    // Sync input supported, of type VIDEO_INTERFACE_SUPPORTED
            DDU8 Setup : 1;        // Display setup
            DDU8 SigLevStd : 2;    // Signal level Standard
            DDU8 IsDigitInput : 1; // 0: Analog input-[Bits 6:1 Above defns.]; 1: Digital input -[Bits 6:1 Reserved]
        };

        struct
        {                                   // (EDID 1.4 definition)
            DDU8 DigitalVideoInterface : 4; // Digital Video Interface Standard Supported, of type VIDEO_INTERFACE_SUPPORTED
            DDU8 ColorBitDepth : 3;         // Color Bit Depth of type DD_PANEL_COLOR_DEPTH
            DDU8 IsDigitalInput : 1;        // 0: Analog input-[Bits 6:1 Reserved]; 1: Digital input -[Bits 6:1 Above defns.]
        };
    };

    DDU8 MaxHImageSize; // Byte 21: Maximum H. image size in cm; (EDID 1.4: Can be Aspect ratio if V is 0, See spec)
    DDU8 MaxVImageSize; // Byte 22: Maximum V. image size in cm; (EDID 1.4: Can be Aspect ratio if H is 0, See spec)

    DDU8 Gamma; // Byte 23: Display Transfer Characteristics(Gamma); Stored Value = (Gamma x 100) – 100

    union
    {
        DDU8 FeatureSupport; // Byte 24: Feature support

        struct
        {                                   // (EDID 1.3 definition)
            DDU8 IsGTFSupported : 1;        // GTF timing support; 0 - No, 1 - Yes
            DDU8 IsPreferredTimingMode : 1; // Preferred timing is 1st DTD 0 - No, 1 - Yes [Must be set if EDID >= 1.3]
            DDU8 ColorSpace : 1;            // Use STD RGB color space 0 - No, [1 - Yes, ColorChars should match sRGB values in EDID spec Appendix A]
            DDU8 DispType : 2;              // Display type, 0 -Monochrome, 1 - RGB Color, 2 - Non RGB multColor, 1 - Undefined
            DDU8 ActiveOff : 1;             // Active off (Display consumes less power/blanks out when it receives an out of range timing)
            DDU8 Suspend : 1;               // Suspend    (Refer VESA DPMS spec)
            DDU8 StandBy : 1;               // Stand-by    (Refer VESA DPMS spec)
        };

        struct
        {                                       // (EDID 1.4 definition), [definitions same as 1.3 named as SameAs1_3 to avoid name collision]
            DDU8 IsContinuousFreqSupported : 1; // 1 - Display supports continuous frequency
            DDU8 IsPTMSameAs1_3 : 1;            // Preferred timing is 1st DTD 0 - No, 1 - Yes [Must be set if EDID >= 1.3]
            DDU8 ColorSpaceSameAs1_3 : 1;       // Use STD RGB color space 0 - No, [1 - Yes, ColorChars should match sRGB values in EDID spec Appendix A]
            DDU8 ColorEncodingFormat : 2;       // Supported Color Encoding Format if Video Input is digital of type EDID_COLOR_ENCODING
            DDU8 ActiveOffSameAs1_3 : 1;        // Active off (Display consumes less power/blanks out when it receives an out of range timing)
            DDU8 SuspendSameAs1_3 : 1;          // Suspend    (Refer VESA DPMS spec)
            DDU8 StandBySameAs1_3 : 1;          // Stand-by    (Refer VESA DPMS spec)
        };
    };
} EDID_BASIC_DISPLAY_PARAMS;

// Phosphor or Filter Chromaticity: 10 bytes
typedef struct _EDID_CHROMATICITY_DATA
{
    union
    {
        DDU8 RedGreenLowBits; // Byte 25
        struct
        {
            DDU8 GreenYLowBits : 2;
            DDU8 GreenXLowBits : 2;
            DDU8 RedYLowBits : 2;
            DDU8 RedXLowBits : 2;
        };
    };

    union
    {
        DDU8 BlueWhiteLowBits; // Byte 26
        struct
        {
            DDU8 WhiteYLowBits : 2;
            DDU8 WhiteXLowBits : 2;
            DDU8 BlueYLowBits : 2;
            DDU8 BlueXLowBits : 2;
        };
    };

    DDU8 RedXUpperBits; // Byte 27
    DDU8 RedYUpperBits; // Byte 28

    DDU8 GreenXUpperBits; // Byte 28
    DDU8 GreenYUpperBits; // Byte 30

    DDU8 BlueXUpperBits; // Byte 31
    DDU8 BlueYUpperBits; // Byte 32

    DDU8 WhiteXUpperBits; // Byte 33
    DDU8 WhiteYUpperBits; // Byte 34
} EDID_CHROMATICITY_DATA;

// Established timings: 3 bytes
typedef struct _EDID_ESTABLISHED_TIMINGS
{
    union
    {
        DDU8 EstTimingData[3]; // bytes 35 to 37

        union
        {
            DDU8 EstTiming1; // Byte 35
            struct
            {
                DDU8 Supports800x600_60 : 1;
                DDU8 Supports800x600_56 : 1;
                DDU8 Supports640x480_75 : 1;
                DDU8 Supports640x480_72 : 1;
                DDU8 Supports640x480_67 : 1;
                DDU8 Supports640x480_60 : 1;
                DDU8 Supports720x400_88 : 1;
                DDU8 Supports720x400_70 : 1;
            };
        };
        union
        {
            DDU8 EstTiming2; // Byte 36
            struct
            {
                DDU8 Supports1280x1024_75 : 1;
                DDU8 Supports1024x768_75 : 1;
                DDU8 Supports1024x768_70 : 1;
                DDU8 Supports1024x768_60 : 1;
                DDU8 Supports1024x768_87i : 1;
                DDU8 Supports832x624_75 : 1;
                DDU8 Supports800x600_75 : 1;
                DDU8 Supports800x600_72 : 1;
            };
        };
        union
        {
            DDU8 MfgTimings; // Byte 37
            struct
            {
                DDU8 MfgReservedTimings : 7;
                DDU8 Supports1152x870_75 : 1;
            };
        };
    };
} EDID_ESTABLISHED_TIMINGS;

typedef struct _ASPECT_RATIO
{
    DDU16 XRatio;
    DDU16 YRatio;
} ASPECT_RATIO;

// Standard Timing Indentification - 8 * 2 = 16 bytes of data,
typedef union _EDID_STD_TIMING
{
    DDU16 StdTiming; // Bytes 38 - 53

    struct
    {
        DDU8 HActive; // (HActive/8) - 31; Range of HActive pixels ==> 256 → 2288, in increments of 8 pixels.
        struct
        {
            DDU8 RefreshRate : 6; // Refresh Rate - Range 60 → 123
            DDU8 AspectRatio : 2; // Aspect ratio (HActive/VActive); values are as per EdidAspectRatioTable
        };
    };
} EDID_STD_TIMING;

// The following structure groups the 12 bytes which are
// Common between  EDID_DTD_TIMING and EDID_S3D_PRIVATE_TIMING
typedef struct _COMMON_TIMING_PART
{
    DDU16 PixelClock; // Byte0: Pixel clock / 10000

    DDU8 HActiveLowBits; // Byte1: Lower 8 bits of H. active pixels
    DDU8 HBlanklowBits;  // Byte2: Lower 8 bits of H. blanking

    union
    {
        DDU8 HActiveHBlankHighBits; // Byte3:
        struct
        {
            DDU8 HBlankHighBits : 4;  // Upper 4 bits of H. blanking
            DDU8 HActiveHighBits : 4; // Upper 4 bits of H. active pixels
        };
    };

    DDU8 VActiveLowBits; // Byte4: Lower 8 bits of V. active lines
    DDU8 VBlankLowBits;  // Byte5: Lower 8 bits of V. blanking

    union
    {
        DDU8 VActiveVBlankHighBits;
        struct
        {
            DDU8 VBlankHighBits : 4;  // Byte6: Upper 4 bits of V. blanking
            DDU8 VActiveHighBits : 4; // Byte7: Upper 4 bits of V. active pixels
        };
    };

    DDU8 HSyncOffsetLowBits;     // Byte8: Lower 8 bits of H. sync offset
    DDU8 HSyncPulseWidthLowBits; // Byte9: Lower 8 bits of H. sync pulse width

    union
    {
        DDU8 VSyncOffsetVSyncPulseWidthLowBits; // Byte10:
        struct
        {
            DDU8 VSyncPulseWidthLowBits : 4; // Lower 4 bits of V. sync pulse width
            DDU8 VSyncOffsetLowBits : 4;     // Lower 4 bits of V. sync offset
        };
    };
    union
    {
        DDU8 HSyncVSyncHighBits; // Byte11:
        struct
        {
            DDU8 VSyncPulseWidthHighBits : 2; // Upper 2 bits of V. sync pulse width
            DDU8 VSyncOffsetHighBits : 2;     // Upper 2 bits of V. sync offset
            DDU8 HSyncPulseWidthHighBits : 2; // Upper 2 bits of H. sync pulse width
            DDU8 HSyncOffsetHighBits : 2;     // Upper 2 bits of H. sync offset
        };
    };
} COMMON_TIMING_PART;

// EDID 1.4 indicated s3d format support by display DTD
typedef enum _DD_DISP_S3DFORMAT
{                                 // Bits   -- 6 5 0
    NO_STEREO,                    // 0 or 1 -- 0 0 x Normal display, no stereo. The value of bit 0 is "don't care"
    FIELD_SEQUENTIAL_RIGHT = 2, // 2      -- 0 1 0 Field sequential stereo, right image when stereo sync. = 1
    FIELD_SEQUENTIAL_LEFT = 4, // 4      -- 1 0 0 Field sequential stereo, left image when stereo sync. = 1
    INTERLEAVED_TWOWAY_RIGHT = 3, // 3      -- 0 1 1 2-way interleaved stereo, right image on even lines
    INTERLEAVED_TWOWAY_LEFT = 5, // 5      -- 1 0 1 2-way interleaved stereo, left image on even lines
    INTERLEAVED_FOURWAY = 6, // 6      -- 1 1 0 4-way interleaved stereo
    INTERLEAVED_SIDE_BY_SIDE = 7  // 7      -- 1 1 1 Side-by-Side interleaved stereo
} DD_DISP_S3DFORMAT;

typedef enum _SYNC_CONF
{
    ANALOG_COMPOSITE = 0,
    BIPOLAR_ANALOG_COMPOSITE,
    DIGITAL_COMPOSITE,
    DIGITAL_SEPERATE,
} SYNC_CONF;

// 18-byte DTD block
typedef struct _EDID_DTD_TIMING
{
    COMMON_TIMING_PART Timing; // Common Timing

    DDU8 HImageSizeLowBits; // Lower 8 bits of H. image size in mm
    DDU8 VImageSizeLowBits; // Lower 8 bits of V. image size in mm
    union
    {
        DDU8 HImageSzVImageSzHighBits;
        struct
        {
            DDU8 VImageSzHighBits : 4; // Upper 4 bits of V. image size
            DDU8 HImageSzHighBits : 4; // Upper 4 bits of H. image size
        };
    };

    DDU8 HBorder; // H. border in pixels
    DDU8 VBorder; // V. border in pixels

    union
    {
        DDU8 Flags; // Hsync & Vsync polarity, etc. flags

        struct
        {
            DDU8 Stereo1 : 1;       // Stereo definition with bit[6:5], of type DISP_S3DFORMAT
            DDU8 HSyncPolarity : 1; // Hsync polarity (0: Neg, 1: Pos)
            DDU8 VSyncPolarity : 1; // Vsync polarity (0: Neg, 1: Pos)
            DDU8 SyncConf : 2;      // Sync configuration, of type SYNC_CONF
            DDU8 Stereo2 : 2;       // Stereo definition, of type DISP_S3DFORMAT
            DDU8 IsInterlaced : 1;  // Interlaced / Non-interlaced; 0 : Non-interlaced, 1 : Interlaced
        };
    };
} EDID_DTD_TIMING;

typedef enum _MONITOR_DESCRIPTOR_TAG
{
    DISPLAY_S3D_PRIVATE_TIMING = 0x05,
    DISPLAY_DUMMY_DESCRIPTOR = 0x10,
    DISPLAY_ESTABLISHED_TIMING3 = 0xF7,
    DISPLAY_CVT_3BYTE_TIMING_CODE,
    DISPLAY_COLOR_MANAGEMENT_DATA,
    DISPLAY_STD_TIMING_ID,
    DISPLAY_COLOR_POINT_DATA,
    DISPLAY_MONITOR_NAME,
    DISPLAY_RANGE_LIMITS,
    DISPLAY_ASCII_STRING,
    DISPLAY_PRODUCT_SERIAL_NUMBER,
} MONITOR_DESCRIPTOR_TAG;

typedef struct _MONITOR_DESCRIPTOR_HEADER
{
    DDU16 Flag0; // Reserved: Should be "0x00" when block is used as descriptor
    DDU8 Flag1;  // Reserved: Should be "0x00" when block is used as descriptor

    DDU8 DataTypeTag; // Monitor descriptor Tag, of type MONITOR_DESCRIPTOR_TAG

    DDU8 Flag2; // Reserved: Should be "0x00" when block is used as descriptor

} MONITOR_DESCRIPTOR_HEADER;

typedef enum _TIMING_FORMULA
{
    DEFAULT_GTF_SUPPORTED = 0,
    RANGE_LIMITS_ONLY,
    SECONDARY_GTF_SUPPORTED,
    CVT_TIMING_SUPPORTED,
} TIMING_FORMULA;

// Display Range Limits Offset Flags.
// Applicable only from EDID 1.4 onwards
typedef union _EDID_RANGE_LIMITS_FLAGS
{
    DDU8 RangeLimitOffsetFlags; // Range Limits Offset Flags
    struct
    {
        DDU8 VerticalRateOffset : 2;   // Vertical Rate Offset
        DDU8 HorizontalRateOffset : 2; // Horizontal Rate Offset
        DDU8 Reserved : 4;             // Reserved.
    };
} EDID_RANGE_LIMITS_FLAGS;

//    Monitor range limits
typedef struct _MONITOR_RANGE_LIMITS
{
    // bytes 5 - 10
    DDU8 MinVFrequency;        // Min Vertical Rate,in Hz
    DDU8 MaxVFrequency;        // Max Vertical Rate, in Hz
    DDU8 MinHFrequency;        // Min Horizontal Rate, in Hz
    DDU8 MaxHFrequency;        // Max Horizontal Rate, in Hz
    DDU8 MaxPixelClock;        // Max Pixel Clock, Represented as PixelClock/10 Mhz
    DDU8 TimingFormulaSupport; // Timing formular supported, of type TIMING_FORMULA

    union // If TimingFormulaSupport is SECONDARY_GTF_SUPPORTED
    {
        struct
        {                          // byte 11 - 17
            DDU8 UNIQUENAME(Rsvd); // 00h
            DDU8 StartFreq;        // Horizontal Freq, Represented as Value/2 KHz
            DDU8 ByteC;            // C Value, Represented as C * 2
            DDU8 LSBOfMValue;      // LSB of M Value
            DDU8 MSBOfMValue;      // MSB of M Value
            DDU8 ByteK;            // K Value
            DDU8 ByteJ;            // J Value, Represented as J * 2
        };

        struct
        {                          // If TimingFormulaSupport is CVT_TIMING_SUPPORTED
            DDU8 CVTVersionNumber; // byte 11, CVT Standard Version Number: e.g. ‘11h’ implies “Version 1.1”
            union
            {
                DDU8 AdditionalPixelcClockPrecision; // byte 12
                struct
                {
                    DDU8 MaxActivePixelsMSB : 2; // bit 1:0, range is 00..11
                    DDU8 AddnPixelClock : 6;     // bit 7:2, 6bits of extra pixel clock for 0.25MHz accuracy
                };
            };

            DDU8 MaxActivePixelsLSB; // byte 13, 0 - No limit

            union
            {
                DDU8 SupportedAspectRatio; // byte 14
                struct
                {
                    DDU8 UNIQUENAME(Rsvd) : 3;
                    DD_BOOL SupportsAR_15_9 : 1;
                    DD_BOOL SupportsAR_5_4 : 1;
                    DD_BOOL SupportsAR_16_10 : 1;
                    DD_BOOL SupportsAR_16_9 : 1;
                    DD_BOOL SupportsAR_4_3 : 1;
                };
            };
            union
            {
                DDU8 AspectRatioCvtBlanking; // byte 15
                struct
                {
                    DDU8 UNIQUENAME(Rsvd) : 3;
                    DD_BOOL CVTStdBlankSupport : 1;
                    DD_BOOL CVTRedBlankSupport : 1;
                    DDU8 PreferredAR : 3;
                };
            };
            union
            {
                DDU8 DisplayScalingSupported; // byte 16
                struct
                {
                    DDU8 UNIQUENAME(Rsvd) : 4;
                    DD_BOOL VertStretch : 1;
                    DD_BOOL VertShrink : 1;
                    DD_BOOL HorStretch : 1;
                    DD_BOOL HorShrink : 1;
                };
            };

            DDU8 PreferredVerticalRefreshRate; // byte 17
        };
    };
} MONITOR_RANGE_LIMITS;

// Color point
typedef struct _COLOR_POINT
{
    DDU8 WhitePointIndexNumber_1;
    DDU8 WhiteLowBits_1;
    DDU8 White_x_1;
    DDU8 White_y_1;
    DDU8 WhiteGamma_1;
    DDU8 WhitePointIndexNumber_2;
    DDU8 WhiteLowBits_2;
    DDU8 White_x_2;
    DDU8 White_y_2;
    DDU8 WhiteGamma_2;
    DDU8 Byte_15;
    DDU8 Byte_16_17[2];
} COLOR_POINT;

// Structure definition for Established Timings III monitor block
typedef struct _EST_TIMINGS_III_BLOCK
{
    DDU8 VesaDMTVersion;      // Byte 5 : Indicates VESA DMT Version.
    DDU8 TimingBitMask[6];    // Bytes 6 - 11: Indicates Timing Bit Mask defined for Est Timing III.
    DDU8 UNIQUENAME(Rsvd)[6]; // Bytes 12 - 17: bytes are reserved
} EST_TIMINGS_III_BLOCK;

// Private S3D timing block (total = 13 bytes)
typedef struct _S3D_PRIVATE_TIMING
{
    COMMON_TIMING_PART Timing; // Bytes 6 - 16: Common Timing part between EDID and Private S3D timings
    DDU8 Flags;                // Byte 17: Reserved should be 0
} S3D_PRIVATE_TIMING;

// 18-byte Monitor descriptor block
typedef struct _MONITOR_DESCRIPTOR
{
    MONITOR_DESCRIPTOR_HEADER MonitorDescHeader; // Bytes 0 - 4: Header

    union
    { // 13bytes: Bytes 5 - 17: Data Changes based on Tag; See below

        DDU8 MonitorSerialNumber[13]; // Monitor S/N; Tag = DISPLAY_PRODUCT_SERIAL_NUMBER

        DDU8 ASCIIString[13]; // ASCII string; Tag = DISPLAY_ASCII_STRING

        MONITOR_RANGE_LIMITS MonitorRangeLimits; // Monitor range limits; Tag = DISPLAY_RANGE_LIMITS

        DDU8 MonitorName[MONITOR_NAME_LENGTH]; // Monitor name; Tag = DISPLAY_MONITOR_NAME

        COLOR_POINT ColorPoint; // Color point data; Tag = DISPLAY_COLOR_POINT_DATA

        struct
        { // Standard timings, Tag = DISPLAY_STD_TIMING_ID
            EDID_STD_TIMING ExtraStdTiming[6];
            DDU8 FixedValueOfA0; // Should be 0xA0
        };

        EST_TIMINGS_III_BLOCK EstTimingsIIIBlock; // Established Timings III BLOCK; Tag = DISPLAY_ESTABLISHED_TIMING3

        S3D_PRIVATE_TIMING S3DPrivateTiming; // S3d Private Timing; Tag = DISPLAY_S3D_PRIVATE_TIMING

        // Manufacturer specific value (ucDataTypeTag = 0F-00)
        DDU8 MfgSpecificData[13];
    };
} MONITOR_DESCRIPTOR;

// Detailed timing/Monitor descriptor section - 72 bytes (4 * 18 bytes)
typedef struct _EDID_MONITOR_DESCRIPTORS
{
    union
    { // Bytes 54 - 126

        EDID_DTD_TIMING DTD[MAX_EDID_DTD_BLOCKS]; // Four DTD data blocks

        struct
        { // (EDID 1.3 definition)
            union
            {
                EDID_DTD_TIMING Edid_13_DTD[MAX_EDID_DTD_BLOCKS];            // Four DTD data blocks
                // or
                MONITOR_DESCRIPTOR Edid_13_MonitorInfo[MAX_EDID_DTD_BLOCKS]; // For Monitor descriptor Blocks
            };
        };

        struct
        {                                       // (EDID 1.4 definition)
            EDID_DTD_TIMING PreferedTimingMode; // Mandatory 18 byte Detailed Timing descriptor

            union
            {
                EDID_DTD_TIMING Edid_14_DTD[MAX_EDID_DTD_BLOCKS - 1];            // Three DTD data blocks
                // or
                MONITOR_DESCRIPTOR Edid_14_MonitorInfo[MAX_EDID_DTD_BLOCKS - 1]; // Three Monitor Descriptor blocks
            };
        };
    };

} EDID_MONITOR_DESCRIPTORS;

//-----------------------------------------------------------------------------
//
//    128-byte EDID block0 structure (Includes both EDID 1.x and EDID 1.4)
//
//-----------------------------------------------------------------------------
typedef struct _EDID_BASE_BLOCK
{
    // Header: 8 bytes
    DDU8 Header[EDID_HEADER_SIZE]; // Bytes 0 - 7: EDID Header of type "BaseEdidHeader"

    EDID_PNPPID PnpID; // Bytes 8 - 17: Vendor and Product Identification

    DDU8 Version;  // Byte 18: EDID version no.
    DDU8 Revision; // Byte 19: EDID revision no.

    EDID_BASIC_DISPLAY_PARAMS BasicDisplayParams; // Bytes 20 - 24: Basic display parameters & features

    EDID_CHROMATICITY_DATA ChromaticityData; // Bytes 25 - 34: Phosphor or Filter Chromaticity

    EDID_ESTABLISHED_TIMINGS EstablishedTimings; // Bytes 35 - 37: Established timings

    EDID_STD_TIMING StdTiming[NUM_STANDARD_TIMING]; // Bytes 38 - 53: Standard timings

    EDID_MONITOR_DESCRIPTORS EdidTiming; // Bytes 54 - 126: Detailed timing section - 72 bytes (4 * 18 bytes)

    DDU8 NumExtBlocks; // Byte 126: Number of extension EDID blocks
    DDU8 Checksum;     // Byte 127: Checksum of the EDID block
} EDID_BASE_BLOCK;

//-----------------------------------------------------------------------------
//
//  EDID Structure definitions -- END
//
//-----------------------------------------------------------------------------

// Edid extension tags defined in EDID spec/Display ID spec
typedef enum _EDID_EXTN_TAG
{
    CEA_EXT_TAG = 0x02,      // CEA-EXT: CEA 861 Series Extension, Refer to the latest revision of the CEA 861 Standard
    VTB_EXT_TAG = 0x10,      // VTB-EXT: Video Timing Block Extension
    EDID_2_0_EXT_TAG = 0x20,      // EDID_2_0-EXT: EDID 2.0 Extension Tag. Refer EDID 1.3 Spec.
    DI_EXT_TAG = 0x40,      // DI-EXT: Display Information Extension
    LS_EXT_TAG = 0x50,      // LS-EXT: Localized String Extension
    DPVL_EXT_TAG = 0x60,      // DPVL-EXT: Digital Packet Video Link Extension
    DID_EXT_TAG = 0x70,      // DID-EXT: Display ID used as EDID extension
    BLOCK_MAP_EXT_TAG = 0xF0,      // Block Map Tag. Currently, we are not parsing Block Map.
    // This is be'coz we are parsing all Exts.(except Block Map) without any conditions.
    // & Also No. of Exts. are less in EDID. So, parsing Block Map doesn't add much value.
    MANF_SPECIFIED_EXT_TAG = 0xFF, // EXTENSIONS defined by the display manufacturer
} EDID_EXTN_TAG;

//-----------------------------------------------------------------------------
//
// CONSTANTs as per CEA spec
//
//-----------------------------------------------------------------------------
#define MAX_VIC_DEFINED 50    // Only upto 50 VIC modes we will support internally
#define MAX_MODES_DEFINED 200 // if we see > 200 modes in total in any EDID block in future, will need to increase this
#define CEA_SHORT_VIDEO_DESCRIPTOR_CODE_MASK 0x7F
#define CEA_MAX_VIC_SUPPORTING_NATIVE_FORMAT 64
#define CEA_NATIVE_FORMAT_BIT_MASK 0x80

#define CEA_HDMI_IEEE_REG_ID 0x00000C03
#define CEA_HDMI2_IEEE_REG_ID 0x00C45DD8 // The IEEE OUI ID is same for HDMI2.0 & HDMI2.1

#define CEA_EDID_HEADER_SIZE 0x04

#define CE_COLORIMETRY_PROFILE_MASK 0xF

#define IS_VIC_IN_FORBIDDEN_RANGE(x) ((((x) > 127) && ((x) < 193)) ? TRUE : FALSE)
#define GET_VIC_INDEX_FOR_VIC_RANGE_193_219(x) (((x) <= MAX_CE_VICID) ? ((x)-65) : (-1))
#define GET_VIC_INDEX(x) ((IS_VIC_IN_FORBIDDEN_RANGE(x)) ? (-1) : GET_VIC_INDEX_FOR_VIC_RANGE_193_219(x)) // Ignore 128-192 VICs - Forbidden

#define GET_HDMI_4K2K_VIC(x) ((x == 95) ? 1 : (x == 94) ? 2 : (x == 93) ? 3 : (x == 98) ? 4 : 0) // This Macro fetches the array index to correct timing from global timings array g_4kx2kModes[]

//-----------------------------------------------------------------------------
//
//  CEA Structure definitions -- START
//
//-----------------------------------------------------------------------------

typedef enum _CEA_EXTN_VERSION
{
    CEA_861_VERSION_1 = 0x01, // Supported for legacy features
    CEA_861_A_VERSION = 0x02, // Not supported
    CEA_861_B_VERSION = 0x03, // All CE extensions 861-B onwards will have version set as 3.
} CEA_EXTN_VERSION;

// CEA Extn Block Capability structure, Valid from CEA_861_B_VERSION onwards
typedef union _CEA_EXT_CAPS
{
    DDU8 Value;
    struct
    {
        DDU8 TotalNativeDTDs : 4;     // Total number of DTDs in extension block
        DDU8 SupportsYCBCR422 : 1;    // Indicates support for YCBCR 4:2:2
        DDU8 SupportsYCBCR444 : 1;    // Indicates support for YCBCR 4:4:4
        DDU8 SupportsBasicAudio : 1;  // Indicates support for Basic audio
        DDU8 UnderscansITFormats : 1; // Indicates underscan behavior of IT formats
    };
} CEA_EXT_CAPS;

// CE extension structure
typedef struct _CE_EDID_EXTN_BLOCK
{
    DDU8 CeTag;             // CE Tag value; Should be "CEA_EXT_TAG"  0x02
    DDU8 Revision;          // CE extension version, of type CEA_EXTN_VERSION
    DDU8 DTDOffset;         // Detailed Timing descriptor Offset
    CEA_EXT_CAPS Capabilty; // CEA Extn Caps, valid from CEA_861_B_VERSION onwards
    DDU8 CeBlockData[123];  // CE Data Blocks, starts with CE_BLK_HEADER
    DDU8 CheckSum;          // EDID checksum
} CE_EDID_EXTN_BLOCK;

typedef enum _CEA_TAGCODE
{
    CEA_TAG_CODE_RESERVED = 0,
    CEA_AUDIO_DATABLOCK,
    CEA_VIDEO_DATABLOCK,
    CEA_VENDOR_DATABLOCK,
    CEA_SPEAKER_DATABLOCK,
    CEA_VESA_DISPLAY_TRANF_CHAR_DATABLOCK,
    CEA_TAG_CODE_RESERVED1,
    CEA_USE_EXTENDED_TAG
} CEA_TAGCODE;

// CE Base Block header structure
typedef union _CE_BLK_HEADER
{
    DDU8 Value;
    struct
    {
        DDU8 Length : 5; // Length of CE Data block
        DDU8 Tag : 3;    // CE block base Tag of type CEA_TAGCODE
    };
} CE_BLK_HEADER;

typedef enum _CEA_EXT_TAGCODE
{
    CEA_VIDEO_CAP_DATABLOCK = 0x0,
    CEA_VENDOR_SPECIFIC_VIDEO_DATABLOCK,
    CEA_VESA_DISPLAY_DEVICE_DATABLOCK,
    CEA_VIDEO_TIMING_BLOCK_EXTN,
    CEA_RSVD_HDMI_VSDB,
    CEA_COLORIMETRY_DATABLOCK,
    CEA_HDR_STATIC_META_DATABLOCK,
    CEA_VIDEO_FORMAT_PREFERENCE_DATABLOCK = 13,
    CEA_420_VIDEO_DATABLOCK,
    CEA_420_CAPABILITY_MAP_DATABLOCK,
    CEA_TYPE_VII_VIDEO_TIMING_DATA_BLOCK = 34,
    CEA_TYPE_VIII_VIDEO_TIMING_DATA_BLOCK = 35,
    CEA_TYPE_X_VIDEO_TIMING_DATA_BLOCK = 42,
    CEA_HF_EEODB_DATABLOCK = 120,
    CEA_HF_SCDB_DATABLOCK = 121,
} CEA_EXT_TAGCODE;

// CE extension Block header structure, Valid only when base Tag is set to CEA_USE_EXTENDED_TAG
typedef union _CE_EXT_BLK_HEADER
{
    DDU16 Value;
    struct
    {
        CE_BLK_HEADER CeHdr; // CE Block header
        DDU8 ExtendedTag;    // CE block extended Tag, of type CEA_EXT_TAGCODE
    };
} CE_EXT_BLK_HEADER;

//-----------------------------------------------------------------------------
//
// CE Short Audio Descriptor block
//  Tag ID: 1 - CEA_AUDIO_DATABLOCK
//  Extended Tag ID: NA:
//  Size - Variable size block -- Based on Length in header
//
//-----------------------------------------------------------------------------
typedef struct _CEA_SHORT_AUDIO_DESCR_BLOCK
{
    CE_BLK_HEADER CeHeader; // Ce base header

    // Multiple entries of Short Audio Descriptors can be present
    CEA_861B_ADB ShortAudioDesc[1]; // Defined as a 1 byte array to get the pointer to first instance of Short Audio Descriptor
} CEA_SHORT_AUDIO_DESCR_BLOCK;

//-----------------------------------------------------------------------------
//
// CE Short Video Descriptor block
//  Tag ID: 2 - CEA_VIDEO_DATABLOCK
//  Extended Tag ID: NA:
//  Size - Variable size block -- Based on Length in header
//
//-----------------------------------------------------------------------------
typedef struct _CEA_SHORT_VIDEO_DESCR_BLOCK
{
    CE_BLK_HEADER CeHeader; // Ce base header

    // Multiple entries of Short Video Descriptors can be present
    DDU8 SVDList; // Defined to get the pointer to first instance of Short Video Descriptor
} CEA_SHORT_VIDEO_DESCR_BLOCK;

//-----------------------------------------------------------------------------
//
// CE Speaker allocation block
//  Tag ID: 4 - CEA_SPEAKER_DATABLOCK
//  Extended Tag ID: NA:
//  Size - 3 bytes --
//
//-----------------------------------------------------------------------------
typedef struct _CEA_SPEAKER_ALLOCATION_BLOCK
{
    CE_BLK_HEADER CeHeader; // Ce base header

    DDU8 SpeakerAllocationBlock[3]; // 3 byte payload and only 1st byte is valid
} CEA_SPEAKER_ALLOCATION_BLOCK;

//-----------------------------------------------------------------------------
//
// CE Video Capability block
//  Tag ID: 7 - CEA_USE_EXTENDED_TAG
//  Extended Tag ID: 0: CEA_VIDEO_CAP_DATABLOCK
//  Size - 1 byte excluding Extened header (Tag byte and Ext Tag byte)
//
//-----------------------------------------------------------------------------
typedef enum _CEA_SCAN_BEHAVIOR
{
    FORMAT_NOT_SUPPORTED = 0,    // Format is not supported
    ALWAYS_OVERSCANNED,          // Format is always overscanned
    ALWAYS_UNDERSCANNED,         // Format is always underscanned
    SUPPORTS_OVER_AND_UNDERSCAN, // Sink supports both overscan and underscan
} CEA_SCAN_BEHAVIOR;

typedef union _VIDEO_CAP_INFO
{
    DDU8 Value;
    struct
    {
        DDU8 CEScanBehavior : 2;         // Indicates scan behavior of CE mode, of type CEA_SCAN_BEHAVIOR
        DDU8 ITScanBehavior : 2;         // Indicates scan behavior of IT mode, of type CEA_SCAN_BEHAVIOR
        DDU8 PTScanBehavior : 2;         // Indicates scan behavior of Preferred mode, of type CEA_SCAN_BEHAVIOR
        DDU8 IsQuantRangeSelectable : 1; // Indicates if RGB Quantization Range can be overridden
        DDU8 UNIQUENAME(Rsvd) : 1;       // Reserved
    };
} VIDEO_CAP_INFO;

typedef struct _CEA_VIDEO_CAPABILITY_BLOCK
{
    CE_EXT_BLK_HEADER CeExtHeader; // CE extension block header

    VIDEO_CAP_INFO VideoCapinfo; // Video Vapability block
} CEA_VIDEO_CAPABILITY_BLOCK;

//-----------------------------------------------------------------------------
//
// CE Colorimetry Data Block
//  Tag ID: 7 - CEA_USE_EXTENDED_TAG
//  Extended Tag ID: 5: CEA_COLORIMETRY_DATABLOCK
//  Size - 2 bytes excluding Extened header (Tag byte and Ext Tag byte)
//
//-----------------------------------------------------------------------------
typedef enum _CE_PROFILE_SUPPORT
{
    CE_PROFILE_NOT_SUPPORTED = 0,
    CE_MD0_PROFILE = (1 << 0),
    CE_MD1_PROFILE = (1 << 1),
    CE_MD2_PROFILE = (1 << 2),
    CE_MD3_PROFILE = (1 << 3),
} CE_PROFILE_SUPPORT;

typedef enum _CE_COLORIMETRY_TYPE
{
    XVYCC_601 = (1 << 0),
    XVYCC_709 = (1 << 1),
    SYCC_601 = (1 << 2),
    ADOBE_YCC601 = (1 << 3),
    ADOBE_RGB = (1 << 4),
    BT2020_C_YCC = (1 << 5),
    BT2020_YCC = (1 << 6),
    BT2020_RGB = (1 << 7),
} CE_COLORIMETRY_TYPE;

// CE colorimetry block Gamut Boundary Data support
typedef struct _CEA_EXT_COLORIMETRY_DATA_BLOCK
{
    CE_EXT_BLK_HEADER CeExtHeader; // CE extension block header

    DDU8 CEColorimetryData; // CE COLORIMETR Data, of type CE_COLORIMETRY_TYPE
    union
    {
        DDU8 Byte2;
        struct
        {
            DDU8 CEGamutBoundaryDataProfile : 3; // For Future gamut-related metadata, of type CE_PROFILE_SUPPORT
            DDU8 UNIQUENAME(Rsvd) : 5;           // Reserved
        };
    };
} CEA_EXT_COLORIMETRY_DATA_BLOCK;

//-----------------------------------------------------------------------------
//
// HDR Static MetaData block
//  Tag ID: 7 - CEA_USE_EXTENDED_TAG
//  Extended Tag ID: 6: CEA_HDR_STATIC_META_DATABLOCK
//  Size - 2 bytes
//
//-----------------------------------------------------------------------------
typedef enum _EOTF_SUPPORT
{
    EOTF_TRADITIONAL_GAMMA_SDR = (1 << 0),
    EOTF_TRADITIONAL_GAMMA_HDR = (1 << 1),
    EOTF_SMPTE_ST2084 = (1 << 2),
    EOTF_FUTURE_EOTF = (1 << 3),
} EOTF_SUPPORT;

typedef enum _HDR_STATIC_METADATA_TYPE
{
    HDR_STATIC_METADATA_TYPE1 = (1 << 0),
} HDR_STATIC_METADATA_TYPE;

typedef struct _HDR_STATIC_META_DATA_BLOCK
{
    CE_EXT_BLK_HEADER CeExtHeader; // CE extension block header

    union
    {
        DDU8 Byte1;
        struct
        {
            DDU8 EOTFSupported : 6;
            DDU8 UNIQUENAME(Rsvd) : 2;
        };
    };
    DDU8 HdrStaticMetaDataType;
    DDU8 DesiredMaxCLL;  // Max content luminance level
    DDU8 DesiredMaxFALL; // Max frame avereage luminance level
    DDU8 DesiredMinCLL;  // Min content luminance level
} HDR_STATIC_META_DATA_BLOCK;

//-----------------------------------------------------------------------------
//
// CE Video Format Preference Data block
//  Tag ID: 7 - CEA_USE_EXTENDED_TAG
//  Extended Tag ID: 13: CEA_VIDEO_FORMAT_PREFERENCE_DATABLOCK
//  Size - Variable size block -- Based on Length in header
//
//-----------------------------------------------------------------------------
typedef struct _CEA_VIDEOFORMAT_PREF_BLOCK
{
    CE_EXT_BLK_HEADER CeExtHeader; // CE extension block header

    // Multiple entries of Video Format Preference Descriptors can be present
    DDU8 PVideoFormatPrefList; // Defined to get the pointer to first preferred video descriptor
} CEA_VIDEOFORMAT_PREF_BLOCK;

//-----------------------------------------------------------------------------
//
// CE 420 Video Descriptor block
//  Tag ID: 7 - CEA_USE_EXTENDED_TAG
//  Extended Tag ID: 14: CEA_420_VIDEO_DATABLOCK
//  Size - Variable size block -- Based on Length in header
//
//-----------------------------------------------------------------------------
typedef struct _CEA_420_DESCR_BLOCK
{
    CE_EXT_BLK_HEADER CeExtHeader; // CE extension block header

    // Multiple entries of Short Video Descriptors can be present
    DDU8 P420VideoDescrList; // Defined to get the pointer to first instance of 420 Video Descriptor
} CEA_420_DESCR_BLOCK;

//-----------------------------------------------------------------------------
//
// CE 420 Capability block
//  Tag ID: 7 - CEA_USE_EXTENDED_TAG
//  Extended Tag ID: 15: CEA_420_CAPABILITY_MAP_DATABLOCK
//  Size - Variable size block -- Based on Length in header
//
//-----------------------------------------------------------------------------
typedef struct _CEA_420_CAP_BLOCK
{
    CE_EXT_BLK_HEADER CeExtHeader; // CE extension block header

    // Multiple entries of 420 capability bytes can be present
    DDU8 P420CapBitMap; // Defined to get the pointer to first instance of 420 Capability pointer
} CEA_420_CAP_BLOCK;

// CE Vendor Specific Video Data Block for HDR10+
typedef struct _CEA_EXT_HDR10P_VENDOR_SPECIFIC_VIDEO_DATA_BLOCK
{
    CE_EXT_BLK_HEADER CeExtHeader; // CE extension block header

    // Byte 2-4: Vendor/Manufacturer IEEE OUI (Organizationally Unique Identification)
    DDU8 IEEERegID[3];

    union
    {
        DDU8 Byte5;
        struct
        {
            DDU8 ApplicationVersion : 2;          // Application Version, Should be set to 1
            DDU8 FullFramePeakLuminanceIndex : 2; // Full Frame Peak Luminance Index, Range: 0-3
            DDU8 PeakLuminanceIndex : 4;          // Peak Luminance Index, Range: 1-15
        };
    };
} CEA_EXT_HDR10P_VENDOR_SPECIFIC_VIDEO_DATA_BLOCK;

//---------------------------------------------------------------------------
//
//  Detailed Timing Data Block
//  Tag ID: 0x22: DisplayID Type 7 Video Timing Data Block (T7VTDB)
//---------------------------------------------------------------------------
typedef struct _TIMINGINFO_TYPE_VII
{
    union
    {
        /** Data Block Revision and Other Data of Enumerated Timing Block 0x23.   */
        DDU8 RevAndOtherData;
        struct
        {
            /** Bits 0-2: Revision. */
            DDU8 Revision : 3;
            /** Bit 3: DscPassThrough. Used by Vesa DisplayID, cleared to 0      .*/
            DDU8 DscPassThrough : 1;
            /** Bits 4-6: Descriptor Length T7_M, only 20 byte desc are used by CTA, cleared to 0. */
            DDU8 DecriptorLength : 3;
            /** Bits 7: Reserved .*/
            DDU8 UNIQUENAME(Rsvd) : 1;
        };
    };
} TIMINGINFO_TYPE_VII;

typedef struct _DID_TIMINGINFO_1 TIMINGINFO_1;

//-----------------------------------------------------------------------------
//
// CE Extension DisplayID Type 7 Timing Data Block
//  Tag ID: 7 - CEA_USE_EXTENDED_TAG
//  Extended Tag ID: 0x22: DisplayID Type 7 Video Timing Data Block (T7VTDB)
//  Size - Variable size block -- Based on Length in header
//
//-----------------------------------------------------------------------------
typedef struct _CEA_EXT_FORMULA_TIMING_VII
{
    /** CEA Extension Block header of Block 0x2A.*/
    CE_EXT_BLK_HEADER CeExtHeader;

    TIMINGINFO_TYPE_VII TimingData7;
    /* Multiple instance of Timing Info X Data Block can be present */
    /** Defined Variable of type timing 7 to get the
     * pointer to first instance of Timing Info X Data Block.
     */
    TIMINGINFO_1 TimingInfo7; // Defined to get the pointer to first instance of Timing Info 7 Data Block
} CEA_EXT_FORMULA_TIMING_VII;

typedef struct _DID_TIMINGINFO_TYPE_VIII TIMINGINFO_TYPE_VIII;

//-----------------------------------------------------------------------------
//
// CE Extension DisplayID Type 8 Enumearated Timing Data Block
//  Tag ID: 7 - CEA_USE_EXTENDED_TAG
//  Extended Tag ID: 0x23: DisplayID Type 8 Video Timing Data Block (T8VTDB)
//  Size - Variable size block -- Based on Length in header
//
//-----------------------------------------------------------------------------
typedef struct _CEA_EXT_FORMULA_TIMING_VIII
{
    /** CEA Extension Block header of Block 0x2A.*/
    CE_EXT_BLK_HEADER CeExtHeader;

    /* Multiple instance of Timing Info X Data Block can be present */
    /** Defined Variable of type timing 8 to get the
     * pointer to first instance of Timing Info X Data Block.
     */
    TIMINGINFO_TYPE_VIII TimingInfo8;
    DDU8 DidTimingInfo8; // Defined to get the pointer to first instance of Timing Info 8 Data Block
} CEA_EXT_FORMULA_TIMING_VIII;

typedef struct _DID_TIMINGINFO_TYPE_X_7BYTE TIMINGINFO_TYPE_X_7BYTE; // forward declaration

//-----------------------------------------------------------------------------
//
// CE Extension DisplayID Type 10 Video Timing Data Block
//  Tag ID: 7 - CEA_USE_EXTENDED_TAG
//  Extended Tag ID: 0x2A: DisplayID Type 10 Video Timing Data Block (T10VTDB)
//  Size - Variable size block -- Based on Length in header
//
//-----------------------------------------------------------------------------
typedef struct _CEA_EXT_FORMULA_TIMING_X
{
    /** CEA Extension Block header of Block 0x2A.*/
    CE_EXT_BLK_HEADER CeExtHeader;
    union
    {
        DDU8 BlockRevAndOtherData; // Data Block Revision and Other Data
        struct
        {
            // used for Type X timings
            DDU8 BlockRevision : 3; // Revision
            DDU8 UNIQUENAME(Rsvd) : 1;
            DDU8 PayloadBytesPerDescriptor : 3; // Number of Payload Bytes (M) in a Timing Descriptor
            DDU8 UNIQUENAME(Rsvd) : 1;
        };
    };
    /* Multiple instance of Timing Info X Data Block can be present */
    /** Defined Variable of type DISPLAYID_2_0_TIMINGINFO_X to get the
     * pointer to first instance of Timing Info X Data Block.
     */
    TIMINGINFO_TYPE_X_7BYTE DidV2Timing10;
} CEA_EXT_FORMULA_TIMING_X;

//-----------------------------------------------------------------------------
//
// HDMI VSDB structures
//
//-----------------------------------------------------------------------------
// Mandatory Blocks for HDMI VSDB if present
// S3D caps within HDMI VSDB structure
typedef union _S3D_CAPS
{
    DDU8 S3DCaps;
    struct
    {
        DDU8 UNIQUENAME(Rsvd) : 3; // Bits 0,1,2 Reserved
        DDU8 ImageSize : 2;        // Field Indicating Image Size
        DDU8 S3DMultiPresent : 2;  // Bits 6,5 indicating multi 3D present support
        DDU8 S3DPresent : 1;       // Indicating 3D present status
    };
} S3D_CAPS;

typedef struct _S3DCAPS_VICLENGTH
{
    S3D_CAPS S3DCaps;
    union
    {
        DDU8 Data; // Byte 9/11/13
        struct
        {
            DDU8 Hdmi3DLen : 5;  // HDMI 3D video format length
            DDU8 HdmiVicLen : 3; // HDMI VIC length
        };
    };
} S3DCAPS_VICLENGTH;

typedef struct _HDMI_VSDB
{
    CE_BLK_HEADER CeDataBlockHdr; // Byte 0

    DDU8 IEEERegID[3];       // Byte 1, 2, 3
    DDU8 SrcPhysicalAddr[2]; // Byte 4, 5
    union
    {
        DDU8 Capabilty; // Byte 6
        struct
        {
            DDU8 DualDVISupport : 1;   // Bit0
            DDU8 UNIQUENAME(Rsvd) : 2; // Bits 2-1
            DDU8 YCbCrS444Support : 1; // Bit3
            DDU8 DC10BitSupport : 1;   // Bit4
            DDU8 DC12BitSupport : 1;   // Bit5
            DDU8 DC16BitSupport : 1;   // Bit6
            DDU8 SupportsAI : 1;       // Bit7
        };
    };

    DDU8 MaxTMDSClockIn5Mhz; // Byte 7
    union
    {
        DDU8 LatencyFlags; // Byte 8
        struct
        {
            DDU8 ContentType0 : 1;         // Bit0
            DDU8 ContentType1 : 1;         // Bit1
            DDU8 ContentType2 : 1;         // Bit2
            DDU8 ContentType3 : 1;         // Bit3
            DDU8 UNIQUENAME(Rsvd) : 1;     // Bit4
            DDU8 HdmiVideoPresent : 1;     // Bit5
            DDU8 ILatencyFieldPresent : 1; // Bit6
            DDU8 LatencyFieldPresent : 1;  // Bit7
        };
    };
    DDU8 VideoLatency;            // Valid if HdmiVideoPresent && LatencyFieldPresent set, else this would be S3DCaps
    DDU8 AudioLatency;            // Valid if HdmiVideoPresent && LatencyFieldPresent set, else this would be S3DCaps
    DDU8 InterlacedVideoLatency;  // Valid if above 2 bits + ILatencyFieldPresent set, else this would be S3DCaps
    DDU8 InterlacedAudioLatency;  // Valid if above 2 bits + ILatencyFieldPresent set, else this would be S3DCaps
    S3DCAPS_VICLENGTH S3DVICCaps; // This would be S3DCaps if HdmiVideoPresent && LatencyFieldPresent && ILatencyFieldPresent set
} HDMI_VSDB;

typedef union _S3D_FORMATS
{
    DDU16 S3DFormats;
    struct
    {
        DDU16 S3DStructureAll_0 : 1;   // Sink Supports " Frame Packing 3D Formats
        DDU16 S3DStructureAll1_5 : 5;  // Reserved
        DDU16 S3DStructureAll_6 : 1;   // Sink Supports " Top & Bottom 3D Formats
        DDU16 S3DStructureAll_7 : 1;   // Reserved
        DDU16 S3DStructureAll_8 : 1;   // Sink Supports " Side by Side with horizontal sub-sampling 3D Formats
        DDU16 S3DStructureAll9_15 : 7; // Reserved
    };
} S3D_FORMATS;

typedef struct _VIC_ORDER_3D_STRUCT_DETAILS
{
    union
    {
        DDU8 Byte0; // 2D Vic order/3D structure
        struct
        {
            DDU8 S3DStruc_X : 4;      // 3D_Structure_X
            DDU8 S2D_VIC_Order_X : 4; // 2D_VIC_Order_X
        };
    };
    union
    {
        DDU8 Byte1; // Optional byte, present if 3D_Structure_X is b/w 0x1000 - 0x1111
        struct
        {
            DDU8 Reserved : 4;    // Reserved
            DDU8 S3DDetail_X : 4; // 3D_Detail_X
        };
    };
} VIC_ORDER_3D_STRUCT_DETAILS;

/***************************************************************
 * @brief HDMI HF-EEODB structures.
 *
 * Description: HDMI Forum EDID Extension Override Data Block (Tag -0x07)
 *              Mandatory size 2 Bytes.
 *              Alternative Method for the Sinks to indicate EDID Extension Block Count > 1.
 *              Only 1 instance of this block is possible.
 *              Sinks that include this block, shall set Extension Flag (Byte(126)) value to 1.
 ***************************************************************/
typedef struct _HDMI_HFEEODB
{
    /** Byte 0:    Data Block Header.  */
    CE_BLK_HEADER CeDataBlockHdr;
    /** Byte 1:    Extended Tag Code (120 = 0x78). */
    DDU8 ExtendedTagCode;
    /** Byte 2:    EDID Extension Block Count. */
    DDU8 EdidExtensionBlockCount;
} HDMI_HFEEODB;

/***************************************************************
 * @brief HDMI HF-VSDB structures.
 *
 * Description: HDMI Forum Vendor Specific Data Block (Tag -0x03)
 *             Mandatory Length is 7Bytes and varies upto 31Bytes
 *             Following struct is part1 of size 7 Bytes.
 ***************************************************************/
typedef struct _HDMI_HFVSDB
{
    /** Byte 0:    Data Block Header.  */
    CE_BLK_HEADER CeDataBlockHdr;
    /** Bytes1-3: IEEE OUI ID as "0xC45DD8" . */
    DDU8 IEEERegID[3];
    /** Byte 4: Version; Set to 1 for HDMI2.1 . */
    DDU8 Version;
    /** Byte 5: Max TMDS Char Rate in 5MHz . */
    DDU8 MaxTMDSCharRateIn5Mhz;
    union
    {
        /** Byte 6: DisplayCaps0: Informs if Sink can -- . */
        DDU8 DisplayCaps0;
        struct
        {
            /** Bit 0: receive 3D_OSD_Disparity indication in HF-VSIF. */
            DDU8 OSDDisparity3D : 1;
            /** Bit 1: receive 3D_Dual_View signaling in HF-VSIF. */
            DDU8 DualView3D : 1;
            /** Bit 2: receive 3D_Indepedent_View signaling in HF-VSIF. */
            DDU8 IndependentView3D : 1;
            /** Bit 3: support Scrambling for TMDS <-340 Mcsc; if SCDC =0, this field is 0. */
            DDU8 IsLte340McscScramblingEnabled : 1;
            /** Bit 4: support Color Content Bits per Component Indication. */
            DDU8 IsCCBPCI : 1;
            /** Bit 5: support cable assembly status indication. */
            DDU8 CableStatus : 1;
            /** Bit 6: initiate SCDC Read Request. */
            DDU8 IsSCDCRRCapable : 1;
            /** Bit 7: support SCDC (Status & Control Data Channel) capability. */
            DDU8 IsSCDCPresent : 1;
        };
    };
    union
    {
        /** Byte 7: DisplayCaps1: Informs if Sink can -- . */
        DDU8 DisplayCaps1;
        struct
        {
            /** Bit 0: support 10 bits/component. */
            DDU8 DC10BitSupport : 1;
            /** Bit 1: support 12 bits/component. */
            DDU8 DC12BitSupport : 1;
            /** Bit 2: support 16 bits/component. */
            DDU8 DC16BitSupport : 1;
            /** Bit 3: Support CTA-861-H VIC signalling in AVI InfoFrame .*/
            DDU8 IsUhdVicSupported : 1;
            /** Bits 4-7: support FRL (Fixed Rate Link):
            values 0-6 indicate combination of Gbps/Lane & no of Lanes supported. */
            DDU8 MaxFrlRateIndex : 4;
        };
    };
    union
    {
        /** Byte 8: DisplayCaps2: Informs if Sink can -- . */
        DDU8 DisplayCaps2;
        struct
        {
            /** Bit 0: Frame Accurate Packet Area start location. */
            DDU8 FAPA_Start_Location : 1;
            /** Bit 1: Auto Low-Latency support. */
            DDU8 AllmSupport : 1;
            /** Bit 2: Fast Vertical Active support. */
            DDU8 FvaSupport : 1;
            /** Bit 3: Negative Mvrr support when FVA and VRR are supported. */
            DDU8 NegativeMVrrSupport : 1;
            /** Bit 4: Cinema VRR. Deprecated. */
            DDU8 CinemaVrrSupport : 1;
            /** Bit 5: Sinks ability to anticipate and compensate frame-to-frame transitions. */
            DDU8 MDelta : 1;
            /** Bit 6: Quick Media Switching support. */
            DDU8 QmsSupport : 1;
            /** Bit 7: support extended FAPA_End_Base. */
            DDU8 FapaEndExtended : 1;
        };
    };
    union
    {
        /** Bytes 9-10: VrrCaps. */
        DDU16 VrrCaps;
        struct
        {
            /** Bits 5:0: Lowest RR in Hz from 0-48 Sink can support, 0 - VRR not supported. */
            DDU16 VrrMin : 6;
            /** Bit 7:6: Upper 2 bits of VrrMax. */
            DDU16 VrrMaxHighBits : 2;
            /** Bit15:8: Lower 8 bits of VRR Max. THis indicates upper Limit for VRR, base value is 100Hz, - means no upper limit and is is limited by Timings supported in base video timing.*/
            DDU16 VrrMaxLowBits : 8;
        };
    };
    union
    {
        /** Byte 11: DscBppSupport: Informs if Sink can -- . */
        DDU8 DscBpcSupport;
        struct
        {
            /** Bit 0: Indicates 10bpc compressed video transport support. */
            DDU8 Dsc10BpcSupport : 1;
            /** Bit 1: Indicates 12bpc compressed video transport support. */
            DDU8 Dsc12BpcSupport : 1;
            /** Bit 2: Indicates 16bpc compressed video transport support. */
            DDU8 Dsc16BpcSupport : 1;
            /** Bit 3: Indicates additional 1/16 bit bpp setting support. */
            DDU8 DscAllBppSupport : 1;
            /** Bit 4: Fractional and Integer Media RR support for RRs below VRRmin when VRR enabled and MConst is in use. */
            DDU8 QmsTfrMinSupport : 1;
            /** Bit 5: Fractional and Integer Media RR support for RRs upto VRRmax. */
            DDU8 QmsTfrMaxSupport : 1;
            /** Bit 6:  Indicates native 420 support decoding of compressed stream decoding. */
            DDU8 DscNative420Support : 1;
            /** Bit 7: VESA DSC 1.2a support. */
            DDU8 Dsc1p2Support : 1;
        };
    };

    /** Byte 12: .*/
    /** Bits 3:0: Maximum number of Horizontal slices Sink can support, 0- not support, 8 - 15 reserved. */
    DDU8 DscMaxSlices : 4;
    /** Bits 7:4: Indicates 12bpc compressed video transport support. */
    DDU8 DscMaxFrlRateIndex : 4;

    union
    {
        /** Byte 13: DscCaps: .*/
        DDU8 DscTotalChunkCaps;
        struct
        {
            /** Bit 5:0: Indicates 16bpc compressed video transport support. */
            DDU8 DscTotalChunkKBytes : 6;
            /** Bit 7:6: Indicates additional 1/16 bit bpp setting support. */
            DDU8 UNIQUENAME(Rsvd) : 2;
        };
    };
} HDMI_HFVSDB;

//-----------------------------------------------------------------------------
//
//  CEA Structure definitions -- END
//
//-----------------------------------------------------------------------------

//==================================================================================
//==================================================================================
//    DATA Structure definitions for VTB parsing.....
//  Reference VESA Documents are VTB Extension(Release A) & CVT standard version 1.1
//===================================================================================
//    #defines for VTB-EXT
//===================================================================================

#define VTB_EXT_SUPPORTED_VERSION 0x03

#define VTB_MAX_DTD_TIMINGS 6
#define VTB_MAX_CVT_TIMINGS 40
#define VTB_MAX_STANDARD_TIMINGS 61

#define VTB_DTD_OFFSET 5
#define VTB_DTD_SIZE 18
#define VTB_CVT_SIZE 3
#define VTB_MAX_RR 5
#define VTB_RR_SUPPORTMASK 0x1F
#define VTB_ST_SIZE 2
#define VTB_DATA_SIZE 122

// This struct is for VTB Extension block.
typedef struct _VTB_EXT
{
    DDU8 Tag;
    DDU8 Version;
    DDU8 NumDTD;
    DDU8 NumCVT;
    DDU8 NumST;
    DDU8 DATA[VTB_DATA_SIZE];
    DDU8 Checksum;
} VTB_EXT;

// Following struct is for CVT descriptor (Version 1.1)
typedef struct _VTB_CVT_TIMING
{

    DDU8 VA_low; // Lower 8 bits of Vertical size. This Vsize = (vertical active lines/2)-1.
    //  Range for VA lines is 2 to 8192. CVT supprts only an even no. of active lines per frame.

    union
    {
        DDU8 VA_high_AR;
        struct
        {

            DDU8 Reserved00 : 2;  // Bits 1-0 are reserved and set to 00h
            DDU8 AspectRatio : 2; //  Aspect Ratio specifier bits.
            // 00:     4:3 Aspect ratio
            // 01:    16:9 Aspect ratio
            // 10:    16:10 Aspect ratio
            // 11: Undefined (Reserved)

            DDU8 VA_high : 4; //  Upper 4 bits of Vertical Size.
        };
    };

    union
    {
        DDU8 Refresh_Rate_Bits;
        struct
        {

            DDU8 RR_60Hz_RB : 1;            // When set, indicates 60Hz support with Reduced Blanking.
            DDU8 RR_85Hz : 1;               //              ||      85Hz                ||                                              .
            DDU8 RR_75Hz : 1;               //              ||      75Hz                ||                                              .
            DDU8 RR_60Hz : 1;               //              ||      60Hz                ||                                              .
            DDU8 RR_50Hz : 1;               // When set, indicates 50Hz Refrash Rate with CRT Blanking supports specified pixel format.
            DDU8 PreferredRefresh_Rate : 2; // Preferred Refresh Rate specifier bits.
            // 00:  50 Hz
            // 01:  60 Hz (this means either CRT blanking or Reduced Blanking whichever is supported.
            //              If both are supported, then RB is preferred.)
            // 10:    75 Hz
            // 11:    85 Hz

            DDU8 Reserved0 : 1; // This bit is reserved and set to '0'.
        };
    };
} VTB_CVT_TIMING;

// This struct is for storing extracted Info from CVT descriptor....
// This is defined by author.....not based on CVT specs.
typedef struct _CVT_INFO
{
    DDU32 YRes;
    DDU32 XRes;
    DDU32 RRate[5]; // As max 5 Refresh Rates can be supported.
    DD_BOOL Red_Blank_Req[5];
    DD_BOOL Preferred_RR[5]; // To set flag for Preffered RR
    DDU32 NumRates;          // Number of Refresh rates Supported. (Max. 5)
} CVT_INFO;

// This structure is for stroing the Display device Data retreived from CEA block
// This is defined as per the Display Device Data Block standard.
typedef struct _DISPLAY_DEVICE_DATA
{
    union
    {
        DDU8 TagAndLength; // Data Block Tag and Block Length. should be 0xFF
        struct
        {
            DDU8 Length : 5;
            DDU8 Tag : 3;
        };
    };
    DDU8 ChildTag; // Child tag required as per CEA spec  should be 0x02
    union
    {
        DDU8 InterfaceType;
        struct
        {
            DDU8 NumOfChannels : 4; // Number of channels supported
            DDU8 InterfaceCode : 4; // Interface code
        };
    };
    union
    {
        DDU8 VerAndRel;
        struct
        {
            DDU8 Release : 4; // Release
            DDU8 Version : 4; // Version.
        };
    };
    DDU8 ContentProtectionSuppFlag; // Flag indicating support for content protection.
    union
    {
        DDU16 ClockFrequency; // Clock Frequency
        struct
        {
            DDU16 MinClockFrequency : 6;  // First 6 bits indicates Min frequency
            DDU16 MaxClockFrequency : 10; // Next 10 bits indicates Max frequency
        };
    };
    union
    {
        DDU8 NativePixelFormat[4]; // Pixel Format
        struct
        {
            DDU8 HorizontalPixelCntLower; // Lower byte value of the Horizontal pixel count
            DDU8 HorizontalPixelCntUpper; // Upper byte value of the Horizontal pixel count
            DDU8 VerticalPixelCntLower;   //  Lower byte value of the vertical pixel count
            DDU8 VerticalPixelCntUpper;   // Upper byte value of the vertical pixel count
        };
    };
    DDU8 AspectRatio; // Byte indicating Aspect ratio.
    union
    {
        DDU8 OrientationAndRotation;
        struct
        {
            DDU8 ScanDirection : 2;      // Scan direction.
            DDU8 ZeroPixelLocation : 2;  // Zero Pixel Location.
            DDU8 RotationCapability : 2; // Indicates rotation capability
            DDU8 DefaultOrientation : 2; // Default Orientation.
        };
    };
    DDU8 SubPixelInfo;    // Sub-Pixle Information.
    DDU8 HorizontalPitch; // Horizontal Pitch
    DDU8 VerticalPitch;   // Vertical Pitch
    union
    {
        DDU8 MiscDisplayCapabilities;
        struct
        {
            DDU8 Reserved : 3;
            DDU8 Deinterlacing : 1; // indicates deinterlacing support
            DDU8 OverdriverNotRecommended : 1;
            DDU8 DirectDrive : 1; // indicates DirectDrive support
            DDU8 Dithering : 2;   // indicates Dithering support.
        };
    };
    union
    {
        DDU8 AudioFlags; // Flags indicating Audio details
        struct
        {
            DDU8 Reserved1 : 4;
            DDU8 AudioInputOverride : 1;         // Indicates Audio Input Override
            DDU8 SeparateAudioInputs : 1;        // Indicates Separate Audio Inputs
            DDU8 AudioInputOnVideoInterface : 1; // Shows whether Audio input is through the video interface.
        };
    };
    union
    {
        DDU8 AudioDelayFlags; // Audio Delay Flags
        struct
        {
            DDU8 AudioDelay : 7; // Absolute offset between the audio and video signals.
            DDU8 AudioSign : 1;  // Indicates positive or negative delay.
        };
    };
    union
    {
        DDU8 FrameRateAndModeConversion;
        struct
        {
            DDU8 FrameRateRange : 6;      // Device Frame rate Range
            DDU8 FrameRateConversion : 2; // 00 – No dedicated rate conversion hardware is provided;
            // 01 – The display provides a single frame buffer
            // 10 – The display provides double-buffering
            // 11- The display provides frame-rate conversion involving interframe interpolation
        };
    };
    DDU8 DeviceNativeRate; // Device Native Frame rate
    union
    {
        DDU8 ColorBitDepth; // Color bit depth
        struct
        {
            DDU8 DisplayDeviceColBitDepth : 4; // Color bit depth of the display device
            DDU8 InterfaceColBitDepth : 4;     // color bit depth supported by the interface.h
        };
    };
    DDU8 AddPrimaryChromaticities[8]; // Additional Primary Chromaticities.
    union
    {
        DDU8 ResponseTimeFlags;
        struct
        {
            DDU8 ResponseTime : 7; // Time for transition.
            DDU8 BlackToWhite : 1; // if 1, then transition from black to white
            // if 0, then transition from white to black
        };
    };
    union
    {
        DDU8 OverscanInformation;
        struct
        {
            DDU8 VerticalPercentage : 4;   // Percentage of Overscan in vertical direction.
            DDU8 HorizontalPercentage : 4; // Percentage of Overscan in horizontal direction.
        };
    };
} DISPLAY_DEVICE_DATA;
#pragma pack()

//-------------------------------------------------------------
//
// MACROs / Inline functions
//
//-------------------------------------------------------------
DD_INLINE DDU8 GET_MONITOR_NAME_LENGTH(DDU8* pMonitorName)
{
    DDU8 MonitorNameLength = 0;

    while ((MonitorNameLength < MONITOR_NAME_LENGTH) && (pMonitorName[MonitorNameLength] != 0x0A))
    {
        MonitorNameLength++;
    }

    return MonitorNameLength;
}
