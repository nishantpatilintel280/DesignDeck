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
 * @file  DisplayContext.h
 * @ingroup driver
 * @brief Private context definition and internal function declarations.
 * Display internal code is divided in to 3 modules:
 *   1. Display OS Service Layer a.k.a. DOSL/OSL/DSL
 *   2. Display Protocol Layer a.k.a. DPL
 *   3. Display HW Abstraction Layer a.k.a. DHL/DHAL/HAL
 *
 * *** All Display internal files are supposed to include this file *only* to get required interfaces and struct definitions ***
 *
 * The Interface access are limited to required components only via macro defines
 * e.g. only OS layer can call Protocol interfaces.
 * Each component does not have access to its own interface function table.
 *
 */

/**
 * @addtogroup driver
 * @{
 */

#ifndef _DISPLAY_CTXT_H_
#define _DISPLAY_CTXT_H_
#define _DISPLAY_CTXT_INC_START_

//-----------------------------------------------------------------------------
// read-only macros
//-----------------------------------------------------------------------------
#ifdef _DISPLAY_INTERNAL_
#define _MINIRW_ const
#define _DDRW_
#else // ! (_YANGRA_DISPLAY_)
#define _MINIRW_
#define _DDRW_ const
#endif // #ifdef _YANGRA_DISPLAY_

#if (defined(_DISPLAY_PROTOCOL_))
#define _DPL_RW_
#else
#define _DPL_RW_ const
#endif

#if (defined(_DISPLAY_HAL_))
#define _DHL_RW_
#else
#define _DHL_RW_ const
#endif

#define IS_GFX_FAMILY_BELOW(family1, family2) ((family1) < (family2) ? TRUE : FALSE)
#define IS_GFX_FAMILY_EQUAL_OR_ABOVE(family1, family2) ((family1) >= (family2) ? TRUE : FALSE)
#define IS_GFX_PRODUCT_EQUAL_OR_ABOVE(product1, product2) (((product1) >= (product2) ? TRUE : FALSE))
#define IS_GFX_PRODUCT_EQUAL_OR_BELOW(product1, product2) (((product1) <= (product2) ? TRUE : FALSE))

#define MAX_COMMIT_RETRY_DOD 3

//-----------------------------------------------------------------------------
// forward declaration
//-----------------------------------------------------------------------------

_DDRW_ typedef struct _DD_OSL_CONTEXT DD_OSL_CONTEXT;
_DPL_RW_ typedef struct _DD_PROTOCOL_CONTEXT DD_DPL_CONTEXT;
_DHL_RW_ typedef struct _DD_DHL_CONTEXT DD_DHL_CONTEXT;

_DHL_RW_ typedef struct _DD_DISPLAY_HAL_SERVICES DD_DISPLAY_HAL_SERVICES;
_DPL_RW_ typedef struct _DD_DISPLAY_PROTO_SERVICES DD_DISPLAY_PROTO_SERVICES;
typedef struct _DD_EXTERNAL_INTERFACE DD_EXTERNAL_INTERFACE;
typedef struct _DISPLAY_PC_CONTEXT DISPLAY_PC_CONTEXT;
typedef struct _DD_MCD_CONTEXT DD_MCD_CONTEXT;

//-----------------------------------------------------------------------------
// Interface Header inc
//-----------------------------------------------------------------------------
//#include <DisplayInterface.h>
//#include "pcu_access.h"
#include "DisplayArgsInternal.h"
#include "DisplayLogging.h"

#if (defined(_DISPLAY_INTERNAL_) && !defined(_DEBUG_EXPORT_))
//#include "DisplayProtocol/DisplayProtocolInterface.h"
//#include "DisplayHAL/DisplayHALInterface.h"
//#include "DisplayExtInterface.h"
#endif
//#include <sku_wa.h>
//#include <igfxfmid.h>

///////////////////////////////////////////////////////////////////////////////
//
// CONSTANTs / ENUMs
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// structs / unions
//
///////////////////////////////////////////////////////////////////////////////

#define DISPLAY_DMC_FW_MIN_SIZE_DWORDS (1 * 1024)  // 1K DWORDs = 4KBytes
#define DISPLAY_DMC_FW_MAX_SIZE_DWORDS (64 * 1024) // 64K DWORDs = 256KBytes

typedef struct UNDERRUN_ISR_CONFIG_REC
{
    DDU32 UnderrunLevel;
    DDU32 UnderrunCounter[MAX_PHYSICAL_PIPES];
} UNDERRUN_ISR_CONFIG;

