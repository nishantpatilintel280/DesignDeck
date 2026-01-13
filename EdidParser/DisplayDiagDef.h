//===========================================================================
//
//   Copyright (c) Intel Corporation (2000 - 2019)
//
//   INTEL MAKES NO WARRANTY OF ANY KIND REGARDING THE CODE.  THIS CODE IS LICENSED
//   ON AN "AS IS" BASIS AND INTEL WILL NOT PROVIDE ANY SUPPORT, ASSISTANCE,
//   INSTALLATION, TRAINING OR OTHER SERVICES.  INTEL DOES NOT PROVIDE ANY UPDATES,
//   ENHANCEMENTS OR EXTENSIONS.  INTEL SPECIFICALLY DISCLAIMS ANY WARRANTY OF
//   MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR ANY PARTICULAR PURPOSE, OR ANY
//   OTHER WARRANTY.  Intel disclaims all liability, including liability for
//   infringement of any proprietary rights, relating to use of the code. No license,
//   express or implied, by estoppel or otherwise, to any intellectual property
//   rights is granted herein.
//
//--------------------------------------------------------------------------

/**
 *
 * @file  DisplayDiagDef.h
 * @brief List of Display diagnostic codes to be used by 3 layers of Display driver
 *
 */

#ifndef _DISPLAY_DIAG_H_
#define _DISPLAY_DIAG_H_

#pragma once

#pragma pack(1)

#ifndef _DISPLAY_INTERNAL_
#define __BIT(x) (1UL << (x))
#endif

#define MAX_INTERNAL_DISPLAYS 2

/**
 * @brief @c DD_DIAGNOSTIC_SOURCE code used across all display driver modules to
 * specify the diagnostic event source.
 *
 * The error code has below major categories+
 * 1. Catastrophic failures leading to MayDay. System maynot survide a Catastro
 * 2. Error conditions which ideally should not happen. But the system can fucntion
 * 3. DPL (Display Protocol layer) error
 * 4. DHL (Display HW-Abstraction layer) error
 * 5. Success codes.
 */

// Do not change the ordering of the enums.
// This definition is pulled as is in to the Manifest
// Every diagnostic data is associated with optional 4 DDU32 data
// have a comment to each entry to get the information displayed in the Viewer

typedef enum _DD_DIAGNOSTIC_SOURCE
{
    // Error. Something failed and the end used saw the after effect(Flicker\Blank out).
    // But the system is still stable
    DD_DIAG_ERR = 0x1,
    // Mode Set Failed, DisplayID, PipeIndex, X Res, Dot Clock.
    DD_DIAG_ERR_MODESET_FAIL = 0x10000001,
    // Link Training Failed, DisplayID, Rate, TBD, TBD.
    DD_DIAG_ERR_LINK_TRAINING_FAIL = 0x10000002,
    // Polled read timeout/failure, Offset, Bits missed expectation, Timeout Value, LastRead Value
    DD_DIAG_ERR_MMIO_POLLED_READ_FAIL = 0x10000003,

    // PCU mailbox polled read failure, MailBoxCmd, Bits missed expectation, Timeout Value, Last ReadResult
    DD_DIAG_ERR_PCU_MAILBOX_POLLED_READ_FAIL = 0x10000004,

    // Scanline stopped updating. Pipe, FrameCount, Scanline, Timeout
    DD_DIAG_ERR_SCANOUT_HUNG = 0x10000005,
    // Empty Modelist for a Target. TargetId, TBD, TBD, TBD
    DD_DIAG_ERR_EMPTY_MODELIST = 0x10000006,
    // Flip failed for a Source. SourceId, NTSTATUS, DDSTATUS, TBD
    DD_DIAG_ERR_FLIP_FAILED = 0x10000007,
    // Wrong target mode pinned. Pinned HActive, Actual HActive, Pinned VActive, Actual VActive
    DD_DIAG_WRONG_MODE_PINNED = 0x10000008,
    // SystemDisplayEnable means going to BSOD. TBD, TBD, TBD, TBD
    DD_DIAG_ERR_BSOD = 0x10000009,
    // Lid State mismatch between opregion and ACPI. ACPI Lid State, Opregion Lid State, TBD, TBD.
    DD_DIAG_ERR_LID_STATE_MISMATCH = 0x1000000A,
    // Unexpected DDIs called by OS in Linkloss, TargetLinkState, Source id, TargetId, NA
    DD_DIAG_ERR_INVALID_DDI_IN_LINKLOSS = 0x1000000B,
    // Not expected to be called on inactive path. VidPnSrcId, TargetId, TBD, TBD
    DD_DIAG_ERR_INVALID_DDI_ON_INACTIVE_DISPLAY = 0x1000000C,
    // Log buffer is not set, but OS gave a flip on this layer. VidPnSrcId, TargetId, LayerIndex, TBD
    DD_DIAG_ERR_LOG_BUFFER_NOT_SET = 0x1000000D,
    // Dsb is hung, Scanline Count, Frame Count, DsbCurrentHeadPtr, PipeId
    DD_DIAG_ERR_DSB_HUNG = 0x1000000E,
    // Pipe Id, IsPipeActive, PC Display Caps Is Valid, NA
    DD_DIAG_ERR_INVALID_PC_DISPLAY_CAPS = 0x1000000F,
    // TargetId, NA
    DD_DIAG_ERR_INVALID_DISPLAY_CAPS = 0x10000010,

    // Warning. Something failed. But the user didn't see the effect
    DD_DIAG_WRN = 0x2,
    // Insufficinet DBuf to enable the planes on a Display, PipeIndex, DBuf Needed, DBuf Available, N Planes.
    DD_DIAG_WRN_INSUFFICIENT_DBUF = 0x20000001,
    // Flip call with Target disabled reported as success to OS (Workaround), SourceId, TBD, TBD, TBD
    DD_DIAG_WRN_FLIP_DISABLED_TARGET = 0x20000002,
    // Wait for active region check is taking more than 1 frame time. Pipe, WaitTime, Scanline, FrameTime
    DD_DIAG_WRN_SCANOUT_DELAYED = 0x20000003,
    // Warning diag when draining Flips internally, if flip entries are in future timestamp. PipeId, IsSwQ, PlaneId, Flip PTS
    DD_DIAG_WRN_FUTURE_TSTAMP_DURING_DRAIN = 0x20000004,
    // VidPn not supported for the topology given by ISV
    DD_DIAG_WRN_ISV_NOT_SUPPORTED = 0x20000005,
    // Invalid PSR2 guardband interrupt delay regkey
    DD_DIAG_WRN_INVALID_PSR2_GUARDBAND_INTERRUPT_DELAY = 0x20000006,

    // Info. Helpful debuf info. Routine data collection.
    // Will get handly when debugging the CAT, ERR and WRN cases
    DD_DIAG_INF = 0x3,

    // Diagnostics related to the function execution time
    // Analyzer will use these clues to dig into the performance issues

    // Polled MMIO Read, Register, Mask, Value, TBD.
    DD_DIAG_INF_POLLED_READ = 0x30000001,
    // Delay introduced by stalling CPU, Delay time in us, TBD, TBD, TBD.
    DD_DIAG_INF_DELAY_STALL_CPU = 0x30000003,
    // Delay introduced by delaying thread, Delay time in us, TBD, TBD, TBD.
    DD_DIAG_INF_DELAY_EXECUTION_THREAD = 0x30000004,
    // DDI Entry, DDI, TBD, TBD, TBD.
    DD_DIAG_INF_DDI_ENTRY = 0x30000005,
    // DDI Exit, DDI, Status, TBD, TBD.
    DD_DIAG_INF_DDI_EXIT = 0x30000006,
    // DPC Entry, TBD, TBD, TBD, TBD.
    DD_DIAG_INF_DPC_ENTRY = 0x30000007,
    // DPC Exit, TBD, Status, TBD, TBD.
    DD_DIAG_INF_INSERT_DPC = 0x30000008,
    // DPC Entry, TBD, TBD, TBD, TBD.
    DD_DIAG_INF_INSERT_WI = 0x30000009,
    // DPC Exit, TBD, Status, TBD, TBD.
    DD_DIAG_INF_DPC_EXIT = 0x3000000A,
    // Work Item Entry, TBD, TBD, TBD, TBD.
    DD_DIAG_INF_WI_ENTRY = 0x3000000B,
    // Work Item Exit, TBD, Status, TBD, TBD.
    DD_DIAG_INF_WI_EXIT = 0x3000000C,
    // Lock Entry, TBD, TBD, TBD, TBD.
    DD_DIAG_INF_LOCK_ENTRY = 0x3000000D,
    // Lock Exit, TBD, Status, TBD, TBD.
    DD_DIAG_INF_LOCK_EXIT = 0x3000000E,

    // Diagnostic Header, Start/Stop, Major Ver, Minor Ver, TBD.
    DD_DIAG_INF_HEADER = 0x3000000F,
    // Instrumentation Header, Start/Stop, TBD, TBD, TBD.
    DD_DIAG_INRUMENTATION_HEADER = 0x30000010,
    // Mem Allocation Start, Size, IsPageable, IRQL, Flags.
    DD_DIAG_MEM_ALLOC_START = 0x30000011,
    // Mem Allocation Done, TBD, TBD, TBD, TBD.
    DD_DIAG_MEM_ALLOC_DONE = 0x30000012,
    // Mem Free Start, TBD, TBD, IRQL, TBD.
    DD_DIAG_MEM_FREE_START = 0x30000013,
    // Mem Free Done, TBD, TBD, TBD, TBD.
    DD_DIAG_MEM_FREE_DONE = 0x30000014,
    // Mem Allocation Start, TBD, TBD, IRQL, TBD.
    DD_DIAG_MEM_ALLOC_LOOKASIDE_START = 0x30000015,
    // Mem Allocation Done, TBD, TBD, TBD, TBD.
    DD_DIAG_MEM_ALLOC_LOOKASIDE_DONE = 0x30000016,
    // Wait for VBlank Entry, PipeID, Current Frame Counter, Scanline count, TBD.
    DD_DIAG_WAIT_FOR_VBLANK_ENTRY = 0x30000017,
    // Wait for VBlank Exit, PipeID, Next Frame Counter, Scanline count, TBD.
    DD_DIAG_WAIT_FOR_VBLANK_EXIT = 0x30000018,
    // SafeScanLineWait Entry, PipeID, SafeScanLineCount, Current scanline count, TBD
    DD_DIAG_WAIT_FOR_SAFE_SCANLINE_ENTRY = 0x30000019,
    // SafeScanLineWait Exit, PipeID, SafeScanLineCount, Current scanline count, TBD
    DD_DIAG_WAIT_FOR_SAFE_SCANLINE_EXIT = 0x3000001A,
    // Sync Exec Entry, TBD, TBD, TBD, TBD.
    DD_DIAG_INF_SYNC_EXEC_ENTRY = 0x3000001B,
    // Sync Exec Exit, TBD, Status, TBD, TBD.
    DD_DIAG_INF_SYNC_EXEC_EXIT = 0x3000001C,
    // Port, Current Time in ms (High), Current Time in ms (Low), NA
    DD_DIAG_INF_SWDPST_TIMER_START = 0x3000001D,
    // Port, Current Time in ms (High), Current Time in ms (Low), NA
    DD_DIAG_INF_SWDPST_TIMER_CANCEL = 0x3000001E,

    // SafeScanLineWait Entry, PipeID, SafeScanLineCount, Current scanline count, TBD
    DD_DIAG_WAIT_FOR_ACTIVE_SCANLINE_ENTRY = 0x30000021,
    // SafeScanLineWait Exit, PipeID, SafeScanLineCount, Current scanline count, TBD
    DD_DIAG_WAIT_FOR_ACTIVE_SCANLINE_EXIT = 0x30000022,

    // Polled read timeout/failure, Offset, Bits missed expectation, Timeout Value, LastRead Value
    DD_DIAG_INF_MMIO_POLLED_READ_INFO = 0x30000023,
    // PCU mailbox polled read failure, MailBoxCmd, Bits missed expectation, Timeout Value, Last ReadResult
    DD_DIAG_INF_PCU_MAILBOX_POLLED_READ_INFO = 0x30000024,
    // Polled read for scanline/framecount increment entry, Current Scanline Count, Current Frame Count, Timeout, TBD
    DD_DIAG_INF_MMIO_POLLED_READ_SCANLINE_FRAME_COUNT_ENTRY = 0x30000025,
    // Polled read for scanline/framecount increment exit, Next Scanline Count, Next Frame Count, Timeout, TBD
    DD_DIAG_INF_MMIO_POLLED_READ_SCANLINE_FRAME_COUNT_EXIT = 0x30000026,

    // DsbWaitTillIdle Entry, Scanline Count, Frame Count, OneFrameTimeInUs, PipeId
    DD_DIAG_DSB_WAIT_FOR_IDLE_ENTRY = 0x30000027,
    // PSR CAPS CHANGE EVENT, Port, Spi Flags, TBD, TBD.
    DD_DIAG_INF_PSR_CAPS_CHANGE_EVENT = 0x30000028,

    // Flip Related diagnostics

    // Check MPO Passed, N Pipes, N Planes, TBD, TBD.
    DD_DIAG_INF_CHECK_MPO_PASSED = 0x30010001,
    // Check MPO Failed, N Pipes, N Planes, Failing Plane, Reason_TBD.
    DD_DIAG_INF_CHECK_MPO_FAILED = 0x30010002,
    // Flip Retry Required, SourceId, DD_FLIP_RETRY_REASON , TBD, TBD
    DD_DIAG_INF_FLIP_RETRY = 0x30010003,
    // HAL Cancel All Entries, PipeId, HwPlaneId, HwBaseAddress, DD_FLIPQ_MODIFY_REQUEST_TYPE
    DD_DIAG_INF_CANCELLED_ALL_ENTRIES = 0x30010004,

    // Generic Display Events
    // Transcoder Enable, Port, SinkType, Status, Pipe.
    DD_DIAG_TRANS_ENABLE = 0x30003001,
    // Transcoder Disable, Port, SinkType, Status, Pipe.
    DD_DIAG_TRANS_DISABLE = 0x30003002,
    // Detect Display, Port, Detected Protocol, ConnectionEvent, TBD, TBD.
    DD_DIAG_DETECTED_DISPLAY = 0x30003003,
    // PLL Enable, Port, Status, PllType, TBD.
    DD_DIAG_PLL_ENABLE = 0x30003004,
    // PLL Disable, Port, Status, TBD, TBD.
    DD_DIAG_PLL_DISABLE = 0x30003005,
    // Port Enable, Port, TBD, TBD, TBD.
    DD_DIAG_PORT_ENABLE = 0x30003006,
    // Port Disable, Port, TBD, TBD, TBD.
    DD_DIAG_PORT_DISABLE = 0x30003007,
    // DotClock, Pipe, RrMuptipliedBy1000, TBD.
    DD_DIAG_DYNAMIC_RR_SWITCH = 0x30003008,
    // TargetId, Port
    DD_DIAG_LINKLOSS_DISP_RESET = 0x30003009,
    // TargetId, Port, Status
    DD_DIAG_LINKLOSS_RECOVERY_RESULT = 0x3000300A,
    // TargetId, Port, isJointTarget
    DD_DIAG_LINKLOSS_OBSERVED = 0x3000300B,
    // TargetId, LinkState, IsDisplayPreserved, SinkType
    DD_DIAG_SETTIMING_RESULT = 0x3000300C,
    // TargetId, Modeslist_len
    DD_DIAG_MODEENUM_LENGTH = 0x3000300D,
    // TargetId, LinkState
    DD_DIAG_DISABLE_PROTCOL_COMPLETE = 0x3000300E,
    // Display Audio Codec Sleep State
    DD_DIAG_DISPLAY_AUDIO_SLEEP_STATE = 0x3000300F,
    // Display End Of SetTiming
    DD_DIAG_DISPLAY_END_OF_SETTIMING = 0x30003010,
    // Detected Port, Tiles found, JointTargetIndex, NA
    DD_DIAG_TILE_PKG_DETECTED = 0x30003011,
    // Port, Primary/Secondary, NA, NA
    DD_DIAG_TILES_IN_PKG = 0x30003012,
    // Port, TargetId, NA, NA
    DD_DIAG_SECONDARY_TILE = 0x30003013,

    // MayDay MayDay MayDay !!! The damage is already done. Who will bell the cat
    // Collect data which will help us identify the cause and take preventive measures
    DD_DIAG_CAT_FAIL = 0x4,
    // Underrun, PipeIndex, UnderRunCount, NA, NA.
    DD_DIAG_CAT_PIPE_UNDER_RUN = 0x40000001,
    // RM Timeout, RmTimeduoutRegisterOffset, NA, NA, NA.
    DD_DIAG_CAT_RM_TIMEOUT = 0x40000002,
} DD_DIAGNOSTIC_SOURCE;

