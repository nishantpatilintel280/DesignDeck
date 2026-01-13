//-----------------------------------------------------------------------------------------------------------
//
// INTEL CONFIDENTIAL
// Copyright (C)2017-2024 Intel Corporation
// This software and the related documents are Intel copyrighted materials,
// and your use of them is governed by the express license under which they were provided to you ("License").
// Unless the License provides otherwise, you may not use, modify, copy, publish,
// distribute, disclose or transmit this software or the related documents without
// Intel's prior written permission. This software and the related documents are provided
// as is, with no express or implied warranties, other than those that are expressly stated in the License.
//
//-----------------------------------------------------------------------------------------------------------

// This file contains definitions for display arguments shared between Display and others also within display layers.
// It shares definitions between legacy Softbios and new display code as well.
// This file currently has mixed strcuture packing directives to align to mainline legacy packing, so that we dont break legacy tools.

#pragma once

#ifndef LOG_ENTRY_ENUMS

#ifndef IN_OUT
#define IN_OUT
#endif

#ifdef _WIN32

#ifndef BOOLEAN
typedef unsigned char BOOLEAN;
#endif

#include "Chipsimc.h"
//#include "OCA.h"
#include "iHDCP.H"
#include "iCP.h"

#include <stdint.h>

#define MAX_HDCP_STREAM_NUM MAX_PHYSICAL_PIPES

#define MAX_INTEL_PIPES MAX_PHYSICAL_PIPES
#define MAX_VIEWS MAX_POSSIBLE_PIPES
#define MAX_PIPES MAX_PHYSICAL_PIPES

#define SB_MAX(a, b) ((a) < (b) ? (b) : (a))
#define MAX_POSSIBLE_PIPES SB_MAX(MAX_PHYSICAL_PIPES, 8) // // Hard coding 8 till MAX_VIRTUAL_PIPES is moving to 8 in Yangra implementation

// 0x5F64, 0x0A - Get/Set Video Parameters ----------------------------------

#define GetSetParameter_FuncCode            (0x5F64)
#define GetSetParameter_SubFunc             (0x0a)

#ifndef UNIQUENAME
#define RESERVED2(x,y)  x##y
#define RESERVED1(x,y)  RESERVED2(x,y)
#define RANDOMNUMBER	__LINE__	// __COUNTER__
#define UNIQUENAME(ValueName) RESERVED1(ValueName, RANDOMNUMBER)
#endif

typedef struct _SB_CONTAINERID_ARGS
{
    GUID Guid;          // GUID of this structure
    unsigned int ulDisplayUID;
    struct
    {
        uint64_t PortId;
        unsigned short ManufacturerName;
        unsigned short ProductCode;
    } EldInfo;
} SB_CONTAINERID_ARGS, * PSB_CONTAINERID_ARGS;

//Following Interface would be used for querying Connector Related Info per Display UID. ported from Aim3Ex.h
// {E79CF31D-8E4D-4a91-BD49-CCCDD76693F7}

#define INTEL_GETCONNECTOR_INFO_GUID    "{E79CF31D-8E4D-4a91-BD49-CCCDD76693F7}"
DEFINE_GUID(GUID_INTEL_GETCONNECTORINFO, 0xe79cf31d, 0x8e4d, 0x4a91, 0xbd, 0x49, 0xcc, 0xcd, 0xd7, 0x66, 0x93, 0xf7);

//SupportedConnectors -> Return the supportedConnectors based on DisplayUID
//Attached Connectors -> If Display is attached then return the connector type i.e. attached
//Active Connectors -> If Display is Active then return the connector type

typedef struct _GET_CONNECTOR_INFO
{
    GUID Guid;
    OUT unsigned int dwSupportedConnectors; //Use the Below Defined Macros for Connector Types.
    OUT unsigned int dwAttachedConnectors;  //Use the Below Defined Macros for Connector Types.
    OUT unsigned int dwActiveConnectors;    //Use the Below Defined Macros for Connector Types.
}GET_CONNECTOR_INFO, * PGET_CONNECTOR_INFO;

#define CONNECTOR_TYPE_UNKNOWN              0xffffffff
#define CONNECTOR_TYPE_NONE                 0x00000000
#define CONNECTOR_TYPE_VGA                  0x00000001 //For CRT
#define CONNECTOR_TYPE_LVDS                 0x00000002
#define CONNECTOR_TYPE_DVI                  0x00000004
#define CONNECTOR_TYPE_HDMI                 0x00000008
#define CONNECTOR_TYPE_SVIDEO               0x00000010
#define CONNECTOR_TYPE_COMPOSITE_VIDEO      0x00000020
#define CONNECTOR_TYPE_COMPONENT_VIDEO      0x00000040
#define CONNECTOR_TYPE_SCART_VIDEO          0x00000080
#define CONNECTOR_TYPE_DISPLAYPORT          0x00000100
#define CONNECTOR_TYPE_EMBEDDED_DISPLAY_PORT         0x00000200
#define CONNECTOR_TYPE_MIPI                 0x00000400

//End of Interface Details for querying Connector Related Info per Display UID.


//Gamma Related Args
//Pallette table is of 1024 values for 10 bit gamma. For split gamma 512 values will be de-gamma and next 512 will be the actual gamma.
//Default table size is 1024. OS will send 256 values as relative gamma. That will be interpolated to 1024 values before applying.
# define MAX_PALETTE_TABLE_SIZE 1024
//OS palette table size is 256. (Fixed from OS) We will interpolate it to 1024 values before applying.
# define MAX_OS_PALETTE_TABLE_SIZE 256

// Pallette table size for 10bit gamma
#define PALLETTE_TABLE_SIZE_10BIT 1024


// cursor constants
#define CURSOR_COLOR_BLACK_ALPHA             0xFF000000
#define CURSOR_COLOR_WHITE_ALPHA             0xFFFFFFFF
#define CURSOR_COLOR_TRANSPARENT_ALPHA       0x00000000
#define CURSOR_COLOR_INVERTED_ALPHA          0x88000000

#define CURSOR_MAX_WIDTH_64_PIXEL          (64)
#define CURSOR_MAX_HEIGHT_64_PIXEL         (64)
#define CURSOR_MAX_WIDTH_128_PIXEL         (128)
#define CURSOR_MAX_HEIGHT_128_PIXEL        (128)
#define CURSOR_MAX_WIDTH_256_PIXEL         (256)
#define CURSOR_MAX_HEIGHT_256_PIXEL        (256)

#define CURSOR_MAX_PALETTE_ENTRIES          4

//DSM Enums
//The following Enum is passed in the _DSM call as Arg2 to specify the Function Index.
typedef enum    _BIOS_DATA_DSM_FUNCS {
    BIOS_DATA_FUNC_SUPPORT = 0x00, // function is supported
    SYSTEM_BIOS_ADAPTER_POWER_STATE_NOTIFICATION = 0x01,
    SYSTEM_BIOS_DISPLAY_POWER_STATE_NOTIFICATION = 0x02,
    SYSTEM_BIOS_POST_COMPLETION_NOTIFICATION = 0x03,
    SYSTEM_BIOS_PRE_HIRES_SET_MODE = 0x04,
    SYSTEM_BIOS_POST_HIRES_SET_MODE = 0x05,
    SYSTEM_BIOS_SET_DISPLAY_DEVICE_NOTIFICATION = 0x06,
    SYSTEM_BIOS_SET_BOOT_DEVICE_PREFERENCE = 0x07,
    SYSTEM_BIOS_SET_PANEL_PREFERENCE = 0x08,
    SYSTEM_BIOS_FULL_SCREEN_DOS = 0x09,
    SYSTEM_BIOS_APM_COMPLETE = 0x0A,
    SYSTEM_BIOS_PLUG_UNPLUG_AUDIO = 0x0B,
    SYSTEM_BIOS_CDCLOCK_CHANGE_NOTIFICATION = 0x0C,
    SYSTEM_BIOS_GET_BOOT_DISPLAY_PREFERENCE = 0x0D,
    SYSTEM_BIOS_GET_PANEL_DETAILS = 0x0E,
    SYSTEM_BIOS_INTERNAL_GRAPHICS = 0x0F,
    SYSTEM_BIOS_GET_AKSV = 0x10,
    SYSTEM_BIOS_ENABLE_S0ix_HPD = 0x11,
    BIOS_DATA_RESERVED //LAST ENTRY
} BIOS_DATA_DSM_FUNCS, * PBIOS_DATA_DSM_FUNCS;

