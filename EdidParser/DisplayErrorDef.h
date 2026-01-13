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
 * @file  DisplayErrorDef.h
 * @ingroup driver
 * @brief List of Display error codes to be used by 3 layers of Display driver.
 *
 */

#ifndef _DISPLAY_ERROR_H_
#define _DISPLAY_ERROR_H_

#define DD_ERROR_CODE_VER 1 // bump up rev when we modify the file

/**
 * @brief @c DDSTATUS error code used across all display driver modules to return function status.
 *
 * The error code has below major categories
 * 1. generic failure
 * 2. DSL (Display OS Layer) error
 * 3. DPL (Display Protocol layer) error
 * 4. DHL (Display HW-Abstraction layer) error
 * 5. Success codes
 *
 * Any new error code should be added to the end of the list for that category w/o affecting old enum values.
 */
typedef enum _DDSTATUS
{
    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////
    //
    //      start of generic failure codes   0x8000 0000 - 0x8000 00FF
    //
    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////
    DDS_UNSUCCESSFUL = (DDS32)0x80000000, // -2147483648
    DDS_NO_MEMORY,
    DDS_INVALID_PARAM,
    DDS_NULL_PARAM,
    DDS_TIMEOUT,
    DDS_I2C_DEVICE_DOES_NOT_EXIST,
    DDS_I2C_DATA_READ_ERROR,
    DDS_I2C_DATA_WRITE_ERROR,
    DDS_BUFFER_TOO_SMALL,
    DDS_BUFFER_OVERFLOW,
    DDS_SET_TIMING_MONITOR_NOT_CONNECTED,
    DDS_ERROR_UNKNOWN,
    DDS_BUSY,
    DDS_NOT_SUPPORTED,
    DDS_DATA_OVERFLOW,
    DDS_DEVICE_NOT_READY,
    DDS_MONITOR_NOT_CONNECTED,
    DDS_INTERFACE_NOT_IMPLEMENTED,
    DDS_INVALID_DATA,
    DDS_INVALID_CONTEXT,
    DDS_DATA_ERROR,
    DDS_INVALID_LEVEL,
    DDS_INVALID_OPERATION,
    DDS_CRC_ERROR,
    DDS_LOCKING_FAILURE,
    DDS_NACK_RECVD,
    DDS_NO_RESOURCE,
    DDS_INVALID_ESC_VER,
    DDS_INVALID_REQ,
    DDS_USER_APC,
    DDS_ABANDONED,
    DDS_NO_RECOMMEND_FUNCTIONAL_VIDPN,
    DDS_MONITOR_NO_MORE_DESCRIPTOR_DATA,
    DDS_MONITOR_NO_DESCRIPTOR_DATA,
    DDS_OBJECT_NAME_NOT_FOUND,
    DDS_FEATURE_SKU_DISABLED,
    DDS_WRITEBACK_NOT_SUPPORTED,
    DDS_INVALID_PATH,
    DDS_INVALID_COLOR_ENCODING,
    DDS_MPO_ACTIVE_LEGACY_OVERLAY_NOT_SUPPORTED,
    DDS_VIRTUAL_PIPE_ACTIVE,
    DDS_3D_LUT_NOT_SUPPORTED_IN_HDR,
    DDS_CSME_IO_TRANSACTION_FAILURE,
    DDS_CSME_IO_OPERATION_FAILURE,
    DDS_FORCED_IS_ACTIVE_OVERLAY_NOT_SUPPORTED,
    DDS_UNSUPPORTED_SCALING,
    DDS_SYNCRONIZATION_FAILED,
    DDS_ACCESS_DENIED,
    DDS_GENLOCK_NOT_SUPPORTED,
    DDS_DISPLAY_NOT_ACTIVE,
    DDS_DMRRS_NOT_POSSIBLE,
    DDS_VRR_NOT_SUPPORTED,
    DDS_LRR_NOT_SUPPORTED,
    DDS_DMRRS_NOT_SUPPORTED,
    DDS_INVALID_ADAPTER,
    DDS_HW_CURSOR_NOT_POSSIBLE,
    DDS_UNSUPPORTED_TARGET,
    DDS_EDID_CORRUPTED,
    DDS_GENLOCK_NO_COMMON_MODE,
    DDS_GENLOCK_NON_IDENTICAL_DP_SINK_CAPS,
    DDS_INVALID_TARGET_ID,
    DDS_INVALID_DISPLAY_COUNT_FOR_GENLOCK,
    // add new generic failure code above this line
    __DD_STATUS_RANGE_GENERIC_FAILURE_MAX, /* max allowed value = 0x80000100,  range limit, *not* to be used in coding */

    /************************************************************************
     *
     *  start of OSL status codes        0x8000 0102 - 0x8FFF FFFF.
     *
     ************************************************************************/

    __DD_STATUS_RANGE_OSL_START = (DDS32)0x80000101, // range limit, *not* to be used in coding
    DDS_INVALID_PC_EVENT,

    __DD_STATUS_RANGE_OSL_FLIP_START = __DD_STATUS_RANGE_OSL_START + 0x100, // range limit, *not* to be used in coding

    // VRR Related Error codes
    DDS_OSL_VRR_DC_BAL_COUNTER_MISMATCH,

    DDS_OSL_LAYER_NOT_ACTIVE,

    // add new flip related error codes above this line
    __DD_STATUS_RANGE_OSL_FLIP_MAX, // max allowed value = 0x80000300, range limit, *not* to be used in coding

    __DD_STATUS_RANGE_OSL_DVM_START = __DD_STATUS_RANGE_OSL_FLIP_START + 0x100, // range limit, *not* to be used in coding
    DDS_DVM_MPO_ON_COLLAGE_NOT_SUPPORTED,
    DDS_DVM_MPO_MULTIPIPE_PATH_NOT_SUPPORTED,
    DDS_DVM_MODE_NOT_FOUND,
    DDS_DVM_MPO_ON_ROTATION_NOT_SUPPORTED_FOR_MULTIPIPE_DISPLAYS,
    DDS_DVM_PIPE_NOT_AVAILABLE,
    DDS_DVM_MPO_NOT_SUPPORTED_FOR_LOCKED_DISPLAY,
    // add new DVM error codes above this line
    __DD_STATUS_RANGE_OSL_DVM_MAX, // max allowed value = 0x80000400, range limit, *not* to be used in coding

    // add new OSL error code above this line
    __DD_STATUS_RANGE_OSL_MAX, /* max allowed value = 0x90000000, range limit, *not* to be used in coding */

    /************************************************************************
     *
     *  start of protocol status codes   0x90000002 - 0x9FFF FFFF .
     *
     ************************************************************************/

    __DD_STATUS_RANGE_DPL_START = (DDS32)0x90000001, // range limit, *not* to be used in coding
    DDS_DPL_VBT_NO_ENCODERS_FOUND,
    DDS_DPL_VBT_PARSING_ERROR,
    DDS_DPL_INVALID_MONITOR_DESC_TAG,
    DDS_DPL_INVALID_VIC_ID,
    DDS_DPL_INVALID_VBT_VERSION,
    DDS_DPL_INVALID_VBT_DATA,
    DDS_DPL_DETECTION_INCOMPLETE,
    DDS_DPL_INVALID_PORT,
    DDS_DPL_INVALID_PIPE,
    DDS_DPL_INVALID_PROTOCOL,
    DDS_DPL_INVALID_CONNECTOR,
    DDS_DPL_FAILED_HAL_PROGRAMMING,
    DDS_DPL_INVALID_HAL_DATA,
    DDS_DPL_FAILED_EDID_READ,
    DDS_DPL_EDID_PARSE_ERROR,
    DDS_DPL_NO_SINK,
    DDS_DPL_INVALID_DSC_PARAMS,
    DDS_DPL_SCANOUT_HUNG,
    DDS_DPL_BLOCK_NOT_FOUND,

    // add new generic DPL code above this line.
    __DD_STATUS_RANGE_DPL_GENERIC_MAX,
    //
    // --------------------------------------
    // DP Protocol error codes
    //
    __DD_STATUS_RANGE_DPL_DP_START = __DD_STATUS_RANGE_DPL_START + 5000,
    DDS_DPL_DP_AUX_FAILURE,
    DDS_DPL_DP_FAILED_BASIC_DPCD_CAPS,
    DDS_DPL_DP_FAILED_EXTENDED_DPCD_CAPS,
    DDS_DPL_DP_FAILED_EDP_DPCD_CAPS,
    DDS_DPL_DP_FAILED_LTTPR_DPCD_CAPS,
    // DP link training related
    DDS_DPL_DP_INVALID_LINK_RATE,
    DDS_DPL_DP_INVALID_LANE_COUNT,
    DDS_DPL_DP_LINK_TRAINING_FAILED,
    DDS_DPL_DP_LT_CLOCK_RECOVERY_FAILED,
    DDS_DPL_DP_LT_FAILED_TO_GET_NEXT_LINKRATE,
    DDS_DPL_DP_LT_MAX_CLK_REC_ITER_REACHED,
    DDS_DPL_DP_LT_CHAN_EQ_FAILED,
    DDS_DPL_DP_LT_MAX_EQ_ITER_REACHED,
    DDS_DPL_DP_GET_REQUESTED_DRIVE_SETTING_FAILED,
    DDS_DPL_DP_ADJUST_DRIVE_SETTING_FAILED,
    DDS_DPL_DP_INSUFFICIENT_LINK_BW,
    DDS_DPL_DP_FAILED_LINK_CONFIG_SET,
    DDS_DPL_DP_FAILED_SET_ASSR_CFG,
    DDS_DPL_DP_FAILED_DPCD_READ,
    DDS_DPL_DP_FAILED_DPCD_WRITE,
    DDS_DPL_DP_LINK_LOST,
    DDS_DPL_DP_LINK_LOST_POST_MODESET,
    DDS_DPL_DP_LQA_FAILED,
    DDS_DPL_DP_ACT_HANDLING_ERROR,
    DDS_DPL_DP_PREPARE_FOR_ENABLE_FAILED,
    DDS_DPL_DP_DSC_CONFIG_ENABLE_FAILED,
    DDS_DPL_DP_LTTPR_LINK_TRAIN_FAILED,
    DDS_DPL_DP_FAILED_VRR_CONFIG_SET,
    DDS_DPL_DP_FEC_ENABLE_FAILED,
    DDS_DPL_DP_FEC_STATUS_NOT_UPDATED_BY_DPRX,
    // DP sideband related
    DDS_DPL_DP_SIDEBAND_MSG_ID_MISMATCH,
    DDS_DPL_DP_SIDEBAND_INVALID_RAD,
    DDS_DPL_DP_SIDEBAND_INVALID_HEADER,
    DDS_DPL_DP_SIDEBAND_SEND_MSG_FAILURE,
    DDS_DPL_DP_SIDEBAND_REPLY_TIMEOUT,
    DDS_DPL_DP_SIDEBAND_INVALID_REPLY,
    DDS_DPL_DP_SIDEBAND_READ_MSG_FAILURE,
    DDS_DPL_DP_SIDEBAND_INVALID_HEADER_CRC,
    DDS_DPL_DP_SIDEBAND_INVALID_BODY_CRC,
    // stream / BANDWIDTH related
    DDS_DPL_FAILED_TO_ENABLE_STREAM,
    DDS_DPL_FAILED_SET_BANDWIDTH,

    // EDP PSR Sepcific
    DDS_SINK_PSR_NOTSUPPORTED,
    DDS_GTC_SET_CONTROL_FAILED,
    DDS_PSR_DISABLE_FAILED,
    DDS_PSR_CONTEXT_NOT_INITIALIZED,
    DDS_DPL_DP_LT_CLK_REC_SAME_ADJ_MAX_ITER_REACHED,
    DDS_DPL_DP_LT_CLK_REC_MAX_VSWING_REACHED,

    // EDP MSO specific
    DDS_DPL_MSO_MODE_NOT_SUPPORTED,

    DDS_DPL_DP_INVALID_BRANCH,

    DDS_DPL_DP_FAILED_DISABLE_DISPLAY,

    DDS_DPL_DP_DSC_INVALID_SLICECOUNT,
    DDS_DPL_DP_DSC_INVALID_SLICEWIDTH,
    DDS_DPL_DP_INVALID_DSC_BPP,

    DDS_DPL_DP_TUNNEL_BW_FAILURE,
    DDS_DPL_DP_INSUFFICIENT_TUNNEL_BW,
    DDS_PANELREPLAY_ENABLED,
    DDS_PANELREPLAY_DISABLED,
    DDS_PANELREPLAY_ENABLE_FAILED,
    // PCON related
    DDS_DPL_PCON_HDMI_SINK_LT_FAILED,
    DDS_DPL_DP_LT_CLOCK_DATA_SWITCH_FAILED,
    DDS_DPL_DP_LT_IN_PROGRESS,
    // add new DP error code above this line
    __DD_STATUS_RANGE_DPL_DP_MAX,
    //
    // --------------------------------------
    // HDCP Protocol error codes
    //
    __DD_STATUS_RANGE_DPL_HDCP_START = __DD_STATUS_RANGE_DPL_DP_START + 2000,
    DDS_DPL_HDCP_REVOKED_DEVICE_ATTACHED,
    DDS_DPL_HDCP_INVALID_KSV,
    DDS_DPL_HDCP_SEND_AN_FAILED,
    DDS_DPL_HDCP_RECEIVE_BKSV_FAILED,
    DDS_DPL_HDCP_RECEIVE_RI_FAILED,
    DDS_DPL_HDCP_LINK_INTEGRITY_FAILED,
    DDS_DPL_HDCP_UNSUPPORTED_SINK,
    DDS_DPL_HDCP_RI_MISMATCH,
    DDS_DPL_HDCP_MAX_TOPOLOGY_EXCEEDED,
    DDS_DPL_HDCP_INVALID_SEQ_NUM,
    DDS_DPL_HDCP_SEQ_NUM_ROLL_OVER,
    // add new HDCP error code above this line
    __DD_STATUS_RANGE_DPL_HDCP_MAX,
    //---------------------------------
    //
    // HDMI protocol related
    __DD_STATUS_RANGE_DPL_HDMI_START = __DD_STATUS_RANGE_DPL_HDCP_START + 1000,
    DDS_DPL_HDMI_INVALID_SINK_VERSION,
    DDS_DPL_HDMI_SCRAMBLING_ENABLE_FAILED,
    DDS_DPL_HDMI21_MODE_NOT_SUPPORTED,
    DDS_DPL_HDMI_FRL_LINKTRAINING_FAILED,
    DDS_DPL_HDMI_PREPARE_FOR_LT_FAILED,
    DDS_DPL_HDMI_LT_FAILED_TO_GET_NEXT_LINKRATE,
    DDS_DPL_HDMI_INVALID_SINK_PARAMS,
    DDS_DPL_HDMI_FRL_LTS2_FAILED,
    DDS_DPL_HDMI_FRL_LTS3_FAILED,
    DDS_DPL_HDMI_SUPPORTED_FRL_RATES_EXHAUSTED,
    DDS_DPL_HDMI_LTS3_TIMEOUT,
    DDS_DPL_HDMI_FAILED_TO_GET_NEXT_FRLRATE,
    DDS_DPL_HDMI_SINK_NOT_IN_FLTREADY_STATE,
    DDS_DPL_HDMI_SINK_FFE_UPDATE_FAILED,
    DDS_DPL_HDMI_SINK_FLR_START_FAILED,
    DDS_DPL_HDMI_SINK_FLT_UPDATE_FAILED,
    DDS_DPL_HDMI_FRL_BW_TBBLANK_EXCEEDS_HBLANK_AUDIOMIN,
    DDS_DPL_HDMI_FRL_BW_TBLANK_MIN_EXCEEDS_TBLANK_REF,
    DDS_DPL_HDMI_FRL_BW_TB_BORROW_REQ_EXCEEDS_MAX_TB_BORROW_ALLOWED,
    DDS_DPL_HDMI_FRL_BW_PAYLOAD_UTILIZATION_EXCEEDS_PAYLOAD_CAPACITY,
    DDS_DPL_HDMI_FRL_NOT_SUPPORTED_BY_SINK,

    // add new HDMI error code above this line
    __DD_STATUS_RANGE_DPL_HDMI_MAX,

    // MIPI DSI protocol related
    __DD_STATUS_RANGE_DPL_MIPI_DSI_START = __DD_STATUS_RANGE_DPL_HDMI_START + 1000,
    DDS_DPL_MIPI_DSI_INVALID_LINK_RATE,

    // add new MIPI DSI error code above this line
    __DD_STATUS_RANGE_DPL_MIPI_DSI_MAX,

    // add new protocol success code ranges above this line
    __DD_STATUS_RANGE_DPL_MAX, /* max allowed value = 0xA0000000, // range limit, *not* to be used in coding */

    /************************************************************************
     *
     *  start of HAL status codes        0xA000 0002 - 0xFFFFFFFE.
     *
     ************************************************************************/
    __DD_STATUS_RANGE_DHL_START = (DDS32)0xA0000001, // range limit, *not* to be used in coding
    DDS_DHL_INTERFACE_UNSUPPORTED_ON_GEN,
    DDS_DHL_INVALID_PORT,
    DDS_DHL_INVALID_PIPE,
    DDS_DHL_INVALID_PLANE,
    DDS_DHL_INVALID_TRANSPORT,
    DDS_DHL_PPS_ERROR,
    DDS_DHL_INVALID_DIP_TYPE,
    DDS_DHL_INVALID_CDCLOCK_FREQ,
    DDS_DHL_INVALID_VOLTAGE_INFO,
    DDS_DHL_INVALID_DMC_FW_SIZE,
    DDS_DHL_INVALID_IRQL,
    DDS_DHL_INVALID_DC_STATE,

    DDS_DHL_AUX_BUSY,
    DDS_DHL_AUX_UNKNOWN,
    DDS_DHL_AUX_NOTSUPPORTED,
    DDS_DHL_AUX_OPEN,
    DDS_DHL_AUX_DATA_READ_ERROR,
    DDS_DHL_AUX_DATA_WRITE_ERROR,
    DDS_DHL_AUX_INVALID_CHANNEL,
    DDS_DHL_AUX_MORE_DATA,
    DDS_DHL_AUX_DEFER,
    DDS_DHL_AUX_TIMEOUT,

    DDS_DHL_I2C_BUSY,
    DDS_DHL_I2C_NACK_RECIEVED,
    DDS_DHL_I2C_TIMEOUT,
    DDS_DHL_I2C_NOTSUPPORTED,
    DDS_DHL_I2C_ERROR_OPEN,
    DDS_DHL_I2C_ERROR_CLOSE,
    DDS_DHL_I2C_ERROR_SEGMENT_POINTER_WRITE,
    DDS_DHL_I2C_INVALID_PARAM,
    DDS_DHL_I2C_ACQUIRE_FAILED,
    DDS_DHL_I2C_BITBASHING_READ_DDC_ERROR,
    DDS_DHL_I2C_GMBUS_HW_NOT_READY,
    DDS_DHL_I2C_GMBUS_HW_NOT_ENTERING_WAIT_PHASE,

    DDS_DHL_TRANS_INVALID_TIMINGS,
    DDS_DHL_TRANS_INVALID_PARAM,
    DDS_DHL_TRANS_TIMEOUT,
    DDS_DHL_TRANS_INVALID_STATE,

    DDS_DHL_DBUF_PWRSTATE_ERROR,

    DDS_PG_DISTRIB_ERROR,
    DDS_PG_NOT_POWERED,
    DDS_PG_INVALID_PARAM,

    DDS_PIPE_INVALID_PIPE_SRCSZ,

    DDS_PLL_NOT_LOCKED,
    DDS_PLL_NOT_UNLOCKED,
    DDS_PLL_POWER_NOT_ENABLED,
    DDS_PLL_POWER_NOT_DISABLED,
    DDS_PLL_INVALID_LINK_RATE,
    DDS_PLL_INVALID_PLL,
    DDS_PLL_INVALID_DIVIDERS,

    DDS_DHL_PORT_INVALID_PATTERN,
    DDS_DHL_PORT_INVALID_DRIVE_SETTING,
    DDS_DHL_PORT_DISABLE_TIMEOUT,
    DDS_DHL_PORT_BUFFER_IDLE,
    DDS_DHL_PORT_INVALID_LANES,
    DDS_DHL_PORT_DP_ACT_SENT_ERROR,
    DDS_DHL_PORT_DP_ACT_HANDLING_ERROR,
    DDS_DHL_PORT_DP_FEC_SENT_ERROR,
    DDS_DHL_PORT_DP_FEC_HANDLING_ERROR,
    DDS_DHL_PORT_POWERUP_LANE_FAILED,
    DDS_DHL_PORT_POWERDOWN_LANE_FAILED,

    DDS_DHL_RM_NUM_PLANE_EXCEEDS_LIMIT,
    DDS_DHL_RM_NUM_SCALARS_EXCEEDS_LIMIT,
    DDS_DHL_RM_INVALID_PIXEL_FORMAT,
    DDS_DHL_RM_INVALID_PLANE_INDEX,
    DDS_DHL_RM_ASYNC_FLIP_NOT_SUPPORTED,
    DDS_DHL_RM_ASYNC_FLIP_HW_RESTRICTION_NOT_MET,
    DDS_DHL_RM_ALPHA_BLEND_NOT_SUPPORTED,
    DDS_DHL_RM_SURF_WIDTH_HEIGHT_HW_RESTRICTION_NOT_MET,
    DDS_DHL_RM_SURF_SIZE_HW_RESTRICTION_NOT_MET,
    DDS_DHL_RM_AUX_SURF_HW_RESTRICTION_NOT_MET,
    DDS_DHL_RM_SURF_OFFSET_HW_RESTRICTION_NOT_MET,
    DDS_DHL_RM_VFLIP_NOT_SUPPORTED,
    DDS_DHL_RM_VFLIP_HW_RESTRICTION_NOT_MET,
    DDS_DHL_RM_HFLIP_NOT_SUPPORTED,
    DDS_DHL_RM_HFLIP_HW_RESTRICTION_NOT_MET,
    DDS_DHL_RM_ROTATION_NOT_SUPPORTED,
    DDS_DHL_RM_ROTATION_DISABLED_FOR_RGB_SW_POLICY,
    DDS_DHL_RM_ROTATION_HW_RESTRICTION_NOT_MET,
    DDS_DHL_RM_SCALE_NOT_SUPPORTED,
    DDS_DHL_RM_SCALE_HW_RESTRICTION_NOT_MET,
    DDS_DHL_RM_HIGH_Q_SCALE_NOT_SUPPORTED,
    DDS_DHL_RM_BILINEAR_SCALE_NOT_SUPPORTED,
    DDS_DHL_RM_HORZ_STRETCH_FACTOR_EXCEEDS_LIMIT,
    DDS_DHL_RM_HORZ_SHRINK_FACTOR_EXCEEDS_LIMIT,
    DDS_DHL_RM_VERT_STRETCH_FACTOR_EXCEEDS_LIMIT,
    DDS_DHL_RM_VERT_SHRINK_FACTOR_EXCEEDS_LIMIT,
    DDS_DHL_RM_HW_WA_RESTRICTION_NOT_MET,
    DDS_DHL_RM_COLOR_CONFIG_NOT_SUPPORTED,
    DDS_DHL_HDCP_AUTH_FAILURE,
    DDS_DHL_HDCP_ENCRYPTION_FAILURE,
    DDS_DHL_HDCP_KEYLOAD_FAILURE,
    DDS_DHL_HDCP_AN_GENERATION_FAILURE,
    DDS_DHL_HDCP_BKSV_UPDATE_FAILURE,
    DDS_DHL_HDCP_RI_NOT_READY,
    DDS_DHL_HDCP_RI_MISMATCH,
    DDS_DHL_HDCP_REPEATER_STATE_UPDATE_FAILURE,
    DDS_DHL_HDCP_COMPUTE_V_FAILURE,
    DDS_DHL_HDCP_VPRIME_MISMATCH,
    DDS_DHL_INVALID_PWMCTRL_TYPE,

    DDS_DHL_RM_WM_EXCEEDED_FIFO,
    DDS_DHL_RM_DBUF_EXCEEDED_PIPE_ALLOC,
    DDS_DHL_3D_LUT_INVALID_PIPE,

    DDS_DHL_DSI_LINK_READY_TIMEOUT,
    DDS_DHL_DCS_HEADER_CREDIT_NOT_AVAILABLE,
    DDS_DHL_DCS_PAYLOAD_CREDIT_NOT_AVAILABLE,
    DDS_DHL_DSI_BLOCK_CLEAR_TIMEOUT,

    DDS_DHL_RM_PIPE_NOT_AVAILABLE,

    DDS_DHL_DYNAMICCD_UNSUPPORTED_PIXELCLOCK,
    DDS_DHL_INVALID_STREAM_STATUS,
    DDS_DHL_RM_BANDWIDTH_EXCEEDED,
    DDS_DHL_FLIP_QUEUE_QUEUE_NOT_FOUND,
    DDS_DHL_FLIP_QUEUE_FULL,
    DDS_DHL_RM_CUS_HW_RESTRICTION_NOT_MET,
    DDS_DHL_RM_CUS_MAX_PLANE_SIZE_EXCEEDS_LIMIT,
    DDS_DHL_FLIP_QUEUE_QUEUE_NOT_INITIALIZED,
    DDS_DHL_AUX_NOT_DONE,
    DDS_DHL_FLIP_QUEUE_PREEMPTION_FAILED,
    DDS_DHL_VRR_DC_BAL_COUNTER_NOT_READY,
    DDS_DHL_RM_PM_DMD_REQ_TIMEOUT, // For Timeout Issues with PM Demand Request for MTL+ Platforms
    DDS_DHL_PORT_D2D_LINK_STATE_ERROR,
    DDS_DHL_PORT_SOC_PHY_READY_TIMEOUT,
    DDS_DHL_PORT_REFCLK_ACK_STATE_TIMEOUT,
    DDS_DHL_PORT_POWERDOWN_UPDATE_TIMEOUT,
    DDS_DHL_PORT_RESET_STATE_TIMEOUT,
    DDS_DHL_PORT_PCLK_PLL_ACK_STATE_TIMEOUT,
    DDS_DHL_RM_DBUF_VALIDATION_FAILED,
    DDS_DHL_MSG_BUS_TRANSACTION_PENDING_TIMEOUT,
    DDS_DHL_MSG_BUS_RESPONSE_FAILED,

    DDS_DHL_INVALID_DSI_PACKET_WORDCOUNT,
    DDS_DHL_INVALID_DSI_PACKET_DATATYPE,

    DDS_DHL_RM_INVALID_MEM_CHANNELS,
    DDS_DHL_RM_INVALID_SAGV_POINTS,
    DDS_DHL_DSB_HUNG,
    // add new HAL error code above this line
    __DD_STATUS_RANGE_DHL_MAX,

    __DD_STATUS_RANGE_INIT_UNINIT_START = 0xB0000000,
    DDS_INIT_HAL_AUDIO_FAILED,
    DDS_INIT_HAL_GMBUS_FAILED,
    DDS_INIT_HAL_BITBASH_FAILED,
    DDS_INIT_HAL_AUX_FAILED,
    DDS_INIT_HAL_I2C_FAILED,
    DDS_INIT_PG_FAILED,
    DDS_INIT_PG_LOCK_ACQUIRE_FAILED,
    DDS_INIT_CLOCK_FAILED,
    DDS_INIT_DBUF_FAILED,
    DDS_UNINIT_CLOCK_FAILED,
    DDS_UNINIT_DBUF_FAILED,
    DDS_INIT_DMC_VERSION_MISMATCH,
    __DD_STATUS_RANGE_INIT_UNINT_MAX, // max allowed value = 0xFFFFFFFF, // range limit, *not* to be used in coding

    /************************************************************************
     *
     *  start of generic success codes   0 - 0x7FFF FFFE .
     *
     ************************************************************************/
    DDS_SUCCESS = (DDS32)0,
    DDS_MORE_ENTRIES,
    DDS_NO_ACTION_REQUIRED,
    DDS_SUCCESS_ENTRY_EXISTS_NO_UPDATE,
    DDS_SUCCESS_ENTRY_EXISTS_MAYBE_REPLACED, // This status denotes that Entry might have been replaced only when it satisfies all favourable conditions.
    DDS_SUCCESS_ENTRY_EXISTS_REPLACED,
    DDS_SUCCESS_ENTRY_NEW_ADDED,
    DDS_SUCCESS_MODE_INTERLACE_MODE_REMOVED_BY_OEM,
    DDS_SUCCESS_MODE_NO_PIXEL_FORMAT_SUPPORTED,
    DDS_CE_EXTN_BLOCK_NOT_FOUND,
    DDS_DISPLAY_ID_BLOCK_NOT_FOUND,
    DDS_HDMI_VSDB_BLOCK_NOT_FOUND,
    DDS_CE_EXTN_DTD_NOT_FOUND,
    DDS_CE_EXTN_S3D_MODES_NOT_PRESENT,
    DDS_DPL_TIMING_NOT_SUPPORTED,
    DDS_DPL_HDMI2_SCDC_NOT_SUPPORTED,
    DDS_PSR_ALREADY_ENABLED,
    DDS_PSR_ALREADY_DISABLED,
    DDS_STATUS_PENDING,
    DDS_HDCP1_ENCRYPTING_RI_YET_TO_MATCH,
    DDS_DPST_UNSUPPORTED_CONFIGURATION,
    DDS_INVALID_NUM_OF_COLLAGE_DISPLAYS,
    DDS_UNSUPPORTED_COLLAGE_DISPLAY,
    DDS_DISSIMILAR_COLLAGE_DISPLAYS,
    DDS_SUCCESS_DATA_UPDATED,
    DDS_SUCCESS_MODE_NOT_SUPPORTED_BY_PLATFORM,
    DDS_DPL_HDMI2_SCDC_RR_NOT_SUPPORTED,
    DDS_STATUS_RETRY,
    DDS_DPL_HDMI_LINK_TRAINING_PASSED,
    DDS_SFSU_DPST_UNSUPPORTED_CONFIGURATION,
    DDS_SUCCESS_INTERLACE_MODE_NOT_SUPPORTED_BY_PLATFORM,
    DDS_DETECTION_PENDING_POST_RESUME,
    DDS_INVALID_NUM_OF_INTERNAL_DISPLAYS,
    DDS_DPL_HDMI_FRL_LTS3_UPDATE_LINKRATE,
    DDS_DPL_HDMI_SINK_REQUESTED_FRL_RETRAIN_W_SAME_FRL_RATE,
    // Add new generic success code in the above region in a suitable fitting position
    __DD_STATUS_RANGE_GENERIC_SUCCESS_MAX /** max value allowed = 0x7FFFFFFF, // range limit, *not* to be used in coding.*/

} DDSTATUS;

