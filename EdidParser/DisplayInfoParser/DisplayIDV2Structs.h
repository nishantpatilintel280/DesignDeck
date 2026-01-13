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
 * @file  DisplayIDV2Structs.h
 * @brief This file contains all data structures used from Display ID 2.0 spec.
 *
 */

#pragma once

 //-----------------------------------------------------------------------------
 //
 // Generic includes
 //
 //-----------------------------------------------------------------------------

#include "DisplayIDStructs.h"

//---------------------------------------------------------------------------
//
// CONSTANTs / ENUMs
// Only define which is extra for DID2.0
//
//---------------------------------------------------------------------------
#define DID_2_0_MAX_DATA_BLOCK_TAGS 12
#define DID_2_0_DOT_CLOCK_SCALE 1000
#define DISPLAY_ID_VER_2_0 0x20
#pragma pack(1)

/***************************************************************
 * @brief  This Enum is used for identifying different data blocks in Display ID section. (Refer Table 3.1).
 * The Mandatory Block IDs are Marked as "M" and Non Mandatory as "NM".
 ***************************************************************/
typedef enum _DISPLAYID2_0_DATA_BLOCK_TAG
{
    /** M  PRODUCTID              0x20 .*/
    DID2_DATA_BLOCK_PRODUCTID = 0x20,
    /** M  DISPLAY_PARAMS         0x21 .*/
    DID2_DATA_BLOCK_DISPLAY_PARAMS,
    /** M  Detailed VII TIMING    0x22 .*/
    DID2_DATA_BLOCK_DETAIL_TIMING_VII,
    /** NM Enumerated_VIII_TIMNG  0x23 .*/
    DID2_DATA_BLOCK_ENUMERATED_TIMING_VIII,
    /** NM Formula Based IX Times 0x24 .*/
    DID2_DATA_BLOCK_FORMULA_TIMING_IX,
    /** NM Dynamic Video Timing   0x25 .*/
    DID2_DATA_BLOCK_DYNAMIC_VIDEO_TIMING,
    /** M  Display I/F Features   0x26 .*/
    DID2_DATA_BLOCK_DISPLAY_IF_FEATURES,
    /** NM Stereo Disp Interface  0x27 .*/
    DID2_DATA_BLOCK_STEREO_DISPLAY_INTERFACE,
    /** NM Tiled Display Topology 0x28 .*/
    DID2_DATA_BLOCK_TILED_DISPLAY_TOPOLOGY,
    /** NM Container ID           0x29 .*/
    DID2_DATA_BLOCK_CONTAINER_ID,
    /** NM Formula Based X Times 0x2A  .*/
    DID2_DATA_BLOCK_FORMULA_TIMING_X = 0x2A,
    /** NM Vendor Specific Data   0x7E .*/
    DID2_DATA_BLOCK_VENDOR_SPECIFIC = 0x7E,
    /** NM CTA Display ID         0x81 .*/
    DID2_DATA_BLOCK_CTA_DISPLAY_ID = 0x81,
} DISPLAYID2_0_DATA_BLOCK_TAG;

/***************************************************************
 * @brief       This Enum defines Display-ID Product Type.
 *              Refer Table 2.3 of Display ID spec 2.0 .
 ***************************************************************/
typedef enum _DISPLAYID2_0_PRODUCT_TYPE
{
    /** Extension section        0x00. */
    DID2_PTYPE_EXTENSION = 0x00,
    /** Test Structure           0x01. */
    DID2_PTYPE_TEST_STRUCTURE,
    /** Generic                  0x02. */
    DID2_PTYPE_GENERIC,
    /** Television               0x03. */
    DID2_PTYPE_TELEVISION,
    /** Productivity             0x04. */
    DID2_PTYPE_PRODUCTIVITY,
    /** Gaming                   0x05. */
    DID2_PTYPE_GAMING,
    /** Prsentation              0x06. */
    DID2_PTYPE_PRESENTATION,
    /** Virtual Reality          0x07. */
    DID2_PTYPE_VIRTUAL_REALITY,
    /** Augmented Reality        0x08. */
    DID2_PTYPE_AUGMENTED_REALITY,
    /** Above 08 it is Reserved.       */
    DID2_PTYPE_RESERVED,
} DISPLAYID2_0_PRODUCT_TYPE;

//---------------------------------------------------------------------------
//
//  Display ID-2.0 Data Block header
//  Header: 4 bytes (Table 3.2 of Display ID spec 2.0)
//  For better common functionality across DID1.3 & 2.0,
//  same DISPLAYID_HEADER struct is maintained.
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Start of Display Id 2.0 Data blocks
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// Product ID data block
// Tag ID: 0x20 - DID2_DATA_BLOCK_PRODUCTID
// Size - 12 mandatory bytes + Variable size block for Product ID string
// Refer Table 4.1
// The Struct is maintained same as for DID1.3. As DID1.3 struct doesn't
// reflect OUI Info, the new struct will be DISPLAYID_PNP_OUI_ID
//---------------------------------------------------------------------------