//Returns if GetBiosData Function is Supported.
//For any _DSM, bit 0 in field above denotes if Function of _DSM with this GUID is supported, bit 1 denotes if AdapterPowerStateNotification is supported and so on.
// definition of output bits
typedef union _DSM_SUPPORT_FIELDS_VECTOR
{
    struct
    {
        unsigned int GetBiosDataFuncSuppt : 1;//BIT 0
        unsigned int AdapterPowerStateNotification : 1;//Bit 1
        unsigned int DisplayPowerStateNotification : 1;//Bit 2
        unsigned int SystemBIOSPOSTCompletionNotification : 1;//BIT 3
        unsigned int PreHiResSetMode : 1;//BIT 4
        unsigned int PostHiResSetMode : 1;//BIT 5
        unsigned int SetDisplayDeviceNotification : 1;//BIT 6
        unsigned int SetBootDevicePreference : 1;//BIT 7
        unsigned int SetPanelPreference : 1;//BIT 8
        unsigned int FullScreenDOS : 1;//BIT 9
        unsigned int APMComplete : 1;//BIT 10
        unsigned int UnplugPlugAudio : 1;//BIT 11
        unsigned int CDClockChangeNotification : 1;//BIT 12
        unsigned int GetBootDisplayPreference : 1;//BIT 13
        unsigned int GetPanelDetails : 1;//BIT 14
        unsigned int InternalGraphics : 1;//BIT 15
        unsigned int GetAKSV : 1;//BIT 16
        unsigned int EnableS0ix : 1;//BIT 17
        unsigned int GetHpdCount : 1;//BIT 18
        unsigned int DecrementHpdCount : 1;//BIT 19
        unsigned int DpInSwitchNotification : 1;//BIT 20
        unsigned int DisplaySwitchRequest : 1;//BIT 21
        unsigned int LidSwitchRequest : 1;//BIT 22
        unsigned int Reserved : 9;//BITS 22:31
    };
    unsigned int ulValue;
}DSM_SUPPORT_FIELDS_VECTOR, * PDSM_SUPPORT_FIELDS_VECTOR;

// HDMI / HDCP related
#define MAX_HDCP_DEVICES 127
#define KSV_SIZE           5
#define V_SIZE            20

#define RECEIVER_ID_SIZE 5
#define MAX_DEVICE_COUNT 31

typedef enum _HDCP2_MESSAGE_ID
{
    NULL_MESSAGE = 1,
    AKE_INIT = 2,
    AKE_SEND_CERT = 3,
    AKE_NO_STORED_KM = 4,
    AKE_STORED_KM = 5,
    AKE_SEND_H_PRIME = 7,
    AKE_SEND_PAIRING_INFO = 8,
    LC_INIT = 9,
    LC_SEND_L_PRIME = 10,
    SKE_SEND_EKS = 11,
    REPEATERAUTH_SEND_RECEIVERID_LIST = 12,
    REPEATERAUTH_SEND_ACK = 15,
    REPEATER_AUTH_STREAM_MANAGE = 16,
    REPEATER_AUTH_STREAM_READY = 17,
    //Special cases
    //Errata
    ERRATA_DP_STREAM_TYPE = 50,
}HDCP2_MESSAGE_ID;

//
// Surface memory type
//
typedef enum _SURFACE_MEMORY_TYPE
{
    SURFACE_MEMORY_INVALID = 0,
    SURFACE_MEMORY_LINEAR = 1,                      // Surface uses linear memory
    SURFACE_MEMORY_TILED = 2,                       // Surface uses tiled memory
    SURFACE_MEMORY_X_TILED = SURFACE_MEMORY_TILED,
    SURFACE_MEMORY_Y_LEGACY_TILED = 4,              // Surface uses Legacy Y tiled memory (Gen9+)
    SURFACE_MEMORY_Y_F_TILED = 8,                   // Surface uses Y F tiled memory
    SURFACE_MEMORY_TILE_4 = 16,                     // surface uses Tile4(gen13 onwards)
}SURFACE_MEMORY_TYPE;

typedef enum _DISPLAY_CONFIG_LEGACY
{
    DD_LEGACY_SINGLE_PIPE_SINGLE_DISPLAY_MODE = 0,
    DD_LEGACY_MULTI_PIPE_HOR_COLLAGE_MODE = 2,
    DD_LEGACY_MULTI_PIPE_VER_COLLAGE_MODE = 4,
}DISPLAY_CONFIG_LEGACY;

// Tile widths to use with each of the above suface types
// For YF tiling, 8bpp has a different width than the others
// All tile widths are in Bytes.
typedef enum _SURFACE_MEMORY_TILE_WIDTH
{
    SURFACE_MEMORY_TILE_WIDTH_DEFAULT = 64,
    SURFACE_MEMORY_TILE_WIDTH_LINEAR = SURFACE_MEMORY_TILE_WIDTH_DEFAULT,
    SURFACE_MEMORY_TILE_WIDTH_TILED = 512,
    SURFACE_MEMORY_TILE_WIDTH_X_TILED = SURFACE_MEMORY_TILE_WIDTH_TILED,
    SURFACE_MEMORY_TILE_WIDTH_Y_LEGACY_TILED = 128,
    SURFACE_MEMORY_TILE_WIDTH_Y_F_TILED_8BPP = 64,
    SURFACE_MEMORY_TILE_WIDTH_Y_F_TILED_NON_8BPP = 128
} SURFACE_MEMORY_TILE_WIDTH;

typedef union _SOURCE_MODE_FLAGS
{
    unsigned int  ulFlags;
    struct {
        unsigned bS3DMode : 1;     //Bit Indicating whether the given mode is S3D
        unsigned bCollage : 1;    //Bit indicating if given mode is Collage
        unsigned bHorCollage : 1;    //bit indicating if given mode is horizontal Collage
        unsigned bVerCollage : 1;    //bit indicating if given mode id vertical Collage
        unsigned bRGBColorSeparation : 1;    //bit indicating color separation support for the given mode
        unsigned bTiledMode : 1;    //bit indicating tiled display mode is applicable
        unsigned bPipeGangedMode : 1;    //bit indicating pipe ganged display mode is applicable
        unsigned bReserved : 25;   // Bits 0:24, Reserved
    };
}SOURCE_MODE_FLAGS, * PSOURCE_MODE_FLAGS;



typedef enum _INTERRUPT_UNION_VALUE
{
    INTCRT = 0x1,
    RESERVED_TV = 0x2,
    SDVOB = 0x4,
    SDVOC = 0x8,
    INTDP_HDMIA = 0x10,
    INTDP_HDMIB = 0x20,
    INTDP_HDMIC = 0x40,
    INTDP_HDMID = 0x80,
    INTDP_HDMIA_SP = 0x100,
    INTDP_HDMIB_SP = 0x200,
    INTDP_HDMIC_SP = 0x400,
    INTDP_HDMID_SP = 0x800,
    REN_GEY_SWCMDCOMPLETE = 0x1000,
    REN_GEY_EVLFREQCHNG = 0x2000,
    REN_GEY_AVGBSYTHRESHOLD = 0x4000,
    REN_GEY_CNTBSYTHRESHOLD = 0x8000,
    REN_GEY_UPEVLINTERVAL = 0x10000,
    REN_GEY_DOWNEVLINTERVAL = 0x20000,
    REN_GEY_CNTRLDISABLESTATE = 0x40000,
    DBG_INTERRUPT = 0x80000,
    PIPECTRLNOTIFY = 0x100000,
    RENDERUSERINTERRUPT = 0x200000,
    RNDRMMIOSYNCFLUSHSTATUS = 0x400000,
    RNDRWATCHDOGCNTREXCD = 0x800000,
    RNDRASCNTXSWITCH = 0x1000000,
    RNDRPGFAULT = 0x2000000,
    VIDEOUSERINTERRUPT = 0x4000000,
    VIDEODECPIPELINECNTREXCD = 0x8000000,
    VIDEOMIFLUSHDWNTFY = 0x10000000,
    VIDEOMMIOSYNCFLUSH = 0x20000000,
    VIDEOASCNTXSWITCH = 0x40000000,
    VIDEOPAGEFAULT = 0x80000000
}INTERRUPT_UNION_VALUE;

typedef enum _INTERRUPT_UNION_VALUE_1
{
    LBPC_PIPEA = 0x1,
    LBPC_PIPEB = 0x2,
    DPST_HIST = 0x4,
    DPST_PHASEIN = 0x8,
    PCUMBEVENT = 0x10,
    PCRNDRFREQDOWNRC6TIMEOUT = 0x20,
    PC_RPFUPTHRESHOLD = 0x40,
    PC_RPFDOWNTHRESHOLD = 0x80,
    BLITTERASCNTXSWITCH = 0x100,
    BLITTERMIFLUSHDWNTFY = 0x200,
    BLITTERMMIOSYNCFLUSH = 0x400,
    BLITTERMIUSER = 0x800,
    BLITTERPGFAULT = 0x1000,
    VSYNCPIPEA = 0x2000,
    VSYNCPIPEB = 0x4000,
    VBLANKPIPEA = 0x8000,
    VBLANKPIPEB = 0x10000,
    GSESYSTEMLEVEL = 0x20000,
    VBLANKTPV = 0x40000,
    ASLEINTERRUPT = 0x80000,
    ALLFIRSTLEVEL = 0x100000,
    SPRITEPLANEAFLIPDONE = 0x200000,
    SPRITEPLANEBFLIPDONE = 0x400000,
    VSYNCPIPEC = 0x800000,
    VBLANKPIPEC = 0x1000000,
    SPRITEPLANECFLIPDONE = 0x2000000,
    AUDIOHDCPREQA = 0x4000000,
    AUDIOHDCPREQB = 0x8000000,
    AUDIOHDCPREQC = 0x10000000,
    AUDIOHDCPREQ = 0x20000000,
    PERFMONBUFFHALFFULL = 0x40000000,
    AUDIO_MAILBOX_INTERRUPT = 0x80000000
}INTERRUPT_UNION_VALUE_1;

