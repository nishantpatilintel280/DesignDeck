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
 * @file  iHDMI.h
 * @brief This file contains all the HDMI spec related definitions
 *
 */

#ifndef __IHDMI_H__
#define __IHDMI_H__

#pragma pack(1)

#define HBR_AUDIO_SAMPLE_RATE 192000  // 192kHz is the sample rate corresponding to the HBR audio formats
#define HBR_AUDIO_SAMPLING_BYTE 0x7F  // 192kHz is the sample rate Support
#define AUDIO_SAMPLE_RATE_48KHZ 48000 // 48Khz Audio Sample Rate
#define AUDIO_CLOCK_PACKET_RATE 1500  // Audio clock packet rate of 1.5kHz has to be considered while calculating audio BW

#define BAR_INFO_LENGTH 8 // 8 bytes of barinfo

 //
 // HDMI command types
 //
typedef enum
{
    HDMI_COMMAND_GET,
    HDMI_COMMAND_SET
} HDMI_COMMAND;

#define AVI_FLAG_ITCONTENT 0x00800000
#define AVI_FLAG_RGB_QUANT_RANGE 0x00040000
#define AVI_FLAG_SCAN_INFO 0x00000001
#define AVI_FLAG_BAR_INFO 0x00000010
#define AVI_FLAG_PAR_INFO 0x00001000

//
// HDMI buffer/information types
//
typedef enum _HDMI_INFO_TYPE
{
    // Non-standard or non-HDMI type
    ELD_TYPE = 0x00, // ELD buffer type
    EELD_TYPE = 0x01, // EELD buffer type

    // Per HDMI Spec, refer Table 2-1 in HDMI EDS
    // or Table 5-8 in HDMI spec
    VS_TYPE = 0x81, // Vendor-Specific InfoFrame type
    AVI_TYPE = 0x82, // AVI InfoFrame type
    SPD_TYPE = 0x83, // SPD InfoFrame type
    AUDIO_TYPE = 0x84, // Audio InfoFrame type
    MS_TYPE = 0x85, // MPEG Source InfoFrame type
    DRM_TYPE = 0x87, // Dynamic Range and Mastering Infoframe type

    // Non-standard or non-HDMI types
    PR_PE_TYPE = 0x86, // Pixel Replication & Pixel Encoding(colorimetry) type
    AUDIO_ENABLE_FLAGS_TYPE = 0x88, // Flags for enabling / disabling audio
    AUDIO_BUFFER_CAPS_TYPE = 0x89  // Flags for getting the encoder audio buffer capablity.
} HDMI_INFO_TYPE;

//
// InfoFrame Version Information
//
typedef enum _INFOFRAME_VERSION
{
    VS_VERSION = 1, // Vendor-Specific InfoFrame Version 1
    AVI_VERSION = 1, // AVI InfoFrame Version 1
    AVI_VERSION2 = 2, // AVI InfoFrame Version 2
    AVI_VERSION3 = 3, // AVI InfoFrame Version 3
    AVI_VERSION4 = 4, // AVI InfoFrame Version 4
    SPD_VERSION = 1, // SPD InfoFrame Version 1
    AUDIO_VERSION = 1, // Audio InfoFrame Version 1
    MS_VERSION = 1, // MPEG Source InfoFrame Version 1
    DRM_VERSION = 1  // DRM InfoFrame Version 1
} INFOFRAME_VERSION;

//
// InfoFrame Payload Length in bytes
//
typedef enum _HDMI_INFOFRAME_LENGTH
{
    HDMI_VS_LENGTH = 27, // Vendor-Specific InfoFrame Payload Length, including IEEE reg ID
    HDMI_AVI_LENGTH = 13, // AVI InfoFrame Payload Length
    HDMI_SPD_LENGTH = 25, // SPD InfoFrame Payload Length
    HDMI_AUDIO_LENGTH = 10, // Audio InfoFrame Payload Length
    HDMI_MS_LENGTH = 10, // MPEG Source InfoFrame Payload Length
    HDMI_PR_PE_LENGTH = 4,  // Length of PR_PE_TYPE
    HDMI_AUDIO_CAPS_LENGTH = 4,  // Length of AUDIO_CAPS_TYPE
    DRM_LENGTH = 26  // Length of DRM InfoFrame Payload Length
} HDMI_INFOFRAME_LENGTH;

//
// InfoFrame TOTAL Length in bytes (includes header + payload)
//
typedef enum _INFOFRAME_TOTAL_LENGTH
{
    VS_MAX_TOTAL_LENGTH = HDMI_VS_LENGTH + 4,    // Max Total size of Vendor-Specific InfoFrame
    AVI_TOTAL_LENGTH = HDMI_AVI_LENGTH + 4,   // Total size of AVI InfoFrame
    SPD_TOTAL_LENGTH = HDMI_SPD_LENGTH + 4,   // Total size of SPD InfoFrame
    AUDIO_TOTAL_LENGTH = HDMI_AUDIO_LENGTH + 4, // Total size of Audio InfoFrame
    MS_TOTAL_LENGTH = HDMI_MS_LENGTH + 4,    // Total size of MPEG Source InfoFrame
    DRM_TOTAL_LENGTH = DRM_LENGTH + 4         // Total size of DRM InfoFrame
} INFOFRAME_TOTAL_LENGTH;

//
// Pixel Replication multipliers
//
typedef enum
{
    HDMI_PR_ONE = 0, // No repetition (ie., pixel sent once)
    HDMI_PR_TWO,     // Pixel sent 2 times (ie.,repeated once)
    HDMI_PR_THREE,   // Pixel sent 3 times
    HDMI_PR_FOUR,    // Pixel sent 4 times
    HDMI_PR_FIVE,    // Pixel sent 5 times
    HDMI_PR_SIX,     // Pixel sent 6 times
    HDMI_PR_SEVEN,   // Pixel sent 7 times
    HDMI_PR_EIGHT,   // Pixel sent 8 times
    HDMI_PR_NINE,    // Pixel sent 9 times
    HDMI_PR_TEN      // Pixel sent 10 times
} HDMI_PIXEL_REPLICATION;

