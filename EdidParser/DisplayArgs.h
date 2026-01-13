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
 * @file DisplayArgs.h
 * @ingroup driver
 * @brief This file contains definitions for display arguments shared outside of display:
 * other driver components or escape translation layers. This file is having definitions for New Display Rewrite code.
 *
 * Display header detailed description and scope after Yangra (Rewritten Display code) changes:
 *
 * Considering build and Link dependencies, the Kernel Mode Driver (KMD.sys) can be divided into 3 major components, namely:
 * 1. Common Miniport: It includes Core KMD Lib (having Common Code like escape etc. , Core Render), KMD Core GMM Lib, KMD PwrCons Lib, KCH Lib
 * 2. Display Legacy: It includes SoftBios Lib, Display Miniport Legacy Lib
 * 3. Display Yangra: It includes Display Rewrite libs (OS Layer, Protocol, HAL), Yangra Display Miniport Lib.
 * The Display Legacy and Display yangra Codes are mutualy exclusive.
 *
 * Header File Usage:
 * + External Headers:
 *     - The 'Common Miniport' and 'Display Yangra' will include new External Header i.e. Source/Display/Code/DisplayArgs.h.
 *     - The old external header Source/inc/common/SbArgs.h can be used by 'Display Legacy' files *only*.
 *     - Some of the Legacy definitions (from SBArgs.h) are used in 'Common Miniport' and Display interfaces.
 *       Those are kept unchanged and placed in 'Source/inc/common/DisplayLegacyShared.h'. i.e. DisplayLegacyShared.h will be shared between Display Legacy and Yangra paths.
 *       Any Display <-> Common Miniport interface args has to be declared here.
 * + Internal Headers:
 *     - Source/Display/Code/DisplayArgsInternal.h will be used inside Display Rewrite Libs (OS Layer, Protocol, HAL)
 *     - Display Legacy internal Headers remain unchanged.
 *
 *    +-----------------------------------------------+
 *    |                                               |
 *    |                                               |
 *    |            Display Legacy Shared              |
 *    |                                               |
 *    |                                               |
 *    +-----------------------------------------------+
 *             |                            |
 *             |                            |
 *             |                            |
 * +------------------------+    +---------------------+
 * |                        |    |                     |
 * |      Display Args      |    |                     |
 * |                        |    |        SB Args      |
 * |                        |    |                     |
 * +------------------------+    +---------------------+
 *             |
 *             |
 * +-------------------------------+
 * |                               |
 * |      Display Args Internal    |
 * |                               |
 * |                               |
 * +-------------------------------+
 *
 */

#pragma once

#include "DisplayDefs.h"
#include "DisplayErrorDef.h"
#include "DisplayLegacyShared.h"
#include "DisplayErrorDef.h"

 // ----------------------------------------------------------------------------
 //
 // Generic CONSTANTS, ENUMS and MACROS
 //
 // ----------------------------------------------------------------------------

 // Mode related
 // Increased the MAX_NUM_IDENTITY_TARGET_MODE_PER_SOURCE_MODE value to 42 after testing with Samsung 4K panel which
 // has RRs - 23,24,25,29,30,50,60 and addition of YCbCr422 Modes. Also, YCbCr420 Preferred mode gets added separately in the mode table.
#define MAX_NUM_IDENTITY_TARGET_MODE_PER_SOURCE_MODE 42
#define MAX_NUM_SCALED_TARGET_MODE_PER_SOURCE_MODE MAX_NUM_IDENTITY_TARGET_MODE_PER_SOURCE_MODE
#define MAX_NUM_TARGET_MODE_PER_SOURCE_MODE (MAX_NUM_IDENTITY_TARGET_MODE_PER_SOURCE_MODE + MAX_NUM_SCALED_TARGET_MODE_PER_SOURCE_MODE)
#define MAX_TARGET_MODES 0xFF
#define DD_MAX_VIEWS 4
#define MAX_EDID_BLOCKS 255 // According to E-EDID Standard doc.
#define EDID_BLOCK_SIZE (128)
#define MAX_EDID_BLOCK_SUPPORTED (32) // EDID override is supported only up to 32 blocks
#define WB_EDID_BLOCK_SIZE (256)
#define HF_EEODB_BLOCK_SIZE 2
#define INVALID_EDID_BLOCK_COUNT 0xFF
#define DDRW_VER 1
#define MAX_DID_AS_DESCRIPTORS 5 // Maximum number of descriptors that driver will parse in DID 2.1 Adaptive Sync data block
// Color related
#define DD_MAX_OS_PALETTE_TABLE_SIZE_LEGACY 256
#define DD_MAX_OS_PALETTE_TABLE_SIZE_IN_COLORSPACE_TRANSFORM 4096
#define DD_COLOR_1DLUT_MAX_NUM_SAMPLES DD_MAX_OS_PALETTE_TABLE_SIZE_IN_COLORSPACE_TRANSFORM
#define DD_COLOR_MATRIX_NUM_COEFFICIENTS 9
#define DD_COLOR_3DLUT_NUM_SAMPLES 4913 // (17*17*17) which is 17 samples per RGB channel
#define DEFAULT_SDR_WHITE_LEVEL 80

#define MAX_PHYSICAL_PIPES_GEN10 3
#define MAX_PHYSICAL_PIPES_GEN11 3
#define MAX_PHYSICAL_PIPES_GEN11P5 4
#define MAX_PHYSICAL_PIPES_GEN12 4
#define MAX_PHYSICAL_PIPES_GEN13 4
#define MAX_PHYSICAL_PIPES_ADLN 3
#define MAX_PHYSICAL_PIPES_GEN14 4
#define MAX_PHYSICAL_PIPES_WITH_PIPE_D_DISABLED 3
#define MAX_PHYSICAL_PIPES_WITH_PIPE_C_AND_D_DISABLED 2
#define MAX_PHYSICAL_PIPES_RKL 3
#define MAX_PHYSICAL_PIPES_GEN15 4
#define MAX_PHYSICAL_PIPES_XE3 4

#define MAX_SPRITE_PLANES_PER_PIPE (MAX_PLANES_PER_PIPE - 1)

#define MAX_TILES_SUPPORTED (MAX_PHYSICAL_PIPES)
#define TILES_SUPPORTED_PER_SINK 2                                                        // Currently sink supports max of 2 tiles, this needs to be modified if Sink supports > 2 tiles in future
#define MAX_JOINT_DISPLAY_SUPPORTED (MAX_PHYSICAL_PIPES_GEN13 / TILES_SUPPORTED_PER_SINK) // Taking GEN13 Enum as its the latest platform and has maximum pipes supported (4 Pipes)

#define INVALID_SCAN_LINE -1
#define DD_LINEAR_ALIGN 64
#define MAX_WRITEBACK_DEVICE 2

#define MAX_CUSTOM_MODES_SUPPORTED 20   // Limiting to 20 per display.
#define MAX_ADD_CUSTOM_MODES_PER_CALL 1 // Only 1 mode can be added at a time.

// #define MAX_PLANES_PER_PIPE     MAX_PLANES_PER_PIPE_GEN11

#define MAX_OVERRIDE_OUTPUT_FORMAT_REG_KEY_SIZE 64
#define MAX_MONITOR_FLICKER_PROFILE_REG_KEY_SIZE 64

#define LACE_MAX_NUM_ENTRIES_IN_LUX_AGGR_MAP 129    // Maximum number of Entries supported in Lace LuxVsAggr map
#define LACE_DEFAULT_NUM_ENTRIES_IN_LUX_AGGR_MAP 51 // Driver supported default number of Entries in Lace LuxVsAggr map
#define GHE_MAX_NUM_ENTRIES_IN_LUX_AGGR_MAP 129     // Maximum number of Entries supported in LuxVsAggr map
// Max supported Displays in SLS
#define MAX_PER_MCD_TARGETS 16 // 4 displays per GPU
#define PNP_BLOCK_SIZE 10

typedef enum _DD_ACPI_PORT_TYPE
{
    DD_CRT_TYPE = 1,
    DD_DFP_TYPE = 3,
    DD_LFP_TYPE = 4,
} DD_ACPI_PORT_TYPE;

#define DD_GET_DISPLAY_TYPE(Arg) (Arg >> 16)
// Don't use DD_IS_XX() macros in yangra, this logic is specific to legacy target id.
// @todo: VSDI-24599 Need to remove legacy specific Macros DD_IS_XX() from Yangra code
#define HDMI_CONNECTOR 6
#define DD_IS_HDMI(Arg) (((Arg >> 8) & 0x1F) == HDMI_CONNECTOR)
#define EXT_DP_CONNECTOR 14
#define DD_IS_DP(Arg) (((Arg >> 8) & 0x1F) == ExtDP_CONNECTOR)
#define TPV_CONNECTOR 16
#define DD_IS_TPV(Arg) 0 // (((Arg >> 8) & 0x1F) == TPV_CONNECTOR)
#define WIGIG_CONNECTOR 19
#define IS_WIGIG_DISPLAY(Arg) (((Arg >> 8) & 0x1F) == WIGIG_CONNECTOR)

#define DD_IS_LFP(TargetId) (((TargetId) & (1 << 23)) > 0)

#define DD_LDA_INDEX_FROM_TARGET_ID(ChildUid) (((ChildUid) >> 18) & 0x3)
#define DD_ADD_LDA_INDEX_TO_TARGET_ID(ChildId, Index) ((ChildId) | (((Index)&0x3) << 18))
#define DD_CLEAR_LDA_INDEX_FROM_TARGET_ID(ChildId) ((ChildId) & ~(3 << 18))

#define DD_MAX_HACTIVE_FOR_SCALING 5120
#define DD_MAX_SCALEABLE_SOURCE_SIZE 4096
#define DD_MAX_BEZEL_VALUE 200             // Max Allowed Bezel Value on any side of the panel is 200
#define DD_MAX_DP_MST_CONNECTOR_PER_PORT 4 // considering 4 MST max

#define DD_IS_VALID_S3DFORMAT(S3DFormat) \
    ((S3DFormat & DD_S3D_FORMAT_MASK(S3D_FRAME_PACKING)) || (S3DFormat & DD_S3D_FORMAT_MASK(S3D_TOP_BOTTOM)) || (S3DFormat & DD_S3D_FORMAT_MASK(S3D_SIDE_BY_SIDE_HALF)))
#define DD_MAX_REGISTRY_PATH_LENGTH 256

// To encapsulate RS1 specific code, enabled as of today, TBD for disabling
#define RS1 1

// Max DP ports
// Considering external DDIs for all platforms (DDI B to DDI I)
#define DD_MAX_DP_PORTS 8

// Max Internal Display Supported
#define DD_MAX_INTERNAL_DISPLAYS 2

// VSync Control for All Sources
#define DD_CTRL_VSYNC_SOURCE_ALL 0xFF

#define DD_LDA_MAX_ADAPTERS 4

#define MAX_CMTG 2 // Lunar lake supports two CMTGs

#define DD_INVALID_PROCESS_ID DDMAXU32

#define DD_MAX_PROCESS_NAME_CHAR 260
#define DD_CACHED_PROCESS_NAME_CHAR 16 // Though 260 is available from GMM, cache only 16 chars in MpoView

#define DD_TDR_DEFAULT_TDR_DELAY_MICROSEC \
    2 * 1000 * 1000 // The number of seconds that the GPU is allowed to delay the preempt request from the scheduler
                    // This is effectively the timeout threshold.
                    // The default value is 2 sec.

typedef enum _DD_CMTG
{
    CMTG_0,
    CMTG_1,
    CMTG_MAX
} DD_CMTG;

typedef struct _DD_RECT
{
    DDS32 Left;
    DDS32 Top;
    DDS32 Right;
    DDS32 Bottom;
} DD_RECT;

/**
 * @brief Intersection two rects calculated and updated in the out rect.
 * @param[in,out] pRcOut
 * @param[in] pRcClip Rect used for setting the Clippng boundaries.
 */
DD_INLINE void DDClipRect(DD_RECT* pRcOut, const DD_RECT* const pRcClip)
{
    DDASSERT(pRcOut);
    DDASSERT(pRcClip);
    pRcOut->Left = DD_MAX(pRcOut->Left, pRcClip->Left);
    pRcOut->Right = DD_MIN(pRcOut->Right, pRcClip->Right);
    pRcOut->Top = DD_MAX(pRcOut->Top, pRcClip->Top);
    pRcOut->Bottom = DD_MIN(pRcOut->Bottom, pRcClip->Bottom);
}

/**
 * @brief Moves the pRcOut rectangle by the specified offsets.
 * @param[in,out] pRcOut
 * @param[in] OffsetX
 * @param[in] OffsetY
 */
DD_INLINE void DDOffsetRect(DD_RECT* pRcOut, DDS32 OffsetX, DDS32 OffsetY)
{
    DDASSERT(pRcOut);
    pRcOut->Left += OffsetX;
    pRcOut->Right += OffsetX;
    pRcOut->Top += OffsetY;
    pRcOut->Bottom += OffsetY;
}

typedef enum _DD_LOCK_RES_TYPE
{
    DD_LOCK_TYPE_NONE,
    DD_LOCK_MUTEX,    // for passive level calls
    DD_LOCK_SPINLOCK, // for all IRQLs
    DD_LOCK_TYPE_MAX
} DD_LOCK_RES_TYPE_EN;

typedef struct _DD_LOCKING_OBJ
{
    DD_LOCK_RES_TYPE_EN LockType;
    void* pLock;
    DDU32 RefCount;
    DDU8 OldIrql;
    DD_BOOL IsIrqlLevelRaised;
    HANDLE ThreadID;
    DDU8 NumContendingClients; // debug purpose
} DD_LOCKING_OBJ_ST;

typedef struct _DD_ESCAPE
{
    DDU32 OpCode;
    DDU32 DataSize;
    void* pEscapeData;
} DD_ESCAPE;

typedef union _DD_FEATURE_TEST_CONTROL
{
    struct
    {
        DDU32 FtrCxsrDisable : DD_BITFIELD_BIT(0);
        DDU32 FtrFbcDisable : DD_BITFIELD_BIT(1);
        DDU32 FtrGsvDisable : DD_BITFIELD_BIT(2);
        DDU32 FtrBlcDisable : DD_BITFIELD_BIT(3);
        DDU32 FtrDpstDisable : DD_BITFIELD_BIT(4);
        DDU32 Reserved1 : DD_BITFIELD_BIT(5);
        DDU32 FtrDpsDisable : DD_BITFIELD_BIT(6);
        DDU32 FtrRsDisable : DD_BITFIELD_BIT(7);
        DDU32 Reserved2 : DD_BITFIELD_RANGE(8, 10);
        DDU32 FtrPsrDisable : DD_BITFIELD_BIT(11);
        DDU32 Reserved3 : DD_BITFIELD_BIT(12);
        DDU32 FtrAdtDisable : DD_BITFIELD_BIT(13);
        DDU32 Reserved4 : DD_BITFIELD_BIT(14);
        DDU32 FtrLaceDisable : DD_BITFIELD_BIT(15);
        DDU32 Reserved5 : DD_BITFIELD_RANGE(16, 31);
    };
    DDU32 Value;
} DD_FEATURE_TEST_CONTROL;

// PCI related
#define PCI_BUS_0 0
#define PCI_CTL_DEVICE 0
#define PCI_GFX_DEVICE 2
#define DEFAULT_FUNC_NUM 0xFF // Used for PCI calls only on PCI_GFX_DEVICE. Used to signal 'do not care whether the current HWDev is appropriate' during PCI calls.
#define GFX_FUNCTION_0 0x0
#define GFX_FUNCTION_1 0x1
#define PCI_CMD_OFFSET 0x4
#define PCI_COMMAND_REG_MEM_ENABLE_BIT 0x2

typedef enum _DD_PORT_TYPE
{
    DD_PORT_TYPE_UNKNOWN = -1,
    DD_PORT_TYPE_DIGITAL_PORT_A,
    DD_PORT_TYPE_DIGITAL_PORT_B,
    DD_PORT_TYPE_DIGITAL_PORT_C,
    DD_PORT_TYPE_DIGITAL_PORT_D,
    DD_PORT_TYPE_DIGITAL_PORT_E,
    DD_PORT_TYPE_DIGITAL_PORT_TC1,
    DD_PORT_TYPE_DIGITAL_PORT_F = DD_PORT_TYPE_DIGITAL_PORT_TC1, // Ports TC1-TC4 to be used from D13 onwards
    DD_PORT_TYPE_DIGITAL_PORT_TC2,
    DD_PORT_TYPE_DIGITAL_PORT_G = DD_PORT_TYPE_DIGITAL_PORT_TC2,
    DD_PORT_TYPE_DIGITAL_PORT_TC3,
    DD_PORT_TYPE_DIGITAL_PORT_H = DD_PORT_TYPE_DIGITAL_PORT_TC3,
    DD_PORT_TYPE_DIGITAL_PORT_TC4,
    DD_PORT_TYPE_DIGITAL_PORT_I = DD_PORT_TYPE_DIGITAL_PORT_TC4,
    DD_PORT_TYPE_DSI_PORT_0,
    DD_PORT_TYPE_DSI_PORT_1,
    DD_PORT_TYPE_MAX_PHYSICAL_PORT,
    DD_PORT_TYPE_WRITEBACK_PORT = DD_PORT_TYPE_MAX_PHYSICAL_PORT, // Will be written to memory,
    DD_PORT_TYPE_VIRTUAL_PORT,
    DD_PORT_TYPE_DYNAMIC_PORT, // Will be used for KVM
    DD_PORT_TYPE_COLLAGE_PORT_0,
    DD_PORT_TYPE_COLLAGE_PORT_1,
    DD_PORT_TYPE_MAX_COLLAGE_PORT, // Add Port_N if number of collage ports increase in future
    DD_PORT_TYPE_MAX
} DD_PORT_TYPE;

#define IS_VALID_PORT_TYPE(port) (((DD_PORT_TYPE)port) > DD_PORT_TYPE_UNKNOWN && ((DD_PORT_TYPE)port) < DD_PORT_TYPE_MAX)
#define IS_MIPI_DSI_PORT_TYPE(port) ((((DD_PORT_TYPE)port) == DD_PORT_TYPE_DSI_PORT_0) || (((DD_PORT_TYPE)port) == DD_PORT_TYPE_DSI_PORT_1))
#define IS_PHYSICAL_PORT_TYPE(port) ((((DD_PORT_TYPE)port) >= DD_PORT_TYPE_DIGITAL_PORT_A) && (((DD_PORT_TYPE)port) < DD_PORT_TYPE_MAX_PHYSICAL_PORT))
#define IS_COLLAGE_PORT_TYPE(port) ((((DD_PORT_TYPE)port) >= DD_PORT_TYPE_COLLAGE_PORT_0) && (((DD_PORT_TYPE)port) < DD_PORT_TYPE_MAX_COLLAGE_PORT))

typedef enum _DD_PORT_PHY_INSTANCE
{
    DEDICATED_PORT_PHY,
    NON_DEDICATED_PORT_PHY,
} DD_PORT_PHY_INSTANCE;

typedef enum _PC_LFP_NUM
{
    LFP_0,
    LFP_1,
    LFP_MAX,
} DD_LFP_PANEL;

typedef enum _DD_HDMI_VERSION
{
    HDMI_VERSION_NONE = 0, // DVI display
    HDMI_VERSION_1_4,      // HDMI 1.4
    HDMI_VERSION_2_0,      // HDMI 2.0
    HDMI_VERSION_2_1,      // HDMI 2.1
} DD_HDMI_VERSION;

// Arrange Plane numbers in ascending order starting from Plane 1, they will be accessed using bitwise operation (1 << PlaneIndex) in HAL Plane.
typedef union _DD_FLIP_DONE_STATUS
{
    DDU32 Value;
    struct
    {
        DDU32 Plane1FlipDoneStatus : 1;
        DDU32 Plane2FlipDoneStatus : 1;
        DDU32 Plane3FlipDoneStatus : 1;
        DDU32 Plane4FlipDoneStatus : 1;
        DDU32 Plane5FlipDoneStatus : 1;
        DDU32 Plane6FlipDoneStatus : 1;
        DDU32 Plane7FlipDoneStatus : 1;
        DDU32 Reserved : 25;
    };
} DD_FLIP_DONE_STATUS;

typedef enum _DD_UNDERRUN_TYPE
{
    DD_UNDERRUN_TYPE_UNDEFINED,
    DD_UNDERRUN_TYPE_HARDUNDERRUN,
    DD_UNDERRUN_TYPE_SOFTUNDERRUN,
    DD_UNDERRUN_TYPE_PORTUNDERRUN,
    DD_UNDERRUN_TYPE_PAVPTEARDOWN,
} DD_UNDERRUN_TYPE;

typedef enum _DD_DMC_ERROR_CODE
{
    DD_UNKNOWN_ERROR,
    DD_INT_1_INVALID_SCANLINE_COMP_ERROR = 0x1,
    DD_INT_2_DC6V_FLIPQ_OVERLAP_ERROR = 0x2,
    DD_INT_FF_PIPE_DMC_FLIPQ_DONE_INT = 0xFF
} DD_DMC_ERROR_CODE;

typedef enum _DD_INTR_SVC_EVENT
{
    DD_SET_HOTPLUG_EVENT = 0,
    DD_SET_SCDC_EVENT,
    DD_SET_PLANE_FLIP_DONE_EVENT,
    DD_SET_UNDERRUN_EVENT,
    DD_SET_SCANLINE_EVENT,
    DD_SET_HISTOGRAM_EVENT,
    DD_SET_VBLANK_EVENT,
    DD_SET_AUDIO_EVENT,
    DD_SET_AUDIO_MAILBOX_EVENT,
    DD_SET_GTT_FAULT_EVENT,
    DD_SET_RM_TIMEOUT_EVENT,
    DD_SET_MAIN_DMC_EVENT,
    DD_SET_PIPE_DMC_FLIPQ_EVENT,
    DD_SET_DBUF_OVERLAP_DETECTED,
    DD_SET_MAIN_DMC_ERROR_EVENT,
    DD_SET_PIPE_DMC_ERROR_EVENT,
    DD_SET_PIPE_DMC_RM_NO_CLAIM_EVENT,
    DD_SET_THERMAL_PANIC_EVENT,
} DD_INTR_SVC_EVENT;

typedef struct _DD_INTR_SVC_ARGS
{
    DD_INTR_SVC_EVENT EventType;
    INTERRUPT_OPERATION Operation;
    PIPE_ID PipeId;
    PLANE_TYPE PlaneType;
    DD_PORT_TYPE Port;
} DD_INTR_SVC_ARGS;

typedef enum _DD_GET_INTR_EVENT
{
    DD_GET_AUDIO_ENC_PREF_EVENT,
    DD_GET_FLIP_DONE_STATUS,
    DD_GET_UNDERRUN_TYPE,
    DD_GET_MAIN_DMC_ERROR,
    DD_GET_PIPE_DMC_ERROR
} DD_GET_INTR_EVENT;

typedef enum _DD_DSI_INFO_OPERATION_TYPE
{
    DD_GET_DSI_PORT_TYPE = 1,
    DD_GET_DSI_PIPE,
    DD_GET_DSI_IS_CMD_MODE,
    DD_IS_DSI_CONFIGURED_IN_VBT
} DD_DSI_INFO_OPERATION_TYPE;

typedef struct _DD_GET_DSI_INFO
{
    DD_IN DD_DSI_INFO_OPERATION_TYPE OpCode;
    DD_IN_OUT PIPE_ID Pipe;
    DD_IN_OUT DD_PORT_TYPE Port;
    DD_OUT DD_BOOL IsCmdMode;
    DD_OUT DD_BOOL IsMipiDsiConfiguredInVbt;
} DD_GET_DSI_INFO;

typedef union _DD_CMTG_STATE
{
    DDU8 CmtgPipeInfo;
    struct
    {
        DDU8 PipeA : 1;
        DDU8 PipeB : 1;
        DDU8 PipeC : 1;
        DDU8 PipeD : 1;
        DDU8 Reserved : 4;
    };
} DD_CMTG_STATE;

// Hot plug Event Args
#define DD_HOTPLUG_EVENT_NONE 0x0
#define DD_HOTPLUG_EVENT_SHORT_PULSE 0x1
#define DD_HOTPLUG_EVENT_LONG_PULSE 0x2

typedef struct _DD_DETECT_HOTPLUG_EVENT_ARGS
{
    DD_IN DD_PORT_TYPE Port;
    DD_OUT DDU8 HotplugEventFlags; // Bit 0: Short pulse, bit 1: Long pulse
} DD_DETECT_HOTPLUG_EVENT_ARGS_ST;

typedef struct _DD_GET_DISPLAY_INTR_DATA
{
    DD_GET_INTR_EVENT EventType;
    PIPE_ID PipeId;
    union
    {
        DD_FLIP_DONE_STATUS FlipDoneStatus; // Valid only when DD_GET_FLIP_DONE_STATUS is EventType
        DD_BOOL EncPreference;              // Valid only when DD_GET_AUDIO_ENC_PREF is EventType
        DD_UNDERRUN_TYPE UnderrunType;      // Valid only when DD_GET_UNDERRUN_TYPE is EventType
        DD_DMC_ERROR_CODE DmcErrorCode;     // Valid only when DD_GET_*_DMC_ERROR is EventType
    };
} DD_GET_DISPLAY_INTR_DATA;

typedef struct _DD_SET_HOTPLUG_ARGS
{
    DD_PORT_TYPE Port;
    DD_BOOL IsEnable;
} DD_SET_HOTPLUG_ARGS;

typedef union _DD_PIPE_DMC_INTR_REQ
{
    DDU8 Value;
    struct
    {
        DDU8 AtsFaultIntr : DD_BITFIELD_BIT(0);
        DDU8 GttFaultIntr : DD_BITFIELD_BIT(1);
        DDU8 ErrorIntr : DD_BITFIELD_BIT(2);
        DDU8 FlipQProgrammingDoneIntr : DD_BITFIELD_BIT(3);
        DDU8 RmNoClaimErrorIntr : DD_BITFIELD_BIT(4);
        DDU8 UNIQUENAME(Reserved) : DD_BITFIELD_RANGE(5, 7);
    };
} DD_PIPE_DMC_INTR_REQ;
C_ASSERT(sizeof(DD_PIPE_DMC_INTR_REQ) == sizeof(DDU8));

typedef struct _DD_GET_SET_PIPE_DMC_INTR
{
    DD_IN PIPE_ID PipeId;
    DD_IN DD_BOOL IsEnable;
    DD_IN DD_BOOL FirstEnable;          // Set enabling first level interrupt from Disable/Mask
    DD_IN DD_PIPE_DMC_INTR_REQ ReqFlag; // Filled only for set
    DD_OUT DD_PIPE_DMC_INTR_REQ CurrFlag;
    DD_OUT DDU8 DmcErrorCode;
} DD_GET_SET_PIPE_DMC_INTR;

typedef struct _DD_PCI_CFG_DATA
{
    DDU32 CoreFreq;         // GFX/GMCH core frequency in MHz
    DDU32 SystemMemFreq;    // System memory frequency in MHz
    DDU32 RenderFreq;       // GFX Render Clock Frequency
    DDU32 NumOfMemChannels; // Num of memory channels
} DD_PCI_CFG_DATA;

// Dual LFP Hinge Based Panel Alignment Data from VBT (VSRI-4335)
typedef struct _DD_PANEL_CUSTOM_ALIGN_DATA
{
    DDU8 TopDummyActiveScanlines;    // This comes from VBT prior to Version 233, else from Opregion
    DDU8 BottomDummyActiveScanlines; // This comes from VBT prior to Version 233, else from Opregion
    DDU8 LeftDummyActiveScanlines;   // This comes from VBT Version 238
    DDU8 RightDummyActiveScanlines;  // This comes from VBT Version 238
} DD_PANEL_CUSTOM_ALIGN_DATA;

//  Opregion Platform Parameters
typedef struct _DD_OPREGION_PLATFORM_PARAMS
{
    DDU8 GopVersion[0x20]; // GOP version
    DD_BOOL IsS0ixCapable;
    DD_BOOL IsIsctCapable;
    DD_BOOL IsDgpuPresent;
    DD_BOOL IsDgpuMbDownConfig;
    DD_BOOL IsVramSrSupport;
    DD_BOOL IsOpregionSupportForPanelAlignmentPresent; // Indicates if Opregion version > 2.4 where the new fields for panel alignment were added
    DD_PANEL_CUSTOM_ALIGN_DATA PanelCustomAlignData[LFP_MAX];
} DD_OPREGION_PLATFORM_PARAMS;

typedef struct _DD_GET_VBT
{
    DDU8* pVbtData; // VBT Data from Opregion
} DD_GET_VBT;

typedef struct _DD_GET_EDID
{
    DD_IN DDU32 ChildId;              // Child ID for which we need to query _DDC
    DD_OUT DDU8 NumEdidBlockReturned; // Number of blocks of EDID returned, here we support only 128 byte blocks, If 0, nothing returned
    DD_OUT DDU8* pEdidData;           // EDID returned by _DDC method
} DD_GET_EDID;

typedef struct _DD_GET_OPREGION_EDID
{
    DDU8* pEdidData; // EDID Data from Opregion
    DD_BOOL IsValid; // OpRegion has valid Edid.
    DDU8 BlockNum;   // Requested block num.
    DDU8 Buffersize; // Size of Buffer; 4 byte Header or Full 128 Bytes
} DD_GET_OPREGION_EDID;

#define ACPI_DISPLAY_INDEX_MASK 0xF
#define DD_MB2_BCLM_FIELD_SIZE 30
#define DD_MB3_BCLM_FIELD_SIZE 20

typedef struct _DD_BCM_FIELD
{
    DDU32 DesiredDutyCycle;  // Indicates the brightness level for LFP1 and LFP2
    DDU16 BrightnessPercent; // Indicates Brightness Percentage
} DD_BCM_FIELD;

// Enum added for the updating of plane pixel format
// DDK/OS independent values defined from DD
// Union of all supported source pixel formats of GMCH
// Only non-alpha formats (ones with X) are valid for set mode operation with >8bpp
typedef enum _DD_PIXELFORMAT
{
    // IF ANY NEW FORMAT IS ADDED HERE, PLEASE UPDATE ALL THE BELOW MACORS.
    DD_8BPP_INDEXED = 0,
    DD_B5G6R5X0,
    DD_B8G8R8X8,
    DD_R8G8B8X8,
    DD_B10G10R10X2,
    DD_R10G10B10X2,
    DD_R10G10B10X2_XR_BIAS,
    DD_R16G16B16X16F,
    DD_YUV422_8,
    DD_YUV422_10,
    DD_YUV422_12,
    DD_YUV422_16,
    DD_YUV444_8,
    DD_YUV444_10,
    DD_YUV444_12,
    DD_YUV444_16,
    DD_NV12YUV420,
    DD_P010YUV420,
    DD_P012YUV420,
    DD_P016YUV420,
    DD_MAX_PIXELFORMAT
    // IF ANY NEW FORMAT IS ADDED HERE, PLEASE UPDATE ALL THE BELOW MACORS.
} DD_PIXELFORMAT;
// Pixel format is being stored in the form of a 32 bit bask in source mode. Therefore we would need updates there if Max pixel format were to increase beyond 32.
C_ASSERT(DD_MAX_PIXELFORMAT <= (sizeof(DDU32) * 8));

// PixelFormat related Macros start here.
#define DD_PIXELFORMAT_GETMASK(PixelFormat) (1 << PixelFormat)

#define DD_32_BPP_RGB_PIXELFORMAT_BITMASK                                                                                                                         \
    DD_PIXELFORMAT_GETMASK(DD_B8G8R8X8) | DD_PIXELFORMAT_GETMASK(DD_R8G8B8X8) | DD_PIXELFORMAT_GETMASK(DD_B10G10R10X2) | DD_PIXELFORMAT_GETMASK(DD_R10G10B10X2) | \
    DD_PIXELFORMAT_GETMASK(DD_R10G10B10X2_XR_BIAS)
#define DD_64_BPP_RGB_PIXELFORMAT_BITMASK DD_PIXELFORMAT_GETMASK(DD_R16G16B16X16F)

#define DD_32_64_BPP_RGB_PIXELFORMAT_BITMASK DD_32_BPP_RGB_PIXELFORMAT_BITMASK | DD_64_BPP_RGB_PIXELFORMAT_BITMASK

#define DD_YUVXXX_RGB_PIXELFORMAT_BITMASK                                                                                                                                                            \
    DD_PIXELFORMAT_GETMASK(DD_YUV422_8) | DD_PIXELFORMAT_GETMASK(DD_YUV422_10) | DD_PIXELFORMAT_GETMASK(DD_YUV422_12) | DD_PIXELFORMAT_GETMASK(DD_YUV422_16) | DD_PIXELFORMAT_GETMASK(DD_YUV444_8) | \
    DD_PIXELFORMAT_GETMASK(DD_YUV444_10) | DD_PIXELFORMAT_GETMASK(DD_YUV444_12) | DD_PIXELFORMAT_GETMASK(DD_YUV444_16)