typedef enum _INTERRUPT_UNION_VALUE_2
{
    FIFO_UNDERRUN_PIPEA = 0x1,
    CRCERROR_PIPEA = 0x2,
    CRCDONE_PIPEA = 0x4,
    FIFO_UNDERRUN_PIPEB = 0x8,
    CRCERROR_PIPEB = 0x10,
    CRCDONE_PIPEB = 0x20,
    FIFO_UNDERRUN_PIPEC = 0x40,
    CRCERROR_PIPEC = 0x80,
    CRCDONE_PIPEC = 0x100,
    VEUSERINTERRUPT = 0x200,
    VEMMIOSYNCFLUSH = 0x400,
    VECMDPARSERMASTERERR = 0x800,
    VEMIFLUSHDWNOTIFY = 0x1000,
    RENDERPARITYERR = 0x2000,
    VIDEOPAVPATTACK = 0x4000,
    VIDEOUSERINT2 = 0x8000,
    VIDEODECPIPELINECNTREXCD2 = 0x10000,
    VIDEOMIFLUSHDWNTFY2 = 0x20000,
    VIDEOMMIOSYNCFLUSH2 = 0x40000,
    VIDEOASCNTXSWITCH2 = 0x80000,
    VIDEOPAGEFAULT2 = 0x100000,
    VIDEOPAVPATTACK2 = 0x200000,
    GUCSHIMERROR = 0x400000,
    GUCDMAINTERROR = 0x800000,
    GUCDMADONE = 0x1000000,
    GUCDOORBELLRANG = 0x2000000,
    GUCIOMMUSENTMSGGUC = 0x4000000,
    GUCSEMAPHORESIGNALED = 0x8000000,
    GUCDISPLAYEVENTRECIEVED = 0x10000000,
    GUCEXECUTIONERROR = 0x20000000,
    GUCINTERRUPTTOHOST = 0x40000000,
    CSTRINVALIDTILEDETECTION = 0x80000000,
}INTERRUPT_UNION_VALUE_2;

typedef enum _INTERRUPT_UNION_VALUE_3
{
    VECSCONTEXTSWITCHINT = 0x1,
    VECSWAITONSEMAPHORE = 0x2,
    WDBOXINTERRUPT = 0x4,
    DPST_HIST_PIPEB = 0x8,
    DPST_PHASEINT_PIPEB = 0x10,
    DPST_HIST_PIPEC = 0x20,
    DPST_PHASEINT_PIPEC = 0x40,
    PIPEA_PLANE1_FLIP_DONE_INT = 0x80,
    PIPEA_PLANE2_FLIP_DONE_INT = 0x100,
    PIPEA_PLANE3_FLIP_DONE_INT = 0x200,
    PIPEB_PLANE1_FLIP_DONE_INT = 0x400,
    PIPEB_PLANE2_FLIP_DONE_INT = 0x800,
    PIPEB_PLANE3_FLIP_DONE_INT = 0x1000,
    PIPEC_PLANE1_FLIP_DONE_INT = 0x2000,
    PIPEC_PLANE2_FLIP_DONE_INT = 0x4000,
    PIPEC_PLANE3_FLIP_DONE_INT = 0x8000,
    PIPEA_PLANE1_FLIP_QUEUE_EMPTY_INT = 0x10000,
    PIPEA_PLANE2_FLIP_QUEUE_EMPTY_INT = 0x20000,
    PIPEA_PLANE3_FLIP_QUEUE_EMPTY_INT = 0x40000,
    PIPEB_PLANE1_FLIP_QUEUE_EMPTY_INT = 0x80000,
    PIPEB_PLANE2_FLIP_QUEUE_EMPTY_INT = 0x100000,
    PIPEB_PLANE3_FLIP_QUEUE_EMPTY_INT = 0x200000,
    PIPEC_PLANE1_FLIP_QUEUE_EMPTY_INT = 0x400000,
    PIPEC_PLANE2_FLIP_QUEUE_EMPTY_INT = 0x800000,
    PIPEC_PLANE3_FLIP_QUEUE_EMPTY_INT = 0x1000000,
    DEMISCSVMWAITDESCCOMPLETE = 0x2000000,
    DEMISCSVMVTDFAULT = 0x4000000,
    DEMISCSVMPRQEVENT = 0x8000000,
    PIPEA_PLANE4_FLIP_DONE_INT = 0x10000000, // BXT
    PIPEB_PLANE4_FLIP_DONE_INT = 0x20000000, // BXT
    DEMISC_GTC_COMBINED_EVENT = 0x40000000,
    VECSWATCHDOGCNTREXCD = 0x80000000,
}INTERRUPT_UNION_VALUE_3;

typedef enum _INTERRUPT_UNION_VALUE_4
{
    // assigning values to match definition in INTERRUPT_ARGS
    MIPIA = 0x1,
    MIPIC = 0x2,
    LPE_PIPEA = 0x4,
    LPE_PIPEB = 0x8,
    ISP = 0x10,
    VED_BLOCK = 0x20,
    VED_POWER = 0x40,
    PIPEA_PLANE4_FLIP_QUEUE_EMPTY_INT = 0x80, // BXT
    PIPEB_PLANE4_FLIP_QUEUE_EMPTY_INT = 0x100, // BXT
    LPE_PIPEC = 0x200,
    CORE_TO_UNCORE_TRAP = 0x400,
    WDBOX_END_OF_FRAME_INTERRUPT = 0X800,
    INTDP_HDMIE = 0x1000,
    INTDP_HDMIE_SP = 0x2000,

    RENDERTDLRETRYINTR = 0x4000, //KBL

    PINNING_CONTEXT_SWITCH = 0x8000,
    PINNING_USER_INTR = 0x10000,
    DEMISC_WD_COMBINED_INTERRUPT = 0x20000,
    PIPEA_UNDERRUN = 0x40000,
    PIPEB_UNDERRUN = 0x80000,
    PIPEC_UNDERRUN = 0x100000,
    PIPEC_PLANE4_FLIP_DONE_INT = 0x200000,
    INVALID_GTT_PAGE_TABLE_ENTRY = 0x400000,
    INVALID_PAGE_TABLE_ENTRY_DATA = 0x800000,
    PIPED_VSYNC = 0x1000000,
    PIPED_VBLANK = 0x2000000,
    INTDP_HDMIF = 0x4000000,
    INTDP_HDMIF_SP = 0x8000000,
    WDBOX2INTERRUPT = 0x10000000,
    WDBOX2_END_OF_FRAME_INTERRUPT = 0x20000000,
    DEMISC_DBUF_OVERLAP_DETECTED = 0x40000000,
    DEMISC_RM_TIMEOUT_INTERRUPT = 0x80000000
}INTERRUPT_UNION_VALUE_4;

typedef enum _INTERRUPT_UNION_VALUE_5
{
    // assigning values to match definition in INTERRUPT_ARGS
    PIPEA_PLANE1_GTT_FAULT_STATUS = 0x1,
    PIPEA_PLANE2_GTT_FAULT_STATUS = 0x2,
    PIPEA_PLANE3_GTT_FAULT_STATUS = 0x4,
    PIPEA_PLANE4_GTT_FAULT_STATUS = 0x8,
    PIPEA_CURSOR_GTT_FAULT_STATUS = 0x10,

    PIPEB_PLANE1_GTT_FAULT_STATUS = 0x20,
    PIPEB_PLANE2_GTT_FAULT_STATUS = 0x40,
    PIPEB_PLANE3_GTT_FAULT_STATUS = 0x80,
    PIPEB_PLANE4_GTT_FAULT_STATUS = 0x100,
    PIPEB_CURSOR_GTT_FAULT_STATUS = 0x200,

    PIPEC_PLANE1_GTT_FAULT_STATUS = 0x400,
    PIPEC_PLANE2_GTT_FAULT_STATUS = 0x800,
    PIPEC_PLANE3_GTT_FAULT_STATUS = 0x1000,
    PIPEC_PLANE4_GTT_FAULT_STATUS = 0x2000,
    PIPEC_CURSOR_GTT_FAULT_STATUS = 0x4000,
    INTDP_HDMIB_SCDC_INTERRUPT = 0x8000,
    INTDP_HDMIC_SCDC_INTERRUPT = 0x10000,
    INTDP_HDMID_SCDC_INTERRUPT = 0x20000,
    INTDP_HDMIE_SCDC_INTERRUPT = 0x40000,
    INTDP_HDMIF_SCDC_INTERRUPT = 0x80000,
    PIPEA_PLANE5_FLIP_DONE_INT = 0x100000,
    PIPEA_PLANE6_FLIP_DONE_INT = 0x200000,
    PIPEA_PLANE7_FLIP_DONE_INT = 0x400000,
    PIPEB_PLANE5_FLIP_DONE_INT = 0x800000,
    PIPEB_PLANE6_FLIP_DONE_INT = 0x1000000,
    PIPEB_PLANE7_FLIP_DONE_INT = 0x2000000,
    PIPEC_PLANE5_FLIP_DONE_INT = 0x4000000,
    PIPEC_PLANE6_FLIP_DONE_INT = 0x8000000,
    PIPEC_PLANE7_FLIP_DONE_INT = 0x10000000,
    PIPED_PLANE5_FLIP_DONE_INT = 0x20000000, // PIPED from Gen11.5
    PIPED_PLANE6_FLIP_DONE_INT = 0x40000000,
    PIPED_PLANE7_FLIP_DONE_INT = 0x80000000,
}INTERRUPT_UNION_VALUE_5;