/***************************************************************
 * @brief      This Enum defines Scan Orientation as Support Flags
 *              in Data Block 0x21 (Refer Table 4.10).
 ***************************************************************/
typedef enum _DISPLAYID_2_0_SCAN_ORIENTATION
{
    /** Default Scan_Left_Right_Top_Bottom   0x00 .*/
    DID2_SCAN_LEFT_RIGHT_TOP_BOTTOM = 0x00,
    /** Scan_Right_Left_Top_Bottom           0x01 .*/
    DID2_SCAN_RIGHT_LEFT_TOP_BOTTOM,
    /** Scan_Top_Bottom_Right_Left           0x02 .*/
    DID2_SCAN_TOP_BOTTOM_RIGHT_LEFT,
    /** Scan_Bottom_Top_Right_Left           0x03 .*/
    DID2_SCAN_BOTTOM_TOP_RIGHT_LEFT,
    /** Scan_Right_Left_Bottom_Top           0x04 .*/
    DID2_SCAN_RIGHT_LEFT_BOTTOM_TOP,
    /** Scan_Left_Right_Bottom_Top           0x05 .*/
    DID2_SCAN_LEFT_RIGHT_BOTTOM_TOP,
    /** Scan_Bottom_Top_Left_Right           0x06 .*/
    DID2_SCAN_BOTTOM_TOP_LEFT_RIGHT,
    /** Scan_Top_Bottom_Left_Right           0x07 .*/
    DID2_SCAN_TOP_BOTTOM_LEFT_RIGHT,
} DISPLAYID_2_0_SCAN_ORIENTATION;

/***************************************************************
 * @brief  This Enum defines Native Color Bit Depth bpc(Bits per Component).
 * in Data Block 0x21 (Refer Table 4.14).
 ***************************************************************/
typedef enum _DISPLAYID_2_0_COLOR_BIT_DEPTH
{
    /** Default Color_Depth_Not_Defined  0x00 .*/
    DID2_COLOR_DEPTH_NOT_DEFINED = 0x00,
    /** Color_Depth_6BPC                 0x01 .*/
    DID2_COLOR_DEPTH_6_BPC,
    /** Color_Depth_8BPC                 0x02 .*/
    DID2_COLOR_DEPTH_8_BPC,
    /** Color_Depth_10BPC                0x03 .*/
    DID2_COLOR_DEPTH_10_BPC,
    /** Color_Depth_12BPC                0x04 .*/
    DID2_COLOR_DEPTH_12_BPC,
    /** Color_Depth_16BPC                0x05 .*/
    DID2_COLOR_DEPTH_16_BPC,
    /** Color_Depth_Reserved1            0x06 .*/
    DID2_COLOR_DEPTH_RESERVED1,
    /** Color_Depth_Reserved7            0x07 .*/
    DID2_COLOR_DEPTH_RESERVED2,
} DISPLAYID_2_0_COLOR_BIT_DEPTH;

/***************************************************************
 * @brief       This struct defines Revision & Other Data Info of
 *              Data Block 0x21 (Refer Table 4.7 ).
 ***************************************************************/
typedef struct _DISPLAYID_2_0_DISPLAY_PARAMS_REVID
{
    union
    {
        /** Data Block Revision and Other Data of 0x21. */
        DDU8 RevAndOtherData;
        struct
        {
            /** Bits 0-2:    Revision. */
            DDU8 Revision : 3;
            /** Bits 3-6:    Reserved. */
            DDU8 UNIQUENAME(Rsvd) : 4;
            /** Bit 7:       ImagePrecision
             * If Bit Value is 0, it means Prescision is 0.1 mm
             * If Bit Value is 1, it means Prescision is 1 mm.
             */
            DDU8 ImagePrecision : 1;
        };
    };
} DISPLAYID_2_0_DISPLAY_PARAMS_REVID;

/***************************************************************
 * @brief The struct "Display Parameters Data Block".
 *
 *              Tag ID: 0x21 - DID2_DATA_BLOCK_DISPLAY_PARAMS
 *              Size - 29 byte Block (Refer Table 4.7)
 *
 *              Chromaticity Data in this Block 0x21 (table 4.11)
 *              Represents 12 Bit (x,y) or (u',v');
 *              Uses 12-bit fractional Integer Numbers
 *              (bit12 x 2^-1 + .. + bit0 x 2^-12)
 *              Same DID1.3 struct DID_CHROMATICITY_DATA for DID2.
 ***************************************************************/
