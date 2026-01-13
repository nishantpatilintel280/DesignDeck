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
 * @file  DisplayArgsInternal.h
 * @ingroup driver
 * @brief display args -- Used to define structures/interfaces used by external non-display components.
 *
 */

#pragma once
#include "DisplayArgs.h"
#include "DisplayDiagDef.h"

#ifdef _DISPLAY_INTERNAL_
#include "DisplayInfoParser/iHDMI.h"

 // SKU and WA Table related
#define GFX_IS_DISP_SKU(s, f) ((s)->pSkuTable->f)
#define GFX_IS_DISP_WA(s, w) ((((s)->pWaTable->w) & ((s)->pWaTable->DisplayWaMask)) ? TRUE : FALSE)
#define GFX_WRITE_DISP_WA(x, y, z) ((x)->pWaTable->y = z)
#define GFX_WRITE_DISP_SKU(x, y, z) ((x)->pSkuTable->y = z)

//----------------------------------------------------------------------------
//
// Generic CONSTANTS, ENUMS and MACROS - START
//
//----------------------------------------------------------------------------
#define MAX_PHYSICAL_PORTS_GEN10 5   // 4 DDIs A - D + F
#define MAX_PHYSICAL_PORTS_GEN11 6   // 2 Combo Phy DDIs (A/B) and 4 Type-C DDIs (C-F)
#define MAX_PHYSICAL_PORTS_GEN11P5 9 // 3 Combo Phy DDIs (A/B/C) and 6 Type-C DDIs (D-I)
#define MAX_PHYSICAL_PORTS_JSL 4     // 4 Combo Phy DDIs A-D

#define MAX_FBC_PIPES 4
#define MAX_FBC_PIPES_GEN14 2
#define ALIGN_TO_NEXT_4K(addr) (((addr & 0xfff) == 0) ? addr : ((addr & 0xfffffffffffff000) + 0x1000))
#define MIN_CFB_SIZE 0x1000000 // 16MB is decided as viable and minimum CFB allocation

#define DUAL_PIPE_TILED_CONFIG 2 // 2 pipe Tiled display config.
#define DD_MAX_NUM_REFRESH_RATES 5

#define NUM_FRL_LINK_RATES 7
#define NUM_MAX_FRL_RATE_INDEX 15

#define CONVERT_TO_ASYNC_SCANLINE_BUFFER 10

#define DD_MAX_LOGICAL_PLANES_PER_PIPE 3

#define DD_MAX_SYMBOL_CLOCK_IN_MHZ_DVFS 810

#define HW_ADDR_FOR_DISABLED_PLANE (DD_MULT_BY_4K(DDMAXU32))

#define MAX_COLOR_BPP 4 // 8, 16, 32 & 64 bpps (doesn't include 4bpp)
#define ALLCOLOR_BPP_MASK 0x1F
#define WINDOWS_COLOR_MASK 0x1E // doesn't have 4bpp

#define INVERSE_100_NANO_SEC 10000000
#define INVERSE_MICRO_SEC 1000000
#define OS_WA_SHRINK_FACTOR_MULT_BY_100 299
#define OS_WA_NUM_LINES_ALLOW_SHRINK 3 // Accomodating OS Request for 3 pixel MPO Downscaling - BugFix:https://hsdes.intel.com/appstore/article/#/1606713528

#define MAX_NUM_UNIQUE_COEF_FOR_SHARPNESS_FILTER 7 // Our HW supports up to 7 tap filter and hence max unique coeffs for sharpness filter is 7, Currently we use only 3 coef at max for 5 tap filter

#define MILLISECOND_TO_HUNDRED_NANOSECONDS (10 * 1000) // Convert Milisecond to 100ns units
#define HUNDREDPERCENT_TO_VALUE(n) (n / 10000)         // Converts hundred percent to actual value

// Color related
#define CUI_LUT_PREFIX L"CUILut%X"
#define ESCAPE_LUT_PREFIX L"CUILut%s%X"
#define OS_LUT_PREFIX L"OSLut%X"
#define IGCL_DGLUT_PREFIX L"IGCLDGLut%X"
#define ABSOLUTE_LUT_PREFIX L"AbsLut%X"
#define RELATIVE_NON_LINEAR_CSC_PREFIX L"RelCSC" // For Non Linear Relative CSC from Escape Call
#define RELATIVE_LINEAR_CSC_PREFIX L"LinearRelCSC"
#define OS_LINEAR_COLOR_TRANSFORM_CSC_PREFIX L"OSLinearColorTransformCsc"
#define REG_KEY_OUTPUT_COLOR_MODEL_PREFIX L"OutputColorModel%X"
#define SELECT_BPC_FROM_REG_KEY L"SelectBPCFromRegistry"
#define REGISTRY_OVERRIDE_BPC_VALUE_KEY L"SelectBPC"
#define REGISTRY_OVERRIDE_HDRMODE_KEY L"ForceHDRMode"
#define REGISTRY_OVERRIDE_FORCE_YUV422_KEY L"ForceApplyYUV422Mode"
#define REGISTRY_HDMI_HPD_DETCTION_DELAY_IN_MS L"HdmiHpdDelayInMs"
#define REGISTRY_DP_FEC_SEQ_RETRY_COUNT L"DpFecSeqRetryCount"

#define DD_MAX_CSC 261632 // 3.9921875*2^16
#define DD_COLOR_3D_LUT_DEPTH 17
#define DD_COLOR_3D_LUT_VALIDATION_THRESHOLD 0.33 // 33 %
#define TONE_MAPPING_MAX_OUTPUT_LUMINANCE 10000
#define DISPLAY_TONE_MAPPING_LUT_SIZE 257
#define BPC_SUPPORTED_8BITS_PER_COLOR BIT3 // bit 3 is set in DDU16 ColorDepthMask
#define BPC_SUPPORTED_6BITS_PER_COLOR BIT1 // bit 1 is set in DDU16 ColorDepthMask

#define MAX_PIPES_WITH_3DLUT_SUPPORTED_GEN12 2
#define MAX_PIPES_WITH_3DLUT_SUPPORTED_GEN13 2
#define MAX_PIPES_WITH_3DLUT_SUPPORTED_GEN14 2
#define MAX_PIPES_WITH_3DLUT_SUPPORTED_GEN15 2

#define MAX_DMC_EVENTS 8 // Currently same across all HALs

#define DD_MAX_VBT_ENCODERS 10

#define SHARP_COEFF_TO_REG_FORMAT(Coeff) (DDU16)(Coeff.Sign << 15 | Coeff.Exponent << 12 | Coeff.Mantissa << 3)
// With Downscale Percentage as 11 for both X and Y,
// the downscale ratio stays within the 1.125 limit on custom scaling
// The downscale ratio of 1.125 is used in both WM and CD clock  downscaling calculations
// HSD Link: https://hsdes.intel.com/appstore/article/#/1508258830
#define MAX_CUSTOM_DOWN_SCALE_PERCENT_X 11
#define MAX_CUSTOM_DOWN_SCALE_PERCENT_Y 11

#define DD_CONVERT_NITS_TO_MILLINITS(Input) (Input * 1000)
#define DD_CONVERT_MILLINITS_TO_NITS(Input) DD_ROUND_DIV(Input, 1000)

#define DD_CONVERT_MM_TO_CM(Input) DD_ROUND_DIV(Input, 10)
#define DD_CONVERT_CM_TO_MM(Input) (Input * 10)

#define PAR_VALUE_FROM_ASPECT_RATIO_DW(dwAspectRatio) (dwAspectRatio & 0x007)
#define SUPPORTED_PAR_FROM_ASPECT_RATIO_DW(dwAspectRatio) (dwAspectRatio >> 16)

// Pre-scale excess pixels for multi-pipe SD configruation. Caluclated as (NUMTAPS - 1)/ 2
// where NUMTAPS parameter for the Scaler's Horizontal FIR is 7 which will remain constant
// atleast till Gen13 as per HW arch. Hence, pre-scale excess pixels = (7 - 1)/ 2 = 3 which
// needs to be rounded up to next even number i.e. 4 as Display pipe requires even number
// of line pixels
#define PRE_SCALE_EXCESS_PIXELS 4

// Integer Scaling Support

#define MIN_SUPPORTED_CUSTOM_SOURCE_MODE_WIDTH 320
#define MIN_SUPPORTED_CUSTOM_SOURCE_MODE_HEIGHT 200

#define NUM_NN_COEF_REG_VAL_IN_UNIQUE_PATTERN 0x7

// Display Number of pixels per clock
#define DISP_NUMPPC 2

// Dynamic Cd clock related
#define PERCENTAGE_PIXEL_CLK_LIMIT_IN_10 1000
#define PERCENTAGE_PIXEL_CLK_LIMIT_FOR_INCREASED_CD_CLK_IN_10 875

// DVFS notification related
#define DVFS_DDI_SYMBOL_CLK_594_MHZ 594

#define MAX_KVM_H_RES 3840
#define MAX_KVM_V_RES 2160

#define DD_MAX_LOGICAL_TILE_HEIGHT 32
#define DD_LINEAR_TILE_WIDTH 64
#define DD_DEFAULT_TILE_WIDTH DD_LINEAR_TILE_WIDTH
#define DD_X_TILE_WIDTH 512
#define DD_LEGACY_Y_TILE_WIDTH 128
#define DD_4_TILE_WIDTH 128
#define DD_IS_SURFACE_TILE4_OR_LEGACY_Y_TILE(SurfaceFormat) ((SurfaceFormat == SURFACE_MEMORY_TILE_4) || (SurfaceFormat == SURFACE_MEMORY_Y_LEGACY_TILED))
#define DD_IS_SURFACE_TILE4(SurfaceFormat) (SurfaceFormat == SURFACE_MEMORY_TILE_4)
#define DD_PAGE_SIZE 0x1000

// Header = 4, Baseline Data = 80 and Vendor (INTEL) specific = 2 as per EELD spec
// 4 + 80 + = 84
#define DD_ELD_BLOCK_SIZE 84
#define MAX_MONITOR_NAME_LENGTH 31

// Maximum Audio SAD blocks allowed are 14
#define DD_MAX_SAD_BLOCK_COUNT 14

// Total combinations of vswing/pre-emph values can be 10
#define MAX_VSWING_PREEMPH_VALUE 10

// Link MN ratio macros
#define LINK_MN_RATIO_10 10
#define LINK_MN_RATIO_15 15

// For Dual LFP Alignment Support (VSRI -4335), driver gets input border values from VBT
// At Present, OEM requirement doesnot have any Limitation to these values as such.
// Hence creating an arbitrary limitation of 127 as max value, as the fields as 8bit
#define DD_MAX_BORDER_FOR_CUSTOM_ALIGNMENT 0x7F

#define DD_MAX_MMIO_OFFSET_DATA_PAIR_ENTRY_IN_FLIP_QUEUE 32

// Minimum RR of known panel used for PSR1/2 exit delay calculation (Not a HW limitation)
// PANELREPLAY EDP Min Rr Support
// Minimum Rr to obtain Max Vtotal for Frametime calculations.
#define MIN_RR_SUPPORT 24

// Default PSR2 Y granularity
#define DEFAULT_PSR2_Y_GRANULARITY_IN_SCANLINES 4

#define PSR_IDLE_FRAME_THRESHOLD 4

#define PSR2_OVERWRITE_IDLE_FRAME_THRESHOLD 1

// The min observated time for PSR state changed from deep sleep to idle is about 500us
// Use min time as polling delay in PSR disable to reduce overmuch MMIOs
#define PSR_DISABLE_POLL_DELAY_IN_US 500

#define PSR_MAX_SLEEP_TIME 31

// Minimum RR of known panel used for PanelReplay exit delay calculation (Not a HW limitation)
#define PANELREPLAY_MIN_RR_SUPPORT 48

#define XPST_GUARDBAND_INTERRUPT_DELAY 4
#define PHASE_BLC_PERIOD_IN_MS 15 // 15ms - BLC phased in period.

#define HISTOGRAM_BIN_READ_RETRY_COUNT 3
#define HISTOGRAM_BIN_READ_DELAY_BEFORE_RETRY 50 // 50 us

#define PSR2_SDP_TRANS_TIME_IN_LINKSYMBOLCLOCKS \
    60 // This is the time in LinkSymbolClocks needed to send 32 bytes of PSR2 SDP data as per bspec i.e. VBID/Mvid/Maud  (12 symbols/lanes) clocks + SDP  (48 symbols/lanes) clocks

#define PSR2_SDP_TRANS_TIME_ADDITIONAL_CLOCKS 11 // 	BS/SR  4 clocks + RTL  5 clocks arbitrary delay + SS  1 clock + SE  1 clock

// For Gen13+ platform, min PSR2 TP2 time is reduced from 2.5ms to 50us.
// The min observated PSR disable time is about 100us
// Use min time as polling delay in PSR2 disable to reduce overmuch MMIOs
#define PSR2_DISABLE_POLL_DELAY_IN_US 100

// When display is set as secondary mode to CMTG, driver checks for CMTG scanline/frame counter instead of pipe scanline/frame counter.
// CMTG is always running unless display is in DC6. Considering lowest panel timing (640x480) and lowest refresh rate(24), one scanline time shouldn't be more
// than 100us. Still giving buffer of 400us for CMTG scanline counter increment.
#define CMTG_SCANLINE_DELAY_IN_US (0.5 * DD_MILLI_TO_MICRO_SEC)

// 1 frame time delay in us (1/refresh rate) * (1000*1000)
#define ONEFRAME_DELAY_IN_US(RefreshRate) ((RefreshRate > 0) ? ((1000 * 1000) / RefreshRate) : 10000)

// For Non-PSR displays, pipe scanline counter should be incrementing always. Using 500us as timeout for pipe scanline verification.
#define DEFAULT_SCANLINE_INCREMENT_DELAY_IN_US (0.5 * DD_MILLI_TO_MICRO_SEC)

// DSC Related
#define DSC_BPP_24 (24 << 4)
#define DSC_BPP_20 (20 << 4)
#define DSC_BPP_16 (16 << 4)
#define DSC_BPP_15 (15 << 4)
#define DSC_BPP_14 (14 << 4)
#define DSC_BPP_12 (12 << 4)
#define DSC_BPP_10 (10 << 4)
#define DSC_BPP_8 (8 << 4)
#define DSC_BPP_7 (7 << 4)
#define DSC_BPP_6 (6 << 4)
#define DSC_BPP_7P6875 123
#define DSC_BPP_7P375 118
#define DSC_BPP_7P4375 119
#define DSC_BPP_6P4375 103
#define DSC_BPP_10P8125 173
#define DSC_BPP_10P9375 175
#define DSC_BPP_8P0625 129
#define DSC_BPP_6P625 106
#define DSC_BPP_8P5625 137
#define DSC_BPP_7P0625 113
#define DSC_BPP_9P8125 157
#define DSC_BPP_9P9375 159
#define DSC_BPP_8P375 134
#define DSC_BPP_8P125 130

#define MAX_DSC_BPP_16 16
#define MAX_DSC_BPP_12 12
#define MIN_PIXELS_PER_DSC_SLICE_444 15000
#define MIN_PIXELS_PER_DSC_SLICE_420_422 30000
#define MAX_BPP_SUPPORTED_BY_JOINER_RAM 0xFFF
#define DSC_BPP_ALLOWED_RANGE_MAX 0
// Compression BPP is represented in U6.4 format.
// 6 bits integral and 4 bits fractional.
#define COMPRESSION_BPP_FRACTIONAL_BITS 4
// Conversion from floating point to fixed point is done by taking the input integer value
// and multiplying it by (2^fractional_bits)
#define FIXED_POINT_U6_4_CONV_MULT ((DDU8)(1 << COMPRESSION_BPP_FRACTIONAL_BITS))
// Conversion from fixed-point to actual real value is done by taking the input integer value
// and dividing it by (2^fractional_bits)
#define FIXED_POINT_U6_4_TO_REALVALUE_CONV_DIVISOR ((DDU8)(1 << COMPRESSION_BPP_FRACTIONAL_BITS))
// Slices Per Scanline needed to Enable the Third VDSC Instance. To be supported only with Ultra-Joiner Modes.
#define SLICES_PER_LINE_NEEDED_FOR_THIRD_VDSC_INSTANCE_SUPPORT 12
// Total Number of VDSC Instances when the Third VDSC Instance is enabled per Pipe for Ultra-Joiner Modes.
#define TOTAL_VDSC_INSTANCES_WITH_THREE_VDSC_INSTANCE_PER_PIPE 12

// By default there is no constraint and Successive-Frame-Duration-Increase-Tolerance can be set it to a high default value (Vmax * Factor below) that is equivalent of No-Constraint
#define VRR_ADAP_BAL_SFDIT_VMAX_MULT_FACTOR 5
#define INTEL_ARC_SYNC_PROFILE_GOOD_FRAME_TIME_FACTOR 1.2
#define INTEL_ARC_SYNC_PROFILE_COMPATIBLE_FRAME_TIME_FACTOR 1.0
#define VRR_ADAP_BAL_MAX_FRAME_TIME_TOLERANCE_IN_US 63750

#define DD_VIRTUAL_DISP_SCANNER_PERIOD 17 * DD_MILLI_TO_MICRO_SEC *DD_MICRO_TO_100_NANO_SEC // Should be 16.667 for perfect 60 Hz. Selecting 17 for better timer accuracy

#define VRR_HDMI_2_1_MAX_VALID_MIN_RR_VALUE 48
#define VRR_HDMI_2_1_MIN_VALID_MAX_RR_VALUE 100

#define DD_INVALID_PROCESS_ID DDMAXU32
#define DD_INVALID_PROCESS_TABLE_INDEX DDMAXU8

#define DD_COLOR_DEPTH_6BPC 6
#define DD_COLOR_DEPTH_8BPC 8
#define DD_COLOR_DEPTH_10BPC 10
#define DD_COLOR_DEPTH_12BPC 12
#define DD_COLOR_DEPTH_24BPP 24
#define DD_DIV_COEFF_FOR_RGB 1
#define DD_DIV_COEFF_FOR_YUV420 2
#define DD_MUL_COEFF_FOR_RGB 3
#define DD_MUL_COEFF_FOR_YUV422 2

#define PIXEL_CLOCK_MULTIPLIER_FOR_10BPC_IN_HUNDRED 125 // ((10 BPC / 8 BPC) * 100) = (1.25 * 100) = 125
#define PIXEL_CLOCK_MULTIPLIER_FOR_12BPC_IN_HUNDRED 150 // ((12 BPC / 8 BPC) * 100) = (1.50 * 100) = 150

// This is to compare vblank time of a panel being very high
#define TARGET_MODE_VERY_HIGH_VBLANK_TIME_IN_NS (5000 * DD_1K)

// Assuming 5ms large Vblank and max of required Vblank as 1ms, leaving 4ms as W2.
#define TARGET_MODE_VERY_HIGH_WINDOW2_TIME_IN_US (4 * DD_1K)

// Vblank limit for a timing
#define TARGET_MODE_VBLANK_LIMIT_IN_NS (460 * DD_1K)

// Number of Payload Bytes (M) in an Adaptive-Sync Operation Mode and Range Descriptor.
// Descriptor size was defined as 6 initially in revision 0. Later a field got added in header to make it variable.
// M can be determined by adding the number of extra bytes defined in 6:4 (01h) with initial size (6).
#define DD_GET_DID_AS_DESCRIPTOR_SIZE(PayloadBytesInDescriptor) (6 + (PayloadBytesInDescriptor))

// HW Darkscreen Detection Macros
#define DARKSCREEN_PROGRAMMED_COMPARE_VALUE_CALCULATION_FACTOR 12
// Compare Value = 16*(2 ^ (bpc-8))
#define DARKSCREEN_COMPARE_VALUE_LIMITED_RANGE_6_BPC 4
#define DARKSCREEN_COMPARE_VALUE_LIMITED_RANGE_8_BPC 16
#define DARKSCREEN_COMPARE_VALUE_LIMITED_RANGE_10_BPC 64
#define DARKSCREEN_COMPARE_VALUE_LIMITED_RANGE_12_BPC 256

// Forward declarations
typedef struct _DSC_CFG DSC_CFG;

// Delays used when training pattern is Idle
#define MIN_IDLE_PATTERNS_SENT_DELAY_SST_IN_US 800  // 800us
#define MIN_IDLE_PATTERNS_SENT_DELAY_MST_IN_US 1640 // 1640us

#define DD_GET_SU_ALPM_WAKE_TIME(TimeInUs, RefreshRate, VTotal) ((DDU32)(DD_ROUND_UP_DIV(((TimeInUs)*DD_PRECISION3DEC), (((MEGA_HERTZ * DD_PRECISION3DEC) / (RefreshRate)) / (VTotal)))))

//----------------------------------------------------------------------------
//
// Generic CONSTANTS, ENUMS and MACROS - END
//
//----------------------------------------------------------------------------

// Used as a value for D3DDDI_VIDEO_PRESENT_SOURCE_ID and D3DDDI_VIDEO_PRESENT_TARGET_ID types to specify
// that the respective video present source/target ID hasn't been initialized.
// #define DD_TARGET_ID_UNINITIALIZED (DDU32)(~0)

//----------------------------------------------------------------------------
//
// Cursor related constants and data structures -- END
//
//----------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
//
// OSL data structures - START
//
//----------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////

typedef enum _DD_MEDIA_RR_TYPE
{
    DD_MEDIA_RR_UNINITIALIZED = 0,
    DD_MEDIA_RR_FRACTIONAL = 1,
    DD_MEDIA_RR_INTEGER = 2,
    DD_MEDIA_RR_NONE = 3
} DD_MEDIA_RR_TYPE;

// DDK/OS independent values defined from DD
typedef enum _DD_PIXEL_VALUE_ACCESS_MODE
{
    DD_PVAM_UNINITIALIZED = 0,
    DD_PVAM_DIRECT = 1,
    DD_PVAM_PRESETPALETTE = 2,
    DD_PVAM_SETTABLEPALETTE = 3,
} DD_PIXEL_VALUE_ACCESS_MODE;

#define DD_IS_SMOOTH_SYNC_PLANE(PlaneType) ((DD_SS_PLANE_TYPE_FRONT == PlaneType) || (DD_SS_PLANE_TYPE_BACK == PlaneType))
typedef enum _DD_SMOOTH_SYNC_PLANE_TYPE
{
    DD_SS_PLANE_TYPE_NONE = 0,
    DD_SS_PLANE_TYPE_BACK = 1,
    DD_SS_PLANE_TYPE_FRONT = 2,
} DD_SMOOTH_SYNC_PLANE_TYPE;

// CCD structures

typedef struct _DD_GET_LIVE_SURF_ADDRESS_ARGS
{
    DD_IN PIPE_ID PipeId;
    DD_IN DDU32 PlaneMask; // Mask to indicate which HW plane needs the update.
    DD_OUT DDU32 LiveAddress[MAX_PLANES_PER_PIPE];
    DD_OUT DD_BOOL FlipDone[MAX_PLANES_PER_PIPE];

    // "FlipEvent" indicates if it is Flip Done or VBlank event to HAL layer. It must be used only when
    // pPlatformParams->pWaTable->WaIgnorePlaneSurfLiveAddressOnPlaneEnable is TRUE. For normal cases it must not be used at all.
    DD_IN DD_BOOL FlipEvent;
    DD_IN DD_BOOL ReadSurfAddress;
    DD_IN DD_BOOL ReadPlaneYOffsets;
    DD_OUT DDU32 SurfAddress[MAX_PLANES_PER_PIPE]; // filled by HAL when ReadSurfAddress flag is set
    DD_OUT DDU32 PlaneYLiveAddress[DD_MAX_LOGICAL_PLANES_PER_PIPE];
    DD_OUT DDU32 PlaneYSurfAddress[DD_MAX_LOGICAL_PLANES_PER_PIPE];
} DD_GET_LIVE_SURF_ADDRESS_ARGS;

typedef struct _DD_GET_CURSOR_LIVE_SURF_ADDRESS_ARGS
{
    DD_IN PIPE_ID PipeId;
    DD_OUT DDU32 LiveAddress;
    DD_OUT DDU32 BaseAddress;
} DD_GET_CURSOR_LIVE_SURF_ADDRESS_ARGS;

//----------------------------------------------------------------------------
//
// MPO related data structures - START
//
//----------------------------------------------------------------------------
typedef struct _DD_SURFACE_SCANOUT_DETAILS
{
    DD_IN DDU32 PanningPosX;
    DD_IN DDU32 PanningPosY;
    DD_IN DDU32 UvPanningPosX;
    DD_IN DDU32 UvPanningPosY;
    DD_IN DDU32 ScanSurfSizeX;
    DD_IN DDU32 ScanSurfSizeY;
} DD_SURFACE_SCANOUT_DETAILS;

typedef enum _NN_SCALING_STATE
{
    NN_SCALING_DISABLE,           // Disable NN Scaling
    NN_SCALING_ENABLE,            // Enable NN Scaling
    FORCE_INTEGER_SCALING_ENABLE, // Force Integer Scaling

} NN_SCALING_STATE;

typedef enum _DD_CAPPED_FPS_STATE
{
    DD_CAPPED_FPS_STATE_DISABLE, // Disable RRC_ASYNC_FLIPS
    DD_CAPPED_FPS_STATE_ENABLE,  // Enable RRC_ASYNC_FLIPS
    DD_CAPPED_FPS_STATE_AUTO,    // Auto RRC_ASYNC_FLIPS based on Plan and Power source
} DD_CAPPED_FPS_STATE;

typedef enum _DD_ALP_MODE
{
    DD_ALPM_MODE_NOT_SUPPORTED = 0,
    DD_ALPM_MODE_AUXWAKE,
    DD_ALPM_MODE_AUXLESS,
    DD_ALPM_MODE_MAX,
} DD_ALPM_MODE;

typedef struct _DD_SURFACE_TRANSFORM
{
    DD_IN DD_ROTATION PlaneOrientation;
    union
    {
        struct
        {
            DD_IN DDU32 VerticalFlip : 1;
            DD_IN DDU32 HorizontalFlip : 1;
            DD_IN DDU32 Scale : 1;
            DD_IN DDU32 HorzStretch : 1; // TRUE = STRETCH, FALSE = SHRINK, Valid only when 'Scale' = TRUE;
            DD_IN DDU32 VertStretch : 1;
            DD_IN DDU32 HighQualityStretch : 1;
            DD_IN DDU32 Positioning : 1; // In case of IS Scaling, Positioning will be used to adjust the position of the Surface and cursor.
            DD_IN DDU32 UNIQUENAME(Reserved) : 25;
        };
        DD_IN DDU32 FlagsValue;
    };
    DD_IN NN_SCALING_STATE NNScalingState;
    DD_IN DDU32 HorzScaleFactorMultBy1000;
    DD_IN DDU32 VertScaleFactorMultBy1000;
    DD_IN DDU32 ScreenSurfSizeX;
    DD_IN DDU32 ScreenSurfSizeY;
} DD_SURFACE_TRANSFORM;

typedef struct _DD_SEL_FETCH_PARAMS // Selective Fetch Parameters
{
    DD_IN DD_SURFACE_SCANOUT_DETAILS SelFetchScanoutDetails;
    DD_IN DDU32 ScreenPosX;
    DD_IN DDU32 ScreenPosY;
} DD_SEL_FETCH_PARAMS;

typedef struct _DD_SU_REGION // Selective Update Parameters for PSR2 SW Manual Tracking
{
    DD_IN DDU32 SUStartOffset;
    DD_IN DDU32 SUEndOffset;
} DD_SU_REGION;

typedef enum _DD_KEYING
{
    KEY_DISABLE = 0x0, // Disable keying for this plane.
    SOURCE_KEY_ENABLE = 0x1, // This plane's pixels will be checked for a key match. The blend between this plane and the plane below will treat the key matched pixels as tr
    // ansparent.
    DESTINATION_KEY_ENABLE = 0x2, // This plane's pixels will be checked for a key match. The blend between this plane and the plane above will treat the pixels above as opa
    // que only where this plane is key matched and the plane above is opaque. When plane gamma is enabled, the gamma processing may shift the
    // pixel color values sent to blender and may cause it to not match the key color as desired. The recommendation is to use the pipe gamma
    // when destination keying is enabled.
    SOURCE_KEY_WINDOW_ENABLE = 0x3, // This plane's pixels will be checked for a key match. The blend between this plane and the plane below will treat the key matched pixel
    // s as transparent only where the plane below is opaque.

} DD_KEYING;

/***************************************************************
 * @brief  This Enum is used for diff allowed value for DpMstDscDisable reg key.
 ***************************************************************/
typedef enum _DP_DSC_LT_REG_VAL
{
    DP_MST_ALWAYS_DISABLE_DSC = 1,   // Disable DSC Support always when "DPMstDscDisable" Reg Key is set to 1.
    DP_MST_DSC_DISABLE_BASED_ON_SINK_PREFERENCE = 2,   // Disable DSC Support based on the Sink Preference (i.e. DPCD 0x30C), when "DPMstDscDisable" Reg Key is set to 2.
    DP_MST_DSC_DISABLE_BASED_ON_BRANCH_ID = 4,   // Disable DSC Support based on the OUI_ID (i.e. DPCD 0x500 - 0x509) of the Branch Device, when "DPMstDscDisable" Reg Key is set to 4.
    DP2_1_SST_DSC_DISABLE = 256, // Disable DSC Support for SST always when "DPMstDscDisable" Reg Key is set to 256(bit 8).
    DP2_1_LINK_RATE_OPTIMIZATION_DISABLE = 512, // Disable Linkrate optimization when "DPMstDscDisable" Reg Key is set to 512(bit 9).
    DP2_1_LINK_RATE_OPTIMIZATION_AND_DSC_FOR_SST = 768, // Disable Link Rate Optimization and Dsc for SST
} DP_DSC_LT_REG_VAL;

typedef struct _DD_CLEAR_COLOR_VAL
{
    DD_BOOL BaseClearColorEnable; // Indication whether Clear Colour resolve need to enabled/disable in HW for the Render Compressed surface.
    DDU64 BaseClearColorValue;    // Clear colour value used by display HW to resolve the Render Compressed surface. Value can be 0.
} DD_CLEAR_COLOR_VAL;

typedef struct _DD_COMPRESSION_ONLY_DETAILS
{
    DD_SURF_COMPRESSION_TYPE CompType;
    DDU32 BaseCompMetaDataDistance; // Control surface distance from RGB main surface in case of Render Compression or Control surface distance from RGB or Y main surface in case of Media Compression
    DDU32 BaseCompMetaDataStride;   // stride of the control surface for RGB main surface in case of Render Compression
    DDU32 UVCompMetaDataDistance;   // Control surface distance from UV main surface in case of Media Compression
    DDU32 UVCompMetaDataStride;     // stride of the control surface from UV main surface in case of Media Compression, but s/w doesn't program this value into h/w
} DD_COMPRESSION_ONLY_DETAILS;

typedef struct _DD_PLANE_SURF
{
    DD_IN DD_BOOL AlphaBlend;
    DD_IN DD_SURFACE_ATTRIBUTES Attrib;
    DD_IN DD_SURFACE_SCANOUT_DETAILS ScanOutDetails;
    DD_IN DD_SURFACE_TRANSFORM Transform;
    DD_IN DDU32 ScreenPositionX;
    DD_IN DDU32 ScreenPositionY;
    DD_IN DD_COMPRESSION_ONLY_DETAILS CompressionDetails;
} DD_PLANE_SURF;

typedef struct _DD_FLIP_QUEUE_PARAMS
{
    union
    {
        DD_IN DDU8 InFlags;
        struct
        {
            DD_IN DDU8 IsQueuedFlip : 1;                    // Whether the flip is a queued flip or not.
            DD_IN DDU8 IsInterruptNotificationRequired : 1; // Whether DMC Interrupt needs to be enabled to Notify completion to OS.
            DD_IN DDU8 UNIQUENAME(Reserved) : 6;
        };
    };
    DD_IN DDU32 PresentationDelay; // The time delay in us for flip on this plane.
} DD_FLIP_QUEUE_PARAMS;

typedef enum _DD_HW_FLIPQ_MODE
{
    DD_HW_FLIPQ_MODE_UNKNOWN = 0x0,
    DD_HW_FLIPQ_MODE_SYNC,
    DD_HW_FLIPQ_MODE_ASYNC,
    DD_HW_FLIPQ_MODE_DISABLE,
    DD_HW_FLIPQ_MODE_MAX
} DD_HW_FLIPQ_MODE;

typedef struct _DD_PROGRAM_FLIPQ_ARGS
{
    PIPE_ID PipeId;
    PIPE_ID MasterPipe; // This will be used to program flipQ pts synchronization
    DD_HW_FLIPQ_MODE Mode;
    DD_BOOL SkipSwQueueReset;
} DD_PROGRAM_HW_FLIPQ_ARGS;

typedef enum _DD_FLIPQ_TYPE
{
    DD_FLIPQ_TYPE_UNKNOWN = 0,
    DD_FLIPQ_TYPE_SW,           // Does not support DMC based queue
    DD_FLIPQ_TYPE_HW_SIMPLE_FQ, // DMC based queue that supports only 2 Offset/Data pairs
    DD_FLIPQ_TYPE_NOT_SUPPORTED,
} DD_FLIPQ_TYPE;
#define DD_IS_FLIPQ_SUPPORTED(FlipQInfo) (((FlipQInfo > DD_FLIPQ_TYPE_UNKNOWN) && (FlipQInfo < DD_FLIPQ_TYPE_NOT_SUPPORTED)) ? TRUE : FALSE)

typedef struct _DD_CHECK_MPO_PLANE_PARAMS
{
    DD_IN DDU32 HwPlaneId;
    DD_IN DD_PLANE_SURF PlaneSurf; // To be filled ONLY when programming all parameters
    DD_IN DD_CLEAR_COLOR_VAL ClearColorVal;
    DD_IN DD_BOOL AsyncFlip;
    DD_IN DDU32 LayerIndex;
} DD_CHECK_MPO_PLANE_PARAMS;

typedef struct _DD_FBC_CFB_PARAMS
{
    DD_LARGE_INTEGER FbcDsmStartAddress;
    DDU64 FbcDsmSize;
    DD_BOOL FbcBufferAllocated[MAX_FBC_PIPES];
    DDU32 CompFrameBufAddr[MAX_FBC_PIPES];
    DDU32 CompFrameBufSize[MAX_FBC_PIPES];
    DDU32 CompressionRatio[MAX_FBC_PIPES];
    DDU32 CompressionRatioInRegistry; // Compression ratio value in registry
    DDU32 DisableCompressionTypeInRegistry;
    DDU32 CBStrideInCacheLines[MAX_FBC_PIPES];
} DD_FBC_CFB_PARAMS;

typedef struct _DD_GET_PIPE_DETAILS_ARGS
{
    DD_IN PIPE_ID PipeId;
    DD_OUT DDU32 SourceSizeX;
    DD_OUT DDU32 SourceSizeY;
    DD_OUT DD_BOOL DownScalingEnabled;
    DD_OUT DDU32 MaxDownScaledSrcSizeX;
    DD_OUT DDU32 MaxDownScaledSrcSizeY;
    DD_OUT DDU64 DotClockInHz;
    DD_OUT DDU32 HTotal;
    DD_OUT DDU32 HActive;
    DD_OUT DDU32 VActive;
    DD_OUT DDU32 VBlankStart;
    DD_OUT DD_BOOL IsInterlaced;
} DD_GET_PIPE_DETAILS_ARGS;

// Creating the below structure in DisplayARgsInternal.h file as it will be used across CCD layer and HAL Layer in future when Locking control moves to CCD
typedef union _DD_PIPE_BITMASK_FOR_DB_LOCK
{
    DDU32 Value;
    struct
    {
        DDU32 PipeA : 1; // Bit 0
        DDU32 PipeB : 1; // Bit 1
        DDU32 PipeC : 1; // Bit 2
        DDU32 PipeD : 1; // Bit 3
        DDU32 Reserved : 28;
    };

} DD_PIPE_BITMASK_FOR_DB_LOCK;
typedef union _DD_FLIP_OUT_FLAGS
{
    DDU8 Value;
    struct
    {
        DDU8 SubmittedToSwQueue : 1; // Submitted to Sw Flip Queue.
        DDU8 SubmittedToHwQueue : 1; // Submitted to Hw Flip Queue.
        DDU8 SubmittedAsCpuMmio : 1; // Programmed right away as CPU MMIO Flip.
        DDU8 SubmittedToDmcHrr : 1;  // Programmed to DMC HRR surface offsets - only D12.
        DDU8 SubmittedOnCpuDsb : 1;  // Programmed right away using CPU through DSB buffer.
        DDU8 UNIQUENAME(Reserved) : 3;
    };
} DD_FLIP_OUT_FLAGS;
C_ASSERT(sizeof(DD_FLIP_OUT_FLAGS) == sizeof(DDU8));

typedef struct _DD_PLANE_PARAMS
{
    DD_IN DDU8 HwPlaneId;                    // REQUIRED FOR    ALL     FLIP TYPES
    DD_IN DDU32 HwStartAddressBase;          // REQUIRED FOR   ALL    FLIP TYPES
    DD_IN DDU32 HwStartAddressUv;            // REQUIRED FOR   ALL    FLIP TYPES
    DD_IN DDU32 HwStartAddressS3DLeftSurf;   // FLIP_TYPE_SYNC_ALL_PARAM ONLY
    DD_IN DDU32 HwStartAddressS3DLeftSurfUv; // FLIP_TYPE_SYNC_ALL_PARAM ONLY
    DD_IN DDU64 PresentId;                   // Valid for all flip types
    DD_IN DDU32 MaxScanLineForAsync;         //     FLIP_TYPE_CONVERT_SYNC_TO_ASYNC  ONLY
    DD_OUT DD_BOOL* pConvertedToAsync;       //    FLIP_TYPE_CONVERT_SYNC_TO_ASYNC  ONLY
    DD_IN DD_BOOL Enable;                    // FLIP_TYPE_SYNC_ALL_PARAM ONLY
    DD_PLANE_SURF PlaneSurf;                 // FLIP_TYPE_SYNC_ALL_PARAM ONLY
    DD_IN DD_CLEAR_COLOR_VAL ClearColorVal;
    DD_IN DD_SMOOTH_SYNC_PLANE_TYPE SmoothSyncPlaneType; // To identify if this is acting as front plane in smooth sync combo.
    DD_IN_OUT DD_FLIP_QUEUE_PARAMS FlipQDetails;         // Filled for queued flip, applicable only for FLIP_TYPE_SYNC_ADDRESS_ONLY
    // Also only Feasible for HwPlaneId == 0
    DD_IN DD_KEYING Keying; // Keying when Legacy Overlay plane is active
    DD_IN DD_BOOL AssignMinDbuff;
    DD_IN DD_BOOL IsAsyncFlip;
    DD_IN DD_BOOL IsFbrEnabled;
    DD_IN DD_BOOL ProgramFlipType; // If TRUE, HAL needs to reprogram the plane control with the Type of Flip (Sync vs Async).
    DD_OUT DD_FLIP_OUT_FLAGS* pOutFlags;
    DD_IN DD_BOOL IsClearColorUpdateRequired; // Starting D14, MTL can support Plane_CC updates regardless of the Flip type.
    DD_IN DD_BOOL ExtendedLeftSide;
    DD_IN DD_BOOL ExtendedRightSide;
} DD_PLANE_PARAMS;

#define DD_MIN_SS_DITHER_STEP_SIZE 1
#define DD_MAX_SS_DITHER_STEP_SIZE 4
#define DD_DEFAULT_SS_DITHER_STEP_SIZE 2

typedef struct _DD_SMOOTH_SYNC_FEATURE_SUPPORT
{
    DD_BOOL EnableSmoothSync;
    DDU8 SmoothSyncDitherStepSize;
} DD_SMOOTH_SYNC_FEATURE_SUPPORT;

typedef enum _DD_VRR_STATUS
{
    DD_VRR_STATUS_UNKNOWN = 0,
    DD_VRR_STATUS_ENABLE_IN_PROGRESS,
    DD_VRR_STATUS_ENABLE,
    DD_VRR_STATUS_DISABLE_IN_PROGRESS,
    DD_VRR_STATUS_DISABLE
} DD_VRR_STATUS;

typedef enum _DD_FMS_STATUS_REASONS
{
    DD_FMS_SUCCESS = 0,
    DD_FMS_NOT_MODIFIED_TIMING,
    DD_FMS_COLLAGE_DISPLAY,
    DD_FMS_TARGET_CHANGED,
    DD_FMS_NOT_PHYSICAL_PIPE,
    DD_FMS_PIPE_CHANGE,
    DD_FMS_COLORFORMAT_ENCODING_CHANGED,
    DD_FMS_COLORFORMAT_GAMUT_CHANGED,
    DD_FMS_COLORFORMAT_CONTENTTYPE_CHANGED,
    DD_FMS_COLORFORMAT_BPC_CHANGED,
    DD_FMS_COLORFORMAT_MODEL_CHANGED,
    DD_FMS_UNSUPPORTED_DISPLAY,
    DD_FMS_LINK_NOT_TRAINIED,
    DD_FMS_TIMING_CHANGE_REQUIRED,
    DD_FMS_CDCLK_CHANGED,
    DD_FMS_FEC_DSC_PARAMS_CHANGED,
    DD_FMS_EDP_PANELREPLAY_NOT_CONFIGURED,
    DD_FMS_EDP_PSR_NOT_CONFIGURED,
    DD_FMS_CUSTOM_VSWING_ENABLED_IN_VBT
} DD_FMS_STATUS_REASONS;

typedef struct _DD_FPS_TRACKING_PARAMS
{
    DD_BOOL VrrFpsTrackingEnabled;    // 1: indicates Vrr is enabled on previous flip
    DDU64 PrevVrrFlipRequestTimeInUs; // Prev flip request time (Tn-1)
    DDU64 PredictedFrameTimeInUs;     // Predicted current avg flip request time
    DDU64 LastFrameTimeInUs;          // Last Frame-time (Tn-1 - Tn-2) [Frame-time is time between current and previous flip (Tn - Tn-1)]
    DDU64 SecondLastFrameTimeInUs;    // Second Last Frame-time (Tn-2 - Tn-3)
    DDU64 AveragePredictionErrorInUs; // Error between predicted frame-time and actual frame-time
} DD_FPS_TRACKING_PARAMS;

/**
 * @brief Struct for Adaptive Balance User Configurable Parameters.
 *
 *        The parameters are provided by user through UI / SDK in the units specified in the comments. They are then converted to number of scanlines (except for percentage) before storing in below
 * structure for direct usage in Adaptive-Balance algorithm.
 */
typedef struct _CCD_ADAP_BAL_USER_CONFIGURABLE_PARAMS
{
    // This is the minimum frame time for the driver to use in microseconds
    // While a low number can be specified, the driver cannot use a value below the monitor's minimum
    // Range: 0 to AdapBalUserVMax.
    // Default : Monitors specified vmin
    DDU32 AdapBalUserVMin;

    // This is the maximum time the display can handle between active frames in microseconds.
    // The user is allowed to overclock the monitor and exceed the maximum time it specifies.
    // However, warnings should be provided that this is outside displays specifications and
    // is likely to cause screen flicker.
    // Range: AdapBalUserVMin to unlimited.
    // Default: Monitor's specified vmax
    DDU32 AdapBalUserVMax;

    // This is the maximum number of milliseconds frametime increase that the specific monitor being driven can cope with before
    // flickering. It may be monitor specific and should be defaulted to appropriate Display ID 2.1 field.
    // Set to vmax if no constraint required
    // Range: 1 to (AdapBalUserVMax - AdapBalUserVMin)
    // Default: Display ID 2.1 is present or (AdapBalUserVMax - AdapBalUserVMin) if no constraint.
    DDU32 AdapBalUserSfdit;

    // This is the maximum number of milliseconds frametime decrease that the specific monitor being driven can cope with before
    // flickering. It may be monitor specific and should be defaulted to appropriate Display ID 2.1 field.
    // Set to vmax if no constraint required
    // Range: 1 to (AdapBalUserVMax - AdapBalUserVMin)
    // Default: Display ID 2.1 is present or (AdapBalUserVMax - AdapBalUserVMin) if no constraint.
    DDU32 AdapBalUserSfddt;

    // Target position for an active frame. This is the percentage of the time between vmin and vmax.
    // This is used as a guide by the driver to choose how many active frames to use for every game frame.
    // A lower number will result in more active frames being inserted than a higher number.
    // More active frames means less monitor flicker but also adds a risk of extra latency, a reduction in FPS
    // and in increase in power.
    // Range from 10 to 90.
    // Default 50%.
    DDU32 AdapBalUserBlankPercent;

    // Sensitivity of the flicker detection algorithm. A lower number will make the driver try to correct for
    // flicker more regularly. A lower number also adds a risk of extra latency, a reduction in FPS and an increase in power.
    // Range: 10 to 1000
    // Default: 50%
    DDU32 AdapBalUserCorrectionSensitivityPercent;

    // Aggressiveness of error correction. This controls how aggressively the driver tries to correct any flicker that may be detected.
    // The CorrectionAggressiveness/1000 is the number of milliseconds to adjust by when the balance is at twice the guardband.
    // Range: 1000 to 10000
    // Default: 4000 (4ms worth of scanlines will be applied)
    DDU32 AdapBalUserCorrectionAggressivenessInUs;
} CCD_ADAP_BAL_USER_CONFIGURABLE_PARAMS;

typedef struct _DD_VRR_ADAPTIVE_BAL_PARAMS
{
    CCD_ADAP_BAL_USER_CONFIGURABLE_PARAMS UserConfigurableParams; // Parameters that can be configured by user through IGCC / SDK
    DDU32 VMaxAdjust;                                             // Blanking period to allow after the predicted time before starting a new active on the final frame
    DD_BOOL UpdateAdaptiveBalParamsPostFlip;                      // Indicates that Adaptive Balance parameters need to be recomputed post-flip to account for the new predicted frame-time
    DDS32 CurrentDcBalanceInScanlines;                            // Stores current DC Balance value in terms of scanlines
    DDS32 DcBalanceForPreviousFlip;                               // Stores DC Balance value (in terms of scanlines) on previous flip, it is used for HW Counter verification
    DDS8 Direction;                                               // Direction of charge, changes between 1 and -1 every frame
    DDU32 NumAdaptiveBalFrames;                                   // Number of frames in which a given prediction has been divided for applying adap-bal modified Vmin / Vmax
    DDU32 CurrentAdaptiveBalFrameCount;                           // Frame-count for which the modified Vmin / Vmax is being applied, it is reset on Adaptive Balance parameters re-computation
    DDU32 EarlierFlipLine;                                        // Output of Adap-Bal algo, Vmin for the first frame post flip
    DDU32 EarlierVTotal;                                          // Output of Adap-Bal algo, Vmax for the first (N-1) frames post flip
    DDU32 FinalFlipLine;
    DDU32 FinalVTotal;                                // Output of Adap-Bal algo, Vmax for the last adap-bal frame post flip
    DD_MONITOR_FLICKER_PROFILE_CONFIG FlickerProfile; // Active flicker profile config for the path
} DD_VRR_ADAPTIVE_BAL_PARAMS;

typedef struct _DD_VRR_UPDATE_PARAMS
{
    DDU32 FlipLineCountValue;
    DDU32 VrrVmax;
} DD_VRR_UPDATE_PARAMS;

// VRR Software Control Data
typedef struct _DD_VRR_SWC_DATA
{
    DDU32 VMinInUs;                           // Vmin Time
    DDU32 TimeForOneScanLineInNanoSec;        // Time of 1 scanline in nano seconds
    DD_FPS_TRACKING_PARAMS FpsTrackingParams; // FPS tracking parameters
    DD_VRR_ADAPTIVE_BAL_PARAMS VrrAdapBalParams;
    DD_VRR_UPDATE_PARAMS VrrParams;
} DD_VRR_SWC_DATA;

typedef struct _DD_ASYNC_LAYER_COUNT
{
    DDU32 NumAffectedOsAsyncLayers;     // Number of affected OS Async layers [FlipImmediate, FlipImmediateNoTearing]
    DDU32 NumActiveOsAsyncLayers;       // Number of active OS Async layers [FlipImmediate, FlipImmediateNoTearing]
    DDU32 AffectedOsAsyncLayerId;       // Affected Layer Id with Os Async flips
    DDU32 NumAffectedDriverAsyncLayers; // Number of affected Driver Async layers [FlipImmediate ]
} DD_ASYNC_LAYER_COUNT;

#define DD_IS_VRR_IN_TRANSITION_PHASE(VrrStatus) ((VrrStatus == DD_VRR_STATUS_DISABLE_IN_PROGRESS) || (VrrStatus == DD_VRR_STATUS_ENABLE_IN_PROGRESS))
typedef enum _FLIP_TYPE
{
    FLIP_TYPE_ASYNC = 0,
    FLIP_TYPE_CONVERT_SYNC_TO_ASYNC,
    FLIP_TYPE_SYNC_ADDRESS_ONLY,
    FLIP_TYPE_SYNC_ALL_PARAM,
    FLIP_TYPE_MAX
} FLIP_TYPE;

typedef struct _SAFE_SCANLINE_WAIT_ARGS
{
    DD_BOOL Required;
    DDU32 SafeScanLineCount; // Denotes the Scanlines driver waits before VBI to avoid disarming of the plane
    DDU32 VBlankStart;       // Indicates the Vblank start to use for waits
} SAFE_SCANLINE_WAIT_ARGS;

typedef struct _DD_CHECK_MPO_PLANES_ARGS
{
    DD_IN DDU32 NumPlanes;
    DD_IN DD_CHECK_MPO_PLANE_PARAMS PlaneParams[MAX_PLANES_PER_PIPE];
} DD_CHECK_MPO_PLANES_ARGS;

typedef struct _DD_GET_MPO_HW_CAPS_ARG
{
    DD_OUT DDU32 MaxRgbPlanes; // Same as MaxPlanes
    DD_OUT DDU32 MaxYuvPlanes;
    DD_OUT DDU32 NumScalars;
    DD_OUT DDU32 MaxFlipQueues;
    DD_OUT DDU32 MaxFlipQueueDepth;
    DD_OUT DDU32 SupportedPixelFormatMask;
    DD_OUT DD_MPO_CAPS OverlayCaps;
    DD_OUT DDU32 MaxStretchFactorMultBy100;
    DD_OUT DDU32 MaxShrinkFactorMultBy100;
    DD_OUT DDU32 AsyncFlipSupportedPixelFormatMask;
    DD_OUT DDU32 AlphaBlendSupportedPixelFormatMask;
    DD_OUT DDU32 PlaneScalingSupportedPixelFormatMask;
    DD_OUT DDU32 RenderCompressionSupportedPixelFormatMask;
    DD_OUT DDU32 MediaCompressionSupportedPixelFormatMask;
    DD_OUT DDU32 HorizontalFlipSupportedPixelFormatmask;
    DD_OUT DDU32 SurfMemoryTypeOrientationSupport[DD_TILE_MAX][DD_ORIENTATION_MAX];
    DD_OUT DD_BOOL HorizontalFlipSupport[DD_TILE_MAX];
    DD_OUT DD_BOOL RenderCompressionSupport[DD_TILE_MAX][DD_ORIENTATION_MAX];
    DD_OUT DD_BOOL MediaCompressionSupport[DD_TILE_MAX][DD_ORIENTATION_MAX];
    DD_OUT DDU32 MaxPlaneOffset;
    DD_OUT DD_BOOL NV12OddSrcSizeSupport; // Support for YUV420/422 odd plane size and pan size
} DD_GET_MPO_HW_CAPS_ARG;

typedef struct _DD_FLIP_TILE_LOCATION
{
    DDU32 TileHLocation;
    DDU32 TileVLocation;
} DD_FLIP_TILE_LOCATION;

typedef struct _DD_PIPE_SPECIFIC_PLANE_LOCK_ARGS
{
    DD_IN PIPE_ID PipeId;  // Filled by DVM
    DD_IN DDU32 NumPlanes; // Number of planes to lock/unlock (in case of 'LockAllPlanes = TRUE', it is used to select Primary Flip plane in VRR enabled mode)
    DD_IN DDU8 HwPlaneId[MAX_PLANES_PER_PIPE];
    DD_IN DD_BOOL MaskOrAllowDbStallManTrk; // This determines whether SU Update Lock is required for this Pipe.
    // This is Pipe Specific since SU can be used only in subset of pipes used in Multipipe config.
} DD_PIPE_SPECIFIC_PLANE_LOCK_ARGS;

typedef struct _DD_LOCK_PLANE_UPDATES_ARG
{
    DD_IN DDU32 NumPipes;        // Filled by DVM
    DD_IN DD_BOOL Lock;          // TRUE = Lock, FALSE = Unlock
    DD_IN DD_BOOL LockAllPlanes; // TRUE = All planes on given pipe are locked/unlocked. In case of Tiled Display, it refers to all pipe planes belonging to the tile
    DD_IN DD_BOOL LockScalers;   // This determines whether Scaler Locks are needed
    DD_IN DD_PIPE_SPECIFIC_PLANE_LOCK_ARGS PipeSpecificPlaneLockArgs[MAX_PHYSICAL_PIPES];
    DD_IN SAFE_SCANLINE_WAIT_ARGS SafeScanLineWaitArgs;
} DD_LOCK_PLANE_UPDATES_ARG;

typedef struct _DD_FLIP_TILE_BEZELDISPLACEMENT
{
    DDU32 XBezelDisplacement;
    DDU32 YBezelDisplacement;
} DD_FLIP_TILE_BEZELDISPLACEMENT;

typedef union _DD_FLIP_QUEUE_SCANLINE_RANGE_STATE
{
    struct
    {
        DDU32 EnableSwFlipQueueScanlineRange : DD_BITFIELD_BIT(0);
        DDU32 Reserved : DD_BITFIELD_RANGE(1, 31);
    };
    DDU32 Value;
} DD_FLIP_QUEUE_SCANLINE_RANGE_STATE;

typedef struct _DD_FBC_STATUS_DATA
{
    DD_BOOL FbcEnabled;          // new FBC state at the end of this flip
    DD_BOOL FbcUpdatePending;    // Indicates if SetFbcState() should be done from CCD/OSL after flip
    DD_BOOL FbcUpdateOnVbi;      // Indicates if SetFbcState() should happen on Vblank interrupt
    DD_BOOL FbcNukeOnVbiPending; // Indicates if nuking is required post Fbc enable
    DD_BOOL FbcEnablePossibleForCurrConfig;
    DDU8 BindingPlaneIndex;
} DD_FBC_STATUS_DATA;

typedef struct _DD_SF_PLANE_ARGS
{
    DD_IN DD_SEL_FETCH_PARAMS SelFetchDetails;
    DD_IN DDU8 HwPlaneId;
} DD_SF_PLANE_ARGS;

typedef struct _DD_DPST_SF_ARGS
{
    DD_BOOL IsHwSfsuDpstEnabled;
    DDU32 SegmentStart;
    DDU32 SegmentEnd;
} DD_DPST_SF_ARGS;

typedef struct _DD_SFSU_PARAMS
{
    DD_IN DD_BOOL SFPossible; // Used by HAL to check whether Selective Fetch Plane Instance programming is required
    DD_IN DD_BOOL FbcSfPossible;
    DD_IN DD_BOOL IsSuCursorOverlap; // Used by HAL to enable WA for ET
    DD_IN DDU32 PipeSrcSizeY;        // User by HAL for LNL WA for ET
    DD_IN DD_SU_REGION SuRegion;     // Man Track Updates
    DD_IN DDU32 NumSfPlanes;
    DD_IN DD_SF_PLANE_ARGS SfPlaneArgs[MAX_PLANES_PER_PIPE];
    DD_IN DD_BOOL EnableFbc; // Used by HAL to check whether Fbc needs to enabled or disabled based on dirty rectangle area.
    DD_IN DD_DPST_SF_ARGS DpstArgs;
    DD_IN DSC_CFG* pDscCfg;
} DD_SFSU_PARAMS;

typedef union _DD_OPTIONAL_FLIP_PROGRAMMING_FLAGS
{
    struct
    {
        DDU32 SurfAddressProgrammingRequired : DD_BITFIELD_BIT(0);
        DDU32 SfSuProgrammingRequired : DD_BITFIELD_BIT(1);
        DDU32 EarlyTptProgrammingRequired : DD_BITFIELD_BIT(2);
        DDU32 NeedSafeLineCheckBeforeW2 : DD_BITFIELD_BIT(3);
        DDU32 FbcSfProgrammingRequired : DD_BITFIELD_BIT(4);
        DDU32 FbcProgrammingRequired : DD_BITFIELD_BIT(5);
        DDU32 Reserved : DD_BITFIELD_RANGE(6, 31);
    };
    DDU32 Value;
} DD_OPTIONAL_FLIP_PROGRAMMING_FLAGS;
C_ASSERT(sizeof(DD_OPTIONAL_FLIP_PROGRAMMING_FLAGS) == sizeof(DDU32));

typedef struct _DD_PROGRAM_PLANES_ARGS
{
    DD_IN FLIP_TYPE FlipType;
    DD_IN DD_OPTIONAL_FLIP_PROGRAMMING_FLAGS OptionalFlipProgrammingFlags;
    DD_IN DDU32 ChildId;   // Filled by CCD and used by DVM and Target
    DD_IN DDU32 PipeIndex; // Filled by DVM
    DD_IN DDU32 NumPlanes;
    DD_IN DD_COLOR_BLENDING_MODE PipeBlendingMode;
    DD_IN_OUT DD_PLANE_PARAMS PlaneParams[MAX_PLANES_PER_PIPE];
    DD_IN DDU32 CompFrameBufAddr[MAX_FBC_PIPES];
    DD_IN DDU32 CompressionRatio[MAX_FBC_PIPES];
    DD_IN DDU32 CBStrideInCacheLines[MAX_FBC_PIPES];
    DD_OUT DD_FBC_STATUS_DATA FbcStatusData;
    DD_IN DD_SFSU_PARAMS SfsuParams;
    DD_IN DDS32 XPos;      // Needed for Early Transport programming of cursor
    DD_IN DDS32 YPos;      // Needed for Early Transport programming of cursor
    DD_IN DDU32 CurHeight; // Needed for Early Transport programming of cursor
    DD_OUT DD_FLIP_QUEUE_SCANLINE_RANGE_STATE FlipQScanlineState;
    DD_IN DD_BOOL HrrEnabledStatus;
    DD_IN DD_BOOL S3DEnabledStatus;
    DD_IN SAFE_SCANLINE_WAIT_ARGS SafeScanlineWaitArgs;
    DD_IN DD_BOOL IsDriverInternalFlip;
    DD_IN DDU32 Window2InVBlank; // VBlankStart - VActive in scanlines
    DD_IN DD_BOOL SeamExcessProgrammingNeeded;
#if (_DEBUG || _RELEASE_INTERNAL)
    // Submits the flip after Vblank during simulation. Restrcited only for simulation use
    DD_BOOL WaitForVblank;
    DDU32 DisableCompressionTypeInRegistry;
#endif // (_DEBUG || _RELEASE_INTERNAL)
} DD_PROGRAM_PLANES_ARGS;

typedef struct _DD_LINEAR_FRAME_BUFFER_ARGS
{
    PIPE_ID PipeId;
    DDU32 SurfSizeX;
    DDU32 SurfSizeY;
    DDU32 SurfPositionX;
    DDU32 SurfPositionY;
    DDU32 SurfStride; // If zero, skip enabling the plane after disabling all planes on given PipeId.
    DDU32 HwStartAddressBase;
    DD_ROTATION Orientation;
} DD_LINEAR_FRAME_BUFFER_ARGS;

// Cursor args structure
typedef struct _DD_CURSOR_ARGS
{
    DD_BOOL Enable;
    DDU32 ChildId;
    PIPE_ID Pipe;
    IN DDS32 XPos;
    IN DDS32 YPos;
    IN DDU32 BaseAddress;            // Needed to trigger the update.
    IN DDU32 CursorWidth;            // Current cursor width
    IN DDU32 CursorHeight;           // Current cursor height
    IN DD_POINTER_FLAGS CursorFlags; // use these flags to avoid b-spec violations
    IN DD_BOOL ShapeChanged;
    IN DDU32 CursorLuminance;
    IN DD_BOOL UseFlipQ; // used by HAL to determine if cursor update should be done via FlipQ
    IN DD_SFSU_PARAMS SfSuParams;
    DD_OPTIONAL_FLIP_PROGRAMMING_FLAGS OptionalCursorProgrammingFlags;
} DD_CURSOR_ARGS;

typedef struct _HRR_ENABLE_ARGS
{
    DD_BOOL Enable;
    DDU32 PipeId;
} HRR_ENABLE_ARGS;

/***************************************************************
 * @brief       This structure contains bitfields for diffrent
 *              types of RR event.
 ***************************************************************/
typedef struct _DD_PLANE_DBUF_ARGS
{
    DD_IN DDU8 HwPlaneId;
    DD_IN DDU16 StartBlock;
    DD_IN DDU16 EndBlock;
} DD_PLANE_DBUF_ARGS;

typedef struct _DD_PIPE_DBUF_ARGS
{
    DD_IN DDU32 PipeIndex;
    DD_IN DDU32 NumOfEnabledPlanes;
    DD_IN DD_PLANE_DBUF_ARGS CursorDbuf;
    DD_IN DD_PLANE_DBUF_ARGS PlaneDBuf[MAX_PLANES_PER_PIPE];
    DD_IN DD_BOOL UpdateHW;          // For D15+ Plane DBuf - update MMIOs too or just the cache
    DD_IN DD_BOOL LinearFrameBuffer; // Is post display path.
} DD_PIPE_DBUF_ARGS;

typedef struct _DBUF_RE_ALLOC_WAIT_ORDER
{
    PIPE_ID PipeIndex;
    DD_BOOL NeedWaitForVbi;
} DBUF_RE_ALLOC_WAIT_ORDER;

typedef struct _DD_GET_CURRENT_SCAN_LINE_ARGS
{
    DD_IN DDU32 PipeIndex;
    DD_OUT DDU32 CurrScanLine;
    DD_OUT DD_BOOL InVerticalBlank;
} DD_GET_CURRENT_SCAN_LINE_ARGS;

// Set scan line compare Args Structure
typedef struct _DD_SET_SCAN_LINE_COMPARE_ARGS
{
    DD_IN DDU32 PipeIndex;                   // Pipe index
    DD_IN DD_BOOL InitiateCompare;           // initiate a scan line compare
    DD_IN DD_BOOL CompareInclusive;          // Trigger inside compare window, or outside window
    DD_IN DD_BOOL CounterSelectPrimaryPlane; // Set counter trigger Primary plane vs timing generator [ Plane vs Pipe scan line counter]
    DD_IN DD_BOOL RenderResponceBCS;         // Set Render Response BCS vs CS
    DD_IN DDU32 StartScanLine;               // Scan line that start the window
    DD_IN DDU32 EndScanLine;                 // Scan line that ends the window
} DD_SET_SCAN_LINE_COMPARE_ARGS;

typedef struct _DD_CHECK_MPO_PIPE_DETAILS
{
    DD_IN PIPE_ID PipeId;
    DD_IN DDU32 Window2InVBlank;
    DD_IN DD_SCALING Scaling;
    DD_IN DD_CHECK_MPO_PLANES_ARGS FlipParams;
    DD_OUT DDU32 FailingHwPlaneId;
    DD_OUT DDU32 SourceSzX;
    DD_OUT DDU32 SourceSzY;
} DD_CHECK_MPO_PIPE_DETAILS;

/**
 * @brief Struct for storing Flip Tile details.
 */
typedef struct _DD_FLIP_TILE_PARAMS
{
    DDU32 NumHTiles;
    DDU32 NumVTiles;
    DDU32 TotalXBezelCorrection;
    DDU32 TotalYBezelCorrection;
    DD_FLIP_TILE_LOCATION PipeTileLocations[MAX_TILES_SUPPORTED];
    DD_FLIP_TILE_BEZELDISPLACEMENT BezelDisplacement[MAX_TILES_SUPPORTED];
} DD_FLIP_TILE_PARAMS;

typedef struct _DD_CHECK_MPO_TARGET_DETAILS
{
    DD_IN DDU32 TargetId;
    DD_IN DDU32 Window2InVBlank;
    DD_IN DD_CHECK_MPO_PLANES_ARGS FlipParams;
    DD_OUT DDU32 FailingHwPlaneId;
    DDU32 SourceSizeX;
    DDU32 SourceSizeY;
} DD_CHECK_MPO_TARGET_DETAILS;

typedef struct _DD_CHECK_MPO_DVM_ARGS
{
    DD_IN DD_CHECK_MPO_TARGET_DETAILS TargetMpoDetails[MAX_PHYSICAL_PIPES]; // MPO is supported only for Physical Pipes.
    DD_OUT DDU32 FailingTargetId;
} DD_CHECK_MPO_DVM_ARGS;

typedef struct _DD_CHECK_MPO_RM_ARGS
{
    DD_IN DD_CHECK_MPO_PIPE_DETAILS PipeDetails[MAX_PHYSICAL_PIPES];
    DD_OUT PIPE_ID FailingPipe;
} DD_CHECK_MPO_RM_ARGS;

typedef struct _DD_CHECK_ODD_SIZE_SUPPORT_ARGS
{
    DDU32 Window2InVBlank;
    DDU32 ScreenPositionY;
    DDU32 ScanSurfSizeY;
    DDU32 SourceSizeY;
    DD_BOOL IsPipeScaled;
    DD_BOOL IsPlaneScaled;
} DD_CHECK_ODD_SIZE_SUPPORT_ARGS;

#define NUM_WM_PER_PLANE 8
#define NUM_SAGV_WM_PER_PLANE 1
#define NUM_REDUCED_WM_PER_PLANE 6
#define SAGV_WM_LATENCY_INDEX NUM_REDUCED_WM_PER_PLANE
#define BYTES_PER_BLOCK 512
#define DBUF_TILE_X 8
#define DBUF_MIN_DBLOCK_SIZE 512
#define WM_MEMORY_READ_LATENCY 2
#define MIN_DBUF_FOR_WIGIG 202
#define LATENCY_INCREMENT_IPC 4

typedef struct _DD_COMPUTE_PIPE_DBUF_ARGS
{
    DD_IN DD_TIMING_INFO TimingInfo;
    DD_IN DD_PIXELFORMAT PixelFormat;
    DD_IN DD_SURFACE_MEMORY_TYPE SurfMemType;
} DD_COMPUTE_PIPE_DBUF_ARGS;

typedef struct _DD_COMPUTE_DBUF_ARGS
{
    DDU32 SourceSizeX;
    DDU32 SourceSizeY;
    DD_TIMING_INFO TimingInfo;
    // DD_ENABLE_DISABLE_PLANES_ARGS FlipParams;
} DD_COMPUTE_DBUF_ARGS;

typedef struct _DD_WM_DATA
{
    DD_OUT DD_BOOL WmEnable;
    DD_OUT DD_BOOL IgnoreLines;
    DD_OUT DDU16 WmLines;
    DD_OUT DDU16 WmBlocks;
} DD_WM_DATA;

typedef struct _DBUF_ALLOC_INFO
{
    DDU16 DBufAllocated;
    DDU16 StartBlock;
    DDU16 EndBlock;
} DBUF_ALLOC_INFO;

typedef struct _DD_PLANE_DATA_FOR_WM_DBUF
{
    DD_IN DDU8 HwPlaneId;
    DD_IN DDU32 SurfWidth;
    DD_IN DDU32 SurfHeight;
    DD_IN DD_PIXELFORMAT PixelFormat;
    DD_IN DD_SURFACE_MEMORY_TYPE SurfMemType;
    DD_IN DD_ROTATION PlaneOrientation;
    DD_IN DDU32 PlaneBytePerPixel;
    DD_IN DDU32 CustomPlaneScalingValue;
    DD_IN DDU32 DBufAllocatedOnPlane;
    DD_IN DD_BOOL AssignMinDbuff;
} DD_PLANE_DATA_FOR_WM_DBUF;

typedef enum _DD_PM_DMD_REQ_TYPE
{
    DD_PM_DMD_QGV = 0,
    DD_PM_DMD_DVFS = 1,
    DD_PM_DMD_RES_CONFIG = 2,
    DD_PM_DMD_INVALID = 3,
} DD_PM_DMD_REQ_TYPE;

typedef struct _DD_PM_DMD_REQ_ARGS
{
    DD_PM_DMD_REQ_TYPE PmDmdReqType;
    DDU32 QgvBandwidthIn100MBs; // QGV bandwidth value in multiples of 100 MB/s = MEM_SS_INFO_GLOBAL Number of populated channels * MEM_SS_INFO_QGV_POINT_<lowest unmasked GV point> Dclk *  1666.7
    DDU32 CDCLKFreqInMhz;       // DVFS
    DDU32 DDIClkFreqInMhz;      // DVFS
    DDU8 QgvGVMask;             // QGV No Punit Usage. Always set it to zero.
    DDU8 VoltageLevelIndex;     // DVFS
    DDU8 ActivePipes;           // Resource Config for Pipe, Phy, Pll, Dbuf, Scaler. To be used after Punit finalizes the usage model
    DDU8 ActivePhys;
    DDU8 ActivePlls;
    DDU8 ActiveDbufs;
    DDU8 ActiveScalars;
} DD_PM_DMD_REQ_ARGS;

typedef struct _DD_ENABLED_PLANE_DATA_ARGS
{
    PIPE_ID PipeId;
    DDU32 NumEnabledPlanes;
    DD_PLANE_DATA_FOR_WM_DBUF PlaneDataForWM[MAX_PLANES_PER_PIPE];
    DD_BOOL WaitForVBlank;
    DD_BOOL UpdateHW;          // D15+ to decide if update MMIOs directly or just update cache.
    DD_BOOL LinearFrameBuffer; // Set to True for BSOD / driver unload cases where we program Linear Frame buffer.
} DD_ENABLED_PLANE_DATA_ARGS;

typedef struct _DD_ENABLED_CURSOR_DATA_ARGS
{
    PIPE_ID PipeId;
    DD_PLANE_DATA_FOR_WM_DBUF PlaneDataForWM;
    DD_BOOL WaitForVBlank;
} DD_ENABLED_CURSOR_DATA_ARGS;

typedef struct _DD_TRANS_LP_WM_DATA
{
    DDU32 Level;
    DD_WM_DATA LPWm;
    DD_WM_DATA TransWm;
    DDU32 LineTime;
} DD_TRANS_LP_WM_DATA;

typedef struct _DD_PLANE_WM_PROGRAM_DATA
{
    DDU32 PipeIndex;
    DDU8 HwPlaneId;
    DD_WM_DATA LPWm[NUM_WM_PER_PLANE];
    DD_WM_DATA TransWm;
    DD_WM_DATA TransSagvWm;
    // For Gen 11 and higher paltforms where the Y plane WM needs to be programmed
    DD_WM_DATA LPWmY[NUM_WM_PER_PLANE];
    DD_WM_DATA TransWmY;
    DD_WM_DATA TransSagvWmY;
    DDU8 NumWmPerPlane;
} DD_WM_PROGRAM_DATA;

typedef struct _DD_COMPUTE_WM_ARGS
{
    DD_IN DDU32 Level;
    DD_IN DD_PLANE_DATA_FOR_WM_DBUF PlaneDataForWM;

    DD_IN DDU32 HTotal;
    DD_IN DDU64 DotClockInKHz;
    DD_IN DD_BOOL IsInterlaced;
    DD_IN DD_BOOL IsS3dMode;
    DD_IN DDU64 CustomPipeScalingValue;
    DD_IN DD_BOOL IsCursorPlane;
    DD_IN DD_BOOL IsWMCalcForDbuf;
    DD_IN DD_BOOL IsWmCalcForUv;
    DD_OUT DD_WM_DATA LPWm;
    DD_OUT DD_WM_DATA TransWm;
} DD_COMPUTE_WM_ARGS;

typedef struct _DD_WM_COMPUTE_INTERMEDIATE_DATA
{
    DDU32 LPLatency;
    DDU32 DBufAllocatedOnPlane;
    DDU64 AdjustedPixelRateInKHz;
    DDU32 PlaneBytePerPixel;
    DDU32 YTileMinLines;
    DDU64 Method1In100;
    DDU64 Method2In100;
    DDU32 YTileMinIn100;
    DDU32 PlaneBlocksPerLineIn100;
    // DDU32 SelectedResultBlocksIn100;
    DDU32 BytesPerLineIn100;
    DDU32 DBufBlockSize;
    DD_BOOL IsCursorPlane;
} DD_WM_COMPUTE_INTERMEDIATE_DATA;

//----------------------------------------------------------------------------
//
// MPO related data structures - END
//
//----------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
//
// OSL data structures - END
//
//----------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
//
// Protocol data structures - START
//
//----------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////

typedef enum _DD_PROTOCOL_TYPE
{
    DD_PROTOCOL_UNKNOWN = 0,
    DD_PROTOCOL_HDMI,
    DD_PROTOCOL_DVI,
    DD_PROTOCOL_DISPLAYPORT_SST,
    DD_PROTOCOL_DISPLAYPORT_EMBEDDED,
    DD_PROTOCOL_DISPLAYPORT_MST,
    DD_PROTOCOL_MIPI_DSI,
    DD_PROTOCOL_WRITEBACK, // Writeback
    DD_PROTOCOL_VIRTUAL,
    DD_PROTOCOL_DYNAMIC_PORT
    // PROTOCOL_MAX = PROTOCOL_VIRTUAL
} DD_PROTOCOL_TYPE;
#define IS_DP_PROTOCOL(proto) ((proto >= DD_PROTOCOL_DISPLAYPORT_SST) && (proto <= DD_PROTOCOL_DISPLAYPORT_MST))
#define IS_EXTERNAL_DP_PROTOCOL(proto) ((proto == DD_PROTOCOL_DISPLAYPORT_SST) || (proto == DD_PROTOCOL_DISPLAYPORT_MST))
#define IS_HDMI_PROTOCOL(proto) (proto == DD_PROTOCOL_HDMI)
#define IS_HDMI_OR_DVI_PROTOCOL(proto) ((DD_PROTOCOL_HDMI == proto) || (DD_PROTOCOL_DVI == proto))
#define IS_MIPI_DSI_PROTOCOL(proto) (proto == DD_PROTOCOL_MIPI_DSI)
#define IS_WRITEBACK_PROTOCOL(proto) (proto == DD_PROTOCOL_WRITEBACK)

typedef enum _DD_PORT_CONFIG_TYPE
{
    PORT_CONFIG_NONE = 0,
    PORT_CONFIG_DVI,
    PORT_CONFIG_HDMI,
    PORT_CONFIG_DISPLAYPORT_EXTERNAL,
    PORT_CONFIG_DISPLAYPORT_EMBEDDED,
    PORT_CONFIG_DISPLAYPORT_DUAL_MODE_HDMI,
    PORT_CONFIG_DISPLAYPORT_DUAL_MODE_DVI,
    PORT_CONFIG_MIPI_DSI,
    PORT_CONFIG_VIRTUAL,
    PORT_CONFIG_WRITEBACK,
    PORT_CONFIG_DYNAMIC,
    PORT_CONFIG_MAX
} DD_PORT_CONFIG_TYPE;

// port config related
#define IS_PORT_CFG_DP_COMPATIBLE(cfg)                                                                                                                  \
    (((cfg) == PORT_CONFIG_DISPLAYPORT_EXTERNAL) || ((cfg) == PORT_CONFIG_DISPLAYPORT_EMBEDDED) || ((cfg) == PORT_CONFIG_DISPLAYPORT_DUAL_MODE_HDMI) || \
     ((cfg) == PORT_CONFIG_DISPLAYPORT_DUAL_MODE_DVI))
#define IS_PORT_CFG_EXT_DP_COMPATIBLE(cfg) (((cfg) == PORT_CONFIG_DISPLAYPORT_EXTERNAL) || ((cfg) == PORT_CONFIG_DISPLAYPORT_DUAL_MODE_HDMI || (cfg) == PORT_CONFIG_DISPLAYPORT_DUAL_MODE_DVI))
#define IS_PORT_CFG_DVI_COMPATIBLE(cfg) (((cfg) == PORT_CONFIG_DISPLAYPORT_DUAL_MODE_DVI) || ((cfg) == PORT_CONFIG_DVI))
#define IS_PORT_CFG_HDMI_COMPATIBLE(cfg) (((cfg) == PORT_CONFIG_DISPLAYPORT_DUAL_MODE_HDMI) || ((cfg) == PORT_CONFIG_HDMI))
#define IS_PORT_CFG_VIRTUAL(cfg) ((cfg) == PORT_CONFIG_VIRTUAL)
#define IS_PORT_CFG_WRITEBACK(cfg) ((cfg) == PORT_CONFIG_WRITEBACK)
#define IS_PORT_CFG_DYNAMIC(cfg) ((cfg) == PORT_CONFIG_DYNAMIC)
#define IS_PHYSICAL_PORT_CFG(cfg) ((((DD_PORT_CONFIG_TYPE)cfg) >= PORT_CONFIG_DVI) && (((DD_PORT_CONFIG_TYPE)cfg) <= PORT_CONFIG_MIPI_DSI))
#define IS_PORT_CFG_EMBEDDED(cfg) (((cfg) == PORT_CONFIG_DISPLAYPORT_EMBEDDED) || ((cfg) == PORT_CONFIG_MIPI_DSI))

typedef struct _DD_GET_SET_VIRTUAL_LIVE_SURF_ADDRESS
{
    DD_IN PIPE_ID PipeId;
    // DD_IN DD_TARGET_DESCRIPTOR  *pTargetDescriptor;
    DD_IN DD_BOOL IsFLip;
    DD_IN_OUT DDU32 LiveAddress;
} DD_GET_SET_VIRTUAL_LIVE_SURF_ADDRESS;
typedef enum _DD_VIRTUAL_SRC_MODE_REQUEST
{
    DD_VIRTUAL_SRC_MODE_QUERY,
    DD_VIRTUAL_SRC_MODE_ADDITION,
    DD_VIRTUAL_SRC_MODE_REMOVAL,
    DD_VIRTUAL_SRC_MODE_REQUEST_MAX
} DD_VIRTUAL_SRC_MODE_REQUEST;
#define DD_MAX_VIRTUAL_SRC_MODES 20
#define DD_VIRTUAL_DISP_DEFAULT_X_RES 1920
#define DD_VIRTUAL_DISP_DEFAULT_Y_RES 1080
#define DD_VIRTUAL_DISP_MAX_X_RES 3840
#define DD_VIRTUAL_DISP_MAX_Y_RES 2160
#define DD_VIRTUAL_DISP_DEFAULT_RR 60

typedef struct _DD_GET_SET_VIRTUAL_SRC_MODES
{
    DD_IN DD_TARGET_DESCRIPTOR TargetDescriptor;
    DD_IN DD_VIRTUAL_SRC_MODE_REQUEST RequestType;
    DD_IN_OUT DDU8 NumModes;
    DD_IN_OUT DD_2DREGION SrcModes[DD_MAX_VIRTUAL_SRC_MODES];
} DD_GET_SET_VIRTUAL_SRC_MODES;

// HDCP encryption results in corruption for few specific timings
// The timing ranges which has the issue
#define HDCP_INVALID_FRONT_PORCH_START 122
#define HDCP_INVALID_FRONT_PORCH_END 130
#define HDCP_INVALID_H_BLANK_PERIOD 106

typedef enum _DD_HDCP_PORT_ACCESS
{
    DD_HDCP_PORT_ACCESS_UNKNOWN = 0,
    DD_HDCP_PORT_GET_VERSION,
    // HDCP 2.2 Specific data
    DD_HDCP_PORT_GET_RX_STATUS,
    // Generic for HDMI HDCP2 messages
    DD_HDCP_PORT_SEND_HDCP2,
    DD_HDCP_PORT_RECEIVE_HDCP2,
    // DP HDCP2 specific
    // HDCP 1.4 Specific Data Reads
    DD_HDCP_PORT_READ_BKSV,
    DD_HDCP_PORT_READ_BCAPS,
    DD_HDCP_PORT_READ_BSTATUS,
    DD_HDCP_PORT_READ_KSVLIST,
    DD_HDCP_PORT_READ_PRIMEV,
    DD_HDCP_PORT_READ_RI,
    DD_HDCP_PORT_READ_BINFO,
    // HDCP 1.4 specific Data Writes
    DD_HDCP_PORT_SEND_AN,
    DD_HDCP_PORT_SEND_AKSV,
    DD_HDCP_PORT_ACCESS_MAX
} DD_HDCP_PORT_ACCESS;

typedef struct _DD_HDCP_PORT_ACCESS_ARGS
{
    DD_PORT_TYPE Port;
    DD_HDCP_PORT_ACCESS RequestType;
    DDU8* pBuffer;
    DDU32 BufferSize;
} DD_HDCP_PORT_ACCESS_ARGS;

//----------------------------------------------------------------------------
//
// Display detection and HPD/SPI related structures - START
//
//----------------------------------------------------------------------------
typedef union _DD_PORT_CONNECTOR_TYPE
{
    DDU8 Value;
    struct
    {
        DDU8 IsTypeC : 1;
        DDU8 IsTbt : 1;
        DDU8 Reserved : 6; // Reserved
    };

} DD_PORT_CONNECTOR_TYPE;

// struct DD_DEVICE_ATTACHED_ARGS: used with IsOutputDeviceAttached
typedef struct _DD_DEVICE_ATTACHED_ARGS
{
    DD_IN DD_PORT_TYPE Port;
    DD_OUT DD_BOOL IsDisplayAttached;
} DD_DEVICE_ATTACHED_ARGS;

#define DD_DEVICE_ATTACHED_ARGS_DEFAULT \
    {                                   \
        DD_PORT_TYPE_UNKNOWN, FALSE     \
    }

typedef struct _DD_DISPLAY_ACTIVE
{
    DDU32 ChildId; // Target Id for CCD/DVM use.
    DD_TARGET_DESCRIPTOR TgtDesc;
    DD_BOOL IsActive;
    DD_BPC_SUPPORTED BitsPerColor;
    DDU8 CurrentBitsPerColor;
    DD_SOURCE_MODE_INFO SourceDetails;
    DD_TIMING_INFO TimingDetails;
    DDU32 ScanAddress;
    DDU32 Pitch;
    DD_BOOL IsPrimaryTarget; // To be used only in case of GetPreBootActiveDisplay, will be filled by MMIO State reading
    DD_BOOL IsScalerEnabled;
} DD_DISPLAY_ACTIVE;

typedef struct _DD_GET_ACTIVE_DISPLAY_ARGS
{
    // The structure is used for getting the boot config. Only Intel pipes are valid here
    DD_DISPLAY_ACTIVE DisplayActive[MAX_PHYSICAL_PIPES];
    DDU32 CurrentCdClockInKhz;
    DDU32 ActiveDisplayCount;
} DD_GET_ACTIVE_DISPLAY_ARGS;

typedef struct _DD_DISPLAY_DETECT_ARGS
{
    DD_IN DD_TARGET_DESCRIPTOR Target;
    DD_IN DD_TARGET_DETECT_MODE DetectMode;
    DD_OUT DD_DISPLAY_CONNECTION_EVENTS ConnectionEvent;
} DD_DISPLAY_DETECT_ARGS;

// enum DD_SPI_EVENTS: List of Short Pulse HPD events
typedef enum _DD_SPI_EVENTS
{
    DD_SPI_NONE,
    DD_SPI_CONNECTION_EVENT,
    DD_SPI_LINK_LOSS_EVENT, // DP link retraining event, handled by OSL
    DD_SPI_ATR_EVENT,
    DD_SPI_PARTIAL_DETECTION_EVENT, // MST CSN
    DD_SPI_CP_EVENT,
    DD_SPI_CRC_ERROR_EVENT,       // To CRC error in PSR
    DD_SPI_RESOURCE_CHANGE_EVENT, // set by TBT tunnel BW manager
    DD_SPI_PSR_CAPS_CHANGE_EVENT, // Set when PSR caps change from PSR to PSR2 or vice versa
    DD_SPI_MAX_EVENTS
} DD_SPI_EVENTS;

#define DD_VDISP_MAX_EDID_BLOCKS 2

typedef struct _DD_VIRTUAL_DISP_HPD_ARGS
{
    DD_BOOL IsDisplayAttached;
    DD_VIDEO_OUTPUT_TECHNOLOGY OutputTechnology;
    DDU8 SinkIndex;
    DDU8 EdidSize;
    DDU8 Edid[EDID_BLOCK_SIZE * DD_VDISP_MAX_EDID_BLOCKS];
    DD_2DREGION NativeResolution;
} DD_VIRTUAL_DISP_HPD_ARGS;

//----------------------------------------------------------------------------
//
// Display detection and HPD/SPI related structures - END
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
//  InfoFrame related data structures -- START
//
//----------------------------------------------------------------------------
#define INFOFRAME_HEADER_SIZE 4

// Inorder to send 36 bytes of payload and 4 bytes of header in the case of VSC EXT VESA SDP, we need to do 20 register writes as mentioned below
// 3 Register Write Operations for setting the registers VideoVscExtSdpConf, VideoVscExtSdpHeader, VideoVscExtSdpCtl0
// 16 Register Write Operations for setting payload using VideoVscExtSdpData0
// 1 Register Write Operation for setting ready bit of VideoVscExtSdpCtl0 register
#define NUM_OF_DSB_REGISTER_WRITES_FOR_SENDING_64_BYTES_OF_PAYLOAD 20
#define NUMBER_OF_VSC_EXT_VESA_SDP_PACKETS_TO_SEND 1

#define MAX_VSC_EXT_BYTES_WHEN_PAYLOADONLY_IN_BUFFER 32

// InfoFrame Payload Length in bytes
// These are defined in iHDMI.h as well as iDP.h as well.
// Re-defining here to combine them
typedef enum _DD_INFOFRAME_LENGTH
{
    VS_LENGTH = 27,  // Vendor Specific (VS), including IEEE reg ID, InfoFrame Payload Length
    AVI_LENGTH = 13,  // Auxiliary Video Information (AVI) InfoFrame Payload Length
    GMP_LENGTH = 28,  // Gamut MetaData Packet (GMP) InfoFrame Payload Length
    SPD_LENGTH = 25,  // Source Product Description (SPD) InfoFrame Payload Length
    GCP_LENGTH = 4,   // General Control Packet (GCP)InfoFrame Payload Length
    AUDIO_LENGTH = 10,  // Audio InfoFrame Payload Length
    MS_LENGTH = 10,  // MPEG Source InfoFrame Payload Length
    PR_PE_LENGTH = 4,   // Length of PR_PE_TYPE
    AUDIO_CAPS_LENGTH = 4,   // Length of AUDIO_CAPS_TYPE
    PSR1_VSC_LENGTH = 8,   // PSR1 Video Stream Configuration (VSC) SDP Packet max length
    PSR2_VSC_LENGTH = 16,  // PSR2 VSC SDP Packet max length
    COLORIMETRY_PIXELENCODING_VSC_LENGTH = 19,  // Colorimetry and Pixel Encoding VSC SDP Packet max length
    HDR_VSC_LENGTH = 32,  // HDR VSC SDP Packet max length
    PPS_LENGTH = 128, // Picture Parameter Set Max length
    VRR_LENGTH = 9,   // VRR SDP max payload bytes
} DD_INFOFRAME_LENGTH;

// Data Island pkt type
// DIP_GCP is handled within setmode
typedef enum _DD_DIP_TYPE
{
    DIP_NONE = 0,
    DIP_AVI,
    DIP_VS,
    DIP_GMP,
    DIP_SPD,
    DIP_VSC,
    DIP_PPS,
    DIP_DRM,
    DIP_VSC_EXT_SDP,
    DIP_VRR_SDP,
    DIP_VTEM_SDP,
    DIP_DYNAMIC_HDR_SDP,
    DIP_VSEM_SDP,
    DIP_SBTM_EMP,
    DIP_ALL = 31
} DD_DIP_TYPE;

// Different type of method available to debug and capture SDPs
typedef enum _DD_SDP_MON_METHOD
{
    DD_NONE_SDP_MON_METHOD = 0,
    DD_PERSISTANT_SDP_MON_METHOD,
    DD_SINGLE_SDP_CAPTURE_METHOD,
    DD_PERSISTANT_FRAME_CAPTURE_METHOD,
    DD_MULTIFRAME_CAPTURE_METHOD,
} DD_SDP_MON_METHOD;

// Select which DP SDP to monitor
typedef enum _DD_SDP_TYPE
{
    DD_NONE_SDP = 0,
    DD_VSC_SDP,
    DD_VRR_SDP,
    DD_VS_SDP,
    DD_GMP_SDP,
    DD_PPS_SDP,
    DD_VSC_EXT_SDP,
    DD_AUDIO_STREAM,
    DD_AUDIO_TIMESTAMP,
    DD_AUDIO_INFOFRAME
} DD_SDP_TYPE;

#define IS_EMP_DIP_TYPE(DipType) ((DipType == DIP_VTEM_SDP) || (DipType == DIP_DYNAMIC_HDR_SDP) || (DipType == DIP_VSEM_SDP))

// GCP Data
typedef union _DD_GCP_DATA
{
    DDU32 Value;
    struct
    {
        DDU32 AvMute : 1;             // GCP_AV_MUTE
        DDU32 IndicateColorDepth : 1; // GCP_COLOR_INDICATION
        DDU32 Reserved : 30;          // Reserved
    };
} DD_GCP_DATA;

typedef enum _DD_AVI_OPERATION
{
    DD_SET_AVI_CUSTOM_INFO = 0,
    DD_GET_AVI_CUSTOM_INFO = 1,
    DD_SEND_COLORIMETRY_INFO = 2
} DD_AVI_OPERATION;

typedef struct _DD_AVI_OPERATION_ARGS
{
    DD_AVI_OPERATION OperationType;
    DD_COLOR_PIXEL_DESC* pOutputPixelFormat;
    AVI_INFOFRAME_CUSTOM* pCustomAviInfoFrame;
    DD_TARGET_DESCRIPTOR TgtDesc;
} DD_AVI_OPERATION_ARGS;

typedef struct _DD_INFO_FRAME_ARGS
{
    DD_BOOL Enable;
    DD_PORT_TYPE Port;
    DDU32 Pipe;
    DD_BOOL IsPsrEnable;
    DD_PROTOCOL_TYPE Protocol;
    DD_DIP_TYPE DipType;
    DDU32 DipSize;
    DDU8* pDipData;
    DDU32 HblankEarlyStartValue; // needed only for VSC EXT SDP programming
    DDU32 PacketsPerChain;
    DD_BOOL IsVscUsedforNonPsr; // Needed only for VSC SDP Programming to select VSC_SELECT
    DD_BOOL IsVscUsedforPcon;   // Set only in PCON path for VSIF use cases
    DD_BOOL IsSwPsrExit;        // Set during SWPSR disable to clear VSC_SELECT bit
} DD_INFO_FRAME_ARGS;

typedef struct _DD_DIP_ENABLE_ARGS
{
    DD_PORT_TYPE Port;
    DDU32 Pipe;
    DD_PROTOCOL_TYPE Protocol;
    DD_DIP_TYPE DipType;
    DD_BOOL Enable;
    DD_BOOL IsSwControlledVscHeader;
    DD_BOOL IsSwControlledVscHeaderAndData;
    DD_BOOL IsSwPsrExit;
} DD_DIP_ENABLE_ARGS;
//----------------------------------------------------------------------------
//
//  InfoFrame related data structures -- START
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
// Static encoder data structures: Related enums and structures - START
//
//----------------------------------------------------------------------------

// Vendor and Product Identification: 11 bytes
// This comprehends the 10Byte EDID structure and 11 bytes DID format
// EDID Structure will fill the 3rd Byte of ManufactureID with zero
typedef union _DD_PNP_ID
{
    DDU8 VendorProductID[11]; // Vendor / Product identification
    struct
    {
        DDU8 ManufacturerID[3]; // Bytes 0 - 2: Manufacturer ID
        DDU8 ProductID[2];      // Bytes 3, 4: Product ID
        DDU8 SerialNumber[4];   // Bytes 5 - 8: Serial numbers
        DDU8 WeekOfManufacture; // Byte 9: Week of manufacture
        DDU8 YearOfManufacture; // Byte 10: Year of manufacture
    };
} DD_PNP_ID;

typedef struct _DD_COLLAGE_PERSISTENCE_DATA
{
    DD_IN DD_COLLAGE_TOPOLOGY CollageTopology;
    DD_IN DD_PNP_ID PnpIdChildTarget[MAX_PER_MCD_TARGETS];
} DD_COLLAGE_PERSISTENCE_DATA;

typedef struct _DD_GENLOCK_DISPLAY_INFO
{
    DD_IN_OUT DDU32 TargetId;
    DD_IN_OUT DD_BOOL IsPrimaryTarget;
    DD_OUT DDU32 LinkRateMbps;
    DD_OUT DDU8 DpLaneWidthSelection;
} DD_GENLOCK_DISPLAY_INFO;

typedef struct _DD_GENLOCK_CONFIG
{
    DD_BOOL IsGenlockSupported;
    DD_BOOL IsGenlockEnabled;
    DD_BOOL IsPrimaryGenlockSystem;
    DDU8 NumGenlockDisplays;
    DD_TIMING_INFO CommonTargetMode;
    DD_GENLOCK_DISPLAY_INFO DisplayInfo[MAX_PHYSICAL_PIPES];
    PIPE_ID PrimaryPipeId;
} DD_GENLOCK_CONFIG;

typedef struct _DD_GENLOCK_PERSISTENCE_DATA
{
    DD_BOOL IsGenlockEnabled;
    DDU32 NumAdapters;
    DDU32 AdapterBdf[DD_LDA_MAX_ADAPTERS];
    DD_GENLOCK_CONFIG Config[DD_LDA_MAX_ADAPTERS];
} DD_GENLOCK_PERSISTENCE_DATA;

typedef struct _DD_MCD_PERSISTENCE_DATA
{
    DD_COLLAGE_TOPOLOGY CollageTopology;
    DD_PNP_ID PnpIdChildTarget[MAX_PER_MCD_TARGETS];
    DD_GENLOCK_PERSISTENCE_DATA GenlockData;
} DD_MCD_PERSISTENCE_DATA;

//----------------------------------------------------------------------------
//
// DP/eDP encoder data related definitions
//
//----------------------------------------------------------------------------

// DP/EDP supported LinkRates in MBPS (Mega Bits Per Second/Lane)
#define DP_LINKRATE_1620_MBPS 1620
#define DP_LINKRATE_2160_MBPS 2160
#define DP_LINKRATE_2430_MBPS 2430
#define DP_LINKRATE_2700_MBPS 2700
#define DP_LINKRATE_3240_MBPS 3240
#define DP_LINKRATE_3780_MBPS 3780
#define DP_LINKRATE_4320_MBPS 4320
#define DP_LINKRATE_5400_MBPS 5400
#define DP_LINKRATE_6480_MBPS 6480
#define DP_LINKRATE_6750_MBPS 6750
#define DP_LINKRATE_8100_MBPS 8100

#define DP_LINKRATE_10000_MBPS 10000 // DP 2.0, UHBR10
#define DP_LINKRATE_13500_MBPS 13500 // DP 2.0, UHBR13.5
#define DP_LINKRATE_20000_MBPS 20000 // DP 2.0, UHBR20

// x: LinkRate value in Mbps
// For UHBR+ link rates we shall always be using 128b/132b channel coding.
// And similarly for link rates less than UHBR10 we shall always be using 8b/10 channel coding.
#define GET_DP_CHANNEL_CODING(x) (((x) >= DP_LINKRATE_10000_MBPS) ? CH_CODING_128B_132B : CH_CODING_8B_10B)

// Enabling FEC results in bandwidth overhead of approximately 2.774%
// (i.e, an FEC-enabled DP link has 2.774% less link bandwidth available for transport of stream(s)).
// Hence, FEC overhead for 1MHz clock would result in multiplier of 972661
// Reference Bspec link: [Transcoder MN Value] https://gfxspecs.intel.com/Predator/Home/Index/4285
#define FEC_OVERHEAD_PER_MEGAHERTZ 972261

// TU_LOW_END <= TU <= TU_HIGH_END
#define TU_LOW_END 32
#define TU_HIGH_END 64

// DP Data Bandwidth Efficiency related
/** Data Bandwidth Efficiency considering SST 8b/10b encoding overhead without FEC: 80%. */
#define DP_DATA_BW_EFFICIENCY_SST_PER_10000 8000
/** Data Bandwidth Efficiency considering SST 8b/10b encoding overhead with FEC (including FEC overhead of 2.4%): 78.08%. */
#define DP_DATA_BW_EFFICIENCY_SST_FEC_PER_10000 7808
/** Data Bandwidth Efficiency considering SST 8b/10b encoding overhead with FEC (including FEC overhead of 2.4%) and EOC: 77.78 %. */
#define DP_DATA_BW_EFFICIENCY_SST_DSC_PER_10000 7778
/** Data Bandwidth Efficiency considering MST 8b/10b encoding overhead and MST overhead of 1.562%: 78.75%. */
#define DP_DATA_BW_EFFICIENCY_MST_PER_10000 7875
/** Data Bandwidth Efficiency considering MST (1.562% overhead) 8b/10b encoding with FEC (2.4% overhead):  76.9%. */
#define DP_DATA_BW_EFFICIENCY_MST_FEC_PER_10000 7686
/** Data Bandwidth Efficiency for 128b/132b encoding: 96.71% (for UHBR+ link rates channel encoding is 128b/132b). */
#define DP_DATA_BW_EFFICIENCY_128B_132B_PER_10000 9671
/** Data Bandwidth Efficiency considering MST (1.562% overhead) 8b/10b encoding overhead with FEC (including FEC overhead of 2.4%) and EOC: 76.56 % */
#define DP_DATA_BW_EFFICIENCY_MST_DSC_PER_10000 7656

// Available MTP TimeSlots
#define AVAILABLE_MTP_TIMESLOTS_8B_10B 63
#define AVAILABLE_MTP_TIMESLOTS_128B_132B 64

#define LS_CLOCK_DIVISOR_DP_1_4 10 ///< For DP1.4, LinkSymbol Clock = LinkRate / 10
#define LS_CLOCK_DIVISOR_DP_2_0 32 ///< For DP2.0, LinkSymbol Clock = LinkRate / 32
#define DP_MAX_LANE_COUNT 4
typedef enum _DD_LANE_WIDTH
{
    LANE_X0 = 0, // No lanes enabled
    LANE_X1 = 1,
    LANE_X2 = 2,
    LANE_X3 = 3, // used only for MIPI DSI
    LANE_X4 = 4,
} DD_LANE_WIDTH;

typedef enum _DD_DP_TRAINING_PATTERN
{
    DP_PAT_TRAINING_NOT_IN_PROGRESS = 0,
    DP_TRAINING_PAT_1 = 1,
    DP_TRAINING_PAT_2 = 2,
    DP_TRAINING_PAT_3 = 3,
    DP_PAT_D10_2_WITHOUT_SCRAMBLING = 4,
    DP_PAT_SYMBOL_ERR_MSR_CNT = 5,
    DP_PAT_PRBS7 = 6,
    DP_PAT_IDLE = 7,
    DP_PAT_SCRAMBLING = 8,
    DP_PAT_HBR2_EYE_COMPLIANCE = 9, // CP2520 Pattern 1
    DP_PAT_BITS80_CUSTOM = 10,
    DP_PAT_PCT = 11,
    DD_PAT_MANCHESTER_ENCODING = 12,
    DP_TRAINING_PAT_4 = 13,
    DP_PAT_HBR3_EYE_COMPLIANCE = 14, // CP2520 Pattern 3
    DP_PAT_128B_132B_TPS1 = 15,
    DP_PAT_128B_132B_TPS2 = 16,
    DP_PAT_PRBS9 = 17,
    DP_PAT_PRBS11 = 18,
    DP_PAT_PRBS15 = 19,
    DP_PAT_PRBS23 = 20,
    DP_PAT_PRBS31 = 21,
    DP_PAT_BITS264_CUSTOM = 22,
    DP_PAT_SQ_DEFAULT = 23, // Square pattern with pre-cursor and post-cursor both enabled. This is default option.
    DP_PAT_SQ_PRECURSOR_DISABLED = 24, // Square pattern with pre-cursor disabled and post-cursor enabled.
    DP_PAT_SQ_POSTCURSOR_DISABLED = 25, // Square pattern with pre-cursor enabled and post-cursor disabled.
    DP_PAT_SQ_PREPOSTCURSOR_DISABLED = 26  // Square pattern with pre-cursor and post-cursor both disabled.
} DD_DP_TRAINING_PATTERN;

typedef enum _DD_DP_PAT_SQ_NUM
{
    DD_DP_PAT_SQ_NUM_INVALID = 0,
    DD_DP_PAT_SQ_NUM_1 = 1,
    DD_DP_PAT_SQ_NUM_2 = 2,
    DD_DP_PAT_SQ_NUM_5 = 5,
    DD_DP_PAT_SQ_NUM_10 = 10,
    DD_DP_PAT_SQ_NUM_16 = 16,
    DD_DP_PAT_SQ_NUM_20 = 20,
    DD_DP_PAT_SQ_NUM_64 = 64,
} DD_DP_PAT_SQ_NUM;

typedef enum _DD_PORT_SYNC_SUPPORT_STATUS
{
    DD_PORT_SYNC_NONE = 0,
    DD_PORT_SYNC_SW,
    DD_PORT_SYNC_HW
} DD_PORT_SYNC_SUPPORT_STATUS;

typedef enum _DD_LINK_OPERATION_TYPE
{
    DP_OP_NONE = 0,
    DP_OP_SET_LINK_PATTERN,
    HDMI_OP_SET_LINK_PATTERN,
    DP_OP_ADJ_DRIVE_SETTINGS,
    HDMI_OP_ADJ_DRIVE_SETTINGS,
    MIPI_DSI_OP_TURNOFF_LINK,
    DP_OP_SET_AUX_DELAY,
    DP_OP_ENABLE_AUX_CHANNEL,
    DP_OP_DISABLE_AUX_CHANNEL,
} DD_LINK_OPERATION_TYPE;

typedef enum _DD_DP_AUX_CHANNEL_TYPE
{
    AUX_CHANNEL_UNDEFINED = -1,
    AUX_CHANNEL_A,
    AUX_CHANNEL_B,
    AUX_CHANNEL_C,
    AUX_CHANNEL_D,
    AUX_CHANNEL_E,
    AUX_CHANNEL_F,
    AUX_CHANNEL_G,
    AUX_CHANNEL_H,
    AUX_CHANNEL_I,
    AUX_CHANNEL_MAX,
} DD_DP_AUX_CHANNEL_TYPE;

typedef enum _DD_DP_VOLTAGE_SWING_LEVEL
{
    DP_VSWING_0_4 = 0,
    DP_VSWING_0_6 = 1,
    DP_VSWING_0_8 = 2,
    DP_VSWING_1_2 = 3,
    DP_VSWING_MAX = DP_VSWING_1_2
} DD_DP_VOLTAGE_SWING_LEVEL;

typedef enum _DD_DP_PREEMPHASIS_LEVEL
{
    DP_PREEMP_NO_PREEMPHASIS = 0,
    DP_PREEMP_3_5DB = 1,
    DP_PREEMP_6DB = 2,
    DP_PREEMP_9_5DB = 3,
    DP_PREEMP_MAX = DP_PREEMP_9_5DB
} DD_DP_PREEMPHASIS_LEVEL;

// TX_FFE_PRESET_VALUE Levels
typedef enum _DD_DP_FFE_PRESET_LEVEL
{
    DP_FFE_PRESET_UNDEFINED = -1,
    DP_FFE_PRESET_0 = 0,
    DP_FFE_PRESET_1,
    DP_FFE_PRESET_2,
    DP_FFE_PRESET_3,
    DP_FFE_PRESET_4,
    DP_FFE_PRESET_5,
    DP_FFE_PRESET_6,
    DP_FFE_PRESET_7,
    DP_FFE_PRESET_8,
    DP_FFE_PRESET_9,
    DP_FFE_PRESET_10,
    DP_FFE_PRESET_11,
    DP_FFE_PRESET_12,
    DP_FFE_PRESET_13,
    DP_FFE_PRESET_14,
    DP_FFE_PRESET_15,
    DP_FFE_PRESET_MAX = DP_FFE_PRESET_15
} DD_DP_FFE_PRESET_LEVEL;

// PCON DP to HDMI 2.1 DownStream-Facing Port (DFP) Caps
#define MAX_HDMI_LANES_TMDS_MODE 3
#define MAX_HDMI_LANES_FRL_MODE 4
#define HDMI_NUM_OF_MAX_FRL_RATES 5

#define FRL_CLOCK_DIVISOR_HDMI_2_1 18 ///< For HDMI 2.1, DDI Clock = Fixed Link Rate / 18

#define HDMI_LINKRATE_3000_MBPS 3000   // HDMI 2.1 TMDS link rate 3Gbps
#define HDMI_LINKRATE_6000_MBPS 6000   // HDMI 2.1 TMDS/FRL link rate 6Gbps
#define HDMI_LINKRATE_8000_MBPS 8000   // HDMI 2.1 FRL link rate 8Gbps
#define HDMI_LINKRATE_10000_MBPS 10000 // HDMI 2.1 FRL link rate 10Gbps
#define HDMI_LINKRATE_12000_MBPS 12000 // HDMI 2.1 FRL link rate 12Gbps
#define TMDS_CHAR_CLOCK_RATE_MULTIPLIER_X10 25
#define VGA_MAX_PIXEL_RATE_MULTIPLIER 8

// TX_FFE_PRESET_VALUE Levels
typedef enum _HDMI_FFE_PRESET_LEVEL
{
    HDMI_FFE_PRESET_UNDEFINED = -1,
    HDMI_FFE_PRESET_0 = 0,
    HDMI_FFE_PRESET_1,
    HDMI_FFE_PRESET_2,
    HDMI_FFE_PRESET_3,
    HDMI_FFE_PRESET_MAX = HDMI_FFE_PRESET_3
} HDMI_FFE_PRESET_LEVEL;

typedef enum _DD_HDMI_TRAINING_PATTERN
{
    HDMI_TRAINING_NOT_IN_PROGRESS = 0,
    HDMI_TRAINING_PAT_1, // All 1's pattern
    HDMI_TRAINING_PAT_2, // All 0's pattern
    HDMI_TRAINING_PAT_3, // Nyquist clock pattern (alternating 0 and 1)
    HDMI_TRAINING_PAT_4, // Tx Feed Forward Equalizer (TxFFE) compliance test pattern (repeating pattern of 128 zeroes followed by 128 ones)
    /* 16b/18b encoded patterns; No Scrambling For each of the following LTP's the Source transmits a sequence of 4096 FRL Characters of the specified Lane LFSR */
    HDMI_TRAINING_PAT_5,            // LFSR 0 pattern
    HDMI_TRAINING_PAT_6,            // LFSR 1 pattern
    HDMI_TRAINING_PAT_7,            // LFSR 2 pattern
    HDMI_TRAINING_PAT_8,            // LFSR 3 pattern
    HDMI_TRAINING_PASS,             // Post link training programming, Like Frl Training complete programming in transcoder.
    HDMI_TRAINING_UPDATE_FFE = 0xE, // Special coded message to program different FFE level.
    HDMI_TRAINING_UPDATE_FRL = 0xF  // Special coded message to lower the FRL Rate
} DD_HDMI_TRAINING_PATTERN;

typedef enum _DD_DRIVE_SETTING_TYPE
{
    DRIVE_SETTING_NORMAL = 0,              // pre-cursor and post-cursor both enabled. This is default option.
    DRIVE_SETTING_PRE_CURSOR_DISABLED,     // pre-cursor disabled and post-cursor enabled.
    DRIVE_SETTING_POST_CURSOR_DISABLED,    // pre-cursor enabled and post-cursor disabled.
    DRIVE_SETTING_PRE_POST_CURSOR_DISABLED // pre-cursor and post-cursor both disabled.
} DD_DRIVE_SETTING_TYPE;

typedef struct _DD_DP_DRIVE_SETTING
{
    DD_DP_VOLTAGE_SWING_LEVEL SwingLevel;
    DD_DP_PREEMPHASIS_LEVEL PreEmpLevel;
    DD_DP_FFE_PRESET_LEVEL TxFfePresetValue128b132b[DP_MAX_LANE_COUNT]; /**< Transmitter FFE(Feed Forward Equalization) Preset Value(0 to 15) for 128b/132b channel coding. */
} DD_DP_DRIVE_SETTING;

typedef struct _DD_DP_REDRIVER_PARAMS
{
    DD_BOOL IsDockablePort;                      // EFP display is routed through Dock?
    DD_BOOL IsOnBoardRedriverPresent;            // Onboard Redriver Present
    DD_DP_DRIVE_SETTING OnBoardRedriverSettings; // Onboard Redriver settings
    DD_BOOL IsOnDockRedriverPresent;             // OnDock Redriver Present
    DD_DP_DRIVE_SETTING OnDockRedriverSettings;  // OnDock Redriver settings
} DD_DP_REDRIVER_PARAMS;

typedef struct _DD_MST_BW_DATA
{
    /** DP MST data. */
    DDU8 StreamId;
    /** RAD of the target device. */
    DD_DP_RELATIVE_ADDRESS RAD;
    DDU8 VcPayloadTableStartIdx;
    /** number of VC payload table slots. */
    DDU8 VcPayloadTableNumSlots;
    /** MSTMode. */
    DD_BOOL EnableMSTMode;
    /** This will indicate how much PBN will actually be needed and is the value used in ALLOCATE_PAYLOAD sideband message. */
    DD_OUT DDU32 ActualPBN;
    /** Allocated PBN : This will indicate how much PBN will be allocated in DP Source based on the number of slots & Link Rate &  Number Of Lanes & HW restrictions. */
    DD_OUT DDU32 AllocatedPBN;
} DD_MST_BW_DATA;

// DP Data/Link M/N TU data
typedef struct _DD_M_N_CONFIG
{
    DDU64 DataM;
    DDU64 DataN;
    DDU32 LinkM;
    DDU32 LinkN;
    DDU32 DataTU;
    DDU64 VFreqHz; /**< absolute pixel rate in units of Hz for 128b/132b channel coding. */
} DD_M_N_CONFIG;

typedef struct _DD_PROGRAM_TIMING_ARGS
{
    PIPE_ID Pipe;
    DD_PORT_TYPE Port;
    DD_BOOL IsModeset;
    DD_BOOL SynchronizeDisplay;
    DD_BOOL IsPsr2Supported;
    DD_BOOL IsInterlacedMode;
    DD_BOOL IsPanelReplaySupported;
    DD_VIDEO_OUTPUT_TECHNOLOGY SinkType;
} DD_PROGRAM_TIMING_ARGS;

typedef union _DD_DISABLE_DMRRS_FLAGS
{
    DDU32 Value;
    struct
    {
        DDU32 DisableDmrrsInternalDisplay : 1; // Bit 0
        DDU32 DisableDmrrsExternalDisplay : 1; // Bit 1
        DDU32 UNIQUENAME(Reserved) : 30;       // Bits (2:31)
    };
} DD_DISABLE_DMRRS_FLAGS;

C_ASSERT(sizeof(DD_DISABLE_DMRRS_FLAGS) == sizeof(DDU32));

typedef struct _DD_COMPARE_ACTIVE_DISPLAY_TIMING_ARGS
{
    DDU32 Pipe;
    DD_TARGET_DESCRIPTOR TgtDesc;
    DD_TIMING_INFO* pNewTimingInfo;
    DD_M_N_CONFIG NewMNTuValue;
    DDU64 DsiDataRateHz;
    DD_BOOL IsPsr2Supported;
    DD_BOOL SynchronizeDisplay;
    DD_BOOL IsPanelReplaySupported;
    DD_BOOL IsLobfSupported;
} DD_COMPARE_ACTIVE_DISPLAY_TIMING_ARGS;

typedef enum _DP_CHANNEL_CODING_TYPE
{
    CH_CODING_8B_10B = 0,
    CH_CODING_128B_132B
} DP_CHANNEL_CODING_TYPE;

// bits_per_pixel is represented in fixed point U6.4 format,
// Keeping 4 fractional bits and rest integral part inside this struct.
// for e.g:
// 1) 24 bits_per_pixel is populated as follows:
//      BitsPerPixel.FractionalPart = 0;
//      BitsPerPixel.IntegralPart = 24
// 2) 12.5 bits_per_pixel is populated as follows:
//      BitsPerPixel.FractionalPart = 8;
//      BitsPerPixel.IntegralPart = 12
typedef union _DD_BITS_PER_PIXEL
{
    DDU16 Bppx16; // BPP value multipled by 16
    struct
    {
        DDU16 FractionalPart : 4; // [3:0] fractional_part of bits_per_pixel
        DDU16 IntegralPart : 12;  // [15:4] integral_part of bits_per_pixel
    };
} DD_BITS_PER_PIXEL;

typedef enum _DD_AUDIO_SUPPORT_MODE
{
    DD_AUDIO_NOT_SUPPORTED = 0,
    DD_AUDIO_SUPPORTED_WITHOUT_SPLIT,
    DD_AUDIO_SUPPORTED_WITH_SPLIT
} DD_AUDIO_SUPPORT_MODE;

typedef struct _DD_AUDIO_SUPPORT
{
    /** Audio Supported Mode. */
    DD_AUDIO_SUPPORT_MODE AudioSupportMode;
    /** Audio Freq in Hz. */
    DDU32 AudioFreqHz;
    /** No Of Audio Channels. */
    DDU8 NoOfAudioChannels;
} DD_AUDIO_SUPPORT;

typedef struct _DD_LINK_BW_DATA
{
    /** Link Rate in Mbps per channel. */
    DDU32 LinkRateMbps;
    /** In hZ. */
    DDU64 DotClockInHz;
    /** Enable down spreading. */
    DD_BOOL EnableSpread;
    /** Bits per pixel. For compression supported panel, it will be output compression BPP. */
    DD_BITS_PER_PIXEL BitsPerPixel;
    /** M/N TU data. */
    DD_M_N_CONFIG MNTUData;
    /** DD_BOOL indicating if MSO is enabled or not. */
    DD_BOOL IsMsoEnabled;
    /** Number of segments in case of MSO panels. 0: not segmented, 2: 2 SST Links, 4: 4 SST Links. */
    DDU8 NumOfLinks;
    /** Number of Overlap Pixel count for MSO panels. */
    DDU8 PixelOverlapCount;
    /** Port width selection for DP sink alone X1/X2/X4 mode. Default is X1 enum used DP_PORT_WIDTH. */
    DD_LANE_WIDTH DpLaneWidthSelection;
    /** DD_BOOL flag indicating whether FEC is to be enabled or not (Used in case of external DP compression.) */
    DD_BOOL EnableFec;
    /** DP Channel Coding Type either 8b/10b or 128b/132b.It will be initialized to DPRX caps, but later can be modified during LT. */
    DP_CHANNEL_CODING_TYPE DpChannelCodingType;
    /** Dp Data Bw in Mbits per sec. */
    DDU64 DataBwMbps;
    /** DP SSC Overhead. */
    DDU32 DpSscOverheadx1e6;
} DD_LINK_BW_DATA;

typedef enum DD_MSA_MISC1_BIT2_BIT1
{
    PIPE_S3D_DISABLED = 0x00,
    PIPE_S3D_RIGHT_FRAME = 0x01,
    PIPE_S3D_LEFT_FRAME = 0x03
} DD_MSA_MISC1_BIT2_BIT1;

typedef enum _DD_MSA_MISC_COMPONENT_FORMAT
{
    DD_MSA_MISC_COMPONENT_FORMAT_RGB,
    DD_MSA_MISC_COMPONENT_FORMAT_YCBCR_422,
    DD_MSA_MISC_COMPONENT_FORMAT_YCBCR_444,
    DD_MSA_MISC_COMPONENT_FORMAT_RESERVED
} DD_MSA_MISC_COMPONENT_FORMAT;

typedef union _DD_MSA_MISC
{
    DDU32 Value;
    struct
    {
        // ulMSA_Misc0                  : 8; // bits 7:0
        DDU32 SyncClock : 1;        // bit 0 (0-asysnchronous , 1- synchronous)
        DDU32 ComponentFormat : 2;  //  bits 2:1 (00 - RGB, 01 YCbCr 4:2:2, 10 YCbCr 4:4:4, 11 -reserved)
        DDU32 Range : 1;            // bit 3 (0- vesa, 1- cea)
        DDU32 YCbCrColorimetry : 1; // bit 4 (0 - ITU-R BT601-5, 1 - ITU-R BT709-5)
        DDU32 ColorDepth : 3;       // bit 7:5 (000 - 6 bits, 001 - 8 bits, 010 - 10 bits, 011 - 12 bits, 100 = 16 bits)

        // ulMSA_Misc1                  : 8; // bit 15:8
        DDU32 MSA_Misc1_bit0 : 1;                    // bit 0
        DD_MSA_MISC1_BIT2_BIT1 MSA_Misc1_bit2_1 : 2; //  bits 2:1
        DDU32 MSA_Misc1_bit5_3 : 3;                  // bits 5:3
        DDU32 MSA_Misc1_bit6 : 1;                    // bit 6 VSC SDP to be used for sending colorimetry information.
        DDU32 MSA_Misc1_bit7 : 1;                    // bit 7

        DDU32 UNIQUENAME(Reserved) : 16; // bit 31:16
    };
} DD_MSA_MISC;

typedef union _DD_PORT_SYNC_MODE
{
    DDU8 Value;
    struct
    {
        DDU8 LfpCmtgSync : 1;
        DDU8 DpSstPortSync : 1;
        DDU8 DpMstPortSync : 1;
        DDU8 SwSync : 1;
        DDU8 GenLock : 1;
        DDU8 PipeLock : 1;
        DDU8 Reserved : 2;
    };
} DD_PORT_SYNC_MODE;

typedef struct _DD_GENLOCK_CFG
{
    union
    {
        DDU8 Flags;
        struct
        {
            DDU8 IsPrimaryGenlockSystem : 1; // Bit 0
            DDU8 IsPrimaryTranscoder : 1;    // Bit 1
            /** Bit 2-7: Reserved.     */
            DDU8 Reserved : 6;
        };
    };
    DD_BOOL IsEnabled;
    PIPE_ID PrimaryPipe;
} DD_GENLOCK_CFG;

typedef struct _DD_PORT_SYNC_CFG
{
    PIPE_ID PrimaryPipe;   // Used by Port sync.
    PIPE_ID SecondaryPipe; // Indicates secondary pipe Id for Tiled Display having only 2 tiles.
} DD_PORT_SYNC_CFG;

typedef struct _DP_LINK_DATA
{
    DD_PORT_TYPE Port;
    DD_VIDEO_OUTPUT_TECHNOLOGY SinkType;
    DD_LINK_BW_DATA LinkBwData;
    DD_DP_DRIVE_SETTING CurrentDriveSetting;
    DD_BOOL EnhancedFramingEnable;
    DD_BOOL ASSRSupport;
    DD_BOOL EnableScrambling;
    DD_BOOL IsMsaIgnoreRequired;
    DD_BOOL IsPsr2Supported;
    DD_BOOL IsPanelReplaySupported;
    DD_BOOL IsLobfSupported;
    DD_BOOL IsFastLinkTrainingPossible; /**< Checked in TrainLink to attempt No-Aux Training, in EndLinkTraining to skip clearing the Training pattern Aux. */
    DD_BOOL IsVrrSdpSupported;
    DD_BOOL IsSsSbmSupported; /**< Whether down-stream DP Branch Device supports Single Stream Sibeband Message Support. (MST_CAP(21h[0] == 0) && SS_SB_MSG_SUP(21h[1] == 1)).*/
    DD_DP_TRAINING_PATTERN ChanEqTrainingPat;
    DD_MST_BW_DATA MstBwData;
    DD_MSA_MISC MsaMisc;
    DD_PORT_SYNC_MODE PortSyncMode;
    DD_BOOL IsLinkTrained;      /**< Used in enable path to decide on Link Cfg requirement. */
    DD_BOOL IsLinkBwSufficient; /**< If Current Link BW can drive the Requested mode. */
    DDU8 RepeaterCount;         /**< Used for each repeater link training. */
    /**
     * Used for denoting Primary Transcoder in MST case. Although name says Transcoder this field is of PIPE_ID type as we can't get Tarnscoder info in Protocol layer.
     * Since Pipe and Transcoder are one-to-one mapped in HW it is okay to use Pipe info for this field.
     */
    PIPE_ID PrimaryTranscoder;
    DD_BOOL DscCapable;
    DDU8 DscSlicesPerScanline;
    DDU32 HActive;
    DDU32 HTotal;
    DDU32 HActiveLLSymbolClocks;      /**< HActive Link Layer Symbol Clock Cycles. */
    DDU32 HBlankLLSymbolClocks;       /**< HBlank Link Layer Symbol Clock Cycles. */
    DDU32 HTotalLLSymbolClocks;       /**< HTotal Link Layer Symbol Clock Cycles. */
    DDU32 ChunkSizeLLSymbolClocks;    /**< Chunk Size Link Layer Symbol Clock Cycles. */
    DD_BOOL IsTunnelingGpuBwMgmtMode; /**< TBT or USB4 Tunnel is in GPU BW management mode. */
    DD_BOOL TrainLinkWithHobl;
    DD_BOOL IsGenLockSupported;
    DD_GENLOCK_CFG GenLockCfg;
    DDU32 HBlankIncLinkSymbolClocks;
    DD_ALPM_MODE AlpmMode;
    DD_LARGE_INTEGER Usb4BWRequestRefrenceTickCount;
} DP_LINK_DATA;

typedef struct _EXT_DP_PROTOCOL_STATE
{
    DD_BOOL IsDp8b10b : 1;
    DD_BOOL IsDp128b132b : 1;
    DD_BOOL IsDpSst : 1;
    DD_BOOL IsDpMstPrimary : 1;
    DD_BOOL IsDpMstSecondary : 1;
} EXT_DP_PROTOCOL_STATE;

// Choose the sub function
typedef enum _DD_DP_APPLET_SUB_FUNC_TYPE
{
    // Override DP parameters.  Parameters: -> LaneCount, LinkRate, Voltage, Preemphasis, SSC
    DD_SET_DP_PARAM = 0,
    // Set DP PHY layer Test patterns.
    DD_SET_PHY_PATTERN = 1,
    // Set DP output Voltage Swing and pre emphasis level without performing any Link Training.
    // Parameters: -> ucVoltage, ucPreemphasis
    DD_SET_DP_PARAM_WITHOUT_LINK_TRAINING = 2,
    // Enable Compliance Mode of Operation. Used for PHY layer testing with Scope connected.
    // Disables hot plug detection so that we can connect a scope after mode set without
    // disabling pipe/port. It also disables AUX transactions as Scope does not support AUX.
    // Parameters: -> NONE.
    DD_ON_COMPLIANCE = 3,
    // Disables Compliance Mode of Operation. Enable HPD and AUX. Parameters: -> NONE.
    DD_ON_NORMAL = 4,
    // Get current DP parameters programmed. Parameters: -> Voltage, Preemphasis, LaneCount, LinkRate.
    DD_GET_DP_PARAM = 5,
    // Does an Internal Mode set.
    DD_SET_MODE = 6,
    // Set Display Device on Pipe B.
    DD_DISABLE_DISPLAY = 7,
    // Disable Pipe Pass Through Mode for Pattern tests
    DD_OFF_PIPE_PASS_THROUGH = 8,
    // Enable Pipe Pass Through Mode for Pattern tests
    DD_ON_PIPE_PASS_THROUGH = 9,
    // Get the currently trained link data like link rate, lane count.
    DD_GET_CURRENT_TRAINED_LINK_DATA = 10
} DD_DP_APPLET_SUB_FUNC_TYPE;

typedef struct DD_DP_SINK_CAPABILITIES
{
    DD_TARGET_DESCRIPTOR TgtDesc;
    DD_DP_APPLET_SUB_FUNC_TYPE DpAppletSubFnType;
    DD_LINK_BW_DATA DpLinkBwData;
    DD_BOOL ForceDPAppletSettings;
    DD_DP_DRIVE_SETTING CurrentDriveSetting;
    DD_DP_TRAINING_PATTERN ChanEqTrainingPat;
    DDU8 SquarePatternNumber;
    DD_BOOL ScramblingEnabled;
} DD_DP_SINK_CAPABILITIES;

typedef struct _DD_PORT_SYNC_CAPS
{
    DD_PORT_SYNC_SUPPORT_STATUS Capability;
    DDU32 PrimaryPipe;   // used to indicate Primary pipe when PORT_SYNC_HW is used
    DDU32 SecondaryPipe; // used to indicate Secondary pipe when PORT_SYCH_HW is used
} DD_PORT_SYNC_CAPS;

//----------------------------------------------------------------------------
//
// DP/eDP encoder data related definitions - END
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
// HDMI related enums/structures - START
//
//----------------------------------------------------------------------------

#define TMDS_CHAR_RATE_25MCSC 25000000 // Min TMDS char rate
#define TMDS_CHAR_RATE_165MCSC 165000000
#define TMDS_CHAR_RATE_297MCSC 297000000
#define TMDS_CHAR_RATE_300MCSC 300000000 // High TMDS char rate
#define TMDS_CHAR_RATE_330MCSC 330000000 // High TMDS char rate
#define TMDS_CHAR_RATE_340MCSC 340000000 // Max for Dual-Mode Type 2 adapter
#define TMDS_CHAR_RATE_594MCSC 594000000
#define TMDS_CHAR_RATE_600MCSC 600000000 // Max for Dual-Mode Type 2 adapter for MB down
#define HDMI_YCBCR420_PIXELCLOCK_RESTRICTION 590000000

typedef enum _DD_HDMI_DATARATE
{
    DATARATE_MAX = 0,
    DATARATE_2_97_GT_S = 1,
    DATARATE_1_65_GT_S = 2,
    DATARATE_5_94_GT_S = 3,
    DATARATE_3_40_GT_S = 4,
    DATARATE_3_00_GT_S = 5
} DD_HDMI_DATARATE;

typedef enum _DD_HDMI_FRLRATE
{
    FRLRATE_NOT_SUPPORTED = 0,
    FRLRATE_INVALID = 1,
    FRLRATE_3000MBPS = 3000,
    FRLRATE_6000MBPS = 6000,
    FRLRATE_8000MBPS = 8000,
    FRLRATE_10000MBPS = 10000,
    FRLRATE_12000MBPS = 12000
} DD_HDMI_FRLRATE;

typedef struct _HDMI_BW_DATA
{
    DDU64 TriBytesBorrowed;          // HDMI 2.1, Tri-bytes borrowed
    DDU64 AvgTriByteRateNominalInHz; // Avergage Tri-byte rate using the Nominal PixelClock for LinkMN Calculation
    DDU32 HCActive;                  // HCActive in Tri-bytes
    DDU32 HCBlank;                   // HCBlank in Tri-bytes
    DDU64 AvgAudioRelatedPktRate;    // AverageAudioRelatedPacket Rate
    DDU32 DscSliceCount;             // Dsc Slicecount, used to determine the PPC/Bpp during FrlPacketPayloadCalculation
} HDMI_BW_DATA;

// HDMI specfic information
typedef struct _HDMI_LINK_DATA
{
    DD_PORT_TYPE Port;
    DDU64 DotClockInHz;         //!< Used for HDMI port enable. For DP, link rate field in pLinkData shall be used.
    DDU64 TmdsCharacterRateCsc; // in units of Characters/second/channel (Csc)
    DD_BOOL EnableScrambling;
    DD_BOOL IsFrlMode;                                                     //!< Is HDMI enabled in FRL mode?
    DDU8 NumHdmiLanesInFrlMode;                                            //!< If HDMI is in FRL mode, Number of lanes used for HDMI FRL (3 or 4)
    DD_HDMI_TRAINING_PATTERN LinkTrainingPattern[MAX_HDMI_LANES_FRL_MODE]; //!< Per lane Link training pattern to be applied
    HDMI_BW_DATA BwData;
    DDU16 MaxFrlPacketPayloadAllowed; // This field allows Software to override the maximum FRL Packet payload that hardware is allowed to schedule
    DDU32 LinkM;                      // LINK M/N values controls the Data Flow Metering's TBDifference,Max pointer
    DDU32 LinkN;
    DD_BOOL ResourceBasedSchedulingEnable; // Determines if Active character buffering needed
    DDU32 FrlRatePerLane;
    DD_BOOL IsLinkTrained;
    HDMI_FFE_PRESET_LEVEL TxFfeLevel[MAX_HDMI_LANES_FRL_MODE]; // Transmitter Feed Forward Equalizer levels ( 0 to 3) for HDMI FRL 16b/18b channel coding
    DD_BOOL IsHdcpWithAudioSupported;
    DD_BOOL DscCapable;
    DDU32 ActiveCharcterBufThreshold;
} HDMI_LINK_DATA;

typedef struct _HDMI_NORMATIVETABLE_VIC_INFO
{
    DDU8 VicId;
    DDU32 FrlRateMbps;
    DDU8 FrlLanes;
    DDU8 DscBpp;
    DD_BOOL Is3G3LUncompressed8BpcMode;
} HDMI_NORMATIVETABLE_VIC_INFO;

typedef union _HDMI_FRL_WA_FLAGS
{
    DDU8 Value;
    struct
    {
        DDU8 EnableHdmiAudioOverheadWa : 1; // Enabled based on HW Wa_14014078854 flag.
        DDU8 EnableHCTotalWa : 1;           // Enabled based on HW Wa_14017837703 flag.
        DDU8 EnableIncreasedHblank : 1;     // Enabled based on HW Wa_14014078854 flag.
        // If enabled add TbOverhead to TbBlank or HCBlank to allow increased BW during FRL capacity calculation. (Workaround Method 2: Add Overhead to Hblank).
        // If disabled driver would add TbOverhead to AudioMin Calculation and prune modes exceeding the limit.(Workaround Method 1: Add Overhead to Hblank)
        DDU8 UNIQUENAME(Reserved) : 5;
    };
} HDMI_FRL_WA_FLAGS;
//----------------------------------------------------------------------------
//
// HDMI related enums/structures - END
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
// eDP and PSR related enums/structures - START
//
//----------------------------------------------------------------------------

typedef enum _DD_EDP_SWING_TABLE
{
    LOW_POWER_SWING_TABLE = 0,
    DEFAULT_SWING_TABLE
} DD_EDP_SWING_TABLE;

typedef enum _DD_LINES_TO_WAIT
{
    LINES_TO_WAIT_0 = 0,
    LINES_TO_WAIT_2,
    LINES_TO_WAIT_4,
    LINES_TO_WAIT_8
} DD_LINES_TO_WAIT;

typedef enum _DD_CLOCK_RECOVERY_WAKEUPTIME
{
    CLOCK_RECOVERY_WAKEUP_TIME_500_US = 0,
    CLOCK_RECOVERY_WAKEUP_TIME_100_US,
    CLOCK_RECOVERY_WAKEUP_TIME_2P5_MS,
    CLOCK_RECOVERY_WAKEUP_TIME_0_SKIP,
} DD_CLOCK_RECOVERY_WAKEUPTIME;

typedef enum _DD_PSR_PR_VERSION
{
    PSR_VERSION_NONE,
    PSR_VERSION_1,
    PSR_VERSION_2,
    PANELREPLAY_LINKON, // DP
    PANELREPLAY_LINKOFF // EDP / DP
} DD_PSR_PR_VERSION;

typedef enum _DD_PSR1_CHANNELEQ_WAKEUPTIME
{
    PSR1_CHANNEL_EQ_WAKEUP_TIME_500_US = 0,
    PSR1_CHANNEL_EQ_WAKEUP_TIME_100_US,
    PSR1_CHANNEL_EQ_WAKEUP_TIME_2P5_MS,
    PSR1_CHANNEL_EQ_WAKEUP_TIME_0_SKIP,
} DD_PSR1_CHANNEL_EQ_WAKEUPTIME;

typedef enum _DD_PSR2_CHANNEL_EQ_WAKEUPTIME
{
    PSR2_CHANNEL_EQ_WAKEUP_TIME_500_US = 0,
    PSR2_CHANNEL_EQ_WAKEUP_TIME_100_US,
    PSR2_CHANNEL_EQ_WAKEUP_TIME_2P5_MS,
    PSR2_CHANNEL_EQ_WAKEUP_TIME_50_US,
} DD_PSR2_CHANNEL_EQ_WAKEUPTIME;

typedef struct _DD_GET_PSR_PR_CONFIG_DETAILS
{
    DD_PSR_PR_VERSION PsrPrVersion;
    DD_BOOL Psr2PrSfSupported;
    DD_BOOL RfbUpdateMode;
    DD_BOOL Psr2OrPrEnabled;
} DD_GET_PSR_PR_CONFIG_DETAILS;

typedef struct _DD_PSR_DISPLAY_CAPS
{
    DDU8 IsPsrSupported : 1;                 // PSR support, dynamically updated in mode-set
    DDU8 IsSuSupported : 1;                  // PSR2 support, dynamically updated in mode-set
    DDU8 IsNoLinkTrainingRequiredOnExit : 1; // 0 (default) - Link training is required
    DDU8 IsTp3Supported : 1;                 // 1 for TPS3, 0 for TPS2
    DDU8 IsYcordRequiredInVsc : 1;           // Y cordinate in VSC
    DDU8 IsFrameSyncRequired : 1;
    DDU8 IsEarlyTptSupported : 1;
    DDU8 IsPsr2EarlyScanLineSdpRequired : 1;    // updated dynamically in mode-set
    DDU8 IsPsrSdpEarlyFrameCaptureRequired : 1; // updated dynamically in mode-set
    DDU8 PsrSuGranularityNeeded : 1;

    DDU8 PsrIdleFrameThreshold;
    DDU8 SuIdleFrames; // updated dynamically in mode-set for Gen15+ if Early frame transport is needed
    DDU8 Psr2BlockSize;

    DD_CLOCK_RECOVERY_WAKEUPTIME ClockRecoveryTime;
    DD_PSR1_CHANNEL_EQ_WAKEUPTIME Psr1ChannelEqualizationTime;
    DD_PSR2_CHANNEL_EQ_WAKEUPTIME Psr2ChannelEqualizationTime;
    DDU32 MaxSfSuDpstSegments;    // used for HW SFSU DPST feature.
    DDU32 MinSfSuDpstSegmentSize; // used for HW SFSU DPST feature.
} DD_PSR_DISPLAY_CAPS;

typedef struct _DD_EDP_FLT_PARAMS
{
    DDU32 LinkRateMbps;                          // Link Rate in Mega Bits Per Second
    DD_LANE_WIDTH NumOfLanesInUse;               // Lane Count to be used of type DD_LANE_WIDTH
    DD_DP_PREEMPHASIS_LEVEL CurrentPreEmpLevel;  // Pre-Emphasis to be used of type DD_DP_PREEMPHASIS_LEVEL
    DD_DP_VOLTAGE_SWING_LEVEL CurrentSwingLevel; // Vswing to be used of type DD_DP_VOLTAGE_SWING_LEVEL
} DD_EDP_FLT_PARAMS;

typedef struct _DD_APICAL_IP_TABLE
{
    DDU32 PanelIeeeOui;        // Apical IP specific field for Panel OUI
    DDU32 DpcdBaseAddress;     // Apical IP specific field for DPCD Base address
    DDU32 DpcdIrdidixControl0; // Apical IP specific field for DPCD Idridix Control 0
    DDU32 DpcdOptionSelect;    // Apical IP specific field for DPCD option select
    DDU32 DpcdBacklight;       // Apical IP specific field for DPCD backlight
    DDU32 AmbientLight;        // Apical IP specific field for Ambient light
    DDU32 BacklightScale;      // Apical IP specific field for Backlight scale
} DD_APICAL_TABLE;

typedef struct _DD_EDP_APICAL_PARAMS
{
    DDU16 ApicalAsrtDispIpEnable; // Apical Assertive Display IP Enable
    DD_APICAL_TABLE ApicalTable;  // Apical Assertive Display IP Table
} DD_EDP_APICAL_PARAMS;
//----------------------------------------------------------------------------
//
// eDP and PSR related enums/structures - END
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
// MipiDsi related enums/structures - START
//
//----------------------------------------------------------------------------

typedef struct _DD_DPHY_PARAMS
{
    DDU8 TClkPrepare;          // TClkPrepare value
    DDU8 TClkTrail;            // ucTClkTrail value
    DDU16 TClkPrepare_ClkZero; // usTClkPrepare + ClkZero value

    DDU8 THSPrepare;         // THSPrepare Value
    DDU16 THSPrepare_HSZero; // ucTHSPrepare + HSZero value
    DDU8 THSTrail;           // THSTrail Value
    DDU8 TLpx;
} DD_DPHY_PARAMS;

typedef enum _DD_MIPI_DSI_MODE
{
    MIPI_DSI_VIDEO_MODE_NON_BURST_SYNC_PULSE = 1,
    MIPI_DSI_VIDEO_MODE_NON_BURST_SYNC_EVENTS,
    MIPI_DSI_VIDEO_MODE_BURST,
    MIPI_DSI_COMMAND_MODE
} DD_MIPI_DSI_MODE;

#define IS_MIPI_DSI_COMMAND_MODE(mode) ((mode) == MIPI_DSI_COMMAND_MODE)
#define IS_MIPI_DSI_VIDEO_MODE(mode) (((mode) == MIPI_DSI_VIDEO_MODE_NON_BURST_SYNC_PULSE) || ((mode) == MIPI_DSI_VIDEO_MODE_NON_BURST_SYNC_EVENTS) || ((mode) == MIPI_DSI_VIDEO_MODE_BURST))

typedef enum _DD_MIPI_DSI_LINK_CONFIG
{
    MIPI_DSI_SINGLE_LINK = 0,
    MIPI_DSI_DUAL_LINK_FRONT_BACK,
    MIPI_DSI_DUAL_LINK_PIXEL_ALTERNATIVE
} DD_MIPI_DSI_LINK_CONFIG;

typedef enum _DD_EDP_LINK_CONFIG
{
    EDP_SINGLE_LINK = 1,
    EDP_MSO_2_LINK = 2,
    EDP_MSO_4_LINK = 4
} DD_EDP_LINK_CONFIG;

typedef enum _DD_LP_BYTECLK_SEL
{
    CLOCK_20MHZ = 0,
    CLOCK_10MHZ,
    CLOCK_5MHZ
} DD_LP_BYTECLK_SEL;

typedef enum _DD_SEQUENCE_TYPE
{
    SEQ_UNDEFINED = 0,
    SEQ_ASSERT_RESET,
    SEQ_SEND_DCS,
    SEQ_DISPLAY_ON,
    SEQ_DISPLAY_OFF,
    SEQ_DE_ASSERT_RESET,
    SEQ_BKLT_ON,
    SEQ_BKLT_OFF,
    SEQ_TEAR_ON,
    SEQ_TEAR_OFF,
    SEQ_POWER_ON,
    SEQ_POWER_OFF,
    SEQ_MAX
} DD_SEQUENCE_TYPE;

typedef enum _DD_SEQ_OP_BYTE
{
    SEQ_OP_UNDEFINED = 0,
    SEQ_OP_SEND_PACKET,
    SEQ_OP_DELAY,
    SEQ_OP_PROG_GPIO,
    SEQ_OP_PROG_I2C,
    SEQ_OP_PROG_SPI,
    SEQ_OP_PROG_PMIC,
    SEQ_OP_MAX
} DD_SEQ_OP_BYTE;

typedef enum _DD_GPIO_PIN_MAPPING
{
    MIPI_RESET_1 = 0,
    MIPI_AVDD_EN_1,
    MIPI_BKLT_EN_1,
    MIPI_AVEE_EN_1,
    MIPI_VIO_EN_1,
    MIPI_RESET_2,
    MIPI_AVDD_EN_2,
    MIPI_BKLT_EN_2,
    MIPI_AVEE_EN_2,
    MIPI_VIO_EN_2,
    MIPI_GPIO_NUMBER_MAX
} DD_GPIO_PIN_MAPPING;

#pragma pack(1)

typedef enum _DD_GPIO_PIN_TYPE
{
    NATIVE_GPIO_PIN_TYPE = 0x0,
    NON_NATIVE_GPIO_PIN_TYPE = 0x1,
} DD_GPIO_PIN_TYPE;

typedef struct _DD_SEQ_GPIO_FLAGS
{
    union
    {
        DDU8 GpioFlags;
        struct
        {
            DDU8 GPIOVal : 1;              // Bit0 GPIO pin state [0 - Pull GPIO pin to low; 1 - PullGPIO Pin to high]
            DDU8 GPIOPinType : 1;          // Bit 1 GPIO Pin Type [0 - Native Pin; 1 - Non-native Pin]
            DDU8 UNIQUENAME(Reserved) : 6; // Bits [7:2] - Reserved
        };
    };
} DD_SEQ_GPIO_FLAGS;

typedef struct _DD_SEQ_GPIO_PROG
{
    DDU8 SeqOperationType;
    DDU8 SizeOfDelay;
    DDU8 GpioResourceIndex;
    DDU8 GpioNumber;
    DD_SEQ_GPIO_FLAGS GPIOFlags;
} DD_SEQ_GPIO_PROG;

typedef struct _DD_SENDPKT_CMD_FLAG
{
    union
    {
        DDU8 CmdFlag;
        struct
        {
            DDU8 PktTransmissionMode : 1; // 0 - Low Power Mode, 1 - High Speed Mode
            DDU8 VirtualChannel : 2;      // Virtual channel # (0 - 3)
            DDU8 PortType : 2;            // 00 - Mipi - A (DSI0), 01 - Mipi - C (DSI1)
            DDU8 Rsvd : 3;                // Reserved
        };
    };
} DD_SEQ_CMD_FLAG;

#define DSI_LONG_PACKET_MAX_WORDCOUNT 250
#define DSI_SHORT_PACKET_MAX_WORDCOUNT 2

typedef enum _DD_DSI_PACKET_DATA_TYPE
{
    GENERIC_SHORT_WRITE_NO_PARAMS = 0x03, // Generic short write, no parameters
    GENERIC_SHORT_WRITE_ONE_PARAM = 0x13, // Generic short write, 1 parameters
    GENERIC_SHORT_WRITE_TWO_PARAMS = 0x23, // Generic short write, 2 parameters
    COMPRESSION_MODE_DATA_TYPE_WRITE = 0x07, // Generic send Compression enable command to panel

    GENERIC_SHORT_READ_NO_PARAMS = 0x04, // Generic read, no parameters, requestes data from  peripheral
    GENERIC_SHORT_READ_ONE_PARAM = 0x14, // Generic read, 1 parameters
    GENERIC_SHORT_READ_TWO_PARAMS = 0x24, // Generic read, 2 parameters

    // Generic Long
    GENERIC_LONG_WRITE = 0x29, // Generic long write: block data transfer
    PPS_LONG_WRITE = 0x0A, // Generic Send Picture Params to panel

    // DCS short
    DCS_SHORT_WRITE_NO_PARAMS = 0x05, // DCS short write, no parameters
    DCS_SHORT_WRITE_ONE_PARAM = 0x15, // DCS short write, 1 parameters
    DCS_SHORT_READ_NO_PARAMS = 0x06, // DCS read, no parameters

    // DCS Long
    DCS_LONG_WRITE = 0x39, // DCS long write

    SET_MAX_RETURN_PACKET_SIZE_SHORT = 0x37, // sets max size of payload transmitted from peripheral to prevent buffer overflow (not reqd for unidirection)

    // Read responses Data types
    GENERIC_SHORT_READ_ONE_BYTE_RESPONSE = 0x11, // Generic Short READ Response, 1 byte returned
    GENERIC_SHORT_READ_TWO_BYTE_RESPONSE = 0x12, // Generic Short READ Response, 2 bytes returned
    DCS_SHORT_READ_ONE_BYTE_RESPONSE = 0x21, // DCS Short READ Response, 1 byte returned
    DCS_SHORT_READ_TWO_BYTE_RESPONSE = 0x22, // DCS Short READ Response, 2 bytes returned
    GENERIC_LONG_READ_RESPONSE = 0x1A, // Generic Long READ Response
    DCS_LONG_READ_RESPONSE = 0x1C, // DCS Long READ Response
} DD_DSI_PACKET_DATA_TYPE;

typedef struct _DD_SEQ_SEND_PACKET_HEADER
{
    DDU8 SeqOperationType;
    DDU8 SizeOfSendPkt; // 4 + N bytes
    DD_SEQ_CMD_FLAG SendPktCmdFlag;
    DDU8 SendPktDataType; // Of type DD_SENDPKT_DATA_TYPE
    DDU16 WordCount;      // Word count
} DD_SEQ_SEND_PACKET_HEADER;

#define PKT_TRANSMISSION_IN_LP_MODE 0
#define PKT_TRANSMISSION_IN_HS_MODE 1

#define MIPI_WRDISBV_REG 0x51
#define MIPI_RDDISBV_REG 0x52

typedef struct _DD_MIPI_DSI_TX_PACKET
{
    DD_PORT_TYPE Port;                // MipiDsi port the packet needs to be sent to
    DD_DSI_PACKET_DATA_TYPE DataType; // The Data Type field specifies if the packet is a Long or Short packet type and the packet format
    DDU8 VirtualChannel;              // Virtual channel
    DD_BOOL PktTransmissionMode;      // LP or HS. HS = 1
    DDU16 WordCount;                  // Number of bytes to be sent
    DDU8* pData;                      // Can have multi-byte data for long write
} DD_MIPI_DSI_TX_PACKET;

typedef struct _DD_MIPI_DSI_RX_PACKET
{
    DD_PORT_TYPE Port;                // MipiDsi port the packet needs to be sent to
    DD_DSI_PACKET_DATA_TYPE DataType; // The Data Type field specifies if the packet is a Long or Short packet type and the packet format
    DDU8 VirtualChannel;              // Virtual channel
    DD_BOOL PktTransmissionMode;      // LP or HS. HS = 1
    DDU8* pRegOffset;                 // Pointer to register address from which read happens
    DDU8* pReadData;                  // Pointer to Read buffer data
    DDU16* pReadCount;                // Pointer to Read Count
    DDU32 AllotedReadBufferCount;     // AllocatedReadBufferContent
} DD_MIPI_DSI_RX_PACKET;

typedef struct _DD_SEQ_DELAY
{
    DDU8 SeqOperationType;
    DDU8 SizeOfDelay;
    DDU32 Delay; // Delay value
} DD_SEQ_DELAY;

typedef struct _DD_GPIO_FLAG
{
    union
    {
        DDU8 GpioFlag;
        struct
        {
            DDU8 PullDirection : 1; // 0 - Pull GPIO Low, 1 - Pull GPIO high
            DDU8 Rsvd : 7;          // Reserved
        };
    };
} DD_GPIO_FLAG;

typedef struct _DD_SEQ_PROG_I2C
{
    DDU8 SeqOperationType;
    DDU8 SizeOfProgI2c; // 7 + N bytes

    DDU8 I2cFlag;           // Rsvd for future
    DDU8 I2cResourceIndex;  // For Windows, this comes as part of I2C resource ACPI enumeration from OS
    DDU8 I2cBusNumber;      // Un-used in Windows
    DDU16 I2cTargetAddress; // Un-used in Windows (10-bit Target address)
    DDU8 I2cRegOffset;      // Register offset
    DDU8 I2cPayloadSize;    // Size of Data to be sent over I2C
} DD_SEQ_PROG_I2C;

typedef struct _DD_SEQ_PROG_SPI
{
    DDU8 SeqOperationType;
    DDU8 SizeOfProgSPI; // 6 + N bytes

    DDU8 SpiFlag;          // Rsvd for future
    DDU8 SpiResourceIndex; // For Windows, this comes as part of SPI resource ACPI enumeration from OS
    DDU8 SpiBusNumber;     // Un-used in Windows
    DDU8 SpiTargetAddress; // Un-used in Windows (10-bit Target address)
    DDU8 SpiRegOffset;     // Register offset
    DDU8 SpiPayloadSize;   // Size of Data to be sent over I2C
} DD_SEQ_PROG_SPI;

typedef struct _DD_SEQ_PROG_PMIC
{
    DDU8 SeqOperationType;
    DDU8 SizeOfProgPmic;     // 15 bytes
    DDU8 PmicFlag;           // Rsvd for future
    DDU16 PmicTargetAddress; // PMIC Target address
    DDU32 PmicRegOffset;     // PMIC register offset
    DDU32 PmicRegData;       // PMIC register value to be written
    DDU32 PmicRegDataMask;   // PMIC register Mask
} DD_SEQ_PROG_PMIC;

//----------------------------------------------------------------------------
//
// MipiDsi related enums/structures - END
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
// LFP and PowerCons related definitions - START
//
//----------------------------------------------------------------------------

typedef struct _DD_CHROMA_AND_LUMA_DATA
{
    DD_BOOL OverrideLumaAvailable;
    DD_BOOL OverrideGammaAvailable;
    DD_BOOL OverrideChromaAvailable;

    // Color primaries data
    DDU16 RedX;
    DDU16 RedY;
    DDU16 GreenX;
    DDU16 GreenY;
    DDU16 BlueX;
    DDU16 BlueY;
    DDU16 WhiteX;
    DDU16 WhiteY;

    // Luminance data
    DDU16 MinLuminance;
    DDU16 MaxLuminance;
    DDU16 MaxFullFrameLuminance;

    // Gamma
    DDU8 Gamma;
} DD_CHROMA_AND_LUMA_DATA;

typedef struct _DD_PANEL_DIMENSIONS
{
    DDU16 PanelWidthInMm;  // Panel Width as given in DTD (in mm)
    DDU16 PanelHeightInMm; // Panel Height as given in DTD (in mm)
} DD_PANEL_DIMENSIONS;

typedef enum _DD_PPS_SIGNAL
{
    DD_VDD = 0,
    DD_DATA,
    DD_PWM,
    DD_BKLT
} DD_PPS_SIGNAL;

typedef struct _DD_PPS_DELAY_TABLE
{
    DD_BOOL IsPpsSupported;       // If PPS delays are non-zero, we assume its supported, else not
    DDU16 PowerOnDelay;           // T3 -  Power-Up delay.
    DDU16 PowerOnToBkltOnDelay;   // T8  - Power-On to Backlight Enable delay.
    DDU16 BkltOffToPowerOffDelay; // T9  - Backlight-Off to Power-Down delay.
    DDU16 PowerOffDelay;          // T10 - Power-Down delay.
    DDU16 PowerCycleDelay;        // T12 - Power cycle delay
    DDU16 PwmOnToBkltOnDelay;     // Tx -  PWM On to Bklt On Delay
    DDU16 BkltOffToPwmOffdelay;   // Ty  - Bklt OFF to PWM Off Delay
} DD_PPS_DELAY_TABLE;

typedef struct _DD_RX_LRR_CAPS
{
    DDU8 IsLrrValid : 1; // FALSE if 314h DPCD is 0 else TRUE
    DDU8 IsSourcePixelClockBasedRRChange : 1;
    DDU8 IsTconBasedRRChange : 1;
    DDU8 IsSourceVTotalBasedRRChange : 1;
    DDU8 IsAlrrSupported : 1;
    DDU8 Reserved : 3;
} DD_RX_LRR_CAPS;

// DPST/OPST Related
#define XPST_BIN_COUNT 32      // Total number of segments in DPST
#define XPST_IET_LUT_LENGTH 33 // Total number of IET entries
#define OPST_PWR_CONTRIBUTION_WEIGHT_LUT_SIZE 17
#define SOLID_COLOR_POWER_THRESHOLD 0.95
#define SOLID_COLOR_SEARCH_WINDOW_SIZE 4
#define OPST_MAX_SLOPE_IN_DIMMING_MULTIPLIER_LUT 2.0
#define OPST_MIN_SLOPE_IN_DIMMING_MULTIPLIER_LUT 0.0
#define DISPLAY_PC_DPST_EPSM_MIN_WEIGHT 70.0 // Min Epsm weight decided based on visual and power testing.
#define DISPLAY_PC_DPST_EPSM_MAX_WEIGHT 100.0
#define DISPLAY_PC_DPST_MIN_AGGR_SETTING 0         // Lowest DPST aggressiveness setting in driver
#define DISPLAY_PC_DPST_MAX_AGGR_SETTING 6         // Max DPST aggressiveness setting in driver, applicable for DPST version's < 8.0
#define DISPLAY_PC_OPST_MAX_AGGR_SETTING_AC 3      // Max OPST aggressiveness setting in driver in AC power source, applicable for OPST v1.0
#define DISPLAY_PC_OPST_MAX_AGGR_SETTING_DC 3      // Max OPST aggressiveness setting in driver in DC power source, applicable for OPST v1.0
#define DISPLAY_PC_OPST_V1_1_MAX_AGGR_SETTING_AC 1 // Max OPST aggressiveness setting in driver in AC power source, applicable for OPST v1.1
#define DISPLAY_PC_DPST_V8_0_MAX_AGGR_SETTING_DC 3 // Max aggressiveness setting in driver for Dpst 8.0 and above in DC power source, applicable for DPST v8.0
#define DISPLAY_PC_DPST_V8_0_MAX_AGGR_SETTING_AC 1 // Max aggressiveness setting in driver for Dpst 8.0 and above in AC power source, applicable for DPST v8.0
#define DISPLAY_PC_ELP_MAX_AGGR_SETTING 3          // Max number of ELP aggressiveness levels
#define DISPLAY_PC_APD_MAX_AGGR_SETTING 3          // Max number of APD aggressiveness levels
#define DISPLAY_PC_CABC_MAX_AGGR_SETTING 6         // Max number of CABC aggressiveness levels

#define DD_PANEL_GAMMA_HUNDREDS_TO_DOUBLE(GammaInHundreds) ((double)GammaInHundreds / 100.0f)

typedef union _OPST_LUMA_HIST_RGB_WEIGHTS
{
    DDU32 Value;
    struct
    {
        DDU32 BlueCoefficient : DD_BITFIELD_RANGE(0, 8);
        DDU32 GreenCoefficient : DD_BITFIELD_RANGE(9, 17);
        DDU32 RedCoefficient : DD_BITFIELD_RANGE(18, 26);
        DDU32 Reserved : DD_BITFIELD_RANGE(27, 31);
    };
} OPST_LUMA_HIST_RGB_WEIGHTS;

typedef enum _DPST8_0_AGGRESSIVENESS_LEVEL
{
    DPST_8_0_UI_DISABLE = DISPLAY_PC_DPST_MIN_AGGR_SETTING,
    DPST_8_0_UI_LEVEL_1,
    DPST_8_0_UI_LEVEL_2,
    DPST_8_0_UI_LEVEL_3,
    DPST_8_0_UI_LEVEL_MAX = DPST_8_0_UI_LEVEL_3,
} DPST8_0_AGGRESSIVENESS_LEVEL;

typedef enum _DD_DPST_OPERATION
{
    DD_DPST_HISTOGRAM_ENABLE = 0,
    DD_DPST_HISTOGRAM_DISABLE,
    DD_DPST_HISTOGRAM_RESET,
    DD_DPST_HISTOGRAM_STATUS,
    DD_DPST_HISTOGRAM_UPDATE_GUARDBAND_THRESHOLD,
    DD_DPST_PROGRAM_DIET_REGS,
    DD_DPST_HISTOGRAM_INTERRUPT_ENABLE_DISABLE,
    DD_DPST_HISTOGRAM_UPDATE_GUARDBAND_DELAY,
    DD_DPST_RESTORE_GUARDBAND_INTERRUPT_DELAY_COUNTER,
    DD_DPST_SFSU_UPDATE_SEGMENT_SIZE,
    DD_DPST_ENABLE_DISABLE_CLOCK_GATING,
    DD_DPST_HW_SFSU_ENABLE_DISABLE
} DD_DPST_OPERATION;

typedef enum _DPST_ALGORITHM_TYPE
{
    DPST_ALGORITHM_TYPE_BACKLIGHT,
    DPST_ALGORITHM_TYPE_OPST
} DPST_ALGORITHM_TYPE;

typedef enum _GHE_ALGORITHM_VERSION
{
    GHE_ALGORITHM_VERSION_NONE = 0,
    GHE_ALGORITHM_VERSION_1_0,
} GHE_ALGORITHM_VERSION;

typedef enum _SFSU_DPST_HANDLING_OPERATION
{
    SFSU_DPST_HANDLING_OPERATION_NONE = 0,
    SFSU_DPST_HANDLING_OPERATION_SET,
    SFSU_DPST_HANDLING_OPERATION_GET,
} SFSU_DPST_HANDLING_OPERATION;

typedef enum _XPST_WARNING
{
    XPST_WARNING_INVALID_CONTEXT,
    XPST_WARNING_INVALID_IMAGE_SIZE_OR_AGGR_LEVEL,
    XPST_WARNING_INVALID_PIXEL_COUNT,
    XPST_WARNING_NO_ACTIVE_LFP,
    XPST_WARNING_PANEL_DOES_NOT_SUPPORT_DPST,
    XPST_WARNING_FAILED_TO_GET_HISTOGRAM,
    XPST_WARNING_SYSTEM_IN_TDR,
    XPST_WARNING_LOCKING_FAILURE,
    XPST_WARNING_GHE_PANEL_NOT_SUPPORTED
} XPST_WARNING;

typedef struct _DD_DPST_ARGS
{
    PIPE_ID PipeId;
    DD_PORT_TYPE Port;
    DD_DPST_OPERATION DpstOpReq; // Operation Request
    DDU32 Histogram[XPST_BIN_COUNT];
    DDU32 LowThresholdGuardBand;
    DDU32 HighThresholdGuardBand;
    DDU32 InterruptDelay;
    DD_BOOL IsEnableDiet;
    DDU32 DietFactor[XPST_IET_LUT_LENGTH];
    DD_BOOL IsHistogramInterruptEnable;
    DD_BOOL IsDitheringEnable;
    DD_BOOL IsSwXpst;
    DPST_ALGORITHM_TYPE DpstAlgorithmType;
    GHE_ALGORITHM_VERSION GheAlgorithmVersion;
    OPST_LUMA_HIST_RGB_WEIGHTS RgbWeights;
    DD_BOOL WaitForVblank;
    DD_PSR_PR_VERSION PsrPrVersion;
    DD_BOOL EnableHwSfSuDpst;
    DDU32 SegmentSize;
    DD_BOOL IsDeepSleepExitNeeded;
    DD_BOOL HistogramStatusSuccess;
    DD_BOOL IsSfsuDpstHandlingNeeded;
    SFSU_DPST_HANDLING_OPERATION SfsuDpstHandlingOperation;
    DD_BOOL DisableClockGate;
    DDU32 ImageSize;
} DD_DPST_ARGS;

// DCE -> Display Contrast Enhancement
typedef struct _DD_GET_SET_DCE
{
    DD_BOOL IsGet; // [in] true = get, false = set operation
    DDU32 TargetId;
    DD_BOOL Enable;                  // [in/out] is the feature currently enabled
    DD_BOOL IsSupported;             // [in/out] is the feature currently enabled
    DDU32 NumBins;                   // [in/out] number of bins in the histogram
    DDU32 Histogram[XPST_BIN_COUNT]; // Bin wise histogram data for current frame.
    DDU32 TargetBrightness;          // The target brightness level for the display
    double PhaseinSpeedMultiplier;   // [in] Transition speed of 1.0 to 2.0 for brightness to take effect, 0 means immediate
    // eg. 1.2 is 1.2x default speed.
} DD_GET_SET_DCE;

typedef enum _DD_CABC_MODE
{
    DD_CABC_OFF = 0,
    DD_CABC_POWERSAVE_LOW,
    DD_CABC_POWERSAVE_MEDIUM,
    DD_CABC_POWERSAVE_HIGH
} DD_CABC_MODE;

typedef enum _DD_BLC_OPERATION
{
    DISP_PC_BLC_OPERATION_GET_BRIGHTNESS = 0,
    DISP_PC_BLC_OPERATION_SET_BRIGHTNESS,
    DISP_PC_BLC_OPERATION_SET_AGGRESSIVE_LEVEL,
    DISP_PC_BLC_OPERATION_SET_ALS_LUX,
    DISP_PC_BLC_OPERATION_RESET_DPST_ADJUST_FACTOR,
    DISP_PC_BLC_OPERATION_CHANGE_BRT_OPT_FTR_STATE // Change brightness optimization features state
} DD_BLC_OPERATION;

typedef struct _DD_BLC_DDI_PARAMS
{
    DD_BLC_OPERATION Operation;
    union
    {
        DD_TARGET_DESCRIPTOR TargetDescriptor;
        PIPE_ID Pipe;
    } Id;
    union
    {
        // DPST operation
        DD_BOOL FeatureActive;

        struct
        {
            // Set/Get Brightness operation
            DD_IN_OUT DDU32 TargetBrightness; // Actual Backlight Setting (Target Brightness Percent in 0-100 for B2, 0-100000 for B3 with High Precision)

            // Get Brightness operation
            DD_OUT DDU32 CurrentBrightness; // Actual Backlight Setting (Current Brightness 0-100000 for B3 with High Precision.
            // In B2, OS doesn't expect Current Brightness)
        };

        DDU16 Lux; // ALS Lux value to override
    };
} DD_BLC_DDI_PARAMS;

typedef struct _DD_GETSET_CABC_BLC_ARGS
{
    DDU32 DisplayUID;                     // Display ID on which CABC has to be enabled
    DD_BLC_OPERATION GetSetCABCBlcParams; // CABC BLC Operation to be performed
    DDU32 BrightnessValue;                // Brightness value to get/set
    DDU32 Maxbrightness;                  // Max Brightness value to be used (Constant from PC)
    DD_CABC_MODE CABCMode;                // CABC mode (Agressiveness level equivalant from OS)
    DD_PORT_TYPE PortType;                // Port Type (Internal parameter to SB, not passed from PC)
} DD_GETSET_CABC_BLC_ARGS;

typedef enum _DD_PWM_POLARITY
{
    PWM_POLARITY_NORMAL = 0, // Normal polarity (0 = full off)
    PWM_POLARITY_INVERSE     // Inverse polarity (0 = full on)
} DD_PWM_POLARITY;

typedef struct _DD_DPST_WAIT_FOR_ACTIVE_SCANLINE_PARAMS
{
    PIPE_ID Pipe;
    DD_BOOL IsDpstWaitForActiveScanLineRegion; // TRUE only if DPST adjustment is in progress
} DD_DPST_WAIT_FOR_ACTIVE_SCANLINE_PARAMS;

typedef struct _DD_BLC_PARAMS
{
    DD_PORT_TYPE Port;
    DDU32 BrightnessValue; // Brightness value with 100% precision for B2 or
    // Brightness value in milli percent for B3 High Precision
    DDU32 BlcPrecisionFactor;                                     // Precision of 100(B2) or 1000(B3 with High Precision Brightness)
    DD_DPST_WAIT_FOR_ACTIVE_SCANLINE_PARAMS DpstSafeScanlineArgs; // Wait for active scanline region
    DD_BOOL IsModeset;

} DD_BLC_PARAMS;

typedef struct _DD_PC_PHASEIN_FLAGS_PARAMS
{
    DD_BOOL ApplyBlc;
    DD_BOOL ApplyIet;
    DD_BOOL DpstPhaseInCompleted;
    DD_BLC_PARAMS BlcParams;
    DD_DPST_ARGS DpstParams;
} DD_PC_PHASEIN_FLAGS_PARAMS;

//----------------------------------------------------------------------------
//
// LFP and PowerCons related definitions - END
//
//----------------------------------------------------------------------------

// Static encoder Final data structures - START

typedef union _DSC_BPC_CAPS
{
    DDU8 Value;
    struct
    {
        DDU8 Reserved1 : 1;
        DDU8 DSC_8BPC_Supported : 1;
        DDU8 DSC_10BPC_Supported : 1;
        DDU8 DSC_12BPC_Supported : 1;
        DDU8 DSC_16BPC_Supported : 1;
        DDU8 Reserved2 : 3;
    };
} DSC_BPC_CAPS;

#define MAX_DSC_SLICES_INDEX 10 // This needs to be updated if more slice count values are added to DP spec. As of now there are 10 possible slice values
// For DP, this comes from Sink, for MIPI DSI this comes from VBT
// So this structure is included only in MIPI_DSI_DATA as encoder data
typedef struct _DD_RX_DSC_CAPS
{
    DDU8 IsDscSupported : 1;
    DDU8 ConvertRgb : 1;
    DDU8 IsRgbSupported : 1;
    DDU8 IsYCbCr444Supported : 1;
    DDU8 IsSimpleYCbCr422Supported : 1;
    DDU8 IsYCbCrNative422Supported : 1;
    DDU8 IsYCbCrNative420Supported : 1;
    DDU8 IsBlockPredictionSupported : 1;
    DDU8 IsFractionalBppSupported : 1;

    // DP 2.0
    DDU8 IsDscPassthroughSupported : 1;
    DDU8 DynamicPpsUpdateSupportComptocomp : 1;
    DDU8 DynamicPpsUpdateSupportUncomptocomp : 1;
    DDU8 RgbColorConversionBypassSupport : 1;
    DDU8 ParentDpBranchDscPassThroughSupport : 1;
    DDU8 MaxDscBppDeltaAvailability : 1;
    DDU8 IsMipiDsiDisplay : 1;
    DDU8 MaxDscBppRgbYCbCr444SimpleYCbCr422;
    DDU8 MaxDscBppNativeYCbCr420;
    DDU8 MaxDscBppNativeYCbCr422;

    DDU8 DscMajorVersion;
    DDU8 DscMinorVersion;
    DDU8 RcBufferBlockSize; ///< Size of 1 block in KB
    DDU8 RcBufferBlocks;
    DDU8 SlicesPerLine;                         // SlicesPerLine is Minimum of MaxSlice supported by Sink and MaxSlice supported by our HW
    DDU8 SupportedSlices[MAX_DSC_SLICES_INDEX]; // Holds the Supported slices as per Sink Capability
    DDU8 LineBufferDepth;
    DDU8 BppIncrement;
    DSC_BPC_CAPS CompressionBpc;
    DD_BITS_PER_PIXEL MaxDscBitsPerPixel; // bits_per_pixel in U6.4 format
    DDU32 DscSliceHeight;
    DDU32 DscMaxSliceWidth;
    DDU32 PeakDscThroughputMode0MHz;          // throughput for 444 and simple422 in MHz
    DDU32 PeakDscThroughputMode1MHz;          // throughput for Native 420/422 in mHz
    DDU32 BranchDscOverallThroughputMode0MPs; // MST Branch's overall throughput for 444 and simple422 in MP/s
    DDU32 BranchDscOverallThroughputMode1MPs; // MST Branch's overall throughput for Native 420/422 in MP/s
    DDU32 BranchDscMaximumLineBufferDepth;    // Branch DSC maximum line buffer depth
} DD_RX_DSC_CAPS;

// DP 2.0 Panel Replay Caps
typedef struct _DD_PANEL_REPLAY_CAPS
{
    DDU8 PanelReplaySupported : 1;
    DDU8 SelectiveUpdateSupport : 1;
    DDU8 PanelReplaySuGranularityNeeded : 1;
    DDU8 PanelReplayEnabled : 1;
    DDU8 EarlyTransportSupported : 1;
    // Tunneling Bw optimization
    DDU8 PanelReplayBwOptimization : 2;
    DDU8 Reserved : 1;

    // Sets the grid pattern granularity in the Y direction.

    DDU8 SuYGranularity;
    DDU8 SuYGranularityExtendedCapability1;
    DDU8 SuYGranularityExtendedCapability2;

    // Sets the grid pattern granularity in the X direction.
    DDU16 SuXGranularity;

    // UnlockMode
    DD_BOOL UnlockModeSupport;

    // PanelReplay SU VSC SDP Early notification
    DD_BOOL PanelReplaySuVscSdpEarlyScanlineRequired;
} DD_PANEL_REPLAY_CAPS;

typedef enum _DD_EXT_SDP_FRAMEWORK_VERSION
{
    DD_EXT_SDP_FRAMEWORK_VERSION_0 = 0, // Default Version
    DD_EXT_SDP_FRAMEWORK_VERSION_1
} DD_EXT_SDP_FRAMEWORK_VERSION;

typedef struct _DD_IBOOST_SETTINGS
{
    DD_BOOL IsIBoostEnabled; // iBoot feature
    DDU8 IBoostMagnitude;    // iBoost Magnitude; Platform dependant enum
} DD_IBOOST_SETTINGS;

typedef struct _DD_DP_DATA
{
    DD_PORT_CONNECTOR_TYPE PortConnectorType; // Port Connector Type Normal port/USB-C/Thunderbolt port
    DDU8 DpPortTraceLength;                   // DP port trace length; This field is platform specific, See Vbt Spec for definitions
    DD_BOOL IsSscEnabled;                     // SSC enabled/disabled
    DD_BOOL IsSscEnabledForDongles;           // SSC enabled/disabled for dongles

    DD_DP_AUX_CHANNEL_TYPE AuxChannelNum; // derived from VBT AUX_CHANNEL_TYPE

    DD_DP_REDRIVER_PARAMS DpRedriverParams; // Redriver Settings

    DD_IBOOST_SETTINGS IBoostSettings; // I-boost settings

    DD_BOOL IsDscDisabled; // VESA DSC compression disabled?

    DD_BOOL UsePlatformMaxSupportedDpLinkRate; // Use Platform supported max link rate
    DDU32 MaxDpLinkRateMbps;                   //  DP Maximum Link Rate In Mbps, used to limit maximum link rate possible for DP
    DD_LANE_WIDTH MaxDpLaneCount;              //  DP Maximum Lane Count from VBT
    DD_BOOL IsVswingOverride;                  // VSwing Override from VBT Table; 0 - No Use Default VSwing, 1 - Yes, Use Vswing from VBT
    DD_BOOL IsLttprNtModeEnabled;              // LTTPR NT mode support on port as per VBT.1 = Enable, 0 = Disable.
} DD_DP_DATA;

typedef struct _DD_HDMI_DATA
{
    DDU8 DdcPinPairIndex;          // GMBUS pin pair Index from VBT to read HDMI/DVI EDID/panel params. Values are platform independant from CNL
    DDU8 HDMILSIndexValue;         // HDMI Level Shifter Index; Platform dependant structure
    DD_HDMI_DATARATE HDMIDataRate; // HDMI data rate defines of type HDMI_DATARATE

    DD_IBOOST_SETTINGS IBoostSettings; // I-boost settings
} DD_HDMI_DATA;

/**
 * @brief Stored PSR specific Config set in VBT.
 *
 */
typedef struct _DD_PSR_STATIC_CAPS
{
    DDU8 IsPsrSupported : 1; // Updated based on VBT, DPCD, Regkey and Ftr
    DDU8 IsSuSupported : 1;  // Updated based on VBT, DPCD, Regkey and Ftr
    DDU8 IsSinkPsrCapable : 1;
    DDU8 PsrIdleFrameThreshold;
    DD_LINES_TO_WAIT LinesNeededForLinkStandby;
    DD_CLOCK_RECOVERY_WAKEUPTIME ClockRecoveryTime;
    DD_PSR1_CHANNEL_EQ_WAKEUPTIME Psr1ChannelEqualizationTime;
    DD_PSR2_CHANNEL_EQ_WAKEUPTIME Psr2ChannelEqualizationTime;
} DD_PSR_STATIC_CAPS;

typedef struct _DD_EDP_DATA
{
    DD_BOOL IsDualPipeEdpEnabled;   // Dual pipe eDP support
    DD_PORT_TYPE SecondaryPortType; // Secondary eDP port type when Dual pipe eDP support is enabled

    DD_EDP_SWING_TABLE EdpSwingTableSelector; // eDP Swing Table selection
    DD_PSR_STATIC_CAPS PsrStaticCaps;         // PSR config params from VBT

    DD_BOOL IsFltParamsfromVbtUsed; // Option to indicate if to use eDP Fast Link Training params from VBT
    DD_EDP_FLT_PARAMS EdpFltParams; // eDP FLT parameters
    DD_BOOL IsValidFLTParams;       // This flag is used to check if the FLT params are valid after Max Link Rate/Lane Count VBT restriction
    DDU32 MaxEdpLinkRateMbps;       // Max eDP link rate from VBT
    DD_BOOL UsePlatformMaxSupportedDpLinkRate;

    DD_BOOL IsEdpT3OptimizationEnabled; // T3 optimization to be enbabled for eDP

    // DD_BOOL               IsEdpFullLTParamsUsed;  // Option to indicate if to use eDP Full Link Training start params from VBT
    // EDP_LT_START_PARAMS   steDPStartParams;       // eDP Full link training parameters
    DD_EDP_APICAL_PARAMS ApicalParams; // eDP apical display params
    DD_BOOL IsVswingOverride;          // VSwing Override from VBT Table; 0 - No Use Default VSwing, 1 - Yes, Use Vswing from VBT
    DD_BOOL IsEdp4k2kHoblEnabled;      // Edp 4k/2k HOBL (Hours Of Battery Life) Feature
    DD_BOOL IsVrrVbtEnabled;
    DD_BOOL IsDscDisabled;
} DD_EDP_DATA;

typedef struct _DD_SEQBLOCK
{
    DDU8 SequenceBlockVersion;
    DDU32 SequenceBlockSize;
    DDU8* pSequence[SEQ_MAX]; // Pointer array of all sequences
} DD_SEQBLOCK;

/**
 * @brief  Describes the source of edid.
 */
typedef enum _DD_EDID_TYPE
{
    DD_EDID_DEFAULT = 0,
    DD_EDID_INF,
    DD_EDID_FAKE,
    DD_EDID_OS,
    DD_EDID_VBT,
    DD_EDID_BIOS,
    DD_EDID_IGCL_SUPPLIED
} DD_EDID_TYPE;

/**
 * @brief Describes the LFP panel position.
 */
typedef enum _DD_PANEL_POSITION
{
    INSIDE_SHELL = 0,
    OUTSIDE_SHELL,
    RSVD_PANEL_POSITION,
} DD_PANEL_POSITION;

/**
 * @brief Describes the type of Descriptor : EDID / DID1.x / DID2.0.
 */
typedef enum _DD_DESCRIPTOR_TYPE
{
    DD_DESCRIPTOR_NONE = 0,
    DD_DESCRIPTOR_EDID,
    DD_DESCRIPTOR_DID_1_3,
    DD_DESCRIPTOR_DID_2_0,
    DD_DESCRIPTOR_DID_2_1,
    DD_DESCRIPTOR_MAX,
} DD_DESCRIPTOR_TYPE;

#define MAX_MIPI_DSI_PORTS 2

// EDID Compliance test 4.7.3.2 passes 10 block EDID (1 Base block + 9 Extension Block) and source is expected to parse the EDID. Adding 1 additional extn block to accommodate future change.
#define MAX_EDID_EXTENSIONS_SUPPORTED 10

#define MAX_EDID_DTD_BLOCKS 4

#define MONITOR_NAME_LENGTH 13 // Should be same as Edid_structs.h definition

typedef struct _DD_LACE_FEATURE_CAPS
{
    DD_BOOL IsDisplayLaceSupported;
    DD_BOOL IsDisplayLaceEnabled;
    DDU32 DisplayLaceAggressivenessLevelProfile;
} DD_LACE_FEATURE_CAPS;

typedef struct _DD_DPST_CAPS
{
    DD_BOOL IsDpstSupported;
    DDU32 DpstLevel;
    DD_BOOL IsDitheringSupported;
} DD_DPST_CAPS;

typedef struct _DD_PC_BRIGHTNESS_OPTIMIZATION_CAPS
{
    DD_BOOL Supported;
    DDU8 AggressivenessLevel;
} DD_PC_BRIGHTNESS_OPTIMIZATION_CAPS;

typedef struct _DD_MIPI_DSI_DATA
{
    DD_MIPI_DSI_MODE MipiDsiMode;                  // Video transfer mode
    DD_MIPI_DSI_LINK_CONFIG MipiDsiLinkConfig;     // Dual link caps
    DD_BOOL BTADisable;                            // BTA feature enable/disable
    DD_LANE_WIDTH LaneCount;                       // Lane count of MIPI DSI panel
    DD_BPC_SUPPORTED PanelBpc;                     // Panel color depth
    DDU64 DsiDataRateHz;                           // DSI data rate in Hz
    DD_BOOL FlipRGB;                               // Flip the order of sending of RGB data to panel
    DD_PORT_TYPE PwmOnOffPort[MAX_MIPI_DSI_PORTS]; // Option to know CABC PWM on/off commands to be sent to port-A/C or both
    DDU32 RequiredBurstModeFreqHz;                 // DSI frequency in Hz to be used in Command mode or when "Burst mode is selected in "ulVideoTransferMode"
    DD_LP_BYTECLK_SEL LPByteClock;                 // Escape clock frequency to be used
    DD_BOOL EoTDisable;                            // EoT to be disabled? (Can we remove this?)
    DD_BOOL ClockStopEnable;                       // Clock stop feature enable/disable
    DD_BOOL LpClockPerFrame;                       // Valid only if Continuous clock mode is enabled. 1 - take clock to stop state once per frame, 0 - Don't take Clock to stop state
    DD_BOOL BlankingPacketDuringBllp;              // 1 - enables sending blanking packet with Link in HS mode during BLLP, 0 - disabled sending blank packet in BLLP
    DD_DPHY_PARAMS DPhyParams;                     // Dphy parameters
    DD_RX_DSC_CAPS SinkDscDecoderCaps;             // MIPI DSI Sink DSC Decoder Caps
    DD_SEQBLOCK MipiDsiSequenceBlock;              // Mipi Dsi sequence Block
    DDU8 PixelOverlapCount;
} DD_MIPI_DSI_DATA;

typedef enum _DD_PC_BRIGHTNESS_OPTIMIZATION_PANEL_TYPE
{
    DD_PC_BRIGHTNESS_OPTIMIZATION_PANEL_TYPE_LCD = 0,
    DD_PC_BRIGHTNESS_OPTIMIZATION_PANEL_TYPE_OLED
} DD_PC_BRIGHTNESS_OPTIMIZATION_PANEL_TYPE;

typedef struct _DD_LFP_PC_FEATURES
{
    DD_LACE_FEATURE_CAPS LaceCaps;
    DD_DPST_CAPS DpstCaps;
    DD_PC_BRIGHTNESS_OPTIMIZATION_CAPS ElpCaps;
    DD_PC_BRIGHTNESS_OPTIMIZATION_CAPS ApdCaps;
    DD_PC_BRIGHTNESS_OPTIMIZATION_CAPS PixOptixCaps;
    DD_PC_BRIGHTNESS_OPTIMIZATION_CAPS OpstCaps;
    DD_PC_BRIGHTNESS_OPTIMIZATION_CAPS CabcCaps;
    DD_PC_BRIGHTNESS_OPTIMIZATION_CAPS XpstCaps;
    DD_PC_BRIGHTNESS_OPTIMIZATION_PANEL_TYPE PanelType;
    DD_BOOL IsTconXpstCoExistenceSupported;
} DD_LFP_PC_FEATURES;

// Used in case there are multiple instances of PWM that needs to be selected/enabled
typedef enum _DD_PWM_CTRL_NUM
{
    DD_PWM_NUM0 = 0,
    DD_PWM_NUM1,
    DD_PWM_NUM_MAX
} DD_PWM_CTRL_NUM;

typedef enum _DD_PWM_TYPE
{
    DD_PWM_TYPE_EXTERNAL = 0,
    DD_PWM_TYPE_I2C,
    DD_PWM_TYPE_INTERNAL,
    DD_PWM_TYPE_RESERVED
} DD_PWM_TYPE;

typedef enum _DD_PWM_CTRL_TYPE
{
    DD_PWM_CTRL_TYPE_UNDEFINED = -1,
    DD_PWM_CTRL_TYPE_INTERNAL_IGD = 2, // PWM source is within display controller
    DD_PWM_CTRL_TYPE_PANEL_PWM = 3, // PWM source is on the display panel
    DD_PWM_CTRL_TYPE_LEGACY_VESA_AUX = 5, // PWM source is legacy VESA AUX
    DD_PWM_CTRL_TYPE_VESA_AUX = 6  // Aux based brightness change as per VESA
} DD_PWM_CTRL_TYPE;

typedef enum _DD_DPST_BKLT_TYPE
{
    DD_BKLT_TYPE_UNDEFINED = 0,
    DD_CCFL_BACKLIGHT,
    DD_LED_BACKLIGHT,
    DD_MAX_BACKLIGHT_TYPE
} DD_DPST_BKLT_TYPE;

typedef enum _DD_PWR_CONS_AGGRESSIVENESS_LEVEL
{
    LEVEL_1 = 0x1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4,
    LEVEL_5,
    LEVEL_6
} DD_PWR_CONS_AGGRESSIVENESS_LEVEL;

typedef struct _DD_BLC_STATIC_CAPS
{
    DD_BOOL IsBlcSupported;
    DD_PWM_CTRL_TYPE PwmType;       // PWM contrller type
    DD_PWM_CTRL_NUM PwmCtrlNum;     // PWM controller number
    DD_BOOL IsPwmPolarityInverted;  // WM *polarity
    DDU32 PWMInverterFrequency;     // (100 * Ref clock frequency in MHz / 2) - 1.
    DDU32 MinBrightness;            // Minimum Brightness
    DDU8 BrightnessPrecisionInBits; // Minimum Brightness precision bits count
    DDU16 HdrDpcdRefreshTimeout;    // Panel HDR DPCD Refresh Timeout
    DD_DPST_BKLT_TYPE BlcBkltType;  // BLC controller type (CCFL/LED), mostly un-used
    DD_PWM_TYPE BlcInverterType;
    DDU32 LfpPostBrightness;
} DD_BLC_STATIC_CAPS;

// Display ID 2.1 Brightness Luminance Range Caps
typedef struct _DD_DID_BRIGHTNESS_LUMINANCE_RANGE_CAPS
{
    DDU16 MaxLuminanceFull;  // Maximum luminance of the display with all pixels programmed to maximum code remains visible (sustainable for longer period)
    DDU16 MaxLuminanceBoost; // Highest possible luminance for a given panel (sustainable for shorter period)
    DDU16 MinLuminanceFull;  // Minimum luminance of the display with all pixels programmed to maximum code remains visible
} DD_DID_BRIGHTNESS_LUMINANCE_RANGE_CAPS;

typedef struct _DD_BLC_FEATURES
{
    DD_BLC_STATIC_CAPS StaticCaps;
    DD_DISPLAY_SINK_BRIGHTNESS_CAPS SinkBrightnessCaps;
    DD_NIT_RANGES NitsRangesInfo;
    DD_DID_BRIGHTNESS_LUMINANCE_RANGE_CAPS BrightnessLuminanceData;
} DD_BLC_FEATURES;

typedef struct _DD_LFP_DATA
{
    DDU8 LfpPanelNum; // Lfp Panel number for Dual LFP cases, its either LFP Panel 0/1
    // Parameters required for Fake EDID/override EDID caps
    DDU8 PanelName[MONITOR_NAME_LENGTH];       // Panel name string
    DD_PNP_ID PnpID;                           // PnP ID for the panel
    DD_BPC_SUPPORTED ColorDepth;               // Panel color depth
    DD_CHROMA_AND_LUMA_DATA ChromaAndLumaData; // Data for color primries and Luminance

    // For downscaling feature: Will re-check if this is needed during implementation
    DD_BOOL IsDownScalingEnabled;                    // Panel downscaling enabled/disabled; Need to be evaluated
    DD_PANEL_DIMENSIONS PanelDimensions;             // Panel X/Y resolution; to be used if we support downscaling
    DD_BOOL IsCustomAlignmentSupported;              // Support for Dual LFP Hinge Based Panel Alignment for panels having additional active region(VSRI-4335)
    DD_PANEL_CUSTOM_ALIGN_DATA PanelCustomAlignData; // Dual LFP Hinge Based Panel Alignment Data(VSRI - 4335)

    DD_DPS_PANEL_TYPE DpsPanelType; // DRRS panel type (static/seamless)

    // DD_BOOL              ForceLCDVCCOnS0;           // Force LVD VCC RCR during monitor OFF; Need to be evaluated
    DD_BOOL IsGpuDitheringEnabled; // GPU Dithering got Banding artifacts enabled?
    DD_BOOL IsDrrsEnabled;         // Dynamic Refresh Rate Switching (DRRS)
    DD_BOOL IsDmrrsEnabled;        // Dynamic Media Refresh Rate Switching (DMRRS)
    DDU8 DpsPanelMinRR;            // DRRS panel minimum refresh rate
    DDU8 PixelOverlapCount;        // Pixel Overlap count (applicable for MIPI DSI and eDP)
    DDU8 VbtPanelIndex;            // Panel Index after cross checking with all VBT PNP IDs
    DDU8 RotationOverride;         // it can be used to override any rotation. As of know we are using it for 180 rotation.
    DD_BOOL IsOutsideShell;
    union
    {
        DD_EDP_DATA EdpData;          // EDP specific data
        DD_MIPI_DSI_DATA MipiDsiData; // MipiDsi specific data
    };
    DD_BOOL DualLfpPortSyncEnabled;    // Denotes whether Port Sync Feature is enabled or not.
    DD_LFP_PC_FEATURES PcFeaturesData; // Static Lfp Power features specific data from VBT.
} DD_LFP_DATA;

typedef enum _DD_AFC_STARTUP_INFO
{
    DD_AFC_STARTUP_VALUE_PRESERVED = 0, // Preserve the afc start up bits
    DD_AFC_STARTUP_VALUE_0,             // Overwrite the the afc start up bits value 0
    DD_AFC_STARTUP_VALUE_7,             // Overwrite the afc start up bits to value 7

} DD_AFC_STARTUP_INFO;

typedef struct _DD_ENCODER_INITIALIZATION_DATA
{
    DD_PORT_TYPE Port;              // Derived from VBT_PORT_TYPE
    DD_PORT_CONFIG_TYPE PortConfig; // Derived from DEVICE_CLASS_DEFN
    DD_BOOL IsInternalToChassis;    // When content protection bit and HPD for DP is not set in Device type by VBT, then we consider it as Internal to Chassis.

    DD_BOOL IsInternalDisplay;  // Derived from DEVICE_CLASS_DEFN
    DD_BOOL IsLaneReversed;     // Lane reversal; 0 - Disabled, 1 - Enabled
    DD_BOOL IsHPDSenseInverted; // HPD Sense Invert for Encoder; 0 - No Inversion needed, 1 - Inversion needed

    DD_BOOL IsEdidSupportedPanel; // Is EDID supported panel?
    DD_BOOL IsHotPlugEnabled;     // Is Physical plug/unplug supported, true for all EFPs and detachable LFPs.

    DD_TIMING_INFO TimingInfo;        // Timing Info if panel does not have EDID
    DD_HDMI_FRLRATE MaxFrlRateInMbps; // Max FrlRate for HDMI2.1
    DD_BOOL IsDp2HdmiPconPresent;     // LSCON / DP-HDMI2.1 dongle present
    DD_DP_DATA DpData;                // DP/eDP related static data
    DD_HDMI_DATA HdmiData;            // HDMI related static data

    DD_PPS_DELAY_TABLE PpsDelayTable; // Panel Power sequencing Params from VBT
    DD_BLC_STATIC_CAPS BlcStaticCaps; // Backlight control Params from VBT

    DD_LFP_DATA LfpData;                // LFP encoder specific data
    DD_AFC_STARTUP_INFO AfcStartUpInfo; // exposed only on TGL platform with VBT version >=250
} DD_ENCODER_INITIALIZATION_DATA;

typedef struct _DD_GET_ENCODER_DATA
{
    DD_OUT DDU32 TotalNumIntEncoders;                        // Will return total number of integrated encoders in VBT when "pProtocolPortCtxData" is NULL
    DD_OUT DD_ENCODER_INITIALIZATION_DATA* pEncoderInitData; // Protocol Context Data for all the encoders (TotalNumIntEncoders)
    DD_OUT DDU32 TotalNumVirtEncoders;                       // Will return total number of integrated encoders in VBT when "pProtocolPortCtxData" is NULL
    DD_OUT DDU32 TotalNumWritebackEncoders;                  // Will return total number of writeback encoders when "pProtocolPortCtxData" is NULL
    DD_OUT DDU32 TotalNumDynamicPortEncoders;                // Will return total number of Dynamic port encoders when "pProtocolPortCtxData" is NULL
    DD_OUT DDU8 NumOfInternalDisplays;                       // Will return the total number of internal displays in the system.
    DD_IN_OUT DD_BOOL IsEdpDisplayPresent[DD_MAX_INTERNAL_DISPLAYS];
    DD_IN DD_BOOL IsBlcOnExternalSupported;
    DD_IN DD_PORT_TYPE BlcOverridePort;
} DD_GET_ENCODER_DATA;

typedef struct _DD_TARGET_DETECT_ARGS
{
    DD_IN DD_TARGET_DETECT_TYPE TargetDetectType;
    DD_IN DDU32 TargetId;        // valid only if target detect type is DETECT_ONE
    DD_IN DD_PORT_TYPE PortType; // valid only if target detect type is DETECT_ALL_ON_PORT
    DD_IN DD_TARGET_DETECT_MODE TargetDetectMode;
    DD_OUT DD_BOOL HasPhysicalDisplay;
    DD_OUT DD_BOOL IsOpmNotificationRequired;
    DD_OUT DD_BOOL IsConnectionStatusChanged;
} DD_TARGET_DETECT_ARGS;

typedef struct _DD_CUSTOM_VSWING_DATA
{
    /** Defines the total number of Vswing tables in block. */
    DDU8 NumOfTables;
    /** Defines the number of columns in each table. */
    DDU8 NumOfColumns;
    /** The below multi-dimensional array will define NumOfTables * 10 * NumOfColumns DWORDS of space.
    We have 10 DWORDS out of which first 9 will be used for DP and 10th for HDMI. */
    DDU8* pVswingTables;
} DD_CUSTOM_VSWING_DATA;

// Static encoder Final data structures - END

//----------------------------------------------------------------------------
//
// Static encoder data structures: Related enums and structures - END
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
// VBT/Opregion Interface structures - START
//
//----------------------------------------------------------------------------

//  VBT Platform Parameters
typedef struct _DD_VBT_PLATFORM_PARAMS
{
    // Below data comes from VBT
    DD_BOOL IsHeadlessSupportForKVMREnabled;   //
    DD_BOOL Is180DegreeRotationEnabled;        //
    DD_BOOL IsEmbeddedPlatform;                //
    DD_BOOL IsDisplayDisabledPlatform;         //
    DDU8 VBIOSMinorVersion;                    //
    DDU16 MaxBSModeXRes;                       // Max X resolution that can be supported by legacy displays (e.g. EDID less CRTs)
    DDU16 MaxBSModeYRes;                       // Max Y resolution that can be supported by legacy displays (e.g. EDID less CRTs)
    DDU8 MaxBSModeRRate;                       // Max RR that can be supported by legacy displays (e.g. EDID less CRTs)
    DD_BOOL IsCDClockChangeSupported;          // Dynamic CD clock change support
    DD_BOOL IsI2IDisplayShiftSupported;        // Runtime eDP Display shift between Intels' iGfx and Intels' dGfx supported: VSRI-5313
    DD_BOOL IsI2ODisplayShiftSupported;        // Runtime eDP Display shift between Intels' iGfx and 3rd Party dGfx supported: VSRI-5499
    DD_BOOL IsOsAssistedDisplayShiftSupported; // Runtime eDP Display shift between Intel's GPU and Intel's/3rd Party GPU via OS assisted way

} DD_VBT_PLATFORM_PARAMS;

// BLC Related
#define DD_BLC_PWM_LOW_PRECISION_FACTOR 100
#define DD_BLC_PWM_HIGH_PRECISION_FACTOR 1000
#define DD_BLC_MAPPING_ERROR_TOLERANCE 1
#define INTEGRATED_BRIGHTNESS_UNINITIALIZED DDMAXU32

// DSM related definitions
typedef struct _DD_DSM_CB_ARGS
{
    DD_DSM_CB_TYPE DsmFunctionCode;
    DDU32 InputArgument;
    DD_DSM_OUTPUT_PARAM OutputArgument;
} DD_DSM_CB_ARGS;

//----------------------------------------------------------------------------
//
// VBT/Oprgion Interface structures - END
//
//----------------------------------------------------------------------------

typedef enum _DD_DISPLAY_SHIFT_STATUS
{
    DD_NO_DISPLAY_SHIFT = 0,                  // No display shift supported, This is set to true only of VBT support and _DSM support are successful
    DD_DISPLAY_SHIFT_ENABLE_SEQ_IN_PROGRESS,  // Current adapter is going to get the MUX/eDP Display ownership and display shift is in progress for the same
    DD_DISPLAY_SHIFT_DISABLE_SEQ_IN_PROGRESS, // Current adapter is going to loose MUX/eDP Display ownership and display shift is in progress for the same
    DD_DISPLAY_SHIFT_MUX_OWNED,               // Current adapter has got the MUX/eDP Display ownership and display shift has completed for the same
    DD_DISPLAY_SHIFT_MUX_NOT_OWNED,           // Current adapter has lost the MUX/eDP Display ownership and display shift has completed for the same
    DD_DISPLAY_SHIFT_MAX,
} DD_DISPLAY_SHIFT_STATUS;

#define DD_IS_MUX_OWNED(MuxStatus) ((DD_DISPLAY_SHIFT_ENABLE_SEQ_IN_PROGRESS == MuxStatus) || (DD_DISPLAY_SHIFT_MUX_OWNED == MuxStatus))
#define DD_IS_MUX_SWITCH_HAPPENING(MuxStatus) ((DD_DISPLAY_SHIFT_ENABLE_SEQ_IN_PROGRESS == MuxStatus) || (DD_DISPLAY_SHIFT_DISABLE_SEQ_IN_PROGRESS == MuxStatus))

typedef enum _DD_DISPLAY_MDM_PANEL_REPORT_STATUS
{
    DD_DISPLAY_MDM_PANEL_STATE_NOT_UPDATED = 0,
    DD_DISPLAY_MDM_PANEL_STATE_DISCONNECTED,
    DD_DISPLAY_MDM_PANEL_STATE_CONNECTED,
} DD_DISPLAY_MDM_PANEL_REPORT_STATUS;

typedef enum _DD_MDM_INFO_POSITION
{
    DD_MDM_INFO_UNINIT_POSITION = 0,
    DD_MDM_INFO_MUX_PRESENCE,              // First call made by OS to report presence of mux. MuxPresent (Data1)
    DD_MDM_INFO_INIT,                      // Driver logging switching mechanism along with all the params used to finalize mdm method
    DD_MDM_INFO_SUPPORT_STATUS,            // logged whenever anyone calls CcdIsOsMdmSupported with internal variables used to determine the support
    DD_MDM_INFO_OS_MUX_STATUS_UPDATE,      // logged when OS calls us to inform the mux status post D0 / during switch. This logs the OS provided params along with internal ownership status
    DD_MDM_INFO_MONITOR_FLAG_UPDATE,       // logged during QCC to indicate if we are setting the extra flag needed to indicate monitor change due to switch
    DD_MDM_INFO_SWITCH_START,              // logged when switch is started. This logs the OS args along with brightness level and mux ownership status
    DD_MDM_INFO_SWITCH_END,                // logged at the end of switch. along with swpsrstate being returned to the OS.
    DD_MDM_INFO_SWITCH_CANCEL,             // logged when switch is cancelled. Logs the OS params along with panel report status & mux ownership status
    DD_MDM_INFO_GET_MUX_STATUS,            // logged when internal functions calls us to get the mux status, saving internal mux ownership status
    DD_MDM_INFO_BLOCK_NOTIFICATION_STATUS, // logging of monitor blocking status in relation to current switch status
} DD_MDM_INFO_POSITION;

typedef struct _DD_DISPLAY_SHIFT_INFO
{
    DISPLAY_SWITCH_METHOD DisplaySwitchMethod;
    DD_BOOL IsIntegratedAdapter;                 // Is current adapter Integrated or Discrete
    DDU32 DisplayShiftTarget;                    // Display Shift enabled Target ID
    DD_DISPLAY_SHIFT_STATUS DisplayMuxOwnership; // Display shift MUX ownership status w.r.t. this adapter
    DD_BOOL IsPanelDataUpdateNeeded;             // ONLY iGfx Returns if Panel data update needed. See above for details
    DD_BOOL IsPanelDataPresent; // ONLY dGfx Returns if Panel data is present in Registry and panel enumerated properly (FALSE - on a fresh driver install before 1st SetPanelDetails call)
    // iGfx always returns TRUE after startdevice
    DD_BOOL IsPanelReadyForSwitch; // ONLY dGfx Returns if Panel is ready for switch. After SetPanelDetails, we do a replug of the panel to OS which can take some time to re-enumerate modes
    // Until then this returns FALSE; iGfx always returns TRUE after startdevice
    DD_BOOL IsFlipBlockThreadQueued;                      // flag to ensure only one WI is queued
    DDU32 FlipBlockTimeinms;                              // Delay in ms to block the flips post setvisibilityCall
    DD_BOOL IsEdpEnumerationNeeded;                       // Is eDP enumeration required
    DD_DISPLAY_MDM_PANEL_REPORT_STATUS PanelReportStatus; //  Mdm tracking of panel status being reported to OS
    DD_BOOL SmoothBrightnessSavedState;                   // save the smooth brightness state to be used post switch completion.
} DD_DISPLAY_SHIFT_INFO;

typedef enum _DD_PANEL_DATA_OP
{
    DD_GET_EDID_DATA = 0,
    DD_SET_EDID_DATA,
    DD_GET_DPCD_DATA,
    DD_SET_DPCD_DATA
} DD_PANEL_DATA_OP;

typedef struct _DD_GET_VRR_STATUS_ARGS
{
    DD_IN PIPE_ID Pipe;      // Filled by DVM
    DD_IN DD_PORT_TYPE Port; // Filled by DVM
    DD_OUT DD_VRR_STATUS VrrStatus;
    DD_OUT DDU32 VrrVtotalPrevious;
    DD_OUT DD_BOOL VrrPushPending;
    DD_IN DD_BOOL IsAlwaysInVrrModeSupported;
} DD_GET_VRR_STATUS_ARGS;

typedef struct _DD_GET_VRR_CONFIG_ARGS
{
    DD_IN PIPE_ID Pipe;
    DD_IN PIPE_ID MasterPipe;
    DD_IN DD_PORT_TYPE Port;
    DD_OUT DD_BOOL IsVrrEnabled;
    DD_OUT DDU32 VrrVmax;
    DD_OUT DDU32 VrrGuardBand;
    DD_OUT DDU32 ContextLatency;
} DD_GET_VRR_CONFIG_ARGS;

typedef struct _DD_VRR_PUSH_ARGS
{
    DD_IN PIPE_ID Pipe;
    DD_IN DD_PORT_TYPE PortId;
} DD_VRR_PUSH_ARGS;

typedef enum _DD_VRR_DCB_COUNTER_OPERATION
{
    DD_VRR_DCB_COUNTER_OPERATION_INVALID = 0,
    DD_VRR_DCB_COUNTER_OPERATION_ENABLE,
    DD_VRR_DCB_COUNTER_OPERATION_DISABLE,
    DD_VRR_DCB_COUNTER_OPERATION_READ,
    DD_VRR_DCB_COUNTER_OPERATION_RESET,
    DD_VRR_DCB_COUNTER_OPERATION_READ_AND_RESET,
} DD_VRR_DCB_COUNTER_OPERATION;

typedef struct _DD_VRR_DC_BALANCE_COUNTER_ARGS
{
    DD_IN PIPE_ID Pipe;
    DD_IN DD_PORT_TYPE PortId;
    DD_IN DD_VRR_DCB_COUNTER_OPERATION Operation;
    DD_OUT DDU32 EvenFrameCount;
    DD_OUT DDU32 OddFrameCount;
    DD_OUT DDU32 EvenFrameLineCount;
    DD_OUT DDU32 OddFrameLineCount;
    union
    {
        DD_OUT DDU8 OverFlowStatus;
        struct
        {
            DD_OUT DDU8 EvenFrameCountOverFlow : 1;
            DD_OUT DDU8 OddFrameCountOverFlow : 1;
            DD_OUT DDU8 EvenFrameLineCountOverFlow : 1;
            DD_OUT DDU8 OddFrameLineCountOverFlow : 1;
            DD_OUT DDU8 Reserved : 4;
        };
    };
} DD_VRR_DC_BALANCE_COUNTER_ARGS;

typedef struct _DD_VRR_UPDATE_PARAMS_ARGS
{
    DD_IN PIPE_ID Pipe;
    DD_IN DD_PORT_TYPE PortId;
    DD_IN DDU32 FlipLineValue; // A non-zero value means FlipLine is programmed to the given value
    DD_IN DDU32 VrrVmax;       // A non-zero value means Vmax is programmed to the given value
} DD_VRR_UPDATE_PARAMS_ARGS;

typedef enum _DD_FLICKER_PROFILE
{
    DD_FLICKER_PROFILE_NONE,
    DD_FLICKER_PROFILE_EXCELLENT,
    DD_FLICKER_PROFILE_COMPATIBLE,
    DD_FLICKER_PROFILE_GOOD,
    DD_FLICKER_PROFILE_CUSTOM
} DD_FLICKER_PROFILE;

typedef struct _DD_MONITOR_FLICKER_PROFILE
{
    DD_PNP_ID PnpId;
    DDU8 PnpIdMaskSizeInBytes; // No. of bytes from Monitor pnpId to be used for comparison.
    DD_VIDEO_OUTPUT_TECHNOLOGY Vot;
    DDU32 BaseRr;
    DD_BOOL IsVrrSupported;
    DD_FLICKER_PROFILE Profile;
    DD_MONITOR_FLICKER_PROFILE_CONFIG FlickerConfig;
} DD_MONITOR_FLICKER_PROFILE;

typedef struct _DD_GET_S3D_STATUS_ARGS
{
    DD_IN PIPE_ID Pipe;
    DD_IN DDU32 Port;
    DD_OUT DD_BOOL S3DEnabled;
    DD_OUT DDU32 S3DHwMode;
} DD_GET_S3D_STATUS_ARGS;

typedef struct _DD_DPS_STATE_DATA
{
    DD_IN PIPE_ID Pipe;                    // Current pipe on which we want to query the state of DPS
    DD_OUT DD_BOOL IsDpsSupported;         // Check if DPS is supported (can be disabled or not valid when eDP is not in the config)
    DD_OUT DD_TARGET_DESCRIPTOR TgtDesc;   // Tgt Desc on which DPS state needs to be transitioned
    DD_OUT DD_BOOL IsDpsTransitionPending; // returned from PC DPS handler if in WAITING state (neither active/inactive)
    DD_OUT DD_BOOL IsLoRr;                 // If DPS state transition is pending, indicates what state RR has to be transitioned to, else indicates current RR state (Hi/Lo)
    DD_OUT DD_BOOL IsPreviousStateLoRr;    // Gets the previous DPS state (Hi or Lo)
} DD_DPS_STATE_DATA;

typedef struct _DD_SEAMLESS_RR_ARGS
{
    DD_TARGET_DESCRIPTOR TargetDescriptor;
    DD_BOOL IsBaseRR; /**< Base RR True means switches original Refreshrate False means lower RR. */
} DD_SEAMLESS_RR_ARGS;

//----------------------------------------------------------------------------
//
// Refresh Rate related structures - START
//
//----------------------------------------------------------------------------

// Sometimes OS doesn't send exact duration for media RR. This number represents the allowed tolerance (in percentage) while
// comparing the convert RR for media RR.
// ex: 24000 * 0.0001 = 2
#define DD_GET_MEDIA_RR_TOLERANCE(MediaRr) (DD_ROUND_DIV((DDU64)(MediaRr), 10000))

// Base Video Timings for VRR and FVA shall have a Refresh Rate of 50 Hz or greater
// Note: Observing tearing issue with multiple Samsung monitors with 50Hz mode. Limiting min base RR for HDMI 2.1 to 60 to avoid the issue.
// Bug: https://hsdes.intel.com/appstore/article/#/15015522454
#define DD_HDMI2_1_VRR_MIN_BASE_RR (60 * DD_1K)

// Base Video Timings for VRR and FVA shall have a VActive of 720 lines or greater
#define DD_HDMI2_1_VRR_MIN_VACTIVE (720)

typedef enum _DD_RR_SWITCH_METHOD
{
    DD_RR_SWITCH_METHOD_NONE = 0,
    DD_RR_SWITCH_METHOD_VRR,        // HW VRR
    DD_RR_SWITCH_METHOD_SW_VTOTAL,  // Vblank Timing Register update through SW
    DD_RR_SWITCH_METHOD_PIXEL_CLOCK // M, N, Tu change
} DD_RR_SWITCH_METHOD;

typedef enum _DD_REFRESH_RATE_MODE
{
    DD_REFRESH_RATE_MODE_NONE,
    DD_REFRESH_RATE_MODE_FIXED,
    DD_REFRESH_RATE_MODE_VARIABLE,
} DD_REFRESH_RATE_MODE;

typedef union _DD_ADAPTIVE_VSYNC_USER_SETTING
{
    DDU32 Value;
    struct
    {
        DDU32 EnableAdaptiveSync : 1;                // Bit 0 - Enable Adaptive VSync
        DDU32 EnableAdaptiveSyncLowFpsSolution : 1;  // Bit 1 - Enable Low FPS solution
        DDU32 EnableAdaptiveSyncHighFpsSolution : 1; // Bit 2 - Enable High FPS solution. TODO: to be removed after UI changes.
        DDU32 UNIQUENAME(Reserved) : 29;             // Bits (3:31)
    };
} DD_ADAPTIVE_VSYNC_USER_SETTING;

typedef struct _DD_VBI_MASKING_PARAMS
{
    DD_BOOL Enable;
    DDU8 VbiMaskingFactor; // 2 for HRR, 2-8 for NRR
} DD_VBI_MASKING_PARAMS;

typedef struct _DD_FIXED_RR_CONFIG
{
    DDU32 FixedRr1000; // Fixed RR value for DRRS/DMRRS/BFR cases
    DD_VBI_MASKING_PARAMS VbiMaskingConfig;
} DD_FIXED_RR_CONFIG;

typedef struct _DD_DC_BALANCING_PARAMS
{
    DD_BOOL IsEnabled;
    DDU32 TemporaryFlipLine;
    DDU8 TemporaryFlipLineFrameCount;
    DDU32 TemporaryVmax;
    DDU8 TemporaryVmaxFrameCount;
} DD_DC_BALANCING_PARAMS;

typedef struct _DD_VARIABLE_RR_CONFIG
{
    DDU32 MinRr1000;
    DDU32 MaxRr1000;
    DDU32 SfditInUs; // Successive Frame Duration Increase Tolerance in micro seconds. 0 - no tolerance.
    DDU32 SfddtInUs; // Successive Frame Duration Decrease Tolerance in micro seconds. 0 - no tolerance.

    // Flag to indicate that OSL is filling the VRR update params. DPL calculation will be skipped in this case.
    // Used in DC balancing path where Vmin/Vmax values are calculated as part of algorithm.
    DD_BOOL IsVrrUpdateParamsAvailable;
    DDU32 Flipline;
    DDU32 Vmax;
    DD_DC_BALANCING_PARAMS Dcb;
} DD_VARIABLE_RR_CONFIG;

// Used to store current or desired refresh rate state for any path.
typedef struct _DD_RR_STATE
{
    DD_REFRESH_RATE_MODE RefreshRateMode;
    DD_RR_SWITCH_METHOD RrSwitchMethod;
    DD_FIXED_RR_CONFIG FixedRrConfig;
    DD_VARIABLE_RR_CONFIG VariableRrConfig;
} DD_RR_STATE;

typedef struct _DD_CMRR_PARAMS
{
    DD_BOOL IsCmrrEnabled;
    DDU64 CmrrM;
    DDU64 CmrrN;
} DD_CMRR_PARAMS;

typedef struct _DD_PROGRAM_VRR_ARGS
{
    DD_BOOL Enable;
    DD_BOOL IsVrrSdpSupported;
    DD_BOOL IsAdaptiveSyncSdpOneLineEarlierSupported;
    DD_BOOL IsModeSetPath;
    DD_BOOL IsAlwaysInVrrSupported;
    DDU32 MinRr;
    DDU32 Vmin;
    DDU32 Vmax;
    DDU32 FlipLine;
    DDU32 FrameFillTimeInScanLines;
    DDU32 VActive;
    DDU32 VsyncStart; // VSync start position relative to Vtotal - Used only for VRR SDP
    DDU32 VsyncEnd;   // VSync end position relative to Vtotal - Used only for VRR SDP
    DDU32 GuardBand;  // Vrr Guardband
    DDU32 Window2Size;
    DD_CMRR_PARAMS CmrrParams;
    DDU8 VicId;
    DDU32 BaseVfront; // used only for HDMI EMP
    DDU32 BaseRrInHz; // used only for HDMI EMP
    DD_DC_BALANCING_PARAMS Dcb;
    DDU8 NumOfJoinedPipes;
    PIPE_ID JoinedPipes[MAX_TILES_SUPPORTED]; // Valid only for pipe joined
} DD_PROGRAM_VRR_ARGS;

typedef struct _DD_SW_VTOTAL_ARGS
{
    DDU32 VTotal;
    DDU32 VSyncStart;
    DDU32 VSyncEnd;
} DD_SW_VTOTAL_ARGS;

typedef struct _DD_PIXEL_CLOCK_RR_ARGS
{
    DD_M_N_CONFIG MntuConfig;
    DDU64 DotClock;
} DD_PIXEL_CLOCK_RR_ARGS;

// RR Switching Args
// Filled in protocol layer and passed to HAL for register programming.
typedef struct _DD_RR_SWITCH_PARAMS
{
    DD_IN PIPE_ID Pipe;
    DD_IN DD_PORT_TYPE Port;
    DD_IN DD_VIDEO_OUTPUT_TECHNOLOGY SinkType;
    DD_IN DD_RR_SWITCH_METHOD RrSwitchingMethod;
    DD_IN DD_BOOL IsVrrUpdateOnly;              // Flag to indicate whether HAL should do a full enable/disable for VRR or just update Vmin/Vmax values.
    DD_IN DD_VBI_MASKING_PARAMS VbiMaskingArgs; // Used for HRR/NRR programming
    DD_IN DD_BOOL IsPsrSupported;
    union
    {
        DD_IN DD_PROGRAM_VRR_ARGS VrrArgs;      // Used for Variable mode or VRR H/W based fixed mode programming
        DD_IN DD_SW_VTOTAL_ARGS VtotalArgs;     // Used for VTotal based RR switching for MIPI and LRR2.5 panels
        DD_IN DD_PIXEL_CLOCK_RR_ARGS ClockArgs; // Used for clock based RR switching.
    };
    DD_OUT DD_BOOL VbiMaskingProgrammed;
} DD_RR_SWITCH_PARAMS;

typedef struct _DD_VARIABLE_RR_CAPS
{
    DD_BOOL IsGamingVrrEnabled;
    DD_BOOL IsLowFpsEnabled;
    DD_BOOL IsHwDcbSupported; // TRUE if H/W supports DC balancing
    DD_BOOL IsPcodeSupported;
    DD_BOOL IsVrrSupportedOutsideMaxRrLimit; // TRUE if VRR is supported over max VRR refresh rate
    DDU32 MinRr1000;
    DDU32 MaxRr1000;
} DD_VARIABLE_RR_CAPS;

typedef struct _DD_FIXED_RR_CAPS
{
    DD_BOOL IsFixedRrSwitchingSupported;
    DD_BOOL IsFullRrRangeSupported; // FALSE for legacy path where limited media RRs are supported
    DD_BOOL IsVbiMaskingSupported;
    DD_BOOL IsCmrrSupported;
    DDU32 MinRr1000;
    DDU32 MaxRr1000;
    DDU8 MaxVbiMaskingFactor;
    DDU8 NumSupportedRr;                                                     // Number of RR present in SupportedRrList
    DDU32 SupportedRrList[MAX_NUM_IDENTITY_TARGET_MODE_PER_SOURCE_MODE / 2]; // (24Hz, 25Hz, 30Hz, 48Hz, 50Hz, 60Hz, MinRr, MaxRr) with fractional RR
    DD_RR_SWITCH_METHOD RrSwitchMethod;
    DD_DPS_PANEL_TYPE DrrsPanelType;
} DD_FIXED_RR_CAPS;

typedef struct _DD_DID_AS_DESCRIPTOR
{
    DD_BOOL IsAvtSupported;
    DDU32 MinRr1000;
    DDU32 MaxRr1000;
    DDU32 SfditInUs;
    DDU32 SfddtInUs;
} DD_DID_AS_DESCRIPTOR;

// RR Switching Caps
typedef struct _DD_RR_SWITCH_CAPS
{
    DDU8 IsVrrSupported : 1;
    DDU8 IsVrrSdpSupported : 1;
    DDU8 IsAlwaysInVrrModeSupported : 1; // Flag to indicate if driver needs to enable VRR during mode set or not. If TRUE, VRR will be always enabled with fixed mode.
    DDU8 IsFractionalRrSupported : 1;
    DDU8 IsAdaptiveSyncSdpOneLineEarlierSupported : 1;
    DDU8 IsFlickerParamsValid : 1; // Flag to indicate if SFDIT and SFDDT values are valid or not.
    DDU8 Reserved : 2;
    DDU32 SfditInUs; // Successive Frame Duration Increase Tolerance value from DID 2.1 Adaptive Sync block. 0 (no-tolerance) for all other cases.
    DDU32 SfddtInUs; // Successive Frame Duration Decrease Tolerance value from DID 2.1 Adaptive Sync block. 0 (no-tolerance) for all other cases.
    DD_VARIABLE_RR_CAPS VariableRrCaps;
    DD_FIXED_RR_CAPS FixedRrCaps;
    DDU32 MaxVtotalInLines;
    DDU8 NumberOfAsDescriptors;
    DD_DID_AS_DESCRIPTOR DidAsDescriptors[MAX_DID_AS_DESCRIPTORS];
} DD_RR_SWITCH_CAPS;

// Filled in target layer and passed to protocol for RR switch param calculation
typedef struct _DD_RR_SWITCH_ARGS
{
    DD_IN PIPE_ID Pipe;
    DD_IN DDU8 NumOfJoinedPipes;
    DD_IN PIPE_ID JoinedPipes[MAX_TILES_SUPPORTED]; // Valid only for pipe joined
    DD_IN DD_TARGET_DESCRIPTOR TargetDesc;
    DD_IN DDU8 BitsPerColor;
    DD_IN DD_RR_SWITCH_CAPS* pActiveRrCaps;
    DD_IN DD_RR_STATE DesiredRrState;
    DD_IN DD_BOOL IsModeSetPath;
    DD_IN DD_TIMING_INFO* pTimingInfo;
    DD_IN DDU64 DotClockForRr;
    DD_IN DDU32 Window2Size;
    DD_OUT DDU32 LastSetVrrVmax;
    DD_OUT DD_BOOL VbiMaskingProgrammed;
} DD_RR_SWITCH_ARGS;

// RR Switching Info
// Filled on mode set path for each path
typedef struct _DD_RR_SWITCH_INFO
{
    DD_RR_SWITCH_CAPS ActiveCaps;
    DD_RR_STATE CurrentRrState;
    DD_RR_STATE DesiredRrState;
    DDU32 LastSetDuration;
    DDU32 LastDurationRcvdInFLip;
    DD_BOOL HrrEnabledStatus;
    DD_BOOL FlipQBasedVbiMaskingStatus;
    DD_BOOL InComplianceMode;
    DDU32 VrrVmax;
    DDU32 VrrVmin;
    DDU32 LastSetVrrVmax;
    DD_VRR_SWC_DATA VrrSwcData; /**< Vrr Data for tracking FPS, Low and High FPS implementation. */
    DD_RR_SWITCH_ARGS VrrScanLineInterruptRrSwitchArgs;
    DD_BOOL IsRrSwitchPendingOnScanLineInterrupt;
    DD_BOOL ScanLineInterruptEnabled;  // Indicates whether Scanline Interrupt has been enabled for RR switching
    DD_PERIODIC_FRAME_ELEM PfnElement; // Element to add into periodic frame ring to register for scanline interrupt
} DD_RR_SWITCH_INFO;

//----------------------------------------------------------------------------
//
// Refresh Rate related structures - END
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
// DisplayInfo parser Interface structures - START
//
//----------------------------------------------------------------------------

// Indicates Scaling type for Tiled display
typedef enum _DD_TILED_SCALING
{
    TILED_SCALING_UNINITIALIZED = 0,
    TILED_SCALING_NO_STRETCH,
    TILED_SCALING_STRETCH_ENTIRE_DISPLAY,
    TILED_SCALING_CLONE_OTHER_DISPLAYS
} DD_TILED_SCALING;

// TBD: Need to review if all this data is used.
typedef struct _DD_TILED_DISPLAY_INFO_BLOCK
{
    DD_BOOL IsValidBlock; // Is Block valid (0x18 bytes of data found)?
    DD_BOOL InSinglePhysicalDisplayEnclosure;
    DD_BOOL IsBezelInfoAvailable;
    DD_TILED_SCALING Scaling; // Tiled display scaling support of type TILED_SCALING_TYPE
    DDU8 TotalNumberOfHTiles;
    DDU8 TotalNumberOfVTiles;
    DDU8 HTileLocation;
    DDU8 VTileLocation;
    DDU32 HTileSizeInPixels;
    DDU32 VTileSizeInLines;
    DDU8 PixelMultiplier;
    union
    {
        DDU32 TileBezelInformation;
        struct
        {
            DDU8 TopBezelsize;    // Top Bezel in pixels = (Pixel Multiplier x Top Bezel Size x 0.1)
            DDU8 BottomBezelsize; // Bottom Bezel in pixels = (Pixel Multiplier x Bottom Bezel Size x 0.1)
            DDU8 RightBezelsize;  // Right Bezel in pixels = (Pixel Multiplier x Right Bezel Size x 0.1)
            DDU8 LeftBezelsize;   // Left Bezel in pixels = (Pixel Multiplier x Left Bezel Size x 0.1)
        };
    };
    DDU8 BlockRevision;
    DDU8 ManufacturerID[3]; // Tiled Display Vendor ID
    DDU8 ProductID[2];      // Tiled Display Product Code
    DDU8 SerialNumber[4];   // Tiled Display Serial Number
} DD_TILED_DISPLAY_INFO_BLOCK;

typedef struct _DD_HF_VSDB_INFO
{
    DDU8 Version;
    union
    {
        DDU32 FtrSupport;
        struct
        {
            DDU32 IsHfVsdbInfoValid : 1;
            DDU32 IsScdcPresent : 1;
            DDU32 IsScdcRrCapable : 1;
            DDU32 IsLte_340Mcsc_Scramble : 1;
            DDU32 IsFrlModeSupported : 1;
            DDU32 IsFapaStartLocationAfter1stHBlank : 1;
            DDU32 IsAllmSupported : 1;
            DDU32 IsFvaSupported : 1;
            DDU32 IsNegativeMVrrSupported : 1;
            DDU32 IsQmsSupported : 1;
            DDU32 IsQmsTfrMinSupported : 1;
            DDU32 IsQmsTfrMaxSupported : 1;
            DDU32 IsMDeltaSupported : 1;
            DDU32 IsDsc1p2Supported : 1;
            DDU32 IsDscAllBppSupported : 1;
            DDU32 IsDscNative420Supported : 1;
            DDU32 IsUhdVicSupported : 1;
            DDU32 IsVrrInfoPresent : 1;
        };
    };
    DDU32 MaxTmdsCharRateCsc;          // Max TMDS Character Rate (in Mega-characters/second/channel) reported in HF-VSDB Block of EDID.
    DDU32 MaxFrlInMbps;                // Max supported BW value: i.e., 04 shows 32 Gbps Max and all below link rates
    DDU8 MaxFrlLaneCount;              // Max No of Lanes for supporting above Link Rate BW
    DSC_BPC_CAPS CompressionBpc;       // Supported compression bpc
    DDU8 MaxDscSlices;                 // MAX DSC slices supported; 0 - no support, > 8 - Reserved, 1-7 - 1, 2, 4, 8 slices (upto 340Mhz/KsliceAdjust), 8, 12, 16 slices (upto  400Mhz/KsliceAdjust)
    DDU32 DscMaxPixelClockPerSliceMHz; // MAX pixel clock supported per slice
    DDU32 MaxDscFrlInMbps;             // Max DSC FRL rate supported; 0 - no support, > 7 reserved, 1-6 - 3, 6 (TMDS), 6 (FRL), 8, 10, 12 Gbps respectively
    DDU8 MaxDscFrlLaneCount;           // Max No of Lanes for supporting above Link Rate BW
    DDU8 DscTotalChunkKBytes;          // Total DSC chunk size supported, the # of bytes is computed as 1024 * (1 + DscTotalChunkKBytes)
} DD_HF_VSDB_INFO;

#define DD_IS_SCDC_PRESENT(HFVsdbInfo) ((HFVsdbInfo).IsHfVsdbInfoValid && (HFVsdbInfo).IsScdcPresent)
#define DD_IS_SCRAMBLING_SUPPORTED(HFVsdbInfo) ((HFVsdbInfo).IsHfVsdbInfoValid && (HFVsdbInfo).IsScdcPresent && (HFVsdbInfo).IsLte_340Mcsc_Scramble)
#define DD_IS_SCDC_RR_CAPABLE(HFVsdbInfo) ((HFVsdbInfo).IsScdcRrCapable)
#define DD_IS_FRL_MODE_SUPPORTED(HFVsdbInfo) ((HFVsdbInfo).IsHfVsdbInfoValid && (HFVsdbInfo).IsFrlModeSupported)

// CE Colorimetry Block
typedef union _DD_CE_COLORIMETRY_TYPE
{
    DDU8 Value;
    struct
    {
        DDU8 XVYCC_601 : 1;
        DDU8 XVYCC_709 : 1;
        DDU8 SYCC_601 : 1;
        DDU8 ADOBE_YCC601 : 1;
        DDU8 ADOBE_RGB : 1;
        DDU8 BT2020_C_YCC : 1;
        DDU8 BT2020_YCC : 1;
        DDU8 BT2020_RGB : 1;
    };
} DD_CE_COLORIMETRY_TYPE;

typedef struct _DD_CE_COLORIMETRY_DATA
{
    DD_CE_COLORIMETRY_TYPE ColorimetryType;
    // DDU8 CeProfileSupported; // Used Mask of CE_PROFILE_SUPPORT
} DD_CE_COLORIMETRY_DATA;

// CE HDR Block
typedef union _DD_CE_HDR_EOTF_TYPE
{
    DDU8 Value;
    struct
    {
        DDU8 TraditionalGammaSDR : 1;
        DDU8 TraditionalGammaHDR : 1;
        DDU8 SmpteST2084 : 1;
        DDU8 FutureEOTF : 1;
        DDU8 Reserved : 4;
    };
} DD_CE_HDR_EOTF_TYPE;

typedef union _DD_CE_HDR_STATIC_METADATA_TYPE
{
    DDU8 Value;
    struct
    {
        DDU8 StaticMetadataType1 : 1;
        DDU8 Reserved : 7;
    };
} DD_CE_HDR_STATIC_METADATA_TYPE;

typedef struct _DD_CE_HDR_STATIC_META_DATA
{
    DD_BOOL HdrMetaDataBlockFound;
    DD_CE_HDR_EOTF_TYPE EOTFSupported;                    // EOTF type supported
    DD_CE_HDR_STATIC_METADATA_TYPE HdrStaticMetaDataType; // HDR Static meta Data type
    DDU8 DesiredMaxCLL;                                   // Max content luminance level
    DDU8 DesiredMaxFALL;                                  // Max frame avereage luminance level
    DDU8 DesiredMinCLL;                                   // Min content luminance level
} DD_CE_HDR_STATIC_META_DATA;

#define VIC_UNDEFINED 0xFF
#define VIC_VGA 0x1
#define MAX_CE_VICID 219

#define IS_VALID_VICID(VicId) ((VicId <= MAX_CE_VICID) && (VicId != 0))

#define IS_MEDIA_RR_MODE_CHECK_NEEDED(pTimingInfo) (pTimingInfo->CeData.VicId[0] <= 128)

// TBD: Need help to group this data appropriately
typedef union _DD_FTR_SUPPORT
{
    DDU64 FeatureSupport;
    struct
    {
        DDU64 IsDigitalInput : 1;                     // Bit 0
        DDU64 IsDisplayIDData : 1;                    // Bit 1; 0 - EDID Data, 1 - Display ID data
        DDU64 IsCeExtnDisplay : 1;                    // Bit 2
        DDU64 IsVideoCapBlockBlockPresent : 1;        // Bit 3
        DDU64 IsYuv420VideoBlockPresent : 1;          // Bit 4
        DDU64 IsHdmiVsdbValid : 1;                    // Bit 5
        DDU64 IsDviSupported : 1;                     // Bit 6
        DDU64 IsDisplayPortSupported : 1;             // Bit 7
        DDU64 IsAudioSupported : 1;                   // Bit 8
        DDU64 IsContinuousFreqSupported : 1;          // Bit 9
        DDU64 IsGTFSupported : 1;                     // Bit 10
        DDU64 IsCVTSupported : 1;                     // Bit 11
        DDU64 IsCVTRedBlankSupported : 1;             // Bit 12
        DDU64 IsS3DLRFramesSupported : 1;             // Bit 13
        DDU64 IsMRLBlockPresent : 1;                  // Bit 14
        DDU64 IsDidAsDataBlockPresent : 1;            // Bit 15
        DDU64 IsFecCapable : 1;                       // Bit 16
        DDU64 IsHdcpCapable : 1;                      // Bit 17
        DDU64 IsAuxBasedEdpHdr : 1;                   // Bit 18
        DDU64 IsHdrSupported : 1;                     // Bit 19
        DDU64 UbrrZrrSupported : 1;                   // Bit 20
        DDU64 UbrrLrrSupported : 1;                   // Bit 21
        DDU64 IsLobfSupported : 1;                    // Bit 22
        DDU64 IsAlrrSupported : 1;                    // Bit 23
        DDU64 IsAudSdpSplittingSupported : 1;         // Bit 24
        DDU64 IsPortSyncSupported : 1;                // Bit 25
        DDU64 IsCommandModeSupported : 1;             // Bit 26
        DDU64 IsDpToHdmi21PconDetected : 1;           // Bit 27
        DDU64 IsPanelOutsideShell : 1;                // Bit 28
        DDU64 IsMipiDualLink : 1;                     // Bit 39
        DDU64 YCbCr422Supported : 1;                  // Bit 30
        DDU64 YCbCr444Supported : 1;                  // Bit 31
        DDU64 IsOnboardPconPresent : 1;               // Bit 32
        DDU64 IsExternalPconPresent : 1;              // Bit 33
        DDU64 IsVscSdpExtForColorimetrySupported : 1; // Bit 34
        DDU64 IsPconDpToHdmi2_1Present : 1;           // Bit 35
        DDU64 IsDrrsEnabled : 1;                      // Bit 36
        DDU64 IsFbcSupported : 1;                     // Bit 37
        DDU64 IsPpsSupported : 1;                     // Bit 38
        DDU64 Reserved : 25;                          // Bits 63:39
    };
} DD_FTR_SUPPORT;

C_ASSERT(sizeof(DDU64) == sizeof(DD_FTR_SUPPORT));

typedef struct _DD_BASIC_DISPLAY_CAPS
{
    DDU8 MonitorName[MONITOR_NAME_LENGTH];
    DDU8 MonitorNameLength; // Used for EELD creation
    DDU8 ELDBlock[DD_ELD_BLOCK_SIZE];
    DD_PNP_ID BaseBlkPnpID;                 // PnpID read from Base Block of EDID / Native DID
    DD_PNP_ID ExtnBlkPnpID;                 // PnpID read from Extn Block of EDID / Native DID, Use ExtnBlkPnpID to parse PnpID from DID Extension blocks
    DD_BOOL IsBaseBlkPnpIDValid;            // PnpID extracted from EDID cannot be 0s and can be marked invalid, and when DID Extension block is not present
    DD_BOOL IsExtnBlkPnpIDValid;            // PnpID extracted from EDID cannot be 0s and can be marked invalid
    DD_COLOR_MODEL ColorModel;              // Used to report to UMD, DD_CB_SRGB or this
    DD_CHROMA_AND_LUMA_DATA ChromaLumaData; // Override data from VBT or EDID
    DDU8 DisplayGamma;
    DD_BPC_SUPPORTED BpcsSupportedForAllModes; // Valid only for EDID 1.4 displays, comes from base block and HDMI VSDB block
    DD_BPC_SUPPORTED BpcsSupportedFor420Modes; // Valid only if HDMI display present and 420 Color depth, comes from HF VSDB block
    DD_SAMPLING_MODE DidSamplingMode;          // Sampling mode from DID Display Interface v1/v2 block
    DDU8 DidMinPixelRateForYCbCr420Modes;      // Minimum pixel rate support for YCbCr420 modes from DID Display Interface v2 block
    DDU64 MaxDotClockSupportedInHz;            // For DP its Max DotClock, for HDMI this is Max TMDS Character Rate
    DDU64 MonitorRangeLimitsMaxPixelClk;       // Baseblock MRL MaximumPixelClk, this is used to prune the baseblock StdModes
    DDU8 HSize;
    DDU8 VSize;
    DDU32 MaxSymbolClockInMHz;
    DD_HDCP_VERSION SystemHdcpVersion;
    DD_DIAG_CONN_TYPE ConnectionType;
    DD_ALPM_MODE AlpmMode;
    DD_ROTATION PanelOrientation;
    DD_BOOL IsDpTunnelingGpuBwMgmtMode;
    DDU8 TunnelUsb4DriverId;
    DD_HDMI_VERSION HdmiDisplayVersion;
    DDU8 DpcdRev;
    DDU8 EdpDpcdRev;
    DD_DIAG_CONN_TYPE PortConnectionInfo;
    DD_PORT_CONNECTOR_TYPE EncoderPortType;
    DD_PORT_CONNECTOR_TYPE DetectedPortConnectorType;
    DD_VIDEO_OUTPUT_TECHNOLOGY DongleDwnStreamPortType;
    DD_PROTOCOL_TYPE Protocol;
    DDU8 LinkCount;
    DD_BOOL IsLogicalPort;
    DDU32 DownstreamPortMaxTmdsCharRateCsc;
} DD_BASIC_DISPLAY_CAPS;

// eDP HDR Capable display capabilities
// TCon capabilities are defined in eDP HDR Aux Interface spec from 0x340-0x343.
// 0x340 - TCon Interface, 0x341 and 0x342 - TCon caps and 0x343 is reserved for future usage.
// eDP display will be HDR capable if 2084Decode and 2020Gamut is supported along with EDID caps
typedef struct _DD_EDP_HDR_DISPLAY_CAPS
{
    DD_BOOL Is2084DecodeSupported;                   // TCon supports 2084 decode?
    DD_BOOL Is2020GamutSupported;                    // TCon supports 2020 gamut?
    DD_BOOL IsPanelToneMappingSupported;             // TCon Tonemapping support indication
    DD_BOOL IsSegmentedBkltSupported;                // Segmented backlight support indication
    DD_BOOL IsSDPForColorimteryAndMetadataSupported; // To override Aux based eDP HDR solution
    DD_BOOL IsSRGBToPanelGamutConversionSupported;   // sRGB to panel gamut support indication
} DD_EDP_HDR_DISPLAY_CAPS;

// Display ID Based HDR Caps
typedef struct _DD_DID_HDR_CAPS
{
    DD_CE_HDR_EOTF_TYPE EOTFSupported; // EOTF type supported
    DD_COLOR_GAMUT GamutSupported;
    DDU16 MaxCLL;  // Max content luminance level for 10% area
    DDU16 MaxFALL; // Max frame avereage luminance level
    DDU16 MinCLL;  // Min content luminance level
} DD_DID_HDR_CAPS;

typedef enum _DD_HDR10P_LOW_LATENCY_MODE
{
    DD_HDR10P_LOW_LATENCY_MODE_DEFAULT = 0, // Default driver policy to enable/disable ALLM. i.e. always enable ALLM except in media cases
    DD_HDR10P_LOW_LATENCY_MODE_DISABLED,    // Disable ALLM
    DD_HDR10P_LOW_LATENCY_MODE_ENABLED,     // Enable ALLM
    DD_HDR10P_LOW_LATENCY_MODE_MAX
} DD_HDR10P_LOW_LATENCY_MODE;

typedef enum _DD_HDR10P_SOURCE_TM
{
    DD_HDR10P_SOURCE_TM_DEFAULT = 0, // Default driver policy. Keep Source TM disabled unless overridden by app
    DD_HDR10P_SOURCE_TM_DISABLED,    // Disable Source TM
    DD_HDR10P_SOURCE_TM_ENABLED,     // Enabled Source Tone mapping
    DD_HDR10P_SOURCE_TM_MAX
} DD_HDR10P_SOURCE_TM;

typedef struct _HDR10_PLUS_CTRL
{
    DD_HDR10P_LOW_LATENCY_MODE LowLatency;
    DD_HDR10P_SOURCE_TM SourceToneMapping;
} HDR10_PLUS_CTRL;

typedef struct _DD_CE_HDR10_PLUS_CAPS
{
    DD_BOOL IsHdr10PlusSupported;
    DD_BOOL IsValidHdr10PlusVsVdbFound; // Indicates if valid HDR10+ VSVDB (Vendor Specific Video Data Block) present in EDID
    DDU8 IEEERegID[3];
    DDU8 ApplicationVersion;          // Application Version, Should be set to 1
    DDU8 FullFramePeakLuminanceIndex; // Full Frame Peak Luminance Index, Range: 0-3
    DDU8 PeakLuminanceIndex;          // Peak Luminance Index, Range: 0-15
    DDU16 MaxFALL;                    // Max frame average luminance level. Derived from FullFramePeakLuminanceIndex and PeakLuminanceIndex
    DDU16 MaxCLL;                     // Max content luminance level. Derived from PeakLuminanceIndex
    HDR10_PLUS_CTRL Hdr10PlusCtrl;
} DD_CE_HDR10_PLUS_CAPS;

typedef union _DD_HDR_PLATFORM_CAPS
{
    DDU32 Value;
    struct
    {
        DDU32 DisplayToneMappingSupported : 1;
        DDU32 MediaToneMappingSupported : 1;
        DDU32 HdrMetaDataSupported : 1;
        DDU32 Reserved : 29;
    };
} DD_HDR_PLATFORM_CAPS;

typedef struct _DD_HDR_CAPS
{
    DD_CE_HDR_STATIC_META_DATA CeEdidData; // From HDMI/DP's CE block
    DD_EDP_HDR_DISPLAY_CAPS EdpData;       // Specific to eDP
    DD_DID_HDR_CAPS DidData;               // HDR Caps based on DID
    DD_CE_HDR10_PLUS_CAPS Hdr10PlusCaps;   // HDR Caps based on CE HDR10+ block
    DD_HDR_PLATFORM_CAPS HdrPlatformCaps;  // This is platform HDR caps, set per Gen HAL support
    DD_CE_COLORIMETRY_DATA CeColorimetryData;
} DD_HDR_CAPS;

typedef union _DD_HDMI_CE_AVI_CAPS
{

    DDU16 Value;
    struct
    {
        DDU16 GraphicsContent : 1;
        DDU16 PhotoContent : 1;
        DDU16 CinemaContent : 1;
        DDU16 GameContent : 1;
        DDU16 UnderscansITFormats : 1;    // Indicates underscan behavior of IT formats
        DDU16 CEScanBehavior : 2;         // Indicates scan behavior of CE mode, of type CEA_SCAN_BEHAVIOR
        DDU16 ITScanBehavior : 2;         // Indicates scan behavior of IT mode, of type CEA_SCAN_BEHAVIOR
        DDU16 PTScanBehavior : 2;         // Indicates scan behavior of Preferred mode, of type CEA_SCAN_BEHAVIOR
        DDU16 IsQuantRangeSelectable : 1; // Indicates if RGB Quantization Range can be overridden
        DDU16 Rsvd : 4;
    };
} DD_HDMI_CE_AVI_CAPS;

C_ASSERT(sizeof(DD_HDMI_CE_AVI_CAPS) == sizeof(DDU16));

// Audio/Speaker allocation blocks required for EELD cration
// This has data mostly from Audio Data block and Speaker allocation block
typedef struct _DD_EDID_AUDIO_CAPS
{
    DD_BOOL IsLpcmSadFound;
    DDU8 NumSADBytes;
    CEA_861B_ADB LpcmSad;                          // Keeping a seperate copy of LPCM SAD as it is used to prune Audio modes for EELD
    CEA_861B_ADB SadBlock[DD_MAX_SAD_BLOCK_COUNT]; // corresponding to 14 possible AUDIO_FORMAT_CODES
    DDU8 SpeakerAllocationBlock;                   // Speaker allocation block
} DD_EDID_AUDIO_CAPS;

typedef struct _DD_DTD_MODE_INFO
{
    DDU64 DotClockInHz; // Pixel clock in Hz
    DDU32 HActive;
    DDU32 VActive;
    DDU32 VRoundedRR; // Refresh Rate
} DD_DTD_MODE_INFO;

typedef struct _DD_MSO_DISPLAY_CAPS
{
    DD_BOOL IsMsoCapable;
    DDU8 NumOfLinks; // 0: not segmented, 2: 2 SST Links, 4: 4 SST Links
    DDU8 NumLanes;
    DDU8 OverlapPixelCount;
    DD_BOOL IsIndependentLink; // 0: all SST links depend on one EDID timing, 1: MSO SST Links are independent
} DD_MSO_DISPLAY_CAPS;

#define DD_MAX_PNPID_BASED_REG_KEY_SIZE 64 // Should be greater than regkey name size + PnP Id size in hex format string

// No of bytes to match in PnPId
#define NO_OF_PNPID_BYTES_TO_MATCH 8

// Brightness value of 100% with x100 precision
#define DD_MAX_BRIGHTNESS_VALUE (10000ul)

#define DD_MAX_BRIGHTNESS_PERCENT_DOUBLE 100.0
#define DD_MAX_BRIGHTNESS_MILLIPERCENT 100000
#define DD_MAX_BRIGHTNESS_PERCENT 100

// Denominator value in the formula MinCLL = MaxFALL * (CV*CV)/(255*255*100)
#define DD_MIN_CLL_EDID_TO_NITS_FACTOR (6502500) // 255*255*100 = 6502500

// Linear conversion of Nits brightness value to percentage value
// PercentBrightness = (TargetNits/MaxPanelNits)* 100
#define DD_BRIGHTNESS_MILLI_NITS_TO_PERCENTAGE(MilliNitsValue, MaxPanelNits) (DDU8)(DD_MILLI_UNIT_TO_UNIT(((MilliNitsValue * 100) / MaxPanelNits)))
#define DD_BRIGHTNESS_MILLI_NITS_TO_MILLI_PERCENTAGE(MilliNitsValue, MaxPanelNits) ((MilliNitsValue * 100) / MaxPanelNits)
#define DD_BRIGHTNESS_MILLI_NITS_TO_MILLI_PERCENTAGE_HIGH_PRECISION(MilliNitsValue, MaxPanelNits) ((MilliNitsValue * 1000) / MaxPanelNits)

// Linear conversion of percentage brightness value to Nits brightness value
// MilliNitsBrightness = 1000 * ((TargetPercent*MaxPanelNits)/100)
#define DD_BRIGHTNESS_PERCENTAGE_TO_MILLI_NITS(PercentageValue, MaxPanelNits) (DD_UNIT_TO_MILLI_UNIT(PercentageValue * MaxPanelNits) / 100)
#define DD_BRIGHTNESS_MILLI_PERCENTAGE_TO_MILLI_NITS(MilliPercentageValue, MaxPanelNits) ((MilliPercentageValue * MaxPanelNits) / 100)
#define DD_BRIGHTNESS_MILLI_PERCENTAGE_TO_MILLI_NITS_HIGH_PRECISION(MilliPercentageValue, MaxPanelNits) ((MilliPercentageValue * MaxPanelNits) / 1000)

// Convert Transition time given in millisecond to number of Phase manager steps
// Number of steps = TransitionTimeSeconds * FramesPerSecond
//                 = (TransitionTimeMilliSeconds * FramesPerSecond) / 1000;
#define DD_BRIGHTNESS_TRANSITION_TIME_TO_FRAMES(TransitionTimeMs, RefreshRate) (DD_MILLI_UNIT_TO_UNIT(TransitionTimeMs * RefreshRate))

typedef enum _DD_LRR_VERSION
{
    DD_LRR_VERSION_UNSUPPORTED = -1, // value if FtrPsr2DrrsCoexistence = 0 or undefined dpcd 314h combination
    DD_LRR_VERSION_INVALID = 0,  // value if 314h dpcd = 0 or if PSR2 is not supported
    DD_LRR_VERSION_1_0,              // Pixel clock based, PSR2 and DRRS/DMRRS can co-exist
    DD_LRR_VERSION_2_0,              // TCON based DRRS, Source VTotal based DMRRS, no PSR2 and DRRS/DMRRS co-existence
    DD_LRR_VERSION_2_5,              // TCON based DRRS, Source VTotal based DMRRS, PSR2 and DMRRS can co-exist
    DD_LRR_VERSION_ALRR,             // Autonomous LRR
} DD_LRR_VERSION;

typedef struct _DD_DP_FEATURE_LIST
{
    union
    {
        DDU8 DpRxFeatures;
        struct
        {
            DDU8 IsVscSdpExtensionForColorimetrySupported : 1;
            DDU8 IsVscExtVesaSdpSupported : 1;
            DDU8 IsVscExtVesaSdpChainingSupported : 1;
            DDU8 AdaptiveSyncSdpSupported : 1;            // Supported from DP v1.4
            DDU8 VscExtSdpFrameworkVersion1Supported : 1; // Supported from DP v2.0
            DDU8 AsSdpOneLineEarlierSupported : 1;
            DDU8 UNIQUENAME(Reserved) : 2;
        };
    };
} DD_DP_FEATURE_LIST;

typedef union _DD_PCON_BPC_CONVERSION_CAPS
{
    DDU8 Caps;
    struct
    {
        DDU8 IsYCbCr420PassthroughSupported : 1;  /** Dongle Supports YCbCr420 Passthrough Byte 3 bit 2 from 80h.*/
        DDU8 IsYCbCr422PassthroughSupported : 1;  /** Dongle Supports YCbCr422 Passthrough Byte 3 bit 3 from 80h.*/
        DDU8 IsYCbCr444toYCbCr422Supported : 1;   /** Dongle Supports YCbCr444 to YCbCr422 conversion Byte 3 bit 4 from 80h.*/
        DDU8 IsYCbCr444toYCbCr420Supported : 1;   /** Dongle Supports YCbCr444 to YCbCr420 conversion Byte 3 bit 5 from 80h.*/
        DDU8 IsRgbBT601toYuv601Supported : 1;     /** Dongle Supports RGB601 to YCbCr601 conversion Byte 3 bit 0 from 80h.*/
        DDU8 IsRgbBT709toYuv709Supported : 1;     /** Dongle Supports RGB709 to YCbCr709 conversion Byte 3 bit 1 from 80h.*/
        DDU8 IsRgbBT2020toYuvBT2020Supported : 1; /** Dongle Supports RGB BT2020 to YCbCr BT2020 conversion Byte 3 bit 2 from 80h.*/
        DDU8 UNIQUENAME(Reserved) : 1;
    };
} DD_PCON_BPC_CONVERSION_CAPS;

// HDMI FRL bandwidth efficiency related
/**
 * @brief Data Bandwidth Efficiency for 16b/18b encoding: 88.88% (for FRL link rates channel encoding is 16b/18b.
 *
 *     HDMI 2.1 has defined additional overhead as it needs to transmit various symbols like SR/SSB, FEC etc...
 *     Max overhead for 3 lane configuration is 2.136% and for 4 lane is 2.184%
 *     Since its not a major difference, for our mode-pruning purposes, we will use 2.184% overhead
 *     Total HDMI 2.1 Link BW efficiency = HDMI 2.1 FRL Link BW efficiency + Overhead
 *     = 88.88 - (2.184 * 88.88 / 100) = 88.88 - 1.95 = 86.93.
 */
#define HDMI_DATA_BW_EFFICIENCY_16B_18B_PER_10000 8693

typedef struct _DP_FRL_LINK_STATUS
{
    DD_BOOL IsHdmiLinkConfigInFrlMode;
    DDU32 HdmiTrainedLinkBwInMbps;
    DDU8 HdmiTrainedLaneCount;
} DP_FRL_LINK_STATUS;

typedef struct _DD_RX_PCON_DFP_CAPS
{
    // MaxFrlLinkRateBw in Mbps per lane. This is decoded value from DPCD considering the lane count as well
    // e.g., If DPCD value is of 2 this will be 6000Mbps with 3 lanes, DPCD value of 3 will be 6000 Mbps with 4 lanes
    DDU32 MaxFrlInMbps;

    union
    {
        DDU8 PconDfpCaps;
        struct
        {
            DDU8 IsSourceControlMode : 1;     // If 0, represents Autonomous mode; Control is with PCON FW
            DDU8 IsConcurrentLinkBringUp : 1; // If 0, "sequential mode" where Link between PCON & Sink is brought up prior to DP LT
            DDU8 IsFRLModeSupported : 1;      // If 0: TMDS Mode, if 1: FRL Mode supported
            DDU8 UNIQUENAME(Reserved) : 5;
        };
    };

    DD_PCON_BPC_CONVERSION_CAPS BpcConvCaps;
    DDU8 MaxHdmiLanes;                  // 3 - when MaxFrlLinkRateIndex = 1, 2 and else it is 4
    DD_BOOL IsPconDpToHdmi2_1_Present;  // If Pcon to HDMI2.1 is present, detects if DPCD bDownStreamPortPresent is 1 & Dfp type is HDMI */
    DD_BOOL IsPconDscEncodingSupported; // If Pcon Supports DSC1.2 encoding, we may need to enable SourceControl Mode(Assuming mode also requires DSC1.2) else not needed
} DD_RX_PCON_DFP_CAPS;

typedef struct _DD_DP_PORT_CAPS
{
    DDU8 HblankExpansionCapable : 1; // Bit 0
    DDU8 HblankReductionCapable : 1; // Bit 1
    DDU8 BufferSizeUnit : 1;         // Bit 2
    DDU8 BufferSizePerPort : 1;      // Bit 3
    DDU8 UNIQUENAME(Reserved) : 4;   // Bit 7:4
    /** ((Value +1) * 32) */
    DDU8 BufferSize;
} DD_DP_PORT_CAPS;

typedef struct _DD_DP_SINK_RESOLUTION_FALLBACK_CAPS
{
    DDU8 Support_VideoFallback_1024x768_60HzMode : 1;  // Bit 0 // As per DP spec, currently 3 modes are supported for Video Fallback, add more modes to this struture when added in DP spec
    DDU8 Support_VideoFallback_1280x720_60HzMode : 1;  // Bit 1
    DDU8 Support_VideoFallback_1920x1080_60HzMode : 1; // Bit 2
    DDU8 UNIQUENAME(Reserved) : 5;                     // Bit 7:3
} DD_DP_SINK_RESOLUTION_FALLBACK_CAPS;

typedef struct _DD_DP_BRANCH_DOWNSTREAM_DFP_DETAILED_CAPS
{
    union // DPCD 00005h for Multi-Stream DP Branch Device
    {
        DDU8 Value;
        struct
        {
            DDU8 DfpPresent : 1;
            DDU8 DetailedCapInfoAvailable : 1; // Bit 1
            DDU8 UNIQUENAME(Reserved) : 6;     // Bit 7:2
        };
    };
    DD_VIDEO_OUTPUT_TECHNOLOGY DFPType;
    DDU8 MaxBitsPerComponent; // DPCD 0081h for all types of DFPs
    union                     // DPCD 00082h for Multi-Stream DP Branch Device
    {
        /** (Value * 2.5) */
        DDU8 MaxTmdsCharClockRate; // for DVI/HDMI DFPs
        /** (Value * 8) */
        DDU8 MaxVgaPixelRate; // for VGA DFP
    };
} DD_DP_BRANCH_DOWNSTREAM_DFP_DETAILED_CAPS;

/**
 * @brief DP Mst Dfp Device Info.
 * TODO: Should this be cached in Dp's Sink context only.
 */
typedef struct _DD_MST_DFP_CAPS
{
    DD_DP_BRANCH_DOWNSTREAM_DFP_DETAILED_CAPS BranchDfpCaps; // DPCD 80h to 83h, 4 bytes of DFP 0
    DD_RX_PCON_DFP_CAPS DpToHdmiPconDfpCaps;                 // DP to HDMI PCON Caps
} DD_MST_DFP_CAPS;

typedef struct _DD_ALLM_ARGS
{
    DD_BOOL Enable : 1;
    DD_BOOL IsAllmSupported : 1;
    DD_TARGET_DESCRIPTOR TgtDesc;
} DD_ALLM_ARGS;

typedef struct _DD_AVI_IF_ARGS
{
    DD_BOOL IsHdrSupported : 1;
    DD_BOOL IsVideoCapBlockBlockPresent : 1;
    DD_BOOL IsCeExtnDisplay : 1;
    DD_BOOL IsAuxBasedEdpHdr : 1;
    DD_BOOL IsVscSdpExtForColorimetrySupported : 1;
    DD_BOOL IsSuSupported : 1;
    DD_BOOL PanelReplaySupported : 1;
    DD_BOOL IsPsrSupported : 1;
    DDU8 DpcdRev;
    DD_HDMI_VERSION HdmiDisplayVersion;
    DD_HDMI_CE_AVI_CAPS HdmiCeCaps;
    DD_EDP_HDR_DISPLAY_CAPS EdpData;
    DD_NITS_BRIGHTNESS_AGGR_TYPE OsNitsBrightnessAggressivenessSetting;
    DD_BOOL IsVesaBasedBrightnessControl;
} DD_AVI_IF_ARGS;

typedef struct _DD_PC_BRIGHTNESS_OPTIMIZATION_FTRS
{
    DD_DPST_CAPS DpstCaps;
    DD_PC_BRIGHTNESS_OPTIMIZATION_CAPS ElpCaps;
    DD_PC_BRIGHTNESS_OPTIMIZATION_CAPS ApdCaps;
    DD_PC_BRIGHTNESS_OPTIMIZATION_CAPS PixOptixCaps;
    DD_PC_BRIGHTNESS_OPTIMIZATION_CAPS OpstCaps;
    DD_PC_BRIGHTNESS_OPTIMIZATION_CAPS CabcCaps;
    DD_BOOL IsTconXpstCoExistenceSupported;
} DD_PC_BRIGHTNESS_OPTIMIZATION_FTRS;

typedef struct _DD_HDMI_CAPS
{
    DD_HF_VSDB_INFO HfVsdbInfo;
    DDU32 HdmiVsdbMaxTmdsClockRate;
    DD_HDMI_CE_AVI_CAPS HdmiCeCaps;
    HDMI_SINK_MAUFACTURER_ID HdmiManufacturerId;
} DD_HDMI_CAPS;

typedef struct _DD_DISPLAYPORT_CAPS
{
    DD_MST_DFP_CAPS MstDfpCaps;
    DD_DP_PORT_CAPS DpPortCaps;
    DD_DP_SINK_RESOLUTION_FALLBACK_CAPS DpSinkCaps;
    DD_BOOL IsSingleStreamSidebandMsgSupport;
    DD_BOOL IsDownStreamPortPresent;
    DD_BOOL IsSscSupported;
} DD_DISPLAYPORT_CAPS;

typedef struct _DD_PSR_PR_CAPS
{
    DD_PSR_DISPLAY_CAPS PsrCaps;
    DD_PANEL_REPLAY_CAPS PrCaps;
} DD_PSR_PR_CAPS;

typedef struct _DD_DISPLAY_CAPS
{
    DD_FTR_SUPPORT FtrSupport;
    DD_BASIC_DISPLAY_CAPS BasicDisplayCaps;
    DD_HDMI_CAPS HdmiCaps;
    DD_DISPLAYPORT_CAPS DpCaps;
    DD_EDID_AUDIO_CAPS CeAudioCaps;
    DD_HDR_CAPS HdrCaps;
    DD_TILED_DISPLAY_INFO_BLOCK TiledDisplayInfo;
    DD_PSR_PR_CAPS PsrPrDisplayCaps;
    DD_MSO_DISPLAY_CAPS MsoDisplayCaps;
    DD_RX_DSC_CAPS SinkDscDecoderCaps;
    DD_RR_SWITCH_CAPS RrSwitchCaps;
    DD_BLC_FEATURES BlcDisplayCaps;
    DD_LACE_FEATURE_CAPS LaceCaps;
    DD_PC_BRIGHTNESS_OPTIMIZATION_FTRS BrightnessOptCaps;
    DD_DTD_MODE_INFO DtdModeInfo[MAX_EDID_DTD_BLOCKS];
    DD_DIAGNOSTIC_INFO DiagnosticInfo;
} DD_DISPLAY_CAPS;

typedef enum _DD_GET_DISPLAY_CAPS_OP
{
    DD_GET_FEATURE_SUPPORT = 0,
    DD_GET_BASIC_DISPLAY_CAPS,
    DD_GET_HDMI_CAPS, // hdmi ver, ceExtnCaps etc...
    DD_GET_DP_CAPS,   // Dpcd ver, mst info, DD_DP_FEATURE_LIST
    DD_GET_AUDIO_CAPS,
    DD_GET_HDR_CAPS,
    DD_GET_TILED_INFO,
    DD_GET_PSR_PR_CAPS,
    DD_GET_MSO_CAPS,
    DD_GET_DSC_CAPS,
    DD_GET_RR_SWITCH_CAPS,
    DD_GET_BLC_CAPS,
    DD_GET_LACE_CAPS,
    DD_GET_BRIGHTNESS_OPTIMIZATION_CAPS, // Elp/Opst/dpst
    DD_GET_MODE_INFO,                    // Hdcp ver, res info
    DD_GET_DIAGNOSTIC_INFO,
    // Add others in future
} DD_GET_DISPLAY_CAPS_OP;

// Get Display caps struct should be a map into original Display caps struct, but as union
typedef struct _DD_GET_DISPLAY_CAPS
{
    DD_IN DD_GET_DISPLAY_CAPS_OP Op;

    // DD_OUT based on OP
    union
    {
        DD_FTR_SUPPORT FtrSupport;
        DD_BASIC_DISPLAY_CAPS BasicDisplayCaps;
        DD_HDMI_CAPS HdmiCaps;
        DD_DISPLAYPORT_CAPS DpCaps;
        DD_EDID_AUDIO_CAPS CeAudioCaps;
        DD_HDR_CAPS HdrCaps;
        DD_TILED_DISPLAY_INFO_BLOCK TiledDisplayInfo;
        DD_PSR_PR_CAPS PsrPrDisplayCaps;
        DD_MSO_DISPLAY_CAPS MsoDisplayCaps;
        DD_RX_DSC_CAPS SinkDscDecoderCaps;
        DD_RR_SWITCH_CAPS RrSwitchCaps;
        DD_BLC_FEATURES BlcDisplayCaps;
        DD_LACE_FEATURE_CAPS LaceCaps;
        DD_PC_BRIGHTNESS_OPTIMIZATION_FTRS BrightnessOptCaps;
        DD_DTD_MODE_INFO DtdModeInfo[MAX_EDID_DTD_BLOCKS];
        DD_DIAGNOSTIC_INFO DiagnosticInfo;
    };
} DD_GET_DISPLAY_CAPS;

typedef struct _DD_GET_DISPLAY_OUTPUT_PROPERTIES
{
    DD_DISPLAY_FEATURES EnabledFeatures; // Enabled features
    DD_BOOL IsDitheringEnabled;          // Is dithering enabled on this display
} DD_GET_DISPLAY_OUTPUT_PROPERTIES;

typedef struct _DD_SET_PSR_PR_CAPS
{
    DD_BOOL IsAlrrSupported;
    DD_BOOL IsVscSdpExtForColorimetrySupported;
    DD_BOOL IsAuxBasedEdpHdr;
    DD_PSR_DISPLAY_CAPS PsrDisplayCaps;
    DD_PANEL_REPLAY_CAPS PanelReplayCaps;
} DD_SET_PSR_PR_CAPS;

typedef enum
{
    DD_S3DHW_FSAUTOMODE = 0,
    DD_S3DHW_FSMANUALMODE = 1,
    DD_S3DHW_STACKEDMODE = 2,
    DD_S3DHW_NONE = 3,
} DD_S3D_HW_MODE;

typedef struct _DD_CUSTOM_MODE_ARGS
{
    DD_IN DDU32 TargetId;               // Display for which custom mode is to be applied
    DD_CUSTOM_SRC_MODE* pCustomSrcMode; // This will point to the variable size data array. (Address = &NumOfModes + sizeof(NumOfModes))
    DD_IN_OUT DDU8 NumOfModes;          // Number of Custom Src Modes to be added/removed/Read.
    // This has to be the last variable in this struct.
    // Variable size data array below. To be used after verifying incoming buffer size.
} DD_CUSTOM_MODE_ARGS;

#define MAX_MODE_ADDITION_ENTRY 6
typedef struct _DD_VBT_MODE_ADDITION_ARGS
{
    DD_SOURCE_MODE_INFO ModeTable[MAX_MODE_ADDITION_ENTRY]; // Vbt has support for only 6 mode entries
    DD_BOOL IsLfp;                                          // Is the request for Lfp
} DD_VBT_MODE_ADDITION_ARGS;

// Interface structure definitions for DisplayInfoParser
typedef DDSTATUS(*PFN_ADDENTRY)(void* pTable, const void* pEntryToAdd, DD_BOOL ForceAdd);

typedef struct _DD_TABLE
{
    DDU32 TableSize;        // number of elements for which memory is allocated
    DDU32 NumEntries;       // number of valid entries in table
    DDU32 EntrySizeInBytes; // size of each entry in Bytes
    DDSTATUS(*pfnAddEntry)(void* pTable, const void* pEntryToAdd, DD_BOOL ForceAdd);
    DD_BOOL(*pfnMatchEntry)(const void* pEntry1, const void* pEntry2, DD_BOOL IsWireformatMatchNeeded);
    void (*pfnReplaceEntry)(void* pEntryToReplace, const void* pNewEntry);
    void* pEntry; // contains an array of entries
} DD_TABLE;

typedef struct _DD_CHECK_TOPOLOGY_FLAGS
{
    union
    {
        DDU16 Value;
        struct
        {
            DDU16 IsDigitalInput : DD_BITFIELD_BIT(0);
            DDU16 IsAudioSupported : DD_BITFIELD_BIT(1);
            DDU16 IsFecCapable : DD_BITFIELD_BIT(2);
            DDU16 IsPsr1Supported : DD_BITFIELD_BIT(3);
            DDU16 IsPsr2Supported : DD_BITFIELD_BIT(4);
            DDU16 IsPanelReplaySupported : DD_BITFIELD_BIT(5);
            DDU16 IsVrrSupported : DD_BITFIELD_BIT(6);
            DDU16 IsVscExtDipUsed : DD_BITFIELD_BIT(7);
            DDU16 IsGmpDipUsed : DD_BITFIELD_BIT(8);
            DDU16 IsAlwaysInVrrModeSupported : DD_BITFIELD_BIT(9);
            DDU16 IsGamingVrrSupported : DD_BITFIELD_BIT(10);
            DDU16 IsLobfSupported : DD_BITFIELD_BIT(11);
            DDU16 IsAsSdpSupported : DD_BITFIELD_BIT(12);
            DDU16 UNIQUENAME(Reserved) : DD_BITFIELD_RANGE(13, 15);
        };
    };
} DD_CHECK_TOPOLOGY_FLAGS;
C_ASSERT(sizeof(DD_CHECK_TOPOLOGY_FLAGS) == sizeof(DDU16));

typedef struct _DD_CAPS_FOR_CHECK_TOPOLOGY
{
    DD_HDMI_VERSION HdmiDisplayVersion;
    DD_HF_VSDB_INFO HfVsdbInfo;
    DDU32 HdmiVsdbMaxTmdsClockRate;
    DDU8 IsVscSdpExtensionForColorimetrySupported;
    DDU32 MaxTmdsCharRateCsc; // MaxTMDSChar Rate, MIN(DownstreamDfpMaxTmdsCharRate [PCON/Dock], SinkMaxTmdsCharRate)
    DD_RX_DSC_CAPS SinkDscDecoderCaps;
    DD_MSO_DISPLAY_CAPS MsoDisplayCaps;
    DD_MODE_ENUM_RESTRICTIONS Restriction;
    DDU64 PeakModePixelBwMbpsWoTunnelingRestriction;
    DD_MST_DFP_CAPS MstDfpCaps; // Multi-Stream DP Branch Device DFP Caps
    DD_DP_PORT_CAPS DpPortCaps;
    DD_CHECK_TOPOLOGY_FLAGS Flags;
} DD_CAPS_FOR_CHECK_TOPOLOGY;

typedef struct _DD_DISPLAY_MODE_CONFIG
{
    DD_TARGET_DESCRIPTOR TargetDesc;
    DD_CAPS_FOR_CHECK_TOPOLOGY CapsForCheckConfigSupport;
    DD_IN DD_TIMING_INFO TimingInfo;
    DD_IN DD_BOOL IsModeSetPath;
    DD_IN DD_BOOL IsCustomTarget;
    DD_IN DD_SCALING Scaling;
    DD_IN DD_BOOL IsSharpnessRequired;
    DD_OUT DD_BPC_SUPPORTED BPCMask;
    DD_OUT DD_BOOL DscCapable;
    DD_OUT DD_BOOL IsFecCapable;
    DD_OUT DD_BOOL IsPipeJoinedMode;
    DD_OUT DD_MODE_ENUM_RESTRICTIONS ModesPruning;
    DD_PIXELFORMAT PixelFormat;
    DD_BOOL IsFakeTarget;
    DD_OUT DDU32 DelayedVBlankStart; // Updated VBlankStart in scanlines
    DD_OUT DDU32 Window2Size;        // Window2Size in scanlines
    DD_OUT TIMING_FEATURE_SUPPORT FeatureSupport;
} DD_DISPLAY_MODE_CONFIG;

typedef struct _DD_DISPLAY_CONFIG_BW_CHECK
{
    DD_IN DDU32 NumPlanes;
    DD_IN DDU64 DotClockInHz;
    DD_IN DDU32 PixelFormat[MAX_PLANES_PER_PIPE];
} DD_DISPLAY_CONFIG_BW_CHECK;

typedef struct _DD_CHECK_BW_SUPPORT_ARG
{
    DDU32 NumPipes;
    DD_DISPLAY_CONFIG_BW_CHECK PipeConfig[MAX_PHYSICAL_PIPES];
    DD_BOOL IsModeEnumPath;
    DD_OUT DDU32 RestrictedSagvPoints;
    DD_OUT DDU32 RestrictedPSF0Points;
    DD_OUT DD_MODE_ENUM_RESTRICTIONS ModesPruning;
    DDU8 LowestPossibleQGVPointIndex;
} DD_CHECK_BW_SUPPORT_ARG;

typedef struct _DD_CHECK_MODE_CONFIG_SUPPORT_ARG
{
    DDU32 NumPorts;
    DD_OUT DD_BOOL Supported;
    DD_DISPLAY_MODE_CONFIG Config[MAX_PHYSICAL_PIPES]; // Limiting to Physical pipes for now to avoid stack
    // overflow error. Will move back to max possible pipes once
    // list/pre-allocated memory based allocation is implemented.
    DD_OUT DD_MODE_ENUM_RESTRICTIONS ModesPruning;
} DD_CHECK_MODE_CONFIG_SUPPORT_ARG;

typedef struct _DD_CHECK_PATH
{
    DD_IN DDU32 ChildId;
    DD_IN DD_TIMING_INFO* pTimingInfo;
    DD_IN DD_PIXELFORMAT PixelFormat;
    DD_IN DD_BOOL IsModeSetPath;
    DD_IN DD_SCALING Scaling;
    DD_IN DD_BOOL IsSharpnessRequired;
    DD_OUT DD_BPC_SUPPORTED BPCMask;
    DD_OUT DD_BOOL IsPipeJoinedMode;
    DD_OUT DD_BOOL DscCapable;
    DD_OUT DD_BOOL IsFecCapable;
    DD_OUT DDU32 DelayedVBlankStart; // Updated VBlankStart in scanlines
    DD_OUT TIMING_FEATURE_SUPPORT FeatureSupport;
    DD_OUT DDU32 Window2Size; // Window2Size in scanlines
} DD_CHECK_PATH;

typedef struct _DD_CHECK_TOPOLOGY_SUPPORTED_ARG
{
    DD_IN DDU32 NumTargets;
    DD_IN DD_CHECK_PATH CheckPath[MAX_POSSIBLE_PIPES]; // Limiting to Physical pipes for now to avoid stack
    // overflow error. Will move back to max possible pipes once
    // list/pre-allocated memory based allocation is implemented.
    DD_OUT DD_BOOL Supported;
    DD_OUT DD_MODE_ENUM_RESTRICTIONS ModesPruning;
} DD_CHECK_TOPOLOGY_SUPPORTED_ARG;

typedef struct _DD_CHECK_TIMING_SUPPORT_ARG
{
    DD_IN DD_TARGET_DESCRIPTOR TargetDesc;
    DD_IN_OUT DD_TIMING_INFO* pTimingInfo;
    DD_IN DD_CAPS_FOR_CHECK_TOPOLOGY* pCapsForCheckConfig;
    DD_IN DD_BOOL IsModeSetPath;
    DD_IN DD_BOOL IsCustomTarget;
    DD_IN DD_SCALING Scaling;
    DD_IN DD_BOOL IsSharpnessRequired;
    DD_OUT DD_BOOL Supported;
    DD_OUT DD_BOOL IsPipeJoinedMode; // Out parameter describing whether HAL can support the timing using pipe joiner.
    DD_OUT DD_MODE_ENUM_RESTRICTIONS ModesPruning;
    DD_OUT DDU32 DelayedVBlankStart; // Delayed VBlankStart
    DD_OUT DDU32 Window2Size;        // Window2Size in scanlines
    DD_OUT TIMING_FEATURE_SUPPORT FeatureSupport;
} DD_CHECK_TIMING_SUPPORT_ARG;

typedef struct _DD_JOINT_TARGET_PROPERTIES_COMBO
{
    DDU8 NumTargets;    // Indicates Number of Standard targets that were Joined to form the Joined target.
    DDU8 NumHorTargets; // Number of tiles that are Joint Horizontally
    DDU8 NumVerTargets; // Number of tiles that are Joint Vertically
    struct
    { // Bezel Information is mostly used for Collage. For others it should be typically 0.
        DDU32 LeftBezelsize;
        DDU32 RightBezelsize;
        DDU32 TopBezelsize;
        DDU32 BottomBezelsize;
    };
    DDU32 CombinedDesktopWidth;
    DDU32 CombinedDesktopHeight;
} DD_JOINT_TARGET_PROPERTIES_COMBO;

typedef struct _DD_JOINT_TARGET_PROPERTIES_SINGLE
{
    DDU32 TargetId;            // Standard target-id that is participating in the custom-group
    DDU8 HorLocation;          // Standard target's Horizontal Location
    DDU8 VerLocation;          // Standard target's Vertical Location
    DDU32 HorTileSizeInPixels; // Standard target's Horizontal Tile Size In Pixels
    DDU32 VerTileSizeInLines;  // Standard target's Vertical Tile Size In Lines
    DD_TILED_SCALING ScalingCap;
    struct
    { // Bezel Information is mostly used for Collage. For others it should be typically 0.
        DDU8 LeftBezelsize;
        DDU8 RightBezelsize;
        DDU8 TopBezelsize;
        DDU8 BottomBezelsize;
    };
    // Variables for Combined Display
    DD_RECT FbSrc;                  // Rectangle that is a part of the combined desktop framebuffer and rendered to Display[i]
    DD_RECT FbPos;                  // Rectangle in Display[i]'s framebuffer where FbSrc[i] is placed
    DD_ROTATION DisplayOrientation; // Rotation info
    DD_TARGET_MODE_INFO TargetMode; // Target mode info
    DDU32 MaxRRCollageDisjoinedTimingModeIndex;
} DD_JOINT_TARGET_PROPERTIES_SINGLE;

typedef struct _DD_GET_EDID_CAPS
{
    DD_IN DDU8* pEdidOrDisplayIDBuf;    // EDID/Display ID Data (All EDID/Display Blocks are passed at once)
    DD_IN DDU32 BufSizeInBytes;         // EDID/Display ID Size (Total size, e.g. 1 block = 128bytes, 2 blocks = 256 bytes, etc...)
    DD_IN DDU8 NumEdidExtensionsParsed; // Number of extenblock successfully parsed
    DD_OUT DD_DISPLAY_CAPS* pData;      // Mode Table filled by parser with modes found in EDID
} DD_GET_EDID_CAPS;

typedef struct _DD_SINK_EDID_DATA
{
    /**
     * Cached EDID Block.
     * This would have 2 copies, one in DPL detection data and the other one in target sink caps. The copy in detection
     * data will be allocated while EDID read during detection and will be freed as part of DplReleasePortTopologyDetectionData().
     * The copy in target sink caps will be allocated while calling the caps parse API and stored in sink caps inside parse. It would be
     * freed whenever new caps has to be stored due to connection change .
     */
    DDU8* pEDIDBlock;
    DDU8 NumEdidExtensionBlocks;            /**< max 255 extension blocks possible. */
    DD_EDID_TYPE EdidType;                  /**< Edid type like fake, vbt, etc. */
    DDU8 ComputedEdidChecksum;              /**< Computed Edid Checksum of last Edid Block read. */
    DD_DP_RELATIVE_ADDRESS RelativeAddress; /**< RAD for Leaf for MST Scenaio. */
} DD_SINK_EDID_DATA;

/** EDID size in bytes. */
#define DD_GET_EDID_SIZE(EdidData) (((EdidData).pEDIDBlock) ? (((EdidData).NumEdidExtensionBlocks + 1) * EDID_BLOCK_SIZE) : 0)

typedef struct _DD_DETECTED_TARGETS
{
    DD_TARGET_DESCRIPTOR TargetDescriptor;
    DD_SINK_EDID_DATA EdidData;  // Blocks for EDID will be allocted in DPL and cleared in target.
    DD_DISPLAY_CAPS DisplayCaps; /**< Edid / Display ID / DPCD caps. */
    DD_DISPLAY_CONNECTION_EVENTS ConnectionEvent;
    DD_BOOL IsPreviousTileBlockValid; // This is used in MST path for PBN calc purpose
} DD_DETECTED_TARGETS;

typedef struct _DD_DETECTED_TILE_TARGETS
{
    DD_TARGET_DESCRIPTOR TargetDescriptor;
    DD_DISPLAY_CONNECTION_EVENTS ConnectionEvent;
} DD_DETECTED_TILE_TARGETS;

typedef struct _DD_DETECTED_TILE_PKG_TARGETS
{
    DD_OUT DD_BOOL IsCollage;
    DD_OUT DD_JOINT_TARGET_PROPERTIES_COMBO JoinedTargetProperties;        // Combo property for Joined Display Tiled/Collage.
    DD_OUT DD_JOINT_TARGET_PROPERTIES TileProperties[MAX_TILES_SUPPORTED]; // Info for each Tile.
    DD_OUT DD_DETECTED_TILE_TARGETS Tiles[MAX_TILES_SUPPORTED];            // Desciptor of Child targets.
} DD_DETECTED_TILE_PKG_TARGETS;

typedef struct _DD_MCD_TILE_PKG_TARGETS
{
    DD_JOINT_TARGET_PROPERTIES_COMBO JoinedTargetProperties;        // Combo property for Joined Display Tiled/Collage.
    DD_JOINT_TARGET_PROPERTIES TileProperties[MAX_PER_MCD_TARGETS]; // Info for each Tile.
    DDU32 ChildTgtIds[MAX_PER_MCD_TARGETS];                         // Display UID of Child targets.
} DD_MCD_TILE_PKG_TARGETS;

typedef struct _DD_AUDIO_PATH
{
    DD_BOOL IsFilled;
    PIPE_ID PipeIndex;
    DDU8 MonitorName[MAX_MONITOR_NAME_LENGTH];
    DDU8 MonitorNameLength;
    DD_PORT_TYPE Port;
} DD_AUDIO_PATH;

//
// Audio Topology structure for existing the audio capable panels
//
typedef struct _AUDIO_TOPOLOGY
{
    DDU8 NoOfPaths;
    DD_AUDIO_PATH Path[DD_PORT_TYPE_MAX_PHYSICAL_PORT];
} DD_AUDIO_TOPOLOGY;

typedef struct _DD_DISPLAY_ACTIVE_DETECT_ON_PORT_ARGS
{
    DD_IN DD_PORT_TYPE Port;
    DD_IN DDU32 NumTargets;
    DD_IN DD_TARGET_DETECT_MODE DetectMode;
    DD_OUT DD_DETECTED_TARGETS* pDetectedTargets;
    DD_OUT DD_USB4_CAPS Usb4Properties;
    DD_OUT DD_BOOL IsVidPnChildStatusChanged;   // Filled by Target.c
    DD_IN_OUT DDS32 DetectionPendingPostResume; // 1: Delayed Work Item detection for External DP topology post suspend-resume cycle, 0 - no detection
    DD_OUT DD_BOOL IsReturnLastStatus;          // Flag is used to skip the caps update when set it to true. Currently this flag is getting set for LFPs display if no active detection required and for
    // MstPartialDetect if no topology changes to skip the unnecessary target caps update.
} DD_DISPLAY_ACTIVE_DETECT_ON_PORT_ARGS;

typedef struct _DD_GET_EDID_MODES
{
    DD_IN DDU8* pEdidOrDisplayIDBuf;    // EDID/Display ID Data (All EDID/Display Blocks are passed at once)
    DD_IN DDU32 BufSizeInBytes;         // EDID/Display ID Size (Total size, e.g. 1 block = 128bytes, 2 blocks = 256 bytes, etc...)
    DD_IN DDU8 NumEdidExtensionsParsed; // Number of extenblock successfully parsed
    DD_OUT DD_TABLE* pModeTable;        // Mode Table filled by parser with modes found in EDID
    DD_TARGET_DESCRIPTOR TargetDesc;
    DD_IN DD_EDID_TYPE EdidType; // Type of Edid (BIOS/FAKE/Display/Virtual etc)
} DD_GET_EDID_MODES;

typedef struct _DD_INIT_DISPLAY_ARGS
{
    DD_ENCODER_INITIALIZATION_DATA* pEncoderInitData;
    DD_IN DDU8 NumOfInternalDisplays;
} DD_INIT_DISPLAY_ARGS;

//----------------------------------------------------------------------------
//
// DisplayInfo parser Interface structures - END
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
// VDSC related structures - START
//
//----------------------------------------------------------------------------

/**
 * @brief Macro for maximum number of DSC Buffer RC range parameters.
 **/
#define DD_NUM_BUF_RANGES 15

typedef enum _DSC_INSTANCES
{
    DSC_INVALID = -1,
    DSC_0 = 0, ///< DSC instance 0
    DSC_1,           ///< DSC instance 1
    DSC_2,           ///< DSC instance 2 (to be used only with Ultra-Joiner)
    MAX_DSC_INSTANCES
} DSC_INSTANCES;

/**
 * @brief This structure stores the 128 byte PPS data.
 **/
typedef struct _PICTURE_PARAMS_SET
{
    union
    {
        DDU8 DscVersion; // PPS0
        struct
        {
            DDU8 MinorVersion : 4; // Bit 0-3 Major version no
            DDU8 MajorVersion : 4; // Bit 4-7 Minor version no
        };
    };

    union
    {
        DDU16 PictureParamsSetIdentifier; // PPS 1 ,2
        struct
        {
            DDU16 PpsIdentifier : 8; // Bit 0-7 Application-specific  identifier that can be used to differentiate  between  different PPS table
            DDU16 Reserved1 : 8;     // Bit 8-15 Reserved
        };
    };

    union
    {
        DDU8 BPCandLBD; // PPS 3
        struct
        {
            DDU8 LineBufferDepth : 4;  //  Bit  0-3 [1000 = 8 bits, 1001 = 9 bits, 1010 = 10 bits, 1011 = 11 bits, 1100 = 12bits}
            DDU8 BitsPerComponent : 4; // Bits 4-7 [1000 = 8 bits per component,1010 = 10 bits per component,1100 = 12 bits per component]
        };
    };

    union
    {
        DDU16 GeneralPPSParams; // PPS 4,5
        struct
        {
            DDU16 BPPLow : 2;                // Bits 0-1 The target bits/pixel (bpp) rate that is used by the encoder, in steps of 1/16 of a bit per pixel
            DDU16 VBRMode : 1;               // Bit 2 [0 = VBR mode is disabled, 1 = VBR mode is enabled]
            DDU16 Simple422 : 1;             // Bit 3 [0 = 4:4:4 sampling, 1 = 4:2:2 sampling]
            DDU16 ConvertRGB : 1;            // Bit 4 [ 0 =  no conversion required, 1 =  need conversion from RGB to YCoCg-R during encoding]
            DDU16 BlockPredictionEnable : 1; // Bit 5 [0 =  If block prediction is not supported on the receiver,1 =  If block prediction is supported on the receiver]
            DDU16 Reserved2 : 2;             // Bit 6-7 Reserved
            DDU16 BPPHigh : 8;               // Bits 8-15  The target bits/pixel (bpp) rate that is used by the encoder, in steps of 1/16 of a bit per pixel
        };
    };

    DDU16 PictureHeight; // PPS 6,7 [2 bytes for pic height]
    DDU16 PictureWidth;  // PPS 8,9 [ 2 bytes for pic width]
    DDU16 SliceHeight;   // PPS 10,11 [2 bytes for slice height]
    DDU16 SliceWidth;    // PPS 12,13 [2 bytes for slice width]
    DDU16 ChunkSize;     // PPS 14, 15 [2 bytes for Chunk size]

    union
    {
        DDU16 InitialTransmissionDelay; // PPS 16,17
        struct
        {
            DDU16 TransmissionDelayLow : 2;  // Bit 0-1 Application-specific  identifier that can be used to differentiate  between  different PPS table
            DDU16 Reserved3 : 6;             // Bit 2-7 Reserved
            DDU16 TransmissionDelayHigh : 8; // Bit 8-15 Application-specific  identifier that can be used to differentiate  between  different PPS table
        };
    };

    /**
    PPS 18,19: 2 bytes for Decode delay.
    Specifies the number of pixel times that the decoder accumulates data in its rate buffer
    before starting to decode and output pixels.
    **/
    DDU16 IntialDecodeDelay;

    union
    {
        DDU16 InitialScaleValue; // PPS 20, 21
        struct
        {
            DDU16 Reserved4 : 8;    // Bit 0-7 Reserved
            DDU16 InitialScale : 6; // Bit 8-13 Specifies the initial rcXformScale factor value used at the beginning of a slice
            DDU16 Reserved5 : 2;    // Bit 14-15 Reserved
        };
    };

    DDU16 ScaleIncrementInterval; // PPS 22, 23 Specifies the number of group times between incrementing the rcXformScale factor at the end of a slice

    union
    {
        DDU16 ScaleDecrementInterval; // PPS 24, 25
        struct
        {
            DDU16 ScaleDecrementLow : 4;  // Bit 0-3 Specifies the number of group times between decrementing the rcXformScale factor at the beginning of a slice
            DDU16 Reserved6 : 4;          // Bit 4-7 Reserved
            DDU16 ScaleDecrementHigh : 8; // Bit 8-15 Specifies the number of group times between decrementing the rcXformScale factor at the beginning of a slice
        };
    };

    DDU8 Reserved7; // PPS 26

    union
    {
        DDU8 FirstLineBPGOffset; // PPS 27
        struct
        {
            DDU8 BPGOffset : 5; // Bit 0-4 Specifies the number of additional bits that are allocated for each group on the first line of a slice.
            DDU8 Reserved8 : 3; // Bit 5-7 Reserved
        };
    };

    /**
    PPS 28, 29
    Specifies the number of bits (including fractional bits) that are de-allocated for each group, for groups after the first line of a slice.
    If the first line has an additional bit budget, the additional bits that are allocated must come out of the budget for coding the remainder of the slice.
    **/
    DDU16 NflBpgOffset;

    /**
    PPS 30, 31
    Specifies the number of bits (including fractional bits) that are de-allocated for each group to enforce the slice constraint,
    while allowing a programmable initial_offset.
    **/
    DDU16 SliceBpgOffset;

    DDU16 InitialOffset; // PPS 32, 33
    DDU16 FinalOffset;   // PPS 34, 35

    union
    {
        DDU8 FlatnessMinQP; // PPS 36
        struct
        {
            DDU8 MinQP : 5;     // Bit 0-4 Major version no
            DDU8 Reserved9 : 3; // Bit 5-7 Reserved
        };
    };
    union
    {
        DDU8 FlatnessMaxQP; // PPS 37
        struct
        {
            DDU8 MaxQP : 5;      // Bit 0-4 Major version no
            DDU8 Reserved10 : 3; // Bit 5-7 Reserved
        };
    };

    DDU16 RCmodelSize; // PPS 38, 39 Specifies the number of bits within theRC model,

    union
    {
        DDU8 RCEdgeFactor; // PPS 40
        struct
        {
            DDU8 EdgeFactor : 4; // Bit 0-3 Major version no
            DDU8 Reserved11 : 4; // Bit 4-7 Reserved
        };
    };
    union
    {
        DDU8 RCQuanIncrLimit0; // PPS 41
        struct
        {
            DDU8 IncrLimit0 : 5; // Bit 0-4 Major version no
            DDU8 Reserved12 : 3; // Bit 5-7 Reserved
        };
    };
    union
    {
        DDU8 RCQuanIncrLimit1; // PPS 42
        struct
        {
            DDU8 IncrLimit1 : 5; // Bit 0-4 Major version no
            DDU8 Reserved13 : 3; // Bit 5-7 Reserved
        };
    };
    union
    {
        DDU8 RCTargetOffset; // PPS 43
        struct
        {
            DDU8 RCTargetOffsetLow : 4;  // Bit 0-3 Major version no
            DDU8 RCTargetOffsetHigh : 4; // Bit 4-7 Reserved
        };
    };

    DDU8 RCBufferThreshold0;  // PPS 44
    DDU8 RCBufferThreshold1;  // PPS 45
    DDU8 RCBufferThreshold2;  // PPS 46
    DDU8 RCBufferThreshold3;  // PPS 47
    DDU8 RCBufferThreshold4;  // PPS 48
    DDU8 RCBufferThreshold5;  // PPS 49
    DDU8 RCBufferThreshold6;  // PPS 50
    DDU8 RCBufferThreshold7;  // PPS 51
    DDU8 RCBufferThreshold8;  // PPS 52
    DDU8 RCBufferThreshold9;  // PPS 53
    DDU8 RCBufferThreshold10; // PPS 54
    DDU8 RCBufferThreshold11; // PPS 55
    DDU8 RCBufferThreshold12; // PPS 56
    DDU8 RCBufferThreshold13; // PPS 57

    DDU16 RCRangeParameter0;  // PPS 58, 59
    DDU16 RCRangeParameter1;  // PPS 60, 61
    DDU16 RCRangeParameter2;  // PPS 62, 63
    DDU16 RCRangeParameter3;  // PPS 64, 65
    DDU16 RCRangeParameter4;  // PPS 66 ,67
    DDU16 RCRangeParameter5;  // PPS 68, 69
    DDU16 RCRangeParameter6;  // PPS 70, 71
    DDU16 RCRangeParameter7;  // PPS 72 ,73
    DDU16 RCRangeParameter8;  // PPS 74, 75
    DDU16 RCRangeParameter9;  // PPS 76, 77
    DDU16 RCRangeParameter10; // PPS 78, 79
    DDU16 RCRangeParameter11; // PPS 80, 81
    DDU16 RCRangeParameter12; // PPS 82, 83
    DDU16 RCRangeParameter13; // PPS 84, 85
    DDU16 RCRangeParameter14; // PPS 86, 87

    union
    {
        DDU8 GeneralPPSParams2; // PPS 88
        struct
        {
            DDU8 Native422 : 1;  // Bit 0 [0 = Native422 not used, 1 = Native422 used]
            DDU8 Native420 : 1;  // Bit 1 [0 = Native420 not used, 1 = Native420 used]
            DDU8 Reserved15 : 1; // Bits 2-7 Reserved
        };
    };

    union
    {
        DDU8 SecondLineBPGOffsetValue; // PPS 89
        struct
        {
            DDU8 SecondLineBPGOffset : 5; // Bit 0-4 Specifies the number of additional bits that are allocated for each group on the second line of a slice in Native420 mode
            DDU8 Reserved16 : 3;          // Bit 5-7 Reserved
        };
    };

    /**
    PPS 90, 91
    Specifies the number of bits (including fractional bits) that are de-allocated for each group,  that is not in the second line of a slice.
    If the second line has an additional bit budget, the additional bits that are allocated must come out of the budget for coding the remainder of the slice.
    **/
    DDU16 NslBpgOffset;

    // PPS 92, 93
    DDU16 SecondLineOfsAdj; // Used as an offset adjustment for the second line in Native420 mode

    DDU16 Reserved17; // PPS 94, 95
    DDU32 Reserved18; // PPS 96, 97, 98, 99
    DDU32 Reserved19; // PPS 100, 101, 102 , 103
    DDU32 Reserved20; // PPS 104, 105, 106 , 107
    DDU32 Reserved21; // PPS 108, 109, 110 , 111
    DDU32 Reserved22; // PPS 112, 113, 114 , 115
    DDU32 Reserved23; // PPS 116, 117, 118 , 119
    DDU32 Reserved24; // PPS 120, 121, 122 , 123
    DDU32 Reserved25; // PPS 124, 125, 126 , 127
} PICTURE_PARAMS_SET;

/**
 * @brief This structure defines the full 132 byte PPS (header + data) packet.
 **/
typedef union _PPS_SDP
{
    DDU8 PpsDip[PPS_LENGTH + 4];

    struct
    {
        // 4 byte header.
        // As per eDP spec, the PPS header should contain four bytes as below:
        // 00, 10, 7F, 00
        IF_HEADER IfHeader;

        // Payload
        PICTURE_PARAMS_SET PpsPayload;
    };
} PPS_SDP;

/**
 * @brief Configuration for a single RC model range.
 **/
typedef struct _RC_RANGE_PARAMETERS
{
    DDU32 RangeMinQp;     ///< Min QP allowed for this range
    DDU32 RangeMaxQp;     ///< Max QP allowed for this range
    DDU32 RangeBpgOffset; ///< Bits/group offset to apply to target for this group
} RC_RANGE_PARAMETERS;

/**
 * @brief This structure defines the RC range parameters to be computed for a given DSC configuration.
 **/
typedef struct
{
    DDU32 InitialXmitDelay;
    DDU32 FirstLineBpgOffset;
    DDU32 InitialOffset;
    DDU32 FlatnessMinQp;
    DDU32 FlatnessMaxQp;
    DDU32 RcQuantIncrLimit0;
    DDU32 RcQuantIncrLimit1;
    RC_RANGE_PARAMETERS RcRangeParams[DD_NUM_BUF_RANGES];
} RC_PARAMETERS;

/**
 * @brief This structure will contain all the information relevant to the PPS for VDSC.
 * We use this structure for the initial computation of PPS.
 *
 * Reason for having separate structures for PPS (See :: PICTURE_PARAMETER_SET) and computation:
 * We will keep the parameters in DSC_CFG and compute the PPS for different values of slice height.
 * If we use a common structure then code becomes complex to save/restore original values for next round of computation.
 **/
typedef struct _DSC_CFG
{
    DD_BOOL DscEnabled;                                       ///< DSC enabled/disabled
    DDU32 LineBufDepth;                                       ///< Bits / component for previous reconstructed line buffer
    DDU32 RcbBits;                                            ///< Rate control buffer size (in bits); not in PPS, used only in C model for checking overflow
    DDU32 BitsPerComponent;                                   ///< Bits / component to code (must be 8, 10, or 12)
    DD_BOOL ConvertRgb;                                       ///< Flag indicating to do RGB - YCoCg conversion and back (should be 1 for RGB input)
    DDU32 SliceCount;                                         ///< Slice count per line
    DDU32 SliceWidth;                                         ///< Slice width
    DDU32 SliceHeight;                                        ///< Slice height
    DD_BOOL Simple422;                                        ///< 4:2:2 enable mode (from PPS, 4:2:2 conversion happens outside of DSC encode/decode algorithm)
    DD_BOOL Native422;                                        ///< 4:2:2 native mode (no conversion is done)
    DD_BOOL Native420;                                        ///< 4:2:0 native mode (no conversion is done)
    DDU32 PicWidth;                                           ///< Picture width
    DDU32 PicHeight;                                          ///< Picture height
    DDU32 RcTgtOffsetHi;                                      ///< Offset to bits/group used by RC to determine QP adjustment
    DDU32 RcTgtOffsetLo;                                      ///< Offset to bits/group used by RC to determine QP adjustment
    DDU32 BitsPerPixel;                                       ///< Bits/pixel target << 4 (ie., 4 fractional bits)
    DDU32 RcEdgeFactor;                                       ///< Factor to determine if an edge is present based on the bits produced
    DDU32 RcQuantIncrLimit1;                                  ///< Slow down incrementing once the range reaches this value
    DDU32 RcQuantIncrLimit0;                                  ///< Slow down incrementing once the range reaches this value
    DDU32 InitialXmitDelay;                                   ///< Number of pixels to delay the initial transmission
    DDU32 InitialDecDelay;                                    ///< Number of pixels to delay the VLD on the decoder, not including SSM
    DD_BOOL BlockPredEnable;                                  ///< Block prediction range (in pixels)
    DDU32 FirstLineBpgOfs;                                    ///< Bits/group offset to use for first line of the slice
    DDU32 SecondLineBpgOfs;                                   ///< For 4:2:0, bits/group offset to use for the 2nd line of the slice
    DDU32 InitialOffset;                                      ///< Value to use for RC model offset at slice start
    DDU32 XStart;                                             ///< X position in the picture of top-left corner of slice
    DDU32 YStart;                                             ///< Y position in the picture of top-left corner of slice
    DDU32 RcBufThresh[DD_NUM_BUF_RANGES - 1];                 ///< Thresholds defining each of the buffer ranges
    RC_RANGE_PARAMETERS RcRangeParameters[DD_NUM_BUF_RANGES]; ///< Parameters for each of the RC ranges
    DDU32 RcModelSize;                                        ///< Total size of RC model
    DDU32 FlatnessMinQp;                                      ///< Minimum QP where flatness information is sent
    DDU32 FlatnessMaxQp;                                      ///< Maximum QP where flatness information is sent
    DDU32 FlatnessDetThresh;                                  ///< MAX-MIN for all components is required to be <= this value for flatness to be used
    DDU32 FlatnessMod;                                        ///< Possible flatness modification
    DDU32 InitialScaleValue;                                  ///< Initial value for scale factor
    DDU32 ScaleDecrementInterval;                             ///< Decrement scale factor every scale_decrement_interval groups
    DDU32 ScaleIncrementInterval;                             ///< Increment scale factor every scale_increment_interval groups
    DDU32 NflBpgOffset;                                       ///< Non-first line BPG offset to use
    DDU32 NslBpgOffset;                                       ///< Non-2nd line BPG offset to use
    DDU32 SliceBpgOffset;                                     ///< BPG offset used to enforce slice bit constraDDU32
    DDU32 FinalOffset;                                        ///< Final RC linear transformation offset value
    DD_BOOL VbrEnable;                                        ///< Enable on-off VBR (ie., disable stuffing bits)
    DDU32 MuxWordSize;                                        ///< Mux word size (in bits) for SSM mode
    DDU32 ChunkSize;                                          ///< The (max) size in bytes of the "chunks" that are used in slice multiplexing
    DDU32 PpsIdentifier;                                      ///< Placeholder for PPS identifier
    DDU32 SecondLineOfsAdj;                                   ///< Adjustment to offset for 2nd line in 4:2:0 (since chroma has no prediction)
    DDU32 FullIchErrPrecision;                                ///< Use full error precision for ICH selection (1.2 only)
    DDU32 ReplicatedPixelsCount;                              ///< Number of Pixels to be replicated and padded at the end of the final horizontal slice
    DDU32 DscVersionMinor;                                    ///< DSC minor version
    DDU32 DscVersionMajor;                                    ///< DSC major version
    DDU32 NumVdscInstances;                                   ///< number of VDSC engines
} DSC_CFG;

/**
 * @brief Row index based on BPC for accessing RC range parameters as per VESA DSC model.
 **/
typedef enum
{
    ROW_INDEX_INVALID = -1,
    ROW_INDEX_6BPP = 0,
    ROW_INDEX_8BPP,
    ROW_INDEX_10BPP,
    ROW_INDEX_12BPP,
    ROW_INDEX_15BPP,
    MAX_ROW_INDEX
} ROW_INDEX_BPP;

/**
 * @brief Column index based on BPP for accessing RC range parameters as per VESA DSC model.
 **/
typedef enum
{
    COLUMN_INDEX_INVALID = -1,
    COLUMN_INDEX_8BPC = 0,
    COLUMN_INDEX_10BPC,
    COLUMN_INDEX_12BPC,
    COLUMN_INDEX_14BPC,
    COLUMN_INDEX_16BPC,
    MAX_COLUMN_INDEX
} COLUMN_INDEX_BPC;

/**
 * @brief Structure to be passed as argument for programming PPS regsiters of a single DSC Engine.
 **/
typedef struct _PROGRAM_PPS_REGS_ARG
{
    PIPE_ID PipeId;                 ///< Will be used to determine DP VDSC engines
    DD_TARGET_DESCRIPTOR* pTgtDesc; ///< Will be used for DP MST information and port to determine VDSC engines.
    DSC_INSTANCES DscInstance;      ///< DSC Instance to be programmed.
    DSC_CFG* pDscCfg;               ///< DSC_CFG data to be programmed in the PPS registers.
} PROGRAM_PPS_REGS_ARG;

/**
 * @brief Structure to be passed as argument for programming DSS_CTL of a single DSS unit.
 **/
typedef struct _PROGRAM_DSS_CTL_ARG
{
    PIPE_ID PipeId;                            ///< Will be used to determine DP VDSC engines
    DD_PORT_TYPE Port;                         ///< Port on which DSC is enabled
    DDU8 NumVdscInstance;                      ///< To determine whether we are using 1 or 2 engines.
    DD_BOOL IsDualLinkMipiDsi;                 ///< To be used to determine if small joiner is to be enabled.
    DD_BOOL IsChipOnGlass;                     ///< To be used to determine if small joiner is to be enabled.
    DD_BOOL IsDsc;                             ///< To be used to determine if LeftBranchVdscEnable/RightBranchVdscEnable is to be enabled.
    DDU8 OverLap;                              ///< To be used for ChipOnGlass / DualLinkMipiDsi devices.
    DD_BOOL Enable;                            ///< Enable/disable flag
    DD_BOOL IsPipeJoinedMode;                  ///< Pipe joined mode or normal mode flag
    DD_BOOL IsUltraPipeJoinedMode;             ///< Four Pipe joined mode flag.
    DD_BOOL IsPrimaryPipe;                     ///< Primary pipe in pipe joined mode or not.
    DD_BOOL IsUltraJoinerPrimaryPipe;          ///< Ultra Joiner Primary pipe in four pipe joined mode or not.
    DD_MIPI_DSI_LINK_CONFIG MipiDsiLinkConfig; ///< Dual link caps
    DDU32 HActive;                             ///< Active in pixels
    DDU8 NoOfMsoSegments;                      ///< Used to determine the no of segments
    DDU32 ReplicatedPixelsCount;               ///< Number of Pixels to be replicated and padded at the end of the final horizontal slice
    DD_BOOL IsOddPixelRemovalRequired;         ///< Extra Pixel appended by Pipe to be dropped by Splitter when HorizontalSourceSize is Even.
} PROGRAM_DSS_CTL_ARG;

#pragma pack()

//----------------------------------------------------------------------------
//
// VDSC related structures - END
//
//----------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
//
// Protocol data structures - END
//
//----------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
//
// HAL data structures - START
//
//----------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////

typedef struct _DD_INIT_HAL
{
    DD_BOOL Enable;
    DD_SET_ADAPTER_PWR_ARGS* pSetPwrStateArgs; // Power State Args passed to HAL
} DD_INIT_HAL;

typedef enum _DD_DC_STATE_REQUEST_TYPE
{
    DD_DC_STATE_REQUEST_TYPE_UNKNOWN,
    DD_DC_STATE_REQUEST_TYPE_DISALLOW_PRE_AUX,
    DD_DC_STATE_REQUEST_TYPE_RESTORE_POST_AUX,
    DD_DC_STATE_REQUEST_TYPE_SET_DC_STATE,
    DD_DC_STATE_REQUEST_TYPE_MAX,
} DD_DC_STATE_REQUEST_TYPE;

typedef enum _DD_DC_PWR_STATE
{
    DC_PWR_STATE_SET_NONE,
    DC_PWR_STATE_SET_UPTO_DC5,
    DC_PWR_STATE_SET_UPTO_DC6V,
    DC_PWR_STATE_SET_UPTO_DC6,
    DC_PWR_STATE_SET_UPTO_DC9,
    DC_PWR_STATE_SET_MAX
} DD_DC_PWR_STATE;

/***************************************************************
 * @brief  This structure contains the requested DC state to be set, the active pipe and port details.
 ***************************************************************/
typedef struct _DD_DCSTATE_ARGS
{
    DD_DC_PWR_STATE DcState;
    union
    {
        DDU8 PipeInfo; // For single/dual eDPs
        struct
        {
            DDU8 PipeA : 1;     // Bit 0
            DDU8 PipeB : 1;     // Bit 1
            DDU8 Reserved1 : 6; // for future use
        };
    };

    union
    {
        DDU8 PortInfo; // For single/dual MIPIs
        struct
        {
            DDU8 Dsi_0 : 1;     // Bit 0
            DDU8 Dsi_1 : 1;     // Bit 1
            DDU8 Reserved2 : 6; // for future use
        };
    };
} DD_DCSTATE_ARGS;

/***************************************************************
 * @brief  This structure is to be used to Synchronize Execution of DC state programming.
 ***************************************************************/
typedef struct _DD_SYNC_DC_STATE_ARGS
{
    DD_DC_STATE_REQUEST_TYPE RequestType;
    void* pDisplayPowerContext;        // Corresponding GenXXDisplayPowerContext
    DD_DP_AUX_CHANNEL_TYPE AuxChannel; // Valid for DD_DC_STATE_REQUEST_TYPE_DISALLOW_PRE_AUX and DD_DC_STATE_REQUEST_TYPE_RESTORE_POST_AUX only
    DD_DCSTATE_ARGS* pDcStateArgs;     // Valid only for DD_DC_STATE_REQUEST_TYPE_SET_DC_STATE
    void* pDisplayPcContext;
} DD_SYNC_DC_STATE_ARGS;

typedef union _DD_PLANE_FLAGS_FOR_ETL
{
    struct
    {
        DDU32 AlphaBlend : 1;
        DDU32 Decryption : 1;
        DDU32 RendDecomp : 1;
        DDU32 MediaDecomp : 1;
        DDU32 ClearColorEn : 1;
        DDU32 KeySelect : 1;
        DDU32 Reserved : 26;
    };
    DDU32 Value;
} DD_PLANE_FLAGS_FOR_ETL;

//----------------------------------------------------------------------------
//
// Color related data structures - START
//
//----------------------------------------------------------------------------
typedef enum _COLOR_RANGE
{
    FULL_RANGE = 0,
    COLOR_RANGE_16_TO_235 = 1
} COLOR_RANGE;

typedef union _DD_BPP_MASK
{
    // Currently required color depth values : 8, 16, 32, 64
    // If in the future, a new color depth support is required, add it here.
    // In this case you should update MAX_COLOR_BPP & ALLCOLOR_BPP_MASK
    DDU8 BPPMask;
    struct
    {
        DDU8 Is4BPP : 1; // 1: Supported, 0 : no supported.
        DDU8 Is8BPP : 1;
        DDU8 Is16BPP : 1;
        DDU8 Is32BPP : 1;
        DDU8 Is64BPP : 1;
        DDU8 Reserved : 3;
    };
} DD_BPP_MASK;

//----------------------------------------------------------------------------
//
// Color related data structures - END
//
//----------------------------------------------------------------------------

// Dynamic CD Clock
typedef struct _DD_CDCLK_PIPE_ARGS
{
    DDU32 ChildId;               // VidPn Child ID, for CCD/DVM use
    DD_TIMING_INFO* pTargetMode; // VidPn Child Target Mode, for CCD/DVM use only.
    // Members used by HAL.
    DD_TARGET_DESCRIPTOR TgtDesc;
    DDU32 HActive;
    DDU32 VActive;
    DDU64 DotClockInHz;
    DD_SCALING Scaling;
    DD_BOOL IsDSCSupported;
    DDU32 DSCSlicePerLine;
    DD_BOOL IsHDROrWCGEnabled;
    DD_SAMPLING_MODE PinnedSamplingMode; // SamplingMode is of format DD_SAMPLING_MODE associated with the timing
    DD_BPC_SUPPORTED PinnedBpc;          // Contains the Supported BPC Mask of all the sampling modes supported by the mode
    DD_OUT DD_BOOL IsCustomScalingPossible;
} DD_CDCLK_PIPE_ARGS;

typedef enum _CDCLK_CHANGE_TYPE
{
    DD_CDCLK_CHANGE_NOT_NEEDED = 0, // No CdClk Change Needed
    DD_CDCLK_FULL_CHANGE,           // Full CdClk Change with Pll Disable and Enable
    DD_CDLCLK_SEAMLESS_CHANGE,      // Crawling/Squashing
    DD_CDLCLK_2X_DIVIDER_CHANGE     // divider change without Pll Disable and Enable
} CDCLK_CHANGE_TYPE;

typedef struct _CDCLK_CHANGE_ARGS
{
    DD_BOOL IsVirtualDisplayInTopology;
    DDU32 OptimalCdClockInKhz;
    CDCLK_CHANGE_TYPE CdClkChangeType;
    DDU32 WaitForVbiOnChildId; // Enabled Physical target on which we can wait for Pipe VBI, CCD/DVM use.
    PIPE_ID WaitForPipeVBI;
    DDU32 MaxSymbolClockInMHz;
    DD_BOOL IsPostFrequencyDvfsRequired;
    DD_BOOL IsCdClockInit;
} CDCLK_CHANGE_ARGS;

typedef struct _DD_CDCLK_QUERY_CAPS
{
    DD_IN DDU32 NumPath;
    DD_IN DDU32 ActivePipesCount;
    DD_IN DD_CDCLK_PIPE_ARGS CDClkPathArgs[MAX_PHYSICAL_PIPES];
    DD_IN DD_CDCLK_PIPE_ARGS CDClkPipeArgs[MAX_PHYSICAL_PIPES];
    DD_IN DD_BOOL IsExternalDisplayPresent;
    DD_IN_OUT DDU32 OptimalCdClockInKhz;
    DD_OUT CDCLK_CHANGE_TYPE CdClkChangeType;
} DD_CDCLK_QUERY_CAPS;

typedef struct _DD_GET_DOWN_SCALE_SUPPORT_ARGS
{
    DD_IN DDU32 HActive;
    DD_IN DDU32 VActive;
    DD_IN DDU32 PpcMultiplier;
    DD_IN DDU64 PixelClockInKhz;
    DD_IN DDU32 MaxCdClockInKHz;
    DD_OUT DDU64 PixelClockInKhzForCustomScale;
} DD_GET_DOWN_SCALE_SUPPORT_ARGS;

//----------------------------------------------------------------------------
//
// Plane/Pipe/Pll/Port/Watermark related structures - START
//
//----------------------------------------------------------------------------

/**
 * @brief Interface struct to be used for port, pll programming during link training.
 */
typedef struct _DD_ENABLE_PLL_ARGS
{
    DD_PORT_TYPE Port;
    PIPE_ID Pipe;
    DD_VIDEO_OUTPUT_TECHNOLOGY SinkType;
    DD_BOOL IsGenLockSupported;
    DD_GENLOCK_CFG GenLockCfg;
    DD_AFC_STARTUP_INFO AfcStartUpInfo; // exposed only on TGL platform with VBT version >=250

    union
    {
        struct
        {
            DP_LINK_DATA* pLinkData;
            DD_BOOL IsEdpPortSyncEnabled; // Port sync status to be used for Pll disable on eDP
        } Dp;
        struct
        {
            DD_TIMING_INFO* pTimingInfo;
            HDMI_LINK_DATA* pLinkData;
        } Hdmi;
    };

} DD_ENABLE_PLL_ARGS;

typedef struct _DD_ENABLE_PORT_ARGS
{
    DD_PORT_TYPE Port;
    PIPE_ID Pipe;
    DD_VIDEO_OUTPUT_TECHNOLOGY SinkType;
    DD_SAMPLING_MODE SamplingMode;
    DD_CUSTOM_VSWING_DATA* pCustomVswingParams;
    DD_ALPM_MODE AlpmMode;
    union
    {
        struct
        {
            DP_LINK_DATA* pLinkData;
        } Dp;
        struct
        {
            DD_BOOL IsVirtualDvi;
            HDMI_LINK_DATA* pLinkData;
            DDU8 ColorDepth;
            DDU32 SyncPolarity;
            DDU32 PixelReplication;
            DD_PROTOCOL_TYPE TransportTypeEnabled;
            DD_DRIVE_SETTING_TYPE HdmiTxEqSettingType;
        } Hdmi;
        struct
        {
            DD_MIPI_DSI_DATA* pMipiDsiData;
            DD_BOOL PortSyncEnable; // Comes from OS to enable Port Sync Enable feature for MipiDsi/Dual EDP
        } Mipi;
    };

} DD_ENABLE_PORT_ARGS;

typedef struct _DD_SET_LINK_CONFIG_ARGS
{
    DD_LINK_OPERATION_TYPE OpCode;
    DD_PORT_TYPE Port;

    union
    {
        struct
        {
            DD_VIDEO_OUTPUT_TECHNOLOGY SinkType;
            DP_LINK_DATA* pDpLinkData;
            DD_PPS_DELAY_TABLE* pPpsDelayTable;
        } PrepareLTArg; //!< used with OpCode = DP_OP_PREPARE_FOR_LINK_TRAINING
        struct
        {
            DD_DP_TRAINING_PATTERN TrainingPattern;
            DD_BOOL IsUhbrPhyCompliancePattern;
            DDU8 SquarePatternNumber;
            DDU8 NumHdmiLanesInFrlMode;
            DD_HDMI_TRAINING_PATTERN HdmiLinkTrainingPattern[MAX_HDMI_LANES_FRL_MODE]; //!< Per lane Link training pattern to be applied
            PIPE_ID Pipe;
            DP_CHANNEL_CODING_TYPE DpChannelCodingType; // Adding channel encoding type so that it can passed down to lower hal layer based on which we add the delay after sending idle patterns
        } SetTPArg;                                     //!< used with OpCode = DP_OP_SET_LINK_PATTERN
        struct
        {
            DD_VIDEO_OUTPUT_TECHNOLOGY SinkType;
            DDU32 LinkRateMbps;
            DD_DRIVE_SETTING_TYPE DriveSettingType;
            DD_DP_DRIVE_SETTING DriveSettings;
            DD_CUSTOM_VSWING_DATA* pCustomVswingParams;
            HDMI_FFE_PRESET_LEVEL TxFfePresetValue16b18b[MAX_HDMI_LANES_FRL_MODE]; /**< Transmitter FFE(Feed Forward Equalization) Preset Value(0 to 3) for HDMI FRL 16b/18b channel coding. */
            DD_BOOL UseHoblSetting;
            DD_BOOL IsFrlMode;
            DDU32 LaneCount;
        } AdjDrvArg; //!< used with OpCode = DP_OP_ADJ_DRIVE_SETTINGS or HDMI_OP_ADJ_DRIVE_SETTINGS
        struct
        {
            DP_LINK_DATA* pDpLinkData;
            DD_CUSTOM_VSWING_DATA* pCustomVswingParams;
            PIPE_ID Pipe;
            DD_BOOL Enable;
            DDU32 DotClockInMHz; // Used for HDMI port enable. For DP, link rate field in pLinkData shall be used.
        } EnableDisableArg;      //!< used with OpCode = DP_OP_DISABLE_PORT_PLL, DP_OP_ENABLE_PORT
        struct
        {
            PIPE_ID Pipe;
            DD_MIPI_DSI_DATA* pMipiDsiData;
            DD_BOOL PortSyncEnable;  // Comes from OS to enable Port Sync Enable feature for MipiDsi
        } PrepareMipiDsiForCmdTxArg; //!< used with OpCode = MIPI_OP_PREPARE_FOR_CMD_TRANSMIT, MIPI_DSI_OP_TURNOFF_LINK
        struct
        {
            DD_BOOL IsLttpr; // Link Training Tunable PHY Repeaters. 0- Not Present, 1 -Present.
        } SetLttprArg;       //!< used with OpCode = DP_OP_SET_AUX_DELAY
    };
} DD_SET_LINK_CONFIG_ARGS;

typedef struct _DD_ENABLE_FEC_ARGS
{
    DD_BOOL Enable;                 ///< Enable or disable FEC.
    PIPE_ID Pipe;                   ///< Pipe
    DD_TARGET_DESCRIPTOR* pTgtDesc; ///< Pointer to the target descriptor.
} DD_ENABLE_FEC_ARGS;

typedef struct _DD_FLIP_QUEUE_EXECUTE_SW_QUEUE_ARGS
{
    PIPE_ID PipeId;
    DD_BOOL KeepScanlineInterruptEnabled;
    DD_BOOL AnyEntryExecuted;
    DDU32 TimeToNextPtsInUs;
    DDU32 CurrentScanLine;
    // Used only by Gen12 for now
    SAFE_SCANLINE_WAIT_ARGS SafeScanLineWaitArgs;
} DD_FLIP_QUEUE_EXECUTE_SW_QUEUE_ARGS;

typedef enum _DD_FLIPQ_MODIFY_REQUEST_TYPE
{
    DD_FLIPQ_MODIFY_REQUEST,
    DD_FLIPQ_CANCEL_REQUEST,             // Used to cancel after a given HwAddres on given HwPlane
    DD_FLIPQ_CANCEL_ALL_ON_PIPE_REQUEST, // Used internally to Purge all queues / planes on a given pipe
} DD_FLIPQ_MODIFY_REQUEST_TYPE;

typedef struct _DD_FLIP_QUEUE_CANCEL_PLANE
{
    DD_IN DDU8 HwPlaneId;                 // Not used for DD_FLIPQ_CANCEL_ALL_ON_PIPE_REQUEST
    DD_IN DD_BOOL SkipPlane;              // Used only for DD_FLIPQ_CANCEL_REQUEST - where there are no flips pending on layer
    DD_IN DDU32 HwAddressBase;            // Not used for DD_FLIPQ_CANCEL_ALL_ON_PIPE_REQUEST
    DD_IN DDU64 PresentId;                // Flips to be cancelled from this presentId onwards
    DD_OUT DDU64 CancelledPresentId;      // PresentId from which the flips were cancelled.
    DD_OUT DDU32 HwAddressBaseCancelled;  // Valid only for DD_FLIPQ_CANCEL_REQUEST
    DD_OUT DD_BOOL IsAnyAddressCancelled; // To identify if any address got cancelled or not
} DD_FLIP_QUEUE_CANCEL_PLANE;

typedef struct _DD_FLIP_QUEUE_CANCEL_MODIFY_ARGS
{
    DD_IN PIPE_ID PipeId; // Filled by DVM
    DD_IN DD_FLIPQ_MODIFY_REQUEST_TYPE RequestType;
    DD_IN DDU32 PlaneCount;
    DD_IN DD_FLIP_QUEUE_CANCEL_PLANE Planes[MAX_PLANES_PER_PIPE];
    DD_OUT DD_BOOL IsModifyCompleted; // Valid only for DD_FLIPQ_MODIFY_REQUEST - Set to FALSE if DMC has already programmed the flip, else set to TRUE.
} DD_FLIP_QUEUE_CANCEL_MODIFY_ARGS;

typedef struct _ENABLE_TRANSCODER_DATA
{
    // Transport type used by this transcoder
    DD_PROTOCOL_TYPE TransportTypeEnabled;
    // Color format: 6/8/10 bits per color (default is 8)
    // 6bpc is valid only for X1 mode
    // One may get this from EDID/CE blocks
    DDU8 ColorDepth;
    // Enable/Disable Dithering
    DD_BOOL EnableDithering;
    // Color range selection
    // Valid only when eSink is eHDMISink and
    // ucBitsPerColor == 8
    // HDMI encoder will set this when mode is
    // 480i, 576p, 576i, 240p or 288p
    // Enable/disable audio (can be enabled only for HDMI sink)
    DD_BOOL EnableAudio;
    DD_BOOL EnablePortSynch;
    DD_BOOL MsoCapable; // To be filled by protocol layer
    DD_BOOL AudSDPSplitSupport;
    DDU8 MsoSegments;              // No of segments
    DDU32 MaxSupportedAudioFreqHz; // maximum audio freq that can be supported in the current config
    DDU8 MaxNumOfAudioChannels;    // maximum number of audio channels that can be supported current config

    union
    {
        DP_LINK_DATA DpLinkData;
        HDMI_LINK_DATA HdmiLinkData;
        DD_MIPI_DSI_DATA MipiDsiData;
    };
} ENABLE_TRANSCODER_DATA;

#define MAX_NUM_LINK_RATE 13

typedef struct _DD_DP_SUPPORTED_LINK_RATES
{
    DDU8 NumOfLinkRates;
    DDU32 LinkRateMbpsList[MAX_NUM_LINK_RATE]; // Linkrate in Mbps
} DD_DP_SUPPORTED_LINK_RATES;

typedef struct _DD_EDP_PSR_CAPS_DATA
{
    DD_BOOL IsMmioBasedSff; // Till D15, Cursor Programming can happen at any time so there's possibility of mismatch between SFSU and cursor update
    // D15+, Cursor will be programmed via Semi-Full Flip Queue, everything will be executed synchrously
    DD_BOOL IsPSR2Enabled;
    DDU32 Psr2MaxXResolution;
    DDU32 Psr2MaxYResolution;
    DDU8 Psr2MaxBpc;
    DDU8 Psr2BlockSize;
    DDU32 Psr2MinLineTimeNs;
    DDU8 Psr2MinBlockCountLines;
    DDU8 Psr2MaxBlockCountLines;
    DDU32 Psr2LineTimeLimitNs;
    DD_BOOL IsPsrSetupTimeRestrictionRequired;
    DD_BOOL IsPsr2IdleFramesOverrideRequired;
    DDU32 MaxSfSuDpstSegments;
    DDU32 MinSfSuDpstSegmentSize;
    DDU32 Psr1TimingGenEnableTime;
    DDU32 Psr2TimingGenEnableTime;
    DDU32 Psr1CmtgTimingGenEnableTime;
    DDU32 Psr2CmtgTimingGenEnableTime;
} DD_EDP_PSR_CAPS_DATA;

typedef enum _DD_DISPLAY_HW_CAPS_OPERATION_TYPE
{
    DD_OP_NONE = 0,
    DD_OP_EDP_PSR_DISPLAY_CAPS,
    DD_OP_DP_MAX_VSWING_LINKRATE_VALUE,
    DD_OP_DP_TYPE_C_PORT_NUM,
    DD_OP_DP_DETECTED_PORT_TYPE,
    DD_OP_DISPLAY_HW_DSC_CAPS,
    DD_OP_DP_PANEL_REPLAY_SOURCE_CAPS,
    DD_OP_DP_MAX_MST_STREAMS,
    DD_OP_DP_MAX_LANES_AVAILABLE,
    DD_OP_DP_MAX_JOINT_DISPLAYS_SUPPORTED,
    DD_OP_EDP_ALPM_WAKE_TIME,
    DD_OP_SUPPORTED_DP_LINK_RATE_LIST,
    DD_OP_SHARPNESS_ADJUSTMENT_SUPPORT,
    DD_OP_PIPELOCK_SUPPORT,
    DD_OP_EDP_ON_TYPEC_SUPPORT,
    DD_OP_VRR_CAPS,
    DD_OP_SUPPORTED_FTR_BRT_OPTIMIZATION_CONCURRENCY_MASK_AND_PRIORITY_LIST,
    DD_OP_I2C_PIN_PAIRS_AVAILABLE,
    DD_OP_HDR_CAPS,
    DD_OP_DP_SSC_OVERHEAD
} DD_DISPLAY_HW_CAPS_OPERATION_TYPE;

typedef struct _DD_DISPLAY_HW_DSC_CAPS
{
    DD_BOOL CompressionSupported;
    DDU8 DscMajorVersion;
    DDU8 DscMinorVersion;
    DDU8 MaxSlicesPerLine;
    DDU8 MaxLineBufferDepth;
    union
    {
        DDU8 Value;
        struct
        {
            DDU8 Reserved1 : 1;
            DDU8 Dsc_8Bpc_Supported : 1;
            DDU8 Dsc_10Bpc_Supported : 1;
            DDU8 Dsc_12Bpc_Supported : 1;
            DDU8 Reserved2 : 4;
        };
    } CompressionBpc;
    DDU8 BigJoinerBits;
    DDU16 MaxPictureHeight;
    DDU16 MaxPictureWidth;
    DDU16 MaxDscSupportedBpp;
    DDU16 MaxDssToDptBits;
    DDU16 MaxUltraJoinerFifoSize;
    DDU32 MaxDssJoinerFifoSize;
} DD_DISPLAY_HW_DSC_CAPS;

typedef struct _DD_DP_PORT_PANEL_REPLAY_SOURCE_CAPS
{
    DDU16 SuXGranularity; // Gets Updated for DD_OP_DP_PANEL_REPLAY_SOURCE_CAPS
    DDU8 SuYGranularity;  // Gets Updated for DD_OP_DP_PANEL_REPLAY_SOURCE_CAPS
} DD_DP_PORT_PANEL_REPLAY_SOURCE_CAPS;

typedef struct _DD_DP_PORT_MAX_SWING_LINKRATE
{
    DD_DP_DRIVE_SETTING MaxVswingEmpValue; // Gets Updated for DD_OP_DP_MAX_VSWING_LINKRATE_VALUE
    DDU32 MaxLinkRateMbps;                 // Gets Updated for DD_OP_DP_MAX_VSWING_LINKRATE_VALUE
} DD_DP_PORT_MAX_SWING_LINKRATE;

typedef struct _DD_ALPM_WAKE_TIME_PARAMS
{
    DD_BOOL AuxWakeAlpmUsed;
    PIPE_ID Pipe;
    DD_PORT_TYPE Port;
    DDU32 LinkRateMbps;
    DDU32 VTotal;
    DDU32 VRoundedRr;
    DD_OUT DDU32 AlpmWakeTimeInLines;
    DD_OUT DDU32 MinSleepTimeInLines;
} DD_ALPM_WAKE_TIME_PARAMS;

typedef struct _DD_SHARPNESS_SUPPORT_HW_CAPS
{
    DD_IN PIPE_ID PipeId;
    DD_OUT DD_BOOL Supported;
} DD_SHARPNESS_SUPPORT_HW_CAPS;

typedef struct _DD_EDP_ON_TC_SUPPORT_CAPS
{
    DD_OUT DD_BOOL IsSupported;
    DD_OUT DD_BOOL IsPresent;
} DD_EDP_ON_TC_SUPPORT_CAPS;

typedef union _DD_VRR_ALWAYS_IN_VRR_SUPPORT
{
    DDU8 Value;
    struct
    {
        DDU8 OnAllPanels : 1;
        DDU8 OnVrrInternalPanels : 1;
        DDU8 OnVrrExternalDpPanels : 1;
        DDU8 OnVrrExternalHdmiPanels : 1;
        DDU8 OnNonVrrInternalPanels : 1;
        DDU8 OnNonVrrExternalDpPanels : 1;
        DDU8 OnNonVrrExternalHdmiPanels : 1;
        DDU8 Reserved : 1;
    };
} DD_VRR_ALWAYS_IN_VRR_SUPPORT;

typedef struct _DD_VRR_HW_CAPS
{
    DD_OUT DD_VRR_ALWAYS_IN_VRR_SUPPORT AlwaysInVrrSupport;
    DD_OUT DDU32 MaxVtotalInLinesNonVrr;
    DD_OUT DDU32 MaxVtotalInLinesWithVrrInternalDisplay;
    DD_OUT DDU32 MaxVtotalInLinesWithVrrExtDisplay;
} DD_VRR_HW_CAPS;

// Brightness optimization features support
typedef struct _DD_PC_FTR_BRT_OPTIMIZATION_SUPPORT
{
    DD_OUT DDU64* pSupportedFeaturesConcurrencyMasks;
    DD_OUT DDU8* pFeaturePriorities;
    DD_OUT DDU8 NumberFeatures;
} DD_PC_FTR_BRT_OPTIMIZATION_SUPPORT;

typedef enum _DD_I2C_PIN_PAIRS
{
    DD_I2C_PIN_PAIR_DISABLED = 0,
    DD_I2C_PIN_PAIR_1,
    DD_I2C_PIN_PAIR_2,
    DD_I2C_PIN_PAIR_3,
    DD_I2C_PIN_PAIR_4,
    DD_I2C_PIN_PAIR_5,
    DD_I2C_PIN_PAIR_6,
    DD_I2C_PIN_PAIR_MAX = DD_I2C_PIN_PAIR_6
} DD_I2C_PIN_PAIRS;

typedef struct _DD_I2C_PIN_PAIRS_CAPS
{
    DDU8 NumI2CPinPairs;                               // To access a pinpair need to send enumerated values below.
    DD_I2C_PIN_PAIRS I2CPinPairs[DD_I2C_PIN_PAIR_MAX]; // List of PinPairs present in current platform, continuous index list.
} DD_I2C_PIN_PAIRS_CAPS;

typedef struct _DD_DISPLAY_HW_CAPS
{
    DD_DISPLAY_HW_CAPS_OPERATION_TYPE OpCode; // Input
    DD_PORT_TYPE Port;                        // Input

    union
    {
        DD_EDP_PSR_CAPS_DATA EdpPsrCapsData;                          // Gets Updated for DD_OP_EDP_PSR_DISPLAY_CAPS
        DD_DP_PORT_MAX_SWING_LINKRATE DpPortMaxSwingLinkRate;         // Gets Updated for DD_OP_DP_MAX_VSWING_LINKRATE_VALUE
        DDU8 DpMaxLanesAvailable;                                     // Gets Updated for DD_OP_DP_MAX_LANES_AVAILABLE
        DDU32 TypeCPortNumber;                                        // Gets Updated for DD_OP_DP_TYPE_C_PORT_NUM
        DD_PORT_CONNECTOR_TYPE DetectedPortType;                      // Gets updated for DD_OP_DP_DETECTED_PORT_TYPE
        DD_DISPLAY_HW_DSC_CAPS SourceDscEncoderCaps;                  // Gets Updated for DD_OP_DISPLAY_HW_DSC_CAPS
        DDU8 DpMaxMstStreams;                                         // Gets Updated for DD_OP_DP_MAX_MST_STREAMS
        DD_DP_PORT_PANEL_REPLAY_SOURCE_CAPS PanelReplaySourceCaps;    // Gets Updated for DD_OP_DP_PANEL_REPLAY_SOURCE_CAPS
        DDU8 MaxJointDisplayCount;                                    // Gets Updated for DD_OP_DP_MAX_JOINT_DISPLAYS_SUPPORTED
        DD_DP_SUPPORTED_LINK_RATES DpSupportedLinkRates;              // Gets Updated for DD_OP_SUPPORTED_DP_LINK_RATE_LIST
        DD_ALPM_WAKE_TIME_PARAMS WakeTimeParams;                      // Gets Updated for DD_OP_EDP_ALPM_WAKE_TIME
        DD_SHARPNESS_SUPPORT_HW_CAPS SharpnessCaps;                   // Use for DD_OP_SHARPNESS_ADJUSTMENT_SUPPORT
        DD_BOOL IsPipeLockSupported;                                  // Gets Updated for DD_OP_PIPELOCK_SUPPORT
        DD_EDP_ON_TC_SUPPORT_CAPS EdpOnTypeC;                         // Use for DD_OP_EDP_ON_TYPEC_SUPPORT
        DD_VRR_HW_CAPS VrrHwCaps;                                     // Gets Updated for DD_OP_VRR_CAPS
        DD_PC_FTR_BRT_OPTIMIZATION_SUPPORT FtrBrtOptimizationSupport; // Gets the number of brightness optimization feature supported, concurrency mask and priotity list
        DD_I2C_PIN_PAIRS_CAPS I2CPinPairCaps;                         // Gets Updated for DD_OP_I2C_PIN_PAIRS_AVAILABLE
        DD_HDR_PLATFORM_CAPS HdrPlatformCaps;
        DDU32 DpSscOverheadx1e6; // Gets updated for DD_OP_DP_SSC_OVERHEAD
    };
} DD_DISPLAY_HW_CAPS;

typedef struct _SPRITE_PLANEPARAMS
{
    DDU32 StartAddress;   // Sprite Start Adress
    DDU32 ScanLineLength; // Sprite scan line length
    DDU32 SpriteXPos;
    DDU32 SpriteYPos;
    DDU32 SpriteXTiledOffset;
    DDU32 SpriteYTiledOffset;
    DDU32 SpriteWidth;
    DDU32 SpriteHeight;
} SPRITE_PLANEPARAMS;

typedef struct _PLANE_CAPABILITY
{
    DDU8 Rotation;
    DDU8 FlipMode;
    DDU8 SteroFormat;
    DDU8 StretchQuality;
    DDU8 FBC : 1;
    DDU8 S3DCompression : 1;
    DDU8 MediaCompression : 1;
    DDU8 NV12 : 1;
    DDU8 YUV : 1;
    DDU8 P0xx : 1;
    DDU8 HFlip : 1;
    DDU8 Reserved : 1;
} PLANE_CAPABILITY;

// READ-ONLY params
typedef struct _PLANE_PARAMS_RO
{
    DDU8 PipeID;                      // Provides info on which plane this pipe can be assigned to
    PLANE_CAPABILITY PlaneCapability; // provides info on the capabilities of the plane, used Gen9 onwards
    PLANE_ZORDER ZOrder;              // Z order of plane

    DDU8 MuxedWithCursor : 1;
    DDU8 Reserved2 : 4;
} PLANE_PARAMS_RO;

typedef struct _DD_PIPE_SOURCE_DETAILS
{
    DDU32 SrcSizeX;
    DDU32 SrcSizeY;
    DD_PIXELFORMAT SrcPixFormat;
} DD_PIPE_SOURCE_DETAILS;

typedef struct _DD_ASSIGN_PIPE_DETAILS
{
    DD_IN DD_TARGET_DESCRIPTOR TargetDesc;
    DD_IN DDU32 NumPipes;
    // A pointer to PipeId is being used below so that pipe id can directly get updated in CCD struct. Otherwise it is lot of reverse calculation to figure out where to update in CCD struct.
    DD_IN_OUT PIPE_ID* pPipeId;
    struct
    {
        DD_IN DDU8 IsCompressionRequired : 1; // Requires compression supported pipe from display hardware.
        DD_IN DDU8 IsPipeJoiningRequired : 1; // Requires adjacent pipe from display hardware.
        DD_IN DDU8 IsMsoPanel : 1;
        DD_IN DDU8 IsSecondaryInternalTarget : 1;
        DD_IN DDU8 Reserved : 4;
    };
} DD_ASSIGN_PIPE_DETAILS;

typedef struct _DD_ASSIGN_PIPE_ARGS
{
    DD_IN DDU32 NumTarget;
    DD_IN DDU8 NumOfInternalDisplays; // This is used to give preference to Internal displays for assigning low power pipes (like PIPE_A/PIPE_B)
    DD_ASSIGN_PIPE_DETAILS Target[MAX_PHYSICAL_PIPES];
    DDU8 NoOfMsoPanelsPresent; // This is used for blocking the Pipe when there is MSO panel in the config , as only Pipe A supports Mso
} DD_ASSIGN_PIPE_ARGS;

typedef struct _DD_ASSIGN_PIPE_DVM_DETAILS
{
    DD_IN DDU32 ChildId;
    DD_IN const DD_TIMING_INFO* pTargetMode; // Required to find if mode-set is for a joined mode
    struct
    {
        DD_IN DDU8 KeepCurrentPipe : 1;
        DD_IN DDU8 IsCompressionRequired : 1; // Requires compression supported pipe from display hardware.
        DD_IN DDU8 IsMsoPanel : 1;
        DD_IN DDU8 Reserved : 5;
    };
} DD_ASSIGN_PIPE_DVM_DETAILS;

typedef struct _DD_ASSIGN_PIPE_DVM_ARGS
{
    DD_IN DDU32 NumTarget;
    DD_IN DDU8 NumOfInternalDisplays; // This is used to give preference to Internal displays for assigning low power pipes (like PIPE_A/PIPE_B)
    DD_ASSIGN_PIPE_DVM_DETAILS Target[MAX_PHYSICAL_PIPES];
    DDU8 NoOfMsoPanelsPresent; // This is used for blocking the Pipe when there is MSO panel in the config , as only Pipe A supports Mso
} DD_ASSIGN_PIPE_DVM_ARGS;

typedef struct _DD_DVM_ASSIGN_DBUF_CHILD_DETAILS
{
    DDU32 ChildId; /**< VidPn child. */
    DD_TIMING_INFO TimingInfo;
    DD_PIXELFORMAT PixelFormat;
} DD_DVM_ASSIGN_DBUF_CHILD_DETAILS;

typedef struct _DD_DVM_ASSIGN_DBUF_ARGS
{
    DDU32 NumActiveTargets;
    DD_DVM_ASSIGN_DBUF_CHILD_DETAILS ChildDetails[MAX_PHYSICAL_PIPES]; /**< Only fills Physical Pipe Targets. */
} DD_DVM_ASSIGN_DBUF_ARGS;

typedef union _DD_CUSTOM_SCALING_DETAILS
{
    struct
    {
        DDU32 ScalingX : 8;
        DDU32 ScalingY : 8;
        DDU32 Reserved : 15;
        DDU32 ScalingEnabled : 1;
    };
    DDU32 Value;
} DD_CUSTOM_SCALING_DETAILS;

typedef struct _DD_UPDATE_ROTATION_INFO_ARGS
{
    PIPE_ID PipeID;                // Pipe index
    PLANE_ORIENTATION Orientation; // Rotation angle
} DD_UPDATE_ROTATION_INFO_ARGS;

typedef struct _DD_WRITEBACK_CONFIG_ARGS
{
    DD_IN DDU8 SinkIndex;
    DD_IN DDU32 Stride;
    DD_OUT DDU32 SurfaceBaseAddr;
} DD_WRITEBACK_CONFIG_ARGS;

typedef enum _DD_SET_TIMING_ACTION
{
    TARGET_UNDEFINED = 0,
    TARGET_KEEP_ENABLED = 1, // path which is already enabled and nothing changed as part of the current modeset request
    TARGET_DISABLE = 2, // we need to disable only, usually for removed path or path which is disabled in this modeset
    TARGET_ENABLE_FRESH = 3, // new display that is added to the topology or enable a path that was previously disabled
    TARGET_ENABLE_MODIFIED_TIMING = 4, // timings or some param changed for an already enabled display, which would involve a disable and enable
    TARGET_ENABLE_PRESERVE_TIMING = 5, // try to perform fastmodeset on an enabled display,
    TARGET_KEEP_DISABLED = 6, // nothing to be done, path which was already disabled and is disabled in current call too
} DD_SET_TIMING_ACTION;

// Structures for Sharpness Enhancement using Display HW Scalars
// Floating point representation of in terms of Sign/Exponent/Mantissa.
// Not using an actual Floating point number as these values will be passed from OSL to HAL and we want to isolate Floating point calculations from the rest of the code.

typedef struct _DD_SHARPNESS_COEFF
{
    DDU16 Sign;
    DDU16 Exponent;
    DDU16 Mantissa;
} DD_SHARPNESS_COEFF;

typedef enum _DD_SHARPNESS_FILTER_TYPE
{
    SHARPNESS_FILTER_TYPE_NONADAPTIVE, ///< Non-adaptive sharpness
    SHARPNESS_FILTER_TYPE_ADAPTIVE,    ///< Adaptive sharpness
    SHARPNESS_FILTER_TYPE_MAX,
} DD_SHARPNESS_FILTER_TYPE;

typedef struct _DD_SHARPNESS_ARGS
{
    union
    {
        struct
        {
            DDS32 SharpnessIntensity : 8;
            DDU32 Reserved : 23;
            DDU32 Enable : 1;
        };
        DDU32 CurStateVal; // This value is synonymous with the value sent by IGCC and this is the value that is stored in registry key for state determination and persistance
    };                     // Keeping this anonymous union as this value is read write from registry
    PIPE_ID PipeId;
    DDU8 NumTaps;
    DD_SHARPNESS_COEFF UniqueCoeff[MAX_NUM_UNIQUE_COEF_FOR_SHARPNESS_FILTER]; // Our HW supports max 7 Taps (A,B,C,D,E,F,G). D is centre tap.Sharpness Enhancement use these values for all 17 phases.
    DD_SHARPNESS_FILTER_TYPE FilterType;
} DD_SHARPNESS_ARGS;

// Args for Pipe modeset enable/disable sequence
typedef struct _DD_SETMODE_ARGS
{
    DD_IN DD_BOOL Enable;
    DD_IN DDU32 ChildId;                        // VidPn Child Id to be passed to DVM, CCD/DVM use.
    DD_IN DD_TARGET_DESCRIPTOR TgtDesc;         // Target device identifier
    DD_IN PIPE_ID Pipe;                         // Pipe to be used for Enable/Disable
    DD_IN DD_SET_TIMING_ACTION SetTimingAction; // To describe the sequence of operations happening on this Pipe during current SetMode DDI.
    struct
    {
        DD_IN DD_PORT_SYNC_MODE Mode;
        DD_IN DD_PORT_SYNC_CFG TiledCfg; // Tiled display/port sync/pipe lock use cases to know primary/secondary pipe
        DD_IN DD_GENLOCK_CFG GenLockCfg; // Multi-Chip Genlock configuration
    } PortSync;

    DD_IN DD_BOOL IsPipeJoinedMode;
    DD_IN PIPE_ID PrimaryPipeForPipeJoinedDisplay;
    DD_IN DD_BOOL ProgramPipeSeamExcessPixel; // If need to program Pipe Seam excess pixel in HAL, true for Tiled and Pipe joined mode.
    DD_IN DDU32 HLocation;                    // To be used to calculate Pipe Seam Excess for joint displays
    // Below parameters required only when 'Enable = TRUE'
    DD_IN DD_PIPE_SOURCE_DETAILS SrcDetails; // Pipe source mode details
    DD_IN DD_TIMING_INFO TimingInfo;         // Pipe Timing
    DD_IN DDU32 Window2Size;                 // Window2 size for SetContextLatency.
    DD_IN TIMING_FEATURE_SUPPORT FeatureSupport;
    DD_IN DD_COLOR_PIXEL_DESC OutputColorFormat; // Pipe OutputColor format
    DD_IN DD_SCALING Scaling;
    DD_IN DD_CUSTOM_SCALING_DETAILS CustomScalingValue;
    DD_IN NN_SCALING_STATE NNScalingState;
    DD_IN DD_BOOL IsMsaIgnoreRequired;           // True if RR can be changed using VRR method.
    DD_IN ENABLE_TRANSCODER_DATA TranscoderData; // to be filled by protocol layer
    DD_IN DSC_CFG DscCfg;                        // DSC Config to be filled by protocol layer
    DD_IN_OUT DD_BOOL PreserveBootDisplay;
    DD_IN_OUT DD_WRITEBACK_CONFIG_ARGS WbConfigArgs;
    DD_IN DD_BOOL IsVirtualDvi;
    DD_IN DD_BOOL IsCustomAlignmentSupported;              // Support for Dual LFP Hinge Based Panel Alignment for panels having additional active region(VSRI-4335)
    DD_IN DD_BOOL AllowPpsSequence;                        // To be set only when PPS needs to be done for target
    DD_IN DD_BOOL AllowBacklightSequence;                  // To be set only when Backlight programming is needed for current target
    DD_IN DD_PANEL_CUSTOM_ALIGN_DATA PanelCustomAlignData; // Dual LFP Hinge Based Panel Alignment Data (VSRI-4335)
    DD_IN DD_SHARPNESS_ARGS SharpnessData;                 // Enable Pipe Scalar for Enhancing Sharpness
    DD_OUT DD_BOOL IsSwPsrEnabled;                         // Set to TRUE for cases where SW PSR was enabled
    DD_OUT DD_BOOL IsReplugNeeded;                         // Set to TRUE if LT Fallback needs a replug.
    DD_IN DD_DISPLAY_CAPS DisplayCaps;                     // Display capabilities
    DD_IN DD_BOOL AllowProtocolFallback;                   // Allow protocol layer fallback for DP Link BW and HDMI mode.
} DD_SETMODE_ARGS;

#define DD_INIT_SETMODE_ARGS(pSetModeArg)                     \
    DD_ZERO_MEM(pSetModeArg, sizeof(*pSetModeArg));           \
    pSetModeArg->Pipe                            = NULL_PIPE; \
    pSetModeArg->PortSync.TiledCfg.PrimaryPipe   = NULL_PIPE; \
    pSetModeArg->PortSync.TiledCfg.SecondaryPipe = NULL_PIPE; \
    pSetModeArg->PrimaryPipeForPipeJoinedDisplay = NULL_PIPE; \
    pSetModeArg->SharpnessData.PipeId            = NULL_PIPE

typedef struct _DD_GET_PORT_SYNC_TYPE_ARGS
{
    DD_IN DDU8 NumTargets;
    DD_IN DD_BOOL IsTiledMode;
    DD_IN DD_TARGET_DESCRIPTOR Targets[MAX_PHYSICAL_PIPES]; // max array limit can be used for SW sync mode.
    DD_IN DD_TIMING_INFO TimingInfo[MAX_PHYSICAL_PIPES];    // To be used for SW port sync
    DD_OUT DD_PORT_SYNC_MODE SyncMode;
} DD_GET_PORT_SYNC_TYPE_ARGS;

typedef struct _DD_FMS_ARGS
{
    DDU32 PathCount;
    DD_SETMODE_ARGS SetModeArgs[MAX_PIPES];
} DD_FMS_ARGS;

typedef struct _DD_NPIPES_SETMODEARGS
{
    DDU32 NoOfPipes;   // more than 1 pipes will be there for Tiled and Pipe joined displays
    DDU32 NoOfTargets; // more than 1 for Tiled display.
    DD_BOOL Enable;
    DD_SETMODE_ARGS SetModeArg[MAX_TILES_SUPPORTED]; // 1 bydefault
    DD_BOOL IsPathModeSetFailed;
} DD_NPIPES_SETMODEARGS;

typedef struct _DD_SEND_HDR_STATIC_METADATA_ARG
{
    DD_TARGET_DESCRIPTOR TargetDesc;
    DD_HDR_STATIC_METADATA* pHdrStaticMetaData;
    DD_BOOL IsAuxBasedEdpHdr;
    DD_BOOL IsPanelToneMappingSupported;
} DD_SEND_HDR_STATIC_METADATA_ARG;

typedef struct _DD_ASSIGN_DBUF_DETAILS
{
    PIPE_ID PipeId;
    DD_TIMING_INFO TimingInfo;
    DD_BOOL IsPipeJoinedMode;
    DD_BOOL IsPSRCapable;
    DD_PIXELFORMAT PixelFormat;
} DD_ASSIGN_DBUF_DETAILS;

typedef struct _DD_ASSIGN_DBUF_ARGS
{
    DDU32 NumActivePipes;
    DD_ASSIGN_DBUF_DETAILS PipeDetails[MAX_PHYSICAL_PIPES];
} DD_ASSIGN_DBUF_ARGS;

typedef struct _DD_GET_TILE_FMT
{
    IN DDU32 HorizontalRes;
    IN DD_PIXELFORMAT PixelFormat;
    IN PLANE_ORIENTATION Rotation;
} DD_GET_TILE_FMT;

typedef struct _DD_MIN_DBUF_INPUT_PARAMS
{
    DDU32 HorizontalRes;
    DD_SURFACE_MEMORY_TYPE MemFormat;
    DD_ROTATION RotationAngle;
    DD_PIXELFORMAT PixelFormat;
} DD_MIN_DBUF_INPUT_PARAMS;
//----------------------------------------------------------------------------
//
// Plane/Pipe/Pll/Port/Watermark related structures - END
//
//----------------------------------------------------------------------------

// Power well
typedef union _POWER_WELL_CONFIG
{
    struct
    {
        DDU32 PowerWell1 : 1;         // 1 - enabled
        DDU32 PowerWell2 : 1;         // 1 - enabled
        DDU32 DdiAAndDdiEIoPower : 1; // 1 - enabled
        DDU32 DdiBIoPower : 1;        // 1 - enabled
        DDU32 DdiCIoPower : 1;        // 1 - enabled
        DDU32 DdiDIoPower : 1;        // 1 - enabled
        DDU32 MiscIoPower : 1;        // 1 - enabled
        DDU32 DdiEIoPower : 1;        // 1 - enabled
        DDU32 AuxAIoPower : 1;        // 1 - enabled
        DDU32 AuxBIoPower : 1;        // 1 - enabled
        DDU32 AuxCIoPower : 1;        // 1 - enabled
        DDU32 AuxDIoPower : 1;        // 1 - enabled
        DDU32 Reserved : 20;
    };
    DDU32 Value;
} POWER_WELL_CONFIG;

// DPST Algorithm version
typedef enum _DD_DPST_ALGORITHM_VERSION
{
    DD_DPST_VERSION_NONE = 0,
    DD_DPST_VERSION_6_3 = 63,
    DD_DPST_VERSION_7_0 = 70,
    DD_DPST_VERSION_7_1 = 71,
    DD_DPST_VERSION_8_0 = 80
} DD_DPST_ALGORITHM_VERSION;

// LACE Feature version
typedef enum _DD_LACE_FEATURE_VERSION
{
    LACE_FEATURE_VERSION_NONE = 0,
    LACE_FEATURE_VERSION_1_0 = 10, // Legacy Lace
    LACE_FEATURE_VERSION_2_0 = 20, // GHE - Global Histogram Equalization
} DD_LACE_FEATURE_VERSION;

// PSR Protocol specific Parameters

// Combined PSR1/PSR2/PR states
typedef enum _DD_PSR_PR_STATE
{
    IN_IDLE_STATE,
    IN_IDLE_STATE_AFTER_DISABLE,
    IN_NOT_IDLE_STATE_AFTER_DISABLE,
    IN_BUSY_CAPTURE_STATE,
    IN_BUSY_NON_CAPTURE_STATE // For PR, this state is used to indicate that RR change is allowed even though PR is busy
} DD_PSR_PR_STATE;

typedef enum _DD_GTC_REQUEST
{
    GTC_DISABLE = 0,
    GTC_ENABLE,
    GTC_UPDATE_LOCK,
} DD_GTC_REQUEST;

// PSR event handler args structure
typedef enum _DD_PSR_PR_COMMAND
{
    PSR_ENABLE = 0,
    PSR_DISABLE,
    PSR_ENABLE_SU,                               // PSR2
    PSR_DISABLE_SU,                              // PSR2
    PSR_MEDIA_PLAYBACK_MODE,                     // PSR2
    PSR_ENABLE_IN_TRANSCODER,                    // Set enable bit of PSR register
    PSR_ENABLE_IN_TRANSCODER_SU,                 // Set enable bit of PSR2 register
    PSR_DISABLE_IN_TRANSCODER,                   // Reset enable bit of PSR register
    PSR_DISABLE_IN_TRANSCODER_SU,                // Reset enable bit of PSR2 register
    PSR_PANELREPLAY_MANUAL_TRACKING_MODE_UPDATE, // Manual tracking update
    PANELREPLAY_ENABLE,
    PANELREPLAY_DISABLE,
    PANELREPLAY_ENABLE_IN_TRANSCODER,
    PANELREPLAY_DISABLE_IN_TRANSCODER,
    PSR_ENABLE_SW_MODE,
    PSR_DISABLE_SW_MODE,
    PSR_UPDATE_IDLE_FRAME_COUNT_IN_TRANSCODER_SU,
    PSR_POLL_FOR_EXIT_IN_TRANSCODER_SU,
    PSR_POLL_FOR_EXIT_IN_TRANSCODER,
    PANELREPLAY_POLL_FOR_EXIT_IN_TRANSCODER,
    PSR_PANELREPLAY_MANUAL_TRACKING_ENABLE,
    PSR_MAX_COMMAND
} DD_PSR_PR_COMMAND;

// PSR2 modes
typedef union _DD_PSR2_MODE
{
    DDU8 Psr2Mode;
    struct
    {
        /** Bit 0: IsContinuousFullFetchMode.    */
        DDU8 IsContinuousFullFetchMode : 1;
        /** Bit 1: IsSingleFullFetchMode.     */
        DDU8 IsSingleFullFetchMode : 1;
        /** Bit 2: IsSfsuMode.     */
        DDU8 IsSfsuMode : 1;
        /** Bit 3-7: Reserved .    */
        DDU8 Reserved : 5;
    };
} DD_PSR2_MODE;

typedef struct _DD_PSR2_MAN_TRK_ARGS
{
    DDU32 SUStartOffset;                    // SU region start address offset in PSR2 Man Trk Reg
    DDU32 SUEndOffset;                      // SU region end address offset in PSR2 Man Trk Reg
    DD_BOOL ContinuousFullFrameFetchEnable; // Bit to enable continuous full frame fetches
    DD_BOOL SingleFullFrameFetchEnable;     // Bit to enable single full frame fetch
    DD_BOOL Psr2PrSfEnable;                 // Enable selective fetch in PSR2 manual tracking
} DD_PSR2_MAN_TRK_ARGS;

typedef struct _DD_SETPSR_ARGS
{
    DD_PSR_PR_COMMAND PsrCmd; // PSR command type
    DD_PORT_TYPE Port;        // needed to identify PIPE_EDP
    DDU32 PipeInUse;
    DDU32 RefreshRate;
    DDU8 IdleFramesNum;
    DDU8 SuIdleFrames;
    DD_PSR2_MAN_TRK_ARGS Psr2ManTrkArgs; // Store Manual tracking args as part of PSR args
    DD_PSR_DISPLAY_CAPS PsrDetails;
    DD_PANEL_REPLAY_CAPS PanelReplayDetails;
    DD_BOOL RfbUpdateMode;
    DD_BOOL DisablePsr2DeepSleep;
    DD_BOOL IsAlrrEnable;
} DD_SETPSR_ARGS;

typedef struct _DD_PSR_ARGS
{
    IN DD_TARGET_DESCRIPTOR TargetDesc;
    IN DD_PSR_PR_COMMAND PsrCommand; // PSR command type
    IN DDU8 IdenticalFramesCount;    // Used for PSR_ENABLE and PSR_HW_MODE.
    IN DD_BOOL ContinuousFullFrameFetchEnable;
    IN DD_BOOL SingleFullFrameFetchEnable;
    IN DD_BOOL RfbUpdateMode;
    IN DD_BOOL DisablePsr2DeepSleep;
    IN DD_BOOL Psr2PrSfSupported;
} DD_PSR_ARGS;

typedef enum _DD_AS_SDP_TRANSMISSION_OPERATION
{
    DD_NO_OPERATION = 0,
    DD_SET_AS_SDP_TRANSMISSION,
    DD_SET_PERIODIC_AS_SDP_TRANSMISSION_DMC_BASED,
    DD_SET_PERIODIC_AS_SDP_TRANSMISSION_HW_BASED,
    DD_SET_AS_SDP_MAX_OPERATION
} DD_AS_SDP_TRANSMISSION_OPERATION;

typedef struct _DD_AS_SDP_TRANSMISSION_ARGS
{
    IN PIPE_ID Pipe;
    IN DD_PORT_TYPE Port;
    IN DD_AS_SDP_TRANSMISSION_OPERATION AsSdpTransmissionOperation;
    IN DD_BOOL EnableAsSdpTransmission;    // Enable As Sdp Transmission.
    IN DD_BOOL UpdatePeriodicAsSdpStatus;  // Setup Periodic As Sdp.
    IN DDU32 PeriodicAsSdpSkipFramesCount; // Used for periodic as sdp skip counter.
} DD_AS_SDP_TRANSMISSION_ARGS;

typedef enum _DD_PSR_SETUP_TIME
{
    DD_PSR_SETUP_TIME_330_US = 330,
    DD_PSR_SETUP_TIME_275_US = 275,
    DD_PSR_SETUP_TIME_220_US = 220,
    DD_PSR_SETUP_TIME_165_US = 165,
    DD_PSR_SETUP_TIME_110_US = 110,
    DD_PSR_SETUP_TIME_55_US = 55,
    DD_PSR_SETUP_TIME_0_US = 0,
} DD_PSR_SETUP_TIME;

//----------------------------------------------------------------------------
//
// Plane/Pipe/Pll/Port/Watermark related structures - START
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
// AUX/I2C Args -- START
//
//----------------------------------------------------------------------------

// I2C BUS request
typedef enum _DD_I2C_BUS_REQUEST
{
    DD_I2C_REQ_UNDEFINED = 0,
    DD_I2C_REQ_AUTO_SELECT = 1,
    DD_I2C_REQ_USE_BITBASHING = 2,
    DD_I2C_REQ_USE_GMBUS = 3,
    DD_I2C_REQ_FORCE_NATIVE = 4
} DD_I2C_BUS_REQUEST;

// I2C Commands
// Note: Use GET_DD_I2C_COMMAND to get the GMCH
// command value from a AIM3 flag value
typedef enum _DD_I2C_COMMAND
{
    DD_I2C_COMMAND_NULL = 0x0000, // Does no command
    DD_I2C_COMMAND_READ = 0x0001, // Reads Data from an I2C Bus Device
    DD_I2C_COMMAND_WRITE = 0x0002, // Writes Data out on the I2C Bus Device
    DD_I2C_COMMAND_STATUS = 0x0003, // Effects I2C Bus State
    DD_I2C_COMMAND_RESET = 0x0004, // Resets the I2C Bus
} DD_I2C_COMMAND;

// I2C Flags
// Note: Use GET_DD_I2C_FLAGS to get the GMCH
// flag value from a AIM3 flag value
typedef union _DD_I2C_FLAGS
{
    DDU32 Value;
    struct
    {
        DDU32 Start : DD_BITFIELD_BIT(0);
        DDU32 Restart : DD_BITFIELD_BIT(1);
        DDU32 Stop : DD_BITFIELD_BIT(2);
        DDU32 Address : DD_BITFIELD_BIT(3);
        DDU32 Index : DD_BITFIELD_BIT(4);
        DDU32 Dword : DD_BITFIELD_BIT(5);
        DDU32 Word : DD_BITFIELD_BIT(6);
        DDU32 Slow : DD_BITFIELD_BIT(7);
        DDU32 Reserved : DD_BITFIELD_RANGE(8, 31);
    };
} DD_I2C_FLAGS;
C_ASSERT(sizeof(DD_I2C_FLAGS) == sizeof(DDU32));

// I2C Destination
// Note: Use GET_DD_I2C_DEST_ADR to get the GMCH
// destination value from a AIM3 flag value
typedef enum _DD_I2C_DEST_ADR
{
    DD_I2C_DEST_CODEC = 0x00, // Implies the Codec
    DD_I2C_DEST_CODEC2 = 0x01, // Implies 2nd Codec (e.g. for Dual-Channel)
    DD_I2C_DEST_SPD = 0x02, // Implies the ADD SPD
    DD_I2C_DEST_CODEC_ADR = 0x03, // Targets an Address on the Codec Bus
    DD_I2C_DEST_DDC_ADR = 0x04, // Targets an Address on the Monitor DDC Bus
} DD_I2C_DEST_ADR;

//----------------------------------------------------------------------------
//
// GMBUS_REQ_SPEED - Caller if required can select a different speed
// by providing a value other than GMBUS_REQ_SPEED_AUTO in
// DD_I2C_ARGUMENTS. Note that this request value is not the same
// as what get programmed in the HW register.
//
// Default Selection is as follows:
//
// GMBUS_REQ_SPEED_400KHZ:
//      DD_I2C_DEST_CODEC,
//      DD_I2C_DEST_CODEC2,
//      DD_I2C_DEST_SPD,
//      DD_I2C_DEST_CODEC_ADR
//
// GMBUS_REQ_SPEED_100KHZ:
//      DD_I2C_DEST_DDC_ADR
//
// GMBUS_REQ_SPEED_50KHZ: When DD_I2C_FLAGS_SLOW is selected
//
// In addition to this the rate will be changed internally based on
// the sDVO switch required. This will happen irrespective of the
// input requested speed. It's expected that they will match, but
// in case, if they don't SB will ASSERT.
// For switch following are the default speeds:
//  DDC-100KHz, REGS-1MHz, EPROM-400KHz
//
//----------------------------------------------------------------------------
typedef enum _GMBUS_REQ_SPEED
{
    GMBUS_REQ_SPEED_AUTO = 0,
    GMBUS_REQ_SPEED_100KHZ = 1,
    GMBUS_REQ_SPEED_50KHZ = 2,
    GMBUS_REQ_SPEED_400KHZ = 3,
} GMBUS_REQ_SPEED;

//----------------------------------------------------------------------------
//
// Macros to get GMCH/GAL I2C flags, command and
// destination values
//
//----------------------------------------------------------------------------
#define GET_DD_I2C_COMMAND(AIMI2CFlags) (AIMI2CFlags & 0x0000000F)
#define GET_DD_I2C_FLAGS(AIMI2CFlags) ((AIMI2CFlags & 0x0000FF00) >> 8)
#define GET_DD_I2C_DEST_ADR(AIMI2CFlags) ((AIMI2CFlags & 0x00FF0000) >> 16)
#define GET_DD_I2C_TARGET_ADR(AIMI2CFlags) ((AIMI2CFlags & 0xFF000000) >> 24)
#define I2C_ADDRESS_EDID 0xA0
#define I2C_ADDRESS_DID2 0xA4
#define I2C_ADDRESS_SCDC_REGISTERS 0xA8
#define I2C_ADDRESS_SOURCE_ADAPTER 0x80
#define HDMI_EDID_OFFSET_MAX_TMDS 0x1D
#define EDID_EXT_BLOCKS 0x7E
#define EDID_CHECKSUM_OFFSET 0x7F
#define DID_EXTENSION_BLOCK_TAG_OFFSET 0x00 // Byte0 would indicate DisplayID EDID Extension Block Tag
#define DID_EXTENSION_TO_EDID_TAG_HEADER_SIZE_IN_BYTES \
    0x05 // EDID Extension Block Tag (Byte 0) + Structure Version/Revision (Byte 1) + Section Size (Byte 2) + Product Type Identifier (Byte 3) + Extension Count (Byte 4)
#define DID_EXTENSION_TO_EDID_SECTION_SIZE_OFFSET_02H 0x02 // Byte 2 indicates Section Size
#define DID_EXTENSION_TO_EDID_TAG_VALUE_70H 0x70           // Value 0x70 would indicate DisplayID Block as an extension to EDID
#define I2C_ADDRESS_EDDC_SEGMENT_POINTER 0x60

typedef struct _DD_I2C_ARGUMENTS
{
    DD_PORT_TYPE Port;
    DD_PROTOCOL_TYPE Protocol;
    /** GMBUS pin pair Index from VBT to read HDMI/DVI EDID/panel params. From CNL, values are platform independant. */
    DDU8 DdcPinPairIndex;
    /** Target address (Final address to be used. e.g. for DDC on sDVO device this will be 0xA0 & not 0x70). */
    DDU32 TargetAddress;
    /** Index. */
    DDU32 Index;
    /** Note: Always a byte pointer to data. */
    DDU8* pBuffer;
    /** Data size in bytes. */
    DDU32 BufferSize;

    /** Command, flags & destination type. */
    DD_I2C_COMMAND Command;
    DD_I2C_FLAGS I2CFlags;
    DD_I2C_DEST_ADR Destination;
    DDU8 SegmentPointer; // Used for E-DDC read for EDID/DID blocks > 2. Currently using in I2CoverAux Read E-DDC flow. Can have max value: 127.

    // Parameters of importance to GMBus only
    /** 1 - Use Bit bashing, Mostly used for MCCS. */
    DD_BOOL UseBitBashing;
    /** (Default: GMBUS_REQ_SPEED_AUTO). */
    GMBUS_REQ_SPEED RequestedGMBusSpeed;
    /** caller can set this if it's middle of a transaction (ie., first transaction or re-start). */
    DD_BOOL MiddleOfTransaction;
    /** Flag to enable Aksv buffer selection [ FALSE => select default buffer stated by GMBUS3; TRUE => select AKSV buffer ]. */
    DD_BOOL AksvBufferSelect;

    /** Aux Channel Type. */
    DD_DP_AUX_CHANNEL_TYPE AuxChannelType;

    /** HDCP 2.2. */
    DD_BOOL HDCP2_2_Message;
    /** 0 based. 0 - No retry, 1 - Retry once and so on... */
    DDU8 RetryCount;
    /** Actual bytes Read/written. */
    DDU32 ActualBytesReadOrWritten;
    /** Need to access I2C Bus based on Pin pair enumeration. Display Connection or Port Type Check not needed. */
    DD_BOOL IsI2cAccessOnPinPair;
} DD_I2C_ARGUMENTS;

// DD_ATOMICI2C_ARGUMENTS - Used by AtomicI2CAccess
typedef struct _DD_ATOMICI2C_ARGUMENTS
{
    DD_I2C_ARGUMENTS I2CArgs;

    // Extra params
    DDU32 ReadBytes;  // Bytes to read
    DDU32 WriteBytes; // Bytes to write
} DD_ATOMICI2C_ARGUMENTS;

// Args for read DDC operation
typedef struct _DD_READ_DDC_ARGUMENTS
{
    DD_PORT_TYPE Port; // Of type PORT_TYPES
    DD_PROTOCOL_TYPE Protocol;

    DDU8 DdcPinPairIndex; // GMBUS pin pair Index from VBT to read HDMI/DVI EDID/panel params. From CNL, values are platform independant

    DDU32 TargetAddress; // Target address (Final address to be used. e.g. for DDC on sDVO device this will be 0xA0 & not 0x70)
    DDU16 BlockNumber;   // EDID block (0, 1, ...)
    DDU8* pBuffer;       // Note: Always a byte pointer to data
    DDU32 BufferSize;    // Data size in bytes

    DD_BOOL UseBitBashing; // 1 - Use Bit bashing, Mostly used for any WAs/experiments

    DD_DP_AUX_CHANNEL_TYPE AuxChannelType; // AUX Channel type
} DD_READ_DDC_ARGUMENTS;

// The HAL accepts max 48 bytes as an atomic Aux transaction request, needed for DP Sideband Req
#define DD_NATIVE_AUX_MAX_REQ_SIZE 635 // RxCert size for HDCP 2.2 is 534 and the KSV FIFO for HDCP 1.4 is 635

// Due to hardware limitations the max aux transaction size is limited to 16 bytes.
// This macro is used to divide the number of bytes into chunks of 16 bytes.
#define DD_AUX_MAX_SIZE 16

typedef enum _DD_AUX_OPERATIONS
{
    DD_AUX_OPERATION_UNKNOWN = 0,
    DD_AUX_READ,
    DD_AUX_WRITE,
    DD_AUX_I2C_READ,
    DD_AUX_I2C_WRITE,
} DD_AUX_OPERATIONS;

typedef enum _DD_AUX_ERROR_TYPES
{
    DD_AUX_ERROR_NO_ERROR = 0,
    DD_AUX_ERROR_AUX_NACK,
    DD_AUX_ERROR_AUX_DEFER,
    DD_AUX_ERROR_I2C_NACK,
    DD_AUX_ERROR_I2C_DEFER,
    DD_AUX_ERROR_PARTIAL_DATA, // Response received in read or incomplete Write, with Aux ACK|I2C ACK.
    DD_AUX_ERROR_AUX_TIMEOUT,
    DD_AUX_ERROR_HW_TRANSACTION_ERROR, // Aux HW write/receive error, HW Busy, Transaction not complete, Invalid data size
    DD_AUX_ERROR_MAX
} DD_AUX_ERROR_TYPES;

typedef struct _DD_AUX_ARGUMENTS
{
    DD_IN DD_PORT_TYPE DisplayPort;
    DD_IN DD_AUX_OPERATIONS Operation;

    DD_IN DDU32 DPCDAddress;
    DD_IN_OUT DDU8* pBuffer;        // Note: Format of data is little-endian only (usual Intel format)
    DD_IN_OUT DDU16 BufferSize;     // > 1 for burst operation. This is treated as DD_IN_OUT -> DD_OUT to send back the size in case of NACK, native aux max 48 bytes supported, I2C 16 bytes only.
    DD_IN DD_BOOL AksvBufferSelect; // TO select AKSV buffer

    // I2C on AUX specific arguments... (Note: Not complete yet)
    DD_IN DD_BOOL MiddleOfTransaction; // caller can set this if it's middle of a transaction (ie., first transaction or re-start)
    DD_IN DD_BOOL IssueWriteStatusRequest;
    DD_IN DDU8 I2CAddress; // 7-bit format I2C address

    DD_IN DD_DP_AUX_CHANNEL_TYPE AuxChannelType;

    DD_IN DD_BOOL StartTransaction;

    // Temp usage
    DD_BOOL IsCurrentChannelBusy;
    DDU32 ActualBytesReadOrWritten;
    DD_OUT DD_AUX_ERROR_TYPES ErrorCode;
} DD_AUX_ARGUMENTS;

typedef enum _DD_I2CAUX_RETRY_MODE
{
    DD_I2CAUX_RETRY_NONE = 0,
    DD_I2CAUX_RETRY_WITH_SAME_REQUEST,
    DD_I2CAUX_RETRY_WITH_STOP_CYCLE,
} DD_I2CAUX_RETRY_MODE;

// Args for One I2C-over-Aux request Transaction.
typedef struct _DD_I2CAUX_REQ_ARGS
{
    DD_IN DD_PORT_TYPE DisplayPort;
    DD_IN DD_DP_AUX_CHANNEL_TYPE AuxChannelType;
    DD_IN DD_AUX_OPERATIONS I2CAuxCmd;
    DD_IN DDU8 I2CAddress;             // 7-bit format I2C address
    DD_IN_OUT DDU8* pBuffer;           // Note: Format of data is little-endian only (usual Intel format)
    DD_IN DDU8 BufferSize;             // > 1 for burst operation. Max I2C 16 bytes only.
    DD_IN DD_BOOL MiddleOfTransaction; // caller can set this if it's middle of a transaction (ie., first transaction or re-start)
    DD_IN DD_BOOL IssueWriteStatusRequest;
    DD_IN DD_BOOL StartTransaction; // Address-Only Transaction
    DD_IN DD_BOOL IsDdcCiTransfer;  // Used for DDC/CI atomic I2C or MCCS calls. Populated from I2C Flag values. Currently used in I2C Read path.

    DD_IN_OUT DDU8 MaxSize; // Max Burst size for current I2C-over-Aux request Transaction. Updated runtime.
    struct _I2CAUX_RETRY_COUNTERS
    {
        DD_IN_OUT DDU8 I2CNack;     // max TIMEOUT_RETRIES
        DD_IN_OUT DDU8 HwError;     // Retry Count for AUX_TIMEOUT / HW_TRANSACTION_ERROR (max TIMEOUT_RETRIES)
        DD_IN_OUT DDU8 AuxDefer;    // MAX_DEFER_RETRIES
        DD_IN_OUT DDU8 I2CDefer;    // MAX_DEFER_RETRIES
        DD_IN_OUT DDU8 PartialData; // MAX_DEFER_RETRIES
    } RetryCount;

    struct _I2CAUX_OUT_ARGS
    {
        DD_OUT DDU32 ActualBytesReadOrWritten;
        DD_OUT DD_AUX_ERROR_TYPES ErrorCode;
        DD_OUT DD_I2CAUX_RETRY_MODE RetryMode;
    } Out; // Out Args
} DD_I2CAUX_REQ_ARGS;

#define DD_CLEAR_I2CAUX_OUT_ARGS(Args)                                \
    do                                                                \
    {                                                                 \
        DD_ZERO_MEM(&((Args).Out), sizeof((Args).Out));               \
        DD_ZERO_MEM(&((Args).RetryCount), sizeof((Args).RetryCount)); \
    } while (0)

// AUX Write / read data args
typedef struct _DD_AUX_READ_WRITE_DATA_ARGS
{
    DDU8* pRequestBuf;
    DDU8 Length;
    DD_BOOL UseAKSVBuffer;
    DD_DP_AUX_CHANNEL_TYPE AuxChannelType;
    DD_PORT_TYPE Port;
} DD_AUX_READ_WRITE_DATA_ARGS;

//----------------------------------------------------------------------------
//
// AUX/I2C Args -- END
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
// HAL port context data structure to store static data from VBT/Opregion
// etc... which does not change in a given boot
//
//----------------------------------------------------------------------------
typedef struct _DD_PORT_INFO
{
    // DP capable port related data
    DD_PORT_CONNECTOR_TYPE PortConnectorType;
    DD_DP_AUX_CHANNEL_TYPE AuxChannel;
    DD_BOOL IsLaneReversed;

    // HDMI capable port related data
    DDU8 HdmiLevelShifterIndex;

    // MIPI DSI specific data
    DD_MIPI_DSI_LINK_CONFIG MipiDsiLinkConfig;
    DD_TIMING_INFO MipiDsiTimingInfo;

    DD_EDP_SWING_TABLE EdpSwingTableSelector;

    // LFP related data
    DD_BOOL IsLfpDisplay;          // To indicate below data is valid
    DD_BOOL IsBlcSupportedDisplay; // If Backlight control is supported
    DD_PWM_CTRL_TYPE PwmCtrlType;
    DD_PWM_CTRL_NUM PwmCtrlNum;
    DDU32 PwmInverterFreq;
    DD_BOOL IsPwmPolarityInverted;
    DDU32 BrightnessValue; // Post brightness change, Cached Brightness value with 100% precision for B2 or
    DD_PANEL_CUSTOM_ALIGN_DATA LfpVbtPanelCustomAlignData;
    DD_PPS_DELAY_TABLE PpsDelayTable;

    // Cached Brightness value in milli percent for B3 High Precision
    DDU32 BlcPrecisionFactor;            // Post brightness change, cache this value  - to be used during S3/S4
    DD_BOOL IsVswingOverride;            // VSwing Override from VBT Table; 0 - No Use Default VSwing, 1 - Yes, Use Vswing from VBT
    DD_BOOL IsLttpr;                     // Link Training Tunable PHY Repeaters. 0- Not Present, 1 -Present.
    DD_BOOL ReportHpdforInternalDisplay; // HPD is blocked for internal display by default and this can be used to report them if needed
    DDU32 LfpPostBrightness;
    // DD_BOOL IsEdpOnTypC;
    DD_BOOL IsPluggableLfp;
} DD_PORT_INFO;

typedef struct _DD_UPDATE_PORTINFO_ARG
{
    DD_PORT_TYPE Port;
    DD_PORT_INFO PortInfo;
} DD_UPDATE_PORTINFO_ARG;

//////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
//
// HAL related data structures - END
//
//----------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
//
// Generic INLINES -- START
//
//----------------------------------------------------------------------------

DD_INLINE void SetShortFormatInI2CFlags(DD_I2C_FLAGS* pFlags)
{
    pFlags->Value = 0;
    pFlags->Start = 1;
    pFlags->Address = 1;
    pFlags->Index = 1;
    pFlags->Stop = 1;
    return;
}

DD_INLINE void SetCombinedFormatInI2CFlags(DD_I2C_FLAGS* pFlags)
{
    pFlags->Value = 0;
    pFlags->Start = 1;
    pFlags->Address = 1;
    pFlags->Index = 1;
    pFlags->Restart = 1;
    pFlags->Stop = 1;
    return;
}

DD_INLINE void SetCombinedFormatInI2CFlagsForMCCSRead(DD_I2C_FLAGS* pFlags)
{
    pFlags->Value = 0;
    pFlags->Stop = 1;
    pFlags->Address = 1;
    pFlags->Index = 1;
    pFlags->Restart = 1;
    return;
}

DD_INLINE void SetCombinedFormatInI2CFlagsForMCCSWrite(DD_I2C_FLAGS* pFlags)
{
    pFlags->Value = 0;
    pFlags->Start = 1;
    pFlags->Address = 1;
    pFlags->Index = 1;
    pFlags->Stop = 1;
    return;
}

DD_INLINE DD_BOOL _DDIsPortMstCapable(DD_PORT_TYPE Port, DD_PORT_CONFIG_TYPE PortConfig)
{
    if (IS_PHYSICAL_PORT_TYPE(Port))
    {
        // Only on CNL-H and earlier platforms port-E is bifurcated,
        // we need to revisit this code if we are adding DDI-E support for CNL-H or previous platforms.
        // If port is configured as embedded DP/MIPI DSI, then return as not-MST capable
        return ((PORT_CONFIG_DISPLAYPORT_EMBEDDED == PortConfig) || (PORT_CONFIG_MIPI_DSI == PortConfig)) ? FALSE : TRUE;
    }
    else
    {
        return FALSE;
    }
}

// Below function will be used when PixelFormatMask contains a single pixel format (only 1 bit set)
DD_INLINE DD_PIXELFORMAT DD_GET_PIXELFORMAT_FROM_MASK(DDU32 PixelFormatMask)
{
    DD_PIXELFORMAT PixelFormat;
    for (DDU32 Count = 0; Count < DD_MAX_PIXELFORMAT; Count++)
    {
        if (DD_PIXELFORMAT_GETMASK(Count) & PixelFormatMask)
        {
            PixelFormat = (DD_PIXELFORMAT)Count;
            return PixelFormat;
        }
    }
    // this means that we didnt find a matching pixelformat
    DDRLSASSERT_UNEXPECTED(DD_DIAG_FEATURE_OSL_COMMON, "Invalid Pixel Format");
    return DD_B8G8R8X8;
}

DD_INLINE DD_BOOL DD_IS_RGB_PIXELFORMAT(DD_PIXELFORMAT PixelFormat)
{
    DD_BOOL Rgb;
    switch (PixelFormat)
    {
    case DD_8BPP_INDEXED:
    case DD_B5G6R5X0:
    case DD_B8G8R8X8:
    case DD_R8G8B8X8:
    case DD_B10G10R10X2:
    case DD_R10G10B10X2:
    case DD_R10G10B10X2_XR_BIAS:
    case DD_R16G16B16X16F:
        Rgb = TRUE;
        break;
    default:
        Rgb = FALSE;
        break;
    }
    return Rgb;
}

#define DD_IS_YUV_PIXELFORMAT(PixelFormat) !DD_IS_RGB_PIXELFORMAT(PixelFormat)

DD_INLINE DD_BOOL DD_IS_YUV_422_PIXELFORMAT(DD_PIXELFORMAT PixelFormat)
{
    DD_BOOL Yuv422Format;
    switch (PixelFormat)
    {
    case DD_YUV422_8:
    case DD_YUV422_10:
    case DD_YUV422_12:
    case DD_YUV422_16:
        Yuv422Format = TRUE;
        break;
    default:
        Yuv422Format = FALSE;
        break;
    }
    return Yuv422Format;
}

// Bspec Link https://gfxspecs.intel.com/Predator/Home/Index/49325
// Below calculation is useful in finding Minimum Plane Width and Minimum DBUF for a given format

DD_INLINE DDU8 DD_GetYPlaneBPPfromPlanarPixelFormat(DD_PIXELFORMAT PixelFormat)
{
    DDU8 Bpp;
    switch (PixelFormat)
    {
    case DD_NV12YUV420:
        Bpp = 8;
        break;
    case DD_P010YUV420:
    case DD_P012YUV420:
    case DD_P016YUV420:
        Bpp = 16;
        break;
    default:
        DDRLSASSERT_UNEXPECTED(DD_DIAG_FEATURE_OSL_COMMON, "Invalid Pixel format"); // Should not be called for Non Planar Formats
        Bpp = 32;                                                                   // default BPP
        break;
    }
    return Bpp;
}

#define DD_IS_RGB_BPP_PIXELFORMAT(PixelFormat, Bpp) (DD_IS_RGB_PIXELFORMAT(PixelFormat) && (Bpp == DD_GetBPPfromPixelFormat(PixelFormat)))

// Macro to get bpc which could be used for the pipe in HW
// Note: For <= 32bpp case, this has to return 8 always
// This is because effective pipe data is of 8 minimum.
// This helps in back compatibility as well
DD_INLINE DDU8 DD_GetBitsPerColor(DD_PIXELFORMAT PixelFormat)
{
    DDU8 Bpc;

    switch (PixelFormat)
    {
    case DD_B5G6R5X0:
        Bpc = 6;
        break;
    case DD_YUV422_8:
    case DD_NV12YUV420:
    case DD_8BPP_INDEXED:
    case DD_B8G8R8X8:
    case DD_R8G8B8X8:
    case DD_YUV444_8:
        Bpc = 8;
        break;
    case DD_R10G10B10X2:
    case DD_B10G10R10X2:
    case DD_R10G10B10X2_XR_BIAS:
    case DD_P010YUV420:
    case DD_YUV444_10:
    case DD_YUV422_10:
        Bpc = 10;
        break;
    case DD_P012YUV420:
    case DD_YUV444_12:
    case DD_YUV422_12:
        Bpc = 12;
        break;
    case DD_R16G16B16X16F:
    case DD_P016YUV420:
    case DD_YUV444_16:
    case DD_YUV422_16:
        Bpc = 16;
        break;
    default:
        DDRLSASSERT_UNEXPECTED(DD_DIAG_FEATURE_OSL_COMMON, "Invalid Pixel format");
        Bpc = 8; // default
        break;
    }
    return Bpc;
}
// PixelFormat related Macros end here.

// To Return the number of Pipe for an incoming Pipe Joined Mode, can be either 2 or 4 based on ModeType.
DD_INLINE DDU8 DD_GET_NUM_PIPES_IN_PIPE_JOINED_MODE(DD_MODE_TYPE ModeType)
{
    DDRLSASSERT_EXPECT_EQ(DD_IS_PIPE_JOINED_MODE(ModeType), TRUE, DD_DIAG_FEATURE_MP_COMMON, "Pipe Joined ModeType should be Big-Joiner/Ultra-Joiner");

    // As of today, 2 pipes can be supported in pipe joined mode on all Platforms represented by DD_PIPE_JOINED_MODE_BIG_JOINER
    // ModeType. In BMG and Xe3 along with the 2 pipe joined mode support also has the support for combining all 4 pipes for a pipe joined
    // mode represented by DD_PIPE_JOINED_MODE_ULTRA_JOINER ModeType.
    return ((DD_PIPE_JOINED_MODE_ULTRA_JOINER == (ModeType)) ? 4 : 2);
}

// To return the dot clock calculated based on the color depth
DD_INLINE DDU64 DD_GET_DOTCLOCK_FOR_COLORDEPTH(DDU64 Freq, DDU8 Bpc)
{
    // add more conditions for higher BPCs when platforms support them (e.g 14, 16 etc)
    if (DD_COLOR_DEPTH_12BPC == Bpc)
    {
        return ((Freq * PIXEL_CLOCK_MULTIPLIER_FOR_12BPC_IN_HUNDRED) / DD_CENT);
    }
    else if (DD_COLOR_DEPTH_10BPC == Bpc)
    {
        return ((Freq * PIXEL_CLOCK_MULTIPLIER_FOR_10BPC_IN_HUNDRED) / DD_CENT);
    }
    else // 8 BPC
    {
        return Freq; // (Freq * 8BPC) / 8BPC = Freq
    }
}

//----------------------------------------------------------------------------
//
// Generic INLINES -- END
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
// To be cleaned up
//
//----------------------------------------------------------------------------

// Still used by PC code

/*
#define GRM_PIPEA_BIT 1
#define GRM_PIPEB_BIT 2
#define GRM_PIPEC_BIT 4

#define GRM_DISPLAYA_BIT 1
#define GRM_DISPLAYB_BIT 2
#define GRM_DISPLAYC_BIT 4
*/
// query display details args structure

// Display details flag enum
typedef enum _DD_DISPLAY_DETAILS_FLAG
{
    DD_QUERY_DISPLAYUID = 1,
    DD_QUERY_DISPLAYTYPE_INDEX
} DD_DISPLAY_DETAILS_FLAG;

typedef enum _DD_DISPLAY_TYPE
{
    // DONOT change the order of type definitions
    // Add new types just before MAX_DISPLAY_TYPES & increment value of MAX_DISPLAY_TYPES
    DD_DISPLAY_TYPE_NONE = 0,
    DD_DISPLAY_TYPE_CRT,
    DD_DISPLAY_TYPE_DFP,
    DD_DISPLAY_TYPE_LFP,
    DD_DISPLAY_TYPE_MAX = DD_DISPLAY_TYPE_LFP,
} DD_DISPLAY_TYPE;

typedef struct _DD_QUERY_DISPLAY_DETAILS_ARGS
{
    // eflag = QUERY_DISPLAYUID -> Indicates that Display Type & Index will be sent & we need to return DisplayUID & bExternalEncoderDriven
    // eflag = QUERY_DISPLAYTYPE_INDEX -> Indicates that DisplayUID will be sent & we need to return  Display Type ,Index & bExternalEncoderDriven
    DD_IN DD_DISPLAY_DETAILS_FLAG Flag;

    DD_IN_OUT DDU32 DisplayUID;
    // #ifndef _COMMON_PPA
    DD_IN_OUT DD_DISPLAY_TYPE Type;
    // #endif
    DD_IN_OUT DDU8 Index;

    // Is display ID driven by external encoder?
    DD_OUT DD_BOOL ExternalEncoderDriven; // Includes both sDVO and NIVO Displays

    DD_OUT DD_BOOL TPVDrivenEncoder;

    // Type of Port Used.
    DD_OUT DD_PORT_TYPE PortType;

    // This interprets logical port mapping for physical connector.
    // This indicates mapping multiple encoders to the same port
    DD_OUT DDU8 LogicalPortIndex;
} DD_QUERY_DISPLAY_DETAILS_ARGS;

typedef struct _DD_REG_MEDIA_REFRESH_RATE_SUPPORT
{
    union
    {
        DDU32 Value;
        struct
        {
            DDU32 Rr24 : 1;                  // Bit 0 - 24 Hz
            DDU32 Rr25 : 1;                  // Bit 1 - 25 Hz
            DDU32 Rr30 : 1;                  // Bit 2 - 30 Hz
            DDU32 Rr48 : 1;                  // Bit 3 - 48 Hz
            DDU32 Rr50 : 1;                  // Bit 4 - 50 Hz
            DDU32 Rr60 : 1;                  // Bit 5 - 60 Hz
            DDU32 Reserved1 : 9;             // Bit 6:14
            DDU32 FractionalRrSupported : 1; // Bit 15 - Fractional RR supported for all supported RRs
            DDU32 Reserved2 : 15;            // Bit 16:30
            DDU32 Reserved3 : 1;             // Bit 31 -> INF override bit, This was for internal debug purpose and not needed anymore as we can debug all issues via ETLs. De-featuring this
        };
    };
} DD_REG_MEDIA_REFRESH_RATE_SUPPORT;

typedef struct _DD_AUDIO_HDCP_PARAMS
{
    PIPE_ID PipeId;
    DD_BOOL IsGetEncryptionPref;
    DD_BOOL EncPreference;
    DD_BOOL CPStatus;
} DD_AUDIO_HDCP_PARAMS;

typedef struct _DD_WAKELOCK_PARAMS
{
    DD_WAKELOCK_THREAD WakelockThread;
    DD_WAKELOCK_REQUEST RequestedOp;
} DD_WAKELOCK_PARAMS;

// Capability Mask
typedef union _DD_HDCP_CAP_MASK
{
    DDU32 Value;
    struct
    {
        DDU32 UNIQUENAME(Reserved) : DD_BITFIELD_RANGE(0, 2); // Unused bits
        DDU32 ProtectionTypeHDCP : DD_BITFIELD_BIT(3);        // HDCP capability
        DDU32 UNIQUENAME(Reserved) : DD_BITFIELD_BIT(4);
        DDU32 ProtectionTypeHDCPTypeEnforcement : DD_BITFIELD_BIT(5); // HDCP Type enforcement capability
        DDU32 UNIQUENAME(Reserved) : DD_BITFIELD_RANGE(6, 31);        // Unused Bits
    };
} DD_HDCP_CAP_MASK;

typedef struct _DD_HDCP_KSV_LIST
{
    DDU32 KSVListLength; // Length of the  KSV list (set)
    PKSV_T pKSVList;     // List of KSVs (set)
} DD_HDCP_KSV_LIST;

typedef struct _DD_GET_HDCP_STATUS_ARGS
{
    DD_IN DD_TARGET_DESCRIPTOR TgtDesc;
    DD_IN DD_PIPE_DETAILS PipeDetails;
    DD_OUT DD_HDCP_CAP_MASK CapMask;
    DD_OUT DDU32 Level;
    DD_OUT DD_BOOL IsiHDCP2; // Not used if LSPCON
    DD_OUT DD_BOOL IsRepeater;
    // AKSV, BKSV, KSVList
    DD_OUT KSV_T AKSV;              // KSV of attached device
    DD_OUT KSV_T BKSV;              // KSV of attached device
    DD_IN DD_HDCP_KSV_LIST SRMData; // SRM revokation data
} DD_GET_HDCP_STATUS_ARGS;

typedef struct _DD_HDCP_GET_CACHED_DEVICE_INFO_ARGS
{
    DD_IN DD_TARGET_DESCRIPTOR TgtDesc;
    DD_OUT DD_HDCP_VERSION HdcpVersion;
    DD_OUT DDU32 Level;
    DD_OUT DD_HDCP_CAP_MASK CapMask;
    DD_OUT KSV_T BKSV; // KSV of attached device
    DD_OUT DD_BOOL IsRepeater;
    // Below fields are valid if 'IsRepeater == true'
    DD_OUT DD_BOOL HasHdcp2_0_Repeater; // DD_BOOL indicating whther the downstream topology has HDCP 2_0 Repeater
    DD_OUT DD_BOOL HasHdcp1_Device;     // DD_BOOL indicating whther the downstream topology has HDCP 1 Device
    DD_OUT DDU8 RptrDeviceCount;        // Number of Repeaters in the down stream. Valid only if bRepeater is TRUE
    DD_OUT DDU8* pReceiverIdList;       // Pointer to List of Receiver Ids, Valid only if IsRepeater is TRUE. Optional Arg. Allocated by caller, size MAX_HDCP_DEVICES * RECEIVER_ID_SIZE.
    DD_OUT DDU8 RptrDepth;
    DD_OUT DD_BOOL MaxCascadeExceeded; // Repeater error
    DD_OUT DD_BOOL MaxDevsExceeded;    // Repeater error
} DD_HDCP_GET_CACHED_DEVICE_INFO_ARGS;

typedef struct _DD_STREAM_INFO
{
    DD_IN DD_PROTOCOL_TYPE ProtocolEnabled;
    DD_IN DDU8 StreamCount;
    struct
    {
        DD_IN DDU8 StreamId;
        DD_IN PIPE_ID PipeId;
    } StreamInfo[DD_MAX_DP_MST_CONNECTOR_PER_PORT];
} DD_STREAM_INFO;

typedef struct _DD_SET_HDCP_STATUS_ARGS
{
    DD_IN DD_TARGET_DESCRIPTOR TgtDesc;
    DD_IN DD_PIPE_DETAILS PipeDetails;
    DD_IN_OUT DDU32 Level;
    DD_IN_OUT DD_HDCP_KSV_LIST KSVFifo; // downstream KSV data
    /** SRM Data. */
    DD_IN DD_HDCP_KSV_LIST SRMData;  // SRM revokation data
    DD_IN DD_BOOL Internal;          // Whether to HDCP disable is initiated internally
    DD_IN DD_BOOL TriggerAuthToSrvc; // Whether an authentication trigger to be sent to Service.
    DD_OUT DD_BOOL IsAsyncNeeded;    // Whether asynchronous authentication is required
    DD_IN DD_STREAM_INFO StreamDetails;
} DD_SET_HDCP_STATUS_ARGS;

typedef struct _DD_HDCP_ENCRYPTION_STATUS_ARGS
{
    DD_IN DD_HDCP_VERSION HDCPVersion;
    DD_IN DD_PROTOCOL_TYPE Protocol;
    DD_IN_OUT DDU32 Level;
    DD_IN_OUT DD_BOOL IsLinkEncryptionEnabled;
    DD_IN_OUT DD_BOOL IsHdcpKeyLoadFailed;
    // SRM Data
} DD_HDCP_ENCRYPTION_STATUS_ARGS;

typedef struct _DD_PORT_HDCP_ARGS
{
    DD_IN PIPE_ID Pipe;
    DD_IN DD_PORT_TYPE Port;
    DD_IN DD_BOOL Enable;
} DD_PORT_HDCP_ARGS;

typedef struct _DD_SET_HDCP_STREAM_STATUS_ARGS
{
    DD_IN DD_HDCP_VERSION HDCPVersion;
    DD_IN DD_BOOL Enable;
} DD_SET_HDCP_STREAM_STATUS_ARGS;

typedef enum _HDCP1_CIPHER_SRVC
{
    HDCP_KEY_LOAD_TRIGGER,
    HDCP_GET_STATUS,
    HDCP_SET_ENCRYPTION,
    HDCP1_GENERATE_AN,
    HDCP1_UPDATE_BKSV,
    HDCP1_WAIT_FOR_RI_READY,
    HDCP1_VERIFY_RI_PRIME,
    HDCP1_UPDATE_REPEATER_STATE,
    HDCP1_COMPUTE_V,
    HDCP1_VERIFY_V_PRIME,
    HDCP_SET_STREAM_ENCRYPTION,

} HDCP1_CIPHER_SRVC;
typedef struct _DD_HDCP_CIPHER_SRVC_ARGS
{
    DD_IN PIPE_ID Pipe;
    DD_IN PIPE_ID PrimaryPipe;
    DD_IN DD_PORT_TYPE Port;
    DD_IN HDCP1_CIPHER_SRVC Service;
    DD_IN_OUT DDU8* pBuffer;
    DD_IN_OUT DDU32 DataSize;
    // SRM Data
} DD_HDCP_CIPHER_SRVC_ARGS;
//
// BSTATUS
//
/* typedef union _HDCP_RX_BSTATUS
{
    DDU16 Value;
    struct
    {
        DDU16 DeviceCount : 7; // bit 6:0
        DDU16 MaxDevsExceeded : 1; // bit 7
        DDU16 Depth : 3; // bit 10:8
        DDU16 MaxCascadeExceeded : 1; // bit 11
        DDU16 RXInHDMIMode : 1; // bit 12
        DDU16 Rserved : 3; // bit 15:13
    };
}HDCP_RX_BSTATUS;*/

typedef struct _DD_HDCP1_COMPUTE_V_ARGS
{
    DD_IN DDU8* pKSVList;
    DD_IN DDU32 KSVBufferSize;
    DD_IN DDU16 TopologyData;
} DD_HDCP1_COMPUTE_V_ARGS;

// Audio related interface structures
typedef union _N_AUD_VALUE
{
    struct
    {
        DDU32 NAudBits_11to0 : DD_BITFIELD_RANGE(0, 11);
        DDU32 NAudBits_19to12 : DD_BITFIELD_RANGE(12, 19);
        DDU32 NAudBits_23to20 : DD_BITFIELD_RANGE(20, 23);
        DDU32 UNIQUENAME(Reserved) : DD_BITFIELD_RANGE(24, 31); // RESERVED
    };
    DDU32 Value;
} N_AUD_VALUE;

typedef struct _AUDIO_EARLY_HBLANK_ENABLE_PARAMS
{
    DD_BOOL DscSupport;
    DD_TIMING_INFO* pTimingInfo;
    DDU32 DpLaneWidth;
    DDU32 LinkClockInKhz;
    DD_BITS_PER_PIXEL BitsPerPixel;
} AUDIO_EARLY_HBLANK_ENABLE_PARAMS;

typedef struct _AUDIO_ENDPOINT_INDICATE_PARAMS
{
    DD_BOOL IsEnable;
    DDU8 AudioTimeStampVersion; // Per DP 1.1 and 1.2 this is DisplayPort version, per DP 1.4 and 2.0 this is Audio Time stamp version
    void* pELD;
    PIPE_ID Pipe;
    DDU64 DotClockHz;
    DD_TARGET_DESCRIPTOR* pTargetDesc;
    N_AUD_VALUE NAudValue;
    AUDIO_EARLY_HBLANK_ENABLE_PARAMS AudHblankEnableParams;
    DD_HDMI_FRLRATE MaxFrlRateInMbps;
    DD_BOOL IsFrlMode;
    DD_BOOL AudSDPSplitSupport;
} AUDIO_ENDPOINT_INDICATE_PARAMS;

typedef struct _AUDIO_NOTIFICATION_PARAMS
{
    DD_BOOL AudioSupportedInCurrentTopology;
    DD_BOOL AudioSupportedInFutureTopology;
    DD_BOOL PreModeSet;
    DDU32 NumAudioPanelsInNextConfig;
} AUDIO_NOTIFICATION_PARAMS;

typedef enum _AUDIO_CODEC_DRIVER_STATUS
{
    AUDIO_CODEC_DRIVER_STATUS_UNKNOWN = 0,
    AUDIO_CODEC_DRIVER_INTEL_LOADED,
    AUDIO_CODEC_DRIVER_NOT_LOADED,
    AUDIO_CODEC_DRIVER_MSFT_LOADED
} AUDIO_CODEC_DRIVER_STATUS;

typedef enum _DISPLAYAUDIO_LINK_HANDLING_EVENT
{
    DISPLAYAUDIO_LINK_HANDLING_EVENT_PRE_SGPC_F0 = 0,
    DISPLAYAUDIO_LINK_HANDLING_EVENT_POST_SGPC_F1,
    DISPLAYAUDIO_LINK_HANDLING_EVENT_POST_POWERWELL_OFF,
    DISPLAYAUDIO_LINK_HANDLING_EVENT_POST_POWERWELL_ON,
    DISPLAYAUDIO_LINK_HANDLING_EVENT_PRE_MODE_SET,
} DISPLAYAUDIO_LINK_HANDLING_EVENT;

typedef enum _AUDIO_POWERWELL_NOTIFICATION_TYPE
{
    AUDIO_PG_NO_NOTIFICATION = 0, // Not in use currently, keeping it for future reference
    AUDIO_PG_ON_NOTIFICATION,
    AUDIO_PG_OFF_NOTIFICATION
} AUDIO_POWERWELL_NOTIFICATION_TYPE;

// Color Related
// Color OSL-HAL structes
typedef struct _DD_COLOR_PIPE_MATRIX_ARGS
{
    DD_COLOR_BLENDING_MODE BlendingMode;
    DD_COLOR_MATRIX_CONFIG* pMatrixConfig;
    DD_BOOL UseFlipQ; // used by HAL to determine if Matrix update should be done via FlipQ
} DD_COLOR_PIPE_MATRIX_ARGS;

typedef struct _DD_COLOR_PIPE_LUT_ARGS
{
    DD_COLOR_BLENDING_MODE BlendingMode;
    DD_COLOR_1DLUT_CONFIG* p1DLUTConfig;
    DD_BOOL IsWaitForVblankRequired;
    DD_BOOL Psr2PrSfSupported; // used by HAL layer for one scanline wait, only for SU capable panels
    DD_BOOL UseFlipQ;          // used by HAL to determine if LUT update should be done via FlipQ
} DD_COLOR_PIPE_LUT_ARGS;

typedef struct _DD_COLOR_PIPE_OUTPUT_CONFIG
{
    DD_BOOL Enabled;
    DD_BOOL IsPipePassThroughMode;
    DDU8 NumPipes;
    PIPE_ID PipeId[MAX_TILES_SUPPORTED];
    DD_COLOR_PIXEL_DESC PipeOutColorFormat;
} DD_COLOR_PIPE_OUTPUT_CONFIG;

typedef struct _DD_COLOR_PIPE_OUTPUT_CONFIG_ARGS
{
    DD_COLOR_BLENDING_MODE BlendingMode;
    DD_COLOR_PIPE_OUTPUT_CONFIG* pPipeOutputConfig;
    DD_BOOL IsLfpPanel;
} DD_COLOR_PIPE_OUTPUT_CONFIG_ARGS;

/**
 * @brief Display features that may require a specific display power well.
 */
typedef enum _DD_FEATURE_POWERWELLS
{
    AUDIO_POWERWELL = 0,
    DSC_POWERWELL,
    MAX_FEATURE_POWERWELLS
} DD_FEATURE_POWERWELLS;

/**
 * @brief  Display power well configuration arguments.
 *
 * The bit field defines the display components to enable,
 * powerwells are enabled accordingly.
 */
typedef struct _DD_POWERWELL_CONFIG_ARGS
{
    union
    {
        DDU32 EnablePowerwells;
        struct
        {
            DDU32 EnablePipeA : 1;             // Bit 0
            DDU32 EnablePipeB : 1;             // Bit 1
            DDU32 EnablePipeC : 1;             // Bit 2
            DDU32 EnablePipeD : 1;             // Bit 3
            DDU32 EnableDdiPortA : 1;          // Bit 4
            DDU32 EnableDdiPortB : 1;          // Bit 5
            DDU32 EnableDdiPortC : 1;          // Bit 6
            DDU32 EnableDdiPortD : 1;          // Bit 7
            DDU32 EnableDdiPortE : 1;          // Bit 8
            DDU32 EnableDdiPortF : 1;          // Bit 9
            DDU32 EnableDdiPortG : 1;          // Bit 10
            DDU32 EnableDdiPortH : 1;          // Bit 11
            DDU32 EnableDdiPortI : 1;          // Bit 12
            DDU32 EnableAudio : 1;             // Bit 13
            DDU32 EnableDSC : 1;               // Bit 14
            DDU32 EnableModesetInProgress : 1; // Bit 15
            DDU32 EnableMipiDualLink : 1;      // Bit 16
            DDU32 Reserved : 15;               // Bit31-Bit17 - reserved
        };
    };
} DD_POWERWELL_CONFIG_ARGS;

/***************************************************************
 * @brief  Display DC state restriction.
 ***************************************************************/
typedef enum _DD_DC_STATE_RESTRICTIONS
{
    DD_DC_STATE_PIPE_CONFIG = 0,
    DD_DC_STATE_VSYNC_ENABLED,
    DD_DC_STATE_IDLE,
    DD_DC_STATE_AUDIO_CODEC_LOADED,
    DD_DC_STATE_3D_LUT_INACTIVE,
    DD_DC_STATE_LOWPOWER_PIPE,
    DD_DC_STATE_POWER_STATE_CHANGE,
    DD_DC_STATE_PSR_PR_SUPPORTED, // DD_DC_STATE_PSR_PR_SUPPORTED is common restriction used for DC5/6
    DD_DC_STATE_LACE_INACTIVE,
    DD_DC_STATE_RR_CHANGE_WITH_VBI_MASKED,
    DD_DC_STATE_CMTG_ENABLED,
    DD_DC_STATE_SCANLINE_INTR,
    DD_DC_STATE_ASYNC_FLIP,
    DD_DC_STATE_HW_FLIPQ_ENABLED,
    DD_DC_STATE_DARK_SCREEN_DETECTION_INACTIVE,
    DD_DC_STATE_PSR2_ENABLED,
    DD_DC_STATE_MAX_RESTRICTIONS
} DD_DC_STATE_RESTRICTIONS;

/***************************************************************
 * @brief  Display Pipe Feature Mask.
 ***************************************************************/
typedef union DD_DISPLAY_PIPE_FEATURE_MASK
{
    DDU32 PipeFtrMask;
    struct
    {
        DDU32 PSR_1 : 1;      // Bit 0
        DDU32 PSR_2 : 1;      // Bit 1
        DDU32 PR : 1;         // Bit 2
        DDU32 DPST : 1;       // Bit 3
        DDU32 LACE : 1;       // Bit 4
        DDU32 FBC : 1;        // Bit 5
        DDU32 Mask_3DLUT : 1; // Bit 6
        DDU32 VDSC : 1;       // Bit 7
        DDU32 Scaler_1 : 1;   // Bit 8
        DDU32 Scaler_2 : 1;   // Bit 9
        DDU32 MSO : 1;        // Bit 10
        DDU32 Reserved : 21;  // Bit31-Bit11 - reserved
    };
} DD_DISPLAY_PIPE_FEATURE_MASK;

/***************************************************************
 * @brief This structure contains Pipe Mask per Display Pipe feature.
 ***************************************************************/
typedef struct _DD_DISPLAY_FEATURE_PIPE_MASK_PARAMS
{
    PIPE_ID Pipe;                                 // (in)
    DD_DISPLAY_PIPE_FEATURE_MASK PipeFeatureMask; // (out)
} DD_DISPLAY_PIPE_FEATURE_MASK_PARAMS;

/***************************************************************
 * @brief Pipe OpCode Enum.
 ***************************************************************/
typedef enum _DD_GET_DISPLAY_HW_PIPE_OPERATION
{
    DD_GET_FEATURE_PIPE_MASK = 0,
    DD_GET_PIPE_CAPS,
} DD_GET_DISPLAY_HW_PIPE_OPERATION;

/***************************************************************
 * @brief per Pipe capabilities.
 ***************************************************************/
typedef struct _DD_DISPLAY_HW_PIPE_CAPS
{
    DDU32 MaxPlaneSurfaceHeight;
    DDU32 MaxPlaneSurfaceWidth;
    DDU32 MinPlaneSurfaceHeight;
    DDU32 MinPlaneStrideInBytes;
    DDU32 MinPlaneSizeIncrement;
    DDU32 MaxCusHorizontalPlaneSize;
    DDU32 MinCusSourceSurfaceWidth;
    DDU32 MinCusSourceSurfaceHeight;
    DDU32 MinScalerSourceSurfaceWidth;
    DDU32 MinScalerSourceSurfaceHeight;
    DDU32 MinScalerSourcePlanarSurfaceHeight;
    DDU32 NumPipeDeGammaBlockBefore3DLut;
    DDU32 NumPipeGammaBlockBefore3DLut;
    DDU32 NumPipeCscBlockBefore3DLut;
    DDU32 MinHActive;
    DDU32 MaxHActive;
} DD_DISPLAY_HW_PIPE_CAPS;

/***************************************************************
 * @brief structure with both Pipe Mask per Display Pipe feature and per Pipe caps.
 ***************************************************************/
typedef struct _DD_DISPLAY_HW_PIPE_ARGS
{
    DD_GET_DISPLAY_HW_PIPE_OPERATION OpCode;
    union
    {
        DD_DISPLAY_PIPE_FEATURE_MASK_PARAMS PipeFeatureMaskParams;
        DD_DISPLAY_HW_PIPE_CAPS HwPipeCaps;
    };
} DD_DISPLAY_HW_PIPE_ARGS;

/***************************************************************
 * @brief  This structure contains bitfields for diffrent
 *              DC state restriction.
 ***************************************************************/
typedef union DISPLAY_DC_STATE_RESTRICTION
{
    DDU32 DisplayDcStateRestrictionMask;
    struct
    {
        /** Bit 0: PipeConfEvent.    */
        DDU32 PipeConfEvent : 1;
        /** Bit 1: VsyncEnabled.     */
        DDU32 VsyncEnabled : 1;
        /** Bit 2: Idle.     */
        DDU32 Idle : 1;
        /** Bit 3: AudioCodecLoaded. */
        DDU32 AudioCodecLoaded : 1;
        /** Bit 4: _3DLutInactive.     */
        DDU32 _3DLutInactive : 1;
        /** Bit 5: LowPowerPipe.     */
        DDU32 LowPowerPipe : 1;
        /** Bit 6: PowerStateChange. */
        DDU32 PowerStateChange : 1;
        /** Bit 7: PSR1/PSR2/PanelReplay Supported. */
        DDU32 PsrPrSupported : 1;
        /** Bit 8: LaceInactive. */
        DDU32 LaceInactive : 1;
        /** Bit 9: HRR/NRR Inactive. */
        DDU32 VbiUnmasked : 1;
        /** Bit 10: Cmtg Enabled .*/
        DDU32 CmtgEnabled : 1;
        /** Bit 11: Scanline Interrupt Disabled .*/
        DDU32 ScanlineInterruptDisabled : 1;
        /** Bit 12: Async Flip .*/
        DDU32 AsyncFlipDisabled : 1;
        /** Bit 13: HW FlipQ Enabled.*/
        DDU32 HwFlipQEnabled : 1;
        /** Bit 14: Dark screen detection inactive.*/
        DDU32 DarkScreenDetectionInactive : 1;
        /** Bit 15: PSR 2 restriction is used for D13/D14.*/
        DDU32 Psr2Enabled : 1;
        /** Bit 16-31: Reserved .    */
        DDU32 Reserved : DD_BITFIELD_RANGE(16, 31);
    };
} DISPLAY_DC_STATE_RESTRICTION;

typedef union _DD_DISPLAY_PC_FEATURE_CONTROL_FLAGS_2
{
    DDU32 Value;
    struct
    {
        DDU32 DisableDmcPeriodicAsSdp : 1; // Bit 0
        DDU32 DisableHwPeriodicAsSdp : 1;  // Bit 1
        /** Bit 2-31: Reserved .    */
        DDU32 Reserved : DD_BITFIELD_RANGE(2, 31);
    };
} DD_DISPLAY_PC_FEATURE_CONTROL_FLAGS_2;

C_ASSERT(sizeof(DD_DISPLAY_PC_FEATURE_CONTROL_FLAGS_2) == sizeof(DDU32));

typedef union _DD_DISPLAY_PC_FEATURE_CONTROL_FLAGS
{
    DDU32 Value;
    struct
    {
        DDU32 DisablePixelClkSupportOnVrrPanel : 1; // Bit 0 - Obsolete
        DDU32 DispPcFtrCtrlReserved : 8;            // Bits (1:8)
        DDU32 UNIQUENAME(Reserved) : 1;             // Bit 9
        DDU32 DisablePsr2SelectiveFetch : 1;        // Bit 10
        DDU32 DisablePanelReplaySelectiveFetch : 1; // Bit 11
        DDU32 DisableFbcSelectiveFetch : 1;         // Bit 12
        DDU32 DisableDC5 : 1;                       // Bit 13
        DDU32 DisableDC6 : 1;                       // Bit 14
        DDU32 DisableDC9 : 1;                       // Bit 15
        DDU32 DisableCappedFps : 1;                 // Bit 16
        DDU32 UNIQUENAME(Reserved) : 1;             // Bit 17
        DDU32 DisableDC6v : 1;                      // Bit 18
        DDU32 DisableDpPanelReplay : 1;             // Bit 19
        DDU32 DisableCdClkBy2Change : 1;            // Bit 20
        DDU32 DisableDmrrsInternalDisplay : 1;      // Bit 21
        DDU32 DisableDmrrsExternalDisplay : 1;      // Bit 22
        DDU32 DisableTconBacklightOptimization : 1; // Bit 23
        DDU32 DisableEnduranceGaming : 1;           // Bit 24
        DDU32 DisableOpst : 1;                      // Bit 25
        DDU32 DisableFbcPsr2Coexistence : 1;        // Bit 26
        DDU32 DisableDeepPkgC8 : 1;                 // Bit 27
        DDU32 DisableFsscSupport : 1;               // Bit 28 Full Screen Solid Color support
        DDU32 DisableEdpPanelReplay : 1;            // Bit 29
        DDU32 DisableAlrr : 1;                      // Bit 30
        DDU32 DisableLobf : 1;                      // Bit 31
    };
} DD_DISPLAY_PC_FEATURE_CONTROL_FLAGS;

C_ASSERT(sizeof(DD_DISPLAY_PC_FEATURE_CONTROL_FLAGS) == sizeof(DDU32));

/***************************************************************
 * @brief  This structure contains bitfields for diffrent
 *              types of color event.
 *
 ***************************************************************/
typedef union DISPLAY_COLOR_EVENT_TYPE
{
    DDU8 DisplayColorEventType;
    struct
    {
        /** Bit 0: ColorChange.    */
        DDU8 ColorChange : 1;
        /** Bit 1: _3DLutActive.   */
        DDU8 _3DLutActive : 1;
        /** Bit 2: GammaChangeInProgress.     */
        DDU8 GammaChangeInProgress : 1;
        /** Bit 3-7: Reserved.     */
        DDU8 Reserved : 5;
    };
} DISPLAY_COLOR_EVENT_TYPE;

DD_INLINE DD_BOOL IsYCbCrFormat(DD_COLOR_MODEL ColorModel)
{
    DD_BOOL retVal = FALSE;

    switch (ColorModel)
    {
    case DD_COLOR_MODEL_YCBCR_601:
    case DD_COLOR_MODEL_YCBCR_709:
    case DD_COLOR_MODEL_YCBCR_2020:
        retVal = TRUE;
        break;
    default:
        retVal = FALSE;
        break;
    }

    return retVal;
}

// Display LACE definitions
typedef enum _DD_LACE_OPS
{
    DD_LACE_ENABLE = 0,
    DD_LACE_DISABLE,
    DD_LACE_GET_HIST_BINS,
    DD_LACE_SET_IE,         // This operation Updates the IETs and Applies them.
    DD_LACE_UPDATE_IE_DATA, // This operation Only Updates the IET DATA but won't APPLY.
    DD_LACE_APPLY_IE_DATA,  // This operation Only Applies the IETs.
} DD_LACE_OPS;

typedef enum _DD_LACE_IE_MODE
{
    LACE_IE_MODE_LUT = 0,
    LACE_IE_MODE_MULTIPLICATIVE
} DD_LACE_IE_MODE;

typedef enum _DD_LACE_ORIENTATION
{
    LACE_ORIENTATION_LANDSCAPE = 0, // 16x9 tile arrangement
    LACE_ORIENTATION_PORTRAIT       // 9x16 tile arrangement
} DD_LACE_ORIENTATION;

typedef enum _DD_LACE_TILE_SIZE
{
    LACE_LARGE_TILE_SIZE = 0, // Tile Size 256x256
    LACE_SMALL_TILE_SIZE      // Tile Size 128x128
} DD_LACE_TILE_SIZE;

#define LACE_HISTOGRAM_NUM_BINS 32              // 32 histogram bins per tile
#define LACE_IE_LUT_LEN 33                      // 33 DIET entries per tile
#define LACE_MAX_IE_INDEX (LACE_IE_LUT_LEN - 1) // Max LUT Index
#define LACE_IET_WRITE_LEN 34                   // Number of IET Entries sent to HAL and programmed in HW.

#define LACE_MAX_TILES_X 16                                      // Maximum horizontal tile count (in landscape mode).
#define LACE_MAX_TILES_Y 9                                       // Maximum vertical tile count (in landscape mode).
#define LACE_MAX_NUM_TILES (LACE_MAX_TILES_X * LACE_MAX_TILES_Y) // Maximum tile count

#define LACE_GET_HISTOGRAM_BIN_START(pHistogram, Row, Col, NumOfTilesPerRow) (pHistogram + ((Row * NumOfTilesPerRow + Col) * LACE_HISTOGRAM_NUM_BINS))
#define LACE_GET_IET_LUT_START(pIet, Row, Col, NumOfTilesPerRow) (pIet + ((Row * NumOfTilesPerRow + Col) * LACE_IET_WRITE_LEN))

typedef struct _DD_LACE_PROGRAM_ARGS
{
    IN PIPE_ID PipeId;
    IN DD_LACE_OPS LaceOp;

    union
    {
        struct
        {
            IN DD_LACE_ORIENTATION Orientation;
            IN DD_LACE_TILE_SIZE TileSize;
        } Enable;

        struct
        {
            IN DDU32 TilesPerRow;
            IN DDU32 TilesPerColumn;
            OUT DDU32* pHbin; // pointer to the start of Entire Histogram Bins.
            OUT DDU16* pIet;  // Pointer to the start of Entire IET data.
        } GetSetTileData;
    };

} DD_LACE_PROGRAM_ARGS;

typedef struct _DD_CONTAINERID_PROTOCOL_ARGS
{
    struct
    {
        DD_IN DDU64 PortId;
        DD_IN DDU16 ManufacturerName;
        DD_IN DDU16 ProductCode;
    } EldInfo;
} DD_CONTAINERID_PROTOCOL_ARGS;

typedef struct _DD_LACE_PLATFORM_CAPS
{
    OUT DDU16 MaxNumTilesPerRow;
    OUT DDU16 MaxNumTilesPerCol;
    OUT DDU8 NumOfPipesSupportLACE; // If NumOfPipesSupportLACE value is 1, it means LACE is supported on PIPE A.
    // If NumOfPipesSupportLACE value is 2, it means LACE is supported on PIPE A and PIPE B.
} DD_LACE_PLATFORM_CAPS;

typedef struct _DD_FBC_GET_SET_STATE_ARGS
{
    PIPE_ID PipeId; // Filled by DVM
    DD_BOOL IsSetFbc;
    DD_BOOL IsEnable;
    DD_BOOL WaitForVblank;    // Valid only if IsSetFbc is TRUE
    DD_BOOL NukeForFbcEnable; // Trigger Nuke for Fbc enable
    DD_BOOL FbcSfPossible;
    DD_BOOL IsPavpSessionActive;
    DD_BOOL IsUpdateFbcRestriction;
} DD_FBC_GET_SET_STATE_ARGS;

typedef enum _DD_DSB_OPCODE
{
    DSB_NOOP = 0,
    DSB_OP_MMIO_WRITE,
    DSB_OP_WAIT_FOR_USEC,
    DSB_OP_WAIT_FOR_N_LINES,
    DSB_OP_WAIT_FOR_VBLANKS,
    DSB_OP_WAIT_FOR_LINE_IN_RANGE,
    DSB_OP_WAIT_FOR_LINE_OUT_OF_RANGE,
    DSB_OP_GENERATE_INTERRUPT,
    DSB_OP_GENERATE_FRMAE_START, // Unused. Removed from RTL
    DSB_OP_INDEXED_REG_WRITE,
    DSB_OP_POLL_FUCNTION,
    DSB_OP_WAIT_FOR_HBLANKS,
    DSB_OP_GOSUB,
    DSB_OP_MAX
} DD_DSB_OPCODE;

typedef union _DD_DSB_INSTRUCTION
{
    struct
    {
        DDU32 Offset : 20;
        DDU32 ByteEn : 4;
        DDU32 Opcode : 8;
    };
    DDU32 Value;

} DD_DSB_INSTRUCTION;

C_ASSERT(4 == sizeof(DD_DSB_INSTRUCTION));

typedef union _DD_DSB_SCANLINE_RANGE
{
    DDU64 Data;
    struct
    {
        DDU64 LowerLimit : 20;
        DDU64 UpperLimit : 20;
        DDU64 Reserved : 24;
    };
} DD_DSB_SCANLINE_RANGE;
C_ASSERT(8 == sizeof(DD_DSB_SCANLINE_RANGE));

typedef union _DD_DSB_GOSUB
{
    DDU64 Data;
    struct
    {
        DDU64 TailPointer : 26;
        DDU64 HeadPointer : 26;
        DDU64 Reserved : 12;
    };
} DD_DSB_GOSUB;
C_ASSERT(8 == sizeof(DD_DSB_GOSUB));

typedef union _DSB_TRANSACTION
{
    struct
    {
        DDU32 PayLoad;
        DD_DSB_INSTRUCTION Instruction; // The gfx address of the cacheline buffer // TBD pick a better data type
    };
    DD_DSB_SCANLINE_RANGE ScanlineRange;
    DD_DSB_GOSUB Gosub;
} DSB_TRANSACTION;
C_ASSERT(8 == sizeof(DSB_TRANSACTION));

typedef struct _DD_DSB_MMIO_TRANSACTION
{
    DDU32 Data;
    DD_DSB_INSTRUCTION Instruction;
} DD_DSB_MMIO_TRANSACTION;
C_ASSERT(sizeof(DD_DSB_MMIO_TRANSACTION) == sizeof(DD_OFFSET_DATA_PAIR));

typedef struct _DD_UPDATE_CUSTOM_SCALING_FACTOR_ARGS
{
    DD_IN PIPE_ID PipeId;
    DD_IN DD_CUSTOM_SCALING_DETAILS CustomScalingDetails;
} DD_UPDATE_CUSTOM_SCALING_FACTOR_ARGS;

typedef struct _DD_WBDEVICE_HPD_ARGS
{
    DD_BOOL HotPlug;                  // Plug or Unplug
    DDU32 DeviceId;                   // Target id of the device that is plugged in or needs unplug
    DD_2DREGION Resolution;           // Mode
    HANDLE hWbSurfaceHandle;          // Surface Address input given by App
    HANDLE hNotifyScreenCaptureEvent; // Screen capture Event that will be set when the capture is complete.
    DD_VIDEO_OUTPUT_TECHNOLOGY OutputTechnology;
    DDU8 SinkIndex;
    DD_BOOL OverrideDefaultEdid;
    DDU8 EdidData[WB_EDID_BLOCK_SIZE];
} DD_WBDEVICE_HPD_ARGS;

typedef struct _DD_WB_CAPTURE_ARGS
{
    DDU8 SinkIndex;
    DDU32 DeviceId;                   // Child Id
    DD_2DREGION Resolution;           // Mode that is applied
    DD_PIXELFORMAT PixelFormat;       // Pixel Format
    SURFACE_MEMORY_TYPE MemoryFormat; // tiling
    DDU32 BufferSize;                 // This will contain size of Buffer.Input can be 0 inorder to query the buffer size.
    DDU8* pWdBuffer;                  // Will contain the captured buffer.
} DD_WB_CAPTURE_ARGS;

typedef struct _DD_PLATFORM_STEPPING
{
    DDU8 Stepping;
    DDU8 SubStepping;
} DD_PLATFORM_STEPPING;

// Brightness Operation to be performed on DD_GETSET_BRIGHTNESS_ARGS
typedef enum _DD_GETSET_BRIGHTNESS_OPERATION
{
    DD_GET_BRIGHTNESS = 0,
    DD_SET_BRIGHTNESS,
    DD_SET_AGRESSIVENESS_LEVEL,
    DD_SET_BRIGHTNESS_CTRL,
    DD_SET_PANEL_LUMINANCE_OVERRIDE,
} DD_GETSET_BRIGHTNESS_OPERATION;

typedef struct _DD_GETSET_BRIGHTNESS_ARGS
{
    DD_TARGET_DESCRIPTOR TgtDesc;                   // Target descriptor for nits brightenss operation
    DD_GETSET_BRIGHTNESS_OPERATION OpType;          // Get brightness/ set brightness/ set aggresiveness level/ Set nits brightness control/ Set Panel Luminance.
    DDU32 BrightnessValue;                          // Brightness units in millinits for edp and in 0 - 0xFFFF duty cycle mapping for Mipi DCS Brightness
    DDU32 FrameCount;                               // Number of frames over which the brightness change should be applied smoothly
    DD_NITS_BRIGHTNESS_AGGRESSIVENESS AggrSettings; // Used when eOpType is DD_SET_AGGRESIVENESS_LEVEL
    DDU32 MaxPanelLuminanceInMilliNits;             // Indicates Maximum Panel Luminance. Used when OpType is DD_SET_NITS_PANEL_LUMINANCE_OVERRIDE
    DDU32 MinPanelLuminanceInMilliNits;             // Indicates Minimum Panel Luminance. Used when OpTypeis DD_SET_NITS_PANEL_LUMINANCE_OVERRIDE
    DDU32 MaxFullFrameLuminanceInMilliNits;         // Indicates Maximum Full Frame Luminance. Used when OpType is DD_SET_NITS_PANEL_LUMINANCE_OVERRIDE
    DD_BOOL EnableAuxBrightnessControl;
    DD_BOOL IsHdrSupported;
    DD_BOOL IsVesaBasedBrightnessControl;
    DDU32 SmoothTransitionTimeInMs;
} DD_GETSET_BRIGHTNESS_ARGS;

typedef struct _DD_SET_HDR_PIXEL_BOOST_ARGS
{
    DDU32 PipeId;
    DDU32 SdrContentLuminance;                // In nits
    DDU32 CurrentDisplayLuminanceInMilliNits; // In Milli Nits
} DD_SET_HDR_PIXEL_BOOST_ARGS;

typedef struct _DD_EDP_AUXSENSE_VBT_PARAM
{
    DDU8 NumEdpEncodersInVbt;
    DD_PORT_TYPE PortType[DD_MAX_INTERNAL_DISPLAYS];                 // Port configured for each eDP
    DD_PWM_CTRL_NUM PwmCtrlNum[DD_MAX_INTERNAL_DISPLAYS];            // PWM Control number configured in VBT. Used to get PPS registers to force enable VDD.
    DD_DP_AUX_CHANNEL_TYPE AuxChannelType[DD_MAX_INTERNAL_DISPLAYS]; // Aux Channel configured for each eDP
} DD_EDP_AUXSENSE_VBT_PARAM;

typedef enum _DD_DPLL_TYPE
{
    DD_DPLL_NONE,
    DD_DPLL_0,
    DD_DPLL_1,
    DD_DPLL_2,
    DD_DPLL_3,
    DD_DPLL_4,
    DD_DPLL_5,
    DD_DPLL_TBT,
} DD_DPLL_TYPE;

typedef struct _DD_PLL_ARGS
{
    DD_DPLL_TYPE PllType;
    DD_BOOL Enable;
    DD_PORT_TYPE Port;
    PIPE_ID PipeId;
    DD_VIDEO_OUTPUT_TECHNOLOGY SinkType;
    DDU32 LinkRateMbps;
    DD_BOOL SscClock;
    DDU64 DotClockHz;
    DD_BOOL IsHdmiFrlMode;
    DDU32 FrlRatePerLaneMbps;
    DDU32 RefClock;
    DD_BOOL IsDualLinkMipiDsi;
    DD_BOOL PortSyncEnable; // Comes from OS to enable Port Sync Enable feature for Mipi Dsi
    DDU32 LaneCount;
    DD_BOOL IsGenlockSupported;
    DD_BOOL IsGenlockEnabled;
    DDU8 IsGenLockPrimarySystem;
    DDU8 IsGenLockPrimaryTranscoder;
} DD_PLL_ARGS;

typedef struct _DD_EDID_OVERRIDE_ARGS
{
    DDU8 NumOfExtensionBlocks;
    DDU8* pOldEdidData;
    DDU8* pEdidData;
    DD_TARGET_DESCRIPTOR* pTgtDesc;
    DDU8 NumBlocksToOverride;
    DDU32 BlocksToOverrideMask;
    DD_EDID_TYPE EdidType;
} DD_EDID_OVERRIDE_ARGS;

typedef struct _DD_TARGET_DIAGNOSTIC_DATA
{
    DDU32 TargetId;
    DD_TARGET_DESCRIPTOR TargetDesc[MAX_TILES_SUPPORTED];
    DDU32 NumPipesUsed;
    DDU32 PipeId[MAX_TILES_SUPPORTED];
    DD_SOURCE_MODE_INFO SourceMode;
    DD_TIMING_INFO TargetMode;
    DD_SCALING Scaling;
} DD_TARGET_DIAGNOSTIC_DATA;

typedef struct _DD_GET_EDID_ARGS
{
    DD_IN DDU32 EdidOffset;
    DD_IN_OUT DDU32 EdidLengthInBytes;
    DD_OUT DD_BOOL IsDisplayID;          // 0 for EDID , 1 for DisplayID
    DD_OUT DDU8* pEdid;                  // edid buffer
    DD_OUT DDU8 NumEdidExtensionsParsed; // Number of extension block successfully parsed
    DD_OUT DD_EDID_TYPE EdidType;
} DD_GET_EDID_ARGS;

typedef struct _DD_PANEL_DATA
{
    DD_PANEL_DATA_OP Op;
    DD_TARGET_DESCRIPTOR TargetDesc;
    union
    {
        DD_GET_EDID_ARGS GetEdidArgs;            // Edid Data to get/set
        DD_DISPLAY_SHIFT_DPCD_BUFFER DpcdBuffer; // DPCD buffer to get or set
    };
} DD_PANEL_DATA;

typedef struct _DD_PC_INIT_ARGS
{
    DD_BOOL IsRegisteredForOsPowerPlanNotifications;
    DD_BOOL IsCapsValid[LFP_MAX];
    DD_LFP_ENCODER_INFO* pLfpEncoderInfo;
    DD_PC_BRIGHTNESS_OPTIMIZATION_FTRS BrightnessOptCaps[LFP_MAX];
    DD_BASIC_DISPLAY_CAPS BasicDisplayCaps[LFP_MAX];
    DD_BLC_FEATURES BlcDisplayCaps[LFP_MAX];
    DD_LACE_FEATURE_CAPS LaceCaps[LFP_MAX];
    DD_PSR_PR_CAPS PsrPrDisplayCaps[LFP_MAX];
    DD_HDR_CAPS HdrCaps[LFP_MAX];
    DD_FTR_SUPPORT FtrSupport[LFP_MAX];
} DD_PC_INIT_ARGS;

// FBC restriction info.
// Do not change or delete the order of this enum
// Note  If any changes done, manifest file should also be updated with the same
typedef enum _DD_FBC_RESTRICTION_TYPE
{
    UNKNOWN_RESTRICTION = 0,
    FBC_PLANE_DISABLED,
    PLANE_ORIENTATION_EITHER_90OR270,
    PLANE_IS_NOT_RGB_OR_B8G8R8X8,
    PLANE_LARGER_THAN_SCREEN_RESOLUTION,
    PLANE_STRIDE_LARGER_THAN_DISPLAY_STRIDE,
    BPP_NOT_SUPPORTED,
    LINE_WIDTH_NOT_SUPPORTED,
    HEIGHT_NOT_SUPPORTED,
    LINEAR_TILE_MEMORY_NOT_SUPPORTED,
    PLANE_STRIDE_INCORRECT,
    ASYNC_FLIP,
    PSR_RESTRICTION,
    SINGLE_PLANE_RESTRICTION,
    DIRTY_RECTANGLE_AREA_RESTRICTION,
    PAVP_ACTIVE_SESSION_RESTRICTION
} DD_FBC_RESTRICTION_TYPE;

// struct DD_GET_SPI_DATA: Returns events for Short pulse HPD
typedef struct _DD_GET_SPI_DATA
{
    DD_IN DD_PORT_TYPE Port;
    DD_IN DDU64 PeakModePixelBwMbpsWoTunnelingRestriction;
    DD_IN DD_HF_VSDB_INFO HfVsdbInfo;
    DD_IN DD_HDMI_VERSION HdmiVersion;
    DD_IN DD_BOOL IsSUCapable;
    DD_IN DD_BOOL IsPanelReplayEnabledEndToEnd;
    DD_IN DD_BOOL IsDp2Hdmi2_1PconPresent;
    DD_IN DD_BOOL IsTunnelingBmMode;
    DD_IN DD_BOOL CheckMstEventsOnly;
    DD_OUT DD_SPI_EVENTS Event;
} DD_GET_SPI_DATA;

// FBC State Change Information
// Please do not change, if any changes done same thing has to be done in manifest file
typedef enum _DD_FBC_STATE_CHANGE
{
    DISABLE = 0,
    ENABLE,
    DISABLE_PENDING,
    ENABLE_PENDING,
    NUKE_PENDING,
    NUKE_COMPLETED
} DD_FBC_STATE_CHANGE;

typedef enum _DD_PHY_POWER_STATE
{
    DD_PHY_POWER_STATE_0 = 0,
    DD_PHY_POWER_STATE_1,
    DD_PHY_POWER_STATE_2,
    DD_PHY_POWER_STATE_3
} DD_PHY_POWER_STATE;

typedef struct _DD_NOTIFY_CONFIG_PM_ARGS
{
    DDU8 NumActivePipes;
    DD_TARGET_DESCRIPTOR TargetDesc[MAX_PHYSICAL_PIPES];

} DD_NOTIFY_CONFIG_PM_ARGS;

typedef enum _DD_PC_FTR_BRT_OPTIMIZATION_INDEX
{
    DD_PC_FTR_BRT_OPTIMIZATION_DPST = 0, // Intel DPST with Backlight control
    DD_PC_FTR_BRT_OPTIMIZATION_CABC,     // Panel TCON specific Content Adaptive Brightness Control mechanism
    DD_PC_FTR_BRT_OPTIMIZATION_OPST,     // Intel OLED Power Saving Technology
    DD_PC_FTR_BRT_OPTIMIZATION_ELP,      // TCON based Edge Luminance Profile
    DD_PC_FTR_BRT_OPTIMIZATION_EPSM,     // Extra power saving mode for DPST
    DD_PC_FTR_BRT_OPTIMIZATION_APD,      // Adaptive Pixel Dimming, TCON based OLED pixel dimming, similar to ELP
    DD_PC_FTR_BRT_OPTIMIZATION_PIXOPTIX, // TCON+ based DPST like solution
    DD_PC_FTR_BRT_OPTIMIZATION_MAX
} DD_PC_FTR_BRT_OPTIMIZATION_INDEX;

typedef enum _DD_PC_FTR_BRT_OPTIMIZATION_CONCURRENCY_MASK
{
    DD_PC_FTR_BRT_OPTIMIZATION_CONCURRENCY_MASK_NONE = 0,
    DD_PC_FTR_BRT_OPTIMIZATION_CONCURRENCY_MASK_DPST = __BIT(DD_PC_FTR_BRT_OPTIMIZATION_DPST),
    DD_PC_FTR_BRT_OPTIMIZATION_CONCURRENCY_MASK_CABC = __BIT(DD_PC_FTR_BRT_OPTIMIZATION_CABC),
    DD_PC_FTR_BRT_OPTIMIZATION_CONCURRENCY_MASK_OPST = __BIT(DD_PC_FTR_BRT_OPTIMIZATION_OPST),
    DD_PC_FTR_BRT_OPTIMIZATION_CONCURRENCY_MASK_ELP = __BIT(DD_PC_FTR_BRT_OPTIMIZATION_ELP),
    DD_PC_FTR_BRT_OPTIMIZATION_CONCURRENCY_MASK_EPSM = __BIT(DD_PC_FTR_BRT_OPTIMIZATION_EPSM),
    DD_PC_FTR_BRT_OPTIMIZATION_CONCURRENCY_MASK_APD = __BIT(DD_PC_FTR_BRT_OPTIMIZATION_APD),
    DD_PC_FTR_BRT_OPTIMIZATION_CONCURRENCY_MASK_PIXOPTIX = __BIT(DD_PC_FTR_BRT_OPTIMIZATION_PIXOPTIX),
    DD_PC_FTR_BRT_OPTIMIZATION_CONCURRENCY_MASK_MAX = 0x80000000
} DD_PC_FTR_BRT_OPTIMIZATION_CONCURRENCY_MASK;

#define DD_UPDATE_DSB_BUFFER_CONTIGIOUS_DATA(pDataBuffer, pRegisterCount, RegData) \
    {                                                                              \
        pDataBuffer[*pRegisterCount] = RegData;                                    \
        (*pRegisterCount)++;                                                       \
    }

// ELP UI aggressiveness levels
typedef enum _DD_ELP_OPTIMIZATION
{
    DD_ELP_OPTIMIZATION_OFF,
    DD_ELP_OPTIMIZATION_BEST_QUALITY,
    DD_ELP_OPTIMIZATION_BALANCED,
    DD_ELP_OPTIMIZATION_BEST_POWER_SAVING
} DD_ELP_OPTIMIZATION;

// APD UI aggressiveness levels
typedef enum _DD_APD_OPTIMIZATION
{
    DD_APD_OPTIMIZATION_OFF,
    DD_APD_OPTIMIZATION_BEST_QUALITY,
    DD_APD_OPTIMIZATION_BALANCED,
    DD_APD_OPTIMIZATION_BEST_POWER_SAVING
} DD_APD_OPTIMIZATION;

typedef enum _DD_CSC_MODE_CONVERSION_TYPE
{
    DD_CSC_MODE_CONVERSION_NONE = 0,
    DD_CSC_MODE_CONVERSION_RGB_TO_YCBCR,
    DD_CSC_MODE_CONVERSION_YCBCR_TO_RGB,
    DD_CSC_MODE_CONVERSION_RGB_TO_RGB
} DD_CSC_MODE_CONVERSION_TYPE;

typedef struct _DD_CSC_CONVERSION_PARAMS
{
    DD_COLOR_PIPE_MATRIX_PARAMS BaseMatrix;
    DD_CSC_RANGE_CONVERSION_TYPE CSCRangeConversionType;
} DD_CSC_CONVERSION_PARAMS;

typedef struct _DD_HDR_SMOOTH_BRIGHTNESS_TIMER_ARGS
{
    DDU32 TargetId;
    struct _DISPLAY_CONTEXT* pDisplayContext;
} DD_HDR_SMOOTH_BRIGHTNESS_TIMER_ARGS;

typedef struct _DD_DISPLAY_HDR_SMOOTH_BRIGHTNESS_CONTEXT
{
    DDU32 TargetBrightnessInMilliNits;
    DDU32 CurrentAppliedBrightnessInMilliNits;
    DDS32 DeltaBrightnessChangeInEachPhase;
    DD_PERIODIC_TIMER_OBJECT* pHDRTimerObject; // Pointer to the Timer Object
    DD_HDR_SMOOTH_BRIGHTNESS_TIMER_ARGS HDRSmoothBrightnessTimerArgs;
} DD_DISPLAY_HDR_SMOOTH_BRIGHTNESS_CONTEXT;

typedef struct _DD_TARGET_COLOR_CONTEXT
{
    DD_COLOR_BLENDING_MODE BlendingMode;
    DD_HDR_STATIC_METADATA HDRDefaultMetaData;
    DD_COLOR_3DLUT_CONFIG* p3DLUTData;             // 3d LUT data needs to be store for inactive targets as well
    DDU32 SdrWhiteLevel;                           // SDR White level for the target in HDR mode in nits.
    DDU32 CurrentDisplayBrightnessInMilliNits;     // Currently intended brightness in MilliNits from OS for integrated panel today, may be extended for external panels in future.
    DD_HDR_STATIC_METADATA LastAppliedHDRMetadata; // Keep track of the Metadata Applied in HW
    // Smooth Brightness Context
    DD_DISPLAY_HDR_SMOOTH_BRIGHTNESS_CONTEXT HDRSmoothBrightnessContext;
    DD_BOOL IsAuxBasedEdpHdr;
    DD_HDR_CAPS HdrCaps;
} DD_TARGET_COLOR_CONTEXT;

typedef struct _DD_DSB_BLOCK_INFO
{
    DDU32 GfxAddress;
    DDU64 pCpuVirtualAddress;
    DDU32 Size;
} DD_DSB_BLOCK_INFO;

// Intermediate structure to pass arguments from OSL/Escape to DPL/HAL
typedef struct _DD_VSIF_PARAMS
{
    DD_TARGET_DESCRIPTOR TgtDesc;
    DD_HDR10P_SOURCE_TM SourceToneMapping;
    DD_HDR10P_LOW_LATENCY_MODE LowLatency;
} DD_VSIF_PARAMS;

// These are error injection that can be used to simulate various failures.
// These are expected to be used only for interop testing.
typedef union _DD_DISPLAY_ERROR_SIMULATION_FLAGS
{
    DDU32 Value;
    struct
    {
        DDU32 EdidReadFail : 1;           // Bit 0 - Fail EDID read
        DDU32 AuxFailure : 1;             // Bit 1 - Fail native AUX
        DDU32 I2COverAuxFailure : 1;      // Bit 2 - Fail I2C Over AUX
        DDU32 ModeEnumerationFailure : 1; // Bit 3 - Fail Mode Enum, report zero modes
        DDU32 ModeSetFailure : 1;         // Bit 4 - Fail Mode Set
        DDU32 DscEnableFailure : 1;       // Bit 5 - Fail DSC enable
        DDU32 LinkTrainingFailure : 2;    // Bit 6, 7 - Fail link training, counter to fail multiple times.
        DDU32 UNIQUENAME(Reserved) : 24;  // Bit 25
    };
} DD_DISPLAY_ERROR_SIMULATION_FLAGS;

// For Default Color Override Preference by OEMs for LFPs
typedef union _DD_INF_OVERRIDE_OUTPUTFORMAT_PREFERENCE
{
    DDU32 RegkeyDwordValue;
    struct
    {
        DDU32 BpcPreference : 8; // 8 Bits to Set preffered bpc value (8/10/12 ,others values Invalid)
        DDU32 Reserved : 24;     // Bit 9-31 Reserved for future Enhancements
    };
} DD_INF_OVERRIDE_OUTPUTFORMAT_PREFERENCE;

typedef struct _DD_BLOCK_PACKAGE_C_ARGS
{
    PIPE_ID PipeID;    // Pipe index
    DD_BOOL BlockPkgC; // Block Package C
} DD_BLOCK_PACKAGE_C_ARGS;

#endif // _DISPLAY_INTERNAL_