//
// Pixel encoding modes
//
typedef enum
{
    HDMI_RGB256 = 0x01,
    HDMI_RGB220 = 0x02,
    HDMI_YCRCB422 = 0x04,
    HDMI_YCRCB444 = 0x08
} HDMI_COLORIMETRY;

//
// AVI InfoFrame definitions - start
//
// Scan Info
typedef enum
{
    AVI_SCAN_NODATA = 0, // No data
    AVI_SCAN_OVERSCAN = 1, // Overscanned (TV)
    AVI_SCAN_UNDERSCAN = 2, // Underscanned (Computer)
    AVI_SCAN_FUTURE = 3  // Future
} AVI_SCAN_INFO;

// Bar Info
typedef enum
{
    AVI_BAR_INVALID = 0, // Bar data not valid
    AVI_BAR_VALID_VERTICAL = 1, // Vertical Bar data valid
    AVI_BAR_VALID_HORIZONTAL = 2, // Horizontal Bar data valid
    AVI_BAR_VALID_BOTH = 3  // Vertical & Horizontal Bar data valid
} AVI_BAR_INFO;

// Active Format Information
typedef enum
{
    AVI_AFI_INVALID = 0, // No data
    AVI_AFI_VALID = 1  // Active Format Information valid
} AVI_AFI_INFO;

// AVI Pixel Encoding modes
typedef enum
{
    AVI_RGB_MODE = 0, // RGB pixel encoding mode
    AVI_YCRCB422_MODE = 1, // YCrCb 4:2:2 mode
    AVI_YCRCB444_MODE = 2, // YCrCb 4:4:4 mode
    AVI_YCRCB420_MODE = 3, // YCrCb 4:2:0 mode
    AVI_FUTURE_MODE = 4  // Future mode
} AVI_ENCODING_MODE;

// AVI Active Format Aspect Ratio
typedef enum
{
    AVI_AFAR_SAME = 8,  // same as picture aspect ratio
    AVI_AFAR_4_3 = 9,  // 4:3 center
    AVI_AFAR_16_9 = 10, // 16:9 center
    AVI_AFAR_14_9 = 11  // 14:9 center
} AVI_AFAR_INFO;

// AVI Picture Aspect Ratio
typedef enum _AVI_PAR_INFO
{
    AVI_PAR_NODATA = 0,        // No Data
    AVI_PAR_4_3 = __BIT(0), // 4:3
    AVI_PAR_16_9 = __BIT(1), // 16:9
    RESERVED = 3,
    AVI_PAR_64_27 = __BIT(2), // 64:27
    AVI_PAR_FUTURE = __BIT(3)  // Future
} AVI_PAR_INFO;

// AVI Colorimetry Information
typedef enum
{
    AVI_COLOR_NODATA = 0, // No data
    AVI_COLOR_ITU601 = 1, // SMPTE 170M, ITU601
    AVI_COLOR_ITU709 = 2, // ITU709
    AVI_COLOR_USEEXTENDED = 3  // Use the extended colorimetry for source format
} AVI_COLOR_INFO;

// AVI Non-uniform Picture Scaling Info
typedef enum
{
    AVI_SCALING_NODATA = 0, // No scaling
    AVI_SCALING_HORIZONTAL = 1, // horizontal scaling
    AVI_SCALING_VERTICAL = 2, // vertical scaling
    AVI_SCALING_BOTH = 3  // horizontal & vertical scaling
} AVI_SCALING_INFO;

// AVI RGB Quantization Range
typedef enum
{
    AVI_RGBQUANT_DEFAULT = 0, // Default value
    AVI_RGBQUANT_LIMITED = 1, // Limited Range
    AVI_RGBQUANT_FULL = 2, // Full Range
    AVI_RGBQUANT_FUTURE = 3  // Future use
} AVI_RGBQUANT_RANGE;

// AVI YCC Quantization Range
typedef enum
{
    AVI_YCCQUANT_LIMITED = 0, // Limited Range
    AVI_YCCQUANT_FULL = 1, // Full Range
    AVI_YCCQUANT_FUTURE = 2  // Future use
} AVI_YCCQUANT_RANGE;

// AVI IT Content
typedef enum
{
    AVI_ITC_NODATA = 0, // No Data  - if ITContent is 0
    AVI_ITC_GRAPHICS = 0, // Graphics - if ITContent is 1
    AVI_ITC_PHOTO = 1, // Photo
    AVI_ITC_CINEMA = 2, // Cinema
    AVI_ITC_GAME = 3  // Game
} AVI_IT_CONTENT;

typedef enum
{
    AVI_EXTCOLORIMETRY_XVYCC601 = 0,
    AVI_EXTCOLORIMETRY_XVYCC709 = 1,
    AVI_EXTCOLORIMETRY_CYCC_BT2020 = 5, // ITU-R BT.2020  constant luminance cY'C'C'
    AVI_EXTCOLORIMETRY_RGB_YCC_BT2020 = 6, // ITU-R BT.2020  R'G'B' or  Y'C'C'
    AVI_EXTCOLORIMETRY_USE_ADDITIONAL = 7  // From CTA-861-G onwards
} AVI_EXTCOLORIMETRY_INFO;

//
// AVI InfoFrame definitions - end
//

//
// SPD InfoFrame definitions - start
//
// SPD InfoFrame Data Byte 25, refer Table-33 in CEA-861H
typedef enum
{
    SPD_SRC_UNKNOWN = 0x00, // unknown
    SPD_SRC_DIGITAL_STB = 0x01, // Digital STB
    SPD_SRC_DVD = 0x02, // DVD
    SPD_SRC_DVHS = 0x03, // D-VHS
    SPD_SRC_HDD_VIDEO = 0x04, // HDD Video
    SPD_SRC_DVC = 0x05, // DVC
    SPD_SRC_DSC = 0x06, // DSC
    SPD_SRC_VCD = 0x07, // Video CD
    SPD_SRC_GAME = 0x08, // Game
    SPD_SRC_PC = 0x09, // PC General
    SPD_SRC_BD = 0x0A, // Blu-Ray Disc (BD)
    SPD_SRC_SA_CD = 0x0B, // Super Audio CD
    SPD_SRC_HD_DVD = 0x0C, // HD DVD
    SPD_SRC_PMP = 0x0D, // PMP
    SPD_SRC_MAX = 0x0E, // Reserved Keep as last entry
} SPD_SRC_TYPES;