typedef struct _DISPLAYID_2_0_DISPLAY_PARAMETERS
{
    /** Block header of 0x21 Data Block.   */
    DISPLAYID_BLOCK BlockHeader;

    /** Final HImage Size (2 Bytes in mm) is calculated as
     * = HImageSize * Image size Multiplier (ImagePrecision).
     */
    DDU16 HImageSize;
    /** Final VImage Size (2 Bytes in mm) is calculated as
     * = VImageSize * Image size Multiplier (ImagePrecision).
     */
    DDU16 VImageSize;
    /** Horizontal Pixel count of 2 bytes. */
    DDU16 HPixelCount;
    /** Vertical Pixel count of 2 bytes.   */
    DDU16 VPixelCount;
    union
    {
        /** 1 Byte Feature Support Flags (Table 4.10).    */
        DDU8 SupportFlags;
        struct
        {
            /** Bits 0-2: DISPLAYID_2_0_SCAN_ORIENTATION. */
            DDU8 ScanOrientation : 3;
            /** Bits 3-4: When Native Luminance Values in Data Block
             * 0x21 contains Positive Values; these two bits represents
             * interpretation of those fields.
             * 0 = Min Guaranteed Value, 1 = Guidance for Source Device
             * Value of 2 & 3 are Reserved.
             */
            DDU8 MaxLuminanceinfo : 2;
            /** Bit-5 is Reserved.   */
            DDU8 UNIQUENAME(Rsvd) : 1;
            /** Bit-6: Interpretation of Color Space Info in 0x21 Block
             * 0 = CIE 1931 (x,y) (default) OR
             * 1 = CIE 1976 (u',v').
             */
            DDU8 ColorSpaceInfo : 1;
            /** Bit-7: AudioSpeakerInfo; 0 = Integrated; 1 = External Jack.   */
            DDU8 AudioSpeakerInfo : 1;
        };
    };
    /** Native Primary Colors & Native White Point Chromaticity (Table 4.11). */
    /** Color1 (x,y) or (u',v').      */
    DID_CHROMATICITY_DATA RedChroma;
    /** Color2 (x,y) or (u',v').      */
    DID_CHROMATICITY_DATA GreenChroma;
    /** Color3 (x,y) or (u',v').      */
    DID_CHROMATICITY_DATA BlueChroma;
    /** White Point (x,y) or (u',v'). */
    DID_CHROMATICITY_DATA WhitePointChroma;

    /** Native Luminance related fields in (candela/square meter) (Table 4.13)
     * Uses 16-bit IEEE 754-2008 half-precision floating-point (-0 =Not Used).
     */

     /** Native Maximum Luminance Full Coverage.                */
    DDU16 MaxLuminanceFull;
    /** Native Maximum Luminance for 10% Rectangular Coverage. */
    DDU16 MaxLuminance10Prcnt;
    /** Native Minimum Luminance.                              */
    DDU16 MinLuminanceFull;
    union
    {
        /** Native Color Depth & Display Device tech.          */
        DDU8 ColorBDDispTech;
        struct
        {
            /** Bits 0-2: DID_2_0_COLOR_BIT_DEPTH.             */
            DDU8 ColorBitDepth : 3;
            /** Bit 3: Reserved.                               */
            DDU8 UNIQUENAME(Rsvd) : 1;
            /** Bits 4-6: 1 = AMLCD, 2 = AMOLED.               */
            DDU8 DisplayTech : 3;
            /** Bit 7: Reserved.                               */
            DDU8 UNIQUENAME(Rsvd) : 1;
        };
    };
    /** Native Gamma EOTF value (1.00-3.54).
     * Stored Val = ( Gamma X 100) - 100. (255 unspecified).
     */
    DDU8 GammaEOTF;

} DISPLAYID_2_0_DISPLAY_PARAMETERS;

//---------------------------------------------------------------------------
//
//  Detailed Timing Data Block
//  Tag ID: 0x22 - DID2_DATA_BLOCK_DETAIL_TIMING_VII
//  Size - Variable size block -- 20 bytes per timing * N # of Timings
//  Refer Table 4.17 and 4.18
//  This is similar to TypeI Timing struct of DID1.3 with two differences
//  1. Pixel Clock Scale is 1K for DID2 v/s 10K for DID1.3
//  2. Aspect Ratio is represented by DidV2AspectRatioTable
//  Renamed DID1.3 TypeI struct to DISPLAYID_DTD_TIMING & struct member
//  to DID_TIMINGINFO to represent 0x22 BLock TypeVII of DID2.0
//---------------------------------------------------------------------------

/***************************************************************
 * @brief  This Enum defines "Timing Algorithm Standard".
 ***************************************************************/
typedef enum _CVT_RB_TIMING_FORMULA_VERSION
{
    /** CVT Ver-1.2 Standard Blanking   0x00 .*/
    VESA_CVT_VER2,
    /** CVT Ver-1.1 Reduced Blanking    0x01 .*/
    VESA_CVT_REDUCED_BLANKING_VER1,
    /** CVT Ver-1.2 Reduced Blanking    0x02 .*/
    VESA_CVT_REDUCED_BLANKING_VER2,
    /** CVT Ver-1.2 Reduced Blanking    0x03 .*/
    VESA_CVT_REDUCED_BLANKING_VER3,
    /** Other Values Reserved.                */
    VESA_STANDARD_RESERVED,
} CVT_RB_TIMING_FORMULA_VERSION;