// Don't change the enum values. This will be used in Diana parsing
// When new enums are added here, add comments for what the params are used
// and ensure that the manifest file is updated with the enum.
typedef enum _DD_DP_DIAGNOSTIC_SOURCE
{
    // Link Training start, Port, Repeater count
    DD_DIAG_LINK_TRAINING_START = 0x0,
    // CR Start , Pre-Emphasis, Voltage Swing, Success Flag
    DD_DIAG_CR_START = 0x1,
    // CR End , Pre-Emphasis, Voltage Swing, Success Flag
    DD_DIAG_CR_END = 0x02,
    // EQ Start , Pre-Emphasis, Voltage Swing, Success Flag
    DD_DIAG_EQ_START = 0x03,
    // EQ End , Pre-Emphasis, Voltage Swing, Success Flag
    DD_DIAG_EQ_END = 0x04,
    // Link Training End, Success flag, Link rate, FLT Flag
    DD_DIAG_LINK_TRAINING_END = 0x05,
    // tile display connected
    DD_DIAG_TILE_DISPLAY = 0x06,

    // MNTU programming
    // DataM, DataN, Tu
    DD_DIAG_DATA_MN_TU = 0x07,
    // LinkM, LinkN
    DD_DIAG_LINK_MN = 0x08,
    // PLL programming
    // SSC , DCOInteger, TBD, TBD
    DD_DIAG_PLL_CR0_PARAMS = 0x09,
    // PDiv, KDiv, QDiv, TBD
    DD_DIAG_PLL_CR1_PARAMS = 0x0a,
    // DDI Control
    // BPC, TBD, TBD, TBD
    DD_DIAG_TRANS_DDI_CNTL_PARAMS = 0x0b,

    // StreamID, StartSlot, NumOfSlots, TBD
    DD_MST_VCPAYLOAD_UPDATE = 0x0c,
    // StreamID, Status, PBNReceived, PBNRequired
    DD_DIAG_MST_PAYLOAD_RESULT = 0x0d,

    // Sink Power up
    DD_DIAG_SINK_POWER_UP_START = 0x0e,
    DD_DIAG_SINK_POWER_UP_END   = 0xf,
    // Sink Power down
    DD_DIAG_SINK_POWER_DOWN = 0x10,
    // Remote Sink Power up
    DD_DIAG_REMOTE_SINK_POWER_UP_START = 0x11,
    DD_DIAG_REMOTE_SINK_POWER_UP_END   = 0x12,
    // Remote Sink Power down
    DD_DIAG_REMOTE_SINK_POWER_DOWN = 0x13,

    // USB4 Dynamic BW Mgmt - BW Request Initiated
    // Port, Sink Index, Status
    DD_DIAG_USB4_BW_ALLOCATION_REQUEST_START = 0x14,
    // USB4 Dynamic BW Mgmt - BW Request Completed
    // Port, Sink Index, Status, DataBwMbpsPort
    DD_DIAG_USB4_BW_ALLOCATION_REQUEST_END = 0x15,

} DD_DP_DIAGNOSTIC_SOURCE;

typedef enum _DD_PCON_DIAGNOSTIC_SOURCE
{
    DD_DIAG_PCON_GET_TX_LINK_STATUS = 0x0,
    DD_DIAG_PCON_FRL_LT_START,
    DD_DIAG_PCON_FRL_LT_END,
    DD_DIAG_PCON_LT_START,
    DD_DIAG_PCON_LT_END,

} DD_PCON_DIAGNOSTIC_SOURCE;

// Don't change the enum values. This will be used in Diana parsing
// When new enums are added here, add comments for what the params are used
// and ensure that the manifest file is updated with the enum.
typedef enum _DD_HDMI_DIAGNOSTIC_SOURCE
{
    // HDMI Sink Configuration: enabling scrambling and SCDC interrupt
    DD_DIAG_HDMI_SINK_CONFIG_START = 0x0,
    // HFVSDB Ftr Support
    DD_DIAG_HDMI_SINK_CONFIG_END = 0x1,

    // FRL Link training sequence Start: Port, IsFrl Mode, TmdsCharRate
    DD_DIAG_FRL_LINK_TRAINING_START = 0x2,

    // LTS1 Start:
    DD_DIAG_FRL_LTS1_START = 0x3,
    // LTS1 End: FrlRatePerLane, SCDC sink Version
    DD_DIAG_FRL_LTS1_END = 0x4,
    // LTS2 Start: FrlRatePerLane, NumOfLanes
    DD_DIAG_FRL_LTS2_START = 0x5,
    // LTS2 End: FltReady, FrlRate
    DD_DIAG_FRL_LTS2_END = 0x6,
    // LTS3 Start:
    DD_DIAG_FRL_LTS3_START = 0x7,
    // LTS3 End: FltUpdate, LnXFfeReq[Ln0], LnXFfeReq[Ln1], LnXFfeReq[Ln2], LnXFfeReq[Ln3], Status
    DD_DIAG_FRL_LTS3_END = 0x8,
    // LTSP Start:
    DD_DIAG_FRL_LTSP_START = 0x9,
    // LTSP End:FrlStart, FltUpdate
    DD_DIAG_FRL_LTSP_END = 0xa,

    // FRL Link Training sequence End, IsLinkTrained, NumOfLanes, FrlRatePerLane, Status
    DD_DIAG_FRL_LINK_TRAINING_END = 0xb,
} DD_HDMI_DIAGNOSTIC_SOURCE;

// Don't change the enum values. This will be used in Diana parsing
// When new enums are added here, add comments for what the params are used
// and ensure that the manifest file is updated with the enum.
typedef enum _DD_DIAG_FEATURE_STATE_INFO
{
    // Endurance Gaming: Param1 - FramePeriodInUs
    DD_DIAG_FEATURE_STATE_ENDURANCE_GAMING,

    // Smooth Sync: Param1 - TargetId
    DD_DIAG_FEATURE_STATE_SMOOTH_SYNC,

    // SpeedFrame: Param1 - TargetId
    DD_DIAG_FEATURE_STATE_SPEED_FRAME,

    // Vsync On: Param1 - TargetId
    DD_DIAG_FEATURE_STATE_VSYNC_ON,

    // Vsync Off: Param1 - TargetId
    DD_DIAG_FEATURE_STATE_VSYNC_OFF,

    // Capped Fps: Param1 - TargetId
    DD_DIAG_FEATURE_STATE_CAPPED_FPS,
} DD_DIAG_FEATURE_STATE_INFO;

// Classiffication for the diagnostic information per feature.
// The classification will be used to specify a feature where classification by call stack is not possible
typedef enum _DD_DIAG_FEATURE
{
    DD_DIAG_FEATURE_UNKNOWN           = 0x0,
    DD_DIAG_FEATURE_OPREGION          = 0x1,
    DD_DIAG_FEATURE_ACPI              = 0x2,
    DD_DIAG_FEATURE_MP_COMMON         = 0x3, // Miniport Common
    DD_DIAG_FEATURE_DIAGNOSTICS       = 0x4,
    DD_DIAG_FEATURE_DISPLAY_DETECTION = 0x5, // DETECTION
    DD_DIAG_FEATURE_CURSOR            = 0x6,
    DD_DIAG_FEATURE_DVM               = 0x7, // TILED, JOINER, COLLAGE
    DD_DIAG_FEATURE_RM                = 0x8, // Resource management - WM, DBUF
    DD_DIAG_FEATURE_GAMING            = 0x9, // Smooth sync, speed frame, EG, etc
    DD_DIAG_FEATURE_RR_CHANGE         = 0xA, // VRR, LRR, HRR, NRR etc
    DD_DIAG_FEATURE_MIPI_COMMON       = 0xB,
    DD_DIAG_FEATURE_HDMI_COMMON       = 0xC, // HDMI, 1.4, 2.0, DSC
    DD_DIAG_FEATURE_HDCP              = 0xD,
    DD_DIAG_FEATURE_DP_COMMON         = 0xE, // DP, MST, DSC, PCON
    DD_DIAG_FEATURE_EDID_DID          = 0xF, // EDID, DISPLAY ID
    DD_DIAG_FEATURE_VBT               = 0x10,
    DD_DIAG_FEATURE_AUDIO             = 0x11,
    DD_DIAG_FEATURE_COLOR             = 0x12, // DPST, OPST, GAMMA, CSC, HDR, LACE, 3DLUT
    DD_DIAG_FEATURE_VIRTUAL_DISPLAY   = 0x13,
    DD_DIAG_FEATURE_DMC               = 0x14,
    DD_DIAG_FEATURE_PLANE             = 0x15, // FLIPS, FBC, FORMAT, TILING, COMPRESSION
    DD_DIAG_FEATURE_FLIPQ             = 0x16, // FLIPQ
    DD_DIAG_FEATURE_DSB               = 0x17, // DSB
    DD_DIAG_FEATURE_POWERWELL         = 0x18,
    DD_DIAG_FEATURE_DC_STATE          = 0x19, // DC5,DC6,DC6V,DC9
    DD_DIAG_FEATURE_PSR               = 0x1A, // PSR, SU, PR
    DD_DIAG_FEATURE_BLC               = 0x1B, // B2, B3, DUAL PANEL BRIGHTNESS
    DD_DIAG_FEATURE_MODE_ENUM         = 0x1C, // EVCM, ISV,PRUNING
    DD_DIAG_FEATURE_ESCAPE            = 0x1D,
    DD_DIAG_FEATURE_MODESET           = 0x1E, // SETTIMING, RE-LINKTRAINING
    DD_DIAG_FEATURE_OSL_COMMON        = 0x1F, // Misc OSL
} DD_DIAG_FEATURE;