// SPD InfoFrame Vendor Name & Descriptor Length in bytes
typedef enum
{
    SPD_VNAME_LENGTH = 8,  // SPD Vendor Name Length in bytes
    SPD_VDESC_LENGTH = 16, // SPD Vendor Descriptor Length in bytes
} SPD_NAMEDESC_LENGTH_INFO;

//
// SPD InfoFrame definitions - end
//

//
// InfoFrame Packet Header - generic
//
typedef union _IF_HEADER
{
    DDU32 Value;
    struct
    {
        DDU8 Type;    // InfoFrame Type
        DDU8 Version; // InfoFrame Version
        DDU8 Length;  // InfoFrame Length
        DDU8 Chksum;  // Checksum of the InfoFrame
    };
} IF_HEADER;

// AVI InfoFrame IT Content
typedef union _AVI_IT_CONTENT_TYPE
{
    DDU8 Value;
    struct
    {
        DDU8 GraphicsContent : 1;
        DDU8 PhotoContent : 1;
        DDU8 CinemaContent : 1;
        DDU8 GameContent : 1;
        DDU8 UNIQUENAME(Rsvd) : 4; // Reserved BIT(4-7)
    };
} AVI_IT_CONTENT_TYPE;

// AVI InfoFrame IT Content
typedef struct _DD_AVI_IT_CONTENT_TYPE
{
    DD_BOOL GraphicsContent;
    DD_BOOL PhotoContent;
    DD_BOOL CinemaContent;
    DD_BOOL GameContent;
} DD_AVI_IT_CONTENT_TYPE;

//
// AVI InfoFrame structure
//
typedef union _AVI_IF
{
    DDU8 AviBuf[AVI_TOTAL_LENGTH];
    struct
    {
        IF_HEADER IfHeader; // AVI header data
        union
        {
            DDU8 Byte1;
            struct
            {
                DDU8 ScanInfo : 2; // scan information
                DDU8 BarInfo : 2;  // bar information
                DDU8 Format : 1;   // active format information
                DDU8 EncMode : 3;  // pixel encoding (RGB or YCrCb 444 or YCbCr 420)
                // DDU8 ucB1Rsvd  :1;     // reserved
            };
        };
        union
        {
            DDU8 Byte2;
            struct
            {
                DDU8 Afar : 4;        // Active Format Aspect Ratio
                DDU8 Par : 2;         // Picture Aspect Ratio
                DDU8 Colorimetry : 2; // colorimetry
            };
        };
        union
        {
            DDU8 Byte3;
            struct
            {
                DDU8 ScalingInfo : 2;    // Scaling information
                DDU8 RgbQuantRange : 2;  // RGB Quantization Range
                DDU8 ExtColorimetry : 3; // Extended Colorimetry uses AVI_EXTCOLORIMETRY_INFO
                DDU8 ITContent : 1;      // IT Content
            };
        };
        union
        {
            DDU8 Byte4;
            struct
            {
                DDU8 Vic : 8; // Video Identification code (refer Table 13 in CEA-861b)
            };
        };
        union
        {
            DDU8 Byte5;
            struct
            {
                DDU8 Pr : 4;            // pixel repetition (refer Table 15 in CEA-861b)
                DDU8 Cn : 2;            // IT Content Type  (Table 8-6 : HDMI 1.4a spec)
                DDU8 YccQuantRange : 2; // YCC Quantization Range (For YCbCr)
            };
        };
        union
        {
            DDU8 BarData[BAR_INFO_LENGTH];
            struct
            {
                DDU8 Byte6;  // end of top bar(lower), set to "00"
                DDU8 Byte7;  // end of top bar(upper), set to "00"
                DDU8 Byte8;  // start of bottom bar(lower), set to "00"
                DDU8 Byte9;  // start of bottom bar(upper), set to "00"
                DDU8 Byte10; // end of left bar(lower), set to "00"
                DDU8 Byte11; // end of left bar(upper), set to "00"
                DDU8 Byte12; // start of right bar(lower), set to "00"
                DDU8 Byte13; // start of right bar(upper), set to "00"
            };
        };
    };
} AVI_IF;

//
// SPD InfoFrame structure
//
typedef union _SPD_IF
{
    DDU8 SpdBuf[SPD_TOTAL_LENGTH];
    struct
    {
        IF_HEADER IfHeader; // SPD header data
        DDU8 Name[8];       // Vendor Name, 8 characters
        DDU8 Desc[16];      // Product Description, 16 characters
        DDU8 Sdi;           // Source Device Information
    };
} SPD_IF;

//
// Vendor Specific InfoFrame structure
//
typedef union _VS_IF
{
    DDU8 VsBuf[VS_MAX_TOTAL_LENGTH];
    struct
    {
        IF_HEADER IfHeader; // VS header data
        DDU8 IeeeRegID[3];  // 3-byte IEEE registration ID
        DDU8 Payload[24];   // Payload bytes
    };
} VS_IF;