#define DD_P0XX_RGB_PIXELFORMAT_BITMASK DD_PIXELFORMAT_GETMASK(DD_P010YUV420) | DD_PIXELFORMAT_GETMASK(DD_P012YUV420) | DD_PIXELFORMAT_GETMASK(DD_P016YUV420)

#define DD_NV12YUV420_RGB_PIXELFORMAT_BITMASK DD_PIXELFORMAT_GETMASK(DD_NV12YUV420)

#define DD_ALL_BPP_RGB_PIXELFORMAT_BITMASK DD_32_64_BPP_RGB_PIXELFORMAT_BITMASK | DD_YUVXXX_RGB_PIXELFORMAT_BITMASK | DD_P0XX_RGB_PIXELFORMAT_BITMASK | DD_NV12YUV420_RGB_PIXELFORMAT_BITMASK

DD_INLINE DD_BOOL DD_IS_PLANAR_PIXELFORMAT(DD_PIXELFORMAT PixelFormat)
{
    DD_BOOL PlanarFormat;
    switch (PixelFormat)
    {
    case DD_NV12YUV420:
    case DD_P010YUV420:
    case DD_P012YUV420:
    case DD_P016YUV420:
        PlanarFormat = TRUE;
        break;
    default:
        PlanarFormat = FALSE;
        break;
    }
    return PlanarFormat;
}

// Below link is helpful in determining BPP.
// https://msdn.microsoft.com/en-us/library/windows/desktop/bb970578(v=vs.85).aspx#_422formats
DD_INLINE DDU8 DD_GetBPPfromPixelFormat(DD_PIXELFORMAT PixelFormat)
{
    DDU8 Bpp;
    switch (PixelFormat)
    {
    case DD_8BPP_INDEXED:
        Bpp = 8;
        break;
    case DD_NV12YUV420:
        Bpp = 12;
        break;
    case DD_YUV422_8:
    case DD_B5G6R5X0:
        Bpp = 16;
        break;
    case DD_P010YUV420:
    case DD_P012YUV420:
    case DD_P016YUV420:
        Bpp = 24;
        break;
    case DD_B8G8R8X8:
    case DD_R8G8B8X8:
    case DD_B10G10R10X2:
    case DD_R10G10B10X2:
    case DD_R10G10B10X2_XR_BIAS:
    case DD_YUV422_10:
    case DD_YUV422_12:
    case DD_YUV422_16:
    case DD_YUV444_8:
    case DD_YUV444_10:
        Bpp = 32;
        break;
    case DD_YUV444_12:
    case DD_YUV444_16:
    case DD_R16G16B16X16F:
        Bpp = 64;
        break;
    default:
        DDRLSASSERT_UNEXPECTED(0, "");
        Bpp = 32; // default
        break;
    }
    return Bpp;
}

typedef union _DD_SCALING_SUPPORT
{
    struct
    {
        DDU32 Identity : 1;
        DDU32 Centered : 1;
        DDU32 Stretched : 1;
        DDU32 AspectRatioCenteredMax : 1;
        DDU32 Custom : 1;
        DDU32 Reserved : 27;
    };
    DDU32 ScalingSupport;
} DD_SCALING_SUPPORT;

typedef union _DD_SOURCE_MODE_ID
{
    DDU32 Value;
    struct
    {
        DDU32 OsPixelFormatIndex : 8;
        DDU32 TgtUniqueIndex : 8;
        DDU32 Index : 16;
    };
} DD_SOURCE_MODE_ID;

typedef struct _DD_SOURCE_MODE_INFO
{
    DD_SOURCE_MODE_ID ModeId;
    DDU32 VisibleScreenX;
    DDU32 VisibleScreenY;
    DDU32 PixelFormatMask;
    DDU32 S3DFormatMask; // Mask of DD_S3D_FORMAT
    DDU8 NumMappedTgtModes;
    DDU8 MappedTgtModeIndex[MAX_NUM_TARGET_MODE_PER_SOURCE_MODE];
} DD_SOURCE_MODE_INFO;

typedef union _DD_CE_ASPECT_RATIO
{
    DDU8 Value;
    struct
    {
        DDU8 Is_Avi_Par_4_3 : 1;
        DDU8 Is_Avi_Par_16_9 : 1;
        DDU8 Is_Avi_Par_64_27 : 1;
        DDU8 ReservedCePar : 5;
    };
} DD_CE_ASPECT_RATIO;

typedef union _DD_OVERRIDE_BPC
{
    struct
    {
        DDU16 DefaultBpc : 1;
        DDU16 Override6Bpc : 1;
        DDU16 Override8Bpc : 1;
        DDU16 Override10Bpc : 1;
        DDU16 Override12Bpc : 1;
        DDU16 Reserved : 11;
    };
    DDU16 OverrideBpc;
} DD_OVERRIDE_BPC;

typedef union _DD_OVERRIDE_ENCODING
{
    struct
    {
        DDU16 DefaultEncoding : 1;
        DDU16 RgbEncoding : 1;
        DDU16 YCbCr420Encoding : 1;
        DDU16 YCbCr422Encoding : 1;
        DDU16 YCbCr444Encoding : 1;
        DDU16 Reserved : 11;
    };
    DDU16 OverrideOutputEncoding;
} DD_OVERRIDE_ENCODING;

typedef struct _DD_OVERRIDE_OUTPUTFORMAT
{
    DDU32 TargetId;
    DD_BOOL EnableOverride;
    DD_OVERRIDE_BPC CurrentOverrideBpc;
    DD_OVERRIDE_BPC SupportedOverrideBpc;
    DD_OVERRIDE_ENCODING CurrentOverrideEncoding;
    DD_OVERRIDE_ENCODING SupportedOverrideEncoding;
} DD_OVERRIDE_OUTPUTFORMAT;

typedef union _DD_SAMPLING_MODE
{
    DDU8 Value;
    struct
    {
        DDU8 Rgb : 1;
        DDU8 Yuv420 : 1;
        DDU8 Yuv422 : 1;
        DDU8 Yuv444 : 1;
        DDU8 Reserved : 4;
    };
} DD_SAMPLING_MODE;

typedef union _DD_BPC_SUPPORTED
{
    DDU16 ColorDepthMask;
    struct
    {
        DDU16 SupportsRGB565Color : 1;
        DDU16 Supports6BitsPerColor : 1;
        DDU16 Supports6BitsLooselyPackedColor : 1;
        DDU16 Supports8BitsPerColor : 1;
        DDU16 Supports10BitsPerColor : 1;
        DDU16 Supports12BitsPerColor : 1;
        DDU16 Supports14BitsPerColor : 1;
        DDU16 Supports16BitsPerColor : 1;
        DDU16 SupportsCompressedBits : 1;
        DDU16 ColordepthReserved : 7;
    };
} DD_BPC_SUPPORTED;

typedef struct _DD_SAMPLING_MODE_BPC_MASK
{
    DD_BPC_SUPPORTED RgbSamplingModeBpcMask;
    DD_BPC_SUPPORTED Yuv420SamplingModeBpcMask;
    DD_BPC_SUPPORTED Yuv422SamplingModeBpcMask;
    DD_BPC_SUPPORTED Yuv444SamplingModeBpcMask;
} DD_SAMPLING_MODE_BPC_MASK;

typedef struct _DD_CE_DATA
{
    union
    {
        DDU8 Value;
        struct
        {
            DDU8 PixelReplication : 5; // Pixel replication associated with the timing, 0=non-replicated timing
            DDU8 ReservedCeData : 3;   // Reserved for Future use
        };
    };

    DD_SAMPLING_MODE SamplingMode;                        // SamplingMode is of format DD_SAMPLING_MODE associated with the timing
    DD_SAMPLING_MODE_BPC_MASK SamplingModeBpcMask;        // Contains the Supported BPC Mask of all the sampling modes supported by the mode
    DD_CE_ASPECT_RATIO Par[MAX_PARS_POSSIBLE_WITH_1_VIC]; // AspectRatio corresponding to per VIC stored for same timing
    DD_BOOL IsNativeFormat[MAX_PARS_POSSIBLE_WITH_1_VIC]; // Native Format bit extracted from SVD/420 block which is per VIC
    DDU8 VicId[MAX_PARS_POSSIBLE_WITH_1_VIC];             // Valid VicID 1 - 127. 0XFF - Vic not defined for mode
    DDU8 VicId4k2k;                                       // 4kx2k mode VicID 1 - 4. Valid only if VicId[0] == 0 (HDMI 4kx2k modes in VSDB block)
} DD_CE_DATA;

typedef enum _MSO_NUMLINKS
{
    NON_SEGMENTED = 0,
    TWO_SST_LINKS = 2,
    FOUR_SST_LINKS = 4
} MSO_NUMLINKS;

DD_INLINE DDU8 DD_GetMaxBPC(DD_BPC_SUPPORTED BpcMask)
{
    if (BpcMask.Supports16BitsPerColor)
    {
        return 16;
    }
    else if (BpcMask.Supports14BitsPerColor)
    {
        return 14;
    }
    else if (BpcMask.Supports12BitsPerColor)
    {
        return 12;
    }
    else if (BpcMask.Supports10BitsPerColor)
    {
        return 10;
    }
    else if (BpcMask.Supports8BitsPerColor)
    {
        return 8;
    }
    else if (BpcMask.Supports6BitsPerColor)
    {
        return 6;
    }
    else
    {
        return 0;
    }
}

DD_INLINE DDU8 DD_GetMaxBitsPerPixel(DD_BPC_SUPPORTED BpcMask)
{
    if (BpcMask.Supports16BitsPerColor)
    {
        return 48;
    }
    else if (BpcMask.Supports14BitsPerColor)
    {
        return 42;
    }
    else if (BpcMask.Supports12BitsPerColor)
    {
        return 36;
    }
    else if (BpcMask.Supports10BitsPerColor)
    {
        return 30;
    }
    else if (BpcMask.Supports8BitsPerColor || BpcMask.Supports6BitsLooselyPackedColor)
    {
        return 24;
    }
    else if (BpcMask.Supports6BitsPerColor)
    {
        return 18;
    }
    else if (BpcMask.SupportsRGB565Color)
    {
        return 16;
    }
    else
    {
        return 0;
    }
}

// TIMING_FLAGS used in the TIMING_INFO structure
typedef struct _DD_TIMING_FLAGS
{
    DD_BPC_SUPPORTED SupportedBPCMask; // Indicates the Supported BPC.This is used for mode enumeration only.
    DDU8 PixelOverlapCount;
    DDU8 DscSlicesPerScanline; // Indicates the no of Slices required for given resolution
    union
    {
        DDU8 FlagsValue;
        struct
        {
            DDU8 PreferredMode : 1; // 1 - Preferred Mode
            DDU8 NumLinks : 3;      // NumLinks used for MSO in pipe-Timing programmings
            DDU8 DscCapable : 1;    // Indicates the Dsc Capability for the given timing
            DDU8 IsFecCapable : 1;  // Indicates the FEC should be enabled or disabled for the given timing
            DDU8 Is64BppPossible : 1;
            DDU8 IsCombinedDisplay : 1; // Indicates if the Timing enumerated for Combined display
        };
    };
} DD_TIMING_FLAGS;

typedef union _DD_TAREGT_MODE_ID
{
    DDU32 Value;
    struct
    {
        DDU32 TgtUniqueIndex : 8;
        DDU32 Index : 24;
    };
} DD_TAREGT_MODE_ID;

typedef enum _DD_COLOR_MODEL
{
    DD_COLOR_MODEL_UNKNOWN = 0,
    DD_COLOR_MODEL_RGB = 1,
    DD_COLOR_MODEL_YCBCR_601 = 2,
    DD_COLOR_MODEL_YCBCR_709 = 3,
    DD_COLOR_MODEL_YCBCR_2020 = 4,
    DD_COLOR_MODEL_YCBCR_PREFERRED = 5,
    DD_COLOR_MODEL_SCRGB = 6,
    DD_COLOR_MODEL_INTENSITY_ONLY = 7,
    DD_COLOR_MODEL_CUSTOM = 8,
    DD_COLOR_MODEL_MAX
} DD_COLOR_MODEL;

// MODE_TYPE - Various type of modes
typedef enum _DD_MODE_TYPE
{
    DD_MODE_TYPE_UNKNOWN,
    DD_EDID_MODE, // General monitor specific modes
    DD_NO_EDID_DEFAULT_MODE,
    DD_MEDIA_RR_MODE,
    DD_OS_ADDL_MODE,
    DD_PIPE_JOINED_MODE_BIG_JOINER,   // Two Pipe Joined Mode
    DD_PIPE_JOINED_MODE_ULTRA_JOINER, // Four Pipe Joined Mode
    DD_TILED_MODE,
    DD_COLLAGE_MODE,
    // #if 0
    //     DD_OEM_CUSTOM_MODE,     // OEM customizable modes
    //     DD_USER_STANDARD_CUSTOM_MODE, // Custom modes added through CUI's Standard Custom Mode page.
    //     DD_USER_DETAILED_CUSTOM_MODE // Custom modes added through CUI's Detailed Custom Mode page
    // #endif // 0
    DD_MODE_TYPE_MAX,
} DD_MODE_TYPE;
C_ASSERT(DD_PIPE_JOINED_MODE_ULTRA_JOINER > DD_PIPE_JOINED_MODE_BIG_JOINER);

// Enum to mention the source of the mode
typedef enum _MODE_SOURCE
{
    BASE_DTD,
    CE_DTD,
    CE_SVD,
    EST_TIMING,
    EST_TIMING_III,
    STD_TIMING,
    DID_DTD,
} MODE_SOURCE;

#define DD_IS_JOINED_MODE(ModeType) ((DD_PIPE_JOINED_MODE_BIG_JOINER == ModeType) || (DD_PIPE_JOINED_MODE_ULTRA_JOINER == ModeType) || (DD_TILED_MODE == ModeType) || (DD_COLLAGE_MODE == ModeType))
#define DD_IS_TILED_OR_COLLAGE_MODE(ModeType) ((DD_TILED_MODE == ModeType) || (DD_COLLAGE_MODE == ModeType))
#define DD_IS_PIPE_JOINED_MODE(ModeType) ((DD_PIPE_JOINED_MODE_BIG_JOINER == ModeType) || (DD_PIPE_JOINED_MODE_ULTRA_JOINER == ModeType))

typedef struct _DD_CUSTOM_SRC_MODE
{
    DDU32 SourceX; // CustomMode Source X Size
    DDU32 SourceY; // CustomMode Source Y Size
} DD_CUSTOM_SRC_MODE;

typedef enum _DD_SIGNAL_STANDARD
{
    DD_SIGNAL_UNKNOWN = 0,
    DD_VESA_DMT = 1,
    DD_VESA_GTF = 2,
    DD_VESA_CVT = 3,
    DD_CEA_861B,

    // Need to add TV related standards here.
} DD_SIGNAL_STANDARD;

typedef enum _DD_COLOR_RANGE_TYPE
{
    DD_COLOR_RANGE_TYPE_DEFAULT = 0,
    DD_COLOR_RANGE_TYPE_LIMITED = 1,
    DD_COLOR_RANGE_TYPE_FULL = 2,
    DD_COLOR_RANGE_TYPE_MAX
} DD_COLOR_RANGE_TYPE;

typedef enum _DD_COLOR_YCBCR_SUBSAMPLING
{
    DD_COLOR_SUBSAMPLING_UNKNOWN = 0,
    DD_COLOR_SUBSAMPLING_444_PACKED = 1,
    DD_COLOR_SUBSAMPLING_422_PACKED = 2,
    DD_COLOR_SUBSAMPLING_422_PLANAR = 3,
    DD_COLOR_SUBSAMPLING_420_PLANAR = 4, // NV12, P0xx YCbCr formats
    DD_COLOR_SUBSAMPLING_MAX
} DD_COLOR_YCBCR_SUBSAMPLING;

typedef enum _DD_COLOR_ENCODING
{
    DD_COLOR_ENCODING_UNKNOWN = 0,
    DD_COLOR_ENCODING_LINEAR = 1,
    DD_COLOR_ENCODING_SRGB_GAMMA = 2,
    DD_COLOR_ENCODING_ST2084 = 3,
    DD_COLOR_ENCODING_HLG = 4, // HLG - Hybrid Log Gamma
    DD_COLOR_ENCODING_MAX
} DD_COLOR_ENCODING;

typedef enum _DD_COLOR_GAMUT
{
    DD_COLOR_GAMUT_UNKNOWN = 0,
    DD_COLOR_GAMUT_601 = 1,
    DD_COLOR_GAMUT_709 = 2,
    DD_COLOR_GAMUT_2020 = 3,
    DD_COLOR_GAMUT_DCIP3 = 4,
    DD_COLOR_GAMUT_CUSTOM = 5,
    DD_COLOR_GAMUT_MAX
} DD_COLOR_GAMUT;

typedef struct _DD_COLOR_CHROMATICITY
{
    DDU32 CIE_xWhite;
    DDU32 CIE_yWhite;
    DDU32 CIE_xRed;
    DDU32 CIE_yRed;
    DDU32 CIE_xGreen;
    DDU32 CIE_yGreen;
    DDU32 CIE_xBlue;
    DDU32 CIE_yBlue;
} DD_COLOR_CHROMATICITY;

typedef enum _DD_MPO_HDR_METADATA_TYPE
{
    DD_MPO_HDR_METADATA_TYPE_NONE = 0,
    DD_MPO_HDR_METADATA_TYPE_HDR10 = 1,
    DD_MPO_HDR_METADATA_TYPE_DEFAULT = 2,
    DD_MPO_HDR_METADATA_TYPE_UMD_ESCAPE = 3
} DD_MPO_HDR_METADATA_TYPE;

typedef struct _DD_COLOR_OPTICAL_DESC
{
    DDU32 MinDisplayLuminanceInMilliNits;
    DDU32 MaxDisplayLuminanceInMilliNits;
    DDU32 MaxFALLInMilliNits;
    DDU32 MaxCLLInMilliNits;
    DD_MPO_HDR_METADATA_TYPE HdrMetadataType; // Indicates HDR metada type
} DD_COLOR_OPTICAL_DESC;

typedef struct _DD_POINT
{
    DDU32 X;
    DDU32 Y;
} DD_POINT;

typedef struct _DD_TONE_MAPPING_PARAMS
{
    DDU32 MinOutputLuminanceInMilliNits;
    DDU32 MaxOutputLuminanceInMilliNits;
    DDU32 MinInputLuminanceInMilliNits;
    DDU32 MaxInputLuminanceInMilliNits;
    DDU32 LutSize; // Generated Output Lut Size
} DD_TONE_MAPPING_PARAMS;

typedef enum _DD_CONTENT_TYPE
{
    DD_CONTENT_TYPE_INVALID = 0,
    DD_CONTENT_TYPE_SDR = 1,
    DD_CONTENT_TYPE_WCG = 2,
    DD_CONTENT_TYPE_HDR = 3
} DD_CONTENT_TYPE;

// If DDU8 bpp is packed as DDU32, later memcmp of PlaneSurf results in
// unreliable results, hence changing to DDU8 pack to avoid garbage.
// push current alignment to stack and set alignment to 1 byte boundary
#pragma pack(push, 1)
typedef struct _DD_COLOR_PIXEL_DESC
{
    DD_COLOR_MODEL ColorModel;
    DD_COLOR_RANGE_TYPE ColorRangeType;
    DD_COLOR_ENCODING Encoding;
    DD_COLOR_GAMUT ColorGamut;
    DD_COLOR_OPTICAL_DESC OpticalDesc;
    DDU8 BitsPerColor;
    DD_COLOR_YCBCR_SUBSAMPLING YCbCrSubSampling;
    DD_COLOR_CHROMATICITY Chromaticity;
    DDU32 SdrContentLuminance;
    DD_CONTENT_TYPE ContentType;
} DD_COLOR_PIXEL_DESC;

typedef struct _DD_DSM_OUTPUT_PARAM
{
    DDU32 Value;
    DDU8 ExitResult;
} DD_DSM_OUTPUT_PARAM;

// DDU8 AggressivenessPercent is packed as DDU32 in custom based reg key,
// hence changing to DDU8 pack to avoid garbage.

typedef struct _DD_LUX_AGGR_MAP_ENTRY
{
    DDU32 Lux;                  //  Ambient lux
    DDU8 AggressivenessPercent; //  Pixel boost Aggressiveness Percent
} DD_LUX_AGGR_MAP_ENTRY;

#pragma pack(pop) // restore original alignment from stack

typedef union _TIMING_FEATURE_SUPPORT
{
    DDU8 Value;
    struct
    {
        // Mandatory for LNL (for full FlipQ).
        // Optional for MTL.In MTL, if FlipQ W2 requirement is not meeting, Dc6v will not be supported (for VRR panels) with this mode.
        DDU8 FlipQInWindow2 : DD_BITFIELD_BIT(0);
        DDU8 Psr2 : DD_BITFIELD_BIT(1);
        DDU8 PanelReplay : DD_BITFIELD_BIT(2);
        DDU8 FirstPlaneScaler : DD_BITFIELD_BIT(1);
        DDU8 SecondPlaneScaler : DD_BITFIELD_BIT(1);
        DDU8 UNIQUENAME(Reserved) : DD_BITFIELD_RANGE(5, 7);
    };
} TIMING_FEATURE_SUPPORT;

//  TIMING_INFO data structure (previous in aim3.h)
typedef struct _DD_TIMING_INFO
{
    DDU64 DotClockInHz;    // Pixel clock in Hz
    DDU32 HTotal;          // Horizontal total in pixels
    DDU32 HActive;         // Active in pixels
    DDU32 HBlankStart;     // From start of active in pixels
    DDU32 HBlankEnd;       // From start of active in pixels
    DDU32 HSyncStart;      // From start of active in pixels
    DDU32 HSyncEnd;        // From start of active in pixels
    DDU32 HRefresh;        // Refresh Rate
    DDU32 VTotal;          // Vertical total in lines
    DDU32 VActive;         // Active lines
    DDU32 VBlankStart;     // From start of active lines
    DDU32 VBlankEnd;       // From start of active lines
    DDU32 VSyncStart;      // From start of active lines
    DDU32 VSyncEnd;        // From start of active lines
    DDU32 VRoundedRR;      // Refresh Rate
    DD_BOOL IsInterlaced;  // 1 = Interlaced Mode
    DD_BOOL HSyncPolarity; // 1 = H. Sync Polarity is Negative going pulse
    DD_BOOL VSyncPolarity; // 1 = V. Sync Polarity is Negative going pulse
    DD_CE_DATA CeData;
    DD_TIMING_FLAGS Flags; // Timing Flags
    DD_MODE_TYPE ModeType;
    MODE_SOURCE ModeSource;
    DD_SIGNAL_STANDARD SignalStandard; // Signal standard
    DDU32 S3DFormatMask;               // Mask of DD_S3D_FORMAT
    DDU8 FvaFactorM1;                  // (FVA Factor - 1) 0 - non-FVA mode, 1-16 : Represents a FVA factor multipled DSC mode, 1 - FVA factor = 2, 2 - FVA factor = 3 and so on...
    DD_TAREGT_MODE_ID ModeId;          // Keep it at bottom as upper struct maps to pre defined timings values.
    // DDU32 LineTimeInus;  @todo: VSDI-22881 - Add LineTime variable into TImingInfo structure and remove from all other places
    DDU32 VSyncMinRr1000;         // Set the minimum of all the RR available for a target timing
    DD_BOOL IsVirtualRRSupported; // 1 = If the target timing is capable of supporting this feature.
    DDU32 HBlankDiff;
    DDU64 OrigDotClockInHz;
} DD_TIMING_INFO;

typedef enum _DD_VERSION_TYPE
{
    DD_VBIOS_VERSION = 0,
    DD_GOP_VERSION
} DD_VERSION_TYPE;

// Get ROM BIOS info args structure
typedef struct _DD_GETROMBIOSINFO_ARGS
{
    DD_OUT DDU8 BuildNum[32]; // Build number for BIOS
    // VBIOS version will be sent by BIOS as a number(4 Bytes).
    // GOP version will be sent by BIOS as a string.
    // RCR # 1024086
    // Variable added for Minor Version of VBIOS.
    DD_OUT DDU8 MinorVersion;
    // Variable added for indicating that the minor version number is invalid
    DD_OUT DD_BOOL InvalidMinorVersion;
    DD_OUT DD_VERSION_TYPE VersionType;
} DD_GETROMBIOSINFO_ARGS;

typedef struct _DD_PIPE_DETAILS
{
    // Is there a valid PIPE attched to the target
    DD_BOOL IsActive;
    // Valid only when IsAttachedToPipe is TRUE
    PIPE_ID PipeId;
} DD_PIPE_DETAILS;

typedef struct _DD_DISPLAY_LIST
{
    DDU32 Displays;
    DDU32 DisplayUID[MAX_POSSIBLE_PIPES];
    DDU32 DeviceConfig; // device configuration (Usefull for dual display)
    DDU8 ConnectorIndex;
} DD_DISPLAY_LIST;

typedef enum _DD_PLATFORM_TYPE
{
    DD_PLATFORM_NONE = 0x00,
    DD_PLATFORM_DESKTOP = 0x01,
    DD_PLATFORM_MOBILE = 0x02,
    DD_PLATFORM_PERFORMANCE = 0X03,
    DD_PLATFORM_ALL = 0xff, // flag used for applying any feature/WA for All platform types
} DD_PLATFORM_TYPE;

typedef enum _DD_PRODUCT_FAMILY
{
    DD_IGFX_UNKNOWN = 0,
    DD_IGFX_GEMINILAKE = 22,
    DD_IGFX_GLENVIEW,
    DD_IGFX_GOLDWATERLAKE,
    DD_IGFX_CANNONLAKE,
    DD_IGFX_CNX_G,
    DD_IGFX_ICELAKE,
    DD_IGFX_ICELAKE_LP,
    DD_IGFX_TIGERLAKE_LP,
    DD_IGFX_XE_HP_SDV,
    DD_IGFX_COFFEELAKE,
    DD_IGFX_RYEFIELD,
    DD_IGFX_DG1,
    DD_IGFX_ROCKETLAKE,
    DD_IGFX_ALDERLAKE_S,
    DD_IGFX_ALDERLAKE_P,
    DD_IGFX_ALDERLAKE_N,
    DD_IGFX_DG2,
    DD_IGFX_METEORLAKE,
    DD_IGFX_ARROWLAKE,
    DD_IGFX_BMG,
    DD_IGFX_ELG = DD_IGFX_BMG,
    DD_IGFX_LUNARLAKE,
    DD_IGFX_CLS,
    DD_IGFX_PTL,
    DD_IGFX_NVL,
    DD_IGFX_NVL_XE3G,
    DD_IGFX_MAX_PRODUCT,

    DD_IGFX_GENNEXT = 0x7ffffffe,
    DD_PRODUCT_FAMILY_FORCE_DDU32 = 0x7fffffff
} DD_PRODUCT_FAMILY;

typedef struct _DD_SIM_ENV
{
    union
    {
        struct
        {
            DDU8 SimEnv_Fulsim : 1;    // Bit 0 - Fulsim environment
            DDU8 SimEnv_Fulchip : 1;   // Bit 1 - Fulchip environment
            DDU8 SimEnv_Pipe3d : 1;    // Bit 2 - Pipe3d environment
            DDU8 SimEnv_Pipe2d : 1;    // Bit 3 - Pipe2d environment
            DDU8 SimEnv_PipeMedia : 1; // Bit 4 - PipeMedia environment
            DDU8 SimEnv_Net : 1;       // Bit 5 - Net environment
            DDU8 SimEnv_Xen : 1;       // Bit 6 - Xen environment
        };

        DDU8 Value; // Bitfield value
    };
} DD_SIM_ENV;

typedef enum _DD_COLOR_OPERATION
{
    DD_COLOR_OPERATION_GET = 0, // Get currently applied configuration
    DD_COLOR_OPERATION_SET = 1, // Set given configuration
    DD_COLOR_OPERATION_RESTORE_DEFAULT = 2, // Restore driver to default settings
    DD_COLOR_OPERATION_MAX
} DD_COLOR_OPERATION;

// The below structure is used for gamma LUT values.
// Format is 8.24 format. This is doen to avoid Floating point caluclations in driver.
// Gamma cannot be negative hence this is unsigned
// This gives 24 bit precision which is good enough for most of OS like windows(16bpc), Linux(24bpc)
typedef union _DDU8_24
{
    DDU32 Value;
    struct
    {
        DDU32 Fraction : DD_BITFIELD_RANGE(0, 23);
        DDU32 Integer : DD_BITFIELD_RANGE(24, 31);
    };
} DDU8_24;

typedef union _DDU16_48
{
    DDU64 Value;
    struct
    {
        DDU64 Fraction : DD_BITFIELD_RANGE(0, 47);
        DDU64 Integer : DD_BITFIELD_RANGE(48, 63);
    };
} DDU16_48;

typedef union _DDU40_24
{
    DDU64 Value;
    struct
    {
        DDU64 Fraction : DD_BITFIELD_RANGE(0, 23);
        DDU64 Integer : DD_BITFIELD_RANGE(24, 63);
    };
} DDU40_24;

// The below structure is used for CSC values.
// Format is 15.16 format. This is done to avoid Floating point caluclations in driver.
// CSC values can be negative hence this is signed
typedef union _DDI15_16
{
    DDS32 Value;
    struct
    {
        DDU32 Fraction : DD_BITFIELD_RANGE(0, 15);
        DDU32 Integer : DD_BITFIELD_RANGE(16, 30);
        DDU32 Sign : DD_BITFIELD_BIT(31);
    };
} DDI15_16;

typedef struct _DD_RGB_DDU8_24 // Similar to D3DDDI_DXGI_RGB
{
    DDU8_24 Red;   // In U8.24 fixed point format
    DDU8_24 Green; // In U8.24 fixed point format
    DDU8_24 Blue;  // In U8.24 fixed point format
} DD_RGB_DDU8_24;

typedef enum _DD_COLOR_CONFIG_FLAGS
{
    DD_COLOR_CONFIG_FLAG_RELATIVE_TRANSFORM = 0,
    DD_COLOR_CONFIG_FLAG_ABSOLUTE_TRANSFORM
} DD_COLOR_CONFIG_FLAGS;

typedef struct _DD_COLOR_PIPE_1DLUT_PARAMS
{
    DD_COLOR_CONFIG_FLAGS Flags;
    DDU32 NumSamples;
    DD_BOOL Enable;
    DD_RGB_DDU8_24 LUTData[DD_COLOR_1DLUT_MAX_NUM_SAMPLES];
} DD_COLOR_PIPE_1DLUT_PARAMS;

typedef struct _DD_COLOR_PIPE_MATRIX_PARAMS
{
    DD_COLOR_CONFIG_FLAGS Flags;
    DD_BOOL Enable;
    DDI15_16 Coefficients[DD_COLOR_MATRIX_NUM_COEFFICIENTS]; // In 8.24 format. Valid when COLOR_CONFIG_FLAG_CUSTOM_MATRIX is used
    DDI15_16 PreOffsets[3];
    DDI15_16 PostOffsets[3];
} DD_COLOR_PIPE_MATRIX_PARAMS;

typedef enum _DD_GAMMA_CLIENT
{
    DD_GAMMA_CLIENT_OS = 0,
    DD_GAMMA_CLIENT_CUI = 1,
    DD_GAMMA_CLIENT_INTERNAL = 2,
    DD_GAMMA_CLIENT_ABS_INF = 3,
} DD_GAMMA_CLIENT;

typedef enum _DD_COLOR_MATRIX_CLIENT
{
    DD_COLOR_MATRIX_CLIENT_OS = 0,
    DD_COLOR_MATRIX_CLIENT_CUI_NON_LINEAR = 1,
    DD_COLOR_MATRIX_CLIENT_CUI_LINEAR = 2,
    DD_COLOR_MATRIX_CLIENT_INTERNAL = 3, // For Driver persistance
} DD_COLOR_MATRIX_CLIENT;

// This is used for get/set post matrix gamma functionality. Caller must get before set.
// CUI structure is same as Driver Structure here. No conversion is required.
// OS structure needs to be converted to Driver structure before calling HAL.

typedef struct _DD_COLOR_1DLUT_CONFIG
{
    DDU32 Size;     // Size of this structure
    DDU32 TargetId; // TagetId when communicating with user mode. Pipe id inside KMD.
    DDU32 NumPipes;
    DDU32 PipeId[MAX_TILES_SUPPORTED];
    DDU32 Reserved[2]; // For future use
    DD_COLOR_OPERATION Operation;
    DD_COLOR_PIPE_1DLUT_PARAMS PipeLUTParams;
} DD_COLOR_1DLUT_CONFIG;

typedef struct _DD_COLOR_1DLUT_CONFIG_ARGS
{
    DD_COLOR_1DLUT_CONFIG ColorParams;
    DD_GAMMA_CLIENT Client;
} DD_COLOR_1DLUT_CONFIG_ARGS;