typedef enum _DD_DIAG_SRVC
{
    DD_DIAG_SRVC_STATE_DUMP, // State Dump at the time of trace start.
    DD_DIAG_SRVC_REGISTER_STATE_DUMP,
    DD_DIAG_SRVC_REGISTER_RANGE_DUMP,
    DD_DIAG_GET_SCANOUT_STATE,
    DD_DIAG_GET_TARGET_MODE_DATA,
    DD_DIAG_GET_EDID_MODE_DATA,
    DD_DIAG_GET_DP_MST_DATA,
    DD_DIAG_SRVC_FLIP_HISTORY,
    DD_DIAG_SRVC_RM_TIMEOUT,
    DD_DIAG_SRVC_MPO_CACHE,
    DD_DIAG_SRVC_TDR_ANALYSIS_PC_CHECK,
    DD_DIAG_SRVC_CONFIG_DIAG_MODE = 0x1000, // Configure the Diagnostic mode. Beware, it can be intrusive

} DD_DIAG_SRVC;

typedef enum _DD_DIAG_SCANLINE_FRAMECOUNTER_STATE
{
    DD_DIAG_SCANLINE_INCREMENTING,           // Scanline incrmenting without any issue.
    DD_DIAG_PIPE_SCANLINE_HUNG,              // Pipe scanline hung
    DD_DIAG_CMTG_SCANLINE_HUNG,              // CMTG scanline hung in DC6v
    DD_DIAG_CMTG_ENABLED_PIPE_SCANLINE_HUNG, // Pipe scanline hung with CMTG enabled
} DD_DIAG_SCANLINE_FRAMECOUNTER_STATE;

typedef union _DD_DIAG_MODE_CONFIG
{
    struct
    {
        DDU64 VBI : 1; // Additional logging on VBI
        DDU64 CRC : 1; // Enable CRC
        DDU64 PSR : 1; // Additional loggin for PSR
    };
    DDU64 Value;
} DD_DIAG_MODE_CONFIG;
C_ASSERT(sizeof(DDU64) == sizeof(DD_DIAG_MODE_CONFIG));

typedef struct _DD_DIAG_REG_DUMP
{
    DD_IN void *pDataBuffer;
    DD_IN DDU32 MaxRegEntries;  // Number of DD_OFFSET_DATA_PAIR that fits in Buffer Available.
    DD_OUT DDU32 NumRegEntries; // Number of reg entries (DD_OFFSET_DATA_PAIR) filled into the buffer.
} DD_DIAG_REG_DUMP;

typedef struct _DD_DIAG_TARGET_DETAILS
{
    PIPE_ID Pipe;
    DDU8 Port;
} DD_DIAG_TARGET_DETAILS;

typedef union _DD_DIAG_SCANOUT_STATE_REQUEST_FLAGS
{
    struct
    {
        DDU8 DarkScreenDetection : DD_BITFIELD_BIT(0);
        DDU8 ScanlineIncrement : DD_BITFIELD_BIT(1);
        DDU8 UNIQUENAME(Reserved) : DD_BITFIELD_RANGE(2, 7);
    };
    DDU8 Value;
} DD_DIAG_SCANOUT_STATE_REQUEST_FLAGS;
C_ASSERT(sizeof(DDU8) == sizeof(DD_DIAG_SCANOUT_STATE_REQUEST_FLAGS));

typedef struct _DD_DIAG_SCANOUT_STATE_DATA
{
    DD_IN DDU32 TimeOutValueInUs; // Used for RequestFlag.ScanlineIncrement
    DD_IN DD_DIAG_SCANOUT_STATE_REQUEST_FLAGS RequestFlags;
    DD_IN DD_BOOL VerifyPortIfCmtgIsEnabled;
    DD_OUT DD_BOOL IsDarkScreenDetected;
    DD_OUT DD_DIAG_SCANLINE_FRAMECOUNTER_STATE ScanlineOrFrameCountState;
} DD_DIAG_SCANOUT_STATE_DATA;

typedef struct _DD_BB_DIAG_MPO_CACHE_DATA
{
    DD_IN DDU8 TimeoutSourceId;
    DD_IN DDU32 BufferSize;
    DD_OUT DDU32 BytesUsed;
    DD_IN void *pDataBuffer;
} DD_BB_DIAG_MPO_CACHE_DATA;

typedef struct _DD_BB_DIAG_BUFFER_DATA
{
    DDU32 BufferSize;
    DDU32 BytesUsed;
    void *pDataBuffer;
} DD_BB_DIAG_BUFFER_DATA;

typedef struct _DD_DIAG_TDR_ANALYSIS_DATA
{
    DD_IN DDU8 NumPipes;
    DD_IN PIPE_ID PipeId[MAX_PHYSICAL_PIPES];
    DD_OUT DD_VSYNC_TIMEOUT_SUBCODE VsyncTimoutCode;
} DD_DIAG_TDR_ANALYSIS_DATA;

typedef struct _DD_DIAG_SRVC_ARGS
{
    // What data to log
    DD_DIAG_SRVC RequestType;
    DD_DIAG_TARGET_DETAILS TargetDetails;

    union
    {
        DD_DIAG_MODE_CONFIG DiagModeConfig;
        DD_DIAG_REG_DUMP RegisterDumpArgs;
        DD_DIAG_SCANOUT_STATE_DATA ScanoutStateData;
        DD_BB_DIAG_MPO_CACHE_DATA DiagMpoCacheData;
        DD_BB_DIAG_BUFFER_DATA DiagBBData;
        DD_DIAG_TDR_ANALYSIS_DATA TdrAnalysisData;
    };
} DD_DIAG_SRVC_ARGS;

/////////////////////////////////////////////////////////////////////////
//
// error code handling macros
//

/// returns true if the code is a catastrophic error
#define IS_DDDIAG_CAT(DiagCode) ((DiagCode >> 28) == DD_DIAG_CAT_FAIL)

#define DISP_EVENT_VBI_CRC(PipeID, pBDF)                                \
    switch (PipeID)                                                     \
    {                                                                   \
    case PIPE_A:                                                        \
        DD_EVENT_VBLANKINTERRUPT_PIPEA(NULL, PipeID, 0, 0, pBDF);       \
        break;                                                          \
    case PIPE_B:                                                        \
        DD_EVENT_VBLANKINTERRUPT_PIPEB(NULL, PipeID, 0, 0, pBDF);       \
        break;                                                          \
    case PIPE_C:                                                        \
        DD_EVENT_VBLANKINTERRUPT_PIPEC(NULL, PipeID, 0, 0, pBDF);       \
        break;                                                          \
    case PIPE_D:                                                        \
        DD_EVENT_VBLANKINTERRUPT_PIPED(NULL, PipeID, 0, 0, pBDF);       \
        break;                                                          \
    default:                                                            \
        DD_EVENT_VBLANKINTERRUPT_PIPEVIRTUAL(NULL, PipeID, 0, 0, pBDF); \
        break;                                                          \
    }

#define DISP_EVENT_FLIP_DONE(PipeID, PlaneID, pBDF)                       \
    switch (PipeID)                                                       \
    {                                                                     \
    case PIPE_A:                                                          \
        DD_EVENT_FLIPDONEINTERRUPT_PIPEA(NULL, PipeID, PlaneID, 0, pBDF); \
        break;                                                            \
    case PIPE_B:                                                          \
        DD_EVENT_FLIPDONEINTERRUPT_PIPEB(NULL, PipeID, PlaneID, 0, pBDF); \
        break;                                                            \
    case PIPE_C:                                                          \
        DD_EVENT_FLIPDONEINTERRUPT_PIPEC(NULL, PipeID, PlaneID, 0, pBDF); \
        break;                                                            \
    case PIPE_D:                                                          \
        DD_EVENT_FLIPDONEINTERRUPT_PIPED(NULL, PipeID, PlaneID, 0, pBDF); \
        break;                                                            \
    default:                                                              \
        break;                                                            \
    }

#define DISP_PERF_START_PROFILING(Session) EventWritePerfProfiler_Start(NULL, Session, 0, __FUNCTION__)
#define DISP_PERF_STOP_PROFILING(Session) EventWritePerfProfiler_Stop(NULL, Session, 0, __FUNCTION__)
#define DISP_PERF_MARK_PROFILING_SUBSTATE(Session, SubState) EventWritePerfProfiler_Start(NULL, Session, SubState, __FUNCTION__)

#define DISP_INSERT_DPC_WI_DATA(DiagCode, FuncPtr, ArgSize, pContext) \
    DISP_INSERT_DIAG_DATA(DiagCode, (((DDU64)FuncPtr) >> 32) & DD_BITRANGE_MASK(31, 0), ((DDU64)FuncPtr) & DD_BITRANGE_MASK(31, 0), ArgSize, 0, pContext)

#define DISP_DP_DIAG_DATA(DiagCode, Param1, Param2, Param3, Param4, BDF) DD_EVENT_DPDIAGNOSTICDATA(NULL, DiagCode, Param1, Param2, Param3, Param4, BDF)
#define DISP_DP_PCON_DIAG_DATA(DiagCode, Param1, Param2, Param3, Param4, BDF) DD_EVENT_PCONDIAGNOSTICDATA(NULL, DiagCode, Param1, Param2, Param3, Param4, BDF)
#define DISP_HDMI_DIAG_DATA(DiagCode, Param1, Param2, Param3, Param4, Param5, Param6, BDF) DD_EVENT_HDMIDIAGNOSTICDATA(NULL, DiagCode, Param1, Param2, Param3, Param4, Param5, Param6, BDF)

#if _DEBUG || _RELEASE_INTERNAL

#else

#endif