//
// Dynamic Range and Mastering InfoFrame structure (HDR)
//
typedef union _DRM_IF
{
    DDU8 DrmBuf[DRM_TOTAL_LENGTH];
    struct
    {
        IF_HEADER IfHeader; // VS header data
        union
        {
            DDU8 Byte1;
            struct
            {
                DDU8 Eotf : 3;             // EOTF information
                DDU8 UNIQUENAME(Rsvd) : 5; // reserved
            };
        };
        union
        {
            DDU8 Byte2;
            struct
            {
                DDU8 SmdId : 3;            // Static Metadata ID information
                DDU8 UNIQUENAME(Rsvd) : 5; // reserved
            };
        };

        union
        {
            DDU16 DisplayPrimariesX0; // 2 bytes
            struct
            {
                DDU8 Byte3; // display_primaries_x[0], LSB
                DDU8 Byte4; // display_primaries_x[0], MSB
            };
        };
        union
        {
            DDU16 DisplayPrimariesY0; // 2 bytes
            struct
            {
                DDU8 Byte5; // display_primaries_y[0], LSB
                DDU8 Byte6; // display_primaries_y[0], MSB
            };
        };
        union
        {
            DDU16 DisplayPrimariesX1;
            struct
            {
                DDU8 Byte7; // display_primaries_x[1], LSB
                DDU8 Byte8; // display_primaries_x[1], MSB
            };
        };
        union
        {
            DDU16 DisplayPrimariesY1;
            struct
            {
                DDU8 Byte9;  // display_primaries_y[1], LSB
                DDU8 Byte10; // display_primaries_y[1], MSB
            };
        };
        union
        {
            DDU16 DisplayPrimariesX2;
            struct
            {
                DDU8 Byte11; // display_primaries_x[2], LSB
                DDU8 Byte12; // display_primaries_x[2], MSB
            };
        };
        union
        {
            DDU16 DisplayPrimariesY2;
            struct
            {
                DDU8 Byte13; // display_primaries_y[2], LSB
                DDU8 Byte14; // display_primaries_y[2], MSB
            };
        };
        union
        {
            DDU16 WhitePointX;
            struct
            {
                DDU8 Byte15; // white_point_x, LSB
                DDU8 Byte16; // white_point_x, MSB
            };
        };
        union
        {
            DDU16 WhitePointY;
            struct
            {
                DDU8 Byte17; // white_point_y, LSB
                DDU8 Byte18; // white_point_y, MSB
            };
        };
        union
        {
            DDU16 MaxDisplayMasteringLuminance;
            struct
            {
                DDU8 Byte19; // max_display_mastering_luminance, LSB
                DDU8 Byte20; // max_display_mastering_luminance, MSB
            };
        };
        union
        {
            DDU16 MinDisplayMasteringLuminance;
            struct
            {
                DDU8 Byte21; // min_display_mastering_luminance, LSB
                DDU8 Byte22; // min_display_mastering_luminance, MSB
            };
        };
        union
        {
            DDU16 MaxCLL;
            struct
            {
                DDU8 Byte23; // max_content_light_level, LSB
                DDU8 Byte24; // max_content_light_level, MSB
            };
        };
        union
        {
            DDU16 MaxFALL;
            struct
            {
                DDU8 Byte25; // max_frame_average_light_level, LSB
                DDU8 Byte26; // max_frame_average_light_level, MSB
            };
        };
    };
} DRM_IF;

//
// AVI Infoframe structure for customization
//
typedef struct _AVI_INFOFRAME_CUSTOM
{
    GUID Guid;                            // GUID
    DDU32 Command;                        // Command
    DDU32 Flags;                          // Flags
    DDU32 TypeCode;                       // Type code of AVI Infoframe
    DDU32 Version;                        // Version of AVI Infoframe
    DDU32 Length;                         // Length of AVI Info Frame
    DD_BOOL R3R0Valid;                    // Reserved
    DD_BOOL ITContent;                    // IT Content
    DDU8 YBarInfo[8];                     // Reserved
    DDU32 ActiveFormatAspectRatio;        // Reserved
    DDU32 NonUniformScaling;              // Reserved
    DDU32 RgbYccIndicator;                // Reserved
    DDU32 ExtColorimetry;                 // Reserved
    DDU32 PixelFactor;                    // Reserved
    DDU32 BarInfoValid;                   // Reserved
    DDU32 Colorimetry;                    // Reserved
    DDU32 AspectRatio;                    // Reserved
    DDU32 QuantRange;                     // RGB Quantization Range
    DDU32 VideoCode;                      // Reserved
    DDU32 ScanInfo;                       // Scan Information
    DDU32 ITContentType;                  // IT Content Type
    DD_AVI_IT_CONTENT_TYPE ITContentCaps; // IT Content Type Caps
} AVI_INFOFRAME_CUSTOM;

//
// CEA Short Audio Descriptor
//
typedef struct _CEA_861B_ADB
{
    union
    {
        DDU8 Byte1;
        struct
        {
            DDU8 MaxChannels : 3;      // Bits[0-2]
            DDU8 AudioFormatCode : 4;  // Bits[3-6], see AUDIO_FORMAT_CODES
            DDU8 UNIQUENAME(Rsvd) : 1; // Bit[7] - reserved
        };
    };
    union
    {
        DDU8 Byte2;
        struct
        {
            DDU8 _32kHz : 1;     // Bit[0] sample rate = 32kHz
            DDU8 _44kHz : 1;     // Bit[1] sample rate = 44kHz
            DDU8 _48kHz : 1;     // Bit[2] sample rate = 48kHz
            DDU8 _88kHz : 1;     // Bit[3] sample rate = 88kHz
            DDU8 _96kHz : 1;     // Bit[4] sample rate = 96kHz
            DDU8 _176kHz : 1;    // Bit[5] sample rate = 176kHz
            DDU8 _192kHz : 1;    // Bit[6] sample rate = 192kHz
            DDU8 B2Reserved : 1; // Bit[7] - reserved
        };
    };
    union
    {
        DDU8 Byte3; // maximum bit rate divided by 8kHz
        // following is the format of 3rd byte for uncompressed(LPCM) audio
        struct
        {
            DDU8 _16Bit : 1;           // Bit[0]
            DDU8 _20Bit : 1;           // Bit[1]
            DDU8 _24Bit : 1;           // Bit[2]
            DDU8 UNIQUENAME(Rsvd) : 5; // Bits[3-7]
        };
    };
} CEA_861B_ADB;