// This is used for relative/absolute matrix or gamut mapping. Caller must get before set.
typedef struct _DD_COLOR_MATRIX_CONFIG
{
    DDU32 Size;     // Size of this structure
    DDU32 TargetId; // TagetId when communicating with user mode.
    DDU32 NumPipes;
    DDU32 PipeId[MAX_TILES_SUPPORTED]; // Pipe id inside KMD.
    DDU32 Reserved[2];                 // For future use
    DD_COLOR_OPERATION Operation;
    DD_COLOR_PIPE_MATRIX_PARAMS PipeMatrix;
} DD_COLOR_MATRIX_CONFIG;

// Value to be used in info-frame
typedef enum _DD_EOTF_TYPE_INFOFRAME
{
    DD_EOTF_INFORFRAME_TYPE_TRADITIONAL_GAMMA_SDR = 0x0,
    DD_EOTF_INFORFRAME_TYPE_TRADITIONAL_GAMMA_HDR = 0x1,
    DD_EOTF_INFORFRAME_TYPE_ST2084 = 0x2, // This is to be used for HDMI HDR.
    DD_EOTF_INFORFRAME_TYPE_FUTURE_EOTF = 0x3
} DD_EOTF_TYPE_INFOFRAME;

typedef enum _DD_COLOR_PRIMARY_INDEX
{
    DD_COLOR_PRIMARY_GREEN_INDEX = 0,
    DD_COLOR_PRIMARY_BLUE_INDEX = 1,
    DD_COLOR_PRIMARY_RED_INDEX = 2
} DD_COLOR_PRIMARY_INDEX;

typedef struct _DD_HDR_STATIC_METADATA
{
    DDU16 EOTF;
    DDU16 DisplayPrimariesX[3];
    DDU16 DisplayPrimariesY[3];
    DDU16 WhitePointX;
    DDU16 WhitePointY;
    DDU32 MaxDisplayMasteringLuminanceInMilliNits;
    DDU32 MinDisplayMasteringLuminanceInMilliNits;
    DDU32 MaxCLLInMilliNits;
    DDU32 MaxFALLInMilliNits; // Maximum Frame Average Light Level.
} DD_HDR_STATIC_METADATA;

typedef struct _DD_MPO_HDR_METADATA
{
    DD_MPO_HDR_METADATA_TYPE Type;
    DD_HDR_STATIC_METADATA HdrStaticMetaData;
} DD_MPO_HDR_METADATA;

typedef struct _DD_HDR_SURFACE_DESC
{
    DD_HDR_STATIC_METADATA HdrMetaData;
    DD_COLOR_GAMUT ColorGamut;
} DD_HDR_SURFACE_DESC;

typedef enum _DD_ESC_HDR_CAPABILITY_FLAGS
{
    DD_ESC_HDR_CAP_ERROR_SYSTEM_INCAPABLE = 0, // Display HW on this platform is incapable of transmitting HDR signals
    DD_ESC_HDR_CAP_ERROR_MULTI_MON_ACTIVE = 1, // Multi-monitor mode is active
    DD_ESC_HDR_CAP_ERROR_NONE_HDR_PORT = 2, // Although this platform can support HDR signals, it cannot do so on the currently
    // active port (display connector being driving by the graphics HW)
    DD_ESC_HDR_CAP_ERROR_NO_HDR_DISPLAY = 3, // Although the currently active port can support HDR-out, the currently attached
    // monitor does not support HDR signals
    DD_ESC_HDR_CAP_SUPPORTED = 4, // Current port and attached display support HDR signals
} DD_ESC_HDR_CAPABILITY_FLAGS;

typedef enum DD_COLOR_CHANNEL
{
    DD_COLOR_CHANNEL_RED = 1,
    DD_COLOR_CHANNEL_GREEN = 2,
    DD_COLOR_CHANNEL_BLUE = 4,
    DD_COLOR_CHANNEL_ALL = (DD_COLOR_CHANNEL_RED | DD_COLOR_CHANNEL_GREEN | DD_COLOR_CHANNEL_BLUE)
} DD_COLOR_CHANNEL;

// 3d lut Color related : The Hardware Expects the Data to be in BGR format (B:0-9 , G:10-19 , R:20-29)
typedef union _DD_BGR_1010102
{
    DDU32 Value;
    struct
    {
        DDU32 Blue : DD_BITFIELD_RANGE(0, 9);
        DDU32 Green : DD_BITFIELD_RANGE(10, 19);
        DDU32 Red : DD_BITFIELD_RANGE(20, 29);
        DDU32 Reserved : DD_BITFIELD_RANGE(30, 31);
    };
} DD_BGR_1010102;

// This is used for pipe 3D LUT functionality.
typedef struct _DD_COLOR_3DLUT_CONFIG
{
    DDU32 Size;     // Size of this structure
    DDU32 TargetId; // TagetId when communicating with user mode.
    DDU32 NumPipes;
    DDU32 PipeId; // Pipe id is used only by KMD.
    DD_BOOL Enable;
    DD_COLOR_CONFIG_FLAGS Flags;
    DD_COLOR_OPERATION Operation;
    DD_BGR_1010102 LutData[DD_COLOR_3DLUT_NUM_SAMPLES];
    DD_BOOL Psr2PrSfSupported;
} DD_COLOR_3DLUT_CONFIG;

// S3D related definitions (Moved from S3D.h)
#define DD_S3D_FORMAT_MASK(eFormat) (1 << eFormat)

typedef enum _DD_S3D_FORMAT
{
    // As per HDMI 1.4 spec
    S3D_FRAME_PACKING = 0,
    S3D_FIELD_ALTERNATIVE,
    S3D_LINE_ALTERNATIVE,
    S3D_SIDE_BY_SIDE_FULL,
    S3D_LDEPTH,
    S3D_LDEPTH_GRAPHICS_DEPTH,
    S3D_TOP_BOTTOM,
    S3D_SIDE_BY_SIDE_HALF = 8,

    // For 120Hz page flipping
    S3D_PAGE_FLIPPING,
    // additional details for Side By Side Half S3D Format
    S3D_SIDE_BY_SIDE_HALF_HOR_QUINCUNX_ALL4_SUBSAMPLING,
    S3D_SIDE_BY_SIDE_HALF_HOR_SUBSAMPLING,
    S3D_SIDE_BY_SIDE_HALF_QUINCUNX_ALL4_SUBSAMPLING,
    S3D_SIDE_BY_SIDE_HALF_QUINCUNX_0_SUBSAMPLING,
    S3D_SIDE_BY_SIDE_HALF_QUINCUNX_1_SUBSAMPLING,
    S3D_SIDE_BY_SIDE_HALF_QUINCUNX_2_SUBSAMPLING,
    S3D_SIDE_BY_SIDE_HALF_QUINCUNX_3_SUBSAMPLING,

    S3D_NON_S3D = 31 // should be the last entry
} DD_S3D_FORMAT;

typedef union _DD_DISPLAY_FEATURE_CONTROL_FLAGS
{
    DDU32 Value;
    struct
    {
        DDU32 YTilingEnabledFlag : 1;                  // Bit 0
        DDU32 MpoNv12EnabledFlag : 1;                  // Bit 1
        DDU32 RendCompEnabledFlag : 1;                 // Bit 2
        DDU32 GtTypeFusedEnabledFlag : 1;              // Bit 3
        DDU32 UNIQUENAME(Reserved) : 1;                // Bit 4
        DDU32 MpoEnabledFlag : 1;                      // Bit 5
        DDU32 MpoInMultiDisplayEnabledFlag : 1;        // Bit 6
        DDU32 MpoYUY2EnabledFlag : 1;                  // Bit 7
        DDU32 MpoYUY2ScalingEnabled : 1;               // Bit 8
        DDU32 HdrFP16ScanoutSupport : 1;               // Bit 9
        DDU32 DisableDelayedDpDetectionPostResume : 1; // Bit 10
        DDU32 UNIQUENAME(Reserved) : 2;                // Bit 11-12
        DDU32 DisableDramBwCheck : 1;                  // Bit 13
        DDU32 DisableSmoothSync : 1;                   // Bit 14
        DDU32 DisableTile4Memory : 1;                  // Bit 15
        DDU32 DisableNNScaling : 1;                    // Bit 16
        DDU32 DisplayEngineToneMappingEnabledFlag : 1; // Bit 17
        DDU32 DisableHrr : 1;                          // Bit 18
        DDU32 DisableDpYuv420EncodingSupport : 1;      // Bit 19
        DDU32 Enable2XWMLatencies : 1;                 // Bit 20
        DDU32 DisableMinDbufForAsyncFlips : 1;         // Bit 21
        DDU32 DisableGammaRegisterWritesUsingDSB : 1;  // Bit 22
        DDU32 DisableAdaptiveSyncSDP : 1;              // Bit 23
        DDU32 DisableEnhancedUnderrunRecovery : 1;     // Bit 24
        DDU32 UNIQUENAME(Reserved) : 1;                // Bit 25
        DDU32 MultiPipeMPOSupportDisableFlag : 1;      // Bit 26
        DDU32 DitheringEnable : 1;                     // Bit 27
        DDU32 EnableMultichipGenlock : 1;              // Bit 28
        DDU32 EnableLinearScalingSupport : 1;          // Bit 29
        DDU32 DisableOverrideEncodingBpc : 1;          // Bit 30
        DDU32 DisableSpeedFrame : 1;                   // Bit 31
    };
} DD_DISPLAY_FEATURE_CONTROL_FLAGS;

C_ASSERT(sizeof(DD_DISPLAY_FEATURE_CONTROL_FLAGS) == sizeof(DDU32));

typedef union _DD_DISPLAY_FEATURE_CONTROL_FLAGS_2
{
    DDU32 Value;
    struct
    {
        DDU32 OsFlipQueueEnableFlag : DD_BITFIELD_BIT(0);
        DDU32 DFTFlipQueueEnableFlag : DD_BITFIELD_BIT(1);
        DDU32 DisableDmcFlipQueue : DD_BITFIELD_BIT(2);
        DDU32 EnableOsUnawareFlipQ : DD_BITFIELD_BIT(3);
        DDU32 EnableVscSDPChainingSupport : DD_BITFIELD_BIT(4);
        DDU32 EnableCenteredScalingSupport : DD_BITFIELD_BIT(5);
        DDU32 DisableDisplayShiftSupport : DD_BITFIELD_BIT(6);
        DDU32 EnableLayerReordering : DD_BITFIELD_BIT(7);
        DDU32 DisableBoostedFrameRate : DD_BITFIELD_BIT(8); // Bit to disable Boosted Frame Rate a.k.a Dynamic Refresh Rate a.k.a Virtual Refresh Rate
        DDU32 DisableHwDarkScreenDetection : DD_BITFIELD_BIT(9);
        DDU32 DisableFlipQVbiOptimization : DD_BITFIELD_BIT(10);
        DDU32 DisbleCASO : DD_BITFIELD_BIT(11);
        DDU32 DisablePipeDmcFlipQInterrupt : DD_BITFIELD_BIT(12);
        DDU32 EnableWakelockSupport : DD_BITFIELD_BIT(13);
        DDU32 DisableAllmSupport : DD_BITFIELD_BIT(14);
        DDU32 FtrHdr10PlusDisable : DD_BITFIELD_BIT(15);
        DDU32 EnableXeFGPacing : DD_BITFIELD_BIT(16);
        DDU32 DisableWcgForLfps : DD_BITFIELD_BIT(17);
        DDU32 DisableP010Support : DD_BITFIELD_BIT(18);
        DDU32 UNIQUENAME(Reserved) : DD_BITFIELD_RANGE(19, 31);
    };
} DD_DISPLAY_FEATURE_CONTROL_FLAGS_2;

C_ASSERT(sizeof(DD_DISPLAY_FEATURE_CONTROL_FLAGS_2) == sizeof(DDU32));

typedef union _DD_DIRECTFLIP_EDIDCD_CONTROL_FLAG
{
    DDU32 Value;
    struct
    {
        DDU32 EnableDirectFlipEdidMgmt : DD_BITFIELD_BIT(0);
        DDU32 EnableDirectFlipCombinedDisplay : DD_BITFIELD_BIT(1);
    };
} DD_DIRECTFLIP_EDIDCD_CONTROL_FLAG;

C_ASSERT(sizeof(DD_DIRECTFLIP_EDIDCD_CONTROL_FLAG) == sizeof(DDU32));

typedef union _DD_DISPLAY_SOFTWARE_WA_CONTROL_FLAG
{
    DDU32 Value;
    struct
    {
        DDU32 WaOsRestrictMaxQueuedMultiPlaneOverlayFlipVSync64 : DD_BITFIELD_BIT(0); // Default value will be 0 , To restrict the max flips per vbi to 1 SET this Bit
        DDU32 WaDisableDelayedVblankProgramming : DD_BITFIELD_BIT(1);                 // Default value will be 0, to disable delayed vblank, set it to 1
        DDU32 WaDisableHdmiFrlIncreasedHblank : DD_BITFIELD_BIT(2);                   // Enabled by default. To disable set bit to 1.
        // If enabled add TbOverhead to TbBlank or HCBlank to allow increased BW during FRL capacity calculation. (Workaround Method 2: Add Overhead to Hblank).
        // If disabled driver would add TbOverhead to AudioMin Calculation and prune modes exceeding the limit.(Workaround Method 1: Add Overhead to Hblank)
        DDU32 UNIQUENAME(Reserved) : DD_BITFIELD_RANGE(3, 31); // Can be used in future for WAs
    };
} DD_DISPLAY_SOFTWARE_WA_CONTROL_FLAG;

C_ASSERT(sizeof(DD_DISPLAY_SOFTWARE_WA_CONTROL_FLAG) == sizeof(DDU32));

typedef union _DD_FORCE_DEBUG_BREAK_CONTROL_FLAGS
{
    DDU32 Value;
    struct
    {
        DDU32 DbgBreakOnVsyncTimeout : DD_BITFIELD_BIT(0); // To force debug break on 0x117 VsyncTimeout TDRs
        DDU32 UNIQUENAME(Reserved) : DD_BITFIELD_RANGE(1, 31);
    };
} DD_FORCE_DEBUG_BREAK_CONTROL_FLAGS;

C_ASSERT(sizeof(DD_FORCE_DEBUG_BREAK_CONTROL_FLAGS) == sizeof(DDU32));

typedef union _DD_MONITOR_FLICKER_PROFILE_CONFIG
{
    DDU32 Value[4];
    struct
    {
        DDU32 MinRrInHz1000;
        DDU32 MaxRrInHz1000;
        DDU32 MaxFrameTimeIncreaseInUs;
        DDU32 MaxFrameTimeDecreaseInUs;
    };
} DD_MONITOR_FLICKER_PROFILE_CONFIG;

// Cursor related constants
#define CURSOR_MAX_WIDTH_256_PIXEL (256)
#define CURSOR_MAX_HEIGHT_256_PIXEL (256)

// ----------------------------------------------------------------------------
//
// Generic CONSTANTS, ENUMS and MACROS - END
//
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//
// OPM/UMD Escape used structures -- START
//
// ----------------------------------------------------------------------------

typedef struct _DD_GETDISPLAYINFO_ARGS
{
    DD_IN DDU32 Size; // Set to sizeof(DD_GETDISPLAYINFO_ARGS)

    DD_IN DDU32 DisplayUID; // Unique ID or Generic UID e.g DISPLAY_ANY_CRT

    DD_OUT DD_BOOL IsTPVDrivenEncoder; // Introduced for CUI..

    // Display device status information
    // DD_OUT DISPLAY_DEVICE_STATUS stDisplayDeviceStatus;

    // Device capability info
    // DD_OUT DISPLAY_DEVICE_CAPS stDisplayDeviceCaps;

    // Display Device BW related information
    // DD_OUT DD_DISPLAY_BANDWIDTH_INFO stDisplayBandWidthInfo;

    // Tiled display info
    DD_OUT DDU32 TileXRes;
    DD_OUT DDU32 TileYRes;
    DD_OUT DDU8 NoOfHTileDisplays;
    DD_OUT DDU8 NoOfVTileDisplays;

    // Flag Specifying the return of Mode Caps
    // Only GRM will be using this flag in the CSLBASE_GetDisplayInfo ().
    // This will be set by GRM to get the Mode Caps for non-CRT devices.
    // For CRT, GRM will still be getting the Display Caps
    DD_IN DD_BOOL ModeCaps;

    // DD_OUT ENCODER_RR_SWITCHING_CAPS stEncoderRrSwitchingCaps;   // Indicates the refresh-rate switching capabilities of the chipset

    DD_OUT DD_BOOL IsLsPconDonglePresent;

    DD_OUT DD_BOOL IsHDRSupported; // Sink supports HDR SMPTE 2084 EOTF
} DD_GETDISPLAYINFO_ARGS;

// Get Set parameters args structure
typedef struct _DD_GETSETPARAM_ARGS
{
    DD_IN DDU32 DisplayUID; // Unique display ID (cannot accept generic types)
    DD_IN_OUT union
    {
        HDCP_PARAMETERS HdcpParameters;
        CP_PARAMETERS_T CPParameters;
    };
} DD_GETSETPARAM_ARGS;

typedef enum _DISPLAY_SWITCH_METHOD
{
    DISPLAY_SWITCH_NO_SUPPORT = 0,
    DISPLAY_SWITCH_I_TO_I = 1,
    DISPLAY_SWITCH_I_TO_O = 2,
    DISPLAY_SWITCH_OS_MDM = 3,
} DISPLAY_SWITCH_METHOD;

// internally we support only switching from I to I & OS MDM Method.
// for I to O, we dont do anything for switching hence that is not included in this list
#define IS_DRIVER_SUPPORTED_SWITCH(SwitchMethod) ((SwitchMethod == DISPLAY_SWITCH_I_TO_I) || (SwitchMethod == DISPLAY_SWITCH_OS_MDM))

typedef struct _DD_PLATFORM_INFO
{
    DDU8 GopVersion[0x20]; // GOP version
    DD_BOOL IsS0ixCapable;
    DD_BOOL IsDgpuPresent;
    DDU8 MaxSupportedPipes;
    DD_BOOL IsVramSrSupport;
    DISPLAY_SWITCH_METHOD DisplaySwitchMethod;
} DD_PLATFORM_INFO;

typedef struct _DD_DISPLAY_INIT_ARG
{
    DD_IN DD_BOOL DodEnabled;
    DD_OUT DDU32 NumberOfVideoPresentSources;
    DD_OUT DDU32 NumberOfChildren;
    DD_OUT DD_PLATFORM_INFO PlatformInfo;
    DD_OUT DDU8 NumLfpDisplays;
} DD_DISPLAY_INIT_ARG;

// ----------------------------------------------------------------------------
//
// OPM/UMDEscape used structures -- END
//
// ----------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------
// Gaming Sync Mode Escape Args
//------------------------------------------------------------------------------------------------------

typedef enum _DD_GAMING_SYNC_MODE
{
    DD_GAMING_SYNC_MODE_INVALID = 0,         // Invalid flag
    DD_GAMING_SYNC_MODE_APPLICATION_DEFAULT, // Turn off all gaming sync mode features
    DD_GAMING_SYNC_MODE_VSYNC_OFF,           // Convert all sync flips to async on the next possible scanline.
    DD_GAMING_SYNC_MODE_VSYNC_ON,            // Convert all async flips to sync flips
    DD_GAMING_SYNC_MODE_SMOOTH_SYNC,         // Reduce tearing effect with async flips
    DD_GAMING_SYNC_MODE_SPEED_FRAME,         // Application unaware triple buffering
    DD_GAMING_SYNC_MODE_CAPPED_FPS,          // Limit the game FPS to panel RR.
} DD_GAMING_SYNC_MODE;

typedef union _DD_GAMING_SYNC_MODE_FEATURE_MASK
{
    struct
    {
        DDU32 VsyncOff : DD_BITFIELD_BIT(0);
        DDU32 VsyncOn : DD_BITFIELD_BIT(1);
        DDU32 SmoothSync : DD_BITFIELD_BIT(3);
        DDU32 SpeedFrame : DD_BITFIELD_BIT(2);
        DDU32 CappedFps : DD_BITFIELD_BIT(4);
        DDU32 Reserved : DD_BITFIELD_RANGE(5, 31);
    };
    DDU32 Value;
} DD_GAMING_SYNC_MODE_FEATURE_MASK;

typedef struct _DD_GAMING_SYNC_MODE_DATA
{
    DD_GAMING_SYNC_MODE DriverSyncMode;
    DD_GAMING_SYNC_MODE_FEATURE_MASK FeatureState;
} DD_GAMING_SYNC_MODE_DATA;

typedef enum _DD_PROCESS_CONFIG_TABLE_ENTRY_ACTION
{
    DD_PROCESS_ENTRY_ACTION_UNINITIALIZED,
    DD_PROCESS_ENTRY_ACTION_ADD,
    DD_PROCESS_ENTRY_ACTION_REMOVE,
    DD_PROCESS_ENTRY_ACTION_UPDATE_FLIP_SUBMISSION,
    DD_PROCESS_ENTRY_ACTION_UPDATE_SYNC_MODE,
    DD_PROCESS_ENTRY_ACTION_NO_ACTION,
    DD_PROCESS_ENTRY_ACTION_READ, // Exclusive for ETL Logging
    DD_PROCESS_ENTRY_ACTION_MAX,
} DD_PROCESS_CONFIG_TABLE_ENTRY_ACTION;

typedef struct _DD_GET_SET_GAMING_SYNC_MODES
{
    DD_BOOL Set; // TRUE = SET, FALSE = GET
    DDU32 ProcessId;
    DDWSTR ProcessName[DD_MAX_PROCESS_NAME_CHAR];
    DD_GAMING_SYNC_MODE SyncMode;                         // SET - Gaming sync mode type request, GET - Currently Enabled Gaming Sync mode
    DD_GAMING_SYNC_MODE_FEATURE_MASK SyncModeSupportMask; // Returns mask of all supported gaming sync modes - used only for GET
    DD_PROCESS_CONFIG_TABLE_ENTRY_ACTION EntryAction;
    DD_BOOL EnableVrrBlt;
} DD_GET_SET_GAMING_SYNC_MODES;

//------------------------------------------------------------------------------------------------------
// Smooth Sync Escape Args
//------------------------------------------------------------------------------------------------------
typedef struct _DD_SMOOTH_SYNC_GET_SET_ARGS
{
    DD_BOOL Set;       // TRUE = SET, FALSE = GET
    DD_BOOL Supported; // Whether Smooth sync is Supported - used only for GET
    DD_BOOL Enable;    // Enable/Disable Smooth sync
} DD_SMOOTH_SYNC_GET_SET_ARGS;

//------------------------------------------------------------------------------------------------------
// Endurance Gaming control types
//------------------------------------------------------------------------------------------------------
typedef enum _DD_ENDURANCE_GAMING_CONTROL
{
    DD_ENDURANCE_GAMING_CONTROL_OFF,
    DD_ENDURANCE_GAMING_CONTROL_ON,
    DD_ENDURANCE_GAMING_CONTROL_AUTO,
    DD_ENDURANCE_GAMING_CONTROL_MAX
} DD_ENDURANCE_GAMING_CONTROL;

//------------------------------------------------------------------------------------------------------
// Endurance Gaming mode types
//------------------------------------------------------------------------------------------------------
typedef enum _DD_ENDURANCE_GAMING_MODE
{
    DD_ENDURANCE_GAMING_MODE_BETTER_PERFORMANCE,
    DD_ENDURANCE_GAMING_MODE_BALANCED,
    DD_ENDURANCE_GAMING_MODE_MAXIMUM_BATTERY,
    DD_ENDURANCE_GAMING_MODE_MAX
} DD_ENDURANCE_GAMING_MODE;

//------------------------------------------------------------------------------------------------------
// Endurance Gaming Escape Args
//------------------------------------------------------------------------------------------------------
typedef struct _DD_ENDURANCE_GAMING_GET_SET_ARGS
{
    DD_BOOL Set;                         // TRUE = SET, FALSE = GET
    DD_BOOL Supported;                   // Whether Endurance Gaming is Supported - used only for GET
    DD_ENDURANCE_GAMING_CONTROL Control; // Enable/Disable Endurance Gaming
    DD_ENDURANCE_GAMING_MODE EGMode;     // EG mode
} DD_ENDURANCE_GAMING_GET_SET_ARGS;

// ----------------------------------------------------------------------------
//
// Header with topology related  definitions - START
//
// ----------------------------------------------------------------------------

#define DD_SOURCE_ID_UNINITIALIZED (DDU32) ~0
#define DD_TARGET_ID_UNINITIALIZED (DDU32) ~0

// map of legacy SOURCE_MODE_FLAGS
typedef union _DD_SOURCE_MODE_FLAGS
{
    DDU32 Flags;
    struct
    {
        unsigned S3DMode : 1;            // Bit Indicating whether the given mode is S3D
        unsigned Collage : 1;            // Bit indicating if given mode is Collage
        unsigned HorCollage : 1;         // bit indicating if given mode is horizontal Collage
        unsigned VerCollage : 1;         // bit indicating if given mode id vertical Collage
        unsigned RGBColorSeparation : 1; // bit indicating color separation support for the given mode
        unsigned TiledMode : 1;          // bit indicating tiled display mode is applicable
        unsigned Reserved : 26;          // Bits 1:27, Reserved
    };
} DD_SOURCE_MODE_FLAGS;

// DD_COPY_PROTECTION_TYPE - Enum Copy protection
typedef enum _DD_COPY_PROTECTION_TYPE
{
    /**Uninitialized. */
    DD_CP_UNINITIALIZED = 0,
    /**No Protection. */
    DD_CP_NO_PROTECTION = 1,
    /**APS Trigger supported. */
    DD_CP_APS_TRIGGER = 2,
    /**Full protection supported.*/
    DD_CP_FULL_PROTECTION = 3,
} DD_COPY_PROTECTION_TYPE;

// DD_COPY_PROTECTION - Copy Protection
typedef struct _DD_COPY_PROTECTION
{
    /**Copy protection values updated or not. */
    DD_BOOL Updated;
    /**CP Type.*/
    DD_COPY_PROTECTION_TYPE CpType;
    /**Asp Trigger Bits. */
    DDU32 AspTriggerBits;
    // Place holder for OEM CP size !
} DD_COPY_PROTECTION;

// DD_ROTATION_SUPPORT - Rotation support set
typedef union _DD_ROTATION_SUPPORT
{
    struct
    {
        DDU32 Identity : 1;
        DDU32 Rotate90 : 1;
        DDU32 Rotate180 : 1;
        DDU32 Rotate270 : 1;
        DDU32 Offset0 : 1;
        DDU32 Offset90 : 1;
        DDU32 Offset180 : 1;
        DDU32 Offset270 : 1;
        DDU32 Reserved : 24;
    };
    DDU32 RotationSupport;
} DD_ROTATION_SUPPORT;

typedef union _DD_DRIVER_FLAGS
{
    struct
    {
        DDU32 PopulateSourceModeSet : 1;
        DDU32 PopulateTargetModeSet : 1;
        DDU32 IndexBasedTargetModeSetAccess : 1;
        DDU32 PopulateScaling : 1;
        DDU32 PopulateRotation : 1;
        DDU32 TargetModePinned : 1;
        DDU32 SourceModePinned : 1;
        DDU32 CombineBpcEnumeration : 1;
        DDU32 ReservedFlags : 24;
    };
    DDU32 DriverFlags;
} DD_DRIVER_FLAGS;

typedef union _DD_OS_FLAGS
{
    struct
    {
        DDU32 SourceModePinned : 1;
        DDU32 SourcePivot : 1;
        DDU32 TargetModePinned : 1;
        DDU32 TargetPivot : 1;
        DDU32 ScalingPinned : 1;
        DDU32 ScalingPivot : 1;
        DDU32 RotationPinned : 1;
        DDU32 RotationPivot : 1;
        DDU32 ReservedFlags : 24;
    };
    DDU32 OsFlags;
} DD_OS_FLAGS;

typedef enum _DD_SCALING
{
    DD_SCALING_UNINITIALIZED = 0,
    DD_SCALING_IDENTITY = 1,
    DD_SCALING_CENTERED = 2,
    DD_SCALING_STRETCHED = 4,
    DD_SCALING_ASPECTRATIOCENTEREDMAX = 8,
    DD_SCALING_CUSTOM = 16,
    DD_SCALING_FORCED_IS = 32, // Forced Integer Scaling using Nearest Neighbour Filter
    DD_SCALING_UNPINNED = 254,
    DD_SCALING_UNSPECIFIED = 255
} DD_SCALING;

#define IS_NON_SCALED_MODE(Scaling) (((Scaling) == DD_SCALING_IDENTITY) || ((Scaling) == DD_SCALING_CENTERED))

/* typedef union _MACROVISIONCAPS
{
    DDU32   Value;
    struct
    {
        DDU32 APSTriggerSupported : 1;    // bit 0
        DDU32 MVFullsupported : 1;    // bit 1
        DDU32 NoProtection : 1;    // bit 2
        DDU32 Reserved : 29;    // bit 3 - 31
    };
}MACROVISIONCAPS;*/

typedef enum _DD_ROTATION_OFFSET
{
    DD_ROT_OFFSET_0 = 1,
    DD_ROT_OFFSET_90 = 2,
    DD_ROT_OFFSET_180 = 3,
    DD_ROT_OFFSET_270 = 4,
} DD_ROTATION_OFFSET;

typedef enum _DD_SYNC_LOCK_STYLE
{
    DD_SYNC_LOCK_STYLE_NONE = 0,
    DD_SYNC_LOCK_STYLE_IDENTICAL = 1,
    DD_SYNC_LOCK_STYLE_DONT_CARE = 2
} DD_SYNC_LOCK_STYLE;

// enum for values of bInterlacedType
/* aim3 // typedef enum _INTERLACED_TYPE_FLAGS
{
    INTERLACED_LVDS_VSYNC_SHIFT = 0,                    // 0 = Interlaced LVDS using vertical sync shift
    INTERLACED_VSYNC_SHIFT,                             // 1 = Interlaced using vertical sync shift
    INTERLACED_VSYNC_HSYNC_FIELD_LEGACY_VSYNC_SHIFT,    // 2 = Interlaced with VSYNC/HSYNC Field indication
    // using legacy vertical sync shift
    INTERLACED_FIELD0_LEGACY_VSYNC_SHIFT,               // 3 = Odd/Even Mode : DVO-C Even, DVO-B Odd
    INTERLACED_SDVO_VSYNC_SHIFT
} INTERLACED_TYPE_FLAGS;
*/

// Timing Info related definitions - END

typedef enum _PLANE_ORIENTATION DD_ROTATION;

#define DD_IS_PORTRAIT_ORIENTATION(PlaneOrientation) ((PlaneOrientation == ORIENTATION_90) || (PlaneOrientation == ORIENTATION_270))

#define DD_IS_LANDSCAPE_ORIENTATION(PlaneOrientation) ((PlaneOrientation == ORIENTATION_0) || (PlaneOrientation == ORIENTATION_180))

typedef struct _DD_SET_TIMING_PATH_FLAGS
{
    union
    {
        DDU32 Value;
        struct
        {
            DDU32 ProcessPath : 1;
            DDU32 Modified : 1;
            DDU32 Enabled : 1;
            DDU32 IgnoreConnectivity : 1;
            DDU32 PathHasSrcId : 1; // TRUE = Path identified by Source id (RS1), FALSE = Path identified by Target id (RS2 Disabled path)
            DDU32 PreserveBootDisplay : 1;
            DDU32 SyncLockGroup : 3; // This number when non-zero will indicate which other paths with same number are in group of displays to be synced.
            DD_SYNC_LOCK_STYLE SyncLockStyle : 4;
            DDU32 Reserved : 19;
        };
    };
} DD_SET_TIMING_PATH_FLAGS;

typedef struct _DD_DOD_RESOURCE_INFO
{
    DDU8* pDodGmmBlock;
    DDU32 DodGfxHWAddress; /* PHYSICAL_ADDRESS */
    DDU8* pDodLinearFrameBufferAddress;
} DD_DOD_RESOURCE_INFO;

// @todo: VSDI-22885 - Cleanup with legacy defn
typedef union _DD_MACROVISIONCAPS
{
    DDU32 Value;
    struct
    {
        DDU32 APSTriggerSupported : 1; // bit 0
        DDU32 MVFullsupported : 1;     // bit 1
        DDU32 NoProtection : 1;        // bit 2
        DDU32 Reserved : 29;           // bit 3 - 31
    };
} DD_MACROVISIONCAPS;