// range check for overflow of major blocks
C_ASSERT(__DD_STATUS_RANGE_GENERIC_FAILURE_MAX < __DD_STATUS_RANGE_OSL_START);
C_ASSERT(__DD_STATUS_RANGE_OSL_MAX < __DD_STATUS_RANGE_DPL_START);
C_ASSERT(__DD_STATUS_RANGE_DPL_MAX < __DD_STATUS_RANGE_DHL_START);
C_ASSERT(__DD_STATUS_RANGE_DHL_MAX < __DD_STATUS_RANGE_INIT_UNINIT_START);
C_ASSERT(__DD_STATUS_RANGE_INIT_UNINT_MAX < DDS_SUCCESS);
C_ASSERT(__DD_STATUS_RANGE_GENERIC_SUCCESS_MAX > DDS_SUCCESS && __DD_STATUS_RANGE_GENERIC_SUCCESS_MAX <= 0x7FFFFFFF);

// range check for overflow of subblocks
C_ASSERT(__DD_STATUS_RANGE_DPL_GENERIC_MAX < __DD_STATUS_RANGE_DPL_DP_START);
C_ASSERT(__DD_STATUS_RANGE_DPL_DP_MAX < __DD_STATUS_RANGE_DPL_HDCP_START);
C_ASSERT(__DD_STATUS_RANGE_DPL_HDCP_MAX < __DD_STATUS_RANGE_DPL_HDMI_START);
C_ASSERT(__DD_STATUS_RANGE_OSL_FLIP_MAX < __DD_STATUS_RANGE_OSL_DVM_START);
C_ASSERT(__DD_STATUS_RANGE_OSL_DVM_MAX < __DD_STATUS_RANGE_OSL_MAX);
/////////////////////////////////////////////////////////////////////////
//
// error code handling macros
//