//
// Audio capability structure
//
typedef struct _DEVICE_AUDIO_CAPS
{
    DDU32 NPLDesign : 8; // max number of audio packets device can
    // deliver per line
    DDU32 K0 : 8;        // The overhead(in pixels) per line requied
    // by device for setting up audio packets when
    // CP is disabled
    DDU32 K1 : 8;        // The overhead(in pixels) per line requied
    // by device for setting up audio packets when
    // CP is enabled
// Misc data
    DDU32 PR : 4;               // Pixel Replication value
    DDU32 IsHDCP : 1;           // Driver, Device and Receiver support HDCP
    DDU32 IsRPTR : 1;           // Receiver is HDCP repeater
    DDU32 UNIQUENAME(Rsvd) : 2; // reserved bits
} DEVICE_AUDIO_CAPS;

typedef struct _AUDIO_ENABLE_FLAGS
{
    DDU32 IsHDMIDisplay : 1; // 1 if HDMI display, 0 if not HDMI display
    DDU32 IsELDValid : 1;    // 1 if ELD valid, 0 if ELD not valid
    DDU32 UNIQUENAME(Rsvd) : 30;
} AUDIO_ENABLE_FLAGS;

//
// Data structure to exchange HDMI data through GetSetParameters interface
//
typedef struct _HDMI_PARAMETERS
{
    GUID Guid;
    HDMI_COMMAND Command;
    DDU8 Type;
    DDU8 Size;
    union
    {
        AVI_IF AviInfoFrame;
        SPD_IF SpdInfoFrame;
        VS_IF VsInfoFrame;
    };
} HDMI_PARAMETERS;

//
// Audio format codes
//
typedef enum _AUDIO_FORMAT_CODES
{
    AUDIO_LPCM = 0x0001, // Linear PCM (eg. IEC60958)
    AUDIO_AC3 = 0x0002, // AC-3
    AUDIO_MPEG1 = 0x0003, // MPEG1 (Layers 1 & 2)
    AUDIO_MP3 = 0x0004, // MP3   (MPEG1 Layer 3)
    AUDIO_MPEG2 = 0x0005, // MPEG2 (multichannel)
    AUDIO_AAC = 0x0006, // AAC
    AUDIO_DTS = 0x0007, // DTS
    AUDIO_ATRAC = 0x0008, // ATRAC
    AUDIO_OBA = 0x0009, // One Bit Audio
    AUDIO_DOLBY_DIGITAL = 0x000A, // Dolby Digital
    AUDIO_DTS_HD = 0x000B, // DTS-HD
    AUDIO_MAT = 0x000C, // MAT (MLP)
    AUDIO_DST = 0x000D, // DST
    AUDIO_WMA_PRO = 0x000E  // WMA Pro
} AUDIO_FORMAT_CODES;

typedef union _HDMI_SINK_HARDWARE_REVISION
{
    struct
    {
        DDU8 MinorRevision : 4; // Bit 3:0
        DDU8 MajorRevision : 4; // Bit 7:4
    };

    DDU8 Value;
} HDMI_SINK_HARDWARE_REVISION;

typedef union _HDMI_SINK_MAUFACTURER_ID
{
    DDU8 Value[13];
    struct
    {
        DDU8 SourceOUI[3];
        DDU8 SourceDeviceIdString[7];
        HDMI_SINK_HARDWARE_REVISION HwRevision;
        DDU8 SwMajorRevision;
        DDU8 SwMinorRevision;
    };
} HDMI_SINK_MAUFACTURER_ID;

/*
 * SCDC register definitions
 * Status and Data Control Channel and Structure for HDMI 2.x
 */

 /*===============================================================================================
  *  SCDC Register Name:     Source and Sink version
  *  SCDC Register Address:  0x01, 0x02
  *================================================================================================*/

#define SCDC_SINK_VERSION_OFFSET 0x01
#define SCDC_SOURCE_VERSION_OFFSET 0x02

#define SCDC_SINK_VERSION 0x01
#define SCDC_SOURCE_VERSION 0x01

  /*===============================================================================================
   *  SCDC Register Name:     Update Flags
   *  SCDC Register Address:  0x10, 0x11
   *================================================================================================*/
#define SCDC_UPDATE_READ_OFFSET 0x10

typedef union _SCDC_UPDATE_FLAGS
{
    DDU8 Value;
    struct
    {
        /** The Sink shall set this bit when a value is changed in the offset 0x40 of the Status Flags register. */
        /** The Source may write this bit to 1 to clear it. */
        DDU8 StatusUpdate : 1; // Bit 0
        /** Character error detection update. */
        /** The Source may write this bit to 1 to clear it. */
        DDU8 CedUpdate : 1; // Bit 1
        /** The Sink shall set (=1) this bit when a test Read Request is generated in response to the setting (=1). */
        /** of the TestReadRequest bit in the Test Configuration register. */
        DDU8 RrTest : 1; // Bit 2
        /** The Sink shall set (=1) this bit when it updates the setting of the Source Test Configuration registers. */
        /** The Source may write this bit to 1 to clear it. */
        DDU8 SourceTestUpdate : 1; // Bit 3
        /** The Sink shall set this bit when Link Training is successful and the Sink is ready to receive video. */
        DDU8 FrlStart : 1; // Bit 4
        /** The Sink shall set this bit when a value is changed in the offsets 0x41, 0x42 of the Status Flags reg. */
        /** The Source may write this bit to 1 to clear it. */
        DDU8 FltUpdate : 1; // Bit 5
        /** Reed-Solomon Character error detection update .*/
        /** The Source may write this bit to 1 to clear it. */
        DDU8 RsedUpdate : 1;       // Bit 6
        DDU8 UNIQUENAME(Rsvd) : 1; // Bit 7
    };
} SCDC_UPDATE_FLAGS;

/*===============================================================================================
 *  SCDC Register Name:     TMDS configuration
 *  SCDC Register Address:  0x20
 *================================================================================================*/
#define SCDC_TMDS_CONFIG_OFFSET 0x20

typedef union _SCDC_TMDS_CONFIG
{
    DDU8 Value;
    struct
    {
        /** The Source shall set (=1) this bit to enable scrambling in the Sink.*/
        DDU8 Scrambling : 1; // Bit 0
        /** 0 = (TMDS Bit Period)/(TMDS Clock Period) ratio is 1/10 . */
        /** 1 = (TMDS Bit Period)/(TMDS Clock Period) ratio is 1/40 . */
        DDU8 HighTmdsCharRate : 1; // Bit 1
        DDU8 UNIQUENAME(Rsvd) : 6; // Bits 7:2
    };
} SCDC_TMDS_CONFIG;