//---------------------------------------------------------------------------
//
//  Enumerated Timing Data Block
//  Tag ID: 0x23 - DID2_DATA_BLOCK_ENUMERATED_TIMING_VIII
//  Size - Variable size block -- Will show all supported modes in enumerated way
//  Refer Table Table 4.19
//  As this is similar to Type4 Timing struct of DID1.3 will use structure
//  DISPLAYID_DMTID_CODE_TIMING_4. Difference is DID2 allows 2 byte Descriptor Codes
//---------------------------------------------------------------------------
typedef struct _DID_TIMINGINFO_TYPE_VIII
{
    union
    {
        /** Data Block Revision and Other Data of Enumerated Timing Block 0x23.   */
        DDU8 RevAndOtherData;
        struct
        {
            /** Bits 0-2: Revision. */
            DDU8 Revision : 3;
            /** Bit 3: TimingCodesize. If 0: One Byte, 1: Two Byte Descriptor    .*/
            DDU8 TimingCodesize : 1;
            /** Bits 4-5: Reserved. */
            DDU8 UNIQUENAME(Rsvd) : 2;
            /** Bits 6-7: Timing Code Type of TIMING_CODE_TYPE defined in DID1.3 .*/
            DDU8 TimingCode : 2;
        };
    };
} DID_TIMINGINFO_TYPE_VIII;

#define TIMINGINFO_IX_SIZE 6

typedef struct _DISPLAYID_2_0_TIMINGINFO_IX
{
    union
    {
        /** Byte-3/9 etc: Timing options.     */
        DDU8 TimingOptions;
        struct
        {
            /** Bits 0-2: Timing Formula/Algorithm of type CVT_RB_TIMING_FORMULA_VERSION. */
            DDU8 TimingFormula : 3;
            /** Bit-3: Reserved. */
            DDU8 UNIQUENAME(Rsvd) : 1;
            /** Bit-4: Refresh Rate supported;
             * 0 - REFRESH RATE*(1000/1001) supported, 1 - not supported.
             */
            DDU8 RRSupported : 1;
            /** Bits 5-6: 3D Stereo support of type S3D_SUPPORT.              */
            DDU8 Stereo3DSupport : 2;
            /** Bit-7: Reserved.                                              */
            DDU8 UNIQUENAME(Rsvd) : 1;
        };
    };
    /** Bytes 4-5: Horizontal Active Image pixels.                            */
    DDU16 HActive;
    /** Bytes 6-7: Vertical Active Image pixels.                              */
    DDU16 VActive;
    /** Byte-8: Refresh Rate 1 to 256 Hz.                                     */
    DDU8 RefreshRate;
} DISPLAYID_2_0_TIMINGINFO_IX;

/***************************************************************
 * @brief  This struct defines "Type IX -Formula Based Timing" block.
 *
 *              0x24 - DID2_DATA_BLOCK_FORMULA_TIMING_IX
 *              Size - 3 Bytes fixed + 6 Bytes per Formula Timing.
 ***************************************************************/
typedef struct _DISPLAYID_2_0_FORMULA_TIMING_IX
{
    /** Data Block header of Block 0x24.                         */
    DISPLAYID_BLOCK BlockHeader;
    /* Multiple instance of Timing Info IX Data Block can be present */
    /** Defined Variable of type DISPLAYID_2_0_TIMINGINFO_IX to get the
     * pointer to first instance of Timing Info IX Data Block.
     */
    DISPLAYID_2_0_TIMINGINFO_IX DidV2Timing9;
} DISPLAYID_2_0_FORMULA_TIMING_IX;

typedef enum _HBLANK_PIXEL_OPTION
{
    HBLANK_PIXEL_80 = 0,
    HBLANK_PIXEL_160,
} HBLANK_PIXEL_OPTION;

#define RB_HBLANK_MIN_SIZE_80_PIXELS 80
#define RB_HBLANK_MIN_SIZE_160_PIXELS 160
#define RB_HBLANK_ADDITIONAL_STEP_SIZE 8
#define RB_VBLANK_MIN_SIZE_TIME 460

typedef enum _RED_BLANK_VER
{
    RED_BLANK_VER_NONE = 0,
    RED_BLANK_VER_1,
    RED_BLANK_VER_2,
    RED_BLANK_VER_3,

} RED_BLANK_VER;

#define TIMINGINFO_X_NORMAL_SIZE 6
#define TIMINGINFO_X_EXTRABYTE_SIZE 7

typedef struct _DID_TIMINGINFO_TYPE_X
{
    union
    {
        /** Byte-0 : Timing options.     */
        DDU8 TimingOptions;
        struct
        {
            /** Bits 0-2: Timing Formula/Algorithm of type CVT_RB_TIMING_FORMULA_VERSION. */
            DDU8 TimingFormula : 3;
            /** Bit-3: Reserved. */
            DDU8 EarlyVsync : 1;
            /** Bit-4: Horizontal blank minimum size supported HBLANK_PIXEL_OPTION. */
            DDU8 HBlankOption : 1;
            /** Bits 5-6: 3D Stereo support of type S3D_SUPPORT.              */
            DDU8 Stereo3DSupport : 2;
            /** Bit-7: Reserved.                                              */
            DDU8 Ycc420Support : 1;
        };
    };
    /** Bytes 2-1: Horizontal Active Image pixels.                            */
    DDU16 HActive;
    /** Bytes 4-3: Vertical Active Image pixels.                              */
    DDU16 VActive;
    /** Byte 5: Refresh rate.                                                */
    DDU8 RefreshRateLow : 8;
} DID_TIMINGINFO_TYPE_X;