/**
 * @brief returns true if @c Err is a success code.
 */
#define IS_DDSTATUS_SUCCESS(Err) (((DDSTATUS)(Err)) >= DDS_SUCCESS)

/**
 * @brief returns true if @c Err is not a success code.
 */
#define IS_DDSTATUS_ERROR(Err) (((DDSTATUS)(Err)) < DDS_SUCCESS)

/**
 * @brief Returns TRUE if @c Status is Display OS Layer error code.
 *
 * @param  Status is the @c DDSTATUS code.
 * @return DD_BOOL
 * @retval True
 * @retval False
 */
DD_S_INLINE DD_BOOL _IsOslError(DDSTATUS Status)
{
    return ((Status > __DD_STATUS_RANGE_GENERIC_FAILURE_MAX) && (Status < __DD_STATUS_RANGE_OSL_MAX));
}

/**
 * @brief Returns TRUE if @c Status is Display Protocol Layer error code.
 *
 * @param  Status is the @c DDSTATUS code.
 * @return DD_BOOL
 * @retval true
 * @retval false
 */
DD_S_INLINE DD_BOOL _IsDplError(DDSTATUS Status)
{
    return ((Status > __DD_STATUS_RANGE_OSL_MAX) && (Status < __DD_STATUS_RANGE_DPL_MAX));
}

