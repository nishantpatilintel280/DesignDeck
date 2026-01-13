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
 * @file  GlobalTimings.h
 * @brief This file contains all data structures needed for Global Timings data.
 *
 */

#pragma once

#include "..\DisplayArgsInternal.h"

 //////////////////////////////////////////////
 //
 //    Constants associated with TIMING_FLAGS
 //
 //////////////////////////////////////////////

 //////////////////////////////////////////////
 //
 //  Pixel Replication Mask
 //
 //////////////////////////////////////////////
#define PR_1X BIT0
#define PR_2X BIT1
#define PR_3X BIT2
#define PR_4X BIT3
#define PR_1X_2X (PR_1X + PR_2X)
#define PR_1X_2X_4X (PR_1X + PR_2X + PR_4X)
#define PR_1X_TO_10X 0x3FF

#define NUM_PR_MASK_SUPPORTED 4 // We support only PR_1X, PR_2X and PR_4X

//////////////////////////////////////////////
//
// Established mode table
// Note: All are of VESA DMTS type
//
//////////////////////////////////////////////
typedef struct _ESTABLISHED_MODE_TABLE
{
    DD_TIMING_INFO* pTimingInfo;
} ESTABLISHED_MODE_TABLE;
extern ESTABLISHED_MODE_TABLE g_EstablishedModeTable[];
extern DDU32 g_ulTotalEstablishedModes;

// Added for ESTABLISHED TIMINGS III BLOCK for EDID 1.4
extern ESTABLISHED_MODE_TABLE g_EstTiming3ModeTable[];
extern DDU32 g_ulTotalEstTiming3modes;

// RCR 1023191 : Support for VESA DisplayID
// Added for DisplayID VESA Standard timings
typedef struct _DISPLAYID_DMTID_STANDARD_TIMING
{
    DDU8 DmtIdIndex;
    DD_TIMING_INFO* pTimingInfo;
} DISPLAYID_DMTID_STANDARD_TIMING;

extern DISPLAYID_DMTID_STANDARD_TIMING g_DisplayID_VESA_DMTModeTable[];
extern DDU32 g_ulTotalDisplayID_VESA_DMTmodes;

typedef struct _DISPLAYID_CE_STANDARD_TIMING
{
    DDU8 CEIndex;
    DDU8 AspectRatio;
    DD_TIMING_INFO* pTimingInfo; // Pointer to mode timing data
} DISPLAYID_CE_STANDARD_TIMING;

#define MAX_DISPLAYID_VICID 62
extern DISPLAYID_CE_STANDARD_TIMING g_DisplayID_CeStandardTiming[];
extern DDU32 g_ulTotalDisplayID_CEA_modes;

/////////////////////////////////////////////////////
//
//  CE-861b Supported Short Video Descriptors Table
//
/////////////////////////////////////////////////////
typedef struct _CE_SHORT_VIDEO_MODE
{
    DDU8 CEIndex;
    DDU8 AspectRatio;
    DDU32 PRMask;                // Mask for supported pixel repetition,  ulPRMask = 1 for no-repeated modes
    DD_TIMING_INFO* pTimingInfo; // Pointer to mode timing data
} CE_SHORT_VIDEO_MODE;

extern CE_SHORT_VIDEO_MODE g_SupportedCeShortVideoModes[];
extern DDU32 g_ulNumSupportedCEModes;

extern CE_SHORT_VIDEO_MODE g_SDVOHDMISupportedCeShortVideoModes[];
extern DDU32 g_ulSDVOHDMINumSupportedCEModes;

extern CE_SHORT_VIDEO_MODE g_4kx2kModes[];
extern DDU32 g_ulNum4kx2kModes;

/////////////////////////////////////////////////////////////////////////
//
// Data Structure for Under Scan Modes
// First 2 entries indicate EDID supported X & Y. Next 2 entries indicate
// the underscan mode which need to be enumerated.
//
/////////////////////////////////////////////////////////////////////////
typedef struct _UNDER_SCAN_MODE
{
    DDU32 XRes;
    DDU32 YRes;
    DDU32 XUnderScanRes;
    DDU32 YUnderScanRes;
} UNDER_SCAN_MODE;
extern UNDER_SCAN_MODE g_UnderScanModes[];
extern DDU32 g_ulNumUnderScanModes;
//////////////////////////////////////////////
//
// Static mode table
// Note: All are of VESA_STD_DMTS_MODE type
// except ones where pTimingInfo is NULL. In
// this case it will be CVT/GTF timings.
//
//////////////////////////////////////////////
typedef struct _STATIC_MODE_TABLE
{
    DDU16 XResolution;
    DDU16 YResolution;
    DDU16 RefreshRate; // Note: Will be used only if pTimingInfo is NULL
    DDU8 SupportedColorBPP_Mask;
    DD_TIMING_INFO* pTimingInfo; // If NULL, ModesManager will create CVT/GTF timings
    DDU8 AddModeMask;            // RCR 294233: AddModeMask updated based on OEM mode list.
    // In future this could be used as DisplayMask.
} STATIC_MODE_TABLE;
extern STATIC_MODE_TABLE g_StaticModeTable[];
extern DDU32 g_ulTotalStaticModes;