typedef enum _DD_DIAG_SOURCE_DDI
{
    DDI_START_DEVICE = 100000,
    DDI_STOP_DEVICE,
    DDI_REMOVE_DEVICE,
    DDI_DISPATCH_IO_REQUEST,
    DDI_INTERRUPT_ROUTINE,
    DDI_DPC_ROUTINE,
    DDI_QUERY_CHILD_RELATIONS,
    DDI_QUERY_CHILD_STATUS,
    DDI_QUERY_DEVICE_DESCRIPTOR,
    DDI_RESERVED,
    DDI_NOTIFY_ACPI_EVENT,
    DDI_RESET_DEVICE,
    DDI_UNLOAD,
    DDI_QUERY_INTERFACE,
    DDI_CONTROL_ETW_LOGGING,
    DDI_QUERYADAPTERINFO,
    DDI_CREATEDEVICE,
    DDI_CREATEALLOCATION,
    DDI_DESTROYALLOCATION,
    DDI_DESCRIBEALLOCATION,
    DDI_GETSTANDARDALLOCATIONDRIVERDATA,
    DDI_ACQUIRESWIZZLINGRANGE,
    DDI_RELEASESWIZZLINGRANGE,
    DDI_PATCH,
    DDI_SUBMITCOMMAND,
    DDI_PREEMPTCOMMAND,
    DDI_BUILDPAGINGBUFFER,
    DDI_SETPALETTE,
    DDI_SETPOINTERPOSITION,
    DDI_SETPOINTERSHAPE,
    DDI_RESETFROMTIMEOUT,
    DDI_RESTARTFROMTIMEOUT,
    DDI_ESCAPE,
    DDI_COLLECTDBGINFO,
    DDI_QUERYCURRENTFENCE,
    DDI_ISSUPPORTEDVIDPN,
    DDI_RECOMMENDFUNCTIONALVIDPN,
    DDI_ENUMVIDPNCOFUNCMODALITY,
    DDI_SETVIDPNSOURCEADDRESS,
    DDI_SETVIDPNSOURCEVISIBILITY,
    DDI_COMMITVIDPN,
    DDI_UPDATEACTIVEVIDPNPRESENTPATH,
    DDI_RECOMMENDMONITORMODES,
    DDI_RECOMMENDVIDPNTOPOLOGY,
    DDI_GETSCANLINE,
    DDI_STOPCAPTURE,
    DDI_CONTROLINTERRUPT,
    DDI_CREATEOVERLAY,
    DDI_DESTROYDEVICE,
    DDI_OPENALLOCATIONINFO,
    DDI_CLOSEALLOCATION,
    DDI_RENDER,
    DDI_PRESENT,
    DDI_UPDATEOVERLAY,
    DDI_FLIPOVERLAY,
    DDI_DESTROYOVERLAY,
    DDI_CREATECONTEXT,
    DDI_DESTROYCONTEXT,
    DDI_LINK_DEVICE,
    DDI_SETDISPLAYPRIVATEDRIVERFORMAT,
    DDI_RENDERKM,
    DDI_QUERYVIDPNHWCAPABILITY,
    DDISETPOWERCOMPONENTFSTATE,
    DDI_QUERYDEPENDENTENGINEGROUP,
    DDI_QUERYENGINESTATUS,
    DDI_RESETENGINE,
    DDI_STOP_DEVICE_AND_RELEASE_POST_DISPLAY_OWNERSHIP,
    DDI_SYSTEM_DISPLAY_ENABLE,
    DDI_SYSTEM_DISPLAY_WRITE,
    DDI_CANCELCOMMAND,
    DDI_GET_CHILD_CONTAINER_ID,
    DDIPOWERRUNTIMECONTROLREQUEST,
    DDI_SETVIDPNSOURCEADDRESSWITHMULTIPLANEOVERLAY,
    DDI_NOTIFY_SURPRISE_REMOVAL,
    DDI_GETNODEMETADATA,
    DDI_SET_POWER_STATE,
    DDI_CONTROLINTERRUPT2,
    DDI_CHECKMULTIPLANEOVERLAYSUPPORT,
    DDI_CALIBRATEGPUCLOCK,
    DDI_FORMATHISTORYBUFFER,
    DDI_RENDERGDI,
    DDI_SUBMITCOMMANDVIRTUAL,
    DDI_SETROOTPAGETABLE,
    DDI_GETROOTPAGETABLESIZE,
    DDI_MAPCPUHOSTAPERTURE,
    DDI_UNMAPCPUHOSTAPERTURE,
    DDI_CHECKMULTIPLANEOVERLAYSUPPORT2,
    DDI_CREATEPROCESS,
    DDI_DESTROYPROCESS,
    DDI_SETVIDPNSOURCEADDRESSWITHMULTIPLANEOVERLAY2,
    DDI_POWERRUNTIMESETDEVICEHANDLE,
    DDI_SETSTABLEPOWERSTATE,
    DDI_SETVIDEOPROTECTEDREGION,
    DDI_CHECKMULTIPLANEOVERLAYSUPPORT3,
    DDI_SETVIDPNSOURCEADDRESSWITHMULTIPLANEOVERLAY3,
    DDI_POSTMULTIPLANEOVERLAYPRESENT,
    DDI_VALIDATEUPDATEALLOCATIONPROPERTY,
    DDI_CONTROLMODEBEHAVIOR,
    DDI_UPDATEMONITORLINKINFO,
    DDI_CREATEHWCONTEXT,
    DDI_DESTROYHWCONTEXT,
    DDI_CREATEHWQUEUE,
    DDI_DESTROYHWQUEUE,
    DDI_SUBMITCOMMANDTOHWQUEUE,
    DDI_SWITCHTOHWCONTEXTLIST,
    DDI_RESETHWENGINE,
    DDI_CREATEPERIODICFRAMENOTIFICATION,
    DDI_DESTROYPERIODICFRAMENOTIFICATION,
    DDI_SETTIMINGSFROMVIDPN,
    DDI_SETTARGETGAMMA,
    DDI_SETTARGETCONTENTTYPE,
    DDI_SETTARGETANALOGCOPYPROTECTION,
    DDI_SETTARGETADJUSTEDCOLORIMETRY,
    DDI_DISPLAYDETECTCONTROL,
    DDI_QUERYCONNECTIONCHANGE,
    DDI_EXCHANGEPRESTARTINFO,
    DDI_GETMULTIPLANEOVERLAYCAPS,
    DDI_GETPOSTCOMPOSITIONCAPS,
    DDI_OPM_GET_CERTIFICATESIZE,
    DDI_OPM_GET_CERTIFICATE,
    DDI_OPM_CREATEPROTECTEDOUTPUT,
    DDI_OPM_CREATEPROTECTEDOUTPUTFORNONLOCALDISPLAY,
    DDI_OPM_GET_RANDOMNUMBER,
    DDI_OPM_SET_SIGNINGKEYANDSEQUENCENUMBERS,
    DDI_OPM_GET_INFO,
    DDI_OPM_GET_COPPCOMPATIBLEINFO,
    DDI_OPM_CONFIGURE_PROTECTEDOUTPUT,
    DDI_OPM_DESTROY_PROTECTEDOUTPUT,
    DDI_I2C_TRANSMITDATATODISPLAY,
    DDI_I2C_RECEIVEDATAFROMDISPLAY,
    DDI_BLC_GETPOSSIBLEBRIGHTNESS,
    DDI_BLC_SET_BRIGHTNESS,
    DDI_BLC_GET_BRIGHTNESS,
    DDI_BLC_GET_BRIGHTNESS_CAPS,
    DDI_BLC_SET_BRIGHTNESS_STATE,
    DDI_BLC_GET_BACKLIGHT_REDUCTION,
    DDI_BLC_SET_BACKLIGHT_OPTIMIZATION,
    DDI_BLC_GET_NITRANGES,
    DDI_BLC_GET_BRIGHTNESS_CAPS3,
    DDI_BLC_GET_BRIGHTNESS3,
    DDI_BLC_SET_BRIGHTNESS3,
    DDI_BLC_SET_BACKLIGHT_OPTIMIZATION3,
    DDI_PRESENT_DISPLAY_ONLY,
    DDI_QUERY_DP_CAPS,
    DDI_GET_DP_ADDRESS,
    DDI_DP_AUX_IO_TRANSMISSION,
    DDI_DP_I2C_IO_TRANSMISSION,
    DDI_DP_SBM_TRANSMISSION,
    DDI_DISP_DIAG_NON_INTRUSIVE,
    DDI_DISP_DIAG_INTRUSIVE,
    DDI_CONTROLINTERRUPT3,
    DDI_QUERY_DSI_CAPS,
    DDI_DSI_TRANSMISSION,
    DDI_DSI_RESET,
    DDI_SETINTERRUPTTARGETPRESENTID,
    DDI_CANCELFLIPS,
    DDI_SETFLIPQUEUELOGBUFFER,
    DDI_UPDATEFLIPQUEUELOG,
    DDI_COLLECTDIAGNOSTICINFO,
    DDI_DISPLAYMUX_REPORT_PRESENCE,
    DDI_DISPLAYMUX_UPDATE_STATE,
    DDI_DISPLAYMUX_POST_SWITCH_TO_PHASE1,
    DDI_DISPLAYMUX_POST_SWITCH_TO_PHASE2,
    DDI_DISPLAYMUX_POST_SWITCH_AWAY,
    DDI_DISPLAYMUX_SWITCH_CANCELED,
    DDI_DISPLAYMUX_PRE_SWITCH_TO,
    DDI_DISPLAYMUX_PRE_SWITCH_AWAY_GET_PRIVATE_DATA,
    DDI_DISPLAYMUX_PRE_SWITCH_AWAY,
    DDI_DISPLAYMUX_GET_RUNTIME_STATUS,
    DDI_DISPLAYMUX_GET_DRIVER_SUPPORT_LEVEL,
    // Keep this as the last entry. Always add entries above this line and update the C_ASSERT
    // The order needs to be maintained for the backward compatibility of the DiAna Analysis framawork
    DDI_MAX_SUPPORTRD_ENTRY,
} DD_DIAG_SOURCE_DDI;
// Assert to make sure the order is maintained. By verifying few points
C_ASSERT((DDI_SETTIMINGSFROMVIDPN) == (DDI_START_DEVICE + 108)); // Got an Assert here? Make sure you add the new enums at the end
C_ASSERT((DDI_DP_SBM_TRANSMISSION) == (DDI_START_DEVICE + 147));
C_ASSERT((DDI_COLLECTDIAGNOSTICINFO) == (DDI_START_DEVICE + 158));

#define DD_BB_SECTION_NAME 32
#define SECTSIZE sizeof(pDisplayBlackBox->Header.SectionName)
#define GET_VERSION(Structure) #Structure