typedef struct _DD_PATH
{
    DDU32 SourceId;
    DDU32 TargetId;
    DD_SOURCE_MODE_INFO PinnedSourceMode;
    DD_TIMING_INFO PinnedTargetMode;
    DD_SCALING Scaling;
    DD_ROTATION Rotation;
    // Below parameters are used by mode enumeration DxgkDdiEnumVidPnCofuncModality only.
    DD_OS_FLAGS OsFlags;
    DD_DRIVER_FLAGS DriverFlags;
    DDU32 NumSourceModes;
    DD_SOURCE_MODE_INFO* pSourceModeSet;
    DDU32 NumTargetModes;                                             // Original num of modes before Rm Verification
    DDU8* pTgtModeSetIndices;                                         // Original Target mode Indices before Rm Verification
    DDU32 NumFilteredTargetModes;                                     // Filtered num of modes After Rm Verification
    DDU8 FilteredTgtModeIndices[MAX_NUM_TARGET_MODE_PER_SOURCE_MODE]; // Filtered Target mode Indices after Rm Verification
    DD_TIMING_INFO* pTargetModeSet;
    DD_SCALING_SUPPORT ScalingSupport;
    DD_ROTATION_SUPPORT RotationSupport;
    // Below parameters are used by DxgkDdiCommitVidPn only.
    DD_SET_TIMING_PATH_FLAGS SetTimingFlags;
    // Below paramters are used by DxgkDdiSetTimingsFromVidPn
    DD_COLOR_PIXEL_DESC OutputColorFormat;
    DD_BPC_SUPPORTED PinnedBpc;          // Setmode BPC Requested by OS
    DD_SAMPLING_MODE PinnedSamplingMode; // Setmode Wireformat Requested By OS
    // Below parameters are mostly Unused and can be removed at some point.
    DD_ROTATION_OFFSET RotationOffset;
    DD_COPY_PROTECTION CopyProtection;
    DDU32 ImportanceOrdinal;
} DD_PATH;

typedef struct _DD_TOPOLOGY
{
    DDU32 NumPaths;
    DD_PATH Paths[MAX_POSSIBLE_PIPES];
    void* hVidPn;
    DD_BOOL IsIhvClone; // HW clone where the flips needs to be replicated on all paths
} DD_TOPOLOGY;

#define DD_TOPOLOGY_GETNUMPATHS(hTopology) (((DD_TOPOLOGY *)hTopology)->NumPaths)
#define DD_PATH_GETTARGETID(hPath) ((DD_PATH *)hPath)->TargetId

typedef struct _DD_TARGETID_PIPE_MAP
{
    DDU32 TargetId[MAX_POSSIBLE_PIPES];
    PIPE_ID PipeId[MAX_POSSIBLE_PIPES];
    DDU32 NumEntry;
} DD_TARGETID_PIPE_MAP;

typedef struct _DD_PREOS_DISPLAY_INFO
{
    DDU32 PreOSDisplaySize;
    DDU64 PreOSScanAddress;
    DD_BOOL IsGfxAddress;
} DD_PREOS_DISPLAY_INFO;

// ----------------------------------------------------------------------------
//
// Header with topology related  definitions - END
//
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//
// Header required to convert OS DDI parameters into driver defined parameters - START
//
// ----------------------------------------------------------------------------

typedef enum _DD_OS_FEATURE_ID
{
    DD_OS_FTR_HWSCH = 0,
    DD_OS_FTR_HWFLIPQUEUE = 1,
} DD_OS_FEATURE_ID;

typedef enum _DD_OS_FEATURE_LEVEL
{
    DD_OS_FEATURE_LEVEL_ALWAYS_OFF = 0,
    DD_OS_FEATURE_LEVEL_EXPERIMENTAL,
    DD_OS_FEATURE_LEVEL_STABLE,
    DD_OS_FEATURE_LEVEL_ALWAYS_ON,
} DD_OS_FEATURE_LEVEL;

typedef struct _DD_QUERY_FEATURE_SUPPORT_ARGS
{
    DD_OS_FEATURE_ID FeatureId;
    DD_OS_FEATURE_LEVEL DriverSupportLevel;
} DD_QUERY_FEATURE_SUPPORT_ARGS;

typedef enum _DD_FIRMWARE_TYPE
{
    DD_FIRMWARE_TYPE_UNKNOWN,
    DD_FIRMWARE_TYPE_BIOS,
    DD_FIRMWARE_TYPE_EFI,
    DD_FIRMWARE_TYPE_MAX
} DD_FIRMWARE_TYPE;

typedef enum _DD_PATH_UPDATE
{
    DD_PATH_UPDATE_UNMODIFIED = 0,
    DD_PATH_UPDATE_ADDED = 1,
    DD_PATH_UPDATE_MODIFIED = 2,
    DD_PATH_UPDATE_REMOVED = 3,
    DD_PATH_UPDATE_MAX_COUNT = 4,
} DD_PATH_UPDATE;

typedef enum _DD_DISPLAY_LINK_EVENTS
{
    DD_DISP_LINK_CONFIG_UNDEFINED,
    DD_DISP_LINK_CONFIG_SUCCESS,
    DD_DISP_LINK_CONFIG_FAILED,
    DD_DISP_LINK_LOSS,
    DD_DISP_LINK_DISABLED,
    DD_DISP_LINK_CONFIG_STARTED,
    DD_DISP_LINK_CONFIG_SUCCESS_FAKE_DISPLAY,
    DD_DISP_LINK_CONFIG_SUCCESS_PREBOOT_DISPLAY,
    DD_DISP_LINK_CONFIG_PENDING_POST_RESUME // Enable pending post resume
} DD_DISPLAY_LINK_EVENTS;

typedef enum _DD_WAKELOCK_THREAD
{
    DD_WAKELOCK_THREAD_NONE,                // Invalid
    DD_WAKELOCK_THREAD_MODESET,             // Used in modeset
    DD_WAKELOCK_THREAD_WORKITEM,            // Generic work item processing
    DD_WAKELOCK_THREAD_ESCAPE,              // Generic escape processing
    DD_WAKELOCK_THREAD_PC_XPST,             // PC DPST/OPST/GHE
    DD_WAKELOCK_THREAD_FLIPDONE,            // Used in flipdone
    DD_WAKELOCK_THREAD_VSYNC_CONTROL,       // Vsync enable/disable
    DD_WAKELOCK_THREAD_LACE,                // LACE processing
    DD_WAKELOCK_THREAD_DETECTION,           // Display setection
    DD_WAKELOCK_THREAD_POWER_EVENTS_UNLOAD, // SetPowerState, StopDevice, ReleasePostDisplayOwnerShip, BSOD etc.
    DD_WAKELOCK_THREAD_GENERIC,             // Placeholder for generic events or wakelock enable/disable
    DD_WAKELOCK_THREAD_MAX                  // Invalid
} DD_WAKELOCK_THREAD;

typedef enum _DD_WAKELOCK_REQUEST
{
    DD_WAKELOCK_REQUEST_NONE,    // Invalid request
    DD_WAKELOCK_REQUEST_ENABLE,  // Enable wakelock mode of operation. Once enabled, MMIO access required to acquire wakelock
    DD_WAKELOCK_REQUEST_DISABLE, // Disable wakelock mode of operation. Once disabled, MMIO access is always allowed.
    DD_WAKELOCK_REQUEST_ACQUIRE, // Acquire wakelock to access required MMIO.
    DD_WAKELOCK_REQUEST_RELEASE, // Release wakelock post accessing MMIO
} DD_WAKELOCK_REQUEST;

// TODO: Change the macro to check for DD_DISP_LINK_LOSS only?
#define DD_IS_LINK_RETRAINING_NEEDED(TargetLinkState) ((DD_DISP_LINK_CONFIG_FAILED == TargetLinkState) || (DD_DISP_LINK_LOSS == TargetLinkState))
#define DD_IS_LINK_TRAINING_IN_PROGRESS(TargetLinkState) ((DD_DISP_LINK_CONFIG_FAILED == TargetLinkState) || (DD_DISP_LINK_LOSS == TargetLinkState) || (DD_DISP_LINK_CONFIG_STARTED == TargetLinkState))
#define DD_IS_DISPLAY_DISABLED(TargetLinkState) ((DD_DISP_LINK_DISABLED == TargetLinkState) || (DD_DISP_LINK_CONFIG_UNDEFINED == TargetLinkState))
#define DD_IS_DISPLAY_ENABLED(TargetLinkState) \
    ((DD_DISP_LINK_CONFIG_SUCCESS == TargetLinkState) || (DD_DISP_LINK_CONFIG_SUCCESS_FAKE_DISPLAY == TargetLinkState) || (DD_DISP_LINK_CONFIG_SUCCESS_PREBOOT_DISPLAY == TargetLinkState))
#define DD_IS_DISPLAY_INACTIVE(TargetLinkState) (DD_IS_DISPLAY_DISABLED(TargetLinkState) || (DD_DISP_LINK_CONFIG_FAILED == TargetLinkState))
#define DD_IS_DISPLAY_INACTIVE_OR_PENDING_ENABLE_POST_RESUME(TargetLinkState) (DD_IS_DISPLAY_INACTIVE(TargetLinkState) || (DD_DISP_LINK_CONFIG_PENDING_POST_RESUME == TargetLinkState))

// Video output technology (VOT) type
typedef enum _DD_VIDEO_OUTPUT_TECHNOLOGY
{
    DD_VOT_UNKNOWN = 0,
    DD_VOT_VGA,
    DD_VOT_DVI,
    DD_VOT_HDMI,
    DD_VOT_DISPLAYPORT_EXTERNAL,
    DD_VOT_DISPLAYPORT_EMBEDDED,
    DD_VOT_MIPI_DSI,
    DD_VOT_VIRTUAL,
    DD_VOT_WDE,
    DD_VOT_MIRACAST,
    DD_VOT_MAX = DD_VOT_MIRACAST
} DD_VIDEO_OUTPUT_TECHNOLOGY;

typedef struct _DD_DISPLAY_SWITCH_ARGS
{
    IN DDU32 TargetId;
    IN DDU32 BrightnessLevel;
    IN DD_BOOL StartDisplaySwitch;
    IN DD_BOOL MuxSwitchIn;
    OUT DD_BOOL SwPsrEnabled;
} DD_DISPLAY_SWITCH_ARGS;

typedef struct _DD_TARGET_STATE
{
    DDU32 TargetId;
    DD_DISPLAY_LINK_EVENTS LinkState;
    DD_VIDEO_OUTPUT_TECHNOLOGY LinkTargetType;
    DD_BOOL IsDisplayPreserved;
} DD_TARGET_STATE;

typedef struct _DD_SETTIMING_OUT_ARGS
{
    DDU32 NumTargets;
    DD_TARGET_STATE TargetState[MAX_POSSIBLE_PIPES];
} DD_SETTIMING_OUT_ARGS;

typedef struct _DD_SET_TIMING_PATH_INFO
{
    DD_IN DDU32 TargetId;
    union
    {
        struct
        {
            DD_IN DDU32 VidPnPathUpdates : 2;
            DD_IN DDU32 Active : 1;
            DD_IN DDU32 IgnoreConnectivity : 1;
            DD_IN DDU32 PreserveInherited : 1;
            DD_IN DDU32 SyncLockGroup : 3;
            DD_IN DD_SYNC_LOCK_STYLE SyncLockStyle : 4;
            DD_IN DDU32 Reserved : 20;
        } Input;
        DD_IN DDU32 InputFlags;
    };
    DD_OUT DD_DISPLAY_LINK_EVENTS LinkState;
    DD_OUT DD_VIDEO_OUTPUT_TECHNOLOGY LinkTargetType;
    DD_OUT DD_BOOL IsDisplayPreserved;
} DD_SET_TIMING_PATH_INFO;

typedef struct _DD_OS_SET_TIMING_ARG
{
    DD_IN DD_TOPOLOGY* pTopology;
    DD_IN DDU32 PathCount;
    DD_IN DD_SET_TIMING_PATH_INFO* pSettimingPathInfo;
    DD_OUT DD_TARGETID_PIPE_MAP TdrLogNxtTrgtIdPipeMap;
} DD_OS_SET_TIMING_ARG;

typedef struct _DD_OS_COMMITVIDPN_ARG
{
    DD_IN DDU32 SourceID;
    DD_IN DD_BOOL Active;
    DD_IN DD_BOOL IgnoreConnectivity;
    DD_IN DD_TOPOLOGY* pTopology;
    DD_IN DDU64* pPresentId;
    DD_OUT DD_TARGETID_PIPE_MAP TdrLogNxtTrgtIdPipeMap;
} DD_OS_COMMITVIDPN_ARG;

typedef struct _DD_ACTIVE_PATH
{
    DDU32 TargetId;
    DDU32 VisibleScreenX;
    DDU32 VisibleScreenY;
    DD_PIXELFORMAT PixelFormat;
    void* pDodLinearFBAdd;
} DD_ACTIVE_PATH;

typedef struct _DD_ACTIVE_TOPOLOGY
{
    DDU32 ActivePathCount;
    DD_ACTIVE_PATH PathView[DD_MAX_VIEWS];
} DD_ACTIVE_TOPOLOGY;

typedef struct _DD_TARGET_MODE_INFO
{
    DDU32 Width;
    DDU32 Height;
    DDU32 RefreshRate;
} DD_TARGET_MODE_INFO;

typedef struct _DD_COLLAGE_TILE_INFO
{
    DDU32 ChildId;
    DDU8 HTileLocation; // 0 based index
    DDU8 VTileLocation; // 0 based index
    union
    {
        DDU32 TileBezelInformation;
        struct
        {
            DDU8 TopBezelsize;
            DDU8 BottomBezelsize;
            DDU8 RightBezelsize;
            DDU8 LeftBezelsize;
        };
    };
    DD_RECT FbSrc;                  // Rectangle that is a part of the combined desktop framebuffer and rendered to Display[i]
    DD_RECT FbPos;                  // Rectangle in Display[i]'s framebuffer where FbSrc[i] is placed
    DD_ROTATION DisplayOrientation; // Rotation info. Only 0 and 180 degree supported
    DD_TARGET_MODE_INFO TargetMode; // Target mode info
    DDU32 AdapterBdfData;           // BDF of Associated Adapter, used as Adapter Unique Id.
} DD_COLLAGE_TILE_INFO;

typedef struct _TARGET TARGET; // forward declaration
typedef struct _DD_COLLAGE_TOPOLOGY
{
    DD_BOOL IsMcd; // Is MGPU Combined Display
    DDU8 TotalNumberOfHTiles;
    DDU8 TotalNumberOfVTiles;
    DDU8 TotalNumTiles; // used by MCD
    DD_COLLAGE_TILE_INFO CollageChildInfo[MAX_PER_MCD_TARGETS];
    TARGET* pChildTarget[MAX_PER_MCD_TARGETS];
    DDU32 CollageTargetId;
    DDU32 CombinedDesktopWidth;  // Width of desired combined desktop
    DDU32 CombinedDesktopHeight; // Height of desired combined desktop
    DD_PORT_TYPE CollagePort;
    DD_BOOL IsValidCommonTiming;
    DD_TIMING_INFO MaxRRCommonTiming; // Filled by driver, Common Child timing w/o Bezel
} DD_COLLAGE_TOPOLOGY;

typedef struct _DD_PRESENT_DISPLAY_ONLY
{
    DDU32 SourceID;
    DDU64 PresentId;
} DD_PRESENT_DISPLAY_ONLY;

typedef struct _DD_CHECK_SEAMLESS_RR_CHANGE_SUPPORT_ARG
{
    DD_IN DDU32 VidPnSourceId;
    DD_IN DDU32 DesiredPresentDuration;
    DD_OUT DDU32 ClosestSmallerDuration;
    DD_OUT DDU32 ClosestLargerDuration;
} DD_CHECK_SEAMLESS_RR_CHANGE_SUPPORT_ARG;

#define IS_DP_SINK_TYPE(Type) (Type == DD_VOT_DISPLAYPORT_EXTERNAL || Type == DD_VOT_DISPLAYPORT_EMBEDDED)
#define IS_HDMI_DVI_SINK_TYPE(Type) (Type == DD_VOT_HDMI || Type == DD_VOT_DVI)
#define IS_EDP_SINK_TYPE(Type) (Type == DD_VOT_DISPLAYPORT_EMBEDDED)
#define IS_EXT_DP_SINK_TYPE(Type) (Type == DD_VOT_DISPLAYPORT_EXTERNAL)
#define IS_HDMI_SINK_TYPE(Type) (Type == DD_VOT_HDMI)
#define IS_DVI_SINK_TYPE(Type) (Type == DD_VOT_DVI)
#define IS_MIPI_DSI_SINK_TYPE(Type) (Type == DD_VOT_MIPI_DSI)
#define IS_LFP_SINK_TYPE(Type) ((Type == DD_VOT_DISPLAYPORT_EMBEDDED) || (Type == DD_VOT_MIPI_DSI))

typedef enum _DD_CHILD_STATUS_TYPE
{
    DD_STATUS_UNINITIALIZED,
    DD_STATUS_CONNECTION,
    DD_STATUS_ROTATION,
    DD_STATUS_MIRACAST_CONNECTION,
} DD_CHILD_STATUS_TYPE;

typedef enum _DD_TARGET_DETECT_TYPE
{
    DD_TARGET_DETECT_UNINITIALIZED,
    DD_TARGET_ENABLE_HPD,
    DD_TARGET_DISABLE_HPD,
    DD_TARGET_DETECT_ONE,
    DD_TARGET_DETECT_ALL,
    DD_TARGET_DETECT_ALL_ON_PORT
} DD_TARGET_DETECT_TYPE;

typedef enum _DD_TARGET_DETECT_MODE
{
    DD_TARGET_NORMAL_DETECTION,
    DD_TARGET_PARTIAL_DETECTION, // Partial Detection, called from IRQ Hotplug path, short pulse event
    DD_TARGET_HOTPLUG_DETECTION, // Hotplug Detection
} DD_TARGET_DETECT_MODE;

typedef struct _DD_DISPLAY_DETECT_CONTROL_ARGS
{
    DD_IN DDU32 TargetId;
    DD_IN DD_TARGET_DETECT_TYPE DetectType;
    DD_IN DD_BOOL NonDestructiveOnly;
} DD_DISPLAY_DETECT_CONTROL_ARGS;

typedef struct _DD_CHILD_STATUS
{
    DD_CHILD_STATUS_TYPE Type;
    DDU32 ChildUid;
    union
    {
        struct
        {
            DD_BOOL Connected;
        } HotPlug;
        struct
        {
            DDU8 Angle;
        } Rotation;
        struct
        {
            DD_BOOL Connected;
            DD_VIDEO_OUTPUT_TECHNOLOGY MiracastMonitorType;
        } Miracast;
    };
} DD_CHILD_STATUS;

typedef struct _DD_ARG_QCS
{
    DD_IN DD_CHILD_STATUS ChildStatus;
    DD_IN DD_BOOL NonDestructiveOnly;
} DD_ARG_QCS;

typedef struct _DD_DEVICE_DESCRIPTOR
{
    DDU32 DescriptorOffset;
    DDU32 DescriptorLength;
    void* pDescriptorBuffer;
    DD_BOOL IsDisplayIDRequest;   // 0 for EDID Request , 1 for DisplayID Request
    DDU8 NumEdidExtensionsParsed; // Number of extenblock successfully parsed
} DD_DEVICE_DESCRIPTOR;

typedef struct _DD_ARG_QDD
{
    DD_IN DDU32 ChildUid;
    DD_IN_OUT DD_DEVICE_DESCRIPTOR DeviceDescriptor;
} DD_ARG_QDD;

typedef struct _DD_ARG_QIDC
{
    DD_IN DDU32 ChildUid;
    // To query integrated display's colorimetry information
    // Color Primaries
    DD_OUT DDU32 RedX;
    DD_OUT DDU32 RedY;
    DD_OUT DDU32 GreenX;
    DD_OUT DDU32 GreenY;
    DD_OUT DDU32 BlueX;
    DD_OUT DDU32 BlueY;
    DD_OUT DDU32 WhiteX;
    DD_OUT DDU32 WhiteY;

    // Luminance Data
    DD_OUT DDU16 MinLuminance;
    DD_OUT DDU16 MaxLuminance;
    DD_OUT DDU16 MaxFullFrameLuminance;

    // To fill the color bit depth override and colorimetry flags, if any
    DD_OUT DD_BPC_SUPPORTED FormatBitDepth;
    DD_OUT DDU32 ColorimetryFlags;
    DD_OUT DD_BOOL IsOverrideAvailable;
} DD_ARG_QIDC;

typedef struct __DD_HDR10_PLUS_ARGS
{
    DD_IN DDU32 ChildUid;

    DD_OUT DD_BOOL IsHdr10PlusSupported;
    DD_OUT DD_BOOL IsSmpteST2084EOTF;
    DD_OUT DD_BOOL IsBT2020RGB;
    DD_OUT DD_BOOL IsBT2020YCC;

    // Color primaries data
    DD_OUT DDU16 RedX;
    DD_OUT DDU16 RedY;
    DD_OUT DDU16 GreenX;
    DD_OUT DDU16 GreenY;
    DD_OUT DDU16 BlueX;
    DD_OUT DDU16 BlueY;
    DD_OUT DDU16 WhiteX;
    DD_OUT DDU16 WhiteY;

    // Luminance Data
    DD_OUT DDU16 MinLuminance;
    DD_OUT DDU16 MaxLuminance;
    DD_OUT DDU16 MaxFullFrameLuminance;

    DD_OUT DD_BPC_SUPPORTED FormatBitDepth;
} DD_HDR10_PLUS_ARGS;

typedef struct _DD_FLIP_MPO_PLANE_ATTRIB_FLAGS
{
    union
    {
        struct
        {
            DDU32 VerticalFlip : 1;
            DDU32 HorizontalFlip : 1;
            DDU32 AlphaBlend : 1;
            DDU32 HighQualityStretch : 1;
            DDU32 Reserved : 28;
        };
        DDU32 Value;
    };
} DD_FLIP_MPO_PLANE_ATTRIB_FLAGS;

#define DD_RECT_WIDTH(Rect) (DDU32)((Rect).Right - (Rect).Left)
#define DD_RECT_HEIGHT(Rect) (DDU32)((Rect).Bottom - (Rect).Top)
#define DD_RECTS_SAME(Rect1, Rect2) ((Rect1.Left == Rect2.Left) && (Rect1.Right == Rect2.Right) && (Rect1.Top == Rect2.Top) && (Rect1.Bottom == Rect2.Bottom))

typedef struct _DD_MPO_PLANE_RECTS
{
    DD_RECT Source;
    DD_RECT Dest;
    DD_RECT Clip;
} DD_MPO_PLANE_RECTS;

typedef union _DD_PROCESS_FLAGS
{
    struct
    {
        DDU32 IsDwm : DD_BITFIELD_BIT(0);
        DDU32 Reserved : DD_BITFIELD_RANGE(1, 31);
    };
    DDU32 Value;
} DD_PROCESS_FLAGS;

typedef struct _DD_PROCESS_DETAILS
{
    WCHAR ProcessName[DD_CACHED_PROCESS_NAME_CHAR];
    DDU32 ProcessId;
    DD_PROCESS_FLAGS ProcessFlags;
} DD_PROCESS_DETAILS;

typedef struct _DD_FLIP_MPO_PLANE_ATTRIBUTES
{
    DD_FLIP_MPO_PLANE_ATTRIB_FLAGS Flags;
    DD_MPO_PLANE_RECTS Rect;
    DD_ROTATION Rotation;
    DD_COLOR_PIXEL_DESC ColorInfo;
    DD_RECT BoundedDirtyRect;
} DD_FLIP_MPO_PLANE_ATTRIBUTES;

typedef struct _DD_FLIP_MPO_IN_FLAGS
{
    union
    {
        struct
        {
            DDU32 ModeChange : 1;
            DDU32 FlipStereo : 1;
            DDU32 FlipStereoTemporaryMono : 1;
            DDU32 FlipStereoPreferRight : 1;
            DDU32 RetryAtLowerIrql : 1;
            DDU32 Reserved : 27;
        };
        DDU32 Value;
    };
} DD_FLIP_MPO_IN_FLAGS;

typedef struct _DD_FLIP_MPO_OUT_FLAGS
{
    union
    {
        struct
        {
            DDU32 PrePresentNeeded : 1;
            DDU32 HwFlipQueueDrainNeeded : 1;
            DDU32 HwFlipQueueDrainAllPlanes : 1;
            DDU32 Reserved : 29;
        };
        DDU32 Value;
    };
} DD_FLIP_MPO_OUT_FLAGS;
C_ASSERT(sizeof(DD_FLIP_MPO_OUT_FLAGS) == sizeof(DDU32));

typedef struct _DD_FLIP_MPO_PLANE_IN_FLAGS
{
    union
    {
        struct
        {
            DDU32 Enabled : 1;
            DDU32 FlipImmediate : 1;
            DDU32 FlipOnNextVSync : 1;
            DDU32 SharedPrimaryTransition : 1;
            DDU32 IndependentFlipExclusive : 1;
            DDU32 StereoFlip : 1;
            DDU32 FlipImmediateNoTearing : 1;
            DDU32 Reserved : 25;
        };
        DDU32 Value;
    };
} DD_FLIP_MPO_PLANE_IN_FLAGS;

typedef struct _DD_FLIP_MPO_PLANE_OUT_FLAGS
{
    union
    {
        struct
        {
            DDU32 PostPresentNeeded : 1;
            DDU32 HsyncInterruptCompletion : 1;
            DDU32 FlipConvertedToImmediate : 1;
            DDU32 Reserved : 29;
        };
        DDU32 Value;
    };
} DD_FLIP_MPO_PLANE_OUT_FLAGS;

typedef struct _DD_FLIP_MPO_PLANE
{
    DDU32 LayerIndex;
    DDU64 PresentId;
    DD_FLIP_MPO_PLANE_IN_FLAGS PlaneInFlags;
    DD_FLIP_MPO_PLANE_OUT_FLAGS PlaneOutFlags;
    DDU32 MaxImmediateFlipLine;
    DDU32 AllocationSegment;
    DD_LARGE_INTEGER AllocationAddress;
    HANDLE hAllocation;
    DD_FLIP_MPO_PLANE_ATTRIBUTES PlaneAttributes;
    DDU32 NumGeneratedFrames;
    DDU32 PresentSeqNumber;
} DD_FLIP_MPO_PLANE;

typedef enum _DD_FLIP_TYPE
{
    DD_FLIP_TYPE_UNKNOWN = 0,
    DD_FLIP_NO_MPO,
    DD_FLIP_BASE_MPO,
    DD_FLIP_MPO_2,
    DD_FLIP_MPO_3,       // To Report with DXGK_MULTIPLANE_OVERLAY_VSYNC_INFO2 Interrupt interface
    DD_FLIP_MPO_3_FLIPQ, // To Report with DXGK_MULTIPLANE_OVERLAY_VSYNC_INFO3 Interrupt interface
} DD_FLIP_TYPE;

typedef struct _DD_ARG_FLIP_MPO
{
    DD_IN DDU32 VidPnSourceId;
    DD_IN DD_FLIP_TYPE FlipType;
    DD_IN DD_FLIP_MPO_IN_FLAGS CommonInFlags;
    DD_OUT DD_FLIP_MPO_OUT_FLAGS CommonOutFlags;
    DD_IN DDU32 PlaneCount;
    DD_IN DD_FLIP_MPO_PLANE* pPlanes;
    DD_IN DDU32 Duration;
    DD_IN DD_MPO_HDR_METADATA HDRMetaData;
    DD_IN DDU64 TargetFlipTimeInUs;
    DD_IN DDU64 TargetFlipTime;
    DD_IN DD_BOOL DriverInternalFlip;
    // TBD - below to be added later.
    // DD_MULTIPLANE_OVERLAY_POST_COMPOSITION*pPostComposition;
#if (_DEBUG || _RELEASE_INTERNAL)
    DD_IN DD_BOOL StubCall; // Identifies the caller into Flip during simulation
    DD_BOOL WaitForVblank;  // Submits the flip after Vblank during simulation
#endif                      // (_DEBUG || _RELEASE_INTERNAL)
} DD_ARG_FLIP_MPO;

typedef struct _DD_ARG_SYNC_FLIP_MPO
{
    DD_ARG_FLIP_MPO FlipMPO;
    void* pHwDev;
} DD_ARG_SYNC_FLIP_MPO;

typedef struct _DD_MPO_CAPS
{
    union
    {
        struct
        {
            DDU32 Rotation : 1;                       // Full rotation
            DDU32 RotationWithoutIndependentFlip : 1; // Rotation, but without simultaneous IndependentFlip support
            DDU32 VerticalFlip : 1;                   // Can flip the data vertically
            DDU32 HorizontalFlip : 1;                 // Can flip the data horizontally
            DDU32 StretchRGB : 1;                     // Supports RGB formats
            DDU32 StretchYUV : 1;                     // Supports YUV formats
            DDU32 BilinearFilter : 1;                 // Blinear filtering
            DDU32 HighFilter : 1;                     // Better than bilinear filtering
            DDU32 Shared : 1;                         // MPO resources are shared across VidPnSources
            DDU32 Immediate : 1;                      // Immediate flip support
            DDU32 Plane0ForVirtualModeOnly : 1;       // Stretching plane 0 will also stretch the HW cursor and should only be used for virtual mode support
            DDU32 QueuedFlip : 1;                     // Queued flip support
            DDU32 FlipQSupportParamChange : 1;        // Allow Queued flips with attribute changes or different set of enabled/disabled planes
            DDU32 FlipQSupportFenceSync : 1;          // Capablity to monitor fence submitted to GPU and write the monitored fence when flip is visible
            DDU32 FlipQSupportHdrMetadataChanges : 1; // Allow Queued flips pHDRMetaData payload changes compared to the last queued flip.
            DDU32 UNIQUENAME(Reserved) : 17;
        };
        DDU32 Value;
    };
} DD_MPO_CAPS;

C_ASSERT(sizeof(DD_MPO_CAPS) == sizeof(DDU32));

typedef enum _DD_ORIENTATION
{
    DD_LANDSCAPE = 0,
    DD_POTRAIT = 1,
    DD_ORIENTATION_MAX = 2,
} DD_ORIENTATION;

typedef enum _DD_SURF_COMPRESSION_TYPE
{
    DD_SURF_UNCOMPRESSED = 0,
    DD_SURF_RENDER_COMPRESSED = 1,
    DD_SURF_MEDIA_COMPRESSED = 2,
    DD_SURF_COMPRESSION_MAX = 3
} DD_SURF_COMPRESSION_TYPE;

typedef enum _DD_SURF_MEMORY_TYPE // Remapping Memory Surface Enum
{
    DD_LINEAR = 0,
    DD_TILE_X = 1,
    DD_TILE_Y = 2,
    DD_TILE_YF = 3,
    DD_TILE_4 = 4,
    DD_TILE_MAX = 5,
} DD_SURF_MEMORY_TYPE;

typedef struct _DD_GET_MPO_CAPS_ARG
{
    DD_IN DDU32 VidPnSourceId;
    DD_OUT DDU32 MaxPlanes;
    DD_OUT DDU32 MaxRgbPlanes;
    DD_OUT DDU32 MaxYuvPlanes;
    DD_OUT DD_MPO_CAPS OverlayCaps;
    DD_OUT DDU32 MaxStretchFactorMultBy100;
    DD_OUT DDU32 MaxShrinkFactorPlanarMultBy100;
    DD_OUT DDU32 MaxShrinkFactorNonPlanarMultBy100;
    DD_OUT DDU32 MaxFlipQueues;     // Valid only for QueuedFlip
    DD_OUT DDU32 MaxFlipQueueDepth; // Valid only for QueuedFlip; Assuming symmetrical depth for each queue
    DD_OUT DDU32 MaxPlaneOffset;
} DD_GET_MPO_CAPS_ARG;

typedef struct _DD_CANCEL_FLIPS_PLANE
{
    DD_IN DDU32 LayerIndex;
    DD_IN DDU64 PresentIdCancelRequested;
    DD_OUT DDU64 PresentIdCancelled;
} DD_CANCEL_FLIPS_PLANE;

typedef struct _DD_CANCEL_FLIPS_ARG
{
    DD_IN DDU32 VidPnSourceId;
    DD_IN DDU32 PlaneCount;
    DD_IN DD_CANCEL_FLIPS_PLANE CancelPlanes[MAX_PLANES_PER_PIPE];
} DD_CANCEL_FLIPS_ARG;

typedef struct _DD_SET_NOTIFY_PRESENT_ID_ARG
{
    DD_IN DDU32 VidPnSourceId;
    DD_IN DDU32 LayerIndex;
    DD_IN DDU64 InterruptTargetPresentId;
} DD_SET_NOTIFY_PRESENT_ID_ARG;

typedef struct _DD_FLIPQUEUE_LOGBUFFER_ENTRY
{
    DDU64 PresentId;
    DDU64 PresentTimestamp;
} DD_FLIPQUEUE_LOGBUFFER_ENTRY;

typedef struct _DD_SET_FLIPQUEUE_LOG_BUFFER_ARG
{
    DD_IN DDU32 VidPnSourceId;
    DD_IN DDU32 LayerIndex;
    DD_IN DDU32 NumberOfEntries;
    DD_IN DD_FLIPQUEUE_LOGBUFFER_ENTRY* pLogBufferArray;
} DD_SET_FLIPQUEUE_LOG_BUFFER_ARG;

