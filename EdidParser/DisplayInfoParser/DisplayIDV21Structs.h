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
 * @file  DisplayIDV21Structs.h
 * @brief This file contains all data structures used from Display ID 2.1 spec.
 *
 */

#pragma once

 //-----------------------------------------------------------------------------
 //
 // Generic includes
 //
 //-----------------------------------------------------------------------------

#include "DisplayIDV2Structs.h"

//---------------------------------------------------------------------------
//
// CONSTANT / ENUM
// Only define which is extra for DID2.1
//
//---------------------------------------------------------------------------
#define DISPLAY_ID_VER_2_1 0x21

// In 6.2 format, there are 3 possible fraction values.
// 01b = 0.25
// 10b = 0.50
// 11b = 0.75
// Below multiplier will be used to convert fractional part to micro seconds
#define FORMAT_6_2_FRACTIONAL_MULTIPLIER_IN_US 250
#pragma pack(1)

/***************************************************************
 * @brief  This Enum is used for identifying different data blocks in Display ID section. (Refer Table 3.1).
 ***************************************************************/
typedef enum _DISPLAYID2_1_DATA_BLOCK_TAG
{
    DID21_DATA_BLOCK_FORMULA_TIMING_X = 0x2A,
    DID21_DATA_BLOCK_ADAPTIVE_SYNC = 0x2B,
    DID21_DATA_BLOCK_AR_VR_HMD = 0x2C,
    DID21_DATA_BLOCK_AR_VR_LAYER = 0x2D,
    DID21_DATA_BLOCK_BRIGHTNESS_LUMINANCE_RANGE = 0x2E,
} DISPLAYID2_1_DATA_BLOCK_TAG;

typedef enum _DISPLAYID2_1_PRODUCT_ID
{
    DID21_PID_EXTENSION = 0,                // 00    -    Extension section
    DID21_PID_TEST_STRUCTURE,               // 01    -    Test structure
    DID21_PID_DISPLAY_PANEL,                // 02    -    Generic Display
    DID21_PID_TV_DISPLAY,                   // 03    -    TV Display
    DID21_PID_DESKTOP_PRODUCTIVITY_DISPLAY, // 04    -    Desktop Productivity Display
    DID21_PID_DESKTOP_GAMING_DISPLAY,       // 05    -    Desktop Gaming Display
    DID21_PID_PRESENTATION_DISPLAY,         // 06    -    Presentation Display
    DID21_PID_VR_DISPLAY,                   // 07    -    Virtual Reality Display
    DID21_PID_AR_DISPLAY,                   // 08    -    Augmented Reality Display
    DID21_PID_RESERVED,                     // 09 or above - Reserved
} DISPLAYID2_1_PRODUCT_ID;

//---------------------------------------------------------------------------
//
//  Display ID-2.1 Data Block header
//  Header: 4 bytes (Table 3.2 of Display ID spec 2.1)
//  For better common functionality across DID1.3, 2.0 & 2.1,
//  same DISPLAYID_HEADER struct is maintained.
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Start of Display Id 2.1 Data blocks
//---------------------------------------------------------------------------

/***************************************************************
 * @brief  This struct defines "Adaptive Sync" data block. 0x2B - DID21_DATA_BLOCK_ADAPTIVE_SYNC. Size - 8 Bytes.
 ***************************************************************/
typedef enum _ADAPTIVE_SYNC_RANGE
{
    // Non-Native panel range. The display implements buffering to support the declared Adaptive Sync range and may repeat frames
    // as necessary.
    ADAPTIVE_SYNC_RANGE_NON_NATIVE = 0,
    // Native panel range. The display does not implement buffering and does not repeat frames.
    ADAPTIVE_SYNC_RANGE_NATIVE
} ADAPTIVE_SYNC_RANGE;

typedef enum _SUCCESSIVE_FRAME_DURATION_TOLERANCE
{
    // Flicker performance is met in any duration change within the refresh rate change
    SUCCESSIVE_FRAME_DURATION_TOLERANCE_NO_LIMIT = 0,
    // Duration change is defined explicitly to meet flicker performance
    SUCCESSIVE_FRAME_DURATION_TOLERANCE_LIMITED
} SUCCESSIVE_FRAME_DURATION_TOLERANCE;

typedef enum _ADAPTIVE_SYNC_MODES
{
    // Fixed Average VTotal (FAVT) mode is supported
    ADAPTIVE_SYNC_MODE_FIXED_AVERAGE_VTOTAL = 0,
    // Both FAVT and Adaptive VTotal (AVT) modes are supported
    ADAPTIVE_SYNC_MODE_ALL
} ADAPTIVE_SYNC_MODES;