#define DD_UPDATE_BB_HEADER(DataSegment, Ver, CircularBufferSize, DataBeingLogged)                      \
    (pDisplayBlackBox->##DataSegment).Header.RawValue        = 0;                                       \
    (pDisplayBlackBox->##DataSegment).Header.Version         = Ver;                                     \
    (pDisplayBlackBox->##DataSegment).Header.DataSize        = sizeof(pDisplayBlackBox->##DataSegment); \
    (pDisplayBlackBox->##DataSegment).Header.TailIndex.Index = DDMAXU32;                                \
    (pDisplayBlackBox->##DataSegment).Header.TailIndex.Mask  = (CircularBufferSize - 1);                \
    memcpy_s(pDisplayBlackBox->##DataSegment.Header.SectionName, SECTSIZE, #DataBeingLogged, strlen(#DataBeingLogged)); // Appending the Section name here.

#define DD_GET_NEXT_INDEX(DataSegment) (LOG_GetNextIndex(&DataSegment.Header.TailIndex.Index) & DataSegment.Header.TailIndex.Mask)
#define DD_GET_CUR_INDEX(DataSegment) ((DataSegment.Header.TailIndex.Index == DDMAXU32) ? 0 : DataSegment.Header.TailIndex.Index & DataSegment.Header.TailIndex.Mask)

typedef struct _DD_BB_TAIL_INDEX
{
    // Circular buffers will have the size as power of 2
    // This enforced to make the Roll over easy
    DDU32 Index; // The pointer to the current valid data
    // Since the size is power of 2 the Roll over should happen when the number crosees Size -1
    // We will compute the next Index as (Index ++ ) & Mask
    DDU32 Mask;
} DD_BB_TAIL_INDEX;

typedef struct _DD_BB_SECTION_HEADER
{
    union
    {
        struct
        {
            DDU32 Version : 8;
            DDU32 Reserved : 24;
        };
        DDU32 RawValue;
    };
    DDU32 DataSize;
    DD_BB_TAIL_INDEX TailIndex;
    DDU8 SectionName[DD_BB_SECTION_NAME];
} DD_BB_SECTION_HEADER;
// C_ASSERT((sizeof(SectionName)) > DD_BB_SECTION_NAME);
C_ASSERT(((sizeof(DDU32) * 4) + DD_BB_SECTION_NAME) == sizeof(DD_BB_SECTION_HEADER));

// A multiplier which can be used to configure the size of the data.
// All the array sizes will proportionally expanded
#define DD_BB_SIZE_MULTIPLIER __BIT(0)

typedef struct _DD_BB_TARGETMODE
{
    DDU32 TargetID;
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
    DD_SIGNAL_STANDARD SignalStandard; // Signal standard
    DDU32 S3DFormatMask;               // Mask of DD_S3D_FORMAT
    DD_TAREGT_MODE_ID ModeId;          // Keep it at bottom as upper struct maps to pre defined timings values.
} DD_BB_TARGETMODE;

#define DD_BB_MODELIST_DATA_VERSION_0X01 1
#define DD_BB_MODELIST_DATA_VERSION DD_BB_MODELIST_DATA_VERSION_0X01
typedef struct _DD_BB_MODELIST_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DDU32 NumEntries;
    DD_BB_TARGETMODE TargetModeList[1];
} DD_BB_MODELIST_CONTAINER;

typedef struct _DD_BB_EDID_DATA
{
    DD_PORT_TYPE Port;
    DDU8 NumEdidExtensionBlocks; // Max 255 Extension Blocks possible
    DDU32 BlockNo;               // EDID Block No
    DDU32 EdidType;              // Edid type like fake, vbt, etc.
    DDU32 SinkType;              // Sink type in DD_VIDEO_OUTPUT_TECHNOLOGY
    DDU32 SinkIndex;             // Starts with index 0 for each display type
    DDU32 ProtocolType;          // Protocol Type in DD_PROTOCOL_TYPE
    DDU8 SerialNumChecksum;
    DD_BOOL IsEdidBlockModified;
    DDU8 EDIDBlock[128];
} DD_BB_EDID_DATA;

#define DD_BB_EDID_DATA_VERSION_0X01 1
#define DD_BB_EDID_DATA_VERSION DD_BB_EDID_DATA_VERSION_0X01
typedef struct _DD_BB_EDID_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DDU32 NumEntries;
    DD_BB_EDID_DATA EdidList[1];
} DD_BB_EDID_CONTAINER;

typedef struct _DD_BB_DP_MST_DATA
{
    DDU8 Port;
    DDS64 TotalTimeElapsed;
    DDU32 TimeoutRequestID;
    DDU8 Branch_OUI[3];
    DDU8 SidebandMsgDebugFlags;
    DDU8 NumDownStreamDeviceRemoval;
    DDU8 NumDownStreamDeviceArrival;
} DD_BB_DP_MST_DATA;

#define DD_BB_DP_MST_DATA_VERSION_0X01 1
#define DD_BB_DP_MST_DATA_VERSION DD_BB_DP_MST_DATA_VERSION_0X01
typedef struct _DD_BB_DP_MST_DATA_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DDU32 NumEntries;
    DD_BB_DP_MST_DATA DpMstData[1];
} DD_BB_DP_MST_DATA_CONTAINER;

typedef struct _DD_BB_LPI_HPD_DATA_V1
{
    DDU64 TimeStamp;
    DDU8 Port;
    DDU8 HotplugEventsFlag;
} DD_BB_LPI_HPD_DATA_V1;
#define DD_BB_LPI_HPD_DATA_VERSION 1
#define DD_BB_LPI_HPD_BUFFER_SIZE (__BIT(6) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_LPI_HPD_DATA_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_LPI_HPD_DATA_V1 LPI_HpdData[DD_BB_LPI_HPD_BUFFER_SIZE];
} DD_BB_LPI_HPD_DATA_CONTAINER;

typedef struct _DD_BB_LPI_LIVESTATE_DATA_V1
{
    DDU64 TimeStamp;
    DDU8 Port;
    DD_BOOL IsLiveStateHigh;
    DDU8 PortConnectorType;
} DD_BB_LPI_LIVESTATE_DATA_V1;
#define DD_BB_LPI_LIVESTATE_DATA_VERSION 1
#define DD_BB_LPI_LIVESTATE_BUFFER_SIZE (__BIT(6) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_LPI_LIVESTATE_DATA_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_LPI_LIVESTATE_DATA_V1 LPI_LivestateData[DD_BB_LPI_LIVESTATE_BUFFER_SIZE];
} DD_BB_LPI_LIVESTATE_DATA_CONTAINER;

typedef struct _DD_BB_LPI_TYPE_C_DATA_V1
{
    DDU64 TimeStamp;
    DDU8 Port;
    DD_BOOL HpdLiveState;
    DD_BOOL DPPMS;
    DD_BOOL DPCSS;
    DDU8 PortConnectorType;
} DD_BB_LPI_TYPE_C_DATA_V1;
#define DD_BB_LPI_TYPE_C_DATA_VERSION 1
#define DD_BB_LPI_TYPE_C_BUFFER_SIZE (__BIT(6) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_LPI_TYPE_C_DATA_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_LPI_TYPE_C_DATA_V1 LPI_TypeCData[DD_BB_LPI_TYPE_C_BUFFER_SIZE];
} DD_BB_LPI_TYPE_C_DATA_CONTAINER;

typedef struct _DD_BB_SPI_DATA_V1
{
    DDU64 TimeStamp;
    DDU8 Port;
    DDU8 Reason;
    DDU16 SpiEventsFlag;
} DD_BB_SPI_DATA_V1;

#define DD_BB_SPI_DATA_VERSION 1
#define DD_BB_SPI_BUFFER_SIZE (__BIT(9) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_SPI_DATA_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_SPI_DATA_V1 SPI_Data[DD_BB_SPI_BUFFER_SIZE];

} DD_BB_SPI_DATA_CONTAINER;

typedef struct _DD_BB_FUNC_TRACK_DATA
{
    DDU64 TimeStamp;
    DDSTATUS Status;
    DDU32 Offset;
} DD_BB_FUNC_TRACK_DATA;

#define DD_BB_FUNC_TRACK_DATA_VERSION 1
#define DD_BB_FUNC_TRACK_BUFFER_SIZE (__BIT(7) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_FUNC_TRACK_DATA_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_FUNC_TRACK_DATA FuncTrackData[DD_BB_FUNC_TRACK_BUFFER_SIZE];

} DD_BB_FUNC_TRACK_DATA_CONTAINER;

typedef struct _DD_BB_ASSERT_DATA
{
    DDU64 TimeStamp;
    DD_ASSERT_TYPE Type;
    DDU64 LhsValue;
    DDU64 RhsValue;
    DDU32 Offset;
} DD_BB_ASSERT_DATA;

#define DD_BB_ASSERT_DATA_VERSION 1
// Smaller size since we don't expect too many assertions to get hit in the field
#define DD_BB_ASSERT_BUFFER_SIZE (__BIT(4) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_ASSERT_DATA_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_ASSERT_DATA FailedAssertData[DD_BB_ASSERT_BUFFER_SIZE];
} DD_BB_ASSERT_DATA_CONTAINER;

typedef union _DD_BB_RR_FEATURE_SUPPORT
{
    struct
    {
        DDU8 IsVrrSupported : 1;
        DDU8 IsVrrSdpSupported : 1;
        DDU8 IsAlwaysInVrrModeSupported : 1;
        DDU8 IsGamingVrrEnabled : 1;
        DDU8 IsFixedRrSwitchingSupported : 1;
        DDU8 IsVbiMaskingSupported : 1;
        DDU8 IsVirtualRrSupported : 1;
        DDU8 IsCmrrSupported : 1;
    };
    DDU8 Value;
} DD_BB_RR_FEATURE_SUPPORT;

typedef struct _DD_BB_RR_SWITCHING_CAPS
{
    DDU64 TimeStamp;
    DDU32 TargetId;
    DD_BB_RR_FEATURE_SUPPORT RrFeatureSupport;
    DDU8 RrSwitchMethod;
    DDU8 DrrsPanelType;
    DDU32 MinFixedRr1000;
    DDU32 MaxFixedRr1000;
    DDU32 MinVariableRr1000;
    DDU32 MaxVariableRr1000;
} DD_BB_RR_SWITCHING_CAPS;

#define DD_RR_SWITCHING_CAPS_VERSION 1
#define DD_RR_SWITCHING_CAPS_BUFFER_SIZE (__BIT(4) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_RR_SWITCHING_CAPS_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_RR_SWITCHING_CAPS RrSwitchingCaps[DD_RR_SWITCHING_CAPS_BUFFER_SIZE];
} DD_BB_RR_SWITCHING_CAPS_CONTAINER;

typedef struct _DD_BB_RR_SWITCHING_STATE
{
    DDU64 TimeStamp;
    DDU64 LastTimeStamp;
    DDU32 Counter; // Number of events occurred between TimeStamp and LastTimeStamp with same values.
    DDU32 TargetId;
    DD_BOOL IsActive;
    DDU8 RefreshRateMode;
    DDU8 RrSwitchMethod;
    DDU8 VbiMaskingFactor;
    DDU32 FixedRr1000;
    DDU32 MinVariableRr1000;
    DDU32 MaxVariableRr1000;
} DD_BB_RR_SWITCHING_STATE;

#define DD_RR_SWITCHING_STATES_VERSION 2
#define DD_RR_SWITCHING_STATES_BUFFER_SIZE (__BIT(6) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_RR_SWITCHING_STATES_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_RR_SWITCHING_STATE RrStates[DD_RR_SWITCHING_STATES_BUFFER_SIZE];
} DD_BB_RR_SWITCHING_STATES_CONTAINER;

typedef enum _DD_DISPLAY_SCANOUT_STATE
{
    DD_DISPLAY_SCANOUT_STATE_UNINITIALIZED = 0,
    DD_DISPLAY_SCANOUT_DISABLED,
    DD_DISPLAY_SCANOUT_ACTIVE,
    DD_DISPLAY_SCANOUT_ACTIVE_BLACK,
    DD_DISPLAY_SCANOUT_STATE_MAX
} DD_DISPLAY_SCANOUT_STATE;

typedef struct _DD_BB_SCANOUT_STATE_INFO
{
    DDU64 TimeStamp;
    PIPE_ID Pipe;
    DD_DISPLAY_SCANOUT_STATE State;
} DD_BB_SCANOUT_STATE_INFO;

#define DD_BB_SCANOUT_STATE_VERSION 1
#define DD_BB_SCANOUT_STATE_SIZE (__BIT(4) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_SCANOUT_STATE
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_SCANOUT_STATE_INFO ScanOutStateHistory[DD_BB_SCANOUT_STATE_SIZE];
} DD_BB_SCANOUT_STATE;

typedef struct _DD_BB_DDI_TRACK_DATA_V1
{
    DDU64 TimeStamp;
    struct
    {
        DDU32 Ddi : 10;
        DDU32 Reserved : 16;
        DDU32 IsExit : 1;
        DDU32 Irql : 5;
    };
    DDU32 NtStatus;
    DDU16 RemainingStackSize;
} DD_BB_DDI_TRACK_DATA_V1;

#define DD_BB_DDI_TRACK_DATA_VERSION 1
#define DD_BB_DDI_TRACK_BUFFER_SIZE (__BIT(8) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_DDI_TRACK_DATA_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_DDI_TRACK_DATA_V1 DDITrackData[DD_BB_DDI_TRACK_BUFFER_SIZE];

} DD_BB_DDI_TRACK_DATA_CONTAINER;

typedef struct _DD_BB_SET_TIMING_DATA_V1
{
    DDU64 TimeStamp;
    DD_BOOL IsEnabled;
    DD_BOOL IsFMS;
    DDU8 SinkIndex;
    DD_PORT_TYPE Port;
    DD_VIDEO_OUTPUT_TECHNOLOGY SinkType;
    PIPE_ID Pipe;
    DDU32 SrcX;
    DDU32 SrcY;
    DDU32 HActive;
    DDU32 VActive;
    DDU32 HTotal;
    DDU32 VTotal;
    DDU32 RoundedRR;
    DDU64 DotClock;
    DD_SCALING Scaling;
    DD_PIXELFORMAT PixelFmt;

} DD_BB_SET_TIMING_DATA_V1;

#define DD_BB_SET_TIMING_DATA_VERSION 1
#define DD_BB_SET_TIMING_BUFFER_SIZE (__BIT(6) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_SET_TIMING_DATA_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_SET_TIMING_DATA_V1 SettimingData[DD_BB_SET_TIMING_BUFFER_SIZE];

} DD_BB_SET_TIMING_DATA_CONTAINER;

typedef struct _DD_BB_PIPE_STATE_DATA_V1
{
    DD_DISPLAY_SCANOUT_STATE ScanoutState;
    DD_BOOL VisibilityOn;
} DD_BB_PIPE_STATE_DATA_V1;

#define DD_BB_PIPE_STATE_DATA_VERSION 1
#define DD_BB_PIPE_STATE_BUFFER_SIZE (__BIT(2) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_PIPE_STATE_DATA_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_PIPE_STATE_DATA_V1 PipeState[MAX_PHYSICAL_PIPES];
} DD_BB_PIPE_STATE_DATA_CONTAINER;
C_ASSERT(DD_BB_PIPE_STATE_BUFFER_SIZE >= MAX_PHYSICAL_PIPES); // keep a check to ensure PIPE count does not change without this getting updated.

