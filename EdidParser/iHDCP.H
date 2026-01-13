//-----------------------------------------------------------------------------------------------------------
//
// INTEL CONFIDENTIAL
// Copyright (C) 2011-2024 Intel Corporation
// This software and the related documents are Intel copyrighted materials,
// and your use of them is governed by the express license under which they were provided to you ("License").
// Unless the License provides otherwise, you may not use, modify, copy, publish,
// distribute, disclose or transmit this software or the related documents without
// Intel's prior written permission. This software and the related documents are provided
// as is, with no express or implied warranties, other than those that are expressly stated in the License.
//
//-----------------------------------------------------------------------------------------------------------

#ifndef __HDCP_H__
#define __HDCP_H__

//#include "Driver_Model.h"       // For our XP and LH Macros

#ifdef _COMMON_PPA
#include"OMPTool.h"
#endif

//
// structure defn specific to DVI/HDMI based HDCP over I2C
// These defn are based upon HDCP 1.3 spec
//
#pragma pack(1)
//
// BCAPS
//
typedef union _HDCP_RX_HDMI_BCAPS
{
    unsigned char Value;
    struct {
        unsigned char FastReauthantication : 1; // bit 0
        unsigned char B1_1FeaturesSupported : 1; // bit 1
        unsigned char Reserved : 2; // bi 3:2
        unsigned char FastTransfer : 1; // bit 4 ( TRUE  = transfer speed at 400 kHz FALSE = transfer speed at 100 Khz)
        unsigned char KSVFifoReady : 1; // bit 5
        unsigned char IsReapeater : 1; // bit 6
        unsigned char Reserved1 : 1; // bit 7
    };
}HDCP_RX_HDMI_BCAPS;

typedef union _HDCP2_VERSION
{
    unsigned char Value;
    struct
    {
        unsigned char Reserved : 2; // bit 1:0
        unsigned char Hdcp2_2Capable : 1; // bit 2
        unsigned char Reserved2 : 5; // bit 7:3:
    };
}HDCP2_VERSION;

// HDCP related definitions are kept here for common usability between
// Integrated and External SDVO based HDCP operations
//I2C Address for HDCP cummnication with Receiver
#define RX_ADDRESS      0x74000000  // HDCP Port I2C Address (Single Link)
//  shifted for call back function

//I2C Subaddress Defines - As per the HDCP Spec
// Downstream spec does not specify which is MSB and LSB?
#define RX_BKSV_0       0x00    // BKSV[7:0]
#define RX_BKSV_1       0x01    // BKSV[15:8]
#define RX_BKSV_2       0x02    // BKSV[23:16]
#define RX_BKSV_3       0x03    // BKSV[31:24]
#define RX_BKSV_4       0x04    // BKSV[39:32]
#define RX_RI_HIGH      0x08    // Ri'[7:0]
#define RX_RI_LOW       0x09    // Ri'[15:8]
#define RX_AKSV_0       0x10    // AKSV[7:0]
#define RX_AKSV_1       0x11    // AKSV[15:8]
#define RX_AKSV_2       0x12    // AKSV[23:16]
#define RX_AKSV_3       0x13    // AKSV[31:24]
#define RX_AKSV_4       0x14    // AKSV[39:32]... write this byte last
#define RX_AINFO        0x15    // Receiver register to inform it to enable 1.1 features
#define RX_AN_0         0x18    // An[7:0]
#define RX_AN_1         0x19    // An[15:8]
#define RX_AN_2         0x1A    // An[23:16]
#define RX_AN_3         0x1B    // An[31:24]
#define RX_AN_4         0x1C    // An[39:32]
#define RX_AN_5         0x1D    // An[47:40]
#define RX_AN_6         0x1E    // An[55:48]
#define RX_AN_7         0x1F    // An[63:56]
#define RX_VPRIME_H0_0  0x20    // V'[7:0]
#define RX_VPRIME_H0_1  0x21    // V'[15:8]
#define RX_VPRIME_H0_2  0x22    // V'[23:16]
#define RX_VPRIME_H0_3  0x23    // V'[31:24]
#define RX_VPRIME_H1_0  0x24    // V'[39:32]
#define RX_VPRIME_H1_1  0x25    // V'[47:40]
#define RX_VPRIME_H1_2  0x26    // V'[55:48]
#define RX_VPRIME_H1_3  0x27    // V'[63:56]
#define RX_VPRIME_H2_0  0x28    // V'[71:64]
#define RX_VPRIME_H2_1  0x29    // V'[79:72]
#define RX_VPRIME_H2_2  0x2A    // V'[87:80]
#define RX_VPRIME_H2_3  0x2B    // V'[95:88]
#define RX_VPRIME_H3_0  0x2C    // V'[103:96]
#define RX_VPRIME_H3_1  0x2D    // V'[111:104]
#define RX_VPRIME_H3_2  0x2E    // V'[119:112]
#define RX_VPRIME_H3_3  0x2F    // V'[127:120]
#define RX_VPRIME_H4_0  0x30    // V'[135:128]
#define RX_VPRIME_H4_1  0x31    // V'[143:136]
#define RX_VPRIME_H4_2  0x32    // V'[151:144]
#define RX_VPRIME_H4_3  0x33    // V'[159:152]
#define RX_BCAPS        0x40    // [7] RSVD, [6] Repeater, [5] Ready, [4] Fast, [3:2] RSVD, [1] Features, [0] Fast_reauthentication
#define RX_BSTATUS_0    0x41    // [7] MAX_DEVS_EXCEEDED, [6:0] DEVICE_COUNT
#define RX_BSTATUS_1    0x42    // [15:14] RSVD, [13] HDMI_RSVD, [12] HDMI_MODE, [11] MAX_CASCADE_EXCEEDED, [10:8] DEPTH
#define RX_KSV_FIFO     0x43