/**
 * @brief Returns TRUE if @c Status is Display HW Abstraction Layer error code.
 *
 * @param  Status is the @c DDSTATUS code.
 * @return DD_BOOL
 * @retval true
 * @retval false
 */
DD_S_INLINE DD_BOOL _IsDhlError(DDSTATUS Status)
{
    return ((Status > __DD_STATUS_RANGE_DPL_MAX) && (Status < __DD_STATUS_RANGE_DHL_MAX));
}

/**
 * @brief Returns TRUE if @c Status corresponds to Flip Retry code.
 *
 * @param  Status is the @c DDSTATUS code.
 * @return DD_BOOL
 * @retval true or false
 */
DD_S_INLINE DD_BOOL _IsFlipRetryStatus(DDSTATUS Status)
{
    return (Status == DDS_STATUS_RETRY) ? TRUE : FALSE;
}
#ifdef __cplusplus
typedef enum _DD_VSYNC_PROGRAMMING_ERRORS : DDU16
#else
typedef enum _DD_VSYNC_PROGRAMMING_ERRORS
#endif
{
    DD_PROG_ERR_UNKNOWN,

    DD_PROG_ERR_DRIVER = 0x1,

    // Generic Driver Error code range
    DD_PROG_ERR_DRV_FLIP_NOT_SUBMITTED = 0x1000,

    // HRR Related Driver Error Code Range
    DD_PROG_ERR_DRV_HRR_SUBMISSION = 0x1100,
    DD_PROG_ERR_DRV_HRR_ENABLING,
    DD_PROG_ERR_DRV_HRR_DISABLING,

    // FlipQ Related Driver Error Code Range
    DD_PROG_ERR_DRV_FLIPQ_DISABLED = 0x1200,
    DD_PROG_ERR_DRV_FLIPQ_PREEMPTED,
    DD_PROG_ERR_DRV_FLIPQ_INVALID_SCANLINE_RANGE,
    DD_PROG_ERR_DRV_FLIPQ_INVALID_DCSTATE,
    DD_PROG_ERR_DRV_FLIPQ_DRAIN,
    DD_PROG_ERR_DRV_CANCEL_ERROR,

    // PC related Error codes
    DD_PROG_ERR_DRV_PC_INCORRECT_DC_STATE = 0x1300,
    DD_PROG_ERR_DRV_PC_DEEP_SLEEP_ALLOWED,
    DD_PROG_ERR_DRV_INVALID_DMC_VERSION,

    DD_PROG_ERR_DMC = 0x2,

    // HRR Related DMC Error Code Range
    DD_PROG_ERR_DMC_HRR_FLIP_PENDING = 0x2100,

    // FlipQ related DMC Error Code Range
    DD_PROG_ERR_DMC_FLIPQ_EXEC_PENDING = 0x2200,
    DD_PROG_ERR_DMC_FLIPQ_EXEC_ERROR,

    DD_PROG_ERR_HARDWARE = 0x3,

    // Generic Hardware Error Code Range
    DD_PROG_ERR_HW_PIPE_HUNG = 0x3000,
    DD_PROG_ERR_HW_PLANE_HUNG,
    DD_PROG_ERR_HW_NV12_PLANE_Y_HUNG,  // Y hung but UV is not
    DD_PROG_ERR_HW_NV12_PLANE_UV_HUNG, // UV is hung but Y is not

    DD_PROG_ERR_OS = 0x4,

    // OS FlipQ Related Error Code Range
    DD_PROG_ERR_OS_QUEUE_SIZE_EXCEEDED = 0x4200,

    DD_PROG_ERR_BUCKETING = 0x5,

} DD_VSYNC_PROGRAMMING_ERRORS;
#ifdef __cplusplus
typedef enum _DD_VSYNC_REPORTING_ERRORS : DDU16
#else
typedef enum _DD_VSYNC_REPORTING_ERRORS
#endif
{
    DD_REPORT_ERR_UNKNOWN,

    DD_REPORT_ERR_DRIVER = 0x1,
    // Generic Driver Error code Range
    DD_REPORT_ERR_DRV_VBI_NOT_ENABLED = 0x1000,
    DD_REPORT_ERR_DRV_VBI_NOT_REPORTED,
    DD_REPORT_ERR_DRV_VBI_WRONG_TARGET_ID_REPORTED,

    DD_REPORT_ERR_DMC = 0x2,

    DD_REPORT_ERR_HARDWARE = 0x3,
    // Generic HW Error code range
    DD_REPORT_ERR_HW_VBI_NOT_GENERATED = 0x3000,
    DD_REPORT_ERR_HW_FLIP_DONE_NOT_GENERATED,

    DD_REPORT_ERR_OS = 0x4,
    // Generic OS error code range
    DD_REPORT_ERR_OS_CTRL_INT_DISABLED = 0x4000,

    DD_REPORT_ERR_BUCKETING = 0x5,

    // Generic Bucketing error code range
    DD_REPORT_ERR_BUCKETING_MISSING_OS_VSYNC_HISTORY = 0x5000,

} DD_VSYNC_REPORTING_ERRORS;