#define DD_BB_UNDERRUN_DATA_VERSION 1
#define DD_BB_UNDERRUN_DBG_DATA_SIZE 1024
typedef struct _DD_BB_UNDERRUN_DBG_DATA_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DDU64 TimeStamp;
    DDU32 WmLatency[2];
    DDU32 NumRegEntries;
    DD_OFFSET_DATA_PAIR RegisterState[DD_BB_UNDERRUN_DBG_DATA_SIZE];
} DD_BB_UNDERRUN_DBG_DATA_CONTAINER;

#define DD_BB_DYNAMIC_PARAM_SIZE 4
typedef struct _DD_BB_SCENARIO_DATA_V1
{
    DDU64 TimeStamp;
    DDU32 State;
    DDU32 Params[DD_BB_DYNAMIC_PARAM_SIZE];
} DD_BB_SCENARIO_DATA_V1;
#define DD_BB_SCENARIO_DATA_VERSION 1
#define DD_BB_SCENARIO_BUFFER_SIZE (__BIT(5) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_SCENARIO_DATA_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_SCENARIO_DATA_V1 SequenceData[DD_BB_SCENARIO_BUFFER_SIZE];

} DD_BB_SCENARIO_DATA_CONTAINER;

typedef struct _DD_DIAGNOSTIC_DATA_V1
{
    DDU64 TimeStamp;
    DD_DIAGNOSTIC_SOURCE Source;
    DDU32 Param1;
    DDU32 Param2;
    DDU32 Param3;
    DDU32 Param4;
} DD_DIAGNOSTIC_DATA_V1;

#define DD_BB_DIAGNOSTIC_DATA_VERSION 1
#define DD_BB_DIAGNOSTIC_BUFFER_SIZE (__BIT(7) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_DIAGNOSTIC_ERROR_DATA
{
    DD_BB_SECTION_HEADER Header;
    DD_DIAGNOSTIC_DATA_V1 Diag_Error[DD_BB_DIAGNOSTIC_BUFFER_SIZE];
} DD_BB_DIAGNOSTIC_ERROR_DATA;

#define DD_BB_DIAGNOSTIC_WARNING_BUFFER_SIZE (__BIT(4) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_DIAGNOSTIC_WARNING_DATA
{
    DD_BB_SECTION_HEADER Header;
    DD_DIAGNOSTIC_DATA_V1 Diag_Warning[DD_BB_DIAGNOSTIC_WARNING_BUFFER_SIZE];
} DD_BB_DIAGNOSTIC_WARNING_DATA;

#define DD_BB_DIAGNOSTIC_INFO_BUFFER_SIZE (__BIT(4) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_DIAGNOSTIC_INFO_DATA
{
    DD_BB_SECTION_HEADER Header;
    DD_DIAGNOSTIC_DATA_V1 Diag_Info[DD_BB_DIAGNOSTIC_INFO_BUFFER_SIZE];
} DD_BB_DIAGNOSTIC_INFO_DATA;

typedef enum _DD_BB_TRACKING_SCENARIO_V1
{
    DD_BB_TRACKING_SCENARIO_BOOT = 0,
    DD_BB_TRACKING_SCENARIO_RESUME,
    DD_BB_TRACKING_SCENARIO_DISPLAY_DETECTION, // ?
    DD_BB_TRACKING_SCENARIO_SET_TIMING,

    DD_BB_TRACKING_SCENARIO_MAX // Keep at the end
} DD_BB_TRACKING_SCENARIO_V1;

typedef struct _DD_BB_SCENARIO_COLLECTION_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_SCENARIO_DATA_CONTAINER ScenarioData[DD_BB_TRACKING_SCENARIO_MAX];
} DD_BB_SCENARIO_COLLECTION_CONTAINER;

typedef struct _DD_BB_VBI_CONTROL_DATA_V1
{
    DDU64 TimeStamp;
    DD_CRTC_VSYNC_STATE State;
    DDU32 AffectedSources;
} DD_BB_VBI_CONTROL_DATA_V1;

#define DD_BB_VBI_CONTROL_DATA_VERSION 1
#define DD_BB_VBI_CONTROL_BUFFER_SIZE (__BIT(5) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_VBI_CONTROL_DATA_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_VBI_CONTROL_DATA_V1 VBIControlData[DD_BB_VBI_CONTROL_BUFFER_SIZE];

} DD_BB_VBI_CONTROL_DATA_CONTAINER;

typedef enum _DD_DISPLAY_CONNECTIVITY
{
    DD_DISPLAY_CONNECTIVITY_UNINITIALIZED = 0,
    DD_DISPLAY_NOT_CONNECTED              = 1,
    DD_DISPLAY_CONNECTED                  = 2,
} DD_DISPLAY_CONNECTIVITY;

typedef struct _DD_DIAG_DISPLAY_CONNECTIVITY_FOR_TARGETID_V1
{
    DDU64 TimeStamp;
    DDU32 TargetID;
    DD_DISPLAY_CONNECTIVITY DisplayConnectivityData;
} DD_DIAG_DISPLAY_CONNECTIVITY_FOR_TARGETID_V1;

#define DD_DIAG_DISPLAY_CONNECTIVITY_VERSION 1
#define DD_DIAG_DISPLAY_CONNECTIVITY_BUFFER_SIZE (__BIT(4) * DD_BB_SIZE_MULTIPLIER)

typedef struct _DD_DISPLAY_CONNECTIVITY_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_DIAG_DISPLAY_CONNECTIVITY_FOR_TARGETID_V1 DispDiagConnectivity[DD_DIAG_DISPLAY_CONNECTIVITY_BUFFER_SIZE];
} DD_DISPLAY_CONNECTIVITY_CONTAINER;

typedef enum _DD_BASIC_DISPLAY_TOPOLOGY
{
    DD_BASIC_DISPLAY_TOPOLOGY_UNINITIALIZED   = 0,
    DD_DISPLAY_CONNECTED_DIRECTLY             = 1,
    DD_DISPLAY_CONNECTED_INDIRECTLY_CONVERTOR = 2,
    DD_DISPLAY_CONNECTED_INDIRECTLY_HUB       = 3,
    DD_DISPLAY_CONNECTED_INDIRECTLY           = 4,
    DD_DISPLAY_CONNECTED_UNKNOWN              = 5,
} DD_BASIC_DISPLAY_TOPOLOGY;

typedef enum _DD_DISPLAY_MODE_SET
{
    DD_DISPLAY_MODE_SET_UNINITIALIZED = 0,
    DD_DISPLAY_MODE_SET_NO            = 1,
    DD_DISPLAY_MODE_SET_YES           = 2,
} DD_DISPLAY_MODE_SET;

#define MAX_NUM_OF_GAMMA_SAMPLES_FOR_DIAGNOSTICS 16
#define ARR_SIZE 3
typedef struct _DD_DIAG_DISPLAY_SAMPLED_GAMMA
{
    DDU32 Red[MAX_NUM_OF_GAMMA_SAMPLES_FOR_DIAGNOSTICS];
    DDU32 Green[MAX_NUM_OF_GAMMA_SAMPLES_FOR_DIAGNOSTICS];
    DDU32 Blue[MAX_NUM_OF_GAMMA_SAMPLES_FOR_DIAGNOSTICS];
} DD_DIAG_DISPLAY_SAMPLED_GAMMA;

typedef struct _DD_DIAG_DISPLAY_CSCDATA
{
    float ColorMatrix[ARR_SIZE][ARR_SIZE];
} DD_DIAG_DISPLAY_CSCDATA;

// CSC data container for Yangra
typedef struct _DD_DIAG_DISPLAY_CSC_DATA_INTEGER
{
    DDU32 ColorMatrix[ARR_SIZE][ARR_SIZE];
} DD_DIAG_DISPLAY_CSC_DATA_INTEGER;

// Gamma Data Containers for Yangra
// For Yangra as the final processing happens in the HAL
// and the HAL should not know the TargetId wwe would need to
// have the data stored in a pipe addressed container.
typedef struct _DD_DIAG_DISPLAY_SAMPLED_GAMMA_FOR_PIPE_V1
{
    DDU64 TimeStamp;
    DDU8 PipeId;
    DD_DIAG_DISPLAY_SAMPLED_GAMMA PipeGammaContainer;
} DD_DIAG_DISPLAY_SAMPLED_GAMMA_FOR_PIPE_V1;

#define DD_DIAG_DISPLAY_SAMPLED_GAMMA_FOR_PIPE_VERSION 1
#define DD_DIAG_DISPLAY_SAMPLED_GAMMA_BUFFER_SIZE_FOR_PIPE (__BIT(4) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_DIAG_DISPLAY_SAMPLED_GAMMA_PER_PIPE_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_DIAG_DISPLAY_SAMPLED_GAMMA_FOR_PIPE_V1 DispDiagGammaDataPerPipe[DD_DIAG_DISPLAY_SAMPLED_GAMMA_BUFFER_SIZE_FOR_PIPE];
} DD_DIAG_DISPLAY_SAMPLED_GAMMA_PER_PIPE_CONTAINER;

typedef struct _DD_DIAG_DISPLAY_SAMPLED_GAMMA_ALL_PIPE_CONTAINER
{
    DD_DIAG_DISPLAY_SAMPLED_GAMMA_PER_PIPE_CONTAINER DispDiagGammaDataAllPipe[MAX_POSSIBLE_PIPES];
} DD_DIAG_DISPLAY_SAMPLED_GAMMA_ALL_PIPE_CONTAINER;

// csc container per pipe
typedef struct _DD_DIAG_DISPLAY_CSC_FOR_PIPE_V1
{
    DDU64 TimeStamp;
    DDU8 PipeId;
    DD_DIAG_DISPLAY_CSC_DATA_INTEGER PipeCSCContainer;
} DD_DIAG_DISPLAY_CSC_FOR_PIPE_V1;

#define DD_DIAG_DISPLAY_CSC_FOR_PIPE_VERSION 1
#define DD_DIAG_DISPLAY_CSC_BUFFER_SIZE_FOR_PIPE (__BIT(4) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_DIAG_DISPLAY_CSC_PER_PIPE_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_DIAG_DISPLAY_CSC_FOR_PIPE_V1 DispDiagCscDataPerPipe[DD_DIAG_DISPLAY_SAMPLED_GAMMA_BUFFER_SIZE_FOR_PIPE];
} DD_DIAG_DISPLAY_CSC_PER_PIPE_CONTAINER;

typedef struct _DD_DIAG_DISPLAY_CSC_ALL_PIPE_CONTAINER
{
    DD_DIAG_DISPLAY_CSC_PER_PIPE_CONTAINER DispDiagCSCDataAllPipe[MAX_POSSIBLE_PIPES];
} DD_DIAG_DISPLAY_CSC_ALL_PIPE_CONTAINER;

// gamma data containers for Yangra end

typedef struct _DD_DIAG_DISPLAY_SAMPLED_GAMMA_FOR_TARGETID_V1
{
    DDU64 TimeStamp;
    DDU32 TargetID;
    DD_DIAG_DISPLAY_SAMPLED_GAMMA GammaVal;
    DD_DIAG_DISPLAY_CSCDATA CSCData;
} DD_DIAG_DISPLAY_SAMPLED_GAMMA_FOR_TARGETID_V1;

#define DD_DIAG_DISPLAY_SAMPLED_GAMMA_VERSION 1
#define DD_DIAG_DISPLAY_SAMPLED_GAMMA_BUFFER_SIZE (__BIT(4) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_DIAG_DISPLAY_SAMPLED_GAMMA_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_DIAG_DISPLAY_SAMPLED_GAMMA_FOR_TARGETID_V1 DispDiagGammaData[DD_DIAG_DISPLAY_SAMPLED_GAMMA_BUFFER_SIZE];
} DD_DIAG_DISPLAY_SAMPLED_GAMMA_CONTAINER;

#define DD_DISPLAY_DATA_FOR_TARGETID_VERSION 1
typedef struct _DD_DISPLAY_DATA_FOR_TARGETID_V1
{
    DDU32 TargetID;
    DD_DISPLAY_CONNECTIVITY DisplayConnectivity;
    DD_BASIC_DISPLAY_TOPOLOGY Topology;
    DD_DISPLAY_MODE_SET ModeSet;
    DD_DIAG_DISPLAY_SAMPLED_GAMMA CurrentGammaVal;
    DD_DIAG_DISPLAY_CSCDATA CSCData;
    DD_BOOL Link_Bandwidth_Limited;
    DD_BOOL ModesetFailed;
} DD_DISPLAY_DATA_FOR_TARGETID_V1;