typedef struct _DD_UPDATE_FLIPQUEUE_LOG_ARG
{
    DD_IN DDU32 VidPnSourceId;
    DD_IN DDU32 LayerIndex;
    DD_OUT DDU32 FirstFreeFlipQueueLogEntryIndex; // out: Returns the index after the last written entry in the flip queue log buffer.
} DD_UPDATE_FLIPQUEUE_LOG_ARG;

// Args for CheckMPO
typedef struct DD_CHECK_MPO_OUT_FLAGS
{
    union
    {
        struct
        {
            DDU32 FailingView : 8;       // The 0 based index of the first view that could not be supported
            DDU32 FailingLayerIndex : 8; // The 0 based index of the first plane that could not be supported
            DDU32 TryAgain : 1;          // The configuration is not supported due to a transition condition, which should shortly go away
            DDU32 Reserved : 15;
        };
        DDU32 Value;
    };
} DD_CHECK_MPO_OUT_FLAGS;

typedef struct _DD_CHECK_MPO_VIEW_DETAILS
{
    DDU32 PlaneCount;
    DD_FLIP_MPO_PLANE Planes[MAX_PLANES_PER_PIPE];
    // TBD - below to be added later.
    // DD_MULTIPLANE_OVERLAY_POST_COMPOSITION*pPostComposition;
    // DD_HDR_METADATA*pHDRMetaData;
} DD_CHECK_MPO_VIEW_DETAILS;

typedef struct _DD_ARG_CHECK_MPO
{
    DD_IN DD_CHECK_MPO_VIEW_DETAILS* pViews;
    DD_OUT DD_BOOL Supported;
    DD_OUT DD_CHECK_MPO_OUT_FLAGS OutFlags;
} DD_ARG_CHECK_MPO;

typedef struct _DD_NOTIFY_FLIP_ARGS
{
    DD_IN PIPE_ID PipeId;
    DD_IN DD_BOOL AsyncFlip; // 1 = Async Flip, 0 = Sync Flip
    DD_IN DDU32 PlaneValue;
    DD_IN DDU32 SourceSizeX;
    DD_IN DDU32 SourceSizeY;
} DD_NOTIFY_FLIP_ARGS;

// Args for DxgkDdiSetVidPnSourceVisibility
typedef struct _DD_ARG_SSV
{
    DDU32 VidPnSourceId;
    DD_BOOL Visible;
} DD_ARG_SSV;

typedef struct _DD_PRESENT_REQ
{
    DDU32 SequenceNumber; // Sequence number that UMD would like to request
    // in response the display driver may respond with older,
    // if requested one is not present;
    DDU32 HistoricalEntriesNumber; // Number of historical data (up to 4 can be requested)
} DD_PRESENT_REQ;

typedef struct _DD_DISPLAY_PRESENT_INFO
{
    DDU32 SequenceNumber; // Sequence number reported; sequence numbers in
    // in single reponse can be repeated if given frame (sequenceNumber)
    // was present on multiple displays
    // order of the sequence nubmers doesn't have to be sequential
    // (it may be easier for display driver to
    // to keep cicular

    DDU32 LayerID;   // LayerID as observed by display driver
    DDU32 SourceID;  // SourceID as observed by display driver
    DDU64 PresentID; // PresentID as observed by display driver

    DD_FLIP_MPO_PLANE_IN_FLAGS Flags;

    DDU64 TimestampPresentSumbittedToHwQueue; // timestamp, where present was sumbitted to HW flip queue; 0 - means event did not happen
    DDU64 TimestampPresentVisibleOnScreen;    // timestamp, where image was actaully displayed (should be next Vsync after being put into the flip queue); 0 - means event did not happen
    DDU64 TargetFlipTimestamp;                // timestamp, where dxgkrnl expects the present to be visible on screen; 0 - means event did not happen
} DD_DISPLAY_PRESENT_INFO;

#define DD_MAX_HISTORICAL_PRESENT_INFOS 16
////////////////////////////////////////////////////////////////////////////////
/// @brief Structure that can be queried from display via escape call.
/// Consists of request part, where request parameters are specified.
/// The subsequent fields contain the feedback information from display driver.
////////////////////////////////////////////////////////////////////////////////
typedef struct _DD_DISPLAY_PRESENT_FEEDBACK_DATA
{

    // Parameters of request from UMD
    DD_PRESENT_REQ Request;

    // Response from display driver consists of an array of this structure

    DD_DISPLAY_PRESENT_INFO PresentsInfo[DD_MAX_HISTORICAL_PRESENT_INFOS]; // An array of response structures
    DDU64 TimestampLastVSync;                                              // timestamp when last VSync happened
    DDU64 TimestampNextVSyncMin;                                           // timestamp when next VSync is expected min value (min is for VRR case, for not VRR case it is the expected time of VSync)
    DDU64 TimestampNextVSyncMax;                                           // timestamp when next VSync is expected min value (max is for VRR case, for not VRR case it is 0)
} DD_DISPLAY_PRESENT_FEEDBACK_DATA;
// Args for DxgkDdiRecommendMonitorModes
typedef struct _DD_COLOR_COEFF_DYNAMIC_RANGES
{
    DDU32 FirstChannel;
    DDU32 SecondChannel;
    DDU32 ThirdChannel;
    DDU32 FourthChannel;
} DD_COLOR_COEFF_DYNAMIC_RANGES;

typedef enum _DD_MONITOR_CAPABILITIES_ORIGIN
{
    DD_MCO_UNINITIALIZED = 0,
    DD_MCO_DEFAULTMONITORPROFILE = 1,
    DD_MCO_MONITORDESCRIPTOR = 2,
    DD_MCO_MONITORDESCRIPTOR_REGISTRYOVERRIDE = 3,
    DD_MCO_SPECIFICCAP_REGISTRYOVERRIDE = 4,
    DD_MCO_DRIVER = 5, //  + display adapter driver.
} DD_MONITOR_CAPABILITIES_ORIGIN;

typedef struct _DD_OS_MONITOR_MODE
{
    DDU32 OsModeId;
    DD_TIMING_INFO TimingInfo;
    DD_COLOR_COEFF_DYNAMIC_RANGES ColorCoeffDynamicRanges;
    DD_MONITOR_CAPABILITIES_ORIGIN Origin;
} DD_OS_MONITOR_MODE;

typedef struct _DD_GET_OS_ADDITIONAL_TARGET_MODE_ARG
{
    DD_IN DDU32 TargetId;
    DD_OUT DDU32 NumModes;
    DD_OUT DD_TIMING_INFO* pTimingInfo;
} DD_GET_OS_ADDITIONAL_TARGET_MODE_ARG;

typedef enum _DD_MONITOR_DESCRIPTOR_TYPE
{
    DD_MDT_UNINITIALIZED = 0,
    DD_MDT_VESA_EDID_V1_BASEBLOCK = 1,
    DD_MDT_VESA_EDID_V1_BLOCKMAP = 2,
    DD_MDT_OTHER = 255
} DD_MONITOR_DESCRIPTOR_TYPE;

typedef struct _DD_MONITOR_DESCRIPTOR
{
    DDU32 Id;
    DD_MONITOR_DESCRIPTOR_TYPE Type;
    DDU32 DataSize;
    void* pData;
    DD_MONITOR_CAPABILITIES_ORIGIN Origin;
} DD_MONITOR_DESCRIPTOR;

typedef struct _DD_JOINT_TARGET_PROPERTIES
{
    DD_BOOL IsValid;
    DD_BOOL IsPrimaryTarget;
    DD_BOOL IsCollageChildTarget;
    DDU8 ScalingCap; // 1- no scaling, 2- full stretch capable, 3- clone others
    DDU8 TotalNumberOfHTiles;
    DDU8 TotalNumberOfVTiles;
    DDU8 HLocation; // 0 based indexing, 0 - left most tile
    DDU8 VLocation; // 0 based indexing, 0 - left most tile
    DDU32 HorTileSizeInPixels;
    DDU32 VerTileSizeInLines;
    DDU8 PixelMultiplier;
    DDU8 TopBezelsize;
    DDU8 BottomBezelsize;
    DDU8 LeftBezelsize;
    DDU8 RightBezelsize;
    DDU8 SerialNumber[4];           // Bytes 8 to 11 : Serial Number obtained from DID corresponding to Display
    DD_RECT FbSrc;                  // Rectangle that is a part of the combined desktop framebuffer and rendered to Display[i]
    DD_RECT FbPos;                  // Rectangle in Display[i]'s framebuffer where FbSrc[i] is placed
    DD_ROTATION DisplayOrientation; // Rotation info
    DD_TARGET_MODE_INFO TargetMode; // Target mode info
} DD_JOINT_TARGET_PROPERTIES;

typedef struct _DD_TARGET_LIST
{
    DDU32 NumTargets;
    DDU32 TargetIds[MAX_POSSIBLE_PIPES];
} DD_TARGET_LIST;

// Target descriptor to be passed to protocol
typedef struct _DD_TARGET_DESCRIPTOR
{
    DD_PORT_TYPE Port;
    DD_VIDEO_OUTPUT_TECHNOLOGY SinkType;
    DDU8 SinkIndex; // starts with index 0 for each display type; non zero for DP MST or other multi-display scenarios on same port
} DD_TARGET_DESCRIPTOR;

typedef struct _DD_OVERRIDE_MONITOR_DESCRIPTOR_ARG
{
    IN DDU32 TargetId;
    IN DD_TARGET_DESCRIPTOR TargetDesc;
    OUT DDU8 NumMonitorDescriptor;
    OUT DD_MONITOR_DESCRIPTOR* pMonitorDescriptor;
    OUT DD_BOOL OverrideRequired;
} DD_OVERRIDE_MONITOR_DESCRIPTOR_ARG;

typedef struct _DD_ARG_RMM
{
    DD_IN DDU32 ChildId;
    DD_IN DDU32 NumOsMonitorModes;
    DD_IN DD_OS_MONITOR_MODE* pOsMonitorModes;
    DD_OUT DDU32 NumDriverMonitorModesToAdd;
    DD_OUT DD_OS_MONITOR_MODE* pDriverMonitorModesToAdd;
} DD_ARG_RMM;

// Args for DxgkDdiIsSupportedVidPn
typedef struct _DD_ARG_ISV
{
    DD_IN DD_TOPOLOGY* pTopology;
    DD_OUT DD_BOOL IsVidPnSupported;
} DD_ARG_ISV;

// Args for Getting Scanline
typedef struct _DD_GET_SCANLINE_ARGS
{
    DD_IN DDU32 TargetId;
    DD_OUT DD_BOOL InVerticalBlank;
    DD_OUT DDU32 CurrScanLine;
} DD_GET_SCANLINE_ARGS;

// Args for DxgkDdiEnumVidPnCofuncModality
typedef struct _DD_ARG_EVCM
{
    DD_IN DD_TOPOLOGY* pConstrainingTopology;
} DD_ARG_EVCM;

// Args for VSync Control Interrupt
typedef enum _DD_CRTC_VSYNC_STATE
{
    DD_VSYNC_ENABLE = 0,
    DD_VSYNC_DISABLE_KEEP_PHASE = 1,
    DD_VSYNC_DISABLE_NO_PHASE = 2,
} DD_CRTC_VSYNC_STATE;

typedef enum _DD_SAGV_CONFIG
{
    DD_DISABLE_ALL_SAGV_POINTS,
    DD_ENABLE_ALL_SAGV_POINTS,
    DD_ENABLE_POSSIBLE_SAGV_POINTS,
} DD_SAGV_CONFIG;

typedef struct _DD_CTRL_VSYNC_INTERRUPT_ARG
{
    DD_CRTC_VSYNC_STATE CrtcVsyncState;
    DDU32 VidPnSourceId;
} DD_CTRL_VSYNC_INTERRUPT_ARG;

// Args for DxgkDdiSetPointerPosition
typedef struct _DD_ARG_SPP
{
    DDU32 VidPnSourceId;
    DDU32 X;
    DDU32 Y;
    DD_BOOL Visible;
} DD_ARG_SPP;

// Args for DxgkDdiSetPointerShape

typedef struct _DD_POINTER_FLAGS
{
    union
    {
        struct
        {
            DDU32 Monochrome : 1;
            DDU32 Color : 1;
            DDU32 MaskedColor : 1;
            DDU32 SoftwareCursor : 1;
            DDU32 Reserved : 28;
        };
        DDU32 Value;
    };
} DD_POINTER_FLAGS;

typedef struct _DD_ARG_SPS
{
    DDU32 VidPnSourceId;
    DDU32 Width;
    DDU32 Height;
    DDU32 Pitch;
    void* pPixels;
    DDU32 XHot;
    DDU32 YHot;
    DD_POINTER_FLAGS Flags;
} DD_ARG_SPS;

// Args for DxgkDdiQueryChildRelations

typedef enum _DD_CHILD_DEVICE_TYPE
{
    DD_CD_TYPE_UNKNOWN,
    DD_CD_TYPE_VIDEO_OUTPUT,
    DD_CD_TYPE_OTHER,
    DD_CD_TYPE_INTEGRATED_DISPLAY
} DD_CHILD_DEVICE_TYPE;

typedef enum _DD_MONITOR_ORIENTATION_AWARENESS
{
    DD_MOA_UNINITIALIZED = 0,
    DD_MOA_NONE = 1,
    DD_MOA_POLLED = 2,
    DD_MOA_INTERRUPTIBLE = 3
} DD_MONITOR_ORIENTATION_AWARENESS;

typedef struct _DD_VIDEO_OUTPUT_CAPABILITIES
{
    DD_VIDEO_OUTPUT_TECHNOLOGY InterfaceTechnology;
    DD_BOOL InternalDisplay;
    DD_MONITOR_ORIENTATION_AWARENESS MonitorOrientationAwareness;
    DD_BOOL SupportsSdtvModes;
} DD_VIDEO_OUTPUT_CAPABILITIES;

typedef enum _DD_CHILD_DEVICE_HPD_AWARENESS
{
    DD_CD_HPDA_UNKNONN = 0,
    DD_CD_HPDA_ALWAYS_CONNECTED = 1,
    DD_CD_HPDA_NONE = 2,
    DD_CD_HPDA_POLLED = 3,
    DD_CD_HPDA_INTERRUPTIBLE = 4
} DD_CHILD_DEVICE_HPD_AWARENESS;

typedef struct _DD_CHILD_CAPABILITIES
{
    union
    {
        // TypeVideoOutput
        DD_VIDEO_OUTPUT_CAPABILITIES VideoOutput;
        // TypeOther
        struct
        {
            DDU32 MustBeZero;
        } Other;
    } Type;

    DD_CHILD_DEVICE_HPD_AWARENESS HpdAwareness;
} DD_CHILD_CAPABILITIES;

typedef struct _DD_CHILD_DESCRIPTOR
{
    DD_CHILD_DEVICE_TYPE ChildDeviceType;
    DD_CHILD_CAPABILITIES ChildCapabilities;
    DDU32 AcpiUid;
    DDU32 ChildUid;
} DD_CHILD_DESCRIPTOR;

typedef struct _DD_ARG_QCR
{
    DD_OUT DD_CHILD_DESCRIPTOR* pChildRelations;
    DD_IN_OUT DDU32 NumChildDescriptor;
} DD_ARG_QCR;

typedef struct _DD_CONTAINERID_ARGS
{
    DD_IN DDU32 DisplayUID;
    struct
    {
        DD_OUT DDU64 PortId;
        DD_OUT DDU16 ManufacturerName;
        DD_OUT DDU16 ProductCode;
    } EldInfo;
} DD_CONTAINERID_ARGS;

typedef struct _DD_SRC_VISIBILITY
{
    DD_IN DDU32 SourceID;
    DD_IN DD_BOOL Visible;
} DD_SRC_VISIBILITY;

typedef enum _DD_OPERATION
{
    DD_OPERATION_QUERY_CURRENT = 0, // Get the details set through last set call
    DD_OPERATION_QUERY_DEFAULT = 1, // Get the driver default values
    DD_OPERATION_QUERY_CAPABILITY = 2, // Get capability
    DD_OPERATION_RESTORE_DEFAULT = 3, // Restore default values.
    DD_OPERATION_SET_CUSTOM = 4, // Set custom values.
    DD_OPERATION_DISABLE = 5, // Disable
    DD_OPEARTION_MAX
} DD_OPERATION;

typedef enum _DD_LACE_TRIGGER
{
    DD_LACE_TRIGGER_AMBIENT_LIGHT = 1, // LACE enhancement depends on Ambient Light
    DD_LACE_TRIGGER_FIXED_AGGRESSIVENESS = 2  // LACE enhancement is as per given fixed aggressiveness Percent
} DD_LACE_TRIGGER;

typedef struct _DD_LACE_LUX_AGGR_MAP
{
    DDU32 NumEntries;                                                                  // Number of entries in the given mapping table
    DD_LUX_AGGR_MAP_ENTRY LuxToAggrMappingTable[LACE_MAX_NUM_ENTRIES_IN_LUX_AGGR_MAP]; // Max_Num_Entries which can be passed in LuxToAggrMappingTable
} DD_LACE_LUX_AGGR_MAP;

typedef struct _DD_GHE_LUX_AGGR_MAP
{
    DDU32 NumEntries;                                                                 // Number of entries in the given mapping table
    DD_LUX_AGGR_MAP_ENTRY LuxToAggrMappingTable[GHE_MAX_NUM_ENTRIES_IN_LUX_AGGR_MAP]; // Max_Num_Entries which can be passed in LuxToAggrMappingTable
} DD_GHE_LUX_AGGR_MAP;

typedef struct _DD_LACE_CONFIG_ARGS
{
    DDU32 Size;   // size of this structure
    DDU8 Version; // version of this structure
    DDU32 TargetID;
    DD_BOOL Enabled;         // Enable or disable LACE feature
    DD_OPERATION OpType;     // Get/Set Operations used for additional settings
    DD_LACE_TRIGGER Trigger; // LACE mode to be triggered in
    union
    {
        DD_LACE_LUX_AGGR_MAP AggressivenessMap; // Lux to enhancement mapping table, applicable for LACE_TRIGGER_AMBIENT_LIGHT
        DDU8 FixedAggressivenessPercent;        // Fixed aggressiveness level, applicable for LACE_TRIGGER_FIXED_AGGRESSIVENESS
    };
    DDU32 Reserved[2]; // Reserved for future use
} DD_LACE_CONFIG_ARGS;

typedef enum _GHE_TRIGGER
{
    GHE_TRIGGER_INVALID = 0,
    GHE_TRIGGER_AMBIENT_LIGHT = 1, // GHE enhancement depends on Ambient Light
    GHE_TRIGGER_FIXED_AGGRESSIVENESS = 2, // GHE enhancement is as per given Fixed Aggressiveness Percent
} GHE_TRIGGER;

typedef struct _DD_GHE_CONFIG_ARGS
{
    DDU32 Size;   // size of this structure
    DDU8 Version; // version of this structure
    DDU32 TargetID;
    DD_BOOL Enabled;     // Enable or disable GHE feature
    DD_OPERATION OpType; // Get/Set Operations used for additional settings
    GHE_TRIGGER Trigger; // GHE mode to be triggered in
    union
    {
        DD_GHE_LUX_AGGR_MAP AggressivenessMap; // Lux to enhancement mapping table, applicable for GHE_TRIGGER_AMBIENT_LIGHT
        DDU8 FixedAggressivenessPercent;       // Fixed aggressiveness level, applicable for GHE_TRIGGER_FIXED_AGGRESSIVENESS
    };
} DD_GHE_CONFIG_ARGS;

// Brightness Optimization Feature Parameters
typedef union _DD_FTR_BRT_OPTIMIZATION_PARAM
{
    struct
    {
        DDU8 NumOfAvailableAggrLevel; // Set by PC to UI in Get operation
        DDU8 UserMaximumAggrLevel;    // Current/User select Maximum Aggressiveness level
        DD_BOOL Enable;
        DD_BOOL Supported;
    };
    DDU32 Value;
} DD_FTR_BRT_OPTIMIZATION_PARAM;

typedef enum _DD_FTR_BRT_OPTIMIZATION_OPERATION
{
    DD_FTR_BRT_OPTIMIZATION_OPERATION_INVALID = 0,
    DD_FTR_BRT_OPTIMIZATION_OPERATION_GET = 1, // Get currently applied configuration
    DD_FTR_BRT_OPTIMIZATION_OPERATION_SET = 2, // Set given configuration
    DD_FTR_BRT_OPTIMIZATION_OPERATION_MAX
} DD_FTR_BRT_OPTIMIZATION_OPERATION;

// I2C MCCS interface
#define AB_HOST_ADDR 0x51
#define BYTES_SENT_FOR_DATA_LENGTH 0x02
#define MCCS_DEVICE_ADDR 0x6E
#define MAX_LUT_AUX_BUFSIZE 0x0200
typedef struct _DD_I2C_ARGS
{
    DDU32 ChildUid;
    DD_BOOL WriteI2C; // 0 - Read I2C, 1 - Write I2C
    DDU32 I2CAddress;
    DDU32 DataLength;
    void* pData;
} DD_I2C_ARGS;

typedef enum _DD_EDID_MGMT_OP
{
    DD_EDID_OP_UNKNOWN = 0,
    DD_EDID_LOCK,
    DD_EDID_UNLOCK,
    DD_EDID_OVERRIDE,
    DD_EDID_REMOVE_OVERRIDE,
    DD_EDID_READ,
    DD_EDID_OP_MAX
} DD_EDID_MGMT_OP;

typedef enum _DD_EDID_REQ_TYPE
{
    DD_EDID_REQ_NONE = 0,
    DD_EDID_REQ_MONITOR,  // in/out
    DD_EDID_REQ_SUPPLIED, // in/out
    DD_EDID_REQ_CURRENT   // in
} DD_EDID_REQ_TYPE;

/**
 * @brief Driver internal struct for EDID management arg.
 *
 */
typedef struct _DD_EDID_MGMT_ARG
{
    IN DDU32 TargetId;
    IN DD_EDID_MGMT_OP OpCode;
    IN OUT DD_EDID_REQ_TYPE EdidType; // DD_EDID_READ (DD_EDID_REQ_CURRENT) will have out arg
    IN OUT DDU32 EdidSizeBytes;       // Size of all blocks together.
    IN OUT DDU8* pEdid;               // Complete EDID buffer
    OUT DD_BOOL OsConnNotification;
    OUT DD_BOOL IsDisplayConnected;
} DD_EDID_MGMT_ARG;
typedef struct _DD_OSL_INIT_ARG
{
    DD_IN DD_BOOL DodEnabled;
    DD_IN DISPLAY_SWITCH_METHOD DisplaySwitchMethod;
    DD_OUT DDU32 NumberOfDisplayChildren;
    DD_OUT DDU8 NumLfpDisplays;
} DD_OSL_INIT_ARG;

//----------------------------------------------------------------------------
//
//  ACPI related enums and data structures - START
//
//----------------------------------------------------------------------------

//  Arguments to evaluate _DOD.
#define DD_MAX_DIDS 0x10

typedef struct _DD_GET_ACPI_ID
{
    DD_IN DDU32 TargetId; // of type DD_TARGET_ID
    DD_OUT DDU32 AcpiId;  // of type ACPI30_DOD_ID
} DD_GET_ACPI_ID;

typedef enum _DD_LID_STATE
{
    DD_LID_STATE_CLOSE = 0,
    DD_LID_STATE_OPEN,
} DD_LID_STATE;

// Args for DxgkDdiNotifyAcpiEvent
typedef enum _DD_ACPI_EVENT_TYPE
{
    DD_ACPI_EVENT_TYPE_UNKNOWN = 0,
    DD_ACPI_EVENT_TYPE_ACPI_EVENT,
    DD_ACPI_EVENT_TYPE_POWER_STATE_EVENT,
    DD_ACPI_EVENT_TYPE_DOCKING_EVENT,
    DD_ACPI_EVENT_TYPE_CHAINED_EVENT,
} DD_ACPI_EVENT_TYPE;

typedef enum _DD_ACPI_EVENT
{
    DD_ACPI_EVENT_UNKNOWN = 0,
    DD_ACPI_EVENT_AC_DC_SWITCH,
    DD_ACPI_EVENT_PANEL_SWITCH,
    DD_ACPI_EVENT_LID_STATE_CHANGE,
    DD_ACPI_EVENT_DOCK_STATE_CHANGE,
    // unused
    /*
    DD_ACPI_EVENT_CYCLE_DISPLAY_HOTKEY,
    DD_ACPI_EVENT_DEVICE_HOTPLUG,
    DD_ACPI_EVENT_PANEL_SWITCH,
    DD_ACPI_EVENT_VIDEO_WAKEUP,
    */
} DD_ACPI_EVENT;

typedef enum _DD_ACPI_FLAGS
{
    ACPI_FLAGS_UNKNOWN = 0,
    ACPI_FLAGS_POLL_DISPLAY_CHILDREN,
    ACPI_FLAGS_CHANGE_DISPLAY_MODE,
} DD_ACPI_FLAGS;

typedef enum _DD_ADAPTER_STATE
{
    DD_ADAPTER_NOTREADY = 0,
    DD_ADAPTER_READY = 1
} DD_ADAPTER_STATE;

typedef struct _DD_NOTIFY_ACPI_EVENT_ARGS
{
    DD_IN DD_ADAPTER_STATE AdapterState;
    DD_IN DD_ACPI_EVENT_TYPE AcpiEventType;
    DD_IN DD_ACPI_EVENT AcpiEvent;
    DD_IN DDU32 EventArg;
    DD_OUT DD_ACPI_FLAGS AcpiFlags;
} DD_NOTIFY_ACPI_EVENT_ARGS;

//  Arguments to get BLC levels from BIOS (got by evaluating _BLC)
typedef struct _DD_GET_ACPI_BCL_LEVELS
{
    DD_IN DDU32 BufferSize;            // The size, in bytes, of the buffer that is passed in the BrightnessLevels parameter
    DD_IN_OUT DDU8* pLevelCount;       // A pointer to a variable that receives the number of brightness levels that the driver returns in the buffer that the BrightnessLevels parameter points to.
    DD_IN_OUT DDU8* pBrightnessLevels; // A pointer to a buffer that receives the brightness levels that an integrated display panel supports.
} DD_GET_ACPI_BCL_LEVELS;

typedef struct _DD_BACKLIGHT_INFO
{
    DDU16 BlcUserSetting;
    DDU32 BlcEffective;
} DD_BACKLIGHT_INFO;

typedef struct _DD_POST_DISPLAY_INFO
{
    DD_IN DDU32 TargetID;
    DD_OUT DDU32 Width;
    DD_OUT DDU32 Height;
    DD_OUT DDU32 Pitch;
    DD_OUT DD_PIXELFORMAT ColorFormat;
    DD_IN DD_BOOL IsSystemInBugCheck;
    DD_IN DDU32 GfxAddress;
} DD_POST_DISPLAY_INFO;

// ----------------------------------------------------------------------------
//
// Header required to convert OS DDI parameters into driver defined parameters - END
//
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//
// CCD data structures -- START
//
// ----------------------------------------------------------------------------

// Tile widths to use with each of the above suface types
// For YF tiling, 8bpp has a different width than the others
// All tile widths are in bytes.
/* typedef enum _SURFACE_MEMORY_TILE_WIDTH
{
    SURFACE_MEMORY_TILE_WIDTH_DEFAULT = 64,
    SURFACE_MEMORY_TILE_WIDTH_LINEAR = SURFACE_MEMORY_TILE_WIDTH_DEFAULT,
    SURFACE_MEMORY_TILE_WIDTH_TILED = 512,
    SURFACE_MEMORY_TILE_WIDTH_X_TILED = SURFACE_MEMORY_TILE_WIDTH_TILED,
    SURFACE_MEMORY_TILE_WIDTH_Y_LEGACY_TILED = 128,
    SURFACE_MEMORY_TILE_WIDTH_Y_F_TILED_8BPP = 64,
    SURFACE_MEMORY_TILE_WIDTH_Y_F_TILED_NON_8BPP = 128
} SURFACE_MEMORY_TILE_WIDTH;*/

typedef enum _SURFACE_MEMORY_TYPE DD_SURFACE_MEMORY_TYPE; // TBD: Need to change this to DD_ defn.

#define DD_IS_Y_TILE_MEMORY(SurfaceMemType) ((SurfaceMemType == SURFACE_MEMORY_Y_LEGACY_TILED) || (SurfaceMemType == SURFACE_MEMORY_Y_F_TILED))
// Struct representing surface memory offset data (used in GMM)
/* typedef struct _SURFACE_MEM_OFFSET_INFO
{
    // Indicates surface memory type
    DD_IN  DD_SURFACE_MEMORY_TYPE SurfaceMemType;
    union
    {
        // Tiled offset
        struct
        {
            DD_IN  DDU32 TiledXOffset;
            DD_IN  DDU32 TiledYOffset;
            DD_IN  DDU32 TiledUVXOffset; // NV12 case
            DD_IN  DDU32 TiledUVYOffset; // NV12 case
            DD_IN  DDU64 BaseAddress;
            DD_IN  DDU64 UVBaseAddress;
        };
    };
    DD_IN DDU32 UvDistance;  // For NV12 surface, as of now nv12 cant come in normal flip path
    DD_IN DDU32 AuxDistance; // Control surface Aux Offset. Will be 0 for non unified allocations, MP is abstracted from it
} SURFACE_MEM_OFFSET_INFO;*/

typedef enum _DD_FLIP_INTERRUPT_TYPE
{
    DD_FLIP_INTERRUPT_TYPE_UNKNOWN,
    DD_FLIP_INTERRUPT_TYPE_VBLANK,
    DD_FLIP_INTERRUPT_TYPE_FLIP_DONE,
    DD_FLIP_INTERRUPT_TYPE_DMC_FLIPQ,
    DD_FLIP_UPDATE_LOG_BUFFER,
    DD_FLIP_INTERRUPT_TYPE_MAX
} DD_FLIP_INTERRUPT_TYPE;

#define DD_IS_VALID_FLIP_INTERRUPT_TYPE(Type) ((Type > DD_FLIP_INTERRUPT_TYPE_UNKNOWN) && (Type < DD_FLIP_INTERRUPT_TYPE_MAX))

// Args for CcdReportFlipDone
typedef struct _DD_ARG_RFD
{
    DD_FLIP_INTERRUPT_TYPE InterruptType;
    PIPE_ID PipeId;
} DD_ARG_RFD;

typedef enum _DD_PLANE_ENCRYPTION_MODE
{
    DD_PLANE_ENCRYPTION_OFF,
    DD_PLANE_HEAVY_ENCRYPTION,
    DD_PLANE_STOUT_ENCRYPTION
} DD_PLANE_ENCRYPTION_MODE;

#define DD_IS_PLANE_ENCRYPTED(EncryptionMode) ((DD_PLANE_HEAVY_ENCRYPTION == EncryptionMode) || (DD_PLANE_STOUT_ENCRYPTION == EncryptionMode))

typedef struct _DD_SURFACE_ATTRIBUTES
{
    DD_IN DD_PIXELFORMAT PixelFormat;
    DD_IN DD_SURFACE_MEMORY_TYPE SurfMemType;
    DD_IN DDU32 SurfStride;
    DD_IN DDU32 UvSurfDistance;
    DD_IN DDU32 UvSurfStride;
    DD_IN DD_PLANE_ENCRYPTION_MODE EncryptionMode;
    DD_IN DD_COLOR_PIXEL_DESC ColorInfo;
} DD_SURFACE_ATTRIBUTES;

typedef struct _DD_COMPRESSION_DETAILS
{
    DD_SURF_COMPRESSION_TYPE CompType;
    DD_BOOL BaseClearColorEnable;   // Indication whether Clear Colour resolve need to enabled/disable in HW for the Render Compressed surface.
    DDU32 BaseCompMetaDataDistance; // Control surface distance from RGB main surface in case of Render Compression or Control surface distance from RGB or Y main surface in case of Media Compression
    DDU32 BaseCompMetaDataStride;   // stride of the control surface for RGB main surface in case of Render Compression
    DDU64 BaseClearColorValue;      // Clear colour value used by display HW to resolve the Render Compressed surface. Value can be 0.
    DDU32 UVCompMetaDataDistance;   // Control surface distance from UV main surface in case of Media Compression
    DDU32 UVCompMetaDataStride;     // stride of the control surface from UV main surface in case of Media Compression, but s/w doesn't program this value into h/w
    DDU64 UVClearColorValue;
} DD_COMPRESSION_DETAILS;

typedef struct _DD_GET_COMPRESSION_DEATILS_ARGS
{
    DD_IN HANDLE hAllocation;
    DD_IN DD_ROTATION PlaneOrientation;
    DD_IN DD_PIXELFORMAT PixelFormat;
    DD_OUT DD_COMPRESSION_DETAILS* pCompDetails;
} DD_GET_COMPRESSION_DEATILS_ARGS;