//
// Display Port Specific DPCD adresses defined for HDCP communication
//

//
// BCAPS
//

typedef union _HDCP_RX_DP_BCAPS
{
    unsigned char  Value;
    struct {
        unsigned char HDCPCapable : 1; // Bit 0
        unsigned char IsReapeater : 1; // Bit 1
        unsigned char Reserved : 6; // Bit 2:7
    };
}HDCP_RX_DP_BCAPS;

//Its the bit definition for the Big endian based format
typedef union _HDCP2_RX_DP_BCAPS
{
    unsigned long  Value;
    struct {
        unsigned long HDCPVersion : 8; // Bit 16:23
        unsigned long RxCapmaskHi : 8; // Bit 8:15
        unsigned long IsReapeater : 1; // Bit 0
        unsigned long HDCPCapable : 1; // Bit 1
        unsigned long RxCapmaskLo : 6; // Bit 2:7

        unsigned long Reserved1 : 8; // Bit 24:31
    };
}HDCP2_RX_DP_BCAPS;

typedef union _HDCP_RX_BCAPS
{
    union
    {
        HDCP_RX_HDMI_BCAPS  Hdmi;
        HDCP_RX_DP_BCAPS    Dp;
    }Hdcp1;
    HDCP2_RX_DP_BCAPS   Hdcp2Dp;
}HDCP_RX_BCAPS;
//
// BINFO
//
typedef union _HDCP_RX_DP_BINFO
{
    unsigned short Value;
    struct {
        unsigned short DeviceCount : 7; // Bit 6:0
        unsigned short MaxDevsExceeded : 1; // Bit 7
        unsigned short Depth : 3; // Bit 10:8
        unsigned short MaxCascadeExceeded : 1; // Bit 11
        unsigned short Reserved : 4; // Bit 15:12
    };
}HDCP_RX_DP_BINFO;


//
// BSTATUS
//
typedef union _HDCP_RX_BSTATUS
{
    unsigned short usValue;
    struct {
        unsigned short usDeviceCount : 7; // bit 6:0
        unsigned short bMaxDevsExceeded : 1; // bit 7
        unsigned short usDepth : 3; // bit 10:8
        unsigned short bMaxCascadeExceeded : 1; // bit 11
        unsigned short bRXInHDMIMode : 1; // bit 12
        unsigned short usRserved : 3; // bit 15:13
    };
}HDCP_RX_BSTATUS;

typedef union _HDCP_RX_DP_BSTATUS
{
    unsigned char Value;
    struct {
        unsigned char KSVFifoReady : 1; // bit 0
        unsigned char RiAvailable : 1; // bit 1
        unsigned char LinkIntegrityFailed : 1; // bit 2
        unsigned char Resered : 5; // bit 7:3
    };
}HDCP_RX_DP_BSTATUS;

#define RX_DP_BKSV      0x68000
#define RX_DP_RI        0x68005
#define RX_DP_AKSV      0x68007
#define RX_DP_AN        0x6800C
#define RX_DP_VPRIME_H0 0x68014
#define RX_DP_VPRIME_H1 0x68018
#define RX_DP_VPRIME_H2 0x6801C
#define RX_DP_VPRIME_H3 0x68020
#define RX_DP_VPRIME_H4 0x68024
#define RX_DP_BCAPS     0x68028
#define RX_DP_BSTATUS   0x68029
#define RX_DP_BINFO     0x6802A
#define RX_DP_KSV_FIFO  0x6802C

#define MAX_DP_KSV_FIFO_READ_SIZE 15

#define MAX_DP_HDCP_RETRY 3 // the number of retries required for DP. This is mandated by the compliance spec