#define DD_DISPLAY_DATA_FOR_TARGETID_BUFFER_SIZE (__BIT(6) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_DISPLAY_DATA_FOR_TARGETID_VERSION_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_DISPLAY_DATA_FOR_TARGETID_V1 DispDiagData_TargetID[DD_DISPLAY_DATA_FOR_TARGETID_BUFFER_SIZE];
} DD_DISPLAY_DATA_FOR_TARGETID_VERSION_CONTAINER;

// Circular buffer data
typedef enum _DD_DISPLAY_LINK_STATE
{
    DD_DISPLAY_LINK_STATE_UNINITIALIZED              = 0,
    DD_DISPLAY_LINK_STATE_NOTAPPLICABLE              = 1,
    DD_DISPLAY_LINK_STATE_STABLE                     = 2,
    DD_DISPLAY_LINK_STATE_FAILED                     = 3,
    DD_DISPLAY_LINK_STATE_CONTINUOUS_TRAINING        = 4,
    DD_DISPLAY_LINK_STATE_CONTINUOUS_TRAINING_STABLE = 5,
    DD_DISPLAY_LINK_STATE_CONTINUOUS_TRAINING_FAILED = 6
} DD_DISPLAY_LINK_STATE;

// Display resume states
// For all states we can pass max 4 patameters, if any required. Params TBD,TBD,TBD,TBT
typedef enum _DD_DISPLAY_RESUME_STATES
{
    DD_STATE_UNINITIALIZED                = 0x0,
    DD_SET_POWER_STATE_D3                 = 0x1,
    DD_SET_POWER_STATE_D0                 = 0x2,
    DD_DISPLAY_POWER_ENTRY                = 0x3,
    DD_UNCORE_INIT_WAIT_END               = 0x4,
    DD_SET_POWER_STATE_NOTIFY_ENTRY       = 0X5,
    DD_SET_POWER_STATE_NOTIFY_EXIT        = 0X6,
    DD_ADAPTER_POWER_STATE_EXIT           = 0x7,
    DD_SET_POWER_STATE_OSL_ENTRY          = 0x50,
    DD_SET_POWER_STATE_OSL_EXIT           = 0x70,
    DD_SET_COMP_POWER_STATE_ENTRY         = 0x101,
    DD_SET_COMP_POWER_STATE_EXIT          = 0x180,
    DD_DISPLAY_POWER_EXIT                 = 0x200,
    DD_ACPI_POWER_ENTRY                   = 0x201,
    DD_ACPI_POWER_EXIT                    = 0x300,
    DD_GMM_POWER_ENTRY                    = 0x301,
    DD_GMM_POWER_EXIT                     = 0x400,
    DD_VED_POWER_ENTRY                    = 0x401,
    DD_VED_POWER_EXIT                     = 0x500,
    DD_RENDER_POWER_ENTRY                 = 0x501,
    DD_RENDER_POWER_EXIT                  = 0x600,
    DD_POWERCONS_POWER_ENTRY              = 0x601,
    DD_POWERCONS_POWER_EXIT               = 0x700,
    DD_INTERRUPT_POWER_ENTRY              = 0x701,
    DD_INTERRUPT_POWER_EXIT               = 0x800,
    DD_DETECT_DISPLAY                     = 0x801,
    DD_DISPLAY_MODESET                    = 0x900,
    DD_SOURCE_VISIBILITY                  = 0xA00,
    DD_PENDING_INTERRUPT_OPERATIONS_START = 0xB00,
    DD_PENDING_INTERRUPT_OPERATIONS_EXIT  = 0xBFF,
    DD_RESUME_STATE_MAX                   = 0XFFFF
} DD_DISPLAY_RESUME_STATES;

typedef struct _DD_DISPLAY_LINK_STATE_FOR_TARGETID_V1
{
    DDU64 TimeStamp;
    DDU8 Port;
    DDU8 LinkTrainingCount;
    DD_DISPLAY_LINK_STATE LinkState;
} DD_DISPLAY_LINK_STATE_PER_PORT_V1;

#define DD_DISPLAY_LINK_STATE_VERSION 1
#define DD_DISPLAY_LINK_STATE_BUFFER_SIZE (__BIT(4) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_DISPLAY_LINK_STATE_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_DISPLAY_LINK_STATE_PER_PORT_V1 DispDiagLinkState[DD_DISPLAY_LINK_STATE_BUFFER_SIZE];
} DD_DISPLAY_LINK_STATE_CONTAINER;

typedef struct _DD_DISPLAY_LINK_STATE_ALL_PORT_CONTAINER
{
    DD_DISPLAY_LINK_STATE_CONTAINER DispDiagLinkStateAllPort[DD_PORT_TYPE_MAX_PHYSICAL_PORT];
} DD_DISPLAY_LINK_STATE_ALL_PORT_CONTAINER;

typedef enum _DD_DISPLAY_LID_STATE
{
    DD_DISPLAY_LID_STATE_UNINITIALIZED = 0,
    DD_DISPLAY_LID_STATE_NOTAPPLICABLE = 1,
    DD_DISPLAY_LID_STATE_OPEN          = 2,
    DD_DISPLAY_LID_STATE_CLOSE         = 3,
    DD_DISPLAY_LID_STATE_UNKNOWN       = 4,
} DD_DISPLAY_LID_STATE;

typedef struct _DD_DISPLAY_LID_STATE_V1
{
    DDU64 TimeStamp;
    DDU8 Port;
    DD_DISPLAY_LID_STATE LidState;
} DD_DISPLAY_LID_STATE_V1;

#define DD_DISPLAY_LID_STATE_VERSION 1
#define DD_DISPLAY_LID_STATE_BUFFER_SIZE (__BIT(4) * DD_BB_SIZE_MULTIPLIER)

typedef struct _DD_DISPLAY_PER_PORT_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_DISPLAY_LID_STATE_V1 LidStatePerPort[DD_DISPLAY_LID_STATE_BUFFER_SIZE];
} DD_DISPLAY_PER_PORT_CONTAINER;

typedef struct _DD_DISPLAY_ALL_PORTS_LID_STATE_CONTAINER
{
    DD_DISPLAY_PER_PORT_CONTAINER PortLidStateData[MAX_INTERNAL_DISPLAYS];
} DD_DISPLAY_ALL_PORTS_LID_STATE_CONTAINER;

typedef enum _DD_TDRINFOFLAGS
{
    DD_TDR_NO_RECOVERYREQUIRED = 1,
    DD_TDR_RECOVERYREQUIRED    = 2,
    DD_TDR_RECOVERED           = 3
} DD_TDRINFOFLAGS;

typedef struct _DD_DISPDIAG_TDRINFO_V1
{
    DDU64 TimeStamp;
    DD_TDRINFOFLAGS TDRInfoFlags;
} DD_DISPDIAG_TDRINFO_V1;

#define DD_DIAG_TDRINFOFLAGS_VERSION 1
#define DD_DIAG_TDRINFOFLAGS_BUFFER_SIZE (__BIT(4) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_DIAG_TDRINFOFLAGS_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_DISPDIAG_TDRINFO_V1 TDRInfoFlags;
} DD_DIAG_TDRINFOFLAGS_CONTAINER;

typedef struct _DISPDIAG_UNDERRUNSTATUS_V1
{
    DDU32 UnderrunCounter : DD_BITFIELD_RANGE(0, 24); // Using the first 25 bits for the underrun counter
    DDU32 UNIQUENAME(Reserved) : DD_BITFIELD_RANGE(25, 30);
    DDU32 UnderrunOccured : DD_BITFIELD_BIT(31); // This bit indicates that underrun occured.
} DD_DISPDIAG_UNDERRUNSTATUS_V1;

typedef struct _DISPDIAG_PIPEUNDERRUNDATA_V1
{
    DD_DISPDIAG_UNDERRUNSTATUS_V1 PipeUnderrun_AfterBoot;
    DD_DISPDIAG_UNDERRUNSTATUS_V1 PipeUnderrun_DuringSettiming;
    DD_DISPDIAG_UNDERRUNSTATUS_V1 PipeUnderrun_AfterSettiming;
} DD_DISPDIAG_PIPEUNDERRUNDATA_V1;

#define DD_DIAG_PIPEUNDERRUNDATA 1
#define DD_DIAG_DISPLAY_PIPEUNDERRUN_BUFFER_SIZE (__BIT(4) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_DIAG_DISPLAY_PIPEUNDERRUN_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_DISPDIAG_PIPEUNDERRUNDATA_V1 DispDiagUnderrun[MAX_PIPES]; // Underrun will be based on Pipes
} DD_DIAG_DISPLAY_PIPEUNDERRUN_CONTAINER;

typedef struct _DD_BB_DP_DIAGNOSTIC_DATA
{
    DDU64 TimeStamp;
    DD_DP_DIAGNOSTIC_SOURCE DiagCode;
    DDU32 Param1;
    DDU32 Param2;
    DDU32 Param3;
    DDU32 Param4;
} DD_BB_DP_DIAGNOSTIC_DATA;

#define DD_BB_DP_DIAGNOSTIC_DATA_VERSION 1
#define DD_BB_DP_DIAGNOSTIC_DATA_BUFFER_SIZE (__BIT(6) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_DP_DIAGNOSTIC_DATA_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_DP_DIAGNOSTIC_DATA DiagData[DD_BB_DP_DIAGNOSTIC_DATA_BUFFER_SIZE];
} DD_BB_DP_DIAGNOSTIC_DATA_CONTAINER;

typedef struct _DD_BB_HDMI_DIAGNOSTIC_DATA
{
    DDU64 TimeStamp;
    DD_HDMI_DIAGNOSTIC_SOURCE DiagCode;
    DDU32 Param1;
    DDU32 Param2;
    DDU32 Param3;
    DDU32 Param4;
    DDU32 Param5;
    DDU32 Param6;
} DD_BB_HDMI_DIAGNOSTIC_DATA;

#define DD_BB_HDMI_DIAGNOSTIC_DATA_VERSION 1
#define DD_BB_HDMI_DIAGNOSTIC_DATA_BUFFER_SIZE (__BIT(6) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_HDMI_DIAGNOSTIC_DATA_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_HDMI_DIAGNOSTIC_DATA HdmiDiagnosticData[DD_BB_HDMI_DIAGNOSTIC_DATA_BUFFER_SIZE];
} DD_BB_HDMI_DIAGNOSTIC_DATA_CONTAINER;

typedef struct _DD_BB_DC_STATE
{
    DDU64 TimeStamp;
    DDU8 RequestedDcState;
} DD_BB_DC_STATE;

#define DD_DC_STATE_VERSION 1
#define DD_BB_DC_STATE_BUFFER_SIZE (__BIT(4) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_DC_STATE_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_DC_STATE DcState[DD_BB_DC_STATE_BUFFER_SIZE];
} DD_BB_DC_STATE_CONTAINER;

typedef struct _DD_BB_PSR_STATE
{
    DDU64 TimeStamp;
    PIPE_ID PipeId;
    DD_BOOL IsPsrEntered;
    DDU8 PsrVersion;
} DD_BB_PSR_STATE;

#define DD_PSR_STATE_FOR_PIPE_VERSION 1
#define DD_BB_PSR_STATE_BUFFER_SIZE_FOR_PIPE (__BIT(4) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_PSR_STATE_PER_PIPE_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_PSR_STATE PsrStatePerPipe[DD_BB_PSR_STATE_BUFFER_SIZE_FOR_PIPE];
} DD_BB_PSR_STATE_PER_PIPE_CONTAINER;
typedef struct _DD_BB_PSR_STATE_ALL_PIPE_CONTAINER
{
    DD_BB_PSR_STATE_PER_PIPE_CONTAINER PsrStateAllPipe[MAX_PHYSICAL_PIPES];
} DD_BB_PSR_STATE_ALL_PIPE_CONTAINER;

typedef struct _DD_BB_CMTG_STATE
{
    DDU64 TimeStamp;
    DD_BOOL IsCmtgEnabled;
} DD_BB_CMTG_STATE;

#define DD_CMTG_STATE_VERSION 1
#define DD_BB_CMTG_STATE_BUFFER_SIZE (__BIT(4) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_CMTG_STATE_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_CMTG_STATE CmtgState[DD_BB_CMTG_STATE_BUFFER_SIZE];
    DD_BOOL IsFirstEntryDone;
} DD_BB_CMTG_STATE_CONTAINER;

typedef struct _DD_BB_CMTG_SLAVE_STATE
{
    DDU64 TimeStamp;
    DD_BOOL IsCmtgSlaved;
} DD_BB_CMTG_SLAVE_STATE;