typedef struct _DD_GET_FBR_STATE_ARGS
{
    DD_IN HANDLE hAllocation;
    DD_IN DD_BOOL FbrEnabled;
} DD_GET_FBR_STATE_ARGS;

typedef struct _DD_GET_SURF_ATTRIB_ARGS
{
    DD_IN HANDLE hAllocation;
    DD_IN DD_ROTATION PlaneOrientation;
    DD_OUT DD_SURFACE_ATTRIBUTES* pPlaneSurfAttributes;
} DD_GET_SURF_ATTRIB_ARGS;

typedef struct _DD_GET_SURF_POSITION_ARGS
{
    DD_IN HANDLE hAllocation;
    DD_IN DD_ROTATION Rotation;
    DD_IN DD_LARGE_INTEGER AllocationAddress;
    DD_IN DDU32 PlaneHeight;
    DD_IN DDU32 LayerIndex;
    DD_IN DDU32 ViewId;
    DD_IN DD_BOOL NeedHwAddress;
    DD_IN_OUT DDU32 PanningPosX;
    DD_IN_OUT DDU32 PanningPosY;
    DD_OUT DDU32 UvPanningPosX;
    DD_OUT DDU32 UvPanningPosY;
    DD_OUT DDU32 HwAddressBase;
    DD_OUT DDU32 HwAddressUv;
} DD_GET_SURF_POSITION_ARGS;

typedef struct _DD_GET_SURF_OFFSET_S3D_ARGS
{
    DD_IN HANDLE hAllocation;
    DD_OUT DDU32 RFrameAddress;
} DD_GET_SURF_OFFSET_S3D_ARGS;

typedef struct _DD_GET_POSSIBLE_TILING_ARGS
{
    DD_IN DDU32 VidPnSourceId;
    DD_IN DDU32 HResolution;
    DD_IN DD_ROTATION Rotation;
    DD_OUT DDU32 MemFormats; // Mask of the suported Formats
} DD_GET_POSSIBLE_TILING_ARGS;

/**
 * @brief  Display connection related events to be reported to OS.
 */
typedef enum _DD_DISPLAY_CONNECTION_EVENTS
{
    DD_DISP_CONN_UNKNOWN,
    DD_DISP_CONN_PLUG,   // display plugged in
    DD_DISP_CONN_UNPLUG, // display unplugged from connector
    DD_DISP_CONN_REPLUG, // display unplugged and then new display plugged in, both of same sink type. possible when switching SST/MST mode from panel settings; MST subtopology change etc.
    DD_DISP_CONN_MAX_EVENT
} DD_DISPLAY_CONNECTION_EVENTS;

#define DD_IS_DISPLAY_ATTACHED(ConnEvent) ((ConnEvent) == DD_DISP_CONN_PLUG || (ConnEvent) == DD_DISP_CONN_REPLUG)

#define USB4_ROUTER_TOPOLOGY_MAX_LEVEL 5
typedef struct _DD_USB4_CAPS
{
    DD_BOOL IsUsb4Display;
    DDU8 DpInAdapterNumber;
    DDU8 Usb4DriverId; // Used to report the USB4 caps to OS
    DDU8 Usb4RouterTopologyId[USB4_ROUTER_TOPOLOGY_MAX_LEVEL];
} DD_USB4_CAPS;

typedef struct _DD_QUERY_CONNECTION_CHANGE
{
    DD_OUT DDU64 ConnectionChangeId;
    DD_OUT DDU32 TargetId;
    DD_OUT DD_DISPLAY_CONNECTION_EVENTS ConnectionStatus;
    DD_OUT DD_DISPLAY_LINK_EVENTS LinkState;
    DD_OUT DD_VIDEO_OUTPUT_TECHNOLOGY LinkTargetType;
    DD_OUT DD_BOOL ConnectionChangesPending;
    DD_OUT DD_BOOL LinkChangesPending;
    DD_OUT DD_USB4_CAPS Usb4Info;
    DD_OUT DD_BOOL MdmMonitorConnectionChange;
} DD_QUERY_CONNECTION_CHANGE;

typedef struct _DD_CONNECTION_STATUS
{
    DDU32 DisplayUid;
    DD_DISPLAY_CONNECTION_EVENTS Event;

    // DD_BOOL NotifyOPM;
    // DD_BOOL IsModesetRequired;
} DD_CONNECTION_STATUS;

typedef struct _DD_VSYNC_INFO
{
    DDU32 LayerIndex;
    DD_BOOL Enabled;
    DDU64 OsPresentId;
    DD_LARGE_INTEGER OsAddress;
    DDU32 LogBufferFreeIndex; // First Free index of the Log Buffer Array
    // Applicable only if Flip Type is DD_FLIP_MPO_3_FLIPQ

    DD_FLIP_MPO_PLANE_ATTRIBUTES PlaneAttribs;
} DD_VSYNC_INFO;

typedef struct _DD_CB_VSYNC_ARG
{
    DD_FLIP_TYPE FlipType;
    DD_BOOL FlipDoneEvent;
    DDU32 TargetId;
    DDU32 VsyncInfoCount;
    DD_VSYNC_INFO VsyncInfo[MAX_PLANES_PER_PIPE];
    void* pPreAllocatedLocal;
} DD_CB_VSYNC_ARG;

typedef struct _DD_CB_PERIODICMONITOREDFENCE_ARG
{
    DDU32 NotificationId; // ID to return to OS
    DDU32 VidPnTargetId;  // VidPnTargetID to return to OS
} DD_CB_PERIODICMONITOREDFENCE_ARG;

#if (_DEBUG || _RELEASE_INTERNAL)

// ----------------------------------------------------------------------------
//
// MPO Simulation data struture
//
// ----------------------------------------------------------------------------

typedef struct _DD_SIMDRV_MPO_ARG
{
    DD_BOOL InMpoSimulationMode;
    DD_BOOL MpoSimulationActiveOnPipe[MAX_POSSIBLE_PIPES];
    DD_BOOL MpoExitSimulationPipe[MAX_POSSIBLE_PIPES];
    void* pMpoViews[MAX_POSSIBLE_PIPES];
    DD_BOOL MpoSimulatedFlipDoneFlag[MAX_POSSIBLE_PIPES][MAX_PLANES_PER_PIPE]; // Used to store the status of FlipDone for Simulated flips on each pipe for each plane
} DD_SIMDRV_MPO_ARG;

#endif // (_DEBUG || _RELEASE_INTERNAL)
// ----------------------------------------------------------------------------
//
// CCD data structures -- END
//
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//
// Power Manager data structures -- START
//
// ----------------------------------------------------------------------------

typedef enum _DD_POWERRUNTIMESTATE
{
    DD_F0_STATE = 0,
    DD_F1_STATE = 1
} DD_POWERRUNTIMESTATE;

typedef enum _DD_DEV_POWER_STATE
{
    DD_POWERSTATE_UNDEFINED = 0,
    DD_POWERSTATE_D0 = 1,
    DD_POWERSTATE_D1,
    DD_POWERSTATE_D2,
    DD_POWERSTATE_D3,
} DD_DEV_POWER_STATE;

typedef enum _DD_SYS_POWER_ACTION
{
    DD_POWERACTION_UNDEFINED = 0,
    DD_POWERACTION_NONE,          // Connected Stand-by mode
    DD_POWERACTION_SLEEP,         // Sleep mode
    DD_POWERACTION_HIBERNATE,     // Hibernate mode  OR (Normal Shutdown when Fast Startup is ENABLED from Power Option page)
    DD_POWERACTION_SHUTDOWN,      // Shutdown sequence initiated from Command prompt
    DD_POWERACTION_SHUTDOWNRESET, // Restart sequence or Restart from cmd prompt
    DD_POWERACTION_SHUTDOWNOFF,   // Shift+shutdown OR ( Normal Shutdown when Fast Startup DISABLED from Power Option page) OR Shutdown Signout from cmd prompt(with or without Fast Startup disabled)
} DD_SYS_POWER_ACTION;

typedef struct _DD_SET_ADAPTER_PWR_ARGS
{
    DD_DEV_POWER_STATE DisplayDevPowerState;
    DD_SYS_POWER_ACTION DisplaySysPowerAction;
} DD_SET_ADAPTER_PWR_ARGS;

// ----------------------------------------------------------------------------
//
// Power Manager data structures -- END
//
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//
// TDR related data structures -- START
//
// ----------------------------------------------------------------------------
typedef enum _DD_TDR_OPERATION
{
    DD_TDR_OPERATION_RESET = 1,              // Reset hardware
    DD_TDR_OPERATION_RESET_PRE_GDRST,        // Used to inform SB prior of GDRST
    DD_TDR_OPERATION_RESET_POST_GDRST,       // Used to inform SB post GDRST.
    DD_TDR_OPERATION_RESTART,                // Restart hardware
    DD_TDR_OPERATION_COLLECT_INFO_REGISTERS, // Dump registers into OCA report
    DD_TDR_OPERATION_COLLECT_INFO_STATE,     // Dump state info into OCA report
} DD_TDR_OPERATION;

typedef struct _DD_TDR_ARGS
{
    DD_TDR_OPERATION Operation; // TDR operation
    void* pOCABlobHdr;          // Pointer to OCA blob header
    void* pDebugBuff;           // pointer to debug buffer
    DDU32 DebugBufAvailSize;    // Available size of the debug buffer
    DDU32 OutputSize;           // output size
} DD_TDR_ARGS;

typedef struct _DD_DIAG_ARGS
{
    void* pOCABlobHdr;             // Pointer to OCA blob header
    void* pDebugBuff;              // pointer to debug buffer
    DD_IN DDU32 DebugBufAvailSize; // Available size of the debug buffer
    DD_IN_OUT DDU32 OutputSize;    // output size
} DD_DIAG_ARGS;

typedef struct _DD_TDR_DBG_FLIP_MPO
{
    DD_ARG_FLIP_MPO* pFlipMpoArg;              // OS MPO Arguments
    DDU32 PendingAddress[MAX_PLANES_PER_PIPE]; // Pending address for flip
    DDU32 LiveAddress[MAX_PLANES_PER_PIPE];    // Current live address in HW
    PIPE_ID PipeId;                            // Pipe enum
} DD_TDR_DBG_FLIP_MPO;

// ----------------------------------------------------------------------------
//
// TDR related data structures -- END
//
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//
// Data Structure for Interrupt operations -- START
//
// ----------------------------------------------------------------------------

typedef enum _DD_DOCK_STATE
{
    DOCK_OPENED = 0x0,
    DOCK_CLOSED = 0x1
} DD_DOCK_STATE;

// The structure is used by Miniport when retraining the link. If reprogramming of pipe
// is needed for the display to be driven properly, SoftBIOS will set the bit accordingly.
typedef struct _DD_LINK_TRAINING_PARAMS
{
    DD_IN DDU32 DisplayUid;                 // DisplayUID of device which generated the Short Pulse Interrupt.
    DD_IN DD_OUT DDU8 LinkTrainingMethod;   // LINK_TRAINING_METHOD
    DD_OUT DD_BOOL IsNeedPipeReprogramming; // Tells Whether PipeReprogramming is needed or not.
} DD_LINK_TRAINING_PARAMS;

typedef struct _SB_INTERRUPT_ARGS DD_INTERRUPT_ARGS;
// ----------------------------------------------------------------------------
//
// Data Structure for Interrupt operations -- END
//
// ----------------------------------------------------------------------------

#define PMIC_NAME L"PMICSPBTEST"
#define PMIC_SYMBOLIC_NAME L"\\DosDevices\\" PMIC_NAME

typedef struct _DD_SURFACE_DIMENSION_INFO
{
    DDU32 YWidth;
    DDU32 YHeight;
    DDU32 UVWidth;
    DDU32 UVHeight;
} DD_SURFACE_DIMENSION_INFO;

// Based on DP1.2 Spec. Total Number of Links is 15
#define DD_MAX_LINK_COUNT 15

// As every Address for a link requires 4 Bits, therefore total 14 links (MAX_LINK_COUNT - 1, since for 1st link RAD is not required) would require 56 bits.
// Hence total 7 Bytes
#define DD_MAX_BYTES_RAD ((DD_MAX_LINK_COUNT) / 2)
typedef struct _DD_DP_RELATIVE_ADDRESS
{
    DDU8 TotalLinkCount;

    // If TotalLinkCount is 1 then Relative Address should have zero value at all the indexes..

    // If the TotalLinkCount is Even then index from 0 till (TotalLinkCount/2 - 1) (apart from the Upper Nibble of last index) is a Valid Address, .

    // If the TotalLinkCount is Odd then index from 0 till (TotalLinkCount)/2 - 1) will be a Valid Address

    // Hence for both odd/even TotalLinkCount, we can use Index from 0 till (TotalLinkCount/2 - 1)

    DDU8 Address[DD_MAX_BYTES_RAD];
} DD_DP_RELATIVE_ADDRESS;

#define DD_MAX_AUX_BUFSIZE 0x0200

// RAD helper macros
#define IS_PORT_PHYSICAL(PORT_NUM) ((PORT_NUM) <= 0x7)
#define IS_PORT_LOGICAL(PORT_NUM) (((PORT_NUM) >= 0x8) && ((PORT_NUM) <= 0xF))

typedef enum _DD_AUX_I2C_OPERATIONS
{
    DD_OPERATION_UNKNOWN = 0,
    DD_NATIVE_AUX, // DPCD
    DD_I2C_AUX,
    DD_I2C,
    DD_ATOMICI2C,
    DD_REMOTE_DPCD,
    DD_I2C_MOT
} DD_AUX_I2C_OPERATIONS;

// Index size for DDC passed by escape call.
typedef enum _DD_ESC_I2C_INDEX_SIZE
{
    DD_ESC_I2C_INDEX_UNSPECIFIED = 0, // Choose Index size based on Index value
    DD_ESC_I2C_INDEX_BYTE,
    DD_ESC_I2C_INDEX_WORD,
    DD_ESC_I2C_INDEX_DWORD
} DD_ESC_I2C_INDEX_SIZE;
// DDC Speed override passed by escape call.
typedef enum _DD_NATIVE_I2C_SPEED
{
    DD_NATIVE_I2C_SPEED_AUTO = 0, // Use best possible Speed/method for current request
    DD_NATIVE_I2C_SPEED_100KHZ,   // Uses GMBUS
    DD_NATIVE_I2C_SPEED_50KHZ,    // Uses GMBUS
    DD_NATIVE_I2C_SPEED_BIT_BASH  // slowest speed, uses SW method to program gpio.
} DD_NATIVE_I2C_SPEED;

typedef struct _DD_I2C_AUX_ARGS
{
    DD_AUX_I2C_OPERATIONS Operation;
    DD_BOOL Write;        // 0-Read,1-Write
    DD_BOOL IsEscapePath; // This flag is set when we get call from  tool/apps which use escape/CUI SDK
    DD_PORT_TYPE Port;
    DD_TARGET_DESCRIPTOR TargetDescriptor;
    DDU32 Address;                     // 7bit I2c address or DPCD address
    DDU32 Index;                       // used in case of I2C [2byte Offset]
    DDU8 RelAddress[DD_MAX_BYTES_RAD]; // used only in case of Remote DPCD
    DDU32 ReadBytes;                   // Bytes to read only in case of Atomic I2C
    DDU32 WriteBytes;                  // Bytes to write only in case of Atomic I2C
    DDU32 DataLength;
    DDU8 Data[DD_MAX_AUX_BUFSIZE];
    DD_BOOL AccessOnI2cPinPair; // Access request on I2C PinPair filled in I2cPinPairIndex
    DDU8 I2cPinPairIndex;       // 1 based index for I2C pin pair, used in HAL
    DD_ESC_I2C_INDEX_SIZE I2cIndexSize;
    DD_NATIVE_I2C_SPEED I2cSpeed;
} DD_I2C_AUX_ARGS;

#define DSI_STD_MAJOR_VERSION 1
#define DSI_STD_MINOR_VERSION 0
#define DSI_SPEC_MAJOR_VERSION 1
#define DSI_SPEC_MINOR_VERSION 3
#define DSI_MAX_RETURN_PACKET_SIZE 32

typedef struct _DD_DSI_ARGUMENTS
{
    /*
      DataId represents the DataType of the DSI packet
      and the virtual channel of the peripheral
    */
    union
    {
        DD_IN DDU8 DataId;
        struct
        {
            DD_IN DDU8 DataType : 6;
            DD_IN DDU8 VirtualChannel : 2;
        };
    };

    /*
      This union shall contain Data0 and Data1 if it is a short packet
      or it will contain wordcount of the Payload if long packet write
    */
    union
    {
        struct
        {
            DD_IN DDU8 Data0;
            DD_IN DDU8 Data1;
        };
        DD_IN DDU16 LongWriteWordCount;
    };

    /*
      pPayload is an array of size 8 generally. If the transaction type
      is DCS read where long sink response is possible this payload size can
      be more than 8. We can get the exact size info from AllocatedBufferSize field.
    */
    DD_OUT DDU8* pPayload;
    struct
    {
        DD_IN DDU16 TransmissionMode : 2; // Transmission mode 0:LP; 1: HS
        DDU16 Reserved : 14;
    };
    /*
      This field needs to be updated with the read response bytecount from DSI sink
    */
    DD_OUT DDU16* pReadCount;
    /*
      This field indicates the size of the Payload array if it is a read transaction.
      it can be either 8 or more if read response is long.
    */
    DD_IN DDU16 AllocatedBufferSize;
} DD_DSI_ARGUMENTS;

typedef struct _DD_DSI_TRANSMISSION
{
    DD_IN DDU32 ChildUid;                 // ChildUid on which transmission is targetted.
    DD_IN DDU8 PacketCount;               // Count of the number of write/read packet transmissions to be done
    DD_OUT DDU8* pFailedPacket;           //  Index of the packet which failed if any.
    DD_IN_OUT DD_DSI_ARGUMENTS* pPackets; // Array of DD_DSI_ARGUMENTS whose size is PacketCount
} DD_DSI_TRANSMISSION;

typedef union _DD_RAD_BYTE_ST
{
    DDU8 Data;
    struct
    {
        DDU8 LowerRAD : 4;
        DDU8 HigherRAD : 4;
    };
} DD_RAD_BYTE_ST;

typedef enum UNDERRUN_ISR_ENABLE_REC
{
    UNDERRUN_DISABLED = 0,      /* Interrupt disabled*/
    UNDERRUN_ENABLE_FLIPS_ONLY, /* Enable Under-run ISR only for Flips */
    UNDERRUN_SETMODE_AND_FLIPS, /* Enable Under-run ISR during modeset and flips */
    UNDERRUN_START_2_END,       /* Enable Under-run ISR begning from CCD_Startmodeset and flips */
} UNDERRUN_ISR_ENABLE;

typedef enum _DD_HDCP_AUTH_STATUS
{
    DD_HDCP2_UNAUTHENTICATED = 1,
    DD_HDCP2_AUTH_IN_PROGRESS,
    DD_HDCP2_AUTH_COMPLETE,
    DD_HDCP2_LINKINTEGRITY_FAILED,
} DD_HDCP_AUTH_STATUS;

typedef struct _DD_HDCP_AUTH_DATA
{
    DD_HDCP_AUTH_STATUS HDCP2AuthStatus;                      // indicates the current status of Authentication
    DD_BOOL HDCPAuthenticationPass;                           // indicates whether HDCP Auth is successfully done for the display or not. Valid only if is bHDCPSupported TRUE
    DDU8 ReceiverId[RECEIVER_ID_SIZE];                        // 40 bit long receiver id extracted from CertRx, or BKSV in case of HDCP 1.4.
    DDU8 MajorVersion;                                        // The HDCP version 2.0 or 2.1
    DDU8 MinorVersion;                                        // The HDCP version 2.0 or 2.1
    DD_BOOL Repeater;                                         // indicating whether the Remote Display is Repeater or not
    DD_BOOL HasHDCP2_0_Repeater;                              // indicating whther the downstream topology has HDCP 2_0 Repeater
    DD_BOOL HasHDCP1_Device;                                  // indicating whther the downstream topology has HDCP 1 Device
    DDU32 RepeaterDeviceCount;                                // Number of Repeaters in the down stream. Valid only if bRepeater is TRUE, for HDCP 2.2 and 1.4
    DDU8 ReceiverIDList[MAX_HDCP_DEVICES * RECEIVER_ID_SIZE]; // List of Receiver Ids (2.2) or KSVs (1.4) each 40 bit long. Max number of repeaters can be 31 as per HDCP 2.0 spec, 127 for HDCP 1.4.
    // Valid only for Repeater.
    DDU8 TypeStatus;                                          // The last successful Type notification. Default value is 0
    DDU8 Depth;                                               // Repeater depth for HDCP 2.2 and 1.4
    DDU32 CapMask;                                            // Cached Caps Mask DD_HDCP_CAP_MASK
    DDU32 ProtectionLevel;                                    // Cached Level CP_PROTECTION_LEVEL_HDCP
} DD_HDCP_AUTH_DATA;

typedef enum
{
    DD_HDCPSTATUS_LINKINTEGRITY, // To pass link integrity state to KMD
    DD_HDCPSTATUS_AUTH_PROGRESS, // To indicates if authentication is in prgress
    DD_HDCPSTATUS_AUTH_DATA      // To pass HDCP 2.0 authentication data
} DD_HDCP_STATUS_TYPE;

typedef enum _DD_HDCP_MSG_CMD
{
    DD_HDCP_MSG_CMD_NULL = 0x0000, // Does no command
    DD_HDCP_MSG_CMD_READ = 0x0001, // Reads Data from an HDCP Bus Device
    DD_HDCP_MSG_CMD_WRITE = 0x0002, // Writes Data out on the HDCP Bus Device
} DD_HDCP_MSG_CMD;

#define MAX_HDCP_MSG_SIZE 534    // Max message size in HDCP 2.2
typedef struct _DD_HDCP_MSG_ARGS // merge with DD_I2C args
{
    DDU32 ChildUid;
    DD_PORT_TYPE Port; //
    DD_HDCP_MSG_CMD Command;
    DDU32 DataSize;
    DDU32 DataProcessed;
    DDU8* pBuffer;
} DD_HDCP_MSG_ARGS;

typedef struct _DD_HDCP_RX_STATUS_ARGS
{
    DD_PORT_TYPE Port;
    DDU32 Protocol;
    DD_BOOL IsLinkIntegrityFailed;
    DD_BOOL IsReAuthRequired;
    DD_BOOL RxReady;
} DD_HDCP_RX_STATUS_ARGS;

typedef struct _DD_HDCP_AUTH_STATUS_ARGS
{
    DDU32 ChildUid;
    DD_TARGET_DESCRIPTOR TgtDesc;
    DD_PIPE_DETAILS PipeDetails;
    DD_HDCP_AUTH_DATA AuthData;
} DD_HDCP_AUTH_STATUS_ARGS;

typedef enum _DD_HDCP_2_ADAPTATION
{
    INVALID_HDCP_2_ADAPTATION = 0,
    HDCP2_2_HDMI_ADAPTATION,
    HDCP2_2_DP_ADAPTATION,
    HDCP_2_2_IF_IND_ADAPTATION, // Interface Independent Adaptation used for WiGig and WiDi
    HDCP_2_2_EXT_SOLUTION,      // LSPCON, CPDP kind of external solutions

    MAX_HDCP_2_ADAPTATION
} DD_HDCP2_ADAPTATION;

typedef struct _DD_HDCP_SRVC_REQUEST
{
    DDU32 ChildUid; // Unique display ID (cannot accept generic types)
    DD_PORT_TYPE Port;
    PIPE_ID PipeId;
    DD_HDCP2_ADAPTATION ProtocolAdaptation; // Which HDCP adaptation protocol to be used
    DD_BOOL RepeaterAuth;
    DDU32 Level;
    DDU32 StreamCount;
    DDU16 StreamIds[MAX_HDCP_STREAM_NUM];
} DD_HDCP_SRVC_REQUEST;

typedef struct _DD_NOTIFY_OPM_ARGS
{
    DD_BOOL IsAudioTrigger;
    DDU32 DisplayUID;
    DD_BOOL IsDisplayAttached;
    DDU32 PathIndex;
} DD_NOTIFY_OPM_ARGS;

typedef struct _DD_GET_VIDPN_DETAILS
{
    DD_BOOL QueryBasedOnPipe;
    PIPE_ID PipeId;
    DDU32 DisplayUID;
    DDU32 PathIndex;
} DD_GET_VIDPN_DETAILS;

// PERIODIC FRAME NOTIFY related
// Maintain a sorted ring of elements
#define DD_MAX_PERIOIDIC_FRAME_ELEMENTS 10 // Per MSFT it should be 8, adding 1 for VRR and 1 for FlipQ

typedef enum _DD_PFN_ELEMENT_TYPE
{
    DD_PFN_ELEMENT_INVALID,
    DD_PFN_ELEMENT_OS,
    DD_PFN_ELEMENT_FLIP_Q,
    DD_PFN_ELEMENT_RR_SWITCHING,
    DD_PFN_ELEMENT_DC_BALANCING
} DD_PFN_ELEMENT_TYPE;

typedef struct _DD_PERIODIC_FRAME_DATA
{
    DD_PFN_ELEMENT_TYPE ElementType;
    DDU32 Scanline;       // Scanline number for trigger
    DDU32 NotificationId; // ID to return to OS

    // Each element needs to contain the TargetID within it since during DxgkDdiDestroyPeriodicFrameNotification,
    // we get only the pointer to the Element to be destroyed, and no separate TargetId
    DDU32 VidPnTargetId; // VidPnTargetId to return to OS.
} DD_PERIODIC_FRAME_DATA;

typedef struct _DD_PERIODIC_FRAME_ELEM
{
    DD_PERIODIC_FRAME_DATA Data;
    struct _DD_PERIODIC_FRAME_ELEM* pPrevElem; // Previous element in ring
    struct _DD_PERIODIC_FRAME_ELEM* pNextElem; // Next element in ring
} DD_PERIODIC_FRAME_ELEM;

typedef struct _DD_PERIOIDIC_FRAME_CURRENT_ELEM_PARAMS
{
    DD_PERIODIC_FRAME_ELEM* pCurrent;     // The current element in Ring that will trigger the next interrupt
    DD_BOOL Invalid;                      // If set, it means that Current Elem was Destroyed and pCurrent points to Current Element's next Element
    DDU32 ScanlineOfDestroyedCurrentElem; // To be looked at only if Invalid = 1,
    // since in that case pCurrent will point to pCurrent->pNextElem, original Current Element has been Destroyed on OS Request
} DD_PERIOIDIC_FRAME_CURRENT_ELEM_PARAMS;

typedef struct _DD_PERIODIC_FRAME_RING
{
    DD_PERIODIC_FRAME_ELEM* pHead; // The "start" of the ring, really the element with min scanline
    DDU8 NumElem;                  // Number of elements in Ring
} DD_PERIODIC_FRAME_RING;

typedef struct _DD_PERIODIC_FRAME_ELEM_CREATE_ARGS
{
    DDU32 TargetId;    // [in] The output that the notification will be attached to
    DDU64 TimeIn100ns; // [in] Represents an offset before the VSync.
    // The Time value may not be longer than a VSync interval.
    // In units of 100ns.
    DDU32 NotificationId; // [in] Id that represents this instance of the notification
    // used to identify which interrupt has fired.
    HANDLE hNotification; // [out] Handle to the notification object, later used to destroy
} DD_PERIODIC_FRAME_CREATE_ARGS;

typedef struct _DD_ACPI_DSM_INOUT_ARGS
{
    DD_IN DDU32 DsmFuncCode;          // DSM function code
    DD_IN DDU32 Length;               // DSM size of output buffer
    DD_IN DDU32 AcpiInputSize;        // DSM input argument size
    DD_IN void* pAcpiInputArgument;   // DSM input argument
    DD_IN DDU32 AcpiOutputSize;       // DSM output argument size
    DD_OUT void* pAcpiOutputArgument; // DSM output argument
} DD_ACPI_DSM_INOUT_ARGS;

typedef struct _DD_ACPI_BCL_INOUT_ARGS
{
    DD_IN DDU32 TargetId; // DSM input argument size
} DD_ACPI_BCL_INOUT_ARGS;

typedef enum _DD_ACPI_METHOD
{
    ACPI_METHOD_DOD = 0,
    ACPI_METHOD_BCL,
    ACPI_METHOD_DSM,
    ACPI_METHOD_DOS,
    ACPI_METHOD_DDC
} DD_ACPI_METHOD;

typedef struct _DD_ACPI_DOS_INOUT_ARGS
{
    DD_IN DDU32 DosArgValue;
    DD_IN DDU32 TargetId;
} DD_ACPI_DOS_INOUT_ARGS;

typedef struct _DD_ACPI_DDC_INOUT_ARGS
{
    DD_IN DDU32 TargetId;
    DD_IN DDU32 DdcArgValue;
    DD_OUT DD_BOOL IsEdidBufferReturned;
} DD_ACPI_DDC_INOUT_ARGS;

typedef struct _DD_ACPI_EVAL_METHOD_INOUT_ARGS
{
    DD_IN DD_ACPI_METHOD MethodName;
    union
    {
        DD_IN DD_ACPI_DSM_INOUT_ARGS DsmInOutArgs; // DSM input args
        DD_IN DD_ACPI_BCL_INOUT_ARGS BclInOutArgs; // Backlight Control Method input args
        DD_IN DD_ACPI_DOS_INOUT_ARGS DosInOutArgs; // DOS Method Input args
        DD_IN DD_ACPI_DDC_INOUT_ARGS DdcInOutArgs; // DDC Method Input/Output Args
    };

    DD_IN_OUT DDU32 Count;      // OUT param when pass 1, IN param when pass 2
    DD_OUT DDU32 Length;        // Valid in pass 2 case to calculate size of output buffer
    DD_IN_OUT void* pDataArray; // NULL when pass 1, non-NULL when pass 2
} DD_ACPI_EVAL_METHOD_INOUT_ARGS;

typedef enum _DD_ACPI_QUERY_TYPE
{
    GET_HPD_COUNT,
    DECREMENT_HPD_COUNT
} DD_ACPI_QUERY_TYPE;

typedef struct _DD_ACPI_QUERY_ARGS
{
    IN DD_ACPI_QUERY_TYPE QueryType;
    IN DDU32 DdiNum;
    IN_OUT DDU32 HpdCount;
} DD_ACPI_QUERY_ARGS;

typedef struct _DD_DSM_DECREMENT_HPD_INPUT_PARAM
{
    DD_IN DDU32 DdiNum;   // 0 - Invalid port No., N - TBT port no.
    DD_IN DDU32 HpdCount; // Total No. of HPDs that had happened on specified TBT port
} DD_DSM_DECREMENT_HPD_INPUT_PARAM;

typedef enum _DD_DSM_CB_TYPE
{
    DD_BIOS_DATA_FUNC_SUPPORT = 0x00, // function is supported
    DD_SYSTEM_BIOS_ADAPTER_POWER_STATE_NOTIFICATION = 0x01,
    DD_SYSTEM_BIOS_DISPLAY_POWER_STATE_NOTIFICATION = 0x02,
    DD_SYSTEM_BIOS_POST_COMPLETION_NOTIFICATION = 0x03,
    DD_SYSTEM_BIOS_PRE_HIRES_SET_MODE = 0x04,
    DD_SYSTEM_BIOS_POST_HIRES_SET_MODE = 0x05,
    DD_SYSTEM_BIOS_SET_DISPLAY_DEVICE_NOTIFICATION = 0x06,
    DD_SYSTEM_BIOS_SET_BOOT_DEVICE_PREFERENCE = 0x07,
    DD_SYSTEM_BIOS_SET_PANEL_PREFERENCE = 0x08,
    DD_SYSTEM_BIOS_FULL_SCREEN_DOS = 0x09,
    DD_SYSTEM_BIOS_APM_COMPLETE = 0x0A,
    DD_SYSTEM_BIOS_PLUG_UNPLUG_AUDIO = 0x0B,
    DD_SYSTEM_BIOS_CDCLOCK_CHANGE_NOTIFICATION = 0x0C,
    DD_SYSTEM_BIOS_GET_BOOT_DISPLAY_PREFERENCE = 0x0D,
    DD_SYSTEM_BIOS_GET_PANEL_DETAILS = 0x0E,
    DD_SYSTEM_BIOS_INTERNAL_GRAPHICS = 0x0F,
    DD_SYSTEM_BIOS_GET_AKSV = 0x10,
    DD_SYSTEM_BIOS_ENABLE_S0IX_HPD = 0x11,
    DD_SYSTEM_BIOS_GET_HPD_COUNT = 0x12,
    DD_SYSTEM_BIOS_DECREMENT_HPD_COUNT = 0x13,
    DD_SYSTEM_BIOS_DP_IN_SWITCH_NOTIFICATION = 0x14,
    DD_SYSTEM_BIOS_LFP_DISPLAY_MUX_SWITCH = 0x15,
    DD_SYSTEM_BIOS_LID_SWITCH_OP = 0x16,
    DD_BIOS_DATA_RESERVED // LAST ENTRY
} DD_DSM_CB_TYPE;