typedef enum _ADAPTIVE_SYNC_SEAMLESS_TRANSITION
{
    // Seamless transition to and from current Adaptive Sync mode and range is supported
    ADAPTIVE_SYNC_SEAMLESS_TRANSITION_SUPPORTED = 0,
    // Seamless transition to and from current Adaptive Sync mode and range is not supported
    ADAPTIVE_SYNC_SEAMLESS_TRANSITION_NOT_SUPPORTED
} ADAPTIVE_SYNC_SEAMLESS_TRANSITION;

typedef struct _ADAPTIVE_SYNC_OPERATION_MODE_RANGE_DESCRIPTOR
{
    union
    {
        DDU8 AdaptiveSyncOperationModeAndRangeInfo;

        struct
        {
            // Panel's capability to support declared Adaptive Sync range (with or without buffering/repeated frames)
            DDU8 AdaptiveSyncRange : 1;
            // Panel's capability to meet flicker performance on duration increase (with or without explicit limit)
            DDU8 SuccessiveFrameDurationIncreaseTolerance : 1;
            // Panel's capability to support FAVT & AVT modes
            DDU8 SupportedAdaptiveSyncModes : 2;
            // Panel's capability to support seamless transition from Adaptive Sync mode and range
            DDU8 SeamlessTransitionNotSupported : 1;
            // Panel's capability to meet flicker performance on duration decrease (with or without explicit limit)
            DDU8 SuccessiveFrameDurationDecreaseTolerance : 1;
            DDU8 UNIQUENAME(Rsvd) : 2;
        };
    };

    union
    {
        // 6.2 format (6 integer bits and 2 fractional bits).
        // Value range 0.00 - 63.75
        // 0h indicates that flicker performance is met in any duration change
        DDU8 MaxSingleFrameDurationIncreaseValue;
        struct
        {
            DDU8 MaxSingleFrameDurationIncreaseFractionalBits : 2;
            DDU8 MaxSingleFrameDurationIncreaseIntegerBits : 6;
        };
    };
    DDU8 MinRefreshRate;
    DDU8 MaxRefreshRateLsb;

    union
    {
        DDU8 MaxRefreshRateMsbValue;

        struct
        {
            DDU8 MaxRefreshRateMsb : 2;
            DDU8 UNIQUENAME(Rsvd) : 6;
        };
    };

    union
    {
        // 6.2 format (6 integer bits and 2 fractional bits).
        // Value range 0.00 - 63.75
        // 0h indicates that flicker performance is met in any duration change
        DDU8 MaxSingleFrameDurationDecreaseValue;
        struct
        {
            DDU8 MaxSingleFrameDurationDecreaseFractionalBits : 2;
            DDU8 MaxSingleFrameDurationDecreaseIntegerBits : 6;
        };
    };
} ADAPTIVE_SYNC_OPERATION_MODE_RANGE_DESCRIPTOR;

typedef struct _DISPLAY_ID_2_1_ADAPTIVE_SYNC_BLOCK_HEADER
{
    DDU8 BlockID; // Data Block Identification
    union
    {
        DDU8 RevAndOtherData; // Data Block Revision and Other Data
        struct
        {
            DDU8 Revision : 3; // Revision
            DDU8 UNIQUENAME(Rsvd) : 1;
            DDU8 PayloadBytesInDescriptor : 3;
            DDU8 UNIQUENAME(Rsvd) : 1;
        };
    };
    DDU8 PayloadBytesInBlock; // Number of Payload Bytes 0 to 248
} DISPLAY_ID_2_1_ADAPTIVE_SYNC_BLOCK_HEADER;

typedef struct _DISPLAY_ID_2_1_BRIGHTNESS_LUMINANCE_RANGE_BLOCK
{
    DDU8 BlockID; // Data Block Identification

    union
    {
        /** Data Block Revision. */
        DDU8 RevAndOtherData;
        struct
        {
            /** Bits 0-2:    Revision. */
            DDU8 Revision : 3;
            /** Bits 3-7:    Reserved. */
            DDU8 UNIQUENAME(Rsvd) : 5;
        };
    };

    DDU8 PayloadBytesInBlock; // Number of Payload Bytes 0 to 248

    DDU16 MinLuminanceFull;
    DDU16 MaxLuminanceFull;
    DDU16 MaxLuminanceBoost;

} DISPLAY_ID_2_1_BRIGHTNESS_LUMINANCE_RANGE_BLOCK;

#pragma pack()