//////////////////////////////////////////////
//
// Extern definitions for all the fixed timings
// Note: GTF times should be derived using the
// formulae
//
//////////////////////////////////////////////
extern DD_TIMING_INFO g_TimingVGA_Mode3_640x480_60;
extern DD_TIMING_INFO g_TimingVGA_Mode3_720x400_60;
extern DD_TIMING_INFO g_Timing640x350_85;
extern DD_TIMING_INFO g_Timing640x400_70;
extern DD_TIMING_INFO g_Timing640x400_85;
extern DD_TIMING_INFO g_Timing640x480_60;
extern DD_TIMING_INFO g_Timing640x480_72;
extern DD_TIMING_INFO g_Timing640x480_75;
extern DD_TIMING_INFO g_Timing640x480_85;
extern DD_TIMING_INFO g_Timing720x400_85;
extern DD_TIMING_INFO g_Timing800x600_56;
extern DD_TIMING_INFO g_Timing800x600_60;
extern DD_TIMING_INFO g_Timing800x600_72;
extern DD_TIMING_INFO g_Timing800x600_75;
extern DD_TIMING_INFO g_Timing800x600_85;
extern DD_TIMING_INFO g_Timing848x480_60;
extern DD_TIMING_INFO g_Timing1024x768_60;
extern DD_TIMING_INFO g_Timing1024x768_70;
extern DD_TIMING_INFO g_Timing1024x768_75;
extern DD_TIMING_INFO g_Timing1024x768_85;
extern DD_TIMING_INFO g_Timing1152x864_75;
extern DD_TIMING_INFO g_Timing1280x960_60;
extern DD_TIMING_INFO g_Timing1280x960_85;
extern DD_TIMING_INFO g_Timing1280x1024_60;
extern DD_TIMING_INFO g_Timing1280x1024_75;
extern DD_TIMING_INFO g_Timing1280x1024_85;
extern DD_TIMING_INFO g_Timing1360x768_60;
extern DD_TIMING_INFO g_Timing1366x768_60;
extern DD_TIMING_INFO g_Timing1366x768_60rb;
extern DD_TIMING_INFO g_Timing1600x900_60rb;
extern DD_TIMING_INFO g_Timing1600x1200_60;
extern DD_TIMING_INFO g_Timing1600x1200_65;
extern DD_TIMING_INFO g_Timing1600x1200_70;
extern DD_TIMING_INFO g_Timing1600x1200_75;
extern DD_TIMING_INFO g_Timing1600x1200_85;
extern DD_TIMING_INFO g_Timing1792x1344_60;
extern DD_TIMING_INFO g_Timing1792x1344_75;
extern DD_TIMING_INFO g_Timing1856x1392_60;
extern DD_TIMING_INFO g_Timing1856x1392_75;
extern DD_TIMING_INFO g_Timing1920x1440_60;
extern DD_TIMING_INFO g_Timing1920x1440_75;
extern DD_TIMING_INFO g_Timing2560x1600_60;
extern DD_TIMING_INFO g_Timing2560x1600_75;

extern DD_TIMING_INFO g_Timing720x480_60;
extern DD_TIMING_INFO g_Timing720x480_a_60;
extern DD_TIMING_INFO g_Timing1024x768_43i;
extern DD_TIMING_INFO g_Timing1280x720_a_60;
extern DD_TIMING_INFO g_Timing1920x1080_i_a_60;
extern DD_TIMING_INFO g_Timing720x576_50;
extern DD_TIMING_INFO g_Timing720x576_a_50;
extern DD_TIMING_INFO g_Timing1280x720_50;
extern DD_TIMING_INFO g_Timing1920x1080_i_a_50;
extern DD_TIMING_INFO g_Timing1920x1080_a_50;
extern DD_TIMING_INFO g_Timing1920x1080_a_60;
extern DD_TIMING_INFO g_Timing2048x1152_60rb;