/*===============================================================================================
 *  SCDC Register Name:     TMDS Scrambler status
 *  SCDC Register Address:  0x21
 *================================================================================================*/
#define SCDC_SCRAMBLER_STS_OFFSET 0x21

typedef union _SCDC_SCRAMBLER_STS
{
    DDU8 Value;
    struct
    {
        /** This bit applies to TMDS Scrambling only. This bit shall be set (=1) by the Sink when the Sink Device
         detects scrambled control code sequences and reset (=0) when the Sink does not detect scrambled control
         code sequences. */
        DDU8 ScramblingEnabled : 1; // Bit 0
        DDU8 UNIQUENAME(Rsvd) : 7;  // Bits 7:1
    };
} SCDC_SCRAMBLER_STS;

/*===============================================================================================
 *  SCDC Register Name:    Sink Configuration 0
 *  SCDC Register Address:  0x30
 *================================================================================================*/
#define SCDC_CONFIG_0_OFFSET 0x30

typedef union _SCDC_CONFIG_0
{
    DDU8 Value;
    struct
    {
        /** The Source shall set this bit (=1) when the Source supports ReadRequest. The Source shall reset this
         bit (=0) when the Source only supports polling of the update flags. The Sink shall reset (=0) this bit when
         the SCDC of the Sink goes from the disabled to the enabled state.*/
        DDU8 RrEnable : 1; // Bit 0
        /** The Source shall not set this bit. Only test equipment functioning as a Source may set this field. */
        DDU8 FltNoRetain : 1;      // Bit 1
        DDU8 DaisyErr : 1;         // Bit 2
        DDU8 MonoDirOn : 1;        // Bit 3
        DDU8 MonoDirErr : 1;       // Bit 4
        DDU8 CaPwrErr : 1;         // Bit 5
        DDU8 UNIQUENAME(Rsvd) : 2; // Bits 7:6
    };
} SCDC_CONFIG_0;

/*===============================================================================================
 *  SCDC Register Name:    Sink Configuration 1
 *  SCDC Register Address:  0x31
 *================================================================================================*/
typedef enum _FRL_RATE
{
    HDMI_DISABLE_FRL = 0,
    HDMI_FRLRATE_3GBPS_3LANES,
    HDMI_FRLRATE_6GBPS_3LANES,
    HDMI_FRLRATE_6GBPS_4LANES,
    HDMI_FRLRATE_8GBPS_4LANES,
    HDMI_FRLRATE_10GBPS_4LANES,
    HDMI_FRLRATE_12GBPS_4LANES,
} FRL_RATE;
#define SCDC_CONFIG_1_OFFSET 0x31

typedef union _SCDC_CONFIG_1
{
    DDU8 Value;
    struct
    {
        /** Sources select the FRL rate and Lane count by writing into this register. This field is written by the
         Source during the Link Training protocol.*/
        DDU8 FrlRate : 4; // Bits 3:0
        // 0 - Disable FRL
        // 1 - 3 Gbps, 3 Lanes
        // 2 - 6 Gbps, 3 Lanes
        // 3 - 6 Gbps, 4 Lanes
        // 4 - 8 Gbps, 4 Lanes
        // 5 - 10 Gbps, 4 Lanes
        // 6 - 12 Gbps, 4 Lanes, Others, reserved
/** The Source shall set this field to indicate the maximum TxFFE level supported for the current FRL Rate.
 Values greater than 3 are reserved.*/
        DDU8 FfeLevels : 4; // Bits 7:4
    };
} SCDC_CONFIG_1;

/*===============================================================================================
 *  SCDC Register Name:    Source test configuration
 *  SCDC Register Address:  0x35
 *================================================================================================*/
#define SCDC_SOURCE_TEST_CONFIG_OFFSET 0x35

 /**
  * @brief Not used by source, Sink acting as test equipment only can set these fields.
  * @todo:VSDI-18381 - Revisit later if its needed for compliance.
  */
typedef union _SCDC_SOURCE_TEST_CONFIG
{
    DDU8 Value;
    struct
    {
        DDU8 UNIQUENAME(Rsvd) : 1;    // Bit 0
        DDU8 TxFfePreShootOnly : 1;   // Bit 1
        DDU8 TxFfeDeEmphasisOnly : 1; // Bit 2
        DDU8 TxFfeNoFfe : 1;          // Bit 3
        DDU8 UNIQUENAME(Rsvd) : 1;    // Bit 4
        DDU8 FltNoTimeout : 1;        // Bit 5
        DDU8 DscFrlMax : 1;           // Bit 6
        DDU8 FrlMax : 1;              // Bit 7
    };
} SCDC_SOURCE_TEST_CONFIG;

/*===============================================================================================
 *  SCDC Register Name:    Status Flags 0
 *  SCDC Register Address:  0x40
 *================================================================================================*/
#define SCDC_STATUS_FLAG_0_OFFSET 0x40

typedef union _SCDC_STATUS_FLAGS_0
{
    DDU8 Value;
    struct
    {
        /** This bit shall be set (=1) by the Sink when the Sink Device detects a valid TMDS clock signal and reset
         by the Sink if this condition no longer exists. In FRL mode the Sink shall reset (=0) this field. */
        DDU8 ClockDetected : 1; // Bit 0
        /** The Sink shall set this bit when the Sink is successfully decoding TMDS data or FRL data. */
        DDU8 Ch0Ln0Locked : 1; // Bit 1
        /** The Sink shall set this bit when the Sink is successfully decoding TMDS data or FRL data. */
        DDU8 Ch1Ln1Locked : 1; // Bit 2
        /** The Sink shall set this bit when the Sink is successfully decoding TMDS data or FRL data. */
        DDU8 Ch2Ln2Locked : 1; // Bit 3
        /** The Sink shall set this bit when the Sink is successfully decoding FRL data. */
        DDU8 Ln3Locked : 1;        // Bit 4
        DDU8 UNIQUENAME(Rsvd) : 1; // Bit 5
        /** The Sink shall set (=1) FLT_ready when the Sink is ready for Link Training. */
        DDU8 FltReady : 1; // Bit 6
        /** Sinks may utilize this bit to indicate that an error state has occurred in the
         Sink that is preventing proper decode of a VESA DSC 1.2a stream. */
        DDU8 DscDecodeFail : 1; // Bit 7
    };
} SCDCS_STATUS_FLAGS_0;