#define HDCP2_2_RX_DP_BCAPS     0x6921D
/*
typedef enum _HDCP1_MESSAGE_ID
{
RxBKSVData = 1,
RxBCaps = 2,
RxBStatus = 3,
RxRepeaterKSVList = 4,
RxRepeaterPrimeV = 5,
RxRiData = 6,
RxBinfo = 7
}HDCP1_MESSAGE_ID;
*/
typedef union _HDCP2_RX_STATUS_ST
{
    union
    {
        unsigned short Value;
        struct {
            unsigned short Msglen : 10;
            unsigned short Rdy : 1;
            unsigned short ReauthReq : 1;
            unsigned short Reserved : 4;
        };
    }HDMI;

    union
    {
        unsigned char Value;
        struct
        {
            unsigned char Ready : 1; //Bit0
            unsigned char Havailable : 1; //Bit 1
            unsigned char PairingAvailable : 1; //Bit 2
            unsigned char ReauthReq : 1; //Bit3
            unsigned char LinkIntegrityFailure : 1; //Bit 4
            unsigned char Reserved : 3; //Bits 7:5
        };
    }DP;
}HDCP2_RX_STATUS_ST;

//HDCP offsets for HDMI
#define RX_HDCP2_VERSION            0x50
#define RX_HDCP2_WRITE_OFFSET       0x60
#define RX_HDCP2_RXSTATUS           0x70
#define RX_HDCP2_READ_OFFSET        0x80

#define DPCD_LSPCON_AKSV_ACK        0x0050F
#define DPCD_LSPCON_AKSV_ACK_VALUE  0x3

//HDCP offsets for      DP
#define RX_DP_HDCP2_RTX             0x69000
#define RX_DP_HDCP2_TXCAPS          0x69008
#define RX_DP_HDCP2_CERT_RX         0x6900B
#define RX_DP_HDCP2_RRX             0x69215
#define RX_DP_HDCP2_RXCAPS          0x6921D
#define RX_DP_HDCP2_EKPUB_KM        0x69220
#define RX_DP_HDCP2_EKH_KM_TX       0x692A0
#define RX_DP_HDCP2_M_TX            0x692B0
#define RX_DP_HDCP2_HPRIME          0x692C0
#define RX_DP_HDCP2_EKH_KM_RX       0x692E0
#define RX_DP_HDCP2_RN              0x692F0
#define RX_DP_HDCP2_LPRIME          0x692F8
#define RX_DP_HDCP2_EDKEY_KS        0x69318
#define RX_DP_HDCP2_RIV             0x69328
#define RX_DP_HDCP2_RXINFO          0x69330
#define RX_DP_HDCP2_SEQ_NUM_V       0x69332
#define RX_DP_HDCP2_V_RX            0x69335
#define RX_DP_HDCP2_RCVR_ID_LST     0x69345
#define RX_DP_HDCP2_V_TX            0x693E0
#define RX_DP_HDCP2_SEQ_NUM_M       0x693F0
#define RX_DP_HDCP2_K               0x693F3
#define RX_DP_HDCP2_STRMID_TYPE     0x693F5
#define RX_DP_HDCP2_M_RX            0x69473
#define RX_DP_HDCP2_RXSTATUS        0x69493
#define RX_DP_HDCP2_ERRATA_TYPE     0x69494

typedef union _HDCP2_DP_RX_STATUS
{
    unsigned char Value;
    struct
    {
        unsigned char Ready : 1; //Bit0
        unsigned char Havailable : 1; //Bit 1
        unsigned char PairingAvailable : 1; //Bit 2
        unsigned char ReauthReq : 1; //Bit3
        unsigned char LinkIntegrityFailure : 1; //Bit 4
        unsigned char Reserved : 3; //Bits 7:5
    };
}HDCP2_DP_RX_STATUS;

typedef union _HDCP2_RX_INFO_ST
{
    struct
    {
        unsigned short DeviceCountHigh : 1;
        unsigned short Depth : 3;
        unsigned short Reserved : 4;

        unsigned short Hdcp1DeviceDownstream : 1;
        unsigned short Hdcp2LegacyDeviceDownstream : 1;
        unsigned short MaxCascadeExceeded : 1;
        unsigned short MaxDeviceExceeded : 1;
        unsigned short DeviceCountLow : 4;

    } Fields;
    unsigned short Value;
} HDCP2_RX_INFO_ST;

typedef union _HDCP2_RX_CAPS_ST
{
    unsigned long Value;
    struct {
        unsigned long Reserved : 8; // Bit 31:24
        unsigned long Version : 8; // Bit 23:16
        unsigned long RcvrCapabilityMask : 14; //Bit15:2
        unsigned long HDCPCapable : 1; //Bit1
        unsigned long Rptr : 1; //Bit0
    };
}HDCP2_RX_CAPS_ST;

typedef union _HDCP2_TX_CAPS_ST
{
    unsigned long Value;
    struct {
        unsigned long Reserved : 8; // Bit 31:24
        unsigned long Version : 8; // Bit 23:16
        unsigned long TrnsmtrCapabilityMask : 16; // Bit 15:0
    };
}HDCP2_TX_CAPS_ST;

#pragma pack()

#endif // __HDCP_H__
// END OF FILE //