// Opregion defined error messages
typedef enum _OPREGION_NRDY_REASON
{
    ACPI_FAILURE_DRIVER_NOT_INITIALIZED = 0x00,
    ACPI_FAILURE_3D_APP_RUNNING,
    ACPI_FAILURE_OVERLAY_ACTIVE,
    ACPI_FAILURE_FSDOS_ACTIVE,
    ACPI_FALIURE_RESOURCE_IN_USE,
    ACPI_FALIURE_DRIVER_IN_LOW_POWER_TRANSITION,
    ACPI_FAILURE_EXTENDED_DESKTOP_ACTIVE,
    ACPI_FAILURE_FATAL,

    // Don't add driver defined err messages here!

    // Driver defined error messages
    ACPI_FAILURE_LVDS_PWR_STATE_CHANGE_FAILED = 0x101,
    ACPI_FAILURE_NO_CHANGE_IN_CONFIG,
    ACPI_FAILURE_GET_NEXT_CONFIG_FROM_EM_FAILED,
    ACPI_FAILURE_GET_EM_HOTKEY_LIST_FAILED,
    ACPI_FAILURE_TURN_OFF_ALL_DISPLAYS,
    ACPI_FAILURE_GET_DISP_INFO_FAILED,
    ACPI_FAILURE_INVALID_ASL_NOTIFICATION,
    ACPI_FAILURE_INVALID_BUFFER_SIZE,
    ACPI_FAILURE_EM_NOT_INITIALIZED,
    ACPI_FAILURE_TMM_ACTIVE
} OPREGION_NRDY_REASON;

// Color related
typedef enum _DD_COLOR_BLENDING_MODE
{
    DD_COLOR_BLENDING_MODE_UNKNOWN = 0, // Unknown or unsupported blending with given plane configurations
    DD_COLOR_BLENDING_MODE_SRGB_NONLINEAR = 1, // Legacy blending
    DD_COLOR_BLENDING_MODE_2020RGB_LINEAR = 2, // BT2020 and HDR plane blending
    DD_COLOR_BLENDING_MODE_2020RGB_NONLINEAR = 3, // BT2020 and HDR plane blending
    DD_COLOR_BLENDING_MODE_SRGB_LINEAR = 4, // Cases (rare) where blending in BT2020 space is not possible
    DD_COLOR_BLENDING_MODE_MAX
} DD_COLOR_BLENDING_MODE;

typedef enum _DD_CSC_RANGE_CONVERSION_TYPE
{
    DD_CSC_RANGE_CONVERSION_TYPE_NONE = 0,
    DD_CSC_RANGE_CONVERSION_RGB_LR_TO_RGB_FR, // LR: Limited Range, FR:Full Range
    DD_CSC_RANGE_CONVERSION_RGB_FR_TO_RGB_LR,
    DD_CSC_RANGE_CONVERSION_RGB_FR_TO_YCBCR_LR,
    DD_CSC_RANGE_CONVERSION_RGB_FR_TO_YCBCR_FR,
    DD_CSC_RANGE_CONVERSION_YCBCR_LR_TO_RGB_FR,
    DD_CSC_RANGE_CONVERSION_YCBCR_FR_TO_RGB_FR,
    DD_CSC_RANGE_CONVERSION_RGB_FR_TO_RGB_FR,
    DD_CSC_RANGE_CONVERSION_YCBCR_LR_TO_YCBCR_FR,
    DD_CSC_RANGE_CONVERSION_YCBCR_FR_TO_YCBCR_LR
} DD_CSC_RANGE_CONVERSION_TYPE;

typedef struct _DD_COLOR_PIPE_CFG
{
    DDU32 TargetId;
    DDU32 SupportedPanelFeatures; // If there is a panel connected to current pipe. It is Read only.
    DDU32 ActivePanelFeatures;    // If there is a panel connected to current pipe. It is Read only.
    DD_COLOR_BLENDING_MODE BlendingMode;
    DD_RGB_DDU8_24 PipeBottomColor;
    DD_COLOR_PIXEL_DESC BlendingColorFormat;
    DD_COLOR_PIXEL_DESC OutputColorFormat;
} DD_COLOR_PIPE_CFG;

// Reg keys for persistence
typedef enum _DD_COLOR_REG_KEY
{
    DD_COLOR_MODEL_REG_KEY,
    DD_CUSTOM_INFOFRAME_REG_KEY,
    DD_3DLUT_STATUS_REG_KEY,
    DD_QUANTIZATION_REG_KEY
} DD_COLOR_REG_KEY;

// VBT/Opregion PowerCons Data

// Opregion Set Event
typedef enum _DD_OPREGION_SET_EVENT
{
    OPREGION_INDICATE_DRIVER_READY = 0,
    OPREGION_NOTIFY_DRIVER_STATUS,
    OPREGION_NOTIFY_STS_FAILURE_REASON,
    OPREGION_SET_ASL_SLEEP_TIMEOUT,
    OPREGION_SET_SUPPORTED_DISPLAYS,
    OPREGION_SET_BLC,
    OPREGION_SET_PFMB,
    OPREGION_SET_BRIGHTNESS_LFP1,
    OPREGION_SET_BRIGHTNESS_LFP2,
    OPREGION_SET_VRAM_SR_STATE,
} DD_OPREGION_SET_EVENT;

typedef enum _OPREGION_NOTIFY_STATUS
{
    NOTIFY_SUCCESS = 0x00,
    NOTIFY_FAILURE,
    NOTIFY_PENDING,
    NOTIFY_DISPATCHED
} OPREGION_NOTIFY_STATUS;

typedef enum _DD_OPREGION_EVENT
{
    OPREGION_EVENT_UNKNOWN = 0,
    OPREGION_HOTKEY_EVENT,
    OPREGION_LID_EVENT,
    OPREGION_DOCK_EVENT,
} DD_OPREGION_EVENT;

typedef struct _DD_GET_LID_DOCK_STATUS
{
    OPREGION_NOTIFY_STATUS OpRegNotifyStatus;
    DD_OPREGION_EVENT OpregionEvent;
    DD_BOOL IsLidOpen; // Lid Status
    DD_BOOL IsDocked;  // Dock Status
} DD_GET_LID_DOCK_STATUS;

typedef struct _DD_SET_OPREGION_EVENT
{
    DD_OPREGION_SET_EVENT OpregionEvent;
    DDU32 EventValue; // Value to write in Opregion, varies between every Set Event type
} DD_SET_OPREGION_EVENT;

typedef enum _DD_DPS_PANEL_TYPE
{
    // For static DRRS panels, driver performs a mode set operation and switches the active RR to lowest,
    // while switching to DC mode.
    DPS_PANEL_TYPE_STATIC_DRRS = 0,

    // Dynamic Display Power Optimization (de-featured).
    DPS_PANEL_TYPE_D2PO,

    // For seamless DRRS panels, driver programs the lowest RR by changing the clock or VTotal programming,
    // on VBI off notification.
    DPS_PANEL_TYPE_SEAMLESS_DRRS,

    // No DRRS.
    DPS_PANEL_TYPE_UNSUPPORTED,
} DD_DPS_PANEL_TYPE;

#define DD_MAX_BRIGHTNESS_PERCENT 100

// Nits brightness related
#define DD_DEFAULT_NITS_OPTIMIZATION_STRENGTH 6
#define DD_MAX_NIT_RANGES 16

typedef struct _DD_DISPLAY_SINK_BRIGHTNESS_CAPS
{
    DD_BOOL IsNitsBrightnessControlSupported;
    DD_BOOL IsAuxModeOnlySupportedForBrightnessControl;
    DD_BOOL IsBrightnessOptimizationSupported; // Scenario based brightness control - RS5
    DD_BOOL IsVesaBasedBrightnessControl;
    DDU16 SinkDesiredMaxFall; // Max frame average luminance level in Nits
    DDU16 SinkDesiredMinCll;  // Min content luminance level in Nits
    DDU16 SinkDesiredMaxCll;  // Max content luminance level in Nits
    DDU32 MaxPanelBrightness; // Max OPR100 panel brightness including boost range in Nits
} DD_DISPLAY_SINK_BRIGHTNESS_CAPS;

typedef struct _DD_RANGE_DETAILS
{
    DDU32 MinBrightnessInMilliNits; // This will be in milli nits
    DDU32 MaxBrightnessInMilliNits; // This will be in milli nits
    DDU32 StepSizeInMilliNits;      // Individual step size for reaching from min to max
} DD_RANGE_DETAILS;

typedef struct _DD_NIT_RANGES
{
    DDU32 NormalRangeCount;
    DDU32 TotalRangeCount;
    DDU32 PreferredMaxBrightness;
    DD_RANGE_DETAILS RangeDetails[DD_MAX_NIT_RANGES];
} DD_NIT_RANGES;

typedef union _DD_LACE_DATA
{
    DDU32 LaceData; // LACE data
    struct
    {
        DDU32 DisplayLaceEnabled : 1;             // Display LACE default enabled status
        DDU32 DisplayLaceSupported : 1;           // Display LACE Supported ?
        DDU32 DisplayLaceAggressivenessLevel : 8; // Display LACE Aggressiveness Level Profile.
        DDU32 DisplayLaceReserved : 22;           // Reserved
    };
} DD_LACE_DATA;

typedef struct _DD_COMMON_PC_FEATURES
{
    DD_BOOL IsRMPMEnabled;        // Rapid Memory Power Management (RMPM)
    DD_BOOL IsFBCEnabled;         // Frame Buffer Compression (Smart 2D Display Technology; S2DDT)
    DD_BOOL IsRSSupported;        // Graphics Render Standby (RS)
    DD_BOOL IsTurboBoostEnabled;  // Turbo Boost enable
    DD_BOOL IsDFPSEnabled;        // Dynamic Frames Per Second (DFPS)
    DD_BOOL IsWakeOnHPDSupported; // This feature enables HPD events like Hotplug/unplug as a wake up source from S0ix/DC9 in supported platforms
} DD_COMMON_PC_FEATURES;

typedef enum _DD_CONNECTOR_TYPE
{
    DD_CONNECTOR_NONE,
    DD_CONNECTOR_EDP,
    DD_CONNECTOR_DP,
    DD_CONNECTOR_HDMI,
    DD_CONNECTOR_DVI,
    DD_CONNECTOR_MIPI
} DD_CONNECTOR_TYPE;

typedef struct _DD_CONNECTOR_INFO
{
    DD_CONNECTOR_TYPE SupportedConnectors;
    DD_VIDEO_OUTPUT_TECHNOLOGY DongleDwnStreamPortType;
} DD_CONNECTOR_INFO;

typedef struct _DD_GET_CONNECTOR_INFO_ARGS
{
    DD_TARGET_DESCRIPTOR TargetDesc;
    DD_CONNECTOR_INFO ConInfo;
    DD_BOOL IsOnboardHdmi21PconPresent;

} DD_GET_CONNECTOR_INFO_ARGS;

typedef enum _DD_GT_PC_NOTIFY_EVENT
{
    DD_MODE_CHANGE_EVENT,
    DD_PWR_SRC_EVENT,
    DD_FLIP_DONE_EVENT,
    DD_FLIP_EVENT,
    DD_VRR_EVENT
} DD_GT_PC_NOTIFY_EVENT;

typedef enum _OPREGION_DRDY_STATUS
{
    DRIVER_NOT_READY = 0,
    DRIVER_READY
} OPREGION_DRDY_STATUS;

typedef enum _DD_PWR_SRC_EVENT_ARGS
{
    DD_PWR_UNKNOWN, // Set to unknown until power source is queried in DxgkDdiStartDevice call path
    DD_PWR_AC,
    DD_PWR_DC
} DD_PWR_SRC_EVENT_ARGS;

typedef struct _DD_2DREGION
{
    DDU32 Cx;
    DDU32 Cy;
} DD_2DREGION;

typedef struct _DD_TIMING_INFO_ARGS
{
    DDU64 DotClock;       // Pixel clock in Hz
    DDU32 HTotal;         // Horizontal total in pixels
    DDU32 VTotal;         // Vertical total in lines
    DDU32 VRoundedRR;     // Refresh Rate
    DD_BOOL IsInterlaced; // 1 = Interlaced Mode
} DD_TIMING_INFO_ARGS;

typedef struct _DD_MODE_EVENT_CONFIG
{
    DDU32 TargetId;
    PIPE_ID PipeID;
    DD_2DREGION SourceMode;
    DD_TIMING_INFO_ARGS TimingInfoArgs;
} DD_MODE_EVENT_CONFIG;

typedef struct _DD_MODE_CHANGE_EVENT_ARGS
{
    DD_BOOL PreModeSet;
    DDU32 PathCount;
    DD_MODE_EVENT_CONFIG Config[MAX_PHYSICAL_PIPES];
} DD_MODE_CHANGE_EVENT_ARGS;

typedef struct _DD_FLIP_INFO_ARGS
{
    DDU32 TargetId;
    PIPE_ID PipeId;
    DD_BOOL IsFlipDone;
    DDU8 BitWiseSyncPlane;  // Planes with Sync Flip
    DDU8 BitWiseAsyncPlane; // Planes with Async flip
} DD_FLIP_INFO_ARGS;

typedef struct _DD_VRR_INFO_ARGS
{
    DD_BOOL VrrEnable;
    DDU8 Pipe;
    DDU32 TargetId;
} DD_VRR_INFO_ARGS;

typedef struct _DD_GT_PC_NOTIFY_EVENT_ARGS
{
    DD_GT_PC_NOTIFY_EVENT EventType;
    union
    {
        DD_MODE_CHANGE_EVENT_ARGS DisplayModeChangeEventArgs;
        DD_PWR_SRC_EVENT_ARGS PowerSourceEventArgs;
        DD_FLIP_INFO_ARGS FlipInfoArgs;
        DD_VRR_INFO_ARGS VrrInfoArgs;
    };
} DD_GT_PC_NOTIFY_EVENT_ARGS;

#define DD_IS_VIRTUAL_TARGET(DisplayID) ((DisplayID & 0xF) == DD_PORT_TYPE_VIRTUAL_PORT)
#define DD_IS_WRITEBACK_TARGET(DisplayID) ((DisplayID & 0xF) == DD_PORT_TYPE_WRITEBACK_PORT)
#define DD_IS_DYNAMICPORT_TARGET(DisplayID) ((DisplayID & 0xF) == DD_PORT_TYPE_DYNAMIC_PORT)
#define DD_GET_VIRTUAL_PIPE(DisplayID) ((DisplayID & 0xF00) >> 8)
#define DD_MAX_CURSOR_BUFFERS_PER_VIEW 3

typedef struct _DD_CURSOR_BUFFER_INFO
{
    void* pCursorBufferLinearAddress;
    void* pGmmBlock;
    DDU32 BaseAddress;
    DD_BOOL Allocated;
} DD_CURSOR_BUFFER_INFO;

typedef struct _DD_CURSOR_ATTRIB
{
    IN DDU32 CursorWidth;                  // Current cursor width
    IN DDU32 CursorHeight;                 // Current cursor height
    IN DDU32 CursorPitch;                  // Pitch of Cursor.
    IN DDU32 BitsPerPixel;                 // Bits per pixel for this cursor
    IN DDU32 BaseAddress;                  // Needed to trigger the update.
    IN PLANE_ORIENTATION PlaneOrientation; // ORIENTATION_DEFAULT,ORIENTATION_0 = ORIENTATION_DEFAULT,ORIENTATION_180
    IN DDU32 CursorSize;
} DD_CURSOR_ATTRIB;

typedef struct _DD_CURSOR_INFO
{
    IN DD_POINTER_FLAGS CursorFlags; // use these flags to avoid b-spec violations
    // ex - 256x256 32bpp ARGB (not available for VGA use)
    DD_CURSOR_ATTRIB CursorAttrib;
    DD_CURSOR_BUFFER_INFO CursorBufferInfo[DD_MAX_CURSOR_BUFFERS_PER_VIEW];
    DD_BOOL ShapeChanged;
    DDS32 XPos;
    DDS32 YPos;
} DD_CURSOR_INFO;

typedef struct _DD_CHECK_LEGACY_OVL_SUPPORT_ARG
{
    DD_IN PIPE_ID PipeId;  // Pipe on which OVL is to be enabled
    DD_IN DDU32 SurfSizeX; // OVL Details
    DD_IN DDU32 SurfSizeY;
    DD_IN DD_PIXELFORMAT PixelFormat;
    DD_IN DD_SURFACE_MEMORY_TYPE SurfMemType;
    DD_OUT DD_BOOL Supported; // Whether OVL can be supported or not
} DD_CHECK_LEGACY_OVL_SUPPORT_ARG;

typedef struct _DD_PROGRAM_LEGACY_OVL_ARG
{
    DD_BOOL Enable;        // Enable or Disable OVL
    PIPE_ID PipeId;        // Pipe on which OVL is to be programmed
    DDU8 BaseHwPlaneId;    // Plane where keying is to be programmed
    DDU8 OverlayHwPlaneId; // Plane where WM/DBUF needs to be set up
    DDU32 SurfSizeX;       // OVL details used for WM/DBUF calculation
    DDU32 SurfSizeY;
    DD_PIXELFORMAT PixelFormat;
    DD_SURFACE_MEMORY_TYPE SurfMemType;
    DD_ROTATION PlaneOrientation;
    DD_BOOL Scale;
    DD_BOOL ProgramDestKey; // Whether dest key needs to be programmed
    DDU32 DestKeyValue;     // dest key details
    DDU32 DestKeyMask;
    DD_OUT DD_BOOL PipeCscEnabled; // Pipe Color details to be used by Render for OVL color programming
    DD_OUT DD_BOOL PipeGammaEnabled;
    DD_OUT DD_COLOR_BLENDING_MODE BlendingMode;
} DD_PROGRAM_LEGACY_OVL_ARG;

typedef struct _DD_WAIT_FOR_VBLANK_ARG
{
    PIPE_ID PipeId;
    DD_BOOL ThreadSleep;
} DD_WAIT_FOR_VBLANK_ARG;

typedef struct _DD_LOCK_PLANE_UPDATES_FOR_LEGACY_OVERLAY_ARG
{
    PIPE_ID PipeId;
    DD_BOOL Lock;          // TRUE = Lock, FALSE = Unlock
    DDU8 BaseHwPlaneId;    // Base plane to be locked
    DDU8 OverlayHwPlaneId; // OVL plane to be locked
} DD_LOCK_PLANE_UPDATES_FOR_LEGACY_OVERLAY_ARG;

typedef struct _DD_GET_DISPLAY_DETAILS
{
    DD_IN DDU32 SourceId; // Below Display details to be provided for given Source Id
    DDU32 NumPipes;       // Config. needs more than 1 pipe (e.g. Tiled display)
    PIPE_ID PipeId[MAX_PIPES];
    DD_BOOL HDRMode;
    DDU32 SourceModeX; // View (and not per pipe) Source Mode
    DDU32 SourceModeY;
    DDU32 SourceModeBpp;
    DDU32 VRoundedRR;           // Pipe RR (should be same across pipes)
    DD_ROTATION Rotation;       // View Rotation
    DDU32 CurrentDisplayConfig; // Current Display config
} DD_GET_DISPLAY_DETAILS;

typedef struct _DD_TARGET_DATA
{
    DDU32 TargetId; // Valid VidPn target id, NULL if invalid
    PLANE_TYPE PlaneId;
    PIPE_ID PipeId;
    DDU32 XRes;
    DDU32 YRes;
    DDU32 VRefresh;
    DDU32 HRefresh;
    DDU16 RefRateNumerator;
    DDU32 RefRateDenominator;
    // SCANLINE_ORDER                      ScanLineOrder;
    DDU16 RotationAngle;
    DD_BOOL LowResMode;
} DD_TARGET_DATA;

typedef struct _DD_SOURCE_MODE_DETAILS
{
    DDU32 XRes;
    DDU32 YRes;
    DDU8 BitsPerPixel;
    void* pGMMBlock;
    SOURCE_MODE_FLAGS SrcModeFlags;
} DD_SOURCE_MODE_DETAILS;

typedef struct _DD_GET_DISPLAY_DETAILS_FROM_SRC_ID
{
    IN DDU32 VidPnSrcId;                               // VidPn source id
    OUT DDU32 TargetNum;                               // Number of Targets per VidPNSource
    OUT DD_TARGET_DATA TargetData[MAX_PHYSICAL_PIPES]; // Target Info array
    OUT DD_SOURCE_MODE_DETAILS SourceModeInfo;         // Source Mode Information
    OUT DD_BOOL InHDRMode;                             // HDR status
} DD_GET_DISPLAY_DETAILS_FROM_SRC_ID;

typedef struct _DD_GET_DISPLAY_DETAILS_FROM_TARGET_ID
{
    DD_IN DDU32 TargetId; // Below Display details to be provided for given Source Id
    PIPE_ID PipeId;       // First Pipe
    DDU32 SrcID;
} DD_GET_DISPLAY_DETAILS_FROM_TARGET_ID;

typedef struct _DD_GET_DISPLAY_CONFIG
{
    DDU32 CurrentDisplayConfig; // Current Display config
    DDU32 TargetID[MAX_POSSIBLE_PIPES];
    DDU32 NumTargets;
} DD_GET_DISPLAY_CONFIG;

typedef enum _DD_DSB_SELECTOR
{
    DD_DSB_SELECTOR_NONE = -1,
    DD_DSB_SELECTOR_PIPE,                  // The General purpose pipe DSB
    DD_DSB_SELECTOR_LACE_AND_HDR_METADATA, // DSB dedicated to LACE and HDR Metadata
    DD_DSB_SELECTOR_3DLUT_AND_GAMMA_LUT,   // DSB dedicated for 3DLUT and Gamma LUT
    DD_DSB_SELECTOR_MAX
} DD_DSB_SELECTOR;

typedef enum _DD_DSB_TRIGGER_MODE
{
    DD_DSB_TRIGGER_MODE_NONE = 0,
    DD_DSB_TRIGGER_MODE_SYNC, // Call waits for he DSB execution to be complete
    // Multiple Async Options
    DD_DSB_TRIGGER_MODE_ASYNC,    // Call returns before the execution is complete. Next call will wait for first DSB execution to complete
    DD_DSB_TRIGGER_MODE_OVERRIDE, // Advanced Async type which supports clearing an already queued Async request and put a new one in place
    DD_DSB_TRIGGER_MODE_APEND,    // Advanced Async type which supports Apend to an already queued Async request // TBD needs more state management

    // Keep this enum at the end
    DD_DSB_TRIGGER_MODE_MAX
} DD_DSB_TRIGGER_MODE;

typedef enum _DD_DSB_WAIT_MODE
{
    DD_DSB_WAIT_FOR_NONE = 0,
    DD_DSB_WAIT_FOR_VBLANK,
    DD_DSB_WAIT_FOR_U_SEC,
    DD_DSB_WAIT_FOR_SCANLINES,
    DD_DSB_WAIT_FOR_SCANLINE_IN_RANGE,
    DD_DSB_WAIT_FOR_SCANLINE_OUT_OF_RANGE,
    DD_DSB_WAIT_FOR_POLL_REG,
    // Start of Pre-fetch waits.
    DD_DSB_PRE_FETCH_WAIT_FOR_VBLANK = 0x100,
    DD_DSB_PRE_FETCH_WAIT_FOR_SCANLINE_RANGE,
} DD_DSB_WAIT_MODE;

typedef struct _DD_DSB_WAIT_CONFIG
{
    DD_BOOL InterruptOnCompletion; // 1 - enabled
    DD_DSB_WAIT_MODE WaitMode;     // 0 - No Wait, 1 - Wait for VBlank, 2 Wait for scanline // DSB_WAIT_MODE
    union
    {
        DDU32 VBlanks;   // The number of VBlanks to be waited for
        DDU32 TimeInUs;  // The number of scan lines to be waited for
        DDU32 ScanLines; // The number of scan lines to be waited for
        union
        {
            struct
            {
                DDU32 ScanLineRangeStart; // The Start scan line to be waited for
                DDU32 ScanLineRangeEnd;   // The End scan line to be waited for
            };
            DDU64 ScanLineRange;
        };
        struct
        {
            DDU32 PollOffset;
            DDU32 PollValue;
            DDU32 PollMask;
        };
    };
} DD_DSB_WAIT_CONFIG;

typedef struct _DD_OFFSET_DATA_PAIR
{
    DDU32 Data;
    DDU32 Offset;
} DD_OFFSET_DATA_PAIR;

#define DD_UPDATE_OFFSET_DATA_PAIR(pOffsetDataPair, pRegisterCount, RegOffset, RegData) \
    {                                                                                   \
        pOffsetDataPair[*pRegisterCount].Offset = RegOffset;                            \
        pOffsetDataPair[*pRegisterCount].Data   = RegData;                              \
        (*pRegisterCount)++;                                                            \
    }

typedef struct _DD_DSB_TRIGGER_ARGS
{
    PIPE_ID PipeId;
    DD_DSB_SELECTOR DsbSelection;    // Which DSB to be used for execution
    DD_DSB_TRIGGER_MODE TriggerMode; // Defines how the DSB trigger needs to be done
    DD_DSB_WAIT_CONFIG WaitConfig;   // Defines the DSB behavior on wait and interrupts
    DD_BOOL IsAutoIncrement;         // Whether to use auto increment. Same offset will be used for all the data
    DDU32 AutoIndexOffset;           // The offset of the Index Register of the AutoIncrement Tracked Data Register
    DDU32 AutoIndexData;             // The Data to be written to the AutoIncrement Index Register
    DDU32 DataOffset;                // Starting offset in case of a contiguous MMIO range. Leave it NULL if not contiguous
    DDU32 DataCount;                 // The number of data entries being written.
    DD_BOOL SwitchToNonPostedMode;   // Flag to indicate if special MMIO programming sequence needs to be followed

    union
    {
        DDU32* pContiguousData;               // Data when Offset is not NULL
        DD_OFFSET_DATA_PAIR* pOffsetDataPair; // Data Picked when Offset is NULL
    } Payload;
} DD_DSB_TRIGGER_ARGS;

// Writeback Interfaces
typedef struct _DD_WRITEBACK_QUERY_ARGS
{
    DD_BOOL IsWbFeatureEnabled;                          // INF is set or not
    DD_BOOL WbPluggedIn[MAX_WRITEBACK_DEVICE];           // To get the status of Writeback device plugged in or not
    DDU32 DeviceId[MAX_WRITEBACK_DEVICE];                // Lists the Device Id if pluggedin
    DD_2DREGION CurrentResolution[MAX_WRITEBACK_DEVICE]; // Current Resolution if active
    DD_2DREGION MaxResolution;                           // Max Resolution supported
    DD_BOOL IsOsOperationMode;                           // OS/DFT mode for flip.
} DD_WRITEBACK_QUERY_ARGS;

// Default DPST aggressiveness levels for Windows 8 backlight optimization
typedef enum _DISPLAY_BLC_OS_OPTIMIZATION
{
    DISPLAY_BLC_OPTIMIZATION_OFF,
    DISPLAY_BLC_OPTIMIZATION_LOW,
    DISPLAY_BLC_OPTIMIZATION_EDR,
    DISPLAY_BLC_OPTIMIZATION_HIGH
} DISPLAY_BLC_OS_OPTIMIZATION;

// Xpst workitem can be called from Dpst event handler and Dpst/Opst/GHE/DCE histogram interrupt. Handling b/w event handler and histogram int is slightly different.
// Ex: Histogramevent needs to be cleared after processing Dpst/Opst/GHE/DCE histogram interrupt which is not required in case of event handler work item call.
typedef enum _DD_XPST_WORKITEM_TYPE
{
    DD_XPST_WORKITEM_HISTOGRAM = 0,
    DD_XPST_WORKITEM_EVENT_HANDLE
} DD_XPST_WORKITEM_TYPE;

typedef struct _DD_PC_XPST_WORKITEM_ARGS
{
    PIPE_ID Pipe;
    DD_XPST_WORKITEM_TYPE XpstWorkItemType;
} DD_PC_XPST_WORKITEM_ARGS;

typedef struct _DD_VBI_PROBLEM_TRACKER
{
    union
    {
        struct
        {
            DDU32 VbiRepReqd : 1;           // Did OS ask for VBI reporting? (Old Structure)
            DDU32 VbiGenReqd : 4;           // Pipe(s) on which VBI generation is required as per applied topology. Each bit represent one pipe.
            DDU32 VbiEnabled : 4;           // Pipe(s) on which VBI is enabled in HW (gets determined at the time of TDR)
            DDU32 VbiGenerated : 4;         // Pipe(s) on which VBI generated after topology got commited.
            DDU32 VbiReported : 4;          // Pipe(s) on which VBI Reported to OS.
            DDU32 VbiCorrectSrcAddrRep : 4; // Pipe(s) state of Source Address is reported to OS. 1--> Correct, 0 ->Wrong
            DDU32 VbiCorrectTgtIdRep : 4;   // Pipe(s) state of Target Id is reported to OS. 1--> Correct, 0 ->Wrong
            DDU32 PerPipeVbiRep : 1;        // Is Per Pipe VBI reporting required
            DDU32 VbiRepPerPipeReqd : 4;    // Pipe(s) on which OS ask for VBI reporting
            DDU32 Reserved : 2;             // Reserved
        };
        DDU32 VbiProbTracker;
    };
} DD_VBI_PROBLEM_TRACKER;

typedef struct _DD_PRESENT_ID_TRACKER
{
    DDU64 ReportedPresentId[MAX_PLANES_PER_PIPE];
    DDU64 FlippedPresentId[MAX_PLANES_PER_PIPE];
} DD_PRESENT_ID_TRACKER;

typedef struct _DD_TARGET_ID_TRACKER
{
    DDU32 ReportedTargetId;
    DDU32 ComittedTargetId;
} DD_TARGET_ID_TRACKER;

typedef struct _DD_VSYNC_TIMEOUT_INFO
{
    DDU8 TimeoutIdentified[MAX_POSSIBLE_PIPES]; // Per source bit field to indicate any identified layer with pending flip over 2 seconds (TdrDelay)
    DDU8 TimeoutLayerIndex;                     // Layer index of first identified Layer / Source
    DDU8 TimeoutSourceId;                       // Layer index of first identified Layer / Source
    DDU32 TimeoutTargetId;
    DDU64 TimeoutPresentId; // PresentId of first identified Layer / Source
    DDU8 TimeoutPresentIdIndex;
    DDU64 TimeoutPresentIdTargetTimeInUs;
    DD_BOOL TimeoutDataValid;
} DD_VSYNC_TIMEOUT_INFO;

typedef struct _DD_TDR_FAULT_ANALYSIS_ARGS
{
    DD_LARGE_INTEGER CurrentTdrTimeStamp;
    DDU64 CurrentTdrQpcTimeInUs;
    DD_LARGE_INTEGER LastVbiTimeStamp[MAX_POSSIBLE_PIPES];
    DD_PRESENT_ID_TRACKER PresentIdTracker[MAX_POSSIBLE_PIPES];
    DD_TARGET_ID_TRACKER TargetIdTracker[MAX_POSSIBLE_PIPES];
    DD_VBI_PROBLEM_TRACKER VbiProbTracker;
    DDU32 ActivePipes;
    DD_VSYNC_TIMEOUT_INFO VsyncTimeoutInfo;
    DDU8 PipeDmcFlipQInterruptMask;
    DD_VSYNC_TIMEOUT_SUBCODE VsyncTimeoutSubcode;
} DD_TDR_FAULT_ANALYSIS_ARGS;

typedef struct _DD_PSR_DIAGTYPE_NOTIFICATIONS
{
    union
    {
        struct
        {
            DDU32 PanelSelfRefreshSoftware : 1; // 0x00000001
            DDU32 PanelSelfRefreshHardware : 1; // 0x00000002
            DDU32 Reserved : 30;                // 0xFFFFFFFC
        };
        DDU32 Value;
    };
} DD_PSR_DIAGTYPE_NOTIFICATIONS;

typedef struct _DD_DIAGNOSTIC_TYPES
{
    union
    {
        DD_PSR_DIAGTYPE_NOTIFICATIONS Notifications;
        DDU32 Value;
    };
} DD_PSR_DIAGNOSTIC_TYPES;

typedef struct _DD_PSR_DIAGNOSTIC_CATEGORIES
{
    union
    {
        struct
        {
            DDU32 Notifications : 1; // 0x00000001
            DDU32 Reserved : 31;     // 0xFFFFFFFE
        };
        DDU32 Value;
    };
} DD_PSR_DIAGNOSTIC_CATEGORIES;