typedef enum _INTERRUPT_UNION_VALUE_6
{
    // assigning values to match definition in INTERRUPT_ARGS
    PIPEA_PLANE5_GTT_FAULT_STATUS = 0x1,
    PIPEA_PLANE6_GTT_FAULT_STATUS = 0x2,
    PIPEA_PLANE7_GTT_FAULT_STATUS = 0x4,
    PIPEB_PLANE5_GTT_FAULT_STATUS = 0x8,
    PIPEB_PLANE6_GTT_FAULT_STATUS = 0x10,
    PIPEB_PLANE7_GTT_FAULT_STATUS = 0x20,
    PIPEC_PLANE5_GTT_FAULT_STATUS = 0x40,
    PIPEC_PLANE6_GTT_FAULT_STATUS = 0x80,
    PIPEC_PLANE7_GTT_FAULT_STATUS = 0x100,
    PIPED_PLANE5_GTT_FAULT_STATUS = 0x200,
    PIPED_PLANE6_GTT_FAULT_STATUS = 0x400,
    PIPED_PLANE7_GTT_FAULT_STATUS = 0x800,
    PIPED_PLANE1_FLIP_DONE_INT = 0x1000,
    PIPED_PLANE2_FLIP_DONE_INT = 0x2000,
    PIPED_PLANE3_FLIP_DONE_INT = 0x4000,
    PIPED_PLANE4_FLIP_DONE_INT = 0x8000,
    PIPED_PLANE1_GTT_FAULT_STATUS = 0x10000,
    PIPED_PLANE2_GTT_FAULT_STATUS = 0x20000,
    PIPED_PLANE3_GTT_FAULT_STATUS = 0x40000,
    PIPED_PLANE4_GTT_FAULT_STATUS = 0x80000,
    PIPED_CURSOR_GTT_FAULT_STATUS = 0x100000,
    PIPED_DPST_HIST = 0x200000,
    PIPED_CRCERROR = 0x400000,
    PIPED_CRCDONE = 0x800000,
    PIPED_UNDERRUN = 0x1000000,
    AUDIOHDCPREQD = 0x2000000,
    INTDP_HDMIA_SCDC_INTERRUPT = 0x4000000,  // Gen11.5
    PIPEA_VRRDOUBLEBUFFERUPDATE = 0x8000000,
    PIPEB_VRRDOUBLEBUFFERUPDATE = 0x10000000,
    PIPEC_VRRDOUBLEBUFFERUPDATE = 0x20000000,
    PIPED_VRRDOUBLEBUFFERUPDATE = 0x40000000,
}INTERRUPT_UNION_VALUE_6;

typedef enum _INTERRUPT_UNION_VALUE_7
{
    // assigning values to match definition in INTERRUPT_ARGS
    PIPEA_SCANLINE_EVENT = 0x1,
    PIPEB_SCANLINE_EVENT = 0x2,
    PIPEC_SCANLINE_EVENT = 0x4,
    PIPED_SCANLINE_EVENT = 0x8,
    // assigning values to match definition in INTERRUPT_ARGS
    KVMR_REQUESTDISPLAY_INTERRUPT = 0x10,
    KVMR_RELEASEDISPLAY_INTERRUPT = 0x20,
    INTDP_HDMIG = 0x40,
    INTDP_HDMIG_SP = 0x80,
    INTDP_HDMIH = 0x100,
    INTDP_HDMIH_SP = 0x200,
    INTDP_HDMII = 0x400,
    INTDP_HDMII_SP = 0x800,
    INTDP_HDMIG_SCDC_INTERRUPT = 0x1000,
    INTDP_HDMIH_SCDC_INTERRUPT = 0x2000,
    INTDP_HDMII_SCDC_INTERRUPT = 0x4000,
    PIPEA_ISCO_RSP_EVENT = 0x8000,
    PIPEB_ISCO_RSP_EVENT = 0x10000,
    PIPEC_ISCO_RSP_EVENT = 0x20000,
    PIPED_ISCO_RSP_EVENT = 0x40000,
    THERMAL_PANIC_MODE_EVENT = 0x80000,
}INTERRUPT_UNION_VALUE_7;

typedef enum _INTERRUPT_UNION_VALUE_8
{
    MAIN_DMC_INTERRUPT = 0x1,
    PIPE_A_DMC_FLIPQ_INTERRUPT = 0x2,
    PIPE_B_DMC_FLIPQ_INTERRUPT = 0x4,
    PIPE_C_DMC_FLIPQ_INTERRUPT = 0x8,
    PIPE_D_DMC_FLIPQ_INTERRUPT = 0x10,
    MAIN_DMC_ERROR_INTERRUPT = 0x20,
    PIPE_A_DMC_ERROR_INTERRUPT = 0x40,
    PIPE_B_DMC_ERROR_INTERRUPT = 0x80,
    PIPE_C_DMC_ERROR_INTERRUPT = 0x100,
    PIPE_D_DMC_ERROR_INTERRUPT = 0x200,
    PIPE_A_DMC_RM_NO_CLAIM_INTERRUPT = 0x400,
    PIPE_B_DMC_RM_NO_CLAIM_INTERRUPT = 0x800,
    PIPE_C_DMC_RM_NO_CLAIM_INTERRUPT = 0x1000,
    PIPE_D_DMC_RM_NO_CLAIM_INTERRUPT = 0x2000,
}INTERRUPT_UNION_VALUE_8;

typedef enum _INTERRUPT_UNION_VALUE_9
{
    CSC_HECI1_INTERRUPT = 0x1,
    CSC_HECI2_INTERRUPT = 0x2,
} INTERRUPT_UNION_VALUE_9;