typedef struct _GTT_PAGE_FAULT_DATA
{
    DDU16 GttFaultCounter[MAX_PHYSICAL_PIPES][MAX_PLANES_PER_PIPE];
} GTT_PAGE_FAULT_DATA;

typedef struct _MCD_CONFIG_CACHE
{
    DD_BOOL IsLda;
    DD_BOOL IsMcd;
} MCD_CONFIG_CACHE;

_DDRW_ typedef struct _DD_DISPLAY_DATA
{
    DD_BOOL IsSystemInTdr;
    UNDERRUN_ISR_CONFIG UnderRunISRConfig;
    DDU32 ScdcInterruptCounter[DD_PORT_TYPE_MAX_PHYSICAL_PORT];
    DDU8 MaxPipes;
    DD_BOOL DodEnabled;
    DD_BOOL IsSystemInBugCheck;
    DDU32 PlatformMaxCdClockInKhz;    // Max Cd Clock value supported by the Platform. Used in Ccd And HAL. CdClkFrequency stored in Khz
    DDU32 PlatformMaxCdClockBy2InKhz; // Max CdClock/2 value supported by the Platform. Used in Ccd And HAL. CdClkFrequency stored in Khz
    DDU32 CurrentCdClockInHz;         // Current programmed CD clock, used in DP/HDMI during modeset for calculations
    MCD_CONFIG_CACHE McdNextConfig;
    DDU32 OsBuildVersion;
    DD_BOOL HdmiFrlWaReportLinkLoss; // Flag to report link loss if system has recovered from underrun with HDMI 2.1 FR and HDCP session is getting enabled
} DD_DISPLAY_DATA;

typedef struct _DD_PCI_DATA
{
    // Device 2 - IGD
    DDU32 PCIVendorID;
    DDU32 PCIDeviceID;
    DDU32 PCISubsystemVenderID;
    DDU32 PCISubSystemID;
    DDU32 PCIRevisionID;
    DDU32 PCISlotNumber;
    DDU32 PCIBusNumber;

    DDU32 DisplayCoreFamily; // Interpreted from device ID
    DDU32 RenderCoreFamily;
    DDU32 PchCoreFamily; // uses PCH_PRODUCT_FAMILY
    DDU32 ProductFamily;
    DDU16 ProductRevId;

    DD_BOOL IsIntelPrimary;
} DD_PCI_DATA;

typedef struct _DD_PLATFORM_MEMORY_DETAILS
{
    DDU32 CoreFreq;         // GFX/GMCH core frequency in MHz
    DDU32 SystemMemFreq;    // System memory frequency in MHz
    DDU32 NumOfMemChannels; // Number of memory channels
    DDU32 MaxCDDotClock;    // Maximum Pipe A dotclock supported
    DDU32 PlatformType;
} DD_PLATFORM_MEMORY_DETAILS;

typedef struct _DD_PLATFORM_PARAMETERS
{
    DD_PCI_DATA PciData;
    DD_PLATFORM_MEMORY_DETAILS PlatformMemoryDetails;
    DD_BOOL IsRenderOnlyWithVirtualDisplay;

    // SKU data
    //SKU_FEATURE_TABLE *pSkuTable;

    // WA data - accessible only to GAL
    //WA_TABLE *pWaTable;

    // Static settings from Opregion/VBT params cached here
    DD_BOOL IsCDClockChangeSupported;
    DD_BOOL IsHeadlessSupportForKVMREnabled;
    DD_BOOL IsDgpuPresent;
    DD_BOOL IsDgpuMbDownConfig;
    DDU8 PhysicalAdapterIndex;

    DDU64 QpcFrequency; // Frequency of Query Performace Counter

    // Flag to check whether seamless cdclock change alone allowed
    DD_BOOL IsCd2XDividerChangeOnlySupported;

    DDU8 BDFData[4];

} DD_PLATFORM_PARAMETERS;

typedef struct _DD_OS_FTR_TABLE
{
    DDU32 OsFtrFlipQueue : 1;
} DD_OS_FTR_TABLE;

typedef struct _DD_HDCP_GSC_BUFFER
{
    void *pGmmBlock;
    DDU64 pGraphicsAddress;
    DDU8 *pLinearAddress;
    DDU32 BufferSize;
} DD_HDCP_GSC_BUFFER;