typedef struct _DD_PSR_DIAGNOSTIC_DRV_PARAM
{
    DD_PSR_DIAGNOSTIC_CATEGORIES DiagnosticCategory;
    DD_PSR_DIAGNOSTIC_TYPES NoninvasiveTypes;
    DD_PSR_DIAGNOSTIC_TYPES InvasiveTypes;
} DD_PSR_DIAGNOSTIC_DRV_PARAM;

typedef struct _DD_PSR_DIAGNOSTIC_HEADER
{
    DD_PSR_DIAGNOSTIC_CATEGORIES Category;
    DD_PSR_DIAGNOSTIC_TYPES Type;
    union
    {
        struct
        {
            DDU32 Size : 16;     // 0x0000FFFF - Size of whole diagnostic structure, including header
            DDU32 Reserved : 16; // 0xFFFF0000
        };
        DDU32 Value;
    };
    DDU32 SequenceNumber;
    union
    {
        DDU32 TargetId;
        DDU32 SourceId;
        DDU32 Id;
    };
    DD_TARGET_DESCRIPTOR TgtDesc;
} DD_PSR_DIAGNOSTIC_HEADER;

typedef struct _DD_PSR_DIAGNOSTIC_PSR
{
    DD_PSR_DIAGNOSTIC_HEADER Header;
    union
    {
        struct
        {
            DDU32 Present : 1;                   // 0x00000001
            DDU32 CursorUpdate : 1;              // 0x00000002
            DDU32 VSyncEnabled : 1;              // 0x00000004
            DDU32 ColorTransformationChange : 1; // 0x00000008
            DDU32 BrightnessChange : 1;          // 0x00000010
            DDU32 SinkRequest : 1;               // 0x00000020
            DDU32 Other : 1;                     // 0x00000040
            DDU32 Reserved : 25;                 // 0xFFFFFF80
        } RefreshReason;
        DDU32 Value;
    };
} DD_PSR_DIAGNOSTIC_PSR;

typedef enum _DD_PSR_MODE
{
    DD_PSR_INVALID_MODE = 0,
    DD_PSR_HW_MODE,
    DD_PSR_SW_MODE,
    DD_PSR_MAX_MODES
} DD_PSR_MODE;

typedef struct _DD_BRIGHTNESS3_CAPS
{
    DD_BOOL SmoothBrightness;
    DD_BOOL AdaptiveBrightness;
    DD_BOOL NitsBasedBrightness;
} DD_BRIGHTNESS3_CAPS;
/**
 * @brief Input brightness in SetBrightness3 can be in Milli Nits or Milli Percent based on Nits panel or non-nits panel.
 *
 *     DXGK_BRIGHTNESS_SET_IN OS structure has single variable for both Milli Nits and Milli Percent.
 *     For Nits panel, driver has to interpret it as Milli Nits, For Non-nits panel, driver has to interpret it as Milli Percent.
 *     Hence Driver data type is defined as TargetBrightnessInMilliUnits and CurrentBrightnessInMilliUnits.
 *     MSDN link - https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/d3dkmdt/ns-d3dkmdt-_dxgk_brightness_set_in.
 */
typedef struct _DD_BRIGHTNESS3_PARAMS
{
    DDU32 TargetBrightnessInMilliUnits;  // This can be in Milli Nits for Nits panel or Milli Percent for Non-nits panel.
    DDU32 CurrentBrightnessInMilliUnits; // This can be in Milli Nits for Nits panel or Milli Percent for Non-nits panel.
    DDU32 SmoothTransitionTimeInMs;
    DDU32 ActiveRR;
} DD_BRIGHTNESS3_PARAMS;

typedef enum _DD_NITS_BRIGHTNESS_AGGR_TYPE
{
    DD_NITS_BRIGHTNESS_AGGR_TYPE_OFF,
    DD_NITS_BRIGHTNESS_AGGR_TYPE_DESKTOP,
    DD_NITS_BRIGHTNESS_AGGR_TYPE_FSMEDIA,
    DD_NITS_BRIGHTNESS_AGGR_TYPE_FSGAME
} DD_NITS_BRIGHTNESS_AGGR_TYPE;

typedef union _DD_NITS_BRIGHTNESS_AGGRESSIVENESS
{
    DDU8 AggrLevel;
    struct
    {
        DDU8 AggrType : 4;             // Type as defined in DD_NITS_BRIGHTNESS_AGGR_TYPE
        DDU8 ACMode : 1;               // 0 for DC mode, 1 for AC mode.
        DDU8 OptimizationStrength : 3; // Between 0 to 8
    };
} DD_NITS_BRIGHTNESS_AGGRESSIVENESS;

typedef struct _DD_BRIGHTNESS_AGGRESSIVENESS_PARAMS
{
    DD_NITS_BRIGHTNESS_AGGRESSIVENESS NitsAggressivenessParams;
    DISPLAY_BLC_OS_OPTIMIZATION AggrLevel;
} DD_BRIGHTNESS_AGGRESSIVENESS_PARAMS;

/** HDCP related */
typedef enum _DD_HDCP_VERSION
{
    DD_INVALIDHDCPVERSION = 0,
    DD_HDCP1_4 = 1,
    DD_HDCP2_2 = 2,
} DD_HDCP_VERSION;
/**
 * @brief Enum for Reporting the Reason to GCP/CUI why the Panel Max Mode is not Supported.
 *
 *  There are two restrictions defined:
 *  1. Platform Restriction(VBT,SKUing)
 *  2. Dongle Restriction(i.e Dongle is not capable to support the particular mode).
 *
 *  NOTE: In case of any new enum, update TranslateModeRestrictionFromDriver function as well.
 */
typedef enum _DD_MODE_ENUM_RESTRICTIONS
{
    DD_NO_RESTRICTION = 0,
    DD_PLATFORM_RESTRICTION,
    DD_DONGLE_RESTRICTION,
    DD_PIPES_JOINING_UNSUPPORTED,
    DD_DSC_RESTRICTION,
    DD_PIXEL_CLOCK_UNSUPPORTED,
    DD_SAMPLING_MODEYUV420_UNSUPPORTED,
    DD_SAMPLING_MODEYUV422_UNSUPPORTED,
    DD_RESOLUTION_UNSUPPORTED,
    DD_MSO_UNSUPPORTED,
    DD_TIMING_UNSUPPORTED,
    DD_INTERLACED_UNSUPPORTED,
    DD_PROTOCOL_BW_UNSUPPORTED,
    DD_BPCMASK_UNSUPPORTED,
    DD_VBLANK_RESTRICTION,
    DD_MODE_ENUM_RESTRICTIONS_MAX
} DD_MODE_ENUM_RESTRICTIONS;

/**
 * @brief HDCP Capabilities of Monitor and System.
 */
typedef struct _DD_HDCP_VERSION_INFO
{
    DD_HDCP_VERSION MonitorHdcpVersion;
    DD_HDCP_VERSION SystemHdcpVersion;
} DD_HDCP_VERSION_INFO;
/**
 * @brief Max Resolution Supported and RR.
 */
typedef struct _DD_RESOLUTION
{
    DDU32 X;
    DDU32 Y;
    DDU32 RefreshRate;
} DD_RESOLUTION;
/**
 * @brief Resolution Capability of Monitor.
 */
typedef struct _DD_RESOLUTION_CAPABILITIES
{
    /** Max Resolution Supported by Monitor. */
    DD_RESOLUTION MonitorMaxResolution;
    DD_MODE_ENUM_RESTRICTIONS Restriction;
    /** Maximum pixel bandwidth (Mbits per sec) required for this Display, considering all modes before Tunneling BW based pruning.
        Currently used in DP Tunneling BW check. */
    DDU64 PeakModePixelBwMbpsWoTunnelingRestriction;
} DD_RESOLUTION_CAPABILITIES;
/**
 * @brief Info will be display to user via CUI/GCP.
 */
typedef struct _DD_DIAGNOSTIC_INFO
{
    DD_HDCP_VERSION_INFO HdcpVersionInfo;
    DD_RESOLUTION_CAPABILITIES ResolutionCaps;
} DD_DIAGNOSTIC_INFO;

typedef struct _DD_DP_CAPS
{
    DD_OUT DDU32 NumRootPorts;
    DD_OUT DDU8 DPVersionMajor;
    DD_OUT DDU8 DPVersionMinor;
} DD_DP_CAPS;

typedef struct _DD_DP_ADDRESS
{
    DD_OUT DDU32 DPNativeError;
    DD_OUT DDU32 RootPortIndex;
    DD_OUT DDU8 NumLinks;
    DD_OUT DDU8* pRelAddress;
    DD_IN DD_TARGET_DESCRIPTOR TargetDesc;
} DD_DP_ADDRESS;

typedef struct _DD_DP_AUX_IO_TRANSMISSION_PARAMS
{
    struct
    {
        DD_IN DDU32 Write : 1;
        DD_IN DDU32 CanUseCachedData : 1;
        DD_IN DDU32 Reserved : 30;
    };
    DD_IN DDU32 RootPortIndex;
    DD_IN DDU32 DPCDAddress;
    DD_IN DDU8 NumBytesRequested;
    DD_OUT DDU32 DPNativeError;
    DD_OUT DDU8 NumBytesDone;
    DD_IN_OUT DDU8* pData;
} DD_DP_AUX_IO_TRANSMISSION_PARAMS;

typedef struct _DD_DP_I2C_IO_TRANSMISSION_PARAMS
{
    struct
    {
        DD_IN DDU32 Read : 1;
        DD_IN DDU32 Write : 1;
        DD_IN DDU32 EDDCMode : 1;
        DD_IN DDU32 OffsetSizeInBytes : 3;
        DD_IN DDU32 CanUseCachedData : 1;
        DD_IN DDU32 Reserved : 25;
    };
    DD_IN DDU32 RootPortIndex;
    DD_IN DDU32 I2CAddress;
    union
    {
        struct
        {
            DD_IN DDU32 WordOffset : 8;
            DD_IN DDU32 SegmentPointer : 7;
            DD_IN DDU32 Reserved1 : 17;
        };
        DD_IN DDU32 Offset;
    };
    DD_IN DDU32 BufferSizeSupplied;
    DD_IN DDU32 BytesToWrite;
    DD_IN DDU32 BytesToRead;
    DD_OUT DDU32 DPNativeError;
    DD_OUT DDU32 BytesWritten;
    DD_OUT DDU32 BytesRead;
    DD_IN_OUT DDU8* pData;
} DD_DP_I2C_IO_TRANSMISSION_PARAMS;

typedef struct _DD_DP_SBM_TRANSMISSION_PARAMS
{
    struct
    {
        DD_IN DDU32 CanUseCachedData : 1;
        DD_IN DDU32 Reserved : 31;
    };
    DD_IN DDU32 RootPortIndex;
    DD_IN DDU32 BufferSizeSupplied;
    DD_IN DDU32 RequestLength;
    DD_IN DDU32 MaxReplyLength;
    DD_OUT DDU32 DPNativeError;
    DD_OUT DDU32 ActualReplyLength;
    DD_IN_OUT DDU8* pData;
} DD_DP_SBM_TRANSMISSION_PARAMS;
/**
 * @brief HDCP2.2 ME .
 */
typedef enum
{
    HDCP2_AUTH_UPDATE = 0,
    HDCP2_RIV_UPDATE,
    HDCP2_SKEY_UPDATE
} HDCP2_OPERATION_ID;
/**
 * @brief MST Port Info.
 */
typedef struct _DD_DP_MST_PORT_INFO
{
    DDU8 NumValidDPPorts;
    DD_PORT_TYPE PortTypeInfo[DD_MAX_DP_PORTS];
} DD_DP_MST_PORT_INFO;

typedef struct _DD_LFP_ENCODER_INFO
{
    DDU8 NumEnumeratedLfps;
    DDU16 PanelGammaInHundreds[LFP_MAX]; // Panel gamma value in hundreds precision
    DD_BOOL IsPortSyncSupported[LFP_MAX];
    DDU8 NoOfMsoPanelsPresent; // No of MSO panels present in the config
    DD_TARGET_DESCRIPTOR LfpTargetDescriptor[LFP_MAX];
    DD_BOOL IsCommandModeSupported[LFP_MAX];
    DDU32 LfpTargetId[LFP_MAX];
} DD_LFP_ENCODER_INFO;

typedef enum _DD_DVM_FEATURE
{
    DVM_FEATURE_CUSTOM_SCALING,
    DVM_FEATURE_CHANGE_COLOR_MODEL,
    DVM_FEATURE_CUSTOM_MODE,
    DVM_FEATURE_ENUMERATE_SCALING,
    DVM_FEATURE_PERIODIC_FRAME_NOTIFICATION,
    DVM_FEATURE_3D_LUT,
    DVM_FEATURE_EDID_MANAGEMENT,
    DVM_FEATURE_MAX
} DD_DVM_FEATURE;

// Static info of encoders which is either VBT based or based on static enumeration of Target to OS
typedef union _DD_ENCODER_CONFIG
{
    struct
    {
        DDU32 IsInternalDisplay : DD_BITFIELD_BIT(0);    // Is this an internal LFP (eDP or MIPI generally)
        DDU32 IsVesaTiledDisplay : DD_BITFIELD_BIT(1);   // Tiled display panel
        DDU32 IsTypeCCapable : DD_BITFIELD_BIT(2);       // Capable of driving TypeC displays
        DDU32 IsTbtCapable : DD_BITFIELD_BIT(3);         // Capable of driving TBT displays
        DDU32 IsDitheringSupported : DD_BITFIELD_BIT(4); // Is dithering supported
        DDU32 IsVirtualDisplay : DD_BITFIELD_BIT(5);     // Is this a virtual display encoder
        DDU32 IsHiddenDisplay : DD_BITFIELD_BIT(6);      // Is this is a hidden display encoder
        DDU32 IsCollageDisplay : DD_BITFIELD_BIT(7);     // Is this collage display encoder
        DDU32 IsSplitDisplay : DD_BITFIELD_BIT(8);       // Is this a split display config
        DDU32 UNIQUENAME(Reserved) : DD_BITFIELD_RANGE(9, 31);
    };
    DDU32 Value;
} DD_ENCODER_CONFIG;

// Static encoder properties per target ID
typedef struct _DD_ENCODER_PROPERTIES
{
    DD_VIDEO_OUTPUT_TECHNOLOGY SinkType; // Sink type
    DD_ENCODER_CONFIG EncoderConfig;     // Encoder configuration
    DD_CONNECTOR_INFO ConInfo;           // Connector details
} DD_ENCODER_PROPERTIES;

// List of Display features, which includes both Intel specific and Industry standard ones
typedef union _DD_DISPLAY_FEATURES
{
    struct
    {
        DDU32 Hdcp : DD_BITFIELD_BIT(0);                // Is HDCP supported/enabled
        DDU32 HdAudio : DD_BITFIELD_BIT(1);             // Is HD Audio supported/enabled
        DDU32 Psr : DD_BITFIELD_BIT(2);                 // Is Panel self refresh supported/enabled
        DDU32 AdaptiveSync : DD_BITFIELD_BIT(3);        // Is Adaptive sync supported/enabled
        DDU32 VesaCompression : DD_BITFIELD_BIT(4);     // Is VESA Display Stream Compression supported/enabled
        DDU32 Hdr : DD_BITFIELD_BIT(5);                 // Is HDR supported/enabled
        DDU32 HdmiQms : DD_BITFIELD_BIT(6);             // Is HDMI QMS-VRR supported/enabled
        DDU32 Dpst : DD_BITFIELD_BIT(7);                // Is Display Power Savings Technology supported/enabled
        DDU32 Lace : DD_BITFIELD_BIT(8);                // Is Lighting Aware Contrast Enhancement supported/enabled
        DDU32 Drrs : DD_BITFIELD_BIT(9);                // Is Dynamic Refresh Rate Switching supported/enabled
        DDU32 Allm : DD_BITFIELD_BIT(10);               // Is Auto Low Latency mode Supported/enabled
        DDU32 Alrr : DD_BITFIELD_BIT(11);               // Is Alrr Supported/enabled
        DDU32 Fbc : DD_BITFIELD_BIT(12);                // Is Fbc supported/enabled
        DDU32 Hdr10PlusCertified : DD_BITFIELD_BIT(13); // Is Panel HDR10+ certified
        DDU32 Acm : DD_BITFIELD_BIT(14);                // Auto Color Management
        DDU32 Reserved : DD_BITFIELD_RANGE(15, 31);
    };
    DDU32 Value;
} DD_DISPLAY_FEATURES;

/**
 * @brief Speed frame operation type.
 */
typedef enum _DD_SPEEDFRAME_EVENT
{
    DD_SPEEDFRAME_ALLOCATE_BUFFER,
    DD_SPEEDFRAME_SWAP_BUFFER
} DD_SPEEDFRAME_EVENT;

/**
 * @brief Struct for doing speed frame buffer operations.
 */
typedef struct _DD_SPEEDFRAME_ARGS
{
    DD_SPEEDFRAME_EVENT Event;
    DDU32 TargetId;
    HANDLE hAllocation;
} DD_SPEEDFRAME_ARGS;

// Display properties enumerated after a display is attached on the port
// This structure will have meaningful data only if a display is attached
typedef struct _DD_DISPLAY_CONNECTOR_PROPERTIES
{
    DD_VIDEO_OUTPUT_TECHNOLOGY SinkType;                // Sink Type
    DD_BPC_SUPPORTED SupportedOutputMaxBpc;             // Max BPC supported on this display
    DD_BOOL IsTypeC;                                    // Is Type display connected
    DD_BOOL IsTbt;                                      // Is TBT display connected
    DD_VIDEO_OUTPUT_TECHNOLOGY DongleDwnStreamPortType; // Downstream port type
    DDU8 DpcdRev;                                       // DPCD revision, valid if this is a Display Port display
    DD_HDMI_VERSION HdmiDisplayVersion;                 // Hdmi Display version, valid if this is a HDMI dissplay
    DD_BOOL IsExternalPconPresent;                      // Is this DP to HDMI 2.0 Pcon/LS-PCon
    DD_BOOL IsOnboardPconPresent;                       // Is this DP to HDMI 2.1 Pcon, this is differentiated by "Source Control Mode" support caps
    DD_DISPLAY_FEATURES SupportedFeatures;              // Feature support
} DD_DISPLAY_CONNECTOR_PROPERTIES;

// Additional Display properties we get to know only after a mode-set
// We will know the timing applied, bpc, and if all of the supported features or a subset of it got enabled here
// This struct will not have a meaningful data pre-modeset
typedef struct _DD_DISPLAY_OUTPUT_PROPERTIES
{
    DD_DISPLAY_FEATURES EnabledFeatures; // Enabled features
    DD_BOOL IsDitheringEnabled;          // Is dithering enabled on this display
    DD_TIMING_INFO TimingInfo;           // Timing applied on this display
} DD_DISPLAY_OUTPUT_PROPERTIES;

typedef struct _MCD_MODES_GET_TARGET_MODES_ARG
{
    DD_IN DDU32 ChildUid; // Target for whom target mode list is required
    DD_OUT DDU32 NumModes;
    DD_OUT DD_TIMING_INFO* pTargetModes;
} MCD_MODES_GET_TARGET_MODES_ARG;

//------------------------------------------------------------------------------------------------------
// Display shift related data structures
//------------------------------------------------------------------------------------------------------

#define DD_DISPLAY_SHIFT_MAX_DPCD_BUFFER_SIZE sizeof(DD_DISPLAY_SHIFT_DPCD_BUFFER)

// We assume 128 bytes per block of data and Max 4 blocks of EDID data
// _DDC also has provision of only upto 4-blocks. So we restrict to max 4-blocks of EDID data
#define DD_DISPLAY_SHIFT_EDID_SIZE (4 * 128)

// This is used to convert DP_RX_CAPS_DPCDS into a generic data buffer array.
// Only required DPCDs for eDP are listed here. This ensures iGfx and dGfx drivers need not be tightly coupled
// This data buffer array will be shared between iGfx KMD to dGPU KMD drivers
// For simplicity, we will keep this in multiples of 0x10 bytes of DPCDs
// Undefined DPCDs from DP_RX_CAPS_DPCD value will be filled as 0 into this struct passed into IGCL/IGCC
// Protocol layer will do back and forth translation of this data buffer
typedef struct _DD_DISPLAY_SHIFT_DPCD_BUFFER
{
    DDU8 _0To2F[0x30];      // Base caps
    DDU8 _60To7F[0x20];     // DSC and PSR caps
    DDU8 _90To9F[0x10];     // FEC caps
    DDU8 _200To20F[0x10];   // Sink Count
    DDU8 _300To31F[0x20];   // LRR caps
    DDU8 _340To34F[0x20];   // EDP HDR
    DDU8 _400To40F[0x10];   // Panel IEEE OUI
    DDU8 _700To70F[0x10];   // EDP caps
    DDU8 _7A0To7AF[0x10];   // MSO
    DDU8 _2210To221F[0x10]; // Adaptive sync caps
} DD_DISPLAY_SHIFT_DPCD_BUFFER;

// Pixel transformation macros, enums & structures

#define DD_CAPI_COLOR_MAX_NUM_SAMPLES_PER_CHANNEL_1D_LUT 2049
#define DD_MAX_NUM_PIXTX_BLOCKS 32

typedef enum _DD_PIXTX_BLOCK_TYPE
{
    DD_PIXTX_BLOCK_TYPE_INVALID = 0, // block type Invalid
    DD_PIXTX_BLOCK_TYPE_1D_LUT = 1, // block type 1D LUT
    DD_PIXTX_BLOCK_TYPE_3D_LUT = 2, // block type 3D LUT
    DD_PIXTX_BLOCK_TYPE_3X3_MATRIX = 3, // block type 3x3 matrix
    DD_PIXTX_BLOCK_TYPE_3X3_MATRIX_AND_OFFSETS = 4, // block type 3x3 matrix and offsets
} DD_PIXTX_BLOCK_TYPE;

typedef enum _DD_PIXTX_LUT_SAMPLING_TYPE
{
    DD_PIXTX_LUT_SAMPLING_TYPE_UNIFORM = 0, // Uniform LUT sampling
    DD_PIXTX_LUT_SAMPLING_TYPE_NONUNIFORM = 1, // Non uniform LUT sampling, Required mainly in HDR mode
} DD_PIXTX_LUT_SAMPLING_TYPE;

typedef enum _DD_PIXTX_GAMMA_ENCODING_TYPE
{
    DD_PIXTX_GAMMA_ENCODING_TYPE_SRGB = 0, // Gamma encoding SRGB
    DD_PIXTX_GAMMA_ENCODING_TYPE_REC709 = 1, // Gamma encoding REC709, Applicable for REC2020 as well
    DD_PIXTX_GAMMA_ENCODING_TYPE_ST2084 = 2, // Gamma encoding ST2084
    DD_PIXTX_GAMMA_ENCODING_TYPE_HLG = 3, // Gamma encoding HLG
    DD_PIXTX_GAMMA_ENCODING_TYPE_LINEAR = 4, // Gamma encoding linear
} DD_PIXTX_GAMMA_ENCODING_TYPE;

typedef enum _DD_PIXTX_COLOR_SPACE
{
    DD_PIXTX_COLOR_SPACE_REC709 = 0,      // Color space REC709
    DD_PIXTX_COLOR_SPACE_REC2020 = 1,      // Color space REC2020
    DD_PIXTX_COLOR_SPACE_ADOBE_RGB = 2,      // Color space AdobeRGB
    DD_PIXTX_COLOR_SPACE_P3_D65 = 3,      // Color space P3_D65
    DD_PIXTX_COLOR_SPACE_P3_DCI = 4,      // Color space P3_DCI
    DD_PIXTX_COLOR_SPACE_P3_D60 = 5,      // Color space P3_D60
    DD_PIXTX_COLOR_SPACE_CUSTOM = 0xFFFF, // Color space custom, Refer DD_PIXTX_COLOR_PRIMARIES for color
    // primary details
} DD_PIXTX_COLOR_SPACE;

typedef enum _DD_PIXTX_COLOR_MODEL
{
    DD_PIXTX_COLOR_MODEL_RGB_FR = 0, // Color model RGB full range
    DD_PIXTX_COLOR_MODEL_RGB_LR = 1, // Color model RGB limited range
    DD_PIXTX_COLOR_MODEL_YCBCR_422_FR = 2, // Color model YCBCR 422 full range
    DD_PIXTX_COLOR_MODEL_YCBCR_422_LR = 3, // Color model YCBCR 422 limited range
    DD_PIXTX_COLOR_MODEL_YCBCR_420_FR = 4, // Color model YCBCR 420 full range
    DD_PIXTX_COLOR_MODEL_YCBCR_420_LR = 5, // Color model YCBCR 420 limited range
    DD_PIXTX_COLOR_MODEL_YCBCR_444_FR = 6, // Color model YCBCR 444 full range
    DD_PIXTX_COLOR_MODEL_YCBCR_444_LR = 7, // Color model YCBCR 444 limited range
} DD_PIXTX_COLOR_MODEL;

typedef DDU32 DDU0_32;

typedef union _DDU16_16
{
    DDU32 Value;
    struct
    {
        DDU32 Fraction : DD_BITFIELD_RANGE(0, 15);
        DDU32 Integer : DD_BITFIELD_RANGE(16, 31);
    };
} DDU16_16;

typedef struct _DD_PIXTX_COLOR_PRIMARIES
{
    DDU0_32 XR; // CIE1931 x value with maximum red pixel value
    DDU0_32 YR; // CIE1931 y value with maximum red pixel value
    DDU0_32 XG; // CIE1931 x value with maximum green pixel value
    DDU0_32 YG; // CIE1931 y value with maximum green pixel value
    DDU0_32 XB; // CIE1931 x value with maximum blue pixel value
    DDU0_32 YB; // CIE1931 y value with maximum blue pixel value
    DDU0_32 XW; // CIE1931 x value with maximum white pixel value
    DDU0_32 YW; // CIE1931 y value with maximum white pixel value
} DD_PIXTX_COLOR_PRIMARIES;

typedef struct _DD_PIXTX_PIXEL_FORMAT
{
    DDU8 BitsPerColor;                         // Bits per color, It will be 16 for FP16 case
    DD_BOOL IsFloat;                           // Will be set for FP16 or other floating point encoding schemes
    DD_PIXTX_GAMMA_ENCODING_TYPE EncodingType; // Encoding Type
    DD_PIXTX_COLOR_SPACE ColorSpace;           // Color space
    DD_PIXTX_COLOR_MODEL ColorModel;           // Color model
    DD_PIXTX_COLOR_PRIMARIES ColorPrimaries;   // Color primaries, Used mainly for custom color space
    DDU16_16 MaxBrightness;                    // Maximum brightness of pixel values.
    DDU16_16 MinBrightness;                    // Minimum brightness of pixel values
} DD_PIXTX_PIXEL_FORMAT;

typedef struct _DD_PIXTX_1DLUT_CONFIG
{
    DD_PIXTX_LUT_SAMPLING_TYPE SamplingType; // Blocks with non-uniform sampling capability support unifrom
    // sampling also but not vice versa.
    DDU16 NumSamplesPerChannel; // Number of samples per channel. Resampled internally based on
    // HW capability for uniformly sampled LUT.Maximum supported value is
    // MAX_NUM_SAMPLES_PER_CHANNEL_1D_LUT Caller needs to use exact sampling
    // position given in pSamplePositions for non-uniformly sampled LUTs.
    DDU8 NumChannels;                                                           // Number of channels, 1 for Grey scale LUT, 3 for RGB LUT
    DDU8_24 SampleValues[3 * DD_CAPI_COLOR_MAX_NUM_SAMPLES_PER_CHANNEL_1D_LUT]; // Array of sample values, R array followed by G and B arrays
    // in case of multi-channel LUT.
    DDU8_24 SamplePositions[DD_CAPI_COLOR_MAX_NUM_SAMPLES_PER_CHANNEL_1D_LUT]; // LUT (same for all channels) to represent sampling positions for
    // non-uniformly sampled LUTs.
} DD_PIXTX_1DLUT_CONFIG;

typedef struct _DD_PIXTX_MATRIX_CONFIG
{
    DDI15_16 PreOffSets[3];  // Pre offsets   s15.16
    DDI15_16 PostOffsets[3]; // Post offsets  s15.16
    DDI15_16 Matrix[3][3];   // 3x3 Matrix    s15.16
} DD_PIXTX_MATRIX_CONFIG;

typedef union _DD_PIXTX_3DLUT_SAMPLE
{
    DDU32 Value;
    struct
    {
        DDU32 Blue : DD_BITFIELD_RANGE(0, 9);
        DDU32 Green : DD_BITFIELD_RANGE(10, 19);
        DDU32 Red : DD_BITFIELD_RANGE(20, 29);
        DDU32 Reserved : DD_BITFIELD_RANGE(30, 31);
    };
} DD_PIXTX_3DLUT_SAMPLE;

typedef struct _DD_PIXTX_3DLUT_CONFIG
{
    DDU32 NumSamplesPerChannel;                                     // Number of samples per channel
    DD_PIXTX_3DLUT_SAMPLE SampleValues[DD_COLOR_3DLUT_NUM_SAMPLES]; // Pointer to 3dlut sample values
} DD_PIXTX_3DLUT_CONFIG;

typedef union _DD_PIXTX_CONFIG
{
    DD_PIXTX_1DLUT_CONFIG OneDLutConfig;   // 1D LUT configuration
    DD_PIXTX_3DLUT_CONFIG ThreeDLutConfig; // 3D LUT configuration
    DD_PIXTX_MATRIX_CONFIG MatrixConfig;   // Matrix configuration
} DD_PIXTX_CONFIG;

typedef struct _DD_PIXTX_BLOCK_CONFIG
{
    DDU32 BlockId; // Unique ID for each pixel processing block. Id for a block is
    // fixed for a platform.
    DD_OPERATION OperationType;    // Set operation type
    DD_PIXTX_BLOCK_TYPE BlockType; // Block type
    DD_PIXTX_CONFIG Config;        // Configuration
} DD_PIXTX_BLOCK_CONFIG;

typedef struct _DD_PIXTX_PIPE_GET_CONFIG
{
    DDU32 Size;                                                  // Size of this structure
    DDU32 TargetID;                                              // Target Id
    DD_PIXTX_PIXEL_FORMAT InputPixelFormat;                      // Input pixel format
    DD_PIXTX_PIXEL_FORMAT OutputPixelFormat;                     // Output pixel format
    DDU8 NumBlocks;                                              // Number of blocks
    DD_PIXTX_BLOCK_CONFIG BlockConfigs[DD_MAX_NUM_PIXTX_BLOCKS]; // Array of block specific configs
} DD_PIXTX_PIPE_GET_CONFIG;

typedef struct _DD_PIXTX_PIPE_SET_CONFIG
{
    DDU32 Size;                                                  // Size of this structure
    DDU32 TargetID;                                              // Target Id
    DDU8 NumBlocks;                                              // Number of blocks
    DD_PIXTX_BLOCK_CONFIG BlockConfigs[DD_MAX_NUM_PIXTX_BLOCKS]; // Array of block specific configs
} DD_PIXTX_PIPE_SET_CONFIG;

typedef enum _DD_LUT_TYPE
{
    DD_INVALID_LUT_TYPE = 0,
    DD_DGLUT = 1,
    DD_GLUT = 2,
} DD_LUT_TYPE;

typedef struct _DD_VBLANK_TS_INFO_PER_PIPE
{
    DDU8 PipeIndex;
    DDU64 FrameTimestampInTicks;
} DD_VBLANK_TS_INFO_PER_PIPE;
typedef struct _DD_VBLANK_TS_INFO
{
    DDU8 NumPipes;
    DD_BOOL WaitForVblank;
    DD_VBLANK_TS_INFO_PER_PIPE VblankTsPerPipeInfo[MAX_PER_MCD_TARGETS];
} DD_VBLANK_TS_INFO;
// Brightness control port override params
typedef struct _DD_BRIGHTNESS_CONTROL_PORT_OVERRIDE_ARGS
{
    DD_BOOL IsAcpiBrightnessPortOverrideRequired; // Flag for brightness port override capability
    DD_PORT_TYPE AcpiPort;                        // Port to Override
} DD_BRIGHTNESS_CONTROL_PORT_OVERRIDE_ARGS;
typedef struct _DD_MGPU_LDA_INFO_ARGS
{
    DD_BOOL LdaEnable;
    DDU32 PhysicalAdapterIndex; // 0 - LDA primary, 1 .. NumNodesInLink - 1 : secondary
    DDU32 NumNodesInLink;
    DDU32 LdaChainId;
    DDU32 AdapterBdfs[DD_LDA_MAX_ADAPTERS];
} DD_MGPU_LDA_INFO_ARGS;

typedef struct _DD_SYSTEM_INFO
{
    DDU32 EuCount;
    DDU32 SliceCount;
    DDU32 DualSubSliceCount;
    DDU32 SubSliceCount;
} DD_SYSTEM_INFO;

//
// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /
// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /
// ----------------------------------------------------------------------------
//
// DO NOT DEFINE ANYTHING AFTER THIS !!!
//
// ----------------------------------------------------------------------------
// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /
// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// ///