// Data Structure for Interrupt operations
typedef enum _INTERRUPT_OPERATION
{
    ENABLE_INTERRUPT = 1,
    DISABLE_INTERRUPT = 2,
    MASK_INTERRUPT = 3,
    UNMASK_INTERRUPT = 4,
    GET_DISABLED_INTERRUPTS = 5, // Flag To Detect Disabled Interrupts also.
} INTERRUPT_OPERATION;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//NOTE: Any change in the interrrupt args structure below has to be reflected in the enum definitions above.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Interrupt Args structure
typedef struct _SB_INTERRUPT_ARGS
{
    INTERRUPT_OPERATION eOperation; // Data structure for interrupt operations
    unsigned int ulPrivateData; // Private Interrupt Data. This will not be used in ILK and GT since enabling/disabling of all interrupts has become much simpler..

    unsigned int PlatformUsesGen11InterruptArchitecture : 1; // Let Event handler code know that all non-display engines are handled by new Selector methods
    unsigned int SelectorInterruptsOccurred : 1; // New for Gen11+  ::  This bit means one of the hierarchical interrupts (has to use selector register) has occurred and needs to be handled by GEN11+ handler
    unsigned int LegacyInterruptsOccurred : 1; // New for Gen11+  ::  This bit means use the IntArgs below for handling the interrupts
    unsigned int LegacyPcuInterruptsOccurred : 1; // New for Gen11+  ::  This bit means use the IntArgs below for handling the interrupts
    unsigned int MemoryBasedInterruptOccurred : 1; // New for Gen12+ SRIOV :: This bit means one of GT-interrupts has occurred for VF, handling is similar to SelectoreInterrupt.
    unsigned int CscInterruptsOccurred : 1; // New for BMG :: This bit means the CSC raised an interrupt for the child driver.
    unsigned int SocHostToDevMemBlockInterruptOccurred : 1; // New for CLS :: This bit means SOC HostToDeviceMemory Block triggered an interrupt.

    IN BOOLEAN HierarchicalInterruptService : 1; // This bit means request for Hirearchial Interrupt services
    IN BOOLEAN MemoryBasedInterruptService : 1; // This bit means request for Memory-based interrupt services

    IN unsigned int ulEngineClass;                           // Can re-use ulValue in union
    IN unsigned int ulEngineIntrpt;                          // Can re-use ulValue1 in union

    IN unsigned char ucGpuTileNumber;           // GPU tile number in case of Multi-Tile architecture platforms like 4xATS. Default Tile will be 0

    union
    {
        unsigned int ulValue; // temp code for SB compilation
        unsigned int Value;
        struct
        {
            //1. Hot Plug Interrupts Definitions - Starts Here

            unsigned int    UNIQUENAME(ulReservedBit0) : 1;    // Bit 0
            unsigned int    UNIQUENAME(ulReservedBit1) : 1;    // Bit 1
            unsigned int    UNIQUENAME(ulReservedBit2) : 1;    // Bit 2
            unsigned int    UNIQUENAME(ulReservedBit3) : 1;    // Bit 3 //From Gen6 onwards,no need to register for this event as sDVO on Port C is disabled..
            unsigned int    bIntDP_HDMIAInterrupt : 1;    // Bit 4  //New Introduction from ILK onwards
            unsigned int    bIntDP_HDMIBInterrupt : 1;    // Bit 5
            unsigned int    bIntDP_HDMICInterrupt : 1;    // Bit 6
            unsigned int    bIntDP_HDMIDInterrupt : 1;    // Bit 7
            unsigned int    bIntDP_HDMIA_SPInterrupt : 1;    // Bit 8 //New Introduction from ILK onwards
            unsigned int    bIntDP_HDMIB_SPInterrupt : 1;    // Bit 9
            unsigned int    bIntDP_HDMIC_SPInterrupt : 1;    // Bit 10
            unsigned int    bIntDP_HDMID_SPInterrupt : 1;    // Bit 11

            //1. Hot Plug Interrupts Definitions - Ends Here

            //2. Render Geyserville Interrupts Definitions - Starts Here

            //Render Geyserville Interrupts common till ILK Platform
            unsigned int    UNIQUENAME(ulReservedBit12) : 1; // bit 12 // Render GeyserVille Interrupt
            unsigned int    UNIQUENAME(ulReservedBit13) : 1; // bit 13 // Render GeyserVille Interrupt

            //New Render Geyserville Interrupts exists only in ILK
            unsigned int    UNIQUENAME(ulReservedBit14) : 1; // bit 14 // Render GeyserVille Interrupt
            unsigned int    UNIQUENAME(ulReservedBit15) : 1; // bit 15 // Render GeyserVille Interrupt

            //Render Geyserville Common between ILK and GT
            unsigned int    UNIQUENAME(ulReservedBit16) : 1; // bit 16 // Render GeyserVille Interrupt
            unsigned int    UNIQUENAME(ulReservedBit17) : 1; // bit 17 // Render GeyserVille Interrupt

            //Render Geyserville Introduced from GT
            unsigned int    UNIQUENAME(ulReservedBit18) : 1; // bit 18 // Render GeyserVille Interrupt

            //2. Render Geyserville Interrupts Definitions - Ends Here

            //3. Basic Render Interrupt Definitions - Starts Here

            unsigned int    UNIQUENAME(ulReservedBit19) : 1; // Bit 19 Gen4 Onwards
            unsigned int    bPipeControlNotifyInterrupt : 1; // Bit 20 Gen4 Onwards
            unsigned int    bRenderUserInterrupt : 1; // Bit 21 Render Cmd UI
            unsigned int    UNIQUENAME(ulReservedBit22) : 1; // Bit 22
            unsigned int    bRenderWatchDogCounterExcd : 1; // Bit 23 //ILK Onwards
            unsigned int    bRenderASContextSwitch : 1; // Bit 24 //ILK Onwards
            unsigned int    bRenderPageFault : 1; // Bit 25 //ILK Onwards

            //3. Basic Render Interrupt Definitions - Ends Here

            //4. Media/Video Interrupt Definitions - Starts Here
            unsigned int    bVideoUserInterrupt : 1; // Bit 26 Gen4 Onwards
            unsigned int    bVideoDecPipelineCntrExceed : 1; // Bit 27 Gen4 Onwards..Same as Video Command Streamer WatchDog Counter Exceeded in GT
            //Following are valid from GT
            unsigned int    bVideoMIFlush_DWNotify : 1; // Bit 28
            unsigned int    UNIQUENAME(ulReservedBit29) : 1; // Bit 29
            unsigned int    bVideoASContextSwitch : 1; // Bit 30
            unsigned int    bVideoPageFault : 1; // Bit 31

            //4. Media/Video Interrupt Definitions - Ends Here
        };
    };

    union
    {
        unsigned int ulValue1;
        unsigned int Value1;
        struct
        {
            //5. Remaining Power Conservation Interrupt Starts here
            unsigned int    UNIQUENAME(ulReservedBit0) : 1;    // Bit 0 - crestline and after. Doesnt exist from ILK Onwards
            unsigned int    UNIQUENAME(ulReservedBit1) : 1;    // Bit 1 - crestline and after. Doesnt exist from ILK Onwards
            unsigned int    bDPST_HistInterrupt : 1;    // Bit 2 - crestline and after
            unsigned int    UNIQUENAME(ulReservedBit3) : 1;    // Bit 3 - crestline and after

            //Valid from GT Onwards
            unsigned int    bPCUDriverMBEvent : 1; // Bit 4
            unsigned int    UNIQUENAME(ulReservedBit5) : 1; // Bit 5
            unsigned int    UNIQUENAME(ulReservedBit6) : 1; // Bit 6
            unsigned int    UNIQUENAME(ulReservedBit7) : 1; // Bit 7

            //5. Remaining Power Conservation Interrupt Ends here

            //6. Blitter Interrupts from GT Onwards Starts here

            unsigned int    bBlitterASContextSwitch : 1; // Bit 8
            unsigned int    bBlitterMIFlush_DWNotify : 1; // Bit 9
            unsigned int    UNIQUENAME(ulReservedBit10) : 1; // Bit 10
            unsigned int    bBlitterMI_User_Interrupt : 1; // Bit 11
            unsigned int    bBlitterPageFault : 1; // Bit 12
            //6. Blitter Interrupts from GT Onwards Ends here

            //7. Misc Interrupts Category Starts here
            unsigned int    UNIQUENAME(ulReservedBit13) : 1;    // Bit 13 //Not Required
            unsigned int    UNIQUENAME(ulReservedBit14) : 1;    // Bit 14 //Not Required
            unsigned int    bVBlank_PipeAInterrupt : 1;    // Bit 15
            unsigned int    bVBlank_PipeBInterrupt : 1;    // Bit 16
            unsigned int    bGSESystemLevel : 1;    // Bit 17  Valid from ILK Replacement for ASLE Interrupt
            unsigned int    bVblankTPV : 1;    // BIT 18 //Used for TPV Vblank Interrupt
            unsigned int    UNIQUENAME(ulReservedBit19) : 1; // Bit 19
            unsigned int    bAllFirstLevelInterrupts : 1;    // BIT 20 //Used for Enabling/Disabling of Interrupts..

            //7. Misc Interrupts Category Ends here

            //8. New added Interrupts
            unsigned int    bSpritePlaneAFlipDoneInterrupt : 1;    // BIT 21 //Used for Enabling/Disabling of Sprite Plane A Flip Done Interrupt..
            unsigned int    bSpritePlaneBFlipDoneInterrupt : 1;    // BIT 22 //Used for Enabling/Disabling of Sprite Plane B Flip Done Interrupt..

            unsigned int    UNIQUENAME(ulReservedBit23) : 1;    // BIT 23
            unsigned int    bVBlank_PipeCInterrupt : 1;    // BIT 24
            unsigned int    bSpritePlaneCFlipDoneInterrupt : 1;    // BIT 25

            unsigned int    bAudioHDCPRequestInterruptA : 1;    // BIT 26 //Audio HDCP request for transcoder A
            unsigned int    bAudioHDCPRequestInterruptB : 1;    // BIT 27 //Audio HDCP request for transcoder B
            unsigned int    bAudioHDCPRequestInterruptC : 1;    // BIT 28 //Audio HDCP request for transcoder C
            unsigned int    bAudioHDCPRequestInterrupt : 1;    // BIT 29 //Audio HDCP request for pre ilk platforms

            unsigned int   bPerfMonBufferHalfFullInterrupt : 1;    // BIT 30

            unsigned int   bAudioMailBoxInterrupt : 1;               // BIT 31
        };
    };

    union
    {
        unsigned int ulValue2;
        unsigned int Value2;
        struct
        {
            // This sections contains error/debug status bits
            unsigned int UNIQUENAME(ulReservedBit0) : 1; // bit 0
            unsigned int bCRC_Error_PipeAInterrupt : 1; // bit 1
            unsigned int bCRC_Done_PipeAInterrupt : 1; // bit 2

            unsigned int UNIQUENAME(ulReservedBit3) : 1; // bit 3
            unsigned int bCRC_Error_PipeBInterrupt : 1; // bit 4
            unsigned int bCRC_Done_PipeBInterrupt : 1; // bit 5

            unsigned int UNIQUENAME(ulReservedBit6) : 1; // bit 6
            unsigned int bCRC_Error_PipeCInterrupt : 1; // bit 7
            unsigned int bCRC_Done_PipeCInterrupt : 1; // bit 8

            // VE (Video Enhancement) Interrupt Definitions - Starts Here - Valid from Gen7_5 (HSW+) onward
            unsigned int bVEUserInterrupt : 1; // bit 9
            unsigned int UNIQUENAME(ulReservedBit10) : 1; // bit 10
            unsigned int UNIQUENAME(ulReservedBit11) : 1; // bit 11
            unsigned int bVEMIFlush_DWNotify : 1; // bit 12
            // VE (Video Enhancement) Interrupt Definitions - Ends Here

            // other interrupt bits that don't fit into the previous unsigned ints
            unsigned int bRenderParityError : 1; // Bit 13 Gen7 Onwards

            unsigned int bVideoPavpUnsolicitedAttack : 1; // Bit 14 Gen7 Onwards

            //Below are valid from BDW
            unsigned int bVideoUserInterrupt2 : 1; // Bit 15
            unsigned int bVideoDecPipelineCntrExceed2 : 1; // Bit 16
            unsigned int bVideoMIFlush_DWNotify2 : 1; // Bit 17
            unsigned int UNIQUENAME(ulReservedBit18) : 1; // Bit 18
            unsigned int bVideoASContextSwitch2 : 1; // Bit 19
            unsigned int UNIQUENAME(ulReservedBit20) : 1; // Bit 20
            unsigned int bVideoPavpUnsolicitedAttack2 : 1; // Bit 21
            unsigned int bKcrPageFault : 1; // Bit 22

            unsigned int UNIQUENAME(ulReservedBit23) : 1;    // bit 23
            unsigned int UNIQUENAME(ulReservedBit24) : 1;    // bit 24
            unsigned int UNIQUENAME(ulReservedBit25) : 1;    // bit 25
            unsigned int UNIQUENAME(ulReservedBit26) : 1;    // bit 26
            unsigned int UNIQUENAME(ulReservedBit27) : 1;    // bit 27
            unsigned int UNIQUENAME(ulReservedBit28) : 1;    // bit 28
            unsigned int UNIQUENAME(ulReservedBit29) : 1;    // bit 29
            unsigned int bGuCInterruptToHost : 1;    // bit 30

            unsigned int UNIQUENAME(ulReservedBit31) : 1;    // bits 31
        };
    };

    union
    {
        unsigned int ulValue3;
        unsigned int Value3;
        struct
        {
            // This sections contains VEC/WiDi interrupts
            unsigned int bVECSContextSwitchInterrupt : 1; // bit 0
            unsigned int ulReservedBit1 : 1; // bit 1
            unsigned int bWDBoxInterrupt : 1; // bit 2
            unsigned int bDPST_HistInterruptPipeB : 1;// bit 3
            unsigned int UNIQUENAME(ulReservedBit4) : 1; // bit 4
            unsigned int bDPST_HistInterruptPipeC : 1;// bit 5
            unsigned int UNIQUENAME(ulReservedBit6) : 1; // bit 6

            unsigned int bPipeA_Plane1FlipDoneInterrupt : 1; //bit 7
            unsigned int bPipeA_Plane2FlipDoneInterrupt : 1; //bit 8
            unsigned int bPipeA_Plane3FlipDoneInterrupt : 1; //bit 9

            unsigned int bPipeB_Plane1FlipDoneInterrupt : 1; //bit 10
            unsigned int bPipeB_Plane2FlipDoneInterrupt : 1; //bit 11
            unsigned int bPipeB_Plane3FlipDoneInterrupt : 1; //bit 12

            unsigned int bPipeC_Plane1FlipDoneInterrupt : 1; //bit 13
            unsigned int bPipeC_Plane2FlipDoneInterrupt : 1; //bit 14
            unsigned int bPipeC_Plane3FlipDoneInterrupt : 1; //bit 15

            unsigned int UNIQUENAME(ulReservedBit16) : 1; //bit 16
            unsigned int UNIQUENAME(ulReservedBit17) : 1; //bit 17
            unsigned int UNIQUENAME(ulReservedBit18) : 1; //bit 18

            unsigned int UNIQUENAME(ulReservedBit19) : 1; //bit 19
            unsigned int UNIQUENAME(ulReservedBit20) : 1; //bit 20
            unsigned int UNIQUENAME(ulReservedBit21) : 1; //bit 21

            unsigned int UNIQUENAME(ulReservedBit22) : 1; //bit 22
            unsigned int UNIQUENAME(ulReservedBit23) : 1; //bit 23
            unsigned int UNIQUENAME(ulReservedBit24) : 1; //bit 24

            unsigned int UNIQUENAME(ulReservedBit25) : 1; // bit 25
            unsigned int UNIQUENAME(ulReservedBit26) : 1; // bit 26
            unsigned int UNIQUENAME(ulReservedBit27) : 1; // bit 27

            unsigned int bPipeA_Plane4FlipDoneInterrupt : 1; // bit 28
            unsigned int bPipeB_Plane4FlipDoneInterrupt : 1; // bit 29
            unsigned int bPSR2GTCLockLoss : 1; // bit 30
            unsigned int bVECSWatchDogCounterExcd : 1; // bit 31
        };
    };
    union
    {
        unsigned int ulValue4;
        unsigned int Value4;
        struct
        {
            unsigned int UNIQUENAME(ulReservedBit0) : 1; // bit 0
            unsigned int UNIQUENAME(ulReservedBit1) : 1; // bit 1
            unsigned int UNIQUENAME(ulReservedBit2) : 1; // bit 2
            unsigned int UNIQUENAME(ulReservedBit3) : 1; // bit 3

            unsigned int bISPInterrupt : 1; // bit 4
            unsigned int UNIQUENAME(ulReservedBit5) : 1; // bit 5
            unsigned int UNIQUENAME(ulReservedBit6) : 1; // bit 6
            unsigned int UNIQUENAME(ulReservedBit7) : 1; // bit 7

            unsigned int UNIQUENAME(ulReservedBit8) : 1; // bit 8
            unsigned int UNIQUENAME(ulReservedBit9) : 1; // bit 9
            unsigned int UNIQUENAME(ulReservedBit10) : 1; //bit 10
            unsigned int bWDBoxEndofFrameInterrupt : 1; //bit 11 corresponds to WDBOX_END_OF_FRAME_INTERRUPT = 0X800, in INTERRUPT_UNION_VALUE_4

            unsigned int bIntDP_HDMIEInterrupt : 1; // Bit 12// skl ddi - e hot plug interrupt
            unsigned int bIntDP_HDMIE_SPInterrupt : 1; // Bit 13
            unsigned int bRenderTDLRetryInterrupt : 1; // bit 14
            unsigned int bPinningContextSwitch : 1; // Bit 15
            unsigned int bPinningUserInterrupt : 1; // Bit 16
            unsigned int bDEMisc_WDCombinedInterrupt : 1; // bit 17 corresponds to DEMISC_WD_COMBINED_INTERRUPT  = 0x20000, in INTERRUPT_UNION_VALUE_4

            unsigned int bPipeA_Underrun : 1; // bit 18
            unsigned int bPipeB_Underrun : 1; // bit 19
            unsigned int bPipeC_Underrun : 1; // bit 20
            unsigned int bPipeC_Plane4FlipDoneInterrupt : 1; // bit 21
            unsigned int UNIQUENAME(ulReservedBit22) : 1; // bit 22
            unsigned int UNIQUENAME(ulReservedBit23) : 1; // bit 23
            unsigned int UNIQUENAME(ulReservedBit24) : 1; // BIT 24
            unsigned int bVBlank_PipeDInterrupt : 1; // BIT 25
            unsigned int bIntDP_HDMIFInterrupt : 1; // Bit 26// ddi - f hot plug interrupt
            unsigned int bIntDP_HDMIF_SPInterrupt : 1; // Bit 27
            unsigned int bWDBox2Interrupt : 1; // bit 28
            unsigned int bWDBox2EndofFrameInterrupt : 1; // bit 29
            unsigned int bDEMisc_DbufOverlapDetected : 1; // bit 30
            unsigned int bDEMisc_RMTimeoutInterrupt : 1; // bit 31
        };
    };
    union
    {
        unsigned int ulValue5;
        unsigned int Value5;
        struct
        {
            unsigned int bPipeA_Plane1GttFaultInterrupt : 1; // bit 0
            unsigned int bPipeA_Plane2GttFaultInterrupt : 1; // bit 1
            unsigned int bPipeA_Plane3GttFaultInterrupt : 1; // bit 2
            unsigned int bPipeA_Plane4GttFaultInterrupt : 1; // bit 3
            unsigned int bPipeA_CursorGttFaultInterrupt : 1; // bit 4

            unsigned int bPipeB_Plane1GttFaultInterrupt : 1; // bit 5
            unsigned int bPipeB_Plane2GttFaultInterrupt : 1; // bit 6
            unsigned int bPipeB_Plane3GttFaultInterrupt : 1; // bit 7
            unsigned int bPipeB_Plane4GttFaultInterrupt : 1; // bit 8
            unsigned int bPipeB_CursorGttFaultInterrupt : 1; // bit 9

            unsigned int bPipeC_Plane1GttFaultInterrupt : 1; // bit 10
            unsigned int bPipeC_Plane2GttFaultInterrupt : 1; // bit 11
            unsigned int bPipeC_Plane3GttFaultInterrupt : 1; // bit 12
            unsigned int bPipeC_Plane4GttFaultInterrupt : 1; // bit 13
            unsigned int bPipeC_CursorGttFaultInterrupt : 1; // bit 14

            unsigned int IntDP_HDMIB_SCDCInterrupt : 1; //bit 15
            unsigned int IntDP_HDMIC_SCDCInterrupt : 1; //bit 16
            unsigned int IntDP_HDMID_SCDCInterrupt : 1; //bit 17
            unsigned int IntDP_HDMIE_SCDCInterrupt : 1; //bit 18
            unsigned int IntDP_HDMIF_SCDCInterrupt : 1; //bit 19

            unsigned int bPipeA_Plane5FlipDoneInterrupt : 1; // bit 20
            unsigned int UNIQUENAME(ulReservedBit21) : 1; // bit 21
            unsigned int UNIQUENAME(ulReservedBit22) : 1; // bit 22
            unsigned int bPipeB_Plane5FlipDoneInterrupt : 1; // bit 23
            unsigned int UNIQUENAME(ulReservedBit24) : 1; // bit 24
            unsigned int UNIQUENAME(ulReservedBit25) : 1; // bit 25
            unsigned int bPipeC_Plane5FlipDoneInterrupt : 1; // bit 26
            unsigned int UNIQUENAME(ulReservedBit27) : 1; // bit 27
            unsigned int UNIQUENAME(ulReservedBit28) : 1; // bit 28
            unsigned int bPipeD_Plane5FlipDoneInterrupt : 1; // bit 29
            unsigned int UNIQUENAME(ulReservedBit30) : 1; // bit 30
            unsigned int UNIQUENAME(ulReservedBit31) : 1; // bit 31
        };
    };
    union
    {
        unsigned int ulValue6;
        unsigned int Value6;
        struct
        {
            unsigned int bPipeA_Plane5GttFaultInterrupt : 1; // bit 0
            unsigned int bPipeA_Plane6GttFaultInterrupt : 1; // bit 1
            unsigned int bPipeA_Plane7GttFaultInterrupt : 1; // bit 2
            unsigned int bPipeB_Plane5GttFaultInterrupt : 1; // bit 3
            unsigned int bPipeB_Plane6GttFaultInterrupt : 1; // bit 4
            unsigned int bPipeB_Plane7GttFaultInterrupt : 1; // bit 5
            unsigned int bPipeC_Plane5GttFaultInterrupt : 1; // bit 6
            unsigned int bPipeC_Plane6GttFaultInterrupt : 1; // bit 7
            unsigned int bPipeC_Plane7GttFaultInterrupt : 1; // bit 8
            unsigned int bPipeD_Plane5GttFaultInterrupt : 1; // bit 9
            unsigned int bPipeD_Plane6GttFaultInterrupt : 1; // bit 10
            unsigned int bPipeD_Plane7GttFaultInterrupt : 1; // bit 11
            unsigned int bPipeD_Plane1FlipDoneInterrupt : 1; //bit 12
            unsigned int bPipeD_Plane2FlipDoneInterrupt : 1; //bit 13
            unsigned int bPipeD_Plane3FlipDoneInterrupt : 1; //bit 14
            unsigned int bPipeD_Plane4FlipDoneInterrupt : 1; //bit 15
            unsigned int bPipeD_Plane1GttFaultInterrupt : 1; // bit 16
            unsigned int bPipeD_Plane2GttFaultInterrupt : 1; // bit 17
            unsigned int bPipeD_Plane3GttFaultInterrupt : 1; // bit 18
            unsigned int bPipeD_Plane4GttFaultInterrupt : 1; // bit 19
            unsigned int bPipeD_CursorGttFaultInterrupt : 1; // bit 20
            unsigned int UNIQUENAME(ulReservedBit21) : 1; // bit 21
            unsigned int bCRC_Error_PipeDInterrupt : 1; // bit 22
            unsigned int bCRC_Done_PipeDInterrupt : 1; // bit 23
            unsigned int bPipeD_Underrun : 1; // bit 24
            unsigned int bAudioHDCPRequestInterruptD : 1; // bit 25
            unsigned int IntDP_HDMIA_SCDCInterrupt : 1; // bit 26
            unsigned int UNIQUENAME(ulReservedBit27) : 1; // bit 27
            unsigned int UNIQUENAME(ulReservedBit28) : 1; // bit 28
            unsigned int UNIQUENAME(ulReservedBit29) : 1; // bit 29
            unsigned int UNIQUENAME(ulReservedBit30) : 1; // bit 30
            unsigned int ulReserved_Bits31_ulValue6 : 1; // bits 31
        };
    };
    union
    {
        unsigned int ulValue7;
        unsigned int Value7;
        struct
        {
            unsigned int bPipeA_ScanLineEvent : 1; //bit 0
            unsigned int bPipeB_ScanLineEvent : 1; //bit 1
            unsigned int bPipeC_ScanLineEvent : 1; //bit 2
            unsigned int bPipeD_ScanLineEvent : 1; //bit 3
            unsigned int bKVMR_RequestDisplayInterrupt : 1; // bit 4
            unsigned int bKVMR_ReleaseDisplayInterrupt : 1; // bit 5
            unsigned int bIntDP_HDMIGInterrupt : 1; // Bit 6// ddi - g hot plug interrupt
            unsigned int bIntDP_HDMIG_SPInterrupt : 1; // Bit 7
            unsigned int bIntDP_HDMIHInterrupt : 1; // Bit 8// ddi - h hot plug interrupt
            unsigned int bIntDP_HDMIH_SPInterrupt : 1; // Bit 9
            unsigned int bIntDP_HDMIIInterrupt : 1; // Bit 10// ddi - i hot plug interrupt
            unsigned int bIntDP_HDMII_SPInterrupt : 1; // Bit 11
            unsigned int IntDP_HDMIG_SCDCInterrupt : 1; // Bit 12
            unsigned int IntDP_HDMIH_SCDCInterrupt : 1; // Bit 13
            unsigned int IntDP_HDMII_SCDCInterrupt : 1; // Bit 14
            unsigned int PIPEA_IscoRspInterrupt : 1; // bit 15
            unsigned int PIPEB_IscoRspInterrupt : 1; // bit 16
            unsigned int PIPEC_IscoRspInterrupt : 1; // bit 17
            unsigned int PIPED_IscoRspInterrupt : 1; // bit 18
            unsigned int bThermalPanicModeInterrupt : 1; // bit 19
            unsigned int ulReserved_Bits_ulValue4 : 3; // bit 20 to bit 22
            unsigned int bDsi0Te : 1; // bit 23
            unsigned int bDsi1Te : 1; // bit 24
            unsigned int ulReserved_Bits_ulValue7 : 7; // bits 25 to 31

        };
    };
    union
    {
        unsigned int ulValue8;
        unsigned int Value8;
        struct
        {
            unsigned int MainDmcInterrupt : 1;
            unsigned int PipeADmcFlipQInterrupt : 1;
            unsigned int PipeBDmcFlipQInterrupt : 1;
            unsigned int PipeCDmcFlipQInterrupt : 1;
            unsigned int PipeDDmcFlipQInterrupt : 1;
            unsigned int MainDmcErrorInterrupt : 1;
            unsigned int PipeADmcErrorInterrupt : 1;
            unsigned int PipeBDmcErrorInterrupt : 1;
            unsigned int PipeCDmcErrorInterrupt : 1;
            unsigned int PipeDDmcErrorInterrupt : 1;
            unsigned int PipeADmcRmNoClaimError : 1;
            unsigned int PipeBDmcRmNoClaimError : 1;
            unsigned int PipeCDmcRmNoClaimError : 1;
            unsigned int PipeDDmcRmNoClaimError : 1;
            unsigned int UNIQUENAME(Reserved) : 18;
        };
    };

    union
    {
        unsigned int ulValue9;
        unsigned int Value9;
        struct
        {
            unsigned int CscHeci1Interrupt : 1;
            unsigned int CscHeci2Interrupt : 1;
            unsigned int UNIQUENAME(Reserved) : 30;
        };
    };

    union
    {
        unsigned int ulValue10;
        unsigned int Value10;
        struct
        {
            unsigned int SocHostToDevMemInterrupt : 1;
            unsigned int UNIQUENAME(Reserved) : 31;
        };
    };
}SB_INTERRUPT_ARGS, * PSB_INTERRUPT_ARGS;