typedef struct _DD_HDCP_GSC_CONTEXT
{
    DD_HDCP_GSC_BUFFER HdcpCmdBuffer;   // HDCP Command buffer for HDCP-GSC transactions
    DD_HDCP_GSC_BUFFER HdcpBatchBuffer; // HDCP Batch Buffer for HDCP-GSC transactions
    DDU32 HdcpFenceID;                  // Auto-incrementing FenceId for HDCP submisisons
    void *pCompletionEvent;             // CompletionEvent for GSC submissions

    // Buffer used for DG flow
    DDU64 HdcpGscBuffer;  // Buffer for HDCP-GSC transactions
    DDU64 HdcpGscPhyAddr; // Physical address of GSC buffer
} DD_HDCP_GSC_CONTEXT;

typedef struct _DISPLAY_CONTEXT
{
    //HW_DEV_EXT *pHwDevExt; // cached pointer

    // private data context
    DD_OSL_CONTEXT *pOslContext;
    DD_DPL_CONTEXT *pDPLContext;
    DD_DHL_CONTEXT *pDHLContext;

    // public interfaces
    DD_DISPLAY_PROTO_SERVICES *pDPLServices;
    DD_DISPLAY_HAL_SERVICES *pDHLServices;

    DD_PLATFORM_PARAMETERS PlatformParams;

    // Display Global Data context
    DD_DISPLAY_DATA DisplayData;
    // OS Feature Support Table
    DD_OS_FTR_TABLE OsFtrTable;
    // Unified storage for the Diagnostic history and current state data
    DD_BLACK_BOX *pDisplayBlackBox;
    // spin lock for controlling access to the black box data
    DD_LOCKING_OBJ_ST BlackBoxResourceSpinlock;
    DD_LOCKING_OBJ_ST ModeTableSyncMutex;
    DD_HDCP_GSC_CONTEXT HdcpGscContext;   // HDCP GSC Context
    GTT_PAGE_FAULT_DATA GttPageFaultData; // Counter to disable GTT page fault

    // Handle from OS to track the driver-supplied callback routine for OS power plan notification callbacks
    HANDLE HandleIntelGfxPowerPlanCallback;

    // Tracks whether or not the driver was successfully registered for OS power plan notifications
    DD_BOOL RegisteredForOsPowerPlanNotifications;
    DD_MCD_CONTEXT *pMcdContext;
    DD_BOOL InStopDevice;
    DD_BOOL IsWakelockEnabled;

    // Temp fix for BSOD: TODO: Move to different place later
    DD_BRIGHTNESS_CONTROL_PORT_OVERRIDE_ARGS BrightnessPortOverrideArgs;

#if (_DEBUG || _RELEASE_INTERNAL)
    DDU32 DmcFwSizeInBytes;
    DDU32 *pDmcFw;
    // Multiplier value to be used only for Pre-Si to increase the timeout.
    DDU32 PreSiPolledReadMultiplier;
#endif // (_DEBUG || _RELEASE_INTERNAL)

#ifdef _DISPLAY_EXPORT_
    void *pDisplayExportCtxt;
#endif //_DISPLAY_EXPORT_
} DISPLAY_CONTEXT;

///////////////////////////////////////////////////////////////////////////////
//
// MACRO routines / function declaration / inline functions
//
///////////////////////////////////////////////////////////////////////////////

//#ifdef _YANGRA_DISPLAY_
//
//DD_PLATFORM_PARAMETERS *GetPlatformParameters(DISPLAY_CONTEXT *pDisplayContext);
//DD_DISPLAY_DATA *GetDisplayData(DISPLAY_CONTEXT *pDisplayContext);
//DD_OS_FTR_TABLE *GetOsFtrTable(DISPLAY_CONTEXT *pDisplayContext);
//DD_DPL_CONTEXT *GetProtocolContext(DISPLAY_CONTEXT *pDisplayContext);
//DD_OSL_CONTEXT *GetOSLContext(DISPLAY_CONTEXT *pDisplayContext);
//DD_DHL_CONTEXT *GetHALContext(DISPLAY_CONTEXT *pDisplayContext);
//DISPLAY_CONTEXT *GetDisplayCtxtFromHwDevExt(//HW_DEV_EXT *pHwDevExt);
//
//#if (defined(_DISPLAY_OSL_))
//DD_DISPLAY_PROTO_SERVICES *GetProtocolInterface(DISPLAY_CONTEXT *pDisplayContext);
//#endif
//
//#if (defined(_DISPLAY_OSL_) || defined(_DISPLAY_PROTOCOL_))
//DD_DISPLAY_HAL_SERVICES *GetHALInterface(DISPLAY_CONTEXT *pDisplayContext);
//#endif
//
//#endif // #ifdef _YANGRA_DISPLAY_

#undef _DISPLAY_CTXT_INC_START_
#endif // _DISPLAY_CTXT_H_

/** @} */ // end of file