typedef struct _DID_TIMINGINFO_TYPE_X_7BYTE
{
    /** Bytes 0 - 5. */
    union
    {
        /** Byte-0 : Timing options.     */
        DDU8 TimingOptions;
        struct
        {
            /** Bits 0-2: Timing Formula/Algorithm of type CVT_RB_TIMING_FORMULA_VERSION. */
            DDU8 TimingFormula : 3;
            /** Bit-3: Reserved. */
            DDU8 EarlyVsync : 1;
            /** Bit-4: Horizontal blank minimum size supported HBLANK_PIXEL_OPTION. */
            DDU8 HBlankOption : 1;
            /** Bits 5-6: 3D Stereo support of type S3D_SUPPORT.              */
            DDU8 Stereo3DSupport : 2;
            /** Bit-7: Reserved.                                              */
            DDU8 Ycc420Support : 1;
        };
    };
    /** Bytes 2-1: Horizontal Active Image pixels.                            */
    DDU16 HActive;
    /** Bytes 4-3: Vertical Active Image pixels.                              */
    DDU16 VActive;
    /** Byte 5: Refresh rate.                                                */
    DDU8 RefreshRateLow : 8;
    union
    {
        /** Byte 6. */
        DDU8 ExtraByte1;
        struct
        {
            /** Bits 0-1: Additional bits for RR. */
            DDU8 RefreshRateHigh : 2;
            /**     */
            DDU8 DeltaHBlank : 3;
            /**     */
            DDU8 AddVBlankTime : 3;
        };
    };

} DID_TIMINGINFO_TYPE_X_7BYTE;

/***************************************************************
 * @brief  This struct defines "Type X -Formula Based Timing" block.
 *
 *              0x2A - DID2_DATA_BLOCK_FORMULA_TIMING_X
 *              Size - 3 Bytes fixed + 6/7 Bytes per Formula Timing.
 ***************************************************************/
typedef struct _DISPLAYID_2_0_FORMULA_TIMING_X
{
    /** Data Block header of Block 0x2A.                         */
    DISPLAYID_BLOCK BlockHeader;
    /* Multiple instance of Timing Info X Data Block can be present */
    /** Defined Variable of type DISPLAYID_2_0_TIMINGINFO_X to get the
     * pointer to first instance of Timing Info X Data Block.
     */
    DID_TIMINGINFO_TYPE_X_7BYTE DidV2Timing10;
} DISPLAYID_2_0_FORMULA_TIMING_X;

/***************************************************************
 * @brief This struct defines "Video Timing Range Limits" block Tag ID: 0x25 - DID2_DATA_BLOCK_DYNAMIC_VIDEO_TIMING
 * Size - 15 byte block.
 ***************************************************************/
typedef struct _DISPLAYID_2_0_VIDEO_RANGE
{
    /** Data Block header of 0x25 Data block. */
    DISPLAYID_BLOCK BlockHeader;
    /** Minimum pixel in KHz units.           */
    DDU8 MinimumPixelClock[3];
    /** Maximum pixel in KHz units.           */
    DDU8 MaximumPixelClock[3];
    /** Minimum V. refresh rate.              */
    DDU8 MinimumVerticalRr;
    /** Maximum V.refresh rate 8bits.         */
    DDU8 MaximumVerticalRrLsb;
    union
    {
        /** Video timing support.             */
        DDU8 DynamicVideoTimingRangeRelatedAndSupportFlag;
        struct
        {
            /** Bits 0-1: Upper 2 bits of 10 bit MaxVertical Rate for Rev1.  */
            DDU8 MaximumVerticalRrMsb : 2;
            /** Bits 2-6: Reserved.           */
            DDU8 UNIQUENAME(Rsvd) : 5;
            /** Bit-7: SeamlessDynamicVideoTiming
             * Suported with Fixed Horz Pixel Rate
             * And Dynamic Vertical Blanking.
             */
            DDU8 SeamlessDynamicVideoTiming : 1;
        };
    };
} DISPLAYID_2_0_VIDEO_RANGE;

/***************************************************************
 * @brief  This Enum defines additional possible EOTF.
 ***************************************************************/