// TODO: C_ASSERT(sizeof(SB_INTERRUPT_ARGS) == (15 * sizeof(unsigned int)));  // Ensure no one added too many bits in one of the unsigned int fields
#endif // _WIN32

//
//  Colorspace type
//
typedef enum _SB_COLORSPACE_TYPES
{
    eInvalidSpace = -1,
    eSRGB = 0, // default case, use 709 primaries for HD & 601 for SD
    // for future use only
    eYCrCb601, // YCrCb 601 output
    eYCrCb709, // YCrCb 709 output
    eYCrCb601_xvYCC, // extended, primaries are the same as non-xvYCC ones, for use with ILK
    eYCrCb709_xvYCC, // extended, primaries are the same as non-xvYCC ones, for use with ILK
    eYCrCb2020,
    eRGB2020,
    eMaxColorSpace,
    // Custom primaries
    eCustomColorSpace = 0xFF, // HP RCR scenario
    SB_COLORSPACE_TYPES_FORCE_ULONG = 0x7fffffff
}SB_COLORSPACE_TYPES, * PSB_COLORSPACE_TYPES;

//
// Per color chromaticity corrdinat
//
typedef struct _SB_CHROMATICITY_COORDINATE
{
    // Valid values 0-1023
    // Actual floating value = SB returned value / 1024;
    // E.g. For a value of usXScaledBy1024 = 1001110001b (=625), actual X coordinate is 625/1024 (=0.6103515625)
    uint16_t usXScaledBy1024;
    uint16_t usYScaledBy1024;
}SB_CHROMATICITY_COORDINATE, * PSB_CHROMATICITY_COORDINATE;


