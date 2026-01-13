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
 * @file  GenericDisplayInfoRoutines.h
 * @ingroup dpl
 * @brief This class is responsible for EDID parsing, mode collection & retriving
 *    EDID specific information about a display (e.g. CE Extn Audio blocks).
 *
 */

#pragma once

 /////////////////////////////////////////////
 //
 // Generic includes
 //
 /////////////////////////////////////////////
#include "DisplayArgsInternal.h"
#include "DisplayErrorDef.h"
#include "DisplayInfoParser/DisplayIDStructs.h"
#include "DisplayInfoParser/DisplayIDV2Structs.h"
#include "DisplayInfoParser/DisplayIDV21Structs.h"
#include "DisplayInfoParser/EDIDStructs.h"

/////////////////////////////////////////////
//
// Includes for various contained objects
//
/////////////////////////////////////////////

typedef struct _CREATE_GTF_TIMING_ARGS
{
    IN DDU32 XRes;
    IN DDU32 YRes;
    IN DDU32 RRate;
    IN DD_BOOL IsProgressiveMode;
    OUT DD_TIMING_INFO* pTimingInfo;
} CREATE_GTF_TIMING_ARGS;

typedef struct _CREATE_CVT_TIMING_ARGS
{
    IN DDU32 XRes;
    IN DDU32 YRes;
    IN DDU32 RRate;
    IN DDU32 ReqVblankTime;
    IN DDU32 ReqHblankPixels;
    IN DD_BOOL IsMargin_Req;
    IN DD_BOOL IsInterLaced;
    IN DD_BOOL IsRed_Blank_Req;
    IN DD_BOOL IsEarlyVsync;
    IN DD_BOOL IsVideoRrReq;
    IN DD_BOOL IsYCbCr420Supported;
    IN RED_BLANK_VER RedBlankVersion;
    OUT DD_TIMING_INFO* pTimingInfo;
} CREATE_CVT_TIMING_ARGS;

typedef struct _GET_ASPECTRATIO_ARGS
{
    IN DDU32 XRes;
    IN DDU32 YRes;
    OUT DDU8 AspectRatio;
} GET_ASPECTRATIO_ARGS;

typedef struct _PIXELCLK_COMPARE_ARGS
{
    IN DDU64 PixelClock1;
    IN DDU64 PixelClock2;
} PIXELCLK_COMPARE_ARGS;

typedef struct _GENERIC_DISPLAY_TIMING_DATA
{
    DDU32 PixelClockInKhz; // Byte0:  Pixel clock in Khz
    DDU16 HActive;         // Byte4:  H. active pixels
    DDU16 HBlank;          // Byte6:  H. blanking
    DDU16 HFrontPorch;     // Byte8:  H. Front Porch
    DDU16 HSync;           // Byte10: H. sync offset
    DDU16 VActive;         // Byte12: V. active lines
    DDU16 VBlank;          // Byte14: V. blanking
    DDU16 VFrontPorch;     // Byte16: V. Front Porch
    DDU16 VSync;           // Byte18: V. Sync
    DDU16 HImageSize;      // Byte20: H. Image Size
    DDU16 VImageSize;      // Byte22: V. Image Size
    union
    {
        DDU8 Flags; // Byte 24: Flags
        struct
        {
            DDU8 HSyncPolarity : 1;        // Bit 7: H. Sync Polarity
            DDU8 VSyncPolarity : 1;        // Bit 6: V. Sync polarity
            DDU8 UNIQUENAME(Reserved) : 6; // Bits [5 - 0]: Reserved
        };
    };
    DDU8 UNIQUENAME(Reserved); // Byte 25: Reserved
    DDU8 UNIQUENAME(Reserved); // Byte 26: Reserved
    DDU8 UNIQUENAME(Reserved); // Byte 27: Reserved
} GENERIC_DISPLAY_TIMING_DATA;

//
// Add other class methods here
//
// Other generic methods
DDU32 DisplayInfoRoutinesGetHdmiLinkBwInMbps(DDU8 MaxFrlRateIndex);
DDSTATUS DisplayInfoRoutinesGetTimingFromDTD(EDID_DTD_TIMING* pDTD, DD_TIMING_INFO* pTimingInfo);
DDSTATUS DisplayInfoRoutinesGetTimingFromGenericDTD(GENERIC_DISPLAY_TIMING_DATA* pGenericDisplayTimingData, DD_TIMING_INFO* pTimingInfo);
DDSTATUS DisplayInfoRoutinesGetDTDFromTimingInfo(DD_TIMING_INFO* pTimingInfo, EDID_DTD_TIMING* pDTD);
DD_BOOL DisplayInfoRoutinesCanTimingBeFitInEdid(DD_TIMING_INFO* pTimingInfo);
void DisplayInfoRoutinesUpdateTimingforCustomAlignment(DD_TIMING_INFO* pTimingInfo, DDU32 VDelta, DDU32 HDelta);
DDSTATUS DisplayInfoRoutinesCreateGTFTiming(CREATE_GTF_TIMING_ARGS* pCreateGTFTimingArgs);
DDSTATUS DisplayInfoRoutinesCreateCVTTiming(CREATE_CVT_TIMING_ARGS* pCreateCVTTimingArgs);
DD_BOOL DisplayInfoRoutinesIsSamePixelClock(PIXELCLK_COMPARE_ARGS* pPixelClkCompareArgs);
DDSTATUS DisplayInfoRoutinesGetTimingFromPRTimingForPRMask(DD_TIMING_INFO* pPRTimingInfo, DD_TIMING_INFO* pOutputTimingInfo, DDU8 PRMask);
DD_BOOL DisplayInfoRoutinesIsSameTiming(DD_TIMING_INFO* pSourceTimings, DD_TIMING_INFO* pTargetTimings);
DDU8 DisplayInfoRoutinesCalcChksum(DDU8* pBuffer, DDU32 BufferLen);
DD_BOOL DisplayInfoRoutinesCreateDscCfg(DD_RX_DSC_CAPS* pSinkDscDecoderCaps, DD_IN_OUT DSC_CFG* pDscCfg, DD_BOOL OptimizeSliceHeight);

// DP MNTu methods
DDSTATUS DpProtocolSstComputeMNTu(DD_IN_OUT DP_LINK_DATA* pDpLinkData);
DDU32 DpProtocolMstGetAvailableLinkPbn(DDU32 MaxLaneCount, DDU32 LinkRateMbps);
DDSTATUS DpProtocolMstComputeMNTuPbn(DD_IN_OUT DP_LINK_DATA* pDpLinkData);
DDSTATUS DpProtocolComputeMNTuPbnConsideringEoc(DD_IN_OUT DP_LINK_DATA* pDpLinkData);
DDSTATUS DpProtocolComputeMNTuPbnLinkLayerSymbolCount(DD_IN_OUT DP_LINK_DATA* pDpLinkData);
DDU64 DpProtocolComputePixelDataBwMbps(DDU64 DotClockInHz, DD_BITS_PER_PIXEL BitsPerPixel);
DDSTATUS DisplayInfoRoutinesCreateCVT2Timing(CREATE_CVT_TIMING_ARGS* pCreateCvtTimingArgs);

// Populatin Common Edid data for DP/Hdmi
void DisplayInfoRoutinePopulateEdidAndMonitorData(DD_VIDEO_OUTPUT_TECHNOLOGY SinkType, EDID_BASE_BLOCK** ppEDID, DD_TIMING_INFO* pTimingInfo, DDU8 DtdNum);