#define DD_CMTG_SLAVE_STATE_FOR_PIPE_VERSION 1
#define DD_BB_CMTG_SLAVE_STATE_BUFFER_SIZE_FOR_PIPE (__BIT(4) * DD_BB_SIZE_MULTIPLIER)
typedef struct _DD_BB_CMTG_SLAVE_STATE_PER_PIPE_CONTAINER
{
    DD_BB_SECTION_HEADER Header;
    DD_BB_CMTG_SLAVE_STATE CmtgSlaveStatePerPipe[DD_BB_CMTG_SLAVE_STATE_BUFFER_SIZE_FOR_PIPE];
    DD_BOOL IsFirstEntryDone;
} DD_BB_CMTG_SLAVE_STATE_PER_PIPE_CONTAINER;
typedef struct _DD_BB_CMTG_SLAVE_STATE_ALL_PIPE_CONTAINER
{
    DD_BB_CMTG_SLAVE_STATE_PER_PIPE_CONTAINER CmtgSlaveStateAllPipe[MAX_PHYSICAL_PIPES];
} DD_BB_CMTG_SLAVE_STATE_ALL_PIPE_CONTAINER;

typedef enum _DD_GET_DISPLAY_STATE_SUBSTATUS_FLAGS
{
    DD_GETDISPLAYSTATE_SUCCESS                 = 0x0,
    DD_GETDISPLAYSTATE_CAUSED_GLITCH           = 0x1,
    DD_GETDISPLAYSTATE_CHANGED_DISPLAY_STATE   = 0x2,
    DD_GETDISPLAYSTATE_MONITOR_NOT_CONNECTED   = 0x4,
    DD_GETDISPLAYSTATE_TIMEOUT                 = 0x8,
    DD_GETDISPLAYSTATE_ERROR_HARDWARE          = 0x10,
    DD_GETDISPLAYSTATE_ERROR_DRIVER            = 0x20,
    DD_GETDISPLAYSTATE_VIDPNTARGETID_NOT_FOUND = 0x40,
} DD_GET_DISPLAY_STATE_SUBSTATUS_FLAG;

typedef struct _DD_DIAG_TARGET_DATA
{
    DDU32 TargetId;
    DDU8 Port;
    DDU8 UniqueIndex;
    DDU8 SinkType;
    DDU8 SinkIndex;
} DD_DIAG_TARGET_DATA;

typedef struct _DD_DIAG_DISPLAY_STATE_NON_INTRUSIVE
{
    DD_DIAG_TARGET_DATA TargetData;
    DDU8 LfpIndex;
    DD_BOOL IsLfpSinkType;
    DD_DISPLAY_CONNECTIVITY DisplayConnectivity;
    DD_DISPLAY_LID_STATE DisplayLidState;
    DD_BASIC_DISPLAY_TOPOLOGY DisplayTopology;
    DD_DISPLAY_LINK_STATE DisplayLinkState;
    DD_DISPLAY_MODE_SET DisplayModeSet;
    DD_GET_DISPLAY_STATE_SUBSTATUS_FLAG ReturnSubStatus;
} DD_DIAG_DISPLAY_STATE_NON_INTRUSIVE;

typedef enum _DD_DISPLAY_MONITOR_STATE
{
    DD_DISPLAY_MONITOR_STATE_UNINITIALIZED = 0,
    DD_DISPLAY_MONITOR_READY,
    DD_DISPLAY_MONITOR_NOT_READY,
    DD_DISPLAY_MONITOR_READY_NOT_APPLICABLE
} DD_DISPLAY_MONITOR_STATE;

typedef enum _DD_DISPLAY_HARDWARE_ERROR_STATE
{
    DD_DISPLAY_HARDWARE_ERROR_STATE_UNINITIALIZED = 0,
    DD_DISPLAY_HARDWARE_ERROR_NONE,
    DD_DISPLAY_HARDWARE_ERROR_SCANOUT_UNDERFLOW,
    DD_DISPLAY_HARDWARE_ERROR_TDRNORECOVERY,
    DD_DISPLAY_HARDWARE_ERROR_UNSPECIFIED
} DD_DISPLAY_HARDWARE_ERROR_STATE;

typedef enum _DD_DISPLAY_SCANOUT_BUFFER_CRC
{
    DD_DISPLAY_SCANOUT_BUFFER_CRC_UNINITIALIZED = 0,
    DD_DISPLAY_SCANOUT_BUFFER_CRC_BLACK,
    DD_DISPLAY_SCANOUT_BUFFER_CRC_NON_BLACK,
    DD_DISPLAY_SCANOUT_BUFFER_CRC_ERROR,
    DD_DISPLAY_SCANOUT_BUFFER_CRC_UNKNOWN
} DD_DISPLAY_SCANOUT_BUFFER_CRC;

typedef struct _DD_DISPLAY_SCANOUT_BUFFER_HISTOGRAM
{
    DDS32 MinPixelValue;
    DDS32 MaxPixelValue;
} DD_DISPLAY_SCANOUT_BUFFER_HISTOGRAM;

typedef struct _DD_DISPLAY_SCANOUT_BUFFER_CONTENT
{
    DD_DISPLAY_SCANOUT_BUFFER_CRC ScanoutBufferCrc;
    DD_DISPLAY_SCANOUT_BUFFER_HISTOGRAM ScanoutBufferHistogram;
} DD_DISPLAY_SCANOUT_BUFFER_CONTENT;

typedef enum _DD_DISPLAY_HARDWARE_BANDWIDTH
{
    DD_DISPLAY_HARDWARE_BANDWIDTH_UNINITIALIZED = 0,
    DD_DISPLAY_HARDWARE_BANDWIDTH_SUFFICIENT,
    DD_DISPLAY_HARDWARE_LINK_BANDWIDTH_LIMITED,
    DD_DISPLAY_HARDWARE_SOC_BANDWIDTH_LIMITED,
    DD_DISPLAY_HARDWARE_BANDWIDTH_ERROR,
    DD_DISPLAY_HARDWARE_BANDWIDTH_UNKNOWN
} DD_DISPLAY_HARDWARE_BANDWIDTH;

typedef struct _DD_DIAG_DISPLAY_STATE_INTRUSIVE
{
    DD_DIAG_TARGET_DATA TargetData;
    DDU8 PipeId;
    DD_DISPLAY_CONNECTIVITY DisplayConnectivity;
    DD_DISPLAY_MONITOR_STATE MonitorState;
    DD_DISPLAY_SCANOUT_STATE DisplayScanoutState;
    DD_DIAG_DISPLAY_SAMPLED_GAMMA DisplaySampledGamma;
    DD_DIAG_DISPLAY_CSC_DATA_INTEGER DisplayCurrentCSC;
    DD_DISPLAY_SCANOUT_BUFFER_CONTENT DisplayBufferContent;
    DD_DISPLAY_HARDWARE_ERROR_STATE DisplayErrorState;
    DD_DISPLAY_HARDWARE_BANDWIDTH DisplayBandwidth;
    DD_GET_DISPLAY_STATE_SUBSTATUS_FLAG ReturnSubStatus;
} DD_DIAG_DISPLAY_STATE_INTRUSIVE;

typedef enum _DD_DIAG_PORT_CONNECTION_DETAILS
{
    DD_DIAG_CONNECTION_TYPE_INVALID = -1,
    DD_DIAG_CONNECTION_TYPE_DIRECT  = 0,
    DD_DIAG_CONNECTION_TYPE_INDIRECT_THROUGH_SST,
    DD_DIAG_CONNECTION_TYPE_INDIRECT_THROUGH_MST,
    DD_DIAG_CONNECTION_TYPE_INDIRECT_THROUGH_CONVERTOR,
    DD_DIAG_CONNECTION_TYPE_INDIRECT,
    DD_DIAG_CONNECTION_TYPE_MAX
} DD_DIAG_CONN_TYPE;

#define DD_BLACK_BOX_DATA_VERSION_0X02 2
#define DD_BLACK_BOX_DATA_VERSION_0X03 3
#define DD_BLACK_BOX_DATA_VERSION DD_BLACK_BOX_DATA_VERSION_0X03 // When updating version, add new #define and use here

typedef struct _DD_BLACK_BOX
{
    DD_BB_SECTION_HEADER Header;

    // Circular Buffers
    // Common
    // Keeping Seperate Trackers for Passive vs ApcDpc vs Dirql
    DD_BB_DDI_TRACK_DATA_CONTAINER DdiTrackDataPassive;
    // DD_BB_DDI_TRACK_DATA_CONTAINER DdiTrackDataApcDpc; // Not used as of now
    // DD_BB_DDI_TRACK_DATA_CONTAINER DdiTrackDataDirql; // Not used as of now

    // Detection - Display Activation
    // Splitting LPI Events into HPD, Livestate, Type-C
    DD_BB_LPI_HPD_DATA_CONTAINER LpiHpdData;
    DD_BB_LPI_LIVESTATE_DATA_CONTAINER LpiLivestateData;
    DD_BB_LPI_TYPE_C_DATA_CONTAINER LpiTypeCData;
    DD_BB_SPI_DATA_CONTAINER SpiData;
    DD_BB_SET_TIMING_DATA_CONTAINER SetTimingData;
    DD_BB_PIPE_STATE_DATA_CONTAINER PipeStateData;

    // Flip
    DD_BB_VBI_CONTROL_DATA_CONTAINER VbiControlData;
    // VBI - FlipDone

    // Sequences
    DD_BB_SCENARIO_COLLECTION_CONTAINER SequenceData;

    // Diagnostic Error Data
    DD_BB_DIAGNOSTIC_ERROR_DATA DiagnosticErrorData;

    // Diagnostic Info Data
    DD_BB_DIAGNOSTIC_INFO_DATA DiagnosticInfoData;

    // Diagnostic Warning Data
    DD_BB_DIAGNOSTIC_WARNING_DATA DiagnosticWarningData;

    // Single entry. Will contain the first entry
    // Error Detection
    DD_BB_UNDERRUN_DBG_DATA_CONTAINER UnderRunDbgData;

    DD_DISPLAY_DATA_FOR_TARGETID_VERSION_CONTAINER DispDiag_TargetID;
    DD_DISPLAY_CONNECTIVITY_CONTAINER DisplayConnectivity;
    DD_DISPLAY_LINK_STATE_ALL_PORT_CONTAINER DisplayLinkState;
    DD_DISPLAY_ALL_PORTS_LID_STATE_CONTAINER DisplayLidState;
    DD_DIAG_DISPLAY_SAMPLED_GAMMA_CONTAINER DisplaySampledGamma;
    DD_DIAG_TDRINFOFLAGS_CONTAINER DispDiag_TDRInfo;
    DD_DIAG_DISPLAY_PIPEUNDERRUN_CONTAINER DispDiag_UnderrunInfo;
    DD_DIAG_DISPLAY_SAMPLED_GAMMA_ALL_PIPE_CONTAINER DispDiagSampledGammaAllPipe;
    DD_DIAG_DISPLAY_CSC_ALL_PIPE_CONTAINER DispDiagCSCAllPipe;
    DD_BB_FUNC_TRACK_DATA_CONTAINER DisplayFunctionTracker;
    DD_BB_SCANOUT_STATE DisplayScanOutState;
    DD_BB_ASSERT_DATA_CONTAINER AssertTracker;

    DD_BB_RR_SWITCHING_CAPS_CONTAINER RrSwitchingCaps;
    DD_BB_RR_SWITCHING_STATES_CONTAINER RrSwitchingStates;

    // Diagnostic Data specific to Protocol's (DP/HDMI)
    DD_BB_DP_DIAGNOSTIC_DATA_CONTAINER DpDiagnosticData;
    DD_BB_HDMI_DIAGNOSTIC_DATA_CONTAINER HdmiDiagnosticData;

    DD_BB_DC_STATE_CONTAINER DcStateData;
    DD_BB_PSR_STATE_ALL_PIPE_CONTAINER PsrStateDataAllPipe;
    DD_BB_CMTG_STATE_CONTAINER CmtgStateData;
    DD_BB_CMTG_SLAVE_STATE_ALL_PIPE_CONTAINER CmtgSlaveStateDataAllPipe;
} DD_BLACK_BOX;

#pragma pack()
#define DD_BLACK_BOX_SIZE_LIMIT DD_KILO(102)              // Size in KB to keep track of the growth
C_ASSERT(sizeof(DD_BLACK_BOX) < DD_BLACK_BOX_SIZE_LIMIT); // Keep the black box data size below the limit

/////////////////////////////////////////////////////////////////////////
#endif // _DISPLAY_DIAG_H_