//
// Display chormaticity details
//
typedef struct _SB_DISPLAY_CHROMATICITY_DETAILS
{
    //Narrow Gamut Enabled
    OUT uint8_t                    bNarrowGamutChromacitySupported;

    OUT uint8_t                    bOverrideEDIDChromaticityValues;

    uint8_t                        Padding[2];

    // Indicates sRGB/601/709/xvYCC709/xvYCC601 etc.
    OUT SB_COLORSPACE_TYPES eDisplayColorSpace;

    // Chromaticity details for red/green/blue/white point
    // Valid iff eDisplayColorSpace != eInvalidSpace
    OUT SB_CHROMATICITY_COORDINATE Red;

    OUT SB_CHROMATICITY_COORDINATE Green;

    OUT SB_CHROMATICITY_COORDINATE Blue;

    OUT SB_CHROMATICITY_COORDINATE White; // white point
}SB_DISPLAY_CHROMATICITY_DETAILS, * PSB_DISPLAY_CHROMATICITY_DETAILS;

#ifdef _WIN32
// CUI is using this data structure from here

//  MODE_FLAGS structure used by MODE_INFO structure
typedef  union _MODE_FLAGS {
    unsigned int  ulFlags;
    struct {
        unsigned bInterlaced : 1;  // Bit 0, 1 = Interlaced mode
        unsigned bDoubleWideMode : 1;  // Bit 1, 1 = Double-wide mode
        unsigned ucUnsupportedMode : 2;  // Bits 3:2, Non-monitor supported special mode
        // e.g. GTF modes for non-GTF supported monitor
        // 0 = Supported on Both
        // 1 = Not supported on Primary UID
        // 2 = Not supported on Non-Primary UID
        // 3 = Not supported on Both
        unsigned ucListIntersection : 2; // Bits 5:4, Mode occurs in mode tables for
        // DISPLAY_LIST display devices
        // 0 = Occurs on both with same timing
        // 1 = Occurs on Primary UID
        // 2 = Occurs on Non-Primary UID
        // 3 = Occurs on Both, but with different timings
        unsigned bAboveMaxRes : 1;  // Bit 6, 1 = Mode resolution is above
        // monitor's maximum resolution
        unsigned bBelowMinRes : 1;  // Bit 7, 1 = Mode resolution is below
        // monitor's minimum resolution
        unsigned bModeInEdid : 1;  // Mode present in Display's Edid
        unsigned bModeIsPreferred : 1;  // Mode is a Preferred Mode if Set
        unsigned bIsTimingFromOEM : 1;  // Mode is OEM customized & got via INF/VBT.
        unsigned bModeAddedbyOS : 1;  // Mode added by OS
        unsigned bModeAddedOnlybyOS : 1;  // Mode exclusively added by OS
        unsigned bS3DFramePackedMode : 1;  // S3D mode in frame packed format as per HDMI 1.4/DP1.2
        unsigned bS3DSideBySideHalfMode : 1;  // S3D mode in page flipping format
        unsigned bS3DTopBottomMode : 1;  // S3D mode in page flipping format
        unsigned bS3DSideBySideFullMode : 1;  // S3D mode in page flipping format
        unsigned bS3DMode : 1;     //Bit Indicating whether the given mode is S3D
        unsigned bReserved : 14; // Bits 15:31, Reserved
    };
} MODE_FLAGS, * PMODE_FLAGS;


