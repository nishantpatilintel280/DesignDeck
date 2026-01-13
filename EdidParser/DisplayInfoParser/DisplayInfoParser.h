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
 * @file  DisplayInfoParser.h
 * @brief This file contains all data structures needed for EDID/Display ID parsing.
 *
 */

#pragma once

#include "..\GenericDisplayInfoRoutines.h"
#include "GlobalTimings.h"
#include "..\DisplayContext.h"
#include "..\OfflineHeaderFiles\MemoryAllocation.h"

 ///////////////////////////////////////////////////////////////////////////////
 //
 // CONSTANTs / ENUMs
 //
 ///////////////////////////////////////////////////////////////////////////////

 ///////////////////////////////////////////////////////////////////////////////
 //
 // CONSTANTs / ENUMs
 //
 ///////////////////////////////////////////////////////////////////////////////
#define MAX_DID_BLOCK_INSTANCES 5

#define DD_GET_ASPECT_RATIO(HActive, VActive) ((HActive * 1000) / (VActive))

#define DD_MAX_DTD_IN_VFPDB_COUNT 16 // possible range for 18-byte DTDs is 129 - 144, meaning VFPDB can mention maximum 16 DTDs

#define VICID_18BYTE_DTDRANGE_START 129
#define VICID_18BYTE_DTDRANGE_END 144

// Some tiled panels don't have DTD timing matching the single tile size,
// Check for the aspect ratio instead of exact timing for adding tiled modes.
// DisplayID spec does not mandate that a timing must be present that have pixel dimensions which
// exactly match the native pixel dimensions of the tiles. Apple pro display is one such panel where
// the tile info block has the horizontal and vertical pixel size which doesn't match any of the EDID modes.
// So check for Aspect ratio to ensure all modes with the correct aspect ratio will be added as tiled mode.
#define DD_IS_TILED_MODE(pTiledBlock, pTimingInfo) \
    ((DD_GET_ASPECT_RATIO(pTiledBlock->HTileSizeInPixels, pTiledBlock->VTileSizeInLines)) == (DD_GET_ASPECT_RATIO(pTimingInfo->HActive, pTimingInfo->VActive)))

///////////////////////////////////////////////////////////////////////////////
//
// Related Enums and structures -- START
//
///////////////////////////////////////////////////////////////////////////////

typedef enum _EDID_PANEL_COLOR_DEPTH
{
    UNDEFINED_COLOR_DEPTH = 0, // Undefined (As per EDID Color bit depth definition)
    COLOR_DEPTH_16BPP = 0, // RGB 5:6:5 (for MIPI case, Mostly unused)
    COLOR_DEPTH_18BPP,         // RBG 6:6:6
    COLOR_DEPTH_24BPP,         // RGB 8:8:8
    COLOR_DEPTH_30BPP,         // RGB 10:10:10
    COLOR_DEPTH_36BPP,         // RBG 12:12:12
    COLOR_DEPTH_42BPP,         // RBG 14:14:14
    COLOR_DEPTH_48BPP,         // RBG 16:16:16
    COLOR_DEPTH_54BPP,         // RBG 18:18:18
} EDID_PANEL_COLOR_DEPTH;

// Structure to hold CE VIC ID and other relevant flags during parsing
// Used to filter out/prioratize/find all possible aspect ratios/preference modes/S3D flags etc... before addition to mode table
typedef struct _CE_MODE_LIST
{
    union
    {
        DDU8 Value;
        struct
        {
            DDU8 IsPreferred : 1; // preferred mode

            DDU8 SignalStandard : 3; // Will get copied to DD_SIGNAL_STANDARD of Timing Info
            DDU8 ReservedCeData : 4; // Reserved
        };
    };

    DD_SAMPLING_MODE SamplingMode; // SamplingMode is of format DD_SAMPLING_MODE indicates RGB or YUV420 or Both sampling
    DD_CE_ASPECT_RATIO Par[MAX_PARS_POSSIBLE_WITH_1_VIC];
    DD_BOOL NativeFormat[MAX_PARS_POSSIBLE_WITH_1_VIC];
    DDU8 VicId[MAX_PARS_POSSIBLE_WITH_1_VIC]; // Valid VicID 1 - 127, 193-219 are adjusted as index to SVD struct. 0XFF - Vic not defined for mode
    DDU8 VicId4k2k;                           // Valid HDMI VIC = 1, 2, 3, 4 corresponding to Table-30 of HDMI 2.1a Specification.
    DDU32 S3DFormatMask;                      // Mask of DD_S3D_FORMAT
    DD_TIMING_INFO* pTimingInfo;              // Pointer to CE timing Info from "g_SupportedCeShortVideoModes" table
} CE_MODE_LIST;