/*===============================================================================================
 *  SCDC Register Name:    Status Flags 0
 *  SCDC Register Address:  0x41
 *================================================================================================*/
#define SCDC_STATUS_FLAG_1_OFFSET 0x41

typedef union _SCDC_STATUS_FLAGS_1
{
    DDU8 Value;
    struct
    {
        DDU8 Ln0LtpRequest : 4; // Bits 3:0
        DDU8 Ln1LtpRequest : 4; // Bits 7:4
    };
} SCDC_STATUS_FLAGS_1;

/*===============================================================================================
 *  SCDC Register Name:    Status Flags 0
 *  SCDC Register Address:  0x41
 *================================================================================================*/
#define SCDC_STATUS_FLAG_2_OFFSET 0x42

typedef union _SCDC_STATUS_FLAGS_2
{
    DDU8 Value;
    struct
    {
        DDU8 Ln2LtpRequest : 4; // Bits 3:0
        DDU8 Ln3LtpRequest : 4; // Bits 7:4
    };
} SCDC_STATUS_FLAGS_2;

/*===============================================================================================
 *  SCDC Register Name:     CED - Character Error Detection Counter
 *  SCDC Register Address:  0x50 (For TMDS/3 Lane FRL 0x50 - 0x56 are valid offsets, For 4 Lane FRL 0x50 - 0x58 are valid offsets.)
 *================================================================================================*/
#define SCDC_NUM_3_LANE_VALID_CED_ERROR_COUNTERS 7 // For TMDS/3 Lane FRL 0x50 - 0x56 are valid offsets
#define SCDC_NUM_4_LANE_VALID_CED_ERROR_COUNTERS 9 // For 4 Lane FRL 0x50 - 0x58 are valid offsets

#define SCDC_CED_COUNTER_START_OFFSET 0x50

 /*===============================================================================================
  *  SCDC Register Name:     RSED - Reed-Solomon Error Detection Counter
  *  SCDC Register Address:  0x59 (0x59 - 0x5A)
  *================================================================================================*/
#define SCDC_NUM_RSED_VALID_ERROR_COUNTERS 2

#define SCDC_RSED_COUNTER_START_OFFSET 0x59

  /*===============================================================================================
   *  SCDC Register Name:     Manufacturer IEEE OUI ID
   *  SCDC Register Address:  0xD0 (0xD0 - 0xD2)
   *================================================================================================*/
#define SCDC_MANUFACTURER_OUI_ID 0xD0

   /*===============================================================================================
    *  HDMI 2.1 Extended meta data packet definitions
    * Refer HDMI 2.1 spec for additional details
    *================================================================================================*/
#define NUM_EMP_CONTROL_BYTES 7

    // Composition of a 32 Byte DIP:
    //   Header : 3 bytes
    //   Header ECC : 1 byte
    //   Payload : 24 bytes
    //   Payload ECC : 4 bytes
    // Each HDMI packet is 7DWs. So we need to write min of 7 DWs when we send any EMP packet even when we have less than 7DWs to write.
    // We will get FIFO underrun if we don't do this.
#define DATA_SET_FRAGMENT_LENGTH 28 // To indicate in EMP_HEADER reg # of DIPs (Total # of payload bytes to send/ # of payload bytes per DIP) sent in EMP payload
#define MAX_DSF_SUPPORTED 4

#define VTEMP_EMP_SIZE 11

typedef enum _HDMI_QMS_NEXT_TFR
{
    HDMI_QMS_NEXT_TFR_INACTIVE = 0,
    HDMI_QMS_NEXT_TFR_24_1001,
    HDMI_QMS_NEXT_TFR_24,
    HDMI_QMS_NEXT_TFR_25,
    HDMI_QMS_NEXT_TFR_30_1001,
    HDMI_QMS_NEXT_TFR_30,
    HDMI_QMS_NEXT_TFR_48_1001,
    HDMI_QMS_NEXT_TFR_48,
    HDMI_QMS_NEXT_TFR_50,
    HDMI_QMS_NEXT_TFR_60_1001,
    HDMI_QMS_NEXT_TFR_60,
    HDMI_QMS_NEXT_TFR_100,
    HDMI_QMS_NEXT_TFR_120_1001,
    HDMI_QMS_NEXT_TFR_120,
} HDMI_QMS_NEXT_TFR;

/**
 * @brief Payload byte 0 definition.
 *
 */
typedef union _HDMI_EMP_PB0
{
    DDU8 Value;
    struct
    {
        DDU8 UNIQUENAME(Rsvd) : 1;
        DDU8 Sync : 1;   /** Source shall set this while sending synchronous EMPs.*/
        DDU8 Vfr : 1;    /** Video Format Related indicator.*/
        DDU8 Afr : 1;    /** Audio Format Related indicator.*/
        DDU8 DsType : 2; /** EM Data Set Type; 00 - Periodic pseudo-static, 01 - Periodic dynamic, 10 - Unique data set, 11 - Reserved.*/
        DDU8 End : 1;    /** Indicates last data set transmission, refer spec for additional details. */
        DDU8 New : 1;    /** Indicates first/new data set transmission refer spec for additional details. */
    };
} HDMI_EMP_PB0;

/**
 * @brief Payload byte 7 definition.
 *
 */