// Enum added for the updating of plane pixel format
// DDK/OS independent values defined from SB
// Union of all supported source pixel formats of GMCH
// Order is LSB-->MSB (as in bspec)
// Only non-alpha formats (ones with X) are valid for set mode operation with >8bpp
typedef enum _SB_PIXELFORMAT
{
    SB_UNINITIALIZED = 0,   // use default pixel format in this case for setmode (e.g. XP might always set this)
    // SB_8BPP_INDEXED for 8bpp, SB_B5G6R5X0 for 16bpp, SB_B8G8R8X8 for 32bpp
    //Keep in the order of increasing BPP
    //Update min, max below if any other format is added
    SB_8BPP_INDEXED,        // for 8bpp
    //Keep in the order of increasing BPP
    //Update min, max below if any other format is added
    SB_B5G6R5X0,            // for 16bpp
    //Keep in the order of increasing BPP
    //Update min, max below if any other format is added
    SB_B8G8R8X8,            // for 32bpp (default)
    SB_B8G8R8A8,
    SB_R8G8B8X8,
    SB_R8G8B8A8,
    SB_R10G10B10X2,         // for 32bpp 10bpc
    SB_R10G10B10A2,         // for 32bpp 10bpc
    SB_B10G10R10X2,         // for 32bpp 10bpc
    SB_B10G10R10A2,         // for 32bpp 10bpc

    SB_R10G10B10A2_XR_BIAS, // for 32bpp 10bpc, XR BIAS format (used by Win7)
    //Keep in the order of increasing BPP
    //Update min, max below if any other format is added
    SB_R16G16B16X16F,       // for 64bpp, 16bit floating
    SB_R16G16B16A16F,       // for 64bpp, 16bit floating
    //Keep in the order of increasing BPP
    //Update min, max below if any other format is added
    //Adding formats used ONLY for MPO, adding at the last to reduce impact
    //Macros below will be updated for them only where it is really required
    SB_MAX_PIXELFORMAT, // Last one - just for internal bitmask usage

    SB_NV12YUV420,
    SB_YUV422,
    SB_P010YUV420,
    SB_P012YUV420,
    SB_P016YUV420,
    SB_YUV444_10,

    SB_YUV422_10,
    SB_YUV422_12,
    SB_YUV422_16,
    SB_YUV444_8,
    SB_YUV444_12,
    SB_YUV444_16,
    SB_MAXALL_PIXELFORMAT,
} SB_PIXELFORMAT, * PSB_PIXELFORMAT;

//  MODE_INFO data structure
typedef struct _MODE_INFO
{
    unsigned short  usXResolution;      // X resolution in pixels
    unsigned short  usYResolution;      // Y resolution in pixels
    unsigned short  usColorBPP;         // Color depth in bits/pixel
    unsigned short  usRefreshRate;      // Vertical refresh rate in Hz
    MODE_FLAGS  flFlags;        // Mode flags

    IN_OUT SB_PIXELFORMAT eSourcePixelFormat; // Pixel format for this mode
    unsigned short usBlankActiveLines; // valid iff flFlags.bS3DFramePackedMode is set
} MODE_INFO, * PMODE_INFO;

typedef enum _SHARED_GFX_POWER_COMPONENT_FSTATE
{
    SGPC_FSTATE_UNKNOWN = -1,
    SGPC_FSTATE_F0,
    SGPC_FSTATE_F1
} SHARED_GFX_POWER_COMPONENT_FSTATE;

#endif // _WIN32
#endif // LOG_ENTRY_ENUMS

typedef enum _PLANE_ORIENTATION
{
    ORIENTATION_0 = 0,                              // 0 degree
    ORIENTATION_DEFAULT = ORIENTATION_0,            // Default value
    ORIENTATION_90 = 1,                             // 90 degree, supported Gen9 onwards
    ORIENTATION_180 = 2,                            // 180 degree
    ORIENTATION_270 = 3,                            // 270 degree, supported Gen9 onwards
    ORIENTATION_MAX = 4,
}PLANE_ORIENTATION;