typedef struct _VFPDB_INFO
{
    DDU16 DtdMask;  // Video Format Preference 18 Byte Dtd Mask, This will have bits 0-16 set based on SVRs from 129-144
    DDU16 DtdCount; // Video Format Preference 18 Byte Dtd Count, Init to 0, keep incrementing for every DTD added as preferred based on mask above
} VFPDB_INFO;

typedef struct _MODES_LIST // this is a structure used by a list for storing the timings & a corresponding force ADD flag, then shifting to mode table
{
    DD_TIMING_INFO TimingInfo;
    DD_BOOL ForceAdd;
} MODES_LIST;

typedef struct _EDID_DATA
{
    DD_BOOL IsDisplayIdBaseBlock; // 0 - Base Block is EDID, 1 - Base Block is Display ID
    DDU8 EdidOrDidVerAndRev;      // EDID/Display ID version and revision (e.g. version/revision 1.4 is represented as 0x14)
    DDU32 NumModesInEdidOrDID;
    DDU32 VicCount;
    CE_MODE_LIST* pCeModeList;
    DDU8* pSVDList;     // Pointer to SVD list in CE block
    DDU8 SvdListLength; // Length of SVD list
    DD_DISPLAY_CAPS* pData;
    DD_BOOL IsPreferredModeAdded; // Flag to prevent adding multiple preferred modes
    VFPDB_INFO Vfpdb;
    DDU8 NumTimingInfo;
    DDU8 NumBaseBlockDtds;
    MODES_LIST* pModes;
} EDID_DATA;

typedef struct _ADD_MODES
{
    DD_TIMING_INFO* pTimingInfo; // Timing Info to add
    DD_TABLE* pTargetModeTable;
    DD_BOOL ForceAdd;
} ADD_MODES;

typedef struct _PARSE_EXTBLK_ARGS
{
    DDU8* pDataBlock;
    DDU8 LenDataBlock;
    DD_TABLE* pModeTable;
} PARSE_EXTBLK_ARGS;

typedef struct _PARSE_STDTIMING_ARGS
{
    EDID_STD_TIMING* pStdTiming;
    DD_TABLE* pModeTable;
} PARSE_STDTIMING_ARGS;

typedef struct _HDMIVSDB_MODES_ARGS
{
    HDMI_VSDB* pHdmiVsdbBlock; // Base of HDMI VSDB Block
    DDU8 VICOffset;            // Offset into S3DCAPS_VICLENGTH byte
    DD_TABLE* pModeTable;      // Mode-table
} HDMIVSDB_MODES_ARGS;

typedef struct _GET_BLOCK_BY_ID_ARGS
{
    IN DDU8 DisplayIdVerAndRev; // Display ID version and revision (DID 1.3 wil be shown as 0x12 & DID2.0 will be as 0x20 )
    IN DDU8 BlockIdOrTag;       // Display ID Block# or CE Tag ID to search
    IN DDU8 CeaExtendedTag;     // Valid only for CE extension block search and when BlockIdOrTag == CEA_USE_EXTENDED_TAG
    IN DDU32 IEEERegNum;        // Valid only for CE extension block search and when BlockIdOrTag == CEA_VENDOR_DATABLOCK
    IN DDU8 InstanceOfBlock;    // Valid only for Display ID base block/extension block; helpful to search if there are multiple instances of timing blocks
    IN DDU8* pInputBuf;         // Display ID buffer or CE block Data
    IN DDU32 InputBlockLength;  // Total size of Input buffer
    OUT DDU8* pOutputBlock;     // Pointer to output Display ID block or CE Bloc matching input ID/Tag

    // Variable size Actual Data.
} GET_BLOCK_BY_ID_ARGS;