typedef enum _DISPLAYID_2_0_ADDITIONAL_EOTF
{
    /** Display Interface Rules  0x00 .*/
    DID2_EOTF_PER_INTERFACE_RULES = 0x00,
    /** sRGB IEC 61966-2-1       0x01 .*/
    DID2_EOTF_SRGB_IEC_61966,
    /** ITU-R BT.601             0x02 .*/
    DID2_EOTF_ITU_R_BT_601,
    /** ITU-R BT.1886 for BT.709 0x03 .*/
    DID2_EOTF_ITU_R_BT_1886,
    /** Adobe RGB                0x04 .*/
    DID2_EOTF_ADOBE_RGB,
    /** DCI-P3 (SMPTE RP 431-2)  0x05 .*/
    DID2_EOTF_DCI_P3_SMPTE_RP_431,
    /** ITU-R BT.2020            0x06 .*/
    DID2_EOTF_ITU_R_BT_2020,
    /** Gamma of 0x21 Byte 31    0x07 .*/
    DID2_EOTF_GAMMA_FUNCTION,
    /** SMPTE ST 2084            0x08 .*/
    DID2_EOTF_SMPTE_ST_2084,
    /** Hybrid Log EOTF          0x09 .*/
    DID2_EOTF_HYBRID_LOG_EOTF,
    /** Details in other block   0x10 .*/
    DID2_EOTF_CUSTOM,
    /** EOTF MAX as 0x10              .*/
    DID2_EOTF_MAX = DID2_EOTF_CUSTOM,
} DISPLAYID_2_0_ADDITIONAL_EOTF;

/***************************************************************
 * @brief  This Enum defines additional possible Color Space.
 ***************************************************************/
typedef enum _DISPLAYID_2_0_ADDITIONAL_COLOR_SPACE
{
    /** Display Interface Rule 0x00 .*/
    DID2_CLRSP_PER_INTERFACE_RULES = 0x00,
    /** sRGB IEC 61966-2-1     0x01 .*/
    DID2_CLRSP_SRGB_IEC_61966,
    /** ITU-R BT.601           0x02 .*/
    DID2_CLRSP_ITU_R_BT_601,
    /** ITU-R BT.709           0x03 .*/
    DID2_CLRSP_ITU_R_BT_709,
    /** Adobe RGB              0x04 .*/
    DID2_CLRSP_ADOBE_RGB,
    /** DCI-P3(SMPTE RP 431-2) 0x05 .*/
    DID2_CLRSP_DCI_P3_SMPTE_RP_431,
    /** ITU-R BT.2020          0x06 .*/
    DID2_CLRSP_ITU_R_BT_2020,
    /** Details in other block 0x07 .*/
    DID2_CLRSP_CUSTOM,
    /** Color Space Max as 0x07  .   */
    DID2_CLRSP_MAX = DID2_CLRSP_CUSTOM,
} DISPLAYID_2_0_ADDITIONAL_COLOR_SPACE;

typedef union _DISPLAYID_2_0_ADDITIONAL_COLOR_SPACE_EOTF
{
    /** AdditionalColorSpace_EOTF_Info: Refer Table 4-27 .  */
    DDU8 AdditionalColorSpace_EOTF_Info;
    struct
    {
        /** Bits 0-3: DISPLAYID_2_0_ADDITIONAL_EOTF.        */
        DDU8 SupportedInterfaceEOTF : 4;
        /** Bits 4-7: DisplayID_2_0_ADDITIONAL_COLOR_SPACE. */
        DDU8 SupportedInterfaceColorSpace : 4;
    };
} DISPLAYID_2_0_ADDITIONAL_COLOR_SPACE_EOTF;

/***************************************************************
 * @brief This struct defines "Display Interface Data Block".
 *
 *              Tag ID: 0x26 - DID2_DATA_BLOCK_DISPLAY_IF_FEATURES
 *              (Refer Table 4.26 )
 *              Size - 11 byte Data block + 0-7 extra bytes
 *              (based on no of addditional Color Space & EOTF).
 ***************************************************************/