typedef union _DD_VSYNC_TIMEOUT_SUBCODE
{
    struct
    {
#ifdef __cplusplus
        DD_VSYNC_PROGRAMMING_ERRORS ProgrammingSubCode;
        DD_VSYNC_REPORTING_ERRORS ReportingSubCode;
#else
        DDU16 ProgrammingSubCode; // DD_VSYNC_PROGRAMMING_ERRORS
        DDU16 ReportingSubCode;   // DD_VSYNC_REPORTING_ERRORS
#endif
    };
    DDU32 SubFailureCode;
} DD_VSYNC_TIMEOUT_SUBCODE;

C_ASSERT(sizeof(DD_VSYNC_TIMEOUT_SUBCODE) == sizeof(DDU32));

/**
 * @brief @c DD_ASSERT_TYPES error code used across all display driver modules to return function status.
 *
 * Any new typee should be added to the end of the list for that category w/o affecting old enum values.
 */
typedef enum _DD_ASSERT_TYPE
{
    DD_EXPECT_EQ  = 1,
    DD_EXPECT_NE  = 2,
    DD_EXPECT_GT  = 3,
    DD_EXPECT_GE  = 4,
    DD_EXPECT_LT  = 5,
    DD_EXPECT_LE  = 6,
    DD_UNEXPECTED = 7,
} DD_ASSERT_TYPE;