typedef union _HDMI_VTEM_MD0
{
    DDU8 Value;
    struct
    {
        // Applicable only for Class 0 VTEM (Sync = 0)
        // When set (=1), this indicates that Gaming-VRR mode is enabled.
        DDU8 VrrEn : 1;

        // Applicable only for Class 1 VTEM (Sync = 1 in PB0):
        //      M_CONST = 1 indicates that the value of Mvrr is constant or dithers between two sequential values.
        //      M_CONST = 0 indicates that frame rate is getting changed or QMS is getting disabled.
        DDU8 Mconst : 1;

        // Applicable only for Class 1 VTEM.
        // When set (=1), this indicates that QMS-VRR mode is enabled.
        DDU8 QmsEnable : 1;

        DDU8 UNIQUENAME(Rsvd) : 1;

        // Applicable only for Class 0 VTEM.
        // The Source shall set this value to accurately indicate the current FVA_Factor minus; Refer spec for details on FVA_Factor.
        DDU8 FvaFactorM1 : 4;
    };
} HDMI_VTEM_MD0;

/**
 * @brief Payload byte 9 definition.
 *
 */
typedef union _HDMI_VTEM_MD2
{
    DDU8 Value;
    struct
    {
        DDU8 BaseRrMsb : 2; /** Higher 2 bits of Base RR. */

        // Bit 2 was assigned to 'Reduced Blanking' in HDMI 2.1a which got removed in HDMI 2.1b
        DDU8 UNIQUENAME(Rsvd) : 1; /** Reserved.*/

        // Applicable only for Class 1 VTEM.
        // When QMS-VRR is enabled and M_CONST=1, indicates the refresh rate in Hz of the frame following the FAPA carrying this value.
        // When QMS-VRR is enabled and M_CONST=0, indicates the anticipated refresh rate of the video once M_CONST is set to 1.
        DDU8 NextTfr : 5;
    };
} HDMI_VTEM_MD2;

typedef union _HDMI_VTEM_EMP
{
    DDU8 EmpBuffer[VTEMP_EMP_SIZE]; // HW has only max of 4 packets of buffer, VTEM uses only 1 packet which is 24 bytes
    struct
    {
        HDMI_EMP_PB0 Pb0;      // PB0 Refer PB0 definition
        DDU8 UNIQUENAME(Rsvd); // PB1 Reserved
        DDU8 OrgId;            // PB2 Organization ID where definition is present; 0 - Vendor specific, 1 - HDMI 2.1 spec, 2 - CTA-861-G, 3 - VESA, others - Reserved
        DDU8 DsTagMsb;         // PB3 Data set tag MSB, defined by Organization indicated in OrgID
        DDU8 DsTagLsb;         // PB4 Data set tag LSB
        DDU8 DsLengthMsb;      // PB5 This field indicates the number of bytes contained within the EM Data Set starting from PB7 of the EMP carrying the first DataSetFragment of the EM Data Set
        DDU8 DsLengthLsb;      // PB6 Data set Length LSB, see definition above
        HDMI_VTEM_MD0 Pb7;     // PB7 PB7 - PB27 is transmitted as MD0-MD20. In case of VRR, this field indicates MD0, subsequent fields are MD1, MD2 and so on...
        DDU8 BaseVfront;       // Base Vertical front
        HDMI_VTEM_MD2 Pb9;     // PB9 PB7 - PB27 is transmitted as MD0-MD20. In case of VRR, this field indicates MD2.
        DDU8 BaseRrLsb;        // Indicates the refresh rate of the Base Timing Format. Valid values are 50 Hz to 1023 Hz.
    };
} HDMI_VTEM_EMP;

typedef union _HDMI_VSEM_EMP
{
    /** HW has only max of 4 packets of buffer, each packet, also called as DSF (Data set fragment) is 28 bytes, VSEM can be supported upto 4 DSFs. */
    DDU8 EmpBuffer[DATA_SET_FRAGMENT_LENGTH * MAX_DSF_SUPPORTED];
    struct
    {
        HDMI_EMP_PB0 Pb0;      // PB0 Refer PB0 definition
        DDU8 UNIQUENAME(Rsvd); // PB1 Reserved
        DDU8 OrgId;            // PB2 Organization ID where definition is present; 0 - Vendor specific, 1 - HDMI 2.1 spec, 2 - CTA-861-G, 3 - VESA, others - Reserved
        DDU8 DsTagMsb;         // PB3 Data set tag MSB, defined by Organization indicated in OrgID
        DDU8 DsTagLsb;         // PB4 Data set tag LSB
    };
} HDMI_VSEM_EMP;

//
// HDR10+ Vendor Specific InfoFrame structure
//

typedef struct _VSIF_HEADER
{
    union
    {
        DDU16 Value;
        struct
        {
            DDU8 Type;    // InfoFrame Type
            DDU8 Version; // InfoFrame Version
        };
    };
    DDU8 Length : 5; // InfoFrame Length
    DDU8 UNIQUENAME(Rsvd) : 3;
} VSIF_HEADER;

typedef union _VSIF_PB
{
    DDU8 PB[28];
    struct
    {
        DDU8 Chksum;       // PB 0 - Checksum
        DDU8 IeeeRegID[3]; // PB 1-3 - IEEE registration ID

        union
        {
            DDU8 PB4;
            struct
            {
                DDU8 UNIQUENAME(Rsvd) : 1;
                DDU8 MaxLuminance : 5; // Targeted system display maximum luminance
                DDU8 ApplicationVersion : 2;
            };
        };
        DDU8 UNIQUENAME(Rsvd)[22];

        union
        {
            DDU8 PB27;
            struct
            {
                DDU8 UNIQUENAME(Rsvd) : 4;
                DDU8 ALLM : 1;
                DDU8 SourceTM_flag : 1;
                DDU8 VsIf_Timing_Mode : 1;
                DDU8 Graphics_Overlay_Flag : 1;
            };
        };
    };
} VSIF_PB;

typedef union _HDR10P_VS_IF
{
    DDU8 VsBuf[VS_MAX_TOTAL_LENGTH];
    struct
    {
        VSIF_HEADER VsIfHeader; // VSIF header data
        VSIF_PB VsIfPb;         // PB : 0-27
    };
} HDR10P_VS_IF;

#pragma pack()
#endif //__IHDMI_H__