typedef struct _DISPLAYID_2_0_DISPLAY_INTERFACE
{
    /** Data Block header of Block 0x26 .*/
    DISPLAYID_BLOCK BlockHeader;
    /** Table 4-24: Supported Interface ColorDepth for different Encodings:
     * RGB, YCbCr4:4:4, YCbCr4:2:2, YCbCr4:2:0 .
     */
    union
    {
        /** Byte -3: SupportedColorDepthRGBencoding.          */
        DDU8 SupportedColorDepthRGBencoding;
        struct
        {
            /** Bit-0: RGB 6BPC   0=No Support, 1=Suppported. */
            DDU8 Support6bitPerPrimaryColorRGB : 1;
            /** Bit-1: RGB 8BPC   0=No Support, 1=Suppported. */
            DDU8 Support8bitPerPrimaryColorRGB : 1;
            /** Bit-2: RGB 10BPC  0=No Support, 1=Suppported. */
            DDU8 Support10bitPerPrimaryColorRGB : 1;
            /** Bit-3: RGB 12BPC  0=No Support, 1=Suppported. */
            DDU8 Support12bitPerPrimaryColorRGB : 1;
            /** Bit-4: RGB 14BPC  0=No Support, 1=Suppported. */
            DDU8 Support14bitPerPrimaryColorRGB : 1;
            /** Bit-5: RGB 16BPC  0=No Support, 1=Suppported. */
            DDU8 Support16bitPerPrimaryColorRGB : 1;
            /** Bits 6-7 Reserved. */
            DDU8 UNIQUENAME(Rsvd) : 2;
        };
    };
    union
    {
        /** Byte -4: SupportedColorDepthYCbCr444encoding.          */
        DDU8 SupportedColorDepthYCbCr444encoding;
        struct
        {
            /** Bit-0: YCbCr444 6BPC   0=No Support, 1=Suppported. */
            DDU8 Support6bitPerPrimaryColorYCbCr444 : 1;
            /** Bit-1: YCbCr444 8BPC   0=No Support, 1=Suppported. */
            DDU8 Support8bitPerPrimaryColorYCbCr444 : 1;
            /** Bit-2: YCbCr444 10BPC  0=No Support, 1=Suppported. */
            DDU8 Support10bitPerPrimaryColorYCbCr444 : 1;
            /** Bit-3: YCbCr444 12BPC  0=No Support, 1=Suppported. */
            DDU8 Support12bitPerPrimaryColorYCbCr444 : 1;
            /** Bit-4: YCbCr444 14BPC  0=No Support, 1=Suppported. */
            DDU8 Support14bitPerPrimaryColorYCbCr444 : 1;
            /** Bit-5: YCbCr444 16BPC  0=No Support, 1=Suppported. */
            DDU8 Support16bitPerPrimaryColorYCbCr444 : 1;
            /** Bits 6-7: YCbCr444 Reserved.                       */
            DDU8 UNIQUENAME(Rsvd) : 2;
        };
    };
    union
    {
        /** Byte -5: SupportedColorDepthYCbCr422encoding.          */
        DDU8 SupportedColorDepthYCbCr422encoding;
        struct
        {
            /** Bit-0: YCbCr422 8BPC   0=No Support, 1=Suppported. */
            DDU8 Support8bitPerPrimaryColorYCbCr422 : 1;
            /** Bit-1: YCbCr422 10BPC  0=No Support, 1=Suppported. */
            DDU8 Support10bitPerPrimaryColorYCbCr422 : 1;
            /** Bit-2: YCbCr422 12BPC  0=No Support, 1=Suppported. */
            DDU8 Support12bitPerPrimaryColorYCbCr422 : 1;
            /** Bit-3: YCbCr422 14BPC  0=No Support, 1=Suppported. */
            DDU8 Support14bitPerPrimaryColorYCbCr422 : 1;
            /** Bit-4: YCbCr422 16BPC  0=No Support, 1=Suppported. */
            DDU8 Support16bitPerPrimaryColorYCbCr422 : 1;
            /** Bits 5-7: YCbCr422 Reserved.                       */
            DDU8 UNIQUENAME(Rsvd) : 3;
        };
    };
    union
    {
        /** Byte -6: SupportedColorDepthYCbCr420encoding.          */
        DDU8 SupportedColorDepthYCbCr420encoding;
        struct
        {
            /** Bit-0: YCbCr420 8BPC   0=No Support, 1=Suppported. */
            DDU8 Support8bitPerPrimaryColorYCbCr420 : 1;
            /** Bit-1: YCbCr420 10BPC  0=No Support, 1=Suppported. */
            DDU8 Support10bitPerPrimaryColorYCbCr420 : 1;
            /** Bit-2: YCbCr420 12BPC  0=No Support, 1=Suppported. */
            DDU8 Support12bitPerPrimaryColorYCbCr420 : 1;
            /** Bit-3: YCbCr420 14BPC  0=No Support, 1=Suppported. */
            DDU8 Support14bitPerPrimaryColorYCbCr420 : 1;
            /** Bit-4: YCbCr420 16BPC  0=No Support, 1=Suppported. */
            DDU8 Support16bitPerPrimaryColorYCbCr420 : 1;
            /** Bits 5-7: YCbCr420 Reserved.                       */
            DDU8 UNIQUENAME(Rsvd) : 3;
        };
    };
    /** Table 4.25: Min pixel Rate at which YCbCr4:2:0 Encoding is supported
     * Pixel Rate = (74.25 MP/s) X (Stored Value), 00 = Suports all.
     */
    DDU8 MinPixelRateYCbCr420Encoding;
    /** Audio Sample Rate Capbilities - Refer Table 4.26 .    */
    union
    {
        DDU8 AudioSampleRateCaps;
        struct
        {
            /**  Bits 0-4 : Reserved.                         */
            DDU8 UNIQUENAME(Rsvd) : 5;
            /** Bit 5 : Audio 48 KHz Sample Rate.             */
            DDU8 Audio48KHzSampleRate : 1;
            /** Bit 6 : Audio 44.1 KHz KHz Sample Rate.       */
            DDU8 Audio44p1KHzSampleRate : 1;
            /** Bit 7 : Audio 32 KHz KHz Sample Rate.         */
            DDU8 Audio32KHzSampleRate : 1;
        };
    };
    /** Color Space & EOTF Combination1 - Refer Table 4.27 .              */
    union
    {
        DDU8 ColorSpaceEOTFCombination1;
        struct
        {
            /** Bit-0: ColorSpace & EOTF of sRGB.                          */
            DDU8 SRGBColorEOTF : 1;
            /** Bit-1: ColorSpace & EOTF of ITU-R BT.601.                  */
            DDU8 BT601ColorEOTF : 1;
            /** Bit-2: ColorSpace ITU-R BT.709 & EOTF ITU-R BT.1886 EOTF.  */
            DDU8 BT709ColorBT1886EOTF : 1;
            /** Bit-3: ColorSpace & EOTF of Adobe RGB.                     */
            DDU8 AdobeRGBColorEOTF : 1;
            /** Bit-4: ColorSpace & EOTF of DCI-P3 (SMPTE RP 431-2).       */
            DDU8 DCIP3ColorEOTF : 1;
            /** Bit-5: ColorSpace & EOTF of ITU-R BT.2020.                 */
            DDU8 BT2020ColorEOTF : 1;
            /** Bit-6: ColorSpace ITU-R BT.2020 & EOTF SMPTE ST 2084 EOTF. */
            DDU8 BT2020ClrSMPTE2084EOTF : 1;
            /** Bit-7: Reserved.                                           */
            DDU8 UNIQUENAME(Rsvd) : 1;
        };
    };
    /** Byte-10 is Reserved       .      */
    DDU8 ColorSpaceEOTFCombination2;
    /** Byte-11 tells how many "Number of additional Color Space & EOTF combinations"
     * are available and defined as "N". Max additional combinations possible 2^3=8.
     */
    union
    {
        /** Number of additional combinations of Color Space and EOTF (N). */
        DDU8 NoOfAdditionalColorSpace_EOTF;
        struct
        {
            /** Bits 0-2: Additional Interface Color & EOTF Combinations
             * Max possible extra combinations are 2^3 = 8.
             */
            DDU8 NoOfAdditionalColorEOTF : 3;
            /** Bit 3-7: Reserved to all 0s .   */
            DDU8 UNIQUENAME(Rsvd) : 5;
        };
    };
    /** If NoOfAdditionalColorEOTF > 0 then all additional byte will be as per
     * DISPLAYID_2_0_ADDITIONAL_COLOR_SPACE_EOTF.
     */
} DISPLAYID_2_0_DISPLAY_INTERFACE;