//void LOG_ConditionalAssert(const char *pFunction, DDU32 Line, DD_BOOL IsFailed, const char *pLeftExpr, DDU64 LhsValue, DD_ASSERT_TYPE Type, const char *pRightExpr, DDU64 RhsValue,
//                           const char *pComment, const char *pFile, DDU32 Mask);
#if (_RELEASE_INTERNAL)
// forward declaration of the Assert logging
void LOG_DisplayAssert(const char *Function, const char *Expression, DDU32 Line, const char *File);
DD_BOOL IsDisplayDebugBreakEnabled();

#if (defined(_ENABLE_DEBUG_BREAK) || defined(_DEBUG))
#define DEBUG_BREAK()                             \
    {                                             \
        if (TRUE == IsDisplayDebugBreakEnabled()) \
            __debugbreak();                       \
    }
#else
#define DEBUG_BREAK()
#endif // (defined(_ENABLE_DEBUG_BREAK) || defined(_DEBUG))

#define DDASSERT(expr)                                              \
    if (FALSE == (expr))                                            \
    {                                                               \
        LOG_DisplayAssert(__FUNCTION__, #expr, __LINE__, __FILE__); \
        DEBUG_BREAK();                                              \
    }

#define DDRLSASSERT_EXPECT_EQ(Lhs, Rhs, Mask, Comment)                                                                                      \
    if ((Lhs) != (Rhs))                                                                                                                     \
    {                                                                                                                                       \
        LOG_ConditionalAssert(__FUNCTION__, __LINE__, TRUE, #Lhs, (DDU64)(Lhs), DD_EXPECT_EQ, #Rhs, (DDU64)(Rhs), Comment, __FILE__, Mask); \
        DEBUG_BREAK();                                                                                                                      \
    }

#define DDRLSASSERT_EXPECT_NE(Lhs, Rhs, Mask, Comment)                                                                                      \
    if ((Lhs) == (Rhs))                                                                                                                     \
    {                                                                                                                                       \
        LOG_ConditionalAssert(__FUNCTION__, __LINE__, TRUE, #Lhs, (DDU64)(Lhs), DD_EXPECT_NE, #Rhs, (DDU64)(Rhs), Comment, __FILE__, Mask); \
        DEBUG_BREAK();                                                                                                                      \
    }

#define DDRLSASSERT_EXPECT_GT(Lhs, Rhs, Mask, Comment)                                                                                      \
    if ((Lhs) <= (Rhs))                                                                                                                     \
    {                                                                                                                                       \
        LOG_ConditionalAssert(__FUNCTION__, __LINE__, TRUE, #Lhs, (DDU64)(Lhs), DD_EXPECT_GT, #Rhs, (DDU64)(Rhs), Comment, __FILE__, Mask); \
        DEBUG_BREAK();                                                                                                                      \
    }

#define DDRLSASSERT_EXPECT_GE(Lhs, Rhs, Mask, Comment)                                                                                      \
    if ((Lhs) < (Rhs))                                                                                                                      \
    {                                                                                                                                       \
        LOG_ConditionalAssert(__FUNCTION__, __LINE__, TRUE, #Lhs, (DDU64)(Lhs), DD_EXPECT_GE, #Rhs, (DDU64)(Rhs), Comment, __FILE__, Mask); \
        DEBUG_BREAK();                                                                                                                      \
    }

#define DDRLSASSERT_EXPECT_LT(Lhs, Rhs, Mask, Comment)                                                                                      \
    if ((Lhs) >= (Rhs))                                                                                                                     \
    {                                                                                                                                       \
        LOG_ConditionalAssert(__FUNCTION__, __LINE__, TRUE, #Lhs, (DDU64)(Lhs), DD_EXPECT_LT, #Rhs, (DDU64)(Rhs), Comment, __FILE__, Mask); \
        DEBUG_BREAK();                                                                                                                      \
    }

#define DDRLSASSERT_EXPECT_LE(Lhs, Rhs, Mask, Comment)                                                                                      \
    if ((Lhs) > (Rhs))                                                                                                                      \
    {                                                                                                                                       \
        LOG_ConditionalAssert(__FUNCTION__, __LINE__, TRUE, #Lhs, (DDU64)(Lhs), DD_EXPECT_LE, #Rhs, (DDU64)(Rhs), Comment, __FILE__, Mask); \
        DEBUG_BREAK();                                                                                                                      \
    }

#define DDRLSASSERT_UNEXPECTED(Mask, Comment)                                                                          \
    {                                                                                                                  \
        LOG_ConditionalAssert(__FUNCTION__, __LINE__, TRUE, NULL, 0, DD_UNEXPECTED, NULL, 0, Comment, __FILE__, Mask); \
        DEBUG_BREAK();                                                                                                 \
    }
#else // (_DISP_LLVM_BUILD || _RELEASE)
#define DDASSERT(expr)
#define DDRLSASSERT_EXPECT_EQ(Lhs, Rhs, Mask, Comment)
#define DDRLSASSERT_EXPECT_NE(Lhs, Rhs, Mask, Comment)
#define DDRLSASSERT_EXPECT_GT(Lhs, Rhs, Mask, Comment)
#define DDRLSASSERT_EXPECT_GE(Lhs, Rhs, Mask, Comment)
#define DDRLSASSERT_EXPECT_LT(Lhs, Rhs, Mask, Comment)
#define DDRLSASSERT_EXPECT_LE(Lhs, Rhs, Mask, Comment)
#define DDRLSASSERT_UNEXPECTED(Mask, Comment)
#endif
#if (_RELEASE_INTERNAL)
    if ((Lhs) != (Rhs))                                                                                                      \
    {                                                                                                                        \
        LOG_ConditionalAssert(NULL, __LINE__, TRUE, NULL, (DDU64)(Lhs), DD_EXPECT_EQ, NULL, (DDU64)(Rhs), NULL, NULL, Mask); \
    }

#define DDRLSASSERT_EXPECT_NE(Lhs, Rhs, Mask, Comment)                                                                       \
    if ((Lhs) == (Rhs))                                                                                                      \
    {                                                                                                                        \
        LOG_ConditionalAssert(NULL, __LINE__, TRUE, NULL, (DDU64)(Lhs), DD_EXPECT_NE, NULL, (DDU64)(Rhs), NULL, NULL, Mask); \
    }

#define DDRLSASSERT_EXPECT_GT(Lhs, Rhs, Mask, Comment)                                                                       \
    if ((Lhs) <= (Rhs))                                                                                                      \
    {                                                                                                                        \
        LOG_ConditionalAssert(NULL, __LINE__, TRUE, NULL, (DDU64)(Lhs), DD_EXPECT_GT, NULL, (DDU64)(Rhs), NULL, NULL, Mask); \
    }

#define DDRLSASSERT_EXPECT_GE(Lhs, Rhs, Mask, Comment)                                                                       \
    if ((Lhs) < (Rhs))                                                                                                       \
    {                                                                                                                        \
        LOG_ConditionalAssert(NULL, __LINE__, TRUE, NULL, (DDU64)(Lhs), DD_EXPECT_GE, NULL, (DDU64)(Rhs), NULL, NULL, Mask); \
    }

#define DDRLSASSERT_EXPECT_LT(Lhs, Rhs, Mask, Comment)                                                                       \
    if ((Lhs) >= (Rhs))                                                                                                      \
    {                                                                                                                        \
        LOG_ConditionalAssert(NULL, __LINE__, TRUE, NULL, (DDU64)(Lhs), DD_EXPECT_LT, NULL, (DDU64)(Rhs), NULL, NULL, Mask); \
    }

#define DDRLSASSERT_EXPECT_LE(Lhs, Rhs, Mask, Comment)                                                                       \
    if ((Lhs) > (Rhs))                                                                                                       \
    {                                                                                                                        \
        LOG_ConditionalAssert(NULL, __LINE__, TRUE, NULL, (DDU64)(Lhs), DD_EXPECT_LE, NULL, (DDU64)(Rhs), NULL, NULL, Mask); \
    }

#define DDRLSASSERT_UNEXPECTED(Mask, Comment)                                                           \
    {                                                                                                   \
        LOG_ConditionalAssert(NULL, __LINE__, TRUE, NULL, 0, DD_UNEXPECTED, NULL, 0, NULL, NULL, Mask); \
    }

#endif

#if (_RELEASE_INTERNAL)
#define FORCE_DEBUG_BREAK()                       \
    {                                             \
        if (TRUE == IsDisplayDebugBreakEnabled()) \
            __debugbreak();                       \
    }
#else // (_DEBUG || _RELEASE)
#define FORCE_DEBUG_BREAK()
#endif
/////////////////////////////////////////////////////////////////////////

#endif // _DISPLAY_ERROR_H_