//-----------------------------------------------------------------------------
//
//  Stereo Display Interface Data Block
//  Tag ID: 0x27 - DID2_DATA_BLOCK_STEREO_DISPLAY_INTERFACE
//  Size - Variable size block
//  This is optional block and need not be parsed as we are de-featuring S3D going forward
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
//  Tiled Display Topology Data Block
//  Tag ID: 0x28 - DID2_DATA_BLOCK_TILED_DISPLAY_TOPOLOGY
//  Size - 22 byte Data Block
//  As this is similar to what is defined in DID1.3, hence separate struct not
//  defined here. Reuse DID1.3 struct "DISPLAYID_TILED_DISPLAY_TOPOLOGY"
//-----------------------------------------------------------------------------

typedef enum _DISPLAYID_2_0_MULTI_SST_OPERATION
{
    DID2_MSO_SINGLE_STREAM = 0,
    DID2_MSO_TWO_STREAMS = 1,
    DID2_MSO_FOUR_STREAMS = 2
} DISPLAYID_2_0_MULTISSTOPERATION;

/***************************************************************
 * @brief This struct defines Vendor Specific Data Block.
 *
 *              Tag ID: 0x7E - DID2_DATA_BLOCK_VENDOR_SPECIFIC
 *              (Refer Table 4.48 )
 *              Size - Variable Length:
 *              First 6 bytes fixed + Length defined at 0x02 Byte.
 ***************************************************************/
typedef struct _DISPLAYID_2_0_VENDOR_SPECIFIC
{
    /** Bytes 1-2: Data Block header of 0x7E Block. */
    DISPLAYID_BLOCK BlockHeader;
    /** Byte 3-5: Vendor/Manufacturer IEEE OUI ID . */
    DDU8 ManufacturerID[3];
    DDU8 VesaDataStructureType;
    /** Byte 7: Defined to get the pointer to Vendor Specific block
     * variable length data.
     */
    union
    {
        DDU8 MsoCaps;
        struct
        {
            DDU8 OverlapPixelCount : 4;
            DDU8 UNIQUENAME(Rsvd) : 1;
            DDU8 MultiSstOperation : 2;
            DDU8 UNIQUENAME(Rsvd) : 1;
        };
    };

} DISPLAYID_2_0_VENDOR_SPECIFIC;

//-----------------------------------------------------------------------------
//
//  CEA Data Block within a DisplayID Data Block
//  Tag ID: 0x81 - DID2_DATA_BLOCK_CTA_DISPLAY_ID
//  Size - Variable size block
//  As this is similar to what is defined in DID1.3, hence no separate struct
//  defined. Reuse DID1.3 struct "DISPLAYID_CEA_EXTN_BLOCK"
//-----------------------------------------------------------------------------
#pragma pack()
