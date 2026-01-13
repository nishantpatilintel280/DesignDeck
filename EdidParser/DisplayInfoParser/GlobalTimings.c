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
 * @file  GlobalTimings.c
 * @brief This file contains all the Timings details created for EDID (CVT/GTF timings), CE spec and Display ID spec.
 *
 */

#include "GlobalTimings.h"

 // We don't need this for UTF builds as its compiled as C++
#ifndef _DISP_UTF_
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#endif

/*===========================================================================
; Timings for Static Modes
***************************************************************/

//
//    Fake VGA mode 3 timings
DD_TIMING_INFO g_TimingVGA_Mode3_640x480_60 = {
    25175000,  // Pixel clock in Hz
    800,       // H. total pixels
    640,       // H. active pixels
    648,       // H. blank start from start of active in pixels
    648 + 143, // H. blank end from start of active in pixels, as per the monitor timing doc
    656,       // H. sync start from start of active in pixels
    656 + 95,  // H. sync end from start of active in pixels
    31469,     // H. frequency in Hz
    525,       // V. total lines
    480,       // V. active lines
    488,       // V. blank start from start of active in lines
    488 + 28,  // V. blank end from start of active in lines
    490,       // V. sync start from start of active in lines
    490 + 1,   // V. sync end from start of active in lines
    60,        // Refresh rate
    0,         // Interlaced
    1,         // Timing flags -- H-, V-
    1,         // Timing flags -- H-, V-
};
DD_TIMING_INFO g_TimingVGA_Mode3_720x400_60 = {
    28322000,  // Pixel clock in Hz
    800,       // H. total pixels
    640,       // H. active pixels
    640,       // H. blank start from start of active in pixels
    640 + 159, // H. blank end from start of active in pixels, as per the monitor timing doc
    656,       // H. sync start from start of active in pixels
    656 + 95,  // H. sync end from start of active in pixels
    31469,     // H. frequency in Hz
    525,       // V. total lines
    400,       // V. active lines
    400,       // V. blank start from start of active in lines
    400 + 124, // V. blank end from start of active in lines
    450,       // V. sync start from start of active in lines
    450 + 1,   // V. sync end from start of active in lines
    60,        // Refresh rate
    0,         // Interlaced
    1,         // Timing flags -- H-, V-
    1,         // Timing flags -- H-, V-
};

//
//    13.x timing 640x400@70
//  Compaq Timing Requirements document, July 15 1999
DD_TIMING_INFO g_Timing640x400_70 = {
    25175000,  // Pixel clock in Hz
    800,       // H. total pixels
    640,       // H. active pixels
    648,       // H. blank start from start of active in pixels
    648 + 143, // H. blank end from start of active in pixels
    656,       // H. sync start from start of active in pixels
    656 + 95,  // H. sync end from start of active in pixels
    31469,     // 29190,                    // H. frequency in Hz
    449,       // V. total lines
    400,       // V. active lines
    406,       // V. blank start from start of active in lines
    406 + 35,  // V. blank end from start of active in lines
    413,       // V. sync start from start of active in lines
    413 + 1,   // V. sync end from start of active in lines
    70,        // Refresh rate
    0,         // Interlaced
    1,         // Timing flags -- H-, V+
    0,         // Timing flags -- H-, V+
};

//
//    VESA DMTS timing 640x350@85
DD_TIMING_INFO g_Timing640x350_85 = {
    31500000,  // Pixel clock in Hz
    832,       // H. total pixels
    640,       // H. active pixels
    640,       // H. blank start from start of active in pixels
    640 + 191, // H. blank end from start of active in pixels
    672,       // H. sync start from start of active in pixels
    672 + 63,  // H. sync end from start of active in pixels
    37861,     // 29190,                    // H. frequency in Hz
    445,       // V. total lines
    350,       // V. active lines
    350,       // V. blank start from start of active in lines
    350 + 94,  // V. blank end from start of active in lines
    382,       // V. sync start from start of active in lines
    382 + 2,   // V. sync end from start of active in lines
    85,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V-
    1,         // Timing flags -- H+, V-
};

//
//    VESA DMTS timing 640x400@85
DD_TIMING_INFO g_Timing640x400_85 = {

    31500000,  // Pixel clock in Hz
    832,       // H. total pixels
    640,       // H. active pixels
    640,       // H. blank start from start of active in pixels
    640 + 191, // H. blank end from start of active in pixels
    672,       // H. sync start from start of active in pixels
    672 + 63,  // H. sync end from start of active in pixels
    37861,     // 29190,                    // H. frequency in Hz
    445,       // V. total lines
    400,       // V. active lines
    400,       // V. blank start from start of active in lines
    400 + 44,  // V. blank end from start of active in lines
    401,       // V. sync start from start of active in lines
    401 + 2,   // V. sync end from start of active in lines
    85,        // Refresh rate
    0,         // Interlaced
    1,         // Timing flags -- H-, V+
    0,         // Timing flags -- H-, V+
};

//
//    VESA DMTS timing 640x480@60
DD_TIMING_INFO g_Timing640x480_60 = {

    25175000,  // Pixel clock in Hz
    800,       // H. total pixels
    640,       // H. active pixels
    648,       // H. blank start from start of active in pixels
    648 + 143, // H. blank end from start of active in pixels , as per the monitor timing doc
    656,       // H. sync start from start of active in pixels
    656 + 95,  // H. sync end from start of active in pixels
    31469,     // H. frequency in Hz
    525,       // V. total lines
    480,       // V. active lines
    488,       // V. blank start from start of active in lines
    488 + 28,  // V. blank end from start of active in lines
    490,       // V. sync start from start of active in lines
    490 + 1,   // V. sync end from start of active in lines
    60,        // Refresh rate
    0,         // Interlaced
    1,         // Timing flags -- H-, V-
    1,         // Timing flags -- H-, V-
};

//
//    VESA DMTS timing 640x480@72
DD_TIMING_INFO g_Timing640x480_72 = {

    31500000,  // Pixel clock in Hz
    832,       // H. total pixels
    640,       // H. active pixels
    648,       // H. blank start from start of active in pixels
    648 + 175, // H. blank end from start of active in pixels
    664,       // H. sync start from start of active in pixels
    664 + 39,  // H. sync end from start of active in pixels
    37861,     // H. frequency in Hz
    520,       // V. total lines
    480,       // V. active lines
    488,       // V. blank start from start of active in lines
    488 + 23,  // V. blank end from start of active in lines
    489,       // V. sync start from start of active in lines
    489 + 2,   // V. sync end from start of active in lines
    72,        // Refresh rate
    0,         // Interlaced
    1,         // Timing flags -- H-, V-
    1,         // Timing flags -- H-, V-
};

//
//    VESA DMTS timing 640x480@75
DD_TIMING_INFO g_Timing640x480_75 = {

    31500000,  // Pixel clock in Hz
    840,       // H. total pixels
    640,       // H. active pixels
    640,       // H. blank start from start of active in pixels
    640 + 199, // H. blank end from start of active in pixels
    656,       // H. sync start from start of active in pixels
    656 + 63,  // H. sync end from start of active in pixels
    37500,     // H. frequency in Hz
    500,       // V. total lines
    480,       // V. active lines
    480,       // V. blank start from start of active in lines
    480 + 19,  // V. blank end from start of active in lines
    481,       // V. sync start from start of active in lines
    481 + 2,   // V. sync end from start of active in lines
    75,        // Refresh rate
    0,         // Interlaced
    1,         // Timing flags -- H-, V-
    1,         // Timing flags -- H-, V-
};

//
//    VESA DMTS timing 640x480@85
DD_TIMING_INFO g_Timing640x480_85 = {

    36000000,  // Pixel clock in Hz
    832,       // H. total pixels
    640,       // H. active pixels
    640,       // H. blank start from start of active in pixels
    640 + 191, // H. blank end from start of active in pixels
    696,       // H. sync start from start of active in pixels
    696 + 55,  // H. sync end from start of active in pixels
    43269,     // H. frequency in Hz
    509,       // V. total lines
    480,       // V. active lines
    480,       // V. blank start from start of active in lines
    480 + 28,  // V. blank end from start of active in lines
    481,       // V. sync start from start of active in lines
    481 + 2,   // V. sync end from start of active in lines
    85,        // Refresh rate
    0,         // Interlaced
    1,         // Timing flags -- H-, V-
    1,         // Timing flags -- H-, V-
};

//
//    VESA DMTS timing 720x400@85
DD_TIMING_INFO g_Timing720x400_85 = {

    35500000,  // Pixel clock in Hz
    936,       // H. total pixels
    720,       // H. active pixels
    720,       // H. blank start from start of active in pixels
    720 + 215, // H. blank end from start of active in pixels
    756,       // H. sync start from start of active in pixels
    756 + 71,  // H. sync end from start of active in pixels
    37927,     // H. frequency in Hz
    446,       // V. total lines
    400,       // V. active lines
    400,       // V. blank start from start of active in lines
    400 + 45,  // V. blank end from start of active in lines
    401,       // V. sync start from start of active in lines
    401 + 2,   // V. sync end from start of active in lines
    85,        // Refresh rate
    0,         // Interlaced
    1,         // Timing flags -- H-, V+
    0,         // Timing flags -- H-, V+
};

//
//    VESA DMTS timing 800x600@56
DD_TIMING_INFO g_Timing800x600_56 = {

    36000000,  // Pixel clock in Hz
    1024,      // H. total pixels
    800,       // H. active pixels
    800,       // H. blank start from start of active in pixels
    800 + 223, // H. blank end from start of active in pixels
    824,       // H. sync start from start of active in pixels
    824 + 71,  // H. sync end from start of active in pixels
    35156,     // H. frequency in Hz
    625,       // V. total lines
    600,       // V. active lines
    600,       // V. blank start from start of active in lines
    600 + 24,  // V. blank end from start of active in lines
    601,       // V. sync start from start of active in lines
    601 + 1,   // V. sync end from start of active in lines
    56,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 800x600@60
DD_TIMING_INFO g_Timing800x600_60 = {

    40000000,  // Pixel clock in Hz
    1056,      // H. total pixels
    800,       // H. active pixels
    800,       // H. blank start from start of active in pixels
    800 + 255, // H. blank end from start of active in pixels
    840,       // H. sync start from start of active in pixels
    840 + 127, // H. sync end from start of active in pixels
    37879,     // H. frequency in Hz
    628,       // V. total lines
    600,       // V. active lines
    600,       // V. blank start from start of active in lines
    600 + 27,  // V. blank end from start of active in lines
    601,       // V. sync start from start of active in lines
    601 + 3,   // V. sync end from start of active in lines
    60,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 800x600@72
DD_TIMING_INFO g_Timing800x600_72 = {

    50000000,  // Pixel clock in Hz
    1040,      // H. total pixels
    800,       // H. active pixels
    800,       // H. blank start from start of active in pixels
    800 + 239, // H. blank end from start of active in pixels
    856,       // H. sync start from start of active in pixels
    856 + 119, // H. sync end from start of active in pixels
    48077,     // H. frequency in Hz
    666,       // V. total lines
    600,       // V. active lines
    600,       // V. blank start from start of active in lines
    600 + 65,  // V. blank end from start of active in lines
    637,       // V. sync start from start of active in lines
    637 + 5,   // V. sync end from start of active in lines
    72,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 800x600@75
DD_TIMING_INFO g_Timing800x600_75 = {

    49500000,  // Pixel clock in Hz
    1056,      // H. total pixels
    800,       // H. active pixels
    800,       // H. blank start from start of active in pixels
    800 + 255, // H. blank end from start of active in pixels
    816,       // H. sync start from start of active in pixels
    816 + 79,  // H. sync end from start of active in pixels
    46875,     // H. frequency in Hz
    625,       // V. total lines
    600,       // V. active lines
    600,       // V. blank start from start of active in lines
    600 + 24,  // V. blank end from start of active in lines
    601,       // V. sync start from start of active in lines
    601 + 2,   // V. sync end from start of active in lines
    75,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 800x600@85
DD_TIMING_INFO g_Timing800x600_85 = {

    56250000,  // Pixel clock in Hz
    1048,      // H. total pixels
    800,       // H. active pixels
    800,       // H. blank start from start of active in pixels
    800 + 247, // H. blank end from start of active in pixels
    832,       // H. sync start from start of active in pixels
    832 + 63,  // H. sync end from start of active in pixels
    53674,     // H. frequency in Hz
    631,       // V. total lines
    600,       // V. active lines
    600,       // V. blank start from start of active in lines
    600 + 30,  // V. blank end from start of active in lines
    601,       // V. sync start from start of active in lines
    601 + 2,   // V. sync end from start of active in lines
    85,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 848x480@60
DD_TIMING_INFO g_Timing848x480_60 = {

    33750000,  // Pixel clock in Hz
    1088,      // H. total pixels
    848,       // H. active pixels
    848,       // H. blank start from start of active in pixels
    848 + 239, // H. blank end from start of active in pixels
    864,       // H. sync start from start of active in pixels
    864 + 111, // H. sync end from start of active in pixels
    31020,     // H. frequency in Hz
    517,       // V. total lines
    480,       // V. active lines
    480,       // V. blank start from start of active in lines
    480 + 36,  // V. blank end from start of active in lines
    486,       // V. sync start from start of active in lines
    486 + 7,   // V. sync end from start of active in lines
    60,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 1024x768@43i
DD_TIMING_INFO g_Timing1024x768_43i = {

    22450000,   // Pixel clock in Hz
    1264,       // H. total pixels
    1024,       // H. active pixels
    1024,       // H. blank start from start of active in pixels
    1024 + 239, // H. blank end from start of active in pixels
    1032,       // H. sync start from start of active in pixels
    1032 + 175, // H. sync end from start of active in pixels
    35522,      // H. frequency in Hz
    818,        // V. total lines
    768,        // V. active lines
    768,        // V. blank start from start of active in lines
    768 + 24,   // V. blank end from start of active in lines
    768,        // V. sync start from start of active in lines
    768 + 3,    // V. sync end from start of active in lines
    43,         // Refresh rate
    1,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+

};

//
//    VESA DMTS timing 1024x768@60
DD_TIMING_INFO g_Timing1024x768_60 = {

    65000000,   // Pixel clock in Hz
    1344,       // H. total pixels
    1024,       // H. active pixels
    1024,       // H. blank start from start of active in pixels
    1024 + 319, // H. blank end from start of active in pixels
    1048,       // H. sync start from start of active in pixels
    1048 + 135, // H. sync end from start of active in pixels
    48363,      // H. frequency in Hz
    806,        // V. total lines
    768,        // V. active lines
    768,        // V. blank start from start of active in lines
    768 + 37,   // V. blank end from start of active in lines
    771,        // V. sync start from start of active in lines
    771 + 5,    // V. sync end from start of active in lines
    60,         // Refresh rate
    0,          // Interlaced
    1,          // Timing flags -- H-, V-
    1,          // Timing flags -- H-, V-
};

//
//    VESA DMTS timing 1024x768@70
DD_TIMING_INFO g_Timing1024x768_70 = {

    75000000,   // Pixel clock in Hz
    1328,       // H. total pixels
    1024,       // H. active pixels
    1024,       // H. blank start from start of active in pixels
    1024 + 303, // H. blank end from start of active in pixels
    1048,       // H. sync start from start of active in pixels
    1048 + 135, // H. sync end from start of active in pixels
    56476,      // H. frequency in Hz
    806,        // V. total lines
    768,        // V. active lines
    768,        // V. blank start from start of active in lines
    768 + 37,   // V. blank end from start of active in lines
    771,        // V. sync start from start of active in lines
    771 + 5,    // V. sync end from start of active in lines
    70,         // Refresh rate
    0,          // Interlaced
    1,          // Timing flags -- H-, V-
    1,          // Timing flags -- H-, V-
};

//
//    VESA DMTS timing 1024x768@75
DD_TIMING_INFO g_Timing1024x768_75 = {

    78750000,   // Pixel clock in Hz
    1312,       // H. total pixels
    1024,       // H. active pixels
    1024,       // H. blank start from start of active in pixels
    1024 + 287, // H. blank end from start of active in pixels
    1040,       // H. sync start from start of active in pixels
    1040 + 95,  // H. sync end from start of active in pixels
    60023,      // H. frequency in Hz
    800,        // V. total lines
    768,        // V. active lines
    768,        // V. blank start from start of active in lines
    768 + 31,   // V. blank end from start of active in lines
    769,        // V. sync start from start of active in lines
    769 + 2,    // V. sync end from start of active in lines
    75,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 1024x768@85
DD_TIMING_INFO g_Timing1024x768_85 = {

    94500000,   // Pixel clock in Hz
    1376,       // H. total pixels
    1024,       // H. active pixels
    1024,       // H. blank start from start of active in pixels
    1024 + 351, // H. blank end from start of active in pixels
    1072,       // H. sync start from start of active in pixels
    1072 + 95,  // H. sync end from start of active in pixels
    68677,      // H. frequency in Hz
    808,        // V. total lines
    768,        // V. active lines
    768,        // V. blank start from start of active in lines
    768 + 39,   // V. blank end from start of active in lines
    769,        // V. sync start from start of active in lines
    769 + 2,    // V. sync end from start of active in lines
    85,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 1152x864@75
DD_TIMING_INFO g_Timing1152x864_75 = {

    108000000,  // Pixel clock in Hz
    1600,       // H. total pixels
    1152,       // H. active pixels
    1152,       // H. blank start from start of active in pixels
    1152 + 447, // H. blank end from start of active in pixels
    1216,       // H. sync start from start of active in pixels
    1216 + 127, // H. sync end from start of active in pixels
    67500,      // H. frequency in Hz
    900,        // V. total lines
    864,        // V. active lines
    864,        // V. blank start from start of active in lines
    864 + 35,   // V. blank end from start of active in lines
    865,        // V. sync start from start of active in lines
    865 + 2,    // V. sync end from start of active in lines
    75,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 1280x960@60
DD_TIMING_INFO g_Timing1280x960_60 = {

    108000000,  // Pixel clock in Hz
    1800,       // H. total pixels
    1280,       // H. active pixels
    1280,       // H. blank start from start of active in pixels
    1280 + 519, // H. blank end from start of active in pixels
    1376,       // H. sync start from start of active in pixels
    1376 + 111, // H. sync end from start of active in pixels
    60000,      // H. frequency in Hz
    1000,       // V. total lines
    960,        // V. active lines
    960,        // V. blank start from start of active in lines
    960 + 39,   // V. blank end from start of active in lines
    961,        // V. sync start from start of active in lines
    961 + 2,    // V. sync end from start of active in lines
    60,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 1280x960@85
DD_TIMING_INFO g_Timing1280x960_85 = {

    148500000,  // Pixel clock in Hz
    1728,       // H. total pixels
    1280,       // H. active pixels
    1280,       // H. blank start from start of active in pixels
    1280 + 447, // H. blank end from start of active in pixels
    1344,       // H. sync start from start of active in pixels
    1344 + 159, // H. sync end from start of active in pixels
    85938,      // H. frequency in Hz
    1011,       // V. total lines
    960,        // V. active lines
    960,        // V. blank start from start of active in lines
    960 + 50,   // V. blank end from start of active in lines
    961,        // V. sync start from start of active in lines
    961 + 2,    // V. sync end from start of active in lines
    85,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 1280x1024@60
DD_TIMING_INFO g_Timing1280x1024_60 = {

    108000000,  // Pixel clock in Hz
    1688,       // H. total pixels
    1280,       // H. active pixels
    1280,       // H. blank start from start of active in pixels
    1280 + 407, // H. blank end from start of active in pixels
    1328,       // H. sync start from start of active in pixels
    1328 + 111, // H. sync end from start of active in pixels
    63981,      // H. frequency in Hz
    1066,       // V. total lines
    1024,       // V. active lines
    1024,       // V. blank start from start of active in lines
    1024 + 41,  // V. blank end from start of active in lines
    1025,       // V. sync start from start of active in lines
    1025 + 2,   // V. sync end from start of active in lines
    60,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 1280x1024@75
DD_TIMING_INFO g_Timing1280x1024_75 = {

    135000000,  // Pixel clock in Hz
    1688,       // H. total pixels
    1280,       // H. active pixels
    1280,       // H. blank start from start of active in pixels
    1280 + 407, // H. blank end from start of active in pixels
    1296,       // H. sync start from start of active in pixels
    1296 + 143, // H. sync end from start of active in pixels
    79976,      // H. frequency in Hz
    1066,       // V. total lines
    1024,       // V. active lines
    1024,       // V. blank start from start of active in lines
    1024 + 41,  // V. blank end from start of active in lines
    1025,       // V. sync start from start of active in lines
    1025 + 2,   // V. sync end from start of active in lines
    75,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 1280x1024@85
DD_TIMING_INFO g_Timing1280x1024_85 = {

    157500000,  // Pixel clock in Hz
    1728,       // H. total pixels
    1280,       // H. active pixels
    1280,       // H. blank start from start of active in pixels
    1280 + 447, // H. blank end from start of active in pixels
    1344,       // H. sync start from start of active in pixels
    1344 + 159, // H. sync end from start of active in pixels
    91146,      // H. frequency in Hz
    1072,       // V. total lines
    1024,       // V. active lines
    1024,       // V. blank start from start of active in lines
    1024 + 47,  // V. blank end from start of active in lines
    1025,       // V. sync start from start of active in lines
    1025 + 2,   // V. sync end from start of active in lines
    85,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 1360x768@60
DD_TIMING_INFO g_Timing1360x768_60 = {

    85500000,   // Pixel clock in Hz
    1792,       // H. total pixels
    1360,       // H. active pixels
    1360,       // H. blank start from start of active in pixels
    1360 + 431, // H. blank end from start of active in pixels
    1424,       // H. sync start from start of active in pixels
    1424 + 111, // H. sync end from start of active in pixels
    47712,      // H. frequency in Hz
    795,        // V. total lines
    768,        // V. active lines
    768,        // V. blank start from start of active in lines
    768 + 26,   // V. blank end from start of active in lines
    771,        // V. sync start from start of active in lines
    771 + 5,    // V. sync end from start of active in lines
    60,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 1366x768@60
DD_TIMING_INFO g_Timing1366x768_60 = {

    85500000,   // Pixel clock in Hz
    1792,       // H. total pixels
    1366,       // H. active pixels
    1366,       // H. blank start from start of active in pixels
    1366 + 425, // H. blank end from start of active in pixels
    1436,       // H. sync start from start of active in pixels
    1436 + 142, // H. sync end from start of active in pixels
    47712,      // H. frequency in Hz
    798,        // V. total lines
    768,        // V. active lines
    768,        // V. blank start from start of active in lines
    768 + 29,   // V. blank end from start of active in lines
    771,        // V. sync start from start of active in lines
    771 + 2,    // V. sync end from start of active in lines
    60,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 1366x768@60rb
DD_TIMING_INFO g_Timing1366x768_60rb = {

    72000000,   // Pixel clock in Hz
    1500,       // H. total pixels
    1366,       // H. active pixels
    1366,       // H. blank start from start of active in pixels
    1366 + 133, // H. blank end from start of active in pixels
    1380,       // H. sync start from start of active in pixels
    1380 + 55,  // H. sync end from start of active in pixels
    48000,      // H. frequency in Hz
    800,        // V. total lines
    768,        // V. active lines
    768,        // V. blank start from start of active in lines
    768 + 31,   // V. blank end from start of active in lines
    769,        // V. sync start from start of active in lines
    769 + 2,    // V. sync end from start of active in lines
    60,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

//    VESA DMTS timing 1600x900@60rb
DD_TIMING_INFO g_Timing1600x900_60rb = {

    108000000,  // Pixel clock in Hz
    1800,       // H. total pixels
    1600,       // H. active pixels
    1600,       // H. blank start from start of active in pixels
    1600 + 199, // H. blank end from start of active in pixels
    1624,       // H. sync start from start of active in pixels
    1624 + 79,  // H. sync end from start of active in pixels
    60000,      // H. frequency in Hz
    1000,       // V. total lines
    900,        // V. active lines
    900,        // V. blank start from start of active in lines
    900 + 99,   // V. blank end from start of active in lines
    901,        // V. sync start from start of active in lines
    901 + 2,    // V. sync end from start of active in lines
    60,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 1600x1200@60
DD_TIMING_INFO g_Timing1600x1200_60 = {

    162000000,  // Pixel clock in Hz
    2160,       // H. total pixels
    1600,       // H. active pixels
    1600,       // H. blank start from start of active in pixels
    1600 + 559, // H. blank end from start of active in pixels
    1664,       // H. sync start from start of active in pixels
    1664 + 191, // H. sync end from start of active in pixels
    75000,      // H. frequency in Hz
    1250,       // V. total lines
    1200,       // V. active lines
    1200,       // V. blank start from start of active in lines
    1200 + 49,  // V. blank end from start of active in lines
    1201,       // V. sync start from start of active in lines
    1201 + 2,   // V. sync end from start of active in lines
    60,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 1600x1200@65
DD_TIMING_INFO g_Timing1600x1200_65 = {

    175500000,  // Pixel clock in Hz
    2160,       // H. total pixels
    1600,       // H. active pixels
    1600,       // H. blank start from start of active in pixels
    1600 + 559, // H. blank end from start of active in pixels
    1664,       // H. sync start from start of active in pixels
    1664 + 191, // H. sync end from start of active in pixels
    81250,      // H. frequency in Hz
    1250,       // V. total lines
    1200,       // V. active lines
    1200,       // V. blank start from start of active in lines
    1200 + 49,  // V. blank end from start of active in lines
    1201,       // V. sync start from start of active in lines
    1201 + 2,   // V. sync end from start of active in lines
    65,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 1600x1200@70
DD_TIMING_INFO g_Timing1600x1200_70 = {

    189000000,  // Pixel clock in Hz
    2160,       // H. total pixels
    1600,       // H. active pixels
    1600,       // H. blank start from start of active in pixels
    1600 + 559, // H. blank end from start of active in pixels
    1664,       // H. sync start from start of active in pixels
    1664 + 191, // H. sync end from start of active in pixels
    87500,      // H. frequency in Hz
    1250,       // V. total lines
    1200,       // V. active lines
    1200,       // V. blank start from start of active in lines
    1200 + 49,  // V. blank end from start of active in lines
    1201,       // V. sync start from start of active in lines
    1201 + 2,   // V. sync end from start of active in lines
    70,         // Refresh rate
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 1600x1200@75
DD_TIMING_INFO g_Timing1600x1200_75 = {

    202500000,  // Pixel clock in Hz
    2160,       // H. total pixels
    1600,       // H. active pixels
    1600,       // H. blank start from start of active in pixels
    1600 + 559, // H. blank end from start of active in pixels
    1664,       // H. sync start from start of active in pixels
    1664 + 191, // H. sync end from start of active in pixels
    93750,      // H. frequency in Hz
    1250,       // V. total lines
    1200,       // V. active lines
    1200,       // V. blank start from start of active in lines
    1200 + 49,  // V. blank end from start of active in lines
    1201,       // V. sync start from start of active in lines
    1201 + 2,   // V. sync end from start of active in lines
    75,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 1600x1200@85
DD_TIMING_INFO g_Timing1600x1200_85 = {

    229500000,  // Pixel clock in Hz
    2160,       // H. total pixels
    1600,       // H. active pixels
    1600,       // H. blank start from start of active in pixels
    1600 + 559, // H. blank end from start of active in pixels
    1664,       // H. sync start from start of active in pixels
    1664 + 191, // H. sync end from start of active in pixels
    106250,     // H. frequency in Hz
    1250,       // V. total lines
    1200,       // V. active lines
    1200,       // V. blank start from start of active in lines
    1200 + 49,  // V. blank end from start of active in lines
    1201,       // V. sync start from start of active in lines
    1201 + 2,   // V. sync end from start of active in lines
    85,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 1792x1344@60
DD_TIMING_INFO g_Timing1792x1344_60 = {

    204750000,  // Pixel clock in Hz
    2448,       // H. total pixels
    1792,       // H. active pixels
    1792,       // H. blank start from start of active in pixels
    1792 + 655, // H. blank end from start of active in pixels
    1920,       // H. sync start from start of active in pixels
    1920 + 199, // H. sync end from start of active in pixels
    83640,      // H. frequency in Hz
    1394,       // V. total lines
    1344,       // V. active lines
    1344,       // V. blank start from start of active in lines
    1344 + 49,  // V. blank end from start of active in lines
    1345,       // V. sync start from start of active in lines
    1345 + 2,   // V. sync end from start of active in lines
    60,         // Refresh rate
    0,          // Interlaced
    1,          // Timing flags -- H-, V+
    0,          // Timing flags -- H-, V+
};

//
//    VESA DMTS timing 1792x1344@75
DD_TIMING_INFO g_Timing1792x1344_75 = {

    261000000,  // Pixel clock in Hz
    2456,       // H. total pixels
    1792,       // H. active pixels
    1792,       // H. blank start from start of active in pixels
    1792 + 663, // H. blank end from start of active in pixels
    1888,       // H. sync start from start of active in pixels
    1888 + 215, // H. sync end from start of active in pixels
    106270,     // H. frequency in Hz
    1417,       // V. total lines
    1344,       // V. active lines
    1344,       // V. blank start from start of active in lines
    1344 + 72,  // V. blank end from start of active in lines
    1345,       // V. sync start from start of active in lines
    1345 + 2,   // V. sync end from start of active in lines
    75,         // Refresh rate
    0,          // Interlaced
    1,          // Timing flags -- H-, V+
    0,          // Timing flags -- H-, V+
};

//
//    VESA DMTS timing 1856x1392@60
DD_TIMING_INFO g_Timing1856x1392_60 = {

    218250000,  // Pixel clock in Hz
    2528,       // H. total pixels
    1856,       // H. active pixels
    1856,       // H. blank start from start of active in pixels
    1856 + 671, // H. blank end from start of active in pixels
    1952,       // H. sync start from start of active in pixels
    1952 + 223, // H. sync end from start of active in pixels
    86333,      // H. frequency in Hz
    1439,       // V. total lines
    1392,       // V. active lines
    1392,       // V. blank start from start of active in lines
    1392 + 46,  // V. blank end from start of active in lines
    1393,       // V. sync start from start of active in lines
    1393 + 2,   // V. sync end from start of active in lines
    60,         // Refresh rate
    0,          // Interlaced
    1,          // Timing flags -- H-, V+
    0,          // Timing flags -- H-, V+
};

//
//    VESA DMTS timing 1856x1392@75
DD_TIMING_INFO g_Timing1856x1392_75 = {

    288000000,  // Pixel clock in Hz
    2560,       // H. total pixels
    1856,       // H. active pixels
    1856,       // H. blank start from start of active in pixels
    1856 + 703, // H. blank end from start of active in pixels
    1984,       // H. sync start from start of active in pixels
    1984 + 223, // H. sync end from start of active in pixels
    112500,     // H. frequency in Hz
    1500,       // V. total lines
    1392,       // V. active lines
    1392,       // V. blank start from start of active in lines
    1392 + 107, // V. blank end from start of active in lines
    1393,       // V. sync start from start of active in lines
    1393 + 2,   // V. sync end from start of active in lines
    75,         // Refresh rate
    0,          // Interlaced
    1,          // Timing flags -- H-, V+
    0,          // Timing flags -- H-, V+
};

//
//    VESA DMTS timing 1920x1440@60
DD_TIMING_INFO g_Timing1920x1440_60 = {

    234000000,  // Pixel clock in Hz
    2600,       // H. total pixels
    1920,       // H. active pixels
    1920,       // H. blank start from start of active in pixels
    1920 + 679, // H. blank end from start of active in pixels
    2048,       // H. sync start from start of active in pixels
    2048 + 207, // H. sync end from start of active in pixels
    90000,      // H. frequency in Hz
    1500,       // V. total lines
    1440,       // V. active lines
    1440,       // V. blank start from start of active in lines
    1440 + 59,  // V. blank end from start of active in lines
    1441,       // V. sync start from start of active in lines
    1441 + 2,   // V. sync end from start of active in lines
    60,         // Refresh rate
    0,          // Interlaced
    1,          // Timing flags -- H-, V+
    0,          // Timing flags -- H-, V+
};

//
//    VESA DMTS timing 1920x1440@75
DD_TIMING_INFO g_Timing1920x1440_75 = {

    297000000,  // Pixel clock in Hz
    2640,       // H. total pixels
    1920,       // H. active pixels
    1920,       // H. blank start from start of active in pixels
    1920 + 719, // H. blank end from start of active in pixels
    2064,       // H. sync start from start of active in pixels
    2064 + 223, // H. sync end from start of active in pixels
    112500,     // H. frequency in Hz
    1500,       // V. total lines
    1440,       // V. active lines
    1440,       // V. blank start from start of active in lines
    1440 + 59,  // V. blank end from start of active in lines
    1441,       // V. sync start from start of active in lines
    1441 + 2,   // V. sync end from start of active in lines
    75,         // Refresh rate
    0,          // Interlaced
    1,          // Timing flags -- H-, V+
    0,          // Timing flags -- H-, V+
};

//
//    CVT reduced blanking timing for 2560x1600@60
DD_TIMING_INFO g_Timing2560x1600_60 = {

    268000000, // Pixel clock in Hz
    2720,      // H. total pixels
    2560,      // H. active pixels
    2560,      // H. blank start from start of active in pixels
    2719,      // H. blank end from start of active in pixels
    2608,      // H. sync start from start of active in pixels
    2639,      // H. sync end from start of active in pixels
    99,        // H. frequency in Hz
    1646,      // V. total lines
    1600,      // V. active lines
    1600,      // V. blank start from start of active in lines
    1645,      // V. blank end from start of active in lines
    1603,      // V. sync start from start of active in lines
    1608,      // V. sync end from start of active in lines
    60,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V-
    1,         // Timing flags -- H+, V-
};

//
//    CVT reduced blanking timing for 2560x1600@75
DD_TIMING_INFO g_Timing2560x1600_75 = {

    3388000000, // Pixel clock in Hz
    2720,       // H. total pixels
    2560,       // H. active pixels
    2560,       // H. blank start from start of active in pixels
    2719,       // H. blank end from start of active in pixels
    2608,       // H. sync start from start of active in pixels
    2639,       // H. sync end from start of active in pixels
    124,        // H. frequency in Hz
    1658,       // V. total lines
    1600,       // V. active lines
    1600,       // V. blank start from start of active in lines
    1657,       // V. blank end from start of active in lines
    1603,       // V. sync start from start of active in lines
    1608,       // V. sync end from start of active in lines
    75,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V-
    1,          // Timing flags -- H+, V-
};

////////////////////////////////////////////////////////
//
// Timing Info for CE-Xtn Short Video Descriptor Modes
//
////////////////////////////////////////////////////////

//
//    CE 480p timing 640x480@60
DD_TIMING_INFO g_Timing640x480p_60 = {
    25200000, // Pixel clock in Hz
    800,      // H. total
    640,      // H active
    640,      // H blank start from start of active
    799,      // H blank end from start of acive
    656,      // H sync start from start of active
    751,      // 752 - 1 // H sync end from start of active
    31500,    // H frequency in Hz
    525,      // V total lines
    480,      // V active
    480,      // V blank start from start of active
    524,      // V blank end from start of active
    490,      // V sync start from start of active
    491,      // V sync end from start of active
    60,       // Refresh rate
    0,        // Interlaced
    1,        // Timing flags -- H-, V-
    1,        // Timing flags -- H-, V-
};

//
//    CE 861b timing 720x480@60 , 4:3 and 16:9
DD_TIMING_INFO g_Timing720x480_60 = {

    27000000, // Pixel clock in Hz
    858,      // H. total pixels
    720,      // H. active pixels
    720,      // H. blank start from start of active in pixels
    857,      // H. blank end from start of active in pixels , as per the monitor timing doc
    736,      // H. sync start from start of active in pixels
    797,      // H. sync end from start of active in pixels
    31468,    // H. frequency in Hz
    525,      // V. total lines
    480,      // V. active lines
    480,      // V. blank start from start of active in lines
    524,      // V. blank end from start of active in lines
    489,      // V. sync start from start of active in lines
    494,      // V. sync end from start of active in lines
    60,       // Refresh rate
    0,        // Interlaced
    1,        // Timing flags -- H-, V+
    1,        // Timing flags -- H-, V-
};

//
//    CE 861b timing 1280x720x60  16:9
DD_TIMING_INFO g_Timing1280x720_a_60 = {

    74250000, // Pixel clock in Hz
    1650,     // H. total pixels
    1280,     // H. active pixels
    1280,     // H. blank start from start of active in pixels
    1649,     // H. blank end from start of active in pixels
    1390,     // H. sync start from start of active in pixels
    1429,     // H. sync end from start of active in pixels
    45000,    // H. frequency in Hz
    750,      // V. total lines
    720,      // V. active lines
    720,      // V. blank start from start of active in lines
    749,      // V. blank end from start of active in lines
    725,      // V. sync start from start of active in lines
    729,      // V. sync end from start of active in lines
    60,       // Refresh Rate
    0,        // Interlaced
    0,        // Timing flags -- H+, V+
    0,        // Timing flags -- H+, V+
};

//
//    VESA DMTS timing 1920X1080X60 INTERLACED 16:9
DD_TIMING_INFO g_Timing1920x1080_i_a_60 = {

    74250000, // Pixel clock in Hz
    2200,     // H. total pixels
    1920,     // H. active pixels
    1920,     // H. blank start from start of active in pixels
    2199,     // H. blank end from start of active in pixels
    2008,     // H. sync start from start of active in pixels
    2051,     // H. sync end from start of active in pixels
    33750,    // H. frequency in Hz
    1125,     // V. total lines
    1080,     // V. active lines
    1080,     // V. blank start from start of active in lines
    1124,     // V. blank end from start of active in lines
    1084,     // V. sync start from start of active in lines
    // 1088,                    // V. sync end from start of active in lines
    1093, // Fix for 1940035: interlaced mode sync pulse width is doubled.
    60,   // Refresh rate
    1,    // Interlaced
    0,    // Timing flags -- H+, V+
    0,    // Timing flags -- H+, V+

};

//
// VESA DMTS timing 720X576X50 /861 CE timing 720X576X50 16:9 and 4:30
DD_TIMING_INFO g_Timing720x576_50 = {

    27000000, // Pixel clock in Hz
    864,      // H. total pixels
    720,      // H. active pixels
    720,      // H. blank start from start of active in pixels
    863,      // H. blank end from start of active in pixels
    732,      // H. sync start from start of active in pixels
    795,      // H. sync end from start of active in pixels
    31250,    // H. frequency in Hz
    625,      // V. total lines
    576,      // V. active lines
    576,      // V. blank start from start of active in lines
    624,      // V. blank end from start of active in lines
    581,      // V. sync start from start of active in lines
    585,      // V. sync end from start of active in lines
    50,       // Refresh rate
    0,        // Interlaced
    1,        // Timing flags -- H-, V-
    1,        // Timing flags -- H-, V-
};

//
// Updated the VESA modes according to latest VESA DMTS spec
//
// VESA DMTS 1280x720@ 60  - Identical to 861 CE Timing
// VESA DMTS 1920x1080@ 60 - Identical to 861 CE Timing

// VESA DMT 2048x1152 @ 60 (Reduced blanking)
DD_TIMING_INFO g_Timing2048x1152_60rb = {

    162000000,  // Pixel clock in Hz
    2250,       // H. total pixels
    2048,       // H. active pixels
    2048,       // H. blank start from start of active in pixels
    2048 + 201, // H. blank end from start of active in pixels
    2074,       // H. sync start from start of active in pixels
    2074 + 79,  // H. sync end from start of active in pixels
    72000,      // H. frequency in Hz
    1200,       // V. total lines
    1152,       // V. active lines
    1152,       // V. blank start from start of active in lines
    1152 + 47,  // V. blank end from start of active in lines
    1153,       // V. sync start from start of active in lines
    1153 + 2,   // V. sync end from start of active in lines
    60,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

//
//    861 CE 1280x720x50
DD_TIMING_INFO g_Timing1280x720_50 = {

    74250000, // Pixel clock in Hz
    1980,     // H. total pixels
    1280,     // H. active pixels
    1280,     // H. blank start from start of active in pixels
    1979,     // H. blank end from start of active in pixels
    1720,     // H. sync start from start of active in pixels
    1759,     // H. sync end from start of active in pixels
    37500,    // H. frequency in Hz
    750,      // V. total lines
    720,      // V. active lines
    720,      // V. blank start from start of active in lines
    749,      // V. blank end from start of active in lines
    725,      // V. sync start from start of active in lines
    729,      // V. sync end from start of active in lines
    50,       // Refresh Rate
    0,        // Interlaced
    0,        // Timing flags -- H+, V+
    0,        // Timing flags -- H+, V+
};

//
//    861 CE 19x12x50  interlaced 16:9
DD_TIMING_INFO g_Timing1920x1080_i_a_50 = {

    74250000, // Pixel clock in Hz
    2640,     // H. total pixels
    1920,     // H. active pixels
    1920,     // H. blank start from start of active in pixels
    2639,     // H. blank end from start of active in pixels
    2448,     // H. sync start from start of active in pixels
    2491,     // H. sync end from start of active in pixels
    28125,    // H. frequency in Hz
    1125,     // V. total lines
    1080,     // V. active lines
    1080,     // V. blank start from start of active in lines
    1124,     // V. blank end from start of active in lines
    1084,     // V. sync start from start of active in lines
    // 1088,                    // V. sync end from start of active in lines
    1093, // Fix for 1940035: interlaced mode sync pulse width is doubled.
    50,   // Refresh rate
    1,    // Interlaced
    0,    // Timing flags -- H+, V+
    0,    // Timing flags -- H+, V+
};

//  VESA DMTS timing 1920x1080x50 16:9
DD_TIMING_INFO g_Timing1920x1080_a_50 = {

    148500000, // Pixel clock in Hz
    2640,      // H. total pixels
    1920,      // H. active pixels
    1920,      // H. blank start from start of active in pixels
    2639,      // H. blank end from start of active in pixels
    2448,      // H. sync start from start of active in pixels
    2491,      // H. sync end from start of active in pixels
    56250,     // H. frequency in Hz
    1125,      // V. total lines
    1080,      // V. active lines
    1080,      // V. blank start from start of active in lines
    1124,      // V. blank end from start of active in lines
    1084,      // V. sync start from start of active in lines
    1088,      // V. sync end from start of active in lines
    50,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

//
//    861 CE 1920x1080x60 16:9
DD_TIMING_INFO g_Timing1920x1080_a_60 = {

    148500000, // Pixel clock in Hz
    2200,      // H. total pixels
    1920,      // H. active pixels
    1920,      // H. blank start from start of active in pixels
    2199,      // H. blank end from start of active in pixels
    2008,      // H. sync start from start of active in pixels
    2051,      // H. sync end from start of active in pixels
    67500,     // H. frequency in Hz
    1125,      // V. total lines
    1080,      // V. active lines
    1080,      // V. blank start from start of active in lines
    1124,      // V. blank end from start of active in lines
    1084,      // V. sync start from start of active in lines
    1088,      // V. sync end from start of active in lines
    60,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Formats 6 and 7
DD_TIMING_INFO g_Timing1440x480_i_60 = {

    27000000, // Pixel clock in Hz
    1716,     // H. total pixels
    1440,     // H. active pixels
    1440,     // H. blank start from start of active in pixels
    1715,     // H. blank end from start of active in pixels , as per the monitor timing doc
    1478,     // H. sync start from start of active in pixels
    1601,     // H. sync end from start of active in pixels
    15734,    // H. frequency in Hz
    525,      // V. total lines
    480,      // V. active lines
    480,      // V. blank start from start of active in lines
    524,      // V. blank end from start of active in lines
    488,      // V. sync start from start of active in lines
    493,      // V. sync end from start of active in lines
    60,       // Refresh rate
    1,        // Interlaced
    1,        // Timing flags -- H-, V-
    1,        // Timing flags -- H-, V-
};

// Formats 8 & 9
DD_TIMING_INFO g_Timing1440x240_60 = {

    27027000, // Pixel clock in Hz
    1716,     // H. total pixels
    1440,     // H. active pixels
    1440,     // H. blank start from start of active in pixels
    1715,     // H. blank end from start of active in pixels , as per the monitor timing doc
    1478,     // H. sync start from start of active in pixels
    1601,     // H. sync end from start of active in pixels
    15734,    // H. frequency in Hz
    262,      // V. total lines
    240,      // V. active lines
    240,      // V. blank start from start of active in lines
    261,      // V. blank end from start of active in lines
    244,      // V. sync start from start of active in lines
    246,      // V. sync end from start of active in lines
    60,       // Refresh rate
    0,        // Interlaced
    1,        // Timing flags -- H-, V-
    1,        // Timing flags -- H-, V-
};

// Formats 10 and 11
DD_TIMING_INFO g_Timing2880x480_i_60 = {

    54000000, // Pixel clock in Hz
    3432,     // H. total pixels
    2880,     // H. active pixels
    2880,     // H. blank start from start of active in pixels
    3431,     // H. blank end from start of active in pixels , as per the monitor timing doc
    2956,     // H. sync start from start of active in pixels
    3203,     // H. sync end from start of active in pixels
    15734,    // H. frequency in Hz
    525,      // V. total lines
    480,      // V. active lines
    480,      // V. blank start from start of active in lines
    524,      // V. blank end from start of active in lines
    488,      // V. sync start from start of active in lines
    493,      // V. sync end from start of active in lines
    60,       // Refresh rate
    1,        // Interlaced
    1,        // Timing flags -- H-, V-
    1,        // Timing flags -- H-, V-
};

// Formats 12 and 13
DD_TIMING_INFO g_Timing2880x240_60 = {

    54000000, // Pixel clock in Hz
    3432,     // H. total pixels
    2880,     // H. active pixels
    2880,     // H. blank start from start of active in pixels
    3431,     // H. blank end from start of active in pixels , as per the monitor timing doc
    2956,     // H. sync start from start of active in pixels
    3203,     // H. sync end from start of active in pixels
    15734,    // H. frequency in Hz
    263,      // V. total lines
    240,      // V. active lines
    240,      // V. blank start from start of active in lines
    262,      // V. blank end from start of active in lines
    245,      // V. sync start from start of active in lines
    247,      // V. sync end from start of active in lines
    60,       // Refresh rate
    0,        // Interlaced
    1,        // Timing flags -- H-, V-
    1,        // Timing flags -- H-, V-
};

// Formats 14 and 15
DD_TIMING_INFO g_Timing1440x480_60 = {

    54000000, // Pixel clock in Hz
    1716,     // H. total pixels
    1440,     // H. active pixels
    1440,     // H. blank start from start of active in pixels
    1715,     // H. blank end from start of active in pixels , as per the monitor timing doc
    1472,     // H. sync start from start of active in pixels
    1595,     // H. sync end from start of active in pixels
    31469,    // H. frequency in Hz
    525,      // V. total lines
    480,      // V. active lines
    480,      // V. blank start from start of active in lines
    524,      // V. blank end from start of active in lines
    489,      // V. sync start from start of active in lines
    494,      // V. sync end from start of active in lines
    60,       // Refresh rate
    0,        // Interlaced
    1,        // Timing flags -- H-, V-
    1,        // Timing flags -- H-, V-
};

// Formats 21 & 22
DD_TIMING_INFO g_Timing1440x576_i_50 = {

    27000000, // Pixel clock in Hz
    1728,     // H. total pixels
    1440,     // H. active pixels
    1440,     // H. blank start from start of active in pixels
    1727,     // H. blank end from start of active in pixels , as per the monitor timing doc
    1464,     // H. sync start from start of active in pixels
    1589,     // H. sync end from start of active in pixels
    15625,    // H. frequency in Hz
    625,      // V. total lines
    576,      // V. active lines
    576,      // V. blank start from start of active in lines
    624,      // V. blank end from start of active in lines
    580,      // V. sync start from start of active in lines
    585,      // V. sync end from start of active in lines
    50,       // Refresh rate
    1,        // Interlaced
    1,        // Timing flags -- H-, V-
    1,        // Timing flags -- H-, V-
};

// Formats 23 and 24
DD_TIMING_INFO g_Timing1440x288_50 = {

    27000000, // Pixel clock in Hz
    1728,     // H. total pixels
    1440,     // H. active pixels
    1440,     // H. blank start from start of active in pixels
    1727,     // H. blank end from start of active in pixels , as per the monitor timing doc
    1464,     // H. sync start from start of active in pixels
    1589,     // H. sync end from start of active in pixels
    15625,    // H. frequency in Hz
    313,      // V. total lines
    288,      // V. active lines
    288,      // V. blank start from start of active in lines
    312,      // V. blank end from start of active in lines
    291,      // V. sync start from start of active in lines
    293,      // V. sync end from start of active in lines
    50,       // Refresh rate
    0,        // Interlaced
    1,        // Timing flags -- H-, V-
    1,        // Timing flags -- H-, V-
};

// Formats 25 and 26
DD_TIMING_INFO g_Timing2880x576_i_50 = {

    54000000, // Pixel clock in Hz
    3456,     // H. total pixels
    2880,     // H. active pixels
    2880,     // H. blank start from start of active in pixels
    3455,     // H. blank end from start of active in pixels , as per the monitor timing doc
    2928,     // H. sync start from start of active in pixels
    3179,     // H. sync end from start of active in pixels
    15625,    // H. frequency in Hz
    625,      // V. total lines
    576,      // V. active lines
    576,      // V. blank start from start of active in lines
    624,      // V. blank end from start of active in lines
    580,      // V. sync start from start of active in lines
    585,      // V. sync end from start of active in lines
    50,       // Refresh rate
    1,        // Interlaced
    1,        // Timing flags -- H-, V-
    1,        // Timing flags -- H-, V-
};

// Formats 27 and 28
DD_TIMING_INFO g_Timing2880x288_50 = {

    54000000, // Pixel clock in Hz
    3456,     // H. total pixels
    2880,     // H. active pixels
    2880,     // H. blank start from start of active in pixels
    3455,     // H. blank end from start of active in pixels , as per the monitor timing doc
    2928,     // H. sync start from start of active in pixels
    3179,     // H. sync end from start of active in pixels
    15625,    // H. frequency in Hz
    313,      // V. total lines
    288,      // V. active lines
    288,      // V. blank start from start of active in lines
    312,      // V. blank end from start of active in lines
    291,      // V. sync start from start of active in lines
    293,      // V. sync end from start of active in lines
    50,       // Refresh rate
    0,        // Interlaced
    1,        // Timing flags -- H-, V-
    1,        // Timing flags -- H-, V-
};

// Formats 29 and 30
DD_TIMING_INFO g_Timing1440x576_50 = {

    54000000, // Pixel clock in Hz
    1728,     // H. total pixels
    1440,     // H. active pixels
    1440,     // H. blank start from start of active in pixels
    1727,     // H. blank end from start of active in pixels , as per the monitor timing doc
    1464,     // H. sync start from start of active in pixels
    1591,     // H. sync end from start of active in pixels
    31250,    // H. frequency in Hz
    625,      // V. total lines
    576,      // V. active lines
    576,      // V. blank start from start of active in lines
    624,      // V. blank end from start of active in lines
    581,      // V. sync start from start of active in lines
    585,      // V. sync end from start of active in lines
    50,       // Refresh rate
    0,        // Interlaced
    1,        // Timing flags -- H-, V-
    1,        // Timing flags -- H-, V-
};

// Format 32 & 72
DD_TIMING_INFO g_Timing1920x1080_24 = {

    74250000, // Pixel clock in Hz
    2750,     // H. total pixels
    1920,     // H. active pixels
    1920,     // H. blank start from start of active in pixels
    2749,     // H. blank end from start of active in pixels
    2558,     // H. sync start from start of active in pixels
    2601,     // H. sync end from start of active in pixels
    27000,    // H. frequency in Hz
    1125,     // V. total lines
    1080,     // V. active lines
    1080,     // V. blank start from start of active in lines
    1124,     // V. blank end from start of active in lines
    1084,     // V. sync start from start of active in lines
    1088,     // V. sync end from start of active in lines
    24,       // Refresh rate
    0,        // Interlaced
    0,        // Timing flags -- H+, V+
    0,        // Timing flags -- H+, V+
};

// Format 33 & 73
DD_TIMING_INFO g_Timing1920x1080_25 = {

    74250000, // Pixel clock in Hz
    2640,     // H. total pixels
    1920,     // H. active pixels
    1920,     // H. blank start from start of active in pixels
    2639,     // H. blank end from start of active in pixels
    2448,     // H. sync start from start of active in pixels
    2491,     // H. sync end from start of active in pixels
    28125,    // H. frequency in Hz
    1125,     // V. total lines
    1080,     // V. active lines
    1080,     // V. blank start from start of active in lines
    1124,     // V. blank end from start of active in lines
    1084,     // V. sync start from start of active in lines
    1088,     // V. sync end from start of active in lines
    25,       // Refresh rate
    0,        // Interlaced
    0,        // Timing flags -- H+, V+
    0,        // Timing flags -- H+, V+
};

// Format 34 & 74
DD_TIMING_INFO g_Timing1920x1080_30 = {

    74250000, // Pixel clock in Hz
    2200,     // H. total pixels
    1920,     // H. active pixels
    1920,     // H. blank start from start of active in pixels
    2199,     // H. blank end from start of active in pixels
    2008,     // H. sync start from start of active in pixels
    2051,     // H. sync end from start of active in pixels
    33750,    // H. frequency in Hz
    1125,     // V. total lines
    1080,     // V. active lines
    1080,     // V. blank start from start of active in lines
    1124,     // V. blank end from start of active in lines
    1084,     // V. sync start from start of active in lines
    1088,     // V. sync end from start of active in lines
    30,       // Refresh rate
    0,        // Interlaced
    0,        // Timing flags -- H+, V+
    0,        // Timing flags -- H+, V+
};

// Format 35 & 36
DD_TIMING_INFO g_Timing2880x480_60 = {

    108000000, // Pixel clock in Hz
    3432,      // H. total pixels
    2880,      // H. active pixels
    2880,      // H. blank start from start of active in pixels
    3431,      // H. blank end from start of active in pixels , as per the monitor timing doc
    2944,      // H. sync start from start of active in pixels
    3191,      // H. sync end from start of active in pixels
    31469,     // H. frequency in Hz
    525,       // V. total lines
    480,       // V. active lines
    480,       // V. blank start from start of active in lines
    524,       // V. blank end from start of active in lines
    489,       // V. sync start from start of active in lines
    494,       // V. sync end from start of active in lines
    60,        // Refresh rate
    0,         // Interlaced
    1,         // Timing flags -- H-, V-
    1,         // Timing flags -- H-, V-
};

// Format 37 & 38
DD_TIMING_INFO g_Timing2880x576_50 = {

    108000000, // Pixel clock in Hz
    3456,      // H. total pixels
    2880,      // H. active pixels
    2880,      // H. blank start from start of active in pixels
    3455,      // H. blank end from start of active in pixels , as per the monitor timing doc
    2928,      // H. sync start from start of active in pixels
    3183,      // H. sync end from start of active in pixels
    31250,     // H. frequency in Hz
    625,       // V. total lines
    576,       // V. active lines
    576,       // V. blank start from start of active in lines
    624,       // V. blank end from start of active in lines
    581,       // V. sync start from start of active in lines
    585,       // V. sync end from start of active in lines
    50,        // Refresh rate
    0,         // Interlaced
    1,         // Timing flags -- H-, V-
    1,         // Timing flags -- H-, V-
};

// Format 39
DD_TIMING_INFO g_Timing1920x1080_i_50_Format39 = {

    72000000, // Pixel clock in Hz
    2304,     // H. total pixels
    1920,     // H. active pixels
    1920,     // H. blank start from start of active in pixels
    2303,     // H. blank end from start of active in pixels
    1952,     // H. sync start from start of active in pixels
    2119,     // H. sync end from start of active in pixels
    31250,    // H. frequency in Hz
    1251,     // V. total lines
    1080,     // V. active lines
    1080,     // V. blank start from start of active in lines
    1250,     // V. blank end from start of active in lines
    1126,     // V. sync start from start of active in lines
    1135,     // V. sync end from start of active in lines
    50,       // Refresh rate
    1,        // Interlaced
    0,        // Timing flags -- H+, V-
    1,        // Timing flags -- H+, V-
};

// Format 40
DD_TIMING_INFO g_Timing1920x1080_i_100 = {

    148500000, // Pixel clock in Hz
    2640,      // H. total pixels
    1920,      // H. active pixels
    1920,      // H. blank start from start of active in pixels
    2639,      // H. blank end from start of active in pixels
    2448,      // H. sync start from start of active in pixels
    2491,      // H. sync end from start of active in pixels
    56250,     // H. frequency in Hz
    1125,      // V. total lines
    1080,      // V. active lines
    1080,      // V. blank start from start of active in lines
    1124,      // V. blank end from start of active in lines
    1084,      // V. sync start from start of active in lines
    1093,      // V. sync end from start of active in lines
    100,       // Refresh rate
    1,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 41 & 70
DD_TIMING_INFO g_Timing1280x720_100 = {

    148500000, // Pixel clock in Hz
    1980,      // H. total pixels
    1280,      // H. active pixels
    1280,      // H. blank start from start of active in pixels
    1979,      // H. blank end from start of active in pixels
    1720,      // H. sync start from start of active in pixels
    1759,      // H. sync end from start of active in pixels
    75000,     // H. frequency in Hz
    750,       // V. total lines
    720,       // V. active lines
    720,       // V. blank start from start of active in lines
    749,       // V. blank end from start of active in lines
    725,       // V. sync start from start of active in lines
    729,       // V. sync end from start of active in lines
    100,       // Refresh Rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 42 & 43
DD_TIMING_INFO g_Timing720x576_100 = {

    54000000, // Pixel clock in Hz
    864,      // H. total pixels
    720,      // H. active pixels
    720,      // H. blank start from start of active in pixels
    863,      // H. blank end from start of active in pixels
    732,      // H. sync start from start of active in pixels
    795,      // H. sync end from start of active in pixels
    62500,    // H. frequency in Hz
    625,      // V. total lines
    576,      // V. active lines
    576,      // V. blank start from start of active in lines
    624,      // V. blank end from start of active in lines
    581,      // V. sync start from start of active in lines
    585,      // V. sync end from start of active in lines
    100,      // Refresh rate
    0,        // Interlaced
    1,        // Timing flags -- H-, V-
    1,        // Timing flags -- H-, V-
};

// Formats 44 & 45
DD_TIMING_INFO g_Timing1440x576_i_100 = {

    54000000, // Pixel clock in Hz
    1728,     // H. total pixels
    1440,     // H. active pixels
    1440,     // H. blank start from start of active in pixels
    1727,     // H. blank end from start of active in pixels , as per the monitor timing doc
    1464,     // H. sync start from start of active in pixels
    1589,     // H. sync end from start of active in pixels
    31250,    // H. frequency in Hz
    625,      // V. total lines
    576,      // V. active lines
    576,      // V. blank start from start of active in lines
    624,      // V. blank end from start of active in lines
    580,      // V. sync start from start of active in lines
    585,      // V. sync end from start of active in lines
    100,      // Refresh rate
    1,        // Interlaced
    1,        // Timing flags -- H-, V-
    1,        // Timing flags -- H-, V-
};

// Format 46
DD_TIMING_INFO g_Timing1920x1080_i_120 = {

    148500000, // Pixel clock in Hz
    2200,      // H. total pixels
    1920,      // H. active pixels
    1920,      // H. blank start from start of active in pixels
    2199,      // H. blank end from start of active in pixels
    2008,      // H. sync start from start of active in pixels
    2051,      // H. sync end from start of active in pixels
    67500,     // H. frequency in Hz
    1125,      // V. total lines
    1080,      // V. active lines
    1080,      // V. blank start from start of active in lines
    1124,      // V. blank end from start of active in lines
    1084,      // V. sync start from start of active in lines
    1093,      // V. sync end from start of active in lines
    120,       // Refresh rate
    1,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 47 & 71
DD_TIMING_INFO g_Timing1280x720_120 = {

    148500000, // Pixel clock in Hz
    1650,      // H. total pixels
    1280,      // H. active pixels
    1280,      // H. blank start from start of active in pixels
    1649,      // H. blank end from start of active in pixels
    1390,      // H. sync start from start of active in pixels
    1429,      // H. sync end from start of active in pixels
    90000,     // H. frequency in Hz
    750,       // V. total lines
    720,       // V. active lines
    720,       // V. blank start from start of active in lines
    749,       // V. blank end from start of active in lines
    725,       // V. sync start from start of active in lines
    729,       // V. sync end from start of active in lines
    120,       // Refresh Rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Formats 48 & 49
DD_TIMING_INFO g_Timing720x480_120 = {

    54054000, // Pixel clock in Hz
    858,      // H. total pixels
    720,      // H. active pixels
    720,      // H. blank start from start of active in pixels
    857,      // H. blank end from start of active in pixels , as per the monitor timing doc
    736,      // H. sync start from start of active in pixels
    797,      // H. sync end from start of active in pixels
    63000,    // H. frequency in Hz
    525,      // V. total lines
    480,      // V. active lines
    480,      // V. blank start from start of active in lines
    524,      // V. blank end from start of active in lines
    489,      // V. sync start from start of active in lines
    494,      // V. sync end from start of active in lines
    120,      // Refresh rate
    0,        // Interlaced
    1,        // Timing flags -- H-, V-
    1,        // Timing flags -- H-, V-
};

// Formats 50 and 51
DD_TIMING_INFO g_Timing1440x480_i_120 = {

    54054000, // Pixel clock in Hz
    1716,     // H. total pixels
    1440,     // H. active pixels
    1440,     // H. blank start from start of active in pixels
    1715,     // H. blank end from start of active in pixels , as per the monitor timing doc
    1478,     // H. sync start from start of active in pixels
    1601,     // H. sync end from start of active in pixels
    31500,    // H. frequency in Hz
    525,      // V. total lines
    480,      // V. active lines
    480,      // V. blank start from start of active in lines
    524,      // V. blank end from start of active in lines
    488,      // V. sync start from start of active in lines
    493,      // V. sync end from start of active in lines
    120,      // Refresh rate
    1,        // Interlaced
    1,        // Timing flags -- H-, V-
    1,        // Timing flags -- H-, V-
};

// Format 52 &53
DD_TIMING_INFO g_Timing720x576_200 = {

    108000000, // Pixel clock in Hz
    864,       // H. total pixels
    720,       // H. active pixels
    720,       // H. blank start from start of active in pixels
    863,       // H. blank end from start of active in pixels
    732,       // H. sync start from start of active in pixels
    795,       // H. sync end from start of active in pixels
    12500,     // H. frequency in Hz
    625,       // V. total lines
    576,       // V. active lines
    576,       // V. blank start from start of active in lines
    624,       // V. blank end from start of active in lines
    581,       // V. sync start from start of active in lines
    585,       // V. sync end from start of active in lines
    200,       // Refresh rate
    0,         // Interlaced
    1,         // Timing flags -- H-, V-
    1,         // Timing flags -- H-, V-
};

// Formats 54 & 55
DD_TIMING_INFO g_Timing1440x576_i_200 = {

    108000000, // Pixel clock in Hz
    1728,      // H. total pixels
    1440,      // H. active pixels
    1440,      // H. blank start from start of active in pixels
    1727,      // H. blank end from start of active in pixels , as per the monitor timing doc
    1464,      // H. sync start from start of active in pixels
    1589,      // H. sync end from start of active in pixels
    62500,     // H. frequency in Hz
    625,       // V. total lines
    576,       // V. active lines
    576,       // V. blank start from start of active in lines
    624,       // V. blank end from start of active in lines
    580,       // V. sync start from start of active in lines
    585,       // V. sync end from start of active in lines
    200,       // Refresh rate
    1,         // Interlaced
    1,         // Timing flags -- H-, V-
    1,         // Timing flags -- H-, V-
};

// Formats 56 & 57
DD_TIMING_INFO g_Timing720x480_240 = {

    108108000, // Pixel clock in Hz
    858,       // H. total pixels
    720,       // H. active pixels
    720,       // H. blank start from start of active in pixels
    857,       // H. blank end from start of active in pixels , as per the monitor timing doc
    736,       // H. sync start from start of active in pixels
    797,       // H. sync end from start of active in pixels
    126000,    // H. frequency in Hz
    525,       // V. total lines
    480,       // V. active lines
    480,       // V. blank start from start of active in lines
    524,       // V. blank end from start of active in lines
    489,       // V. sync start from start of active in lines
    494,       // V. sync end from start of active in lines
    240,       // Refresh rate
    0,         // Interlaced
    1,         // Timing flags -- H-, V-
    1,         // Timing flags -- H-, V-
};

// Formats 58 and 59
DD_TIMING_INFO g_Timing1440x480_i_240 = {

    108108000, // Pixel clock in Hz
    1716,      // H. total pixels
    1440,      // H. active pixels
    1440,      // H. blank start from start of active in pixels
    1715,      // H. blank end from start of active in pixels , as per the monitor timing doc
    1478,      // H. sync start from start of active in pixels
    1601,      // H. sync end from start of active in pixels
    63000,     // H. frequency in Hz
    525,       // V. total lines
    480,       // V. active lines
    480,       // V. blank start from start of active in lines
    524,       // V. blank end from start of active in lines
    488,       // V. sync start from start of active in lines
    493,       // V. sync end from start of active in lines
    240,       // Refresh rate
    1,         // Interlaced
    1,         // Timing flags -- H-, V-
    1,         // Timing flags -- H-, V-
};

// Format 60 & 65
DD_TIMING_INFO g_Timing1280x720_p_24 = {

    59400000, // Pixel clock in Hz
    3300,     // H. total pixels
    1280,     // H. active pixels
    1280,     // H. blank start from start of active in pixels
    3299,     // H. blank end from start of active in pixels , as per the monitor timing doc
    3040,     // H. sync start from start of active in pixels
    3079,     // H. sync end from start of active in pixels
    18000,    // H. frequency in Hz
    750,      // V. total lines
    720,      // V. active lines
    720,      // V. blank start from start of active in lines
    749,      // V. blank end from start of active in lines
    725,      // V. sync start from start of active in lines
    729,      // V. sync end from start of active in lines
    24,       // Refresh rate
    0,        // Interlaced
    0,        // Timing flags -- H+, V+
    0,        // Timing flags -- H+, V+
};

// Format 61 & 66
DD_TIMING_INFO g_Timing1280x720_p_25 = {

    74250000, // Pixel clock in Hz
    3960,     // H. total pixels
    1280,     // H. active pixels
    1280,     // H. blank start from start of active in pixels
    3959,     // H. blank end from start of active in pixels , as per the monitor timing doc
    3700,     // H. sync start from start of active in pixels
    3739,     // H. sync end from start of active in pixels
    18750,    // H. frequency in Hz
    750,      // V. total lines
    720,      // V. active lines
    720,      // V. blank start from start of active in lines
    749,      // V. blank end from start of active in lines
    725,      // V. sync start from start of active in lines
    729,      // V. sync end from start of active in lines
    25,       // Refresh rate
    0,        // Interlaced
    0,        // Timing flags -- H+, V+
    0,        // Timing flags -- H+, V+
};

// Format 62 & 67
DD_TIMING_INFO g_Timing1280x720_p_30 = {

    74250000, // Pixel clock in Hz
    3300,     // H. total pixels
    1280,     // H. active pixels
    1280,     // H. blank start from start of active in pixels
    3299,     // H. blank end from start of active in pixels , as per the monitor timing doc
    3040,     // H. sync start from start of active in pixels
    3079,     // H. sync end from start of active in pixels
    22500,    // H. frequency in Hz
    750,      // V. total lines
    720,      // V. active lines
    720,      // V. blank start from start of active in lines
    749,      // V. blank end from start of active in lines
    725,      // V. sync start from start of active in lines
    729,      // V. sync end from start of active in lines
    30,       // Refresh rate
    0,        // Interlaced
    0,        // Timing flags -- H+, V+
    0,        // Timing flags -- H+, V+
};

// Format 63 & 78
DD_TIMING_INFO g_Timing1920x1080_p_120 = {

    297000000, // Pixel clock in Hz
    2200,      // H. total pixels
    1920,      // H. active pixels
    1920,      // H. blank start from start of active in pixels
    2199,      // H. blank end from start of active in pixels
    2008,      // H. sync start from start of active in pixels
    2051,      // H. sync end from start of active in pixels
    135000,    // H. frequency in Hz
    1125,      // V. total lines
    1080,      // V. active lines
    1080,      // V. blank start from start of active in lines
    1124,      // V. blank end from start of active in lines
    1084,      // V. sync start from start of active in lines
    1088,      // V. sync end from start of active in lines
    120,       // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 64 & 77
DD_TIMING_INFO g_Timing1920x1080_p_100 = {

    297000000, // Pixel clock in Hz
    2640,      // H. total pixels
    1920,      // H. active pixels
    1920,      // H. blank start from start of active in pixels
    2639,      // H. blank end from start of active in pixels
    2448,      // H. sync start from start of active in pixels
    2491,      // H. sync end from start of active in pixels
    112500,    // H. frequency in Hz
    1125,      // V. total lines
    1080,      // V. active lines
    1080,      // V. blank start from start of active in lines
    1124,      // V. blank end from start of active in lines
    1084,      // V. sync start from start of active in lines
    1088,      // V. sync end from start of active in lines
    100,       // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 79
DD_TIMING_INFO g_Timing1680x720_p_24 = {

    59400000, // Pixel clock in Hz
    3300,     // H. total pixels
    1680,     // H. active pixels
    1680,     // H. blank start from start of active in pixels
    3299,     // H. blank end from start of active in pixels
    3040,     // H. sync start from start of active in pixels
    3079,     // H. sync end from start of active in pixels
    18000,    // H. frequency in Hz
    750,      // V. total lines
    720,      // V. active lines
    720,      // V. blank start from start of active in lines
    749,      // V. blank end from start of active in lines
    725,      // V. sync start from start of active in lines
    729,      // V. sync end from start of active in lines
    24,       // Refresh rate
    0,        // Interlaced
    0,        // Timing flags -- H+, V+
    0,        // Timing flags -- H+, V+
};
// Format 80
DD_TIMING_INFO g_Timing1680x720_p_25 = {

    59400000, // Pixel clock in Hz
    3168,     // H. total pixels
    1680,     // H. active pixels
    1680,     // H. blank start from start of active in pixels
    3167,     // H. blank end from start of active in pixels
    2908,     // H. sync start from start of active in pixels
    2947,     // H. sync end from start of active in pixels
    18750,    // H. frequency in Hz
    750,      // V. total lines
    720,      // V. active lines
    720,      // V. blank start from start of active in lines
    749,      // V. blank end from start of active in lines
    725,      // V. sync start from start of active in lines
    729,      // V. sync end from start of active in lines
    25,       // Refresh rate
    0,        // Interlaced
    0,        // Timing flags -- H+, V+
    0,        // Timing flags -- H+, V+
};
// Format 81
DD_TIMING_INFO g_Timing1680x720_p_30 = {

    59400000, // Pixel clock in Hz
    2640,     // H. total pixels
    1680,     // H. active pixels
    1680,     // H. blank start from start of active in pixels
    2639,     // H. blank end from start of active in pixels
    2380,     // H. sync start from start of active in pixels
    2419,     // H. sync end from start of active in pixels
    22500,    // H. frequency in Hz
    750,      // V. total lines
    720,      // V. active lines
    720,      // V. blank start from start of active in lines
    749,      // V. blank end from start of active in lines
    725,      // V. sync start from start of active in lines
    729,      // V. sync end from start of active in lines
    30,       // Refresh rate
    0,        // Interlaced
    0,        // Timing flags -- H+, V+
    0,        // Timing flags -- H+, V+
};
// Format 82
DD_TIMING_INFO g_Timing1680x720_p_50 = {

    82500000, // Pixel clock in Hz
    2200,     // H. total pixels
    1680,     // H. active pixels
    1680,     // H. blank start from start of active in pixels
    2199,     // H. blank end from start of active in pixels
    1940,     // H. sync start from start of active in pixels
    1979,     // H. sync end from start of active in pixels
    37500,    // H. frequency in Hz
    750,      // V. total lines
    720,      // V. active lines
    720,      // V. blank start from start of active in lines
    749,      // V. blank end from start of active in lines
    725,      // V. sync start from start of active in lines
    729,      // V. sync end from start of active in lines
    50,       // Refresh rate
    0,        // Interlaced
    0,        // Timing flags -- H+, V+
    0,        // Timing flags -- H+, V+
};
// Format 83
DD_TIMING_INFO g_Timing1680x720_p_60 = {

    99000000, // Pixel clock in Hz
    2200,     // H. total pixels
    1680,     // H. active pixels
    1680,     // H. blank start from start of active in pixels
    2199,     // H. blank end from start of active in pixels
    1940,     // H. sync start from start of active in pixels
    1979,     // H. sync end from start of active in pixels
    45000,    // H. frequency in Hz
    750,      // V. total lines
    720,      // V. active lines
    720,      // V. blank start from start of active in lines
    749,      // V. blank end from start of active in lines
    725,      // V. sync start from start of active in lines
    729,      // V. sync end from start of active in lines
    60,       // Refresh rate
    0,        // Interlaced
    0,        // Timing flags -- H+, V+
    0,        // Timing flags -- H+, V+
};

// Format 84
DD_TIMING_INFO g_Timing1680x720_p_100 = {

    165000000, // Pixel clock in Hz
    2000,      // H. total pixels
    1680,      // H. active pixels
    1680,      // H. blank start from start of active in pixels
    1999,      // H. blank end from start of active in pixels
    1740,      // H. sync start from start of active in pixels
    1779,      // H. sync end from start of active in pixels
    82500,     // H. frequency in Hz
    825,       // V. total lines
    720,       // V. active lines
    720,       // V. blank start from start of active in lines
    824,       // V. blank end from start of active in lines
    725,       // V. sync start from start of active in lines
    729,       // V. sync end from start of active in lines
    100,       // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 85
DD_TIMING_INFO g_Timing1680x720_p_120 = {

    198000000, // Pixel clock in Hz
    2000,      // H. total pixels
    1680,      // H. active pixels
    1680,      // H. blank start from start of active in pixels
    1999,      // H. blank end from start of active in pixels
    1740,      // H. sync start from start of active in pixels
    1779,      // H. sync end from start of active in pixels
    99000,     // H. frequency in Hz
    825,       // V. total lines
    720,       // V. active lines
    720,       // V. blank start from start of active in lines
    824,       // V. blank end from start of active in lines
    725,       // V. sync start from start of active in lines
    729,       // V. sync end from start of active in lines
    120,       // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 86
DD_TIMING_INFO g_Timing2560x1080_p_24 = {

    99000000, // Pixel clock in Hz
    3750,     // H. total pixels
    2560,     // H. active pixels
    2560,     // H. blank start from start of active in pixels
    3749,     // H. blank end from start of active in pixels
    3558,     // H. sync start from start of active in pixels
    3601,     // H. sync end from start of active in pixels
    26400,    // H. frequency in Hz
    1100,     // V. total lines
    1080,     // V. active lines
    1080,     // V. blank start from start of active in lines
    1099,     // V. blank end from start of active in lines
    1084,     // V. sync start from start of active in lines
    1088,     // V. sync end from start of active in lines
    24,       // Refresh rate
    0,        // Interlaced
    0,        // Timing flags -- H+, V+
    0,        // Timing flags -- H+, V+
};

// Format 87
DD_TIMING_INFO g_Timing2560x1080_p_25 = {

    90000000, // Pixel clock in Hz
    3200,     // H. total pixels
    2560,     // H. active pixels
    2560,     // H. blank start from start of active in pixels
    3199,     // H. blank end from start of active in pixels
    3008,     // H. sync start from start of active in pixels
    3051,     // H. sync end from start of active in pixels
    28125,    // H. frequency in Hz
    1125,     // V. total lines
    1080,     // V. active lines
    1080,     // V. blank start from start of active in lines
    1124,     // V. blank end from start of active in lines
    1084,     // V. sync start from start of active in lines
    1088,     // V. sync end from start of active in lines
    25,       // Refresh rate
    0,        // Interlaced
    0,        // Timing flags -- H+, V+
    0,        // Timing flags -- H+, V+
};

// Format 88
DD_TIMING_INFO g_Timing2560x1080_p_30 = {

    118800000, // Pixel clock in Hz
    3520,      // H. total pixels
    2560,      // H. active pixels
    2560,      // H. blank start from start of active in pixels
    3519,      // H. blank end from start of active in pixels
    3328,      // H. sync start from start of active in pixels
    3371,      // H. sync end from start of active in pixels
    33750,     // H. frequency in Hz
    1125,      // V. total lines
    1080,      // V. active lines
    1080,      // V. blank start from start of active in lines
    1124,      // V. blank end from start of active in lines
    1084,      // V. sync start from start of active in lines
    1088,      // V. sync end from start of active in lines
    30,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 89
DD_TIMING_INFO g_Timing2560x1080_p_50 = {

    185625000, // Pixel clock in Hz
    3300,      // H. total pixels
    2560,      // H. active pixels
    2560,      // H. blank start from start of active in pixels
    3299,      // H. blank end from start of active in pixels
    3108,      // H. sync start from start of active in pixels
    3151,      // H. sync end from start of active in pixels
    56250,     // H. frequency in Hz
    1125,      // V. total lines
    1080,      // V. active lines
    1080,      // V. blank start from start of active in lines
    1124,      // V. blank end from start of active in lines
    1084,      // V. sync start from start of active in lines
    1088,      // V. sync end from start of active in lines
    50,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 90
DD_TIMING_INFO g_Timing2560x1080_p_60 = {

    198000000, // Pixel clock in Hz
    3000,      // H. total pixels
    2560,      // H. active pixels
    2560,      // H. blank start from start of active in pixels
    2999,      // H. blank end from start of active in pixels
    2808,      // H. sync start from start of active in pixels
    2851,      // H. sync end from start of active in pixels
    66000,     // H. frequency in Hz
    1100,      // V. total lines
    1080,      // V. active lines
    1080,      // V. blank start from start of active in lines
    1099,      // V. blank end from start of active in lines
    1084,      // V. sync start from start of active in lines
    1088,      // V. sync end from start of active in lines
    60,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 91
DD_TIMING_INFO g_Timing2560x1080_p_100 = {

    371250000, // Pixel clock in Hz
    2970,      // H. total pixels
    2560,      // H. active pixels
    2560,      // H. blank start from start of active in pixels
    2969,      // H. blank end from start of active in pixels
    2778,      // H. sync start from start of active in pixels
    2821,      // H. sync end from start of active in pixels
    125000,    // H. frequency in Hz
    1250,      // V. total lines
    1080,      // V. active lines
    1080,      // V. blank start from start of active in lines
    1249,      // V. blank end from start of active in lines
    1084,      // V. sync start from start of active in lines
    1088,      // V. sync end from start of active in lines
    100,       // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 92
DD_TIMING_INFO g_Timing2560x1080_p_120 = {

    495000000, // Pixel clock in Hz
    3300,      // H. total pixels
    2560,      // H. active pixels
    2560,      // H. blank start from start of active in pixels
    3299,      // H. blank end from start of active in pixels
    3108,      // H. sync start from start of active in pixels
    3151,      // H. sync end from start of active in pixels
    150000,    // H. frequency in Hz
    1250,      // V. total lines
    1080,      // V. active lines
    1080,      // V. blank start from start of active in lines
    1249,      // V. blank end from start of active in lines
    1084,      // V. sync start from start of active in lines
    1088,      // V. sync end from start of active in lines
    120,       // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

/*
// Format 93 , 103
DD_TIMING_INFO  g_Timing3840x2160_p_24 = {
sizeof(DD_TIMING_INFO),    // Size of the DD_TIMING_INFO structure
297000000,                // Pixel clock in Hz
5500,                    // H. total pixels
3840,                    // H. active pixels
3840,                    // H. blank start from start of active in pixels
5499,                    // H. blank end from start of active in pixels
2448,                    // H. sync start from start of active in pixels
2491,                    // H. sync end from start of active in pixels
54000,                    // H. frequency in Hz
2250,                    // V. total lines
2160,                    // V. active lines
2160,                    // V. blank start from start of active in lines
2249,                    // V. blank end from start of active in lines
1084,                    // V. sync start from start of active in lines
1088,                    // V. sync end from start of active in lines
24,                    // Refresh rate
0,                      // Interlaced
0,                      // Timing flags -- H+, V+
0,                      // Timing flags -- H+, V+
};

// Format 94 , 104
DD_TIMING_INFO  g_Timing3840x2160_p_25 = {
sizeof(DD_TIMING_INFO),    // Size of the DD_TIMING_INFO structure
297000000,                // Pixel clock in Hz
5280,                    // H. total pixels
3840,                    // H. active pixels
3840,                    // H. blank start from start of active in pixels
5279,                    // H. blank end from start of active in pixels
2448,                    // H. sync start from start of active in pixels
2491,                    // H. sync end from start of active in pixels
56250,                    // H. frequency in Hz
2250,                    // V. total lines
2160,                    // V. active lines
2160,                    // V. blank start from start of active in lines
2249,                    // V. blank end from start of active in lines
1084,                    // V. sync start from start of active in lines
1088,                    // V. sync end from start of active in lines
25,                    // Refresh rate
0,                      // Interlaced
0,                      // Timing flags -- H+, V+
0,                      // Timing flags -- H+, V+
};

// Format 95 , 105
DD_TIMING_INFO  g_Timing3840x2160_p_30 = {
sizeof(DD_TIMING_INFO),    // Size of the DD_TIMING_INFO structure
297000000,                // Pixel clock in Hz
4400,                    // H. total pixels
3840,                    // H. active pixels
3840,                    // H. blank start from start of active in pixels
4399,                    // H. blank end from start of active in pixels
2448,                    // H. sync start from start of active in pixels
2491,                    // H. sync end from start of active in pixels
67500,                    // H. frequency in Hz
2250,                    // V. total lines
2160,                    // V. active lines
2160,                    // V. blank start from start of active in lines
2249,                    // V. blank end from start of active in lines
1084,                    // V. sync start from start of active in lines
1088,                    // V. sync end from start of active in lines
30,                    // Refresh rate
0,                      // Interlaced
0,                      // Timing flags -- H+, V+
0,                      // Timing flags -- H+, V+
};*/

// Format 96,106 <MIGHT CONTAIN ERROR, REVIEW THE DD_TIMING_INFO>
DD_TIMING_INFO g_Timing3840x2160_p_50 = {

    594000000, // Pixel clock in Hz
    5280,      // H. total pixels
    3840,      // H. active pixels
    3840,      // H. blank start from start of active in pixels
    5279,      // H. blank end from start of active in pixels
    4896,      // H. sync start from start of active in pixels
    4983,      // H. sync end from start of active in pixels
    112500,    // H. frequency in Hz
    2250,      // V. total lines
    2160,      // V. active lines
    2160,      // V. blank start from start of active in lines
    2249,      // V. blank end from start of active in lines
    2168,      // V. sync start from start of active in lines
    2177,      // V. sync end from start of active in lines
    50,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 97,107 <MIGHT CONTAIN ERROR, REVIEW THE DD_TIMING_INFO>
DD_TIMING_INFO g_Timing3840x2160_p_60 = {

    594000000, // Pixel clock in Hz
    4400,      // H. total pixels
    3840,      // H. active pixels
    3840,      // H. blank start from start of active in pixels
    4399,      // H. blank end from start of active in pixels
    4016,      // H. sync start from start of active in pixels
    4103,      // H. sync end from start of active in pixels
    135000,    // H. frequency in Hz
    2250,      // V. total lines
    2160,      // V. active lines
    2160,      // V. blank start from start of active in lines
    2249,      // V. blank end from start of active in lines
    2168,      // V. sync start from start of active in lines
    2177,      // V. sync end from start of active in lines
    60,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};
/*
// Format 98 - This is not required as we use this timing from HF-VSDB definitions
DD_TIMING_INFO  g_Timing4096x2160_p_24 = {
sizeof(DD_TIMING_INFO),    // Size of the DD_TIMING_INFO structure
297000000,                // Pixel clock in Hz
4400,                    // H. total pixels
3840,                    // H. active pixels
3840,                    // H. blank start from start of active in pixels
4399,                    // H. blank end from start of active in pixels
2448,                    // H. sync start from start of active in pixels
2491,                    // H. sync end from start of active in pixels
67500,                    // H. frequency in Hz
2250,                    // V. total lines
2160,                    // V. active lines
2160,                    // V. blank start from start of active in lines
2249,                    // V. blank end from start of active in lines
1084,                    // V. sync start from start of active in lines
1088,                    // V. sync end from start of active in lines
30,                    // Refresh rate
0,                      // Interlaced
0,                      // Timing flags -- H+, V+
0,                      // Timing flags -- H+, V+
};*/

// Format 99
DD_TIMING_INFO g_Timing4096x2160_p_25 = {

    297000000, // Pixel clock in Hz
    5280,      // H. total pixels
    4096,      // H. active pixels
    4096,      // H. blank start from start of active in pixels
    5279,      // H. blank end from start of active in pixels
    5064,      // H. sync start from start of active in pixels
    5151,      // H. sync end from start of active in pixels
    56260,     // H. frequency in Hz
    2250,      // V. total lines
    2160,      // V. active lines
    2160,      // V. blank start from start of active in lines
    2249,      // V. blank end from start of active in lines
    2168,      // V. sync start from start of active in lines
    2177,      // V. sync end from start of active in lines
    25,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 100
DD_TIMING_INFO g_Timing4096x2160_p_30 = {

    297000000, // Pixel clock in Hz
    4400,      // H. total pixels
    4096,      // H. active pixels
    4096,      // H. blank start from start of active in pixels
    4399,      // H. blank end from start of active in pixels
    4184,      // H. sync start from start of active in pixels
    4271,      // H. sync end from start of active in pixels
    67500,     // H. frequency in Hz
    2250,      // V. total lines
    2160,      // V. active lines
    2160,      // V. blank start from start of active in lines
    2249,      // V. blank end from start of active in lines
    2168,      // V. sync start from start of active in lines
    2177,      // V. sync end from start of active in lines
    30,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 101 <MIGHT CONTAIN ERROR, REVIEW THE DD_TIMING_INFO>
DD_TIMING_INFO g_Timing4096x2160_p_50 = {

    594000000, // Pixel clock in Hz
    5280,      // H. total pixels
    4096,      // H. active pixels
    4096,      // H. blank start from start of active in pixels
    5279,      // H. blank end from start of active in pixels
    5064,      // H. sync start from start of active in pixels
    5151,      // H. sync end from start of active in pixels
    112500,    // H. frequency in Hz
    2250,      // V. total lines
    2160,      // V. active lines
    2160,      // V. blank start from start of active in lines
    2249,      // V. blank end from start of active in lines
    2168,      // V. sync start from start of active in lines
    2177,      // V. sync end from start of active in lines
    50,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 102 <MIGHT CONTAIN ERROR, REVIEW THE DD_TIMING_INFO>
DD_TIMING_INFO g_Timing4096x2160_p_60 = {

    594000000, // Pixel clock in Hz
    4400,      // H. total pixels
    4096,      // H. active pixels
    4096,      // H. blank start from start of active in pixels
    4399,      // H. blank end from start of active in pixels
    4184,      // H. sync start from start of active in pixels
    4271,      // H. sync end from start of active in pixels
    135000,    // H. frequency in Hz
    2250,      // V. total lines
    2160,      // V. active lines
    2160,      // V. blank start from start of active in lines
    2249,      // V. blank end from start of active in lines
    2168,      // V. sync start from start of active in lines
    2177,      // V. sync end from start of active in lines
    60,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// CE-861G New Supported Modes Starts Here
// Format 108 and 109 1280x720x48
DD_TIMING_INFO g_Timing1280x720_p_48 = {
    90000000, // Pixel clock in Hz
    2500,     // H. total pixels
    1280,     // H. active pixels
    1280,     // H. blank start from start of active in pixels
    2499,     // H. blank end from start of active in pixels
    2240,     // H. sync start from start of active in pixels
    2279,     // H. sync end from start of active in pixels
    36000,    // H. frequency in Hz
    750,      // V. total lines
    720,      // V. active lines
    720,      // V. blank start from start of active in lines
    749,      // V. blank end from start of active in lines
    725,      // V. sync start from start of active in lines
    729,      // V. sync end from start of active in lines
    48,       // Refresh Rate
    0,        // Interlaced
    0,        // Timing flags -- H+, V+
    0,        // Timing flags -- H+, V+
};

// Format 110
DD_TIMING_INFO g_Timing1680x720_p_48 = {
    99000000, // Pixel clock in Hz
    2750,     // H. total pixels
    1680,     // H. active pixels
    1680,     // H. blank start from start of active in pixels
    2749,     // H. blank end from start of active in pixels
    2490,     // H. sync start from start of active in pixels
    2529,     // H. sync end from start of active in pixels
    36000,    // H. frequency in Hz
    750,      // V. total lines
    720,      // V. active lines
    720,      // V. blank start from start of active in lines
    749,      // V. blank end from start of active in lines
    725,      // V. sync start from start of active in lines
    729,      // V. sync end from start of active in lines
    48,       // Refresh rate
    0,        // Interlaced
    0,        // Timing flags -- H+, V+
    0,        // Timing flags -- H+, V+
};

// Format 111 and 112
DD_TIMING_INFO g_Timing1920x1080_p_48 = {
    148500000, // Pixel clock in Hz
    2750,      // H. total pixels
    1920,      // H. active pixels
    1920,      // H. blank start from start of active in pixels
    2749,      // H. blank end from start of active in pixels
    2558,      // H. sync start from start of active in pixels
    2601,      // H. sync end from start of active in pixels
    58000,     // H. frequency in Hz
    1125,      // V. total lines
    1080,      // V. active lines
    1080,      // V. blank start from start of active in lines
    1124,      // V. blank end from start of active in lines
    1084,      // V. sync start from start of active in lines
    1088,      // V. sync end from start of active in lines
    48,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 113
DD_TIMING_INFO g_Timing2560x1080_p_48 = {
    198000000, // Pixel clock in Hz
    3750,      // H. total pixels
    2560,      // H. active pixels
    2560,      // H. blank start from start of active in pixels
    3749,      // H. blank end from start of active in pixels
    3558,      // H. sync start from start of active in pixels
    3601,      // H. sync end from start of active in pixels
    52800,     // H. frequency in Hz
    1100,      // V. total lines
    1080,      // V. active lines
    1080,      // V. blank start from start of active in lines
    1099,      // V. blank end from start of active in lines
    1084,      // V. sync start from start of active in lines
    1088,      // V. sync end from start of active in lines
    48,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 114 and 116
DD_TIMING_INFO g_Timing3840x2160_p_48 = {
    594000000, // Pixel clock in Hz
    5500,      // H. total pixels
    3840,      // H. active pixels
    3840,      // H. blank start from start of active in pixels
    5499,      // H. blank end from start of active in pixels
    5116,      // H. sync start from start of active in pixels
    5203,      // H. sync end from start of active in pixels
    108000,    // H. frequency in Hz
    2250,      // V. total lines
    2160,      // V. active lines
    2160,      // V. blank start from start of active in lines
    2249,      // V. blank end from start of active in lines
    2168,      // V. sync start from start of active in lines
    2177,      // V. sync end from start of active in lines
    48,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 115
DD_TIMING_INFO g_Timing4096x2160_p_48 = {
    594000000, // Pixel clock in Hz
    5500,      // H. total pixels
    4096,      // H. active pixels
    4096,      // H. blank start from start of active in pixels
    5499,      // H. blank end from start of active in pixels
    5116,      // H. sync start from start of active in pixels
    5203,      // H. sync end from start of active in pixels
    108000,    // H. frequency in Hz
    2250,      // V. total lines
    2160,      // V. active lines
    2160,      // V. blank start from start of active in lines
    2249,      // V. blank end from start of active in lines
    2168,      // V. sync start from start of active in lines
    2177,      // V. sync end from start of active in lines
    48,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 117 & 119
DD_TIMING_INFO g_Timing3840x2160_p_100 = {
    1188000000, // Pixel clock in Hz
    5280,       // H. total pixels
    3840,       // H. active pixels
    3840,       // H. blank start from start of active in pixels
    5279,       // H. blank end from start of active in pixels
    4896,       // H. sync start from start of active in pixels
    4983,       // H. sync end from start of active in pixels
    225000,     // H. frequency in Hz
    2250,       // V. total lines
    2160,       // V. active lines
    2160,       // V. blank start from start of active in lines
    2249,       // V. blank end from start of active in lines
    2168,       // V. sync start from start of active in lines
    2177,       // V. sync end from start of active in lines
    100,        // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 118 & 120
DD_TIMING_INFO g_Timing3840x2160_p_120 = {
    1188000000, // Pixel clock in Hz
    4400,       // H. total pixels
    3840,       // H. active pixels
    3840,       // H. blank start from start of active in pixels
    4399,       // H. blank end from start of active in pixels
    4016,       // H. sync start from start of active in pixels
    4103,       // H. sync end from start of active in pixels
    270000,     // H. frequency in Hz
    2250,       // V. total lines
    2160,       // V. active lines
    2160,       // V. blank start from start of active in lines
    2249,       // V. blank end from start of active in lines
    2168,       // V. sync start from start of active in lines
    2177,       // V. sync end from start of active in lines
    120,        // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 121
DD_TIMING_INFO g_Timing5120x2160_p_24 = {
    396000000, // Pixel clock in Hz
    7500,      // H. total pixels
    5120,      // H. active pixels
    5120,      // H. blank start from start of active in pixels
    7499,      // H. blank end from start of active in pixels
    7116,      // H. sync start from start of active in pixels
    7203,      // H. sync end from start of active in pixels
    52800,     // H. frequency in Hz
    2200,      // V. total lines
    2160,      // V. active lines
    2160,      // V. blank start from start of active in lines
    2199,      // V. blank end from start of active in lines
    2168,      // V. sync start from start of active in lines
    2177,      // V. sync end from start of active in lines
    24,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 122
DD_TIMING_INFO g_Timing5120x2160_p_25 = {
    396000000, // Pixel clock in Hz
    7200,      // H. total pixels
    5120,      // H. active pixels
    5120,      // H. blank start from start of active in pixels
    7199,      // H. blank end from start of active in pixels
    6816,      // H. sync start from start of active in pixels
    6903,      // H. sync end from start of active in pixels
    55000,     // H. frequency in Hz
    2200,      // V. total lines
    2160,      // V. active lines
    2160,      // V. blank start from start of active in lines
    2199,      // V. blank end from start of active in lines
    2168,      // V. sync start from start of active in lines
    2177,      // V. sync end from start of active in lines
    25,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 123
DD_TIMING_INFO g_Timing5120x2160_p_30 = {
    396000000, // Pixel clock in Hz
    6000,      // H. total pixels
    5120,      // H. active pixels
    5120,      // H. blank start from start of active in pixels
    5999,      // H. blank end from start of active in pixels
    5784,      // H. sync start from start of active in pixels
    5871,      // H. sync end from start of active in pixels
    66000,     // H. frequency in Hz
    2200,      // V. total lines
    2160,      // V. active lines
    2160,      // V. blank start from start of active in lines
    2199,      // V. blank end from start of active in lines
    2168,      // V. sync start from start of active in lines
    2177,      // V. sync end from start of active in lines
    30,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 124
DD_TIMING_INFO g_Timing5120x2160_p_48 = {
    742500000, // Pixel clock in Hz
    6250,      // H. total pixels
    5120,      // H. active pixels
    5120,      // H. blank start from start of active in pixels
    6249,      // H. blank end from start of active in pixels
    5866,      // H. sync start from start of active in pixels
    5953,      // H. sync end from start of active in pixels
    118800,    // H. frequency in Hz
    2475,      // V. total lines
    2160,      // V. active lines
    2160,      // V. blank start from start of active in lines
    2474,      // V. blank end from start of active in lines
    2168,      // V. sync start from start of active in lines
    2177,      // V. sync end from start of active in lines
    48,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 125
DD_TIMING_INFO g_Timing5120x2160_p_50 = {
    742500000, // Pixel clock in Hz
    6600,      // H. total pixels
    5120,      // H. active pixels
    5120,      // H. blank start from start of active in pixels
    6599,      // H. blank end from start of active in pixels
    6216,      // H. sync start from start of active in pixels
    6303,      // H. sync end from start of active in pixels
    112500,    // H. frequency in Hz
    2250,      // V. total lines
    2160,      // V. active lines
    2160,      // V. blank start from start of active in lines
    2249,      // V. blank end from start of active in lines
    2168,      // V. sync start from start of active in lines
    2177,      // V. sync end from start of active in lines
    50,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 126
DD_TIMING_INFO g_Timing5120x2160_p_60 = {
    742500000, // Pixel clock in Hz
    5500,      // H. total pixels
    5120,      // H. active pixels
    5120,      // H. blank start from start of active in pixels
    5499,      // H. blank end from start of active in pixels
    5284,      // H. sync start from start of active in pixels
    5371,      // H. sync end from start of active in pixels
    135000,    // H. frequency in Hz
    2250,      // V. total lines
    2160,      // V. active lines
    2160,      // V. blank start from start of active in lines
    2249,      // V. blank end from start of active in lines
    2168,      // V. sync start from start of active in lines
    2177,      // V. sync end from start of active in lines
    60,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// Format 127
DD_TIMING_INFO g_Timing5120x2160_p_100 = {
    1485000000, // Pixel clock in Hz
    6600,       // H. total pixels
    5120,       // H. active pixels
    5120,       // H. blank start from start of active in pixels
    6599,       // H. blank end from start of active in pixels
    6216,       // H. sync start from start of active in pixels
    6303,       // H. sync end from start of active in pixels
    225000,     // H. frequency in Hz
    2250,       // V. total lines
    2160,       // V. active lines
    2160,       // V. blank start from start of active in lines
    2249,       // V. blank end from start of active in lines
    2168,       // V. sync start from start of active in lines
    2177,       // V. sync end from start of active in lines
    100,        // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Formats 128-192 are Forbidden as per CE 861-G standard

// Format 193
DD_TIMING_INFO g_Timing5120x2160_p_120 = {
    1485000000, // Pixel clock in Hz
    5500,       // H. total pixels
    5120,       // H. active pixels
    5120,       // H. blank start from start of active in pixels
    5499,       // H. blank end from start of active in pixels
    5284,       // H. sync start from start of active in pixels
    5371,       // H. sync end from start of active in pixels
    270000,     // H. frequency in Hz
    2250,       // V. total lines
    2160,       // V. active lines
    2160,       // V. blank start from start of active in lines
    2249,       // V. blank end from start of active in lines
    2168,       // V. sync start from start of active in lines
    2177,       // V. sync end from start of active in lines
    120,        // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 194 and 202
DD_TIMING_INFO g_Timing7680x4320_p_24 = {
    1188000000, // Pixel clock in Hz
    11000,      // H. total pixels
    7680,       // H. active pixels
    7680,       // H. blank start from start of active in pixels
    10999,      // H. blank end from start of active in pixels
    10232,      // H. sync start from start of active in pixels
    10407,      // H. sync end from start of active in pixels
    108000,     // H. frequency in Hz
    4500,       // V. total lines
    4320,       // V. active lines
    4320,       // V. blank start from start of active in lines
    4499,       // V. blank end from start of active in lines
    4336,       // V. sync start from start of active in lines
    4355,       // V. sync end from start of active in lines
    24,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 195 and 203
DD_TIMING_INFO g_Timing7680x4320_p_25 = {
    1188000000, // Pixel clock in Hz
    10800,      // H. total pixels
    7680,       // H. active pixels
    7680,       // H. blank start from start of active in pixels
    10799,      // H. blank end from start of active in pixels
    10032,      // H. sync start from start of active in pixels
    10207,      // H. sync end from start of active in pixels
    110000,     // H. frequency in Hz
    4400,       // V. total lines
    4320,       // V. active lines
    4320,       // V. blank start from start of active in lines
    4399,       // V. blank end from start of active in lines
    4336,       // V. sync start from start of active in lines
    4355,       // V. sync end from start of active in lines
    25,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 196 and 204
DD_TIMING_INFO g_Timing7680x4320_p_30 = {
    1188000000, // Pixel clock in Hz
    9000,       // H. total pixels
    7680,       // H. active pixels
    7680,       // H. blank start from start of active in pixels
    8999,       // H. blank end from start of active in pixels
    8232,       // H. sync start from start of active in pixels
    8407,       // H. sync end from start of active in pixels
    132000,     // H. frequency in Hz
    4400,       // V. total lines
    4320,       // V. active lines
    4320,       // V. blank start from start of active in lines
    4399,       // V. blank end from start of active in lines
    4336,       // V. sync start from start of active in lines
    4355,       // V. sync end from start of active in lines
    30,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 197 and 205
DD_TIMING_INFO g_Timing7680x4320_p_48 = {
    2376000000, // Pixel clock in Hz
    11000,      // H. total pixels
    7680,       // H. active pixels
    7680,       // H. blank start from start of active in pixels
    10999,      // H. blank end from start of active in pixels
    10232,      // H. sync start from start of active in pixels
    10407,      // H. sync end from start of active in pixels
    216000,     // H. frequency in Hz
    4500,       // V. total lines
    4320,       // V. active lines
    4320,       // V. blank start from start of active in lines
    4499,       // V. blank end from start of active in lines
    4336,       // V. sync start from start of active in lines
    4355,       // V. sync end from start of active in lines
    48,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 198 and 206
DD_TIMING_INFO g_Timing7680x4320_p_50 = {
    2376000000, // Pixel clock in Hz
    10800,      // H. total pixels
    7680,       // H. active pixels
    7680,       // H. blank start from start of active in pixels
    10799,      // H. blank end from start of active in pixels
    10032,      // H. sync start from start of active in pixels
    10207,      // H. sync end from start of active in pixels
    220000,     // H. frequency in Hz
    4400,       // V. total lines
    4320,       // V. active lines
    4320,       // V. blank start from start of active in lines
    4399,       // V. blank end from start of active in lines
    4336,       // V. sync start from start of active in lines
    4355,       // V. sync end from start of active in lines
    50,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 199 and 207
DD_TIMING_INFO g_Timing7680x4320_p_60 = {
    2376000000, // Pixel clock in Hz
    9000,       // H. total pixels
    7680,       // H. active pixels
    7680,       // H. blank start from start of active in pixels
    8999,       // H. blank end from start of active in pixels
    8232,       // H. sync start from start of active in pixels
    8407,       // H. sync end from start of active in pixels
    264000,     // H. frequency in Hz
    4400,       // V. total lines
    4320,       // V. active lines
    4320,       // V. blank start from start of active in lines
    4399,       // V. blank end from start of active in lines
    4336,       // V. sync start from start of active in lines
    4355,       // V. sync end from start of active in lines
    60,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 210
DD_TIMING_INFO g_Timing10240x4320_p_24 = {
    1485000000, // Pixel clock in Hz
    12500,      // H. total pixels
    10240,      // H. active pixels
    10240,      // H. blank start from start of active in pixels
    12499,      // H. blank end from start of active in pixels
    11732,      // H. sync start from start of active in pixels
    11907,      // H. sync end from start of active in pixels
    118800,     // H. frequency in Hz
    4950,       // V. total lines
    4320,       // V. active lines
    4320,       // V. blank start from start of active in lines
    4949,       // V. blank end from start of active in lines
    4336,       // V. sync start from start of active in lines
    4355,       // V. sync end from start of active in lines
    24,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 211
DD_TIMING_INFO g_Timing10240x4320_p_25 = {
    1485000000, // Pixel clock in Hz
    13500,      // H. total pixels
    10240,      // H. active pixels
    10240,      // H. blank start from start of active in pixels
    13499,      // H. blank end from start of active in pixels
    12732,      // H. sync start from start of active in pixels
    12907,      // H. sync end from start of active in pixels
    110000,     // H. frequency in Hz
    4400,       // V. total lines
    4320,       // V. active lines
    4320,       // V. blank start from start of active in lines
    4399,       // V. blank end from start of active in lines
    4336,       // V. sync start from start of active in lines
    4355,       // V. sync end from start of active in lines
    25,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 212
DD_TIMING_INFO g_Timing10240x4320_p_30 = {
    1485000000, // Pixel clock in Hz
    11000,      // H. total pixels
    10240,      // H. active pixels
    10240,      // H. blank start from start of active in pixels
    10999,      // H. blank end from start of active in pixels
    10528,      // H. sync start from start of active in pixels
    10703,      // H. sync end from start of active in pixels
    135000,     // H. frequency in Hz
    4500,       // V. total lines
    4320,       // V. active lines
    4320,       // V. blank start from start of active in lines
    4499,       // V. blank end from start of active in lines
    4336,       // V. sync start from start of active in lines
    4355,       // V. sync end from start of active in lines
    30,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 213
DD_TIMING_INFO g_Timing10240x4320_p_48 = {
    2970000000, // Pixel clock in Hz
    12500,      // H. total pixels
    10240,      // H. active pixels
    10240,      // H. blank start from start of active in pixels
    12499,      // H. blank end from start of active in pixels
    11732,      // H. sync start from start of active in pixels
    11907,      // H. sync end from start of active in pixels
    237600,     // H. frequency in Hz
    4950,       // V. total lines
    4320,       // V. active lines
    4320,       // V. blank start from start of active in lines
    4949,       // V. blank end from start of active in lines
    4336,       // V. sync start from start of active in lines
    4355,       // V. sync end from start of active in lines
    48,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 214
DD_TIMING_INFO g_Timing10240x4320_p_50 = {
    2970000000, // Pixel clock in Hz
    13500,      // H. total pixels
    10240,      // H. active pixels
    10240,      // H. blank start from start of active in pixels
    13499,      // H. blank end from start of active in pixels
    12732,      // H. sync start from start of active in pixels
    12907,      // H. sync end from start of active in pixels
    220000,     // H. frequency in Hz
    4400,       // V. total lines
    4320,       // V. active lines
    4320,       // V. blank start from start of active in lines
    4399,       // V. blank end from start of active in lines
    4336,       // V. sync start from start of active in lines
    4355,       // V. sync end from start of active in lines
    50,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 215
DD_TIMING_INFO g_Timing10240x4320_p_60 = {
    2970000000, // Pixel clock in Hz
    11000,      // H. total pixels
    10240,      // H. active pixels
    10240,      // H. blank start from start of active in pixels
    10999,      // H. blank end from start of active in pixels
    10528,      // H. sync start from start of active in pixels
    10703,      // H. sync end from start of active in pixels
    270000,     // H. frequency in Hz
    4500,       // V. total lines
    4320,       // V. active lines
    4320,       // V. blank start from start of active in lines
    4499,       // V. blank end from start of active in lines
    4336,       // V. sync start from start of active in lines
    4355,       // V. sync end from start of active in lines
    60,         // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 200 and 208
DD_TIMING_INFO g_Timing7680x4320_p_100 = {
    4752000000, // Pixel clock in Hz
    10560,      // H. total pixels
    7680,       // H. active pixels
    7680,       // H. blank start from start of active in pixels
    10559,      // H. blank end from start of active in pixels
    9792,       // H. sync start from start of active in pixels
    9967,       // H. sync end from start of active in pixels
    450000,     // H. frequency in Hz
    4500,       // V. total lines
    4320,       // V. active lines
    4320,       // V. blank start from start of active in lines
    4499,       // V. blank end from start of active in lines
    4336,       // V. sync start from start of active in lines
    4355,       // V. sync end from start of active in lines
    100,        // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 201 and 209
DD_TIMING_INFO g_Timing7680x4320_p_120 = {
    4752000000, // Pixel clock in Hz
    8800,       // H. total pixels
    7680,       // H. active pixels
    7680,       // H. blank start from start of active in pixels
    8799,       // H. blank end from start of active in pixels
    8032,       // H. sync start from start of active in pixels
    8207,       // H. sync end from start of active in pixels
    540000,     // H. frequency in Hz
    4500,       // V. total lines
    4320,       // V. active lines
    4320,       // V. blank start from start of active in lines
    4499,       // V. blank end from start of active in lines
    4336,       // V. sync start from start of active in lines
    4355,       // V. sync end from start of active in lines
    120,        // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 216
DD_TIMING_INFO g_Timing10240x4320_p_100 = {
    5940000000, // Pixel clock in Hz
    13200,      // H. total pixels
    10240,      // H. active pixels
    10240,      // H. blank start from start of active in pixels
    13199,      // H. blank end from start of active in pixels
    12432,      // H. sync start from start of active in pixels
    12607,      // H. sync end from start of active in pixels
    450000,     // H. frequency in Hz
    4500,       // V. total lines
    4320,       // V. active lines
    4320,       // V. blank start from start of active in lines
    4499,       // V. blank end from start of active in lines
    4336,       // V. sync start from start of active in lines
    4355,       // V. sync end from start of active in lines
    100,        // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 217
DD_TIMING_INFO g_Timing10240x4320_p_120 = {
    5940000000, // Pixel clock in Hz
    11000,      // H. total pixels
    10240,      // H. active pixels
    10240,      // H. blank start from start of active in pixels
    10999,      // H. blank end from start of active in pixels
    10528,      // H. sync start from start of active in pixels
    10703,      // H. sync end from start of active in pixels
    540000,     // H. frequency in Hz
    4500,       // V. total lines
    4320,       // V. active lines
    4320,       // V. blank start from start of active in lines
    4499,       // V. blank end from start of active in lines
    4336,       // V. sync start from start of active in lines
    4355,       // V. sync end from start of active in lines
    120,        // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 218
DD_TIMING_INFO g_Timing4096x2160_p_100 = {
    1188000000, // Pixel clock in Hz
    5280,       // H. total pixels
    4096,       // H. active pixels
    4096,       // H. blank start from start of active in pixels
    5279,       // H. blank end from start of active in pixels
    4896,       // H. sync start from start of active in pixels
    4983,       // H. sync end from start of active in pixels
    225000,     // H. frequency in Hz
    2250,       // V. total lines
    2160,       // V. active lines
    2160,       // V. blank start from start of active in lines
    2249,       // V. blank end from start of active in lines
    2168,       // V. sync start from start of active in lines
    2177,       // V. sync end from start of active in lines
    100,        // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// Format 219
DD_TIMING_INFO g_Timing4096x2160_p_120 = {
    1188000000, // Pixel clock in Hz
    4400,       // H. total pixels
    4096,       // H. active pixels
    4096,       // H. blank start from start of active in pixels
    4399,       // H. blank end from start of active in pixels
    4184,       // H. sync start from start of active in pixels
    4271,       // H. sync end from start of active in pixels
    270000,     // H. frequency in Hz
    2250,       // V. total lines
    2160,       // V. active lines
    2160,       // V. blank start from start of active in lines
    2249,       // V. blank end from start of active in lines
    2168,       // V. sync start from start of active in lines
    2177,       // V. sync end from start of active in lines
    120,        // Refresh rate
    0,          // Interlaced
    0,          // Timing flags -- H+, V+
    0,          // Timing flags -- H+, V+
};

// HDMI_VIC 1
DD_TIMING_INFO g_Timing3840x2160_p_30 = {

    297000000, // Pixel clock in Hz
    4400,      // H. total pixels
    3840,      // H. active pixels
    3840,      // H. blank start from start of active in pixels
    4399,      // H. blank end from start of active in pixels
    4016,      // H. sync start from start of active in pixels
    4103,      // H. sync end from start of active in pixels
    67500,     // H. frequency in Hz
    2250,      // V. total lines
    2160,      // V. active lines
    2160,      // V. blank start from start of active in lines
    2249,      // V. blank end from start of active in lines
    2168,      // V. sync start from start of active in lines
    2177,      // V. sync end from start of active in lines
    30,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// HDMI_VIC 2
DD_TIMING_INFO g_Timing3840x2160_p_25 = {

    297000000, // Pixel clock in Hz
    5280,      // H. total pixels
    3840,      // H. active pixels
    3840,      // H. blank start from start of active in pixels
    5279,      // H. blank end from start of active in pixels
    4896,      // H. sync start from start of active in pixels
    4983,      // H. sync end from start of active in pixels
    56250,     // H. frequency in Hz
    2250,      // V. total lines
    2160,      // V. active lines
    2160,      // V. blank start from start of active in lines
    2249,      // V. blank end from start of active in lines
    2168,      // V. sync start from start of active in lines
    2177,      // V. sync end from start of active in lines
    25,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// HDMI_VIC 3
DD_TIMING_INFO g_Timing3840x2160_p_24 = {

    297000000, // Pixel clock in Hz
    5500,      // H. total pixels
    3840,      // H. active pixels
    3840,      // H. blank start from start of active in pixels
    5499,      // H. blank end from start of active in pixels
    5116,      // H. sync start from start of active in pixels
    5203,      // H. sync end from start of active in pixels
    54000,     // H. frequency in Hz
    2250,      // V. total lines
    2160,      // V. active lines
    2160,      // V. blank start from start of active in lines
    2249,      // V. blank end from start of active in lines
    2168,      // V. sync start from start of active in lines
    2177,      // V. sync end from start of active in lines
    24,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

// HDMI_VIC 4
DD_TIMING_INFO g_Timing4096x2160_p_24 = {

    297000000, // Pixel clock in Hz
    5500,      // H. total pixels
    4096,      // H. active pixels
    4096,      // H. blank start from start of active in pixels
    5499,      // H. blank end from start of active in pixels
    5116,      // H. sync start from start of active in pixels
    5203,      // H. sync end from start of active in pixels
    54000,     // H. frequency in Hz
    2250,      // V. total lines
    2160,      // V. active lines
    2160,      // V. blank start from start of active in lines
    2249,      // V. blank end from start of active in lines
    2168,      // V. sync start from start of active in lines
    2177,      // V. sync end from start of active in lines
    24,        // Refresh rate
    0,         // Interlaced
    0,         // Timing flags -- H+, V+
    0,         // Timing flags -- H+, V+
};

//
//    Timing 1366x768_120_S3D_CPT - 33.7% VBI
DD_TIMING_INFO g_Timing1366x768_120_S3D_CPT = {

    200000000, // Pixel clock in Hz
    1438,      // H. total pixels
    1366,      // H. active pixels
    1366,      // H. blank start from start of active in pixels
    1437,      // H. blank end from start of active in pixels
    1374,      // H. sync start from start of active in pixels
    1429,      // H. sync end from start of active in pixels
    139082,    // H. frequency in Hz
    1159,      // V. total lines
    768,       // V. active lines
    768,       // V. blank start from start of active in lines
    1158,      // V. blank end from start of active in lines
    771,       // V. sync start from start of active in lines
    776,       // V. sync end from start of active in lines
    120,       // Refresh rate
    0,         // Interlaced
    1,         // Timing flags -- H-, V+
    0,         // Timing flags -- H-, V+
};

//
//    Timing 1600x900_120_S3D_CPT - 39.5% VBI
DD_TIMING_INFO g_Timing1600x900_120_S3D_CPT = {

    360000000, // Pixel clock in Hz
    2016,      // H. total pixels
    1600,      // H. active pixels
    1600,      // H. blank start from start of active in pixels
    2015,      // H. blank end from start of active in pixels
    1608,      // H. sync start from start of active in pixels
    2007,      // H. sync end from start of active in pixels
    178571,    // H. frequency in Hz
    1488,      // V. total lines
    900,       // V. active lines
    900,       // V. blank start from start of active in lines
    1487,      // V. blank end from start of active in lines
    900,       // V. sync start from start of active in lines
    905,       // V. sync end from start of active in lines
    120,       // Refresh rate
    0,         // Interlaced
    1,         // Timing flags -- H-, V+
    0,         // Timing flags -- H-, V+
};

//
//    Timing 1920x1080_120_S3D (CMI+MSTAR timing with HBlank restriction of 80) // Mstar said 70, but using 80 as it's divisible by 8. Gives 28% VBI
DD_TIMING_INFO g_Timing1920x1080_120_S3D = {

    360000000, // Pixel clock in Hz
    2000,      // H. total pixels
    1920,      // H. active pixels
    1920,      // H. blank start from start of active in pixels
    1999,      // H. blank end from start of active in pixels
    1928,      // H. sync start from start of active in pixels
    1991,      // H. sync end from start of active in pixels
    180000,    // H. frequency in Hz
    1500,      // V. total lines
    1080,      // V. active lines
    1080,      // V. blank start from start of active in lines
    1499,      // V. blank end from start of active in lines
    1080,      // V. sync start from start of active in lines
    1085,      // V. sync end from start of active in lines
    120,       // Refresh rate
    0,         // Interlaced
    1,         // Timing flags -- H-, V+
    0,         // Timing flags -- H-, V+
};

//
//    Timing 1920x1080_100_S3D (CMI+MSTAR timing with HBlank restriction of 80) // Mstar said 70, but using 80 as it's divisible by 8. Gives 40% VBI with 100Hz
// Lot of flicker was observed with this on CMI
DD_TIMING_INFO g_Timing1920x1080_100_S3D = {

    360000000, // Pixel clock in Hz
    2000,      // H. total pixels
    1920,      // H. active pixels
    1920,      // H. blank start from start of active in pixels
    1999,      // H. blank end from start of active in pixels
    1928,      // H. sync start from start of active in pixels
    1991,      // H. sync end from start of active in pixels
    180000,    // H. frequency in Hz
    1800,      // V. total lines
    1080,      // V. active lines
    1080,      // V. blank start from start of active in lines
    1799,      // V. blank end from start of active in lines
    1080,      // V. sync start from start of active in lines
    1085,      // V. sync end from start of active in lines
    100,       // Refresh rate
    0,         // Interlaced
    1,         // Timing flags -- H-, V+
    0,         // Timing flags -- H-, V+
};

DD_TIMING_INFO g_Timing1280x768_60 = {
    79500000,   // Pixel clock in Hz
    1664,       // H. total pixels
    1280,       // H. active pixels
    1280,       // H. blank start from start of active in pixels
    1663,       // H. blank end from start of active in pixels
    1344,       // H. sync start from start of active in pixels
    1344 + 127, // H. sync end from start of active in pixels
    47776,      // H. frequency in Hz
    798,        // V. total lines
    768,        // V. active lines
    768,        // V. blank start from start of active in lines
    797,        // V. blank end from start of active in lines
    771,        // V. sync start from start of active in lines
    771 + 6,    // V. sync end from start of active in lines
    60,         // Refresh rate
    0,          // Interlaced
    1,          // Timing flags -- H-, V+
    0,          // Timing flags -- H-, V+
};

DD_TIMING_INFO g_Timing1280x800_60 = {

    83500000,   // Pixel clock in Hz
    1680,       // H. total pixels
    1280,       // H. active pixels
    1280,       // H. blank start from start of active in pixels
    1679,       // H. blank end from start of active in pixels
    1352,       // H. sync start from start of active in pixels
    1352 + 127, // H. sync end from start of active in pixels
    48702,      // H. frequency in Hz
    831,        // V. total lines
    800,        // V. active lines
    800,        // V. blank start from start of active in lines
    830,        // V. blank end from start of active in lines
    803,        // V. sync start from start of active in lines
    803 + 5,    // V. sync end from start of active in lines
    60,         // Refresh rate
    0,          // Interlaced
    1,          // Timing flags -- H-, V+
    0,          // Timing flags -- H-, V+
};

DD_TIMING_INFO g_Timing1440x900_60 = {

    106500000,  // Pixel clock in Hz
    1904,       // H. total pixels
    1440,       // H. active pixels
    1440,       // H. blank start from start of active in pixels
    1903,       // H. blank end from start of active in pixels
    1520,       // H. sync start from start of active in pixels
    1520 + 151, // H. sync end from start of active in pixels
    55935,      // H. frequency in Hz
    934,        // V. total lines
    900,        // V. active lines
    900,        // V. blank start from start of active in lines
    933,        // V. blank end from start of active in lines
    903,        // V. sync start from start of active in lines
    903 + 5,    // V. sync end from start of active in lines
    60,         // Refresh rate
    0,          // Interlaced
    1,          // Timing flags -- H-, V+
    0,          // Timing flags -- H-, V+
};

DD_TIMING_INFO g_Timing1400x1050_60 = {

    121750000,  // Pixel clock in Hz
    1864,       // H. total pixels
    1400,       // H. active pixels
    1400,       // H. blank start from start of active in pixels
    1863,       // H. blank end from start of active in pixels
    1488,       // H. sync start from start of active in pixels
    1488 + 143, // H. sync end from start of active in pixels
    65317,      // H. frequency in Hz
    1089,       // V. total lines
    1050,       // V. active lines
    1050,       // V. blank start from start of active in lines
    1088,       // V. blank end from start of active in lines
    1053,       // V. sync start from start of active in lines
    1053 + 3,   // V. sync end from start of active in lines
    60,         // Refresh rate
    0,          // Interlaced
    1,          // Timing flags -- H-, V+
    0,          // Timing flags -- H-, V+
};

DD_TIMING_INFO g_Timing1680x1050_60 = {

    146250000, // Pixel clock in Hz
    2240,      // H. total pixels
    1680,      // H. active pixels
    1680,      // H. blank start from start of active in pixels
    2239,      // H. blank end from start of active in pixels
    1784,      // H. sync start from start of active in pixels
    1959,      // H. sync end from start of active in pixels
    65290,     // H. frequency in Hz
    1089,      // V. total lines
    1050,      // V. active lines
    1050,      // V. blank start from start of active in lines
    1088,      // V. blank end from start of active in lines
    1053,      // V. sync start from start of active in lines
    1058,      // V. sync end from start of active in lines
    60,        // Refresh rate
    0,         // Interlaced
    1,         // Timing flags -- H-, V+
    0,         // Timing flags -- H-, V+
};

//////////////////////////////////////////
//
// Table of established modes
//
// Note: The table is alligned in the
// bitmask order. Don't add anything in
// between. i.e. Bit5 indicates 640x480x67Hz
// Since we don't support it put an empty
// entry of NULL.
//
//////////////////////////////////////////
ESTABLISHED_MODE_TABLE g_EstablishedModeTable[] = {
    { &g_Timing800x600_60 },
    { NULL }, //{&g_Timing800x600_56},
    { &g_Timing640x480_75 },
    { &g_Timing640x480_72 },
    { NULL }, //&g_Timing640x480_67},
    { &g_Timing640x480_60 },
    { NULL }, //&g_Timing720x400_88},
    { NULL }, //&g_Timing720x400_70},
    { &g_Timing1280x1024_75 },
    { &g_Timing1024x768_75 },
    { &g_Timing1024x768_70 },
    { &g_Timing1024x768_60 },
    { NULL }, //&g_Timing1024x768_87i},
    { NULL }, //&g_Timing832x624_75},
    { &g_Timing800x600_75 },
    { &g_Timing800x600_72 },
    { NULL },
    { NULL },
    { NULL },
    { NULL },
    { NULL },
    { NULL },
    { NULL },
    { NULL } //&g_Timing1152x870_75}
};
DDU32 g_ulTotalEstablishedModes = sizeof(g_EstablishedModeTable) / sizeof(g_EstablishedModeTable[0]);

//////////////////////////////////////////
//
// Table of modes in Established Timings III
// This got introduced with the EDID spec 1.4
//
// All the NULL fields in the table will be replaced by
// timings from VESA DMT in future. Right now we wont be enumerating these modes
//
// Note: The table is alligned in the
// bitmask order. Don't add anything in
// between.
//
//////////////////////////////////////////
ESTABLISHED_MODE_TABLE g_EstTiming3ModeTable[] = {
    { NULL }, // No Timing for DMT ID 0, added so that it helps to get
    // required Timing just using it as table index
    { &g_Timing1152x864_75 }, // 1152 x 864 @ 75 Hz
    { &g_Timing1024x768_85 }, // 1024 x 768 @ 85 Hz
    { &g_Timing800x600_85 },  // 800 x 600 @ 85 Hz
    { NULL },                 // {&g_Timing848x480_60},// 848 x 480 @ 60 Hz
    { &g_Timing640x480_85 },  // 640 x 480 @ 85 Hz
    { NULL },                 //{&g_Timing720x400_85},// 720 x 400 @ 85 Hz
    { NULL },                 //{&g_Timing640x400_85},// 640 x 400 @ 85 Hz
    { NULL },                 //{&g_Timing640x350_85},// 640 x 350 @ 85 Hz

    { &g_Timing1280x1024_85 }, // 1280 x 1024 @ 85 Hz
    { &g_Timing1280x1024_60 }, // 1280 x 1024 @ 60 Hz
    { &g_Timing1280x960_85 },  // 1280 x 960 @ 85 Hz
    { &g_Timing1280x960_60 },  // 1280 x 960 @ 60 Hz
    { NULL },                  //{&g_Timing1280x768_85},// 1280 x 768 @ 85 Hz
    { NULL },                  //{&g_Timing1280x768_75},// 1280 x 768 @ 75 Hz
    { NULL },                  //{&g_Timing1280x768_60},// 1280 x 768 @ 60 Hz
    { NULL },                  //{&g_Timing1280x768_60},// 1280 x 768 @ 60 Hz (RB)

    { NULL },                 //{&g_Timing1440x1050_75},// 1400 x 1050 @ 75 Hz
    { NULL },                 //{&g_Timing1440x1050_60},// 1400 x 1050 @ 60 Hz
    { NULL },                 //{&g_Timing1440x1050_60},// 1400 x 1050 @ 60 Hz (RB)
    { NULL },                 //{&g_Timing1440x900_85},// 1440 x 900 @ 85 Hz
    { NULL },                 //{&g_Timing1440x900_75},// 1440 x 900 @ 75 Hz
    { NULL },                 //{&g_Timing1440x900_60},// 1440 x 900 @ 60 Hz
    { NULL },                 //{&g_Timing1440x900_60},// 1440 x 900 @ 60 Hz (RB)
    { &g_Timing1360x768_60 }, // 1360 x 768 @ 60 Hz

    { &g_Timing1600x1200_70 }, // 1600 x 1200 @ 70 Hz
    { NULL },                  //{&g_Timing1600x1200_65},// 1600 x 1200 @ 65 Hz
    { &g_Timing1600x1200_60 }, // 1600 x 1200 @ 60 Hz
    { NULL },                  //{&g_Timing1680x1050_85},// 1680 x 1050 @ 85 Hz
    { NULL },                  //{&g_Timing1680x1050_75},// 1680 x 1050 @ 75 Hz
    { NULL },                  //{&g_Timing1680x1050_60},// 1680 x 1050 @ 60 Hz
    { NULL },                  //{&g_Timing1680x1050_60},// 1680 x 1050 @ 60 Hz (RB)
    { NULL },                  //{&g_Timing1440x1050_85},// 1400 x 1050 @ 85 Hz

    { NULL },                  //{&g_Timing1920x1200_60},// 1920 x 1200 @ 60 Hz
    { NULL },                  //{&g_Timing1920x1200_60},// 1920 x 1200 @ 60 Hz (RB)
    { &g_Timing1856x1392_75 }, // 1856 x 1392 @ 75 Hz
    { &g_Timing1856x1392_60 }, // 1856 x 1392 @ 60 Hz
    { &g_Timing1792x1344_75 }, // 1792 x 1344 @ 75 Hz
    { &g_Timing1792x1344_60 }, // 1792 x 1344 @ 60 Hz
    { &g_Timing1600x1200_85 }, // 1600 x 1200 @ 85 Hz
    { &g_Timing1600x1200_75 }, // 1600 x 1200 @ 75 Hz

    { &g_Timing1920x1440_75 }, // 1920 x 1440 @ 75 Hz
    { &g_Timing1920x1440_60 }, // 1920 x 1440 @ 60 Hz
    { NULL },                  //{&g_Timing1920x1200_85},// 1920 x 1200 @ 85 Hz
    { NULL },                  //{&g_Timing1920x1200_75},// 1920 x 1200 @ 75 Hz
    { NULL },                  // No Timing for this DMT ID, added so that it helps to get
    { NULL },                  // No Timing for this DMT ID, added so that it helps to get
    { NULL },                  // No Timing this DMT ID, added so that it helps to get
    { NULL },                  // No Timing this DMT ID, added so that it helps to get
};

DDU32 g_ulTotalEstTiming3modes = sizeof(g_EstTiming3ModeTable) / sizeof(g_EstTiming3ModeTable[0]);

/////////////////////////////////////////////////////////////////////////
//  CE-861b Supported Short Video Descriptors Table
//    <Index of the entry into the Table><Aspect Ratio><PR Mask><Timing Info >
//    Ref : CE-Extension Spec
/////////////////////////////////////////////////////////////////////////
CE_SHORT_VIDEO_MODE g_SDVOHDMISupportedCeShortVideoModes[] = {
    { 1, AVI_PAR_4_3, PR_1X, &g_Timing640x480p_60 }, // No Timing for DMT ID 0, added so that it helps to get   required Timing just using it as table index
    { 2, AVI_PAR_4_3, PR_1X, &g_Timing720x480_60 },        { 3, AVI_PAR_16_9, PR_1X, &g_Timing720x480_60 },      { 4, AVI_PAR_16_9, PR_1X, &g_Timing1280x720_a_60 },
    { 5, AVI_PAR_16_9, PR_1X, &g_Timing1920x1080_i_a_60 }, { 16, AVI_PAR_16_9, PR_1X, &g_Timing1920x1080_a_60 }, { 17, AVI_PAR_4_3, PR_1X, &g_Timing720x576_50 },
    { 18, AVI_PAR_16_9, PR_1X, &g_Timing720x576_50 },      { 19, AVI_PAR_16_9, PR_1X, &g_Timing1280x720_50 },    { 20, AVI_PAR_16_9, PR_1X, &g_Timing1920x1080_i_a_50 },
    { 31, AVI_PAR_16_9, PR_1X, &g_Timing1920x1080_a_50 },
};
DDU32 g_ulSDVOHDMINumSupportedCEModes = sizeof(g_SDVOHDMISupportedCeShortVideoModes) / sizeof(g_SDVOHDMISupportedCeShortVideoModes[0]);

/////////////////////////////////////////////////////////////////////////
//  CE-861G Supported Short Video Descriptors Table
//    <Index of the entry into the Table><Aspect Ratio><PR Mask><Timing Info >
//    Ref : CE-Extension Spec
/////////////////////////////////////////////////////////////////////////

CE_SHORT_VIDEO_MODE g_SupportedCeShortVideoModes[] = {
    { 0, AVI_PAR_NODATA, PR_1X, NULL }, // No Timing for DMT ID 0, added so that it helps to get  required Timing just using it as table index
    { 1, AVI_PAR_4_3, PR_1X, &g_Timing640x480p_60 },
    { 2, AVI_PAR_4_3, PR_1X, &g_Timing720x480_60 },
    { 3, AVI_PAR_16_9, PR_1X, &g_Timing720x480_60 },
    { 4, AVI_PAR_16_9, PR_1X, &g_Timing1280x720_a_60 },
    { 5, AVI_PAR_16_9, PR_1X, &g_Timing1920x1080_i_a_60 },
    { 6, AVI_PAR_4_3, PR_2X, &g_Timing1440x480_i_60 },
    { 7, AVI_PAR_16_9, PR_2X, &g_Timing1440x480_i_60 },
    { 8, AVI_PAR_4_3, PR_2X, &g_Timing1440x240_60 },
    { 9, AVI_PAR_16_9, PR_2X, &g_Timing1440x240_60 },
    { 10, AVI_PAR_4_3, PR_1X_TO_10X, &g_Timing2880x480_i_60 },
    { 11, AVI_PAR_16_9, PR_1X_TO_10X, &g_Timing2880x480_i_60 },
    { 12, AVI_PAR_4_3, PR_1X_TO_10X, &g_Timing2880x240_60 },
    { 13, AVI_PAR_16_9, PR_1X_TO_10X, &g_Timing2880x240_60 },
    { 14, AVI_PAR_4_3, PR_1X_2X, &g_Timing1440x480_60 },
    { 15, AVI_PAR_16_9, PR_1X_2X, &g_Timing1440x480_60 },
    { 16, AVI_PAR_16_9, PR_1X, &g_Timing1920x1080_a_60 },
    { 17, AVI_PAR_4_3, PR_1X, &g_Timing720x576_50 },
    { 18, AVI_PAR_16_9, PR_1X, &g_Timing720x576_50 },
    { 19, AVI_PAR_16_9, PR_1X, &g_Timing1280x720_50 },
    { 20, AVI_PAR_16_9, PR_1X, &g_Timing1920x1080_i_a_50 },
    { 21, AVI_PAR_4_3, PR_2X, &g_Timing1440x576_i_50 },
    { 22, AVI_PAR_16_9, PR_2X, &g_Timing1440x576_i_50 },
    { 23, AVI_PAR_4_3, PR_2X, &g_Timing1440x288_50 },
    { 24, AVI_PAR_16_9, PR_2X, &g_Timing1440x288_50 },
    { 25, AVI_PAR_4_3, PR_1X_TO_10X, &g_Timing2880x576_i_50 },
    { 26, AVI_PAR_16_9, PR_1X_TO_10X, &g_Timing2880x576_i_50 },
    { 27, AVI_PAR_4_3, PR_1X_TO_10X, &g_Timing2880x288_50 },
    { 28, AVI_PAR_16_9, PR_1X_TO_10X, &g_Timing2880x288_50 },
    { 29, AVI_PAR_4_3, PR_1X_2X, &g_Timing1440x576_50 },
    { 30, AVI_PAR_16_9, PR_1X_2X, &g_Timing1440x576_50 },
    { 31, AVI_PAR_16_9, PR_1X, &g_Timing1920x1080_a_50 },
    { 32, AVI_PAR_16_9, PR_1X, &g_Timing1920x1080_24 },
    { 33, AVI_PAR_16_9, PR_1X, &g_Timing1920x1080_25 },
    { 34, AVI_PAR_16_9, PR_1X, &g_Timing1920x1080_30 },
    { 35, AVI_PAR_4_3, PR_1X_2X_4X, &g_Timing2880x480_60 },
    { 36, AVI_PAR_16_9, PR_1X_2X_4X, &g_Timing2880x480_60 },
    { 37, AVI_PAR_4_3, PR_1X_2X_4X, &g_Timing2880x576_50 },
    { 38, AVI_PAR_16_9, PR_1X_2X_4X, &g_Timing2880x576_50 },
    { 39, AVI_PAR_16_9, PR_1X, NULL }, // &g_Timing1920x1080_i_50_Format39, // Cantiga does not support Interlaced mode with even VTotal (Cantiga HW sighting #304714)
    { 40, AVI_PAR_16_9, PR_1X, &g_Timing1920x1080_i_100 },
    { 41, AVI_PAR_16_9, PR_1X, &g_Timing1280x720_100 },
    { 42, AVI_PAR_4_3, PR_1X, &g_Timing720x576_100 },
    { 43, AVI_PAR_16_9, PR_1X, &g_Timing720x576_100 },
    { 44, AVI_PAR_4_3, PR_2X, &g_Timing1440x576_i_100 },
    { 45, AVI_PAR_16_9, PR_2X, &g_Timing1440x576_i_100 },
    { 46, AVI_PAR_16_9, PR_1X, &g_Timing1920x1080_i_120 },
    { 47, AVI_PAR_16_9, PR_1X, &g_Timing1280x720_120 },
    { 48, AVI_PAR_4_3, PR_1X, &g_Timing720x480_120 },
    { 49, AVI_PAR_16_9, PR_1X, &g_Timing720x480_120 },
    { 50, AVI_PAR_4_3, PR_2X, &g_Timing1440x480_i_120 },
    { 51, AVI_PAR_16_9, PR_2X, &g_Timing1440x480_i_120 },
    { 52, AVI_PAR_4_3, PR_1X, &g_Timing720x576_200 },
    { 53, AVI_PAR_16_9, PR_1X, &g_Timing720x576_200 },
    { 54, AVI_PAR_4_3, PR_2X, &g_Timing1440x576_i_200 },
    { 55, AVI_PAR_16_9, PR_2X, &g_Timing1440x576_i_200 },
    { 56, AVI_PAR_4_3, PR_1X, &g_Timing720x480_240 },
    { 57, AVI_PAR_16_9, PR_1X, &g_Timing720x480_240 },
    { 58, AVI_PAR_4_3, PR_2X, &g_Timing1440x480_i_240 },
    { 59, AVI_PAR_16_9, PR_2X, &g_Timing1440x480_i_240 },
    { 60, AVI_PAR_16_9, PR_1X, &g_Timing1280x720_p_24 },
    { 61, AVI_PAR_16_9, PR_1X, &g_Timing1280x720_p_25 },
    { 62, AVI_PAR_16_9, PR_1X, &g_Timing1280x720_p_30 },
    { 63, AVI_PAR_16_9, PR_1X, &g_Timing1920x1080_p_120 },
    { 64, AVI_PAR_16_9, PR_1X, &g_Timing1920x1080_p_100 },
    { 65, AVI_PAR_64_27, PR_1X, &g_Timing1280x720_p_24 },
    { 66, AVI_PAR_64_27, PR_1X, &g_Timing1280x720_p_25 },
    { 67, AVI_PAR_64_27, PR_1X, &g_Timing1280x720_p_30 },
    { 68, AVI_PAR_64_27, PR_1X, &g_Timing1280x720_50 },
    { 69, AVI_PAR_64_27, PR_1X, &g_Timing1280x720_a_60 },
    { 70, AVI_PAR_64_27, PR_1X, &g_Timing1280x720_100 },
    { 71, AVI_PAR_64_27, PR_1X, &g_Timing1280x720_120 },
    { 72, AVI_PAR_64_27, PR_1X, &g_Timing1920x1080_24 },
    { 73, AVI_PAR_64_27, PR_1X, &g_Timing1920x1080_25 },
    { 74, AVI_PAR_64_27, PR_1X, &g_Timing1920x1080_30 },
    { 75, AVI_PAR_64_27, PR_1X, &g_Timing1920x1080_a_50 },
    { 76, AVI_PAR_64_27, PR_1X, &g_Timing1920x1080_a_60 },
    { 77, AVI_PAR_64_27, PR_1X, &g_Timing1920x1080_p_100 },
    { 78, AVI_PAR_64_27, PR_1X, &g_Timing1920x1080_p_120 },
    { 79, AVI_PAR_64_27, PR_1X, &g_Timing1680x720_p_24 },
    { 80, AVI_PAR_64_27, PR_1X, &g_Timing1680x720_p_25 },
    { 81, AVI_PAR_64_27, PR_1X, &g_Timing1680x720_p_30 },
    { 82, AVI_PAR_64_27, PR_1X, &g_Timing1680x720_p_50 },
    { 83, AVI_PAR_64_27, PR_1X, &g_Timing1680x720_p_60 },
    { 84, AVI_PAR_64_27, PR_1X, &g_Timing1680x720_p_100 },
    { 85, AVI_PAR_64_27, PR_1X, &g_Timing1680x720_p_120 },
    { 86, AVI_PAR_64_27, PR_1X, &g_Timing2560x1080_p_24 },
    { 87, AVI_PAR_64_27, PR_1X, &g_Timing2560x1080_p_25 },
    { 88, AVI_PAR_64_27, PR_1X, &g_Timing2560x1080_p_30 },
    { 89, AVI_PAR_64_27, PR_1X, &g_Timing2560x1080_p_50 },
    { 90, AVI_PAR_64_27, PR_1X, &g_Timing2560x1080_p_60 },
    { 91, AVI_PAR_64_27, PR_1X, &g_Timing2560x1080_p_100 },
    { 92, AVI_PAR_64_27, PR_1X, &g_Timing2560x1080_p_120 },
    { 93, AVI_PAR_16_9, PR_1X, &g_Timing3840x2160_p_24 },
    { 94, AVI_PAR_16_9, PR_1X, &g_Timing3840x2160_p_25 },
    { 95, AVI_PAR_16_9, PR_1X, &g_Timing3840x2160_p_30 },
    { 96, AVI_PAR_16_9, PR_1X, &g_Timing3840x2160_p_50 },
    { 97, AVI_PAR_16_9, PR_1X, &g_Timing3840x2160_p_60 },
    { 98, AVI_PAR_NODATA, PR_1X, &g_Timing4096x2160_p_24 },
    { 99, AVI_PAR_NODATA, PR_1X, &g_Timing4096x2160_p_25 },
    { 100, AVI_PAR_NODATA, PR_1X, &g_Timing4096x2160_p_30 },
    { 101, AVI_PAR_NODATA, PR_1X, &g_Timing4096x2160_p_50 },
    { 102, AVI_PAR_NODATA, PR_1X, &g_Timing4096x2160_p_60 },
    { 103, AVI_PAR_64_27, PR_1X, &g_Timing3840x2160_p_24 },
    { 104, AVI_PAR_64_27, PR_1X, &g_Timing3840x2160_p_25 },
    { 105, AVI_PAR_64_27, PR_1X, &g_Timing3840x2160_p_30 },
    { 106, AVI_PAR_64_27, PR_1X, &g_Timing3840x2160_p_50 },
    { 107, AVI_PAR_64_27, PR_1X, &g_Timing3840x2160_p_60 },
    { 108, AVI_PAR_16_9, PR_1X, &g_Timing1280x720_p_48 },
    { 109, AVI_PAR_64_27, PR_1X, &g_Timing1280x720_p_48 },
    { 110, AVI_PAR_64_27, PR_1X, &g_Timing1680x720_p_48 },
    { 111, AVI_PAR_16_9, PR_1X, &g_Timing1920x1080_p_48 },
    { 112, AVI_PAR_64_27, PR_1X, &g_Timing1920x1080_p_48 },
    { 113, AVI_PAR_64_27, PR_1X, &g_Timing2560x1080_p_48 },
    { 114, AVI_PAR_16_9, PR_1X, &g_Timing3840x2160_p_48 },
    { 115, AVI_PAR_NODATA, PR_1X, &g_Timing4096x2160_p_48 },
    { 116, AVI_PAR_64_27, PR_1X, &g_Timing3840x2160_p_48 },
    { 117, AVI_PAR_16_9, PR_1X, &g_Timing3840x2160_p_100 },
    { 118, AVI_PAR_16_9, PR_1X, &g_Timing3840x2160_p_120 },
    { 119, AVI_PAR_64_27, PR_1X, &g_Timing3840x2160_p_100 },
    { 120, AVI_PAR_64_27, PR_1X, &g_Timing3840x2160_p_120 },
    { 121, AVI_PAR_64_27, PR_1X, &g_Timing5120x2160_p_24 },
    { 122, AVI_PAR_64_27, PR_1X, &g_Timing5120x2160_p_25 },
    { 123, AVI_PAR_64_27, PR_1X, &g_Timing5120x2160_p_30 },
    { 124, AVI_PAR_64_27, PR_1X, &g_Timing5120x2160_p_48 },
    { 125, AVI_PAR_64_27, PR_1X, &g_Timing5120x2160_p_50 },
    { 126, AVI_PAR_64_27, PR_1X, &g_Timing5120x2160_p_60 },
    { 127, AVI_PAR_64_27, PR_1X, &g_Timing5120x2160_p_100 },
    // Formats 128 to 192 are forbidden
    { 193, AVI_PAR_64_27, PR_1X, &g_Timing5120x2160_p_120 },
    { 194, AVI_PAR_16_9, PR_1X, &g_Timing7680x4320_p_24 },
    { 195, AVI_PAR_16_9, PR_1X, &g_Timing7680x4320_p_25 },
    { 196, AVI_PAR_16_9, PR_1X, &g_Timing7680x4320_p_30 },
    { 197, AVI_PAR_16_9, PR_1X, &g_Timing7680x4320_p_48 },
    { 198, AVI_PAR_16_9, PR_1X, &g_Timing7680x4320_p_50 },
    { 199, AVI_PAR_16_9, PR_1X, &g_Timing7680x4320_p_60 },
    { 200, AVI_PAR_16_9, PR_1X, &g_Timing7680x4320_p_100 },
    { 201, AVI_PAR_16_9, PR_1X, &g_Timing7680x4320_p_120 },
    { 202, AVI_PAR_64_27, PR_1X, &g_Timing7680x4320_p_24 },
    { 203, AVI_PAR_64_27, PR_1X, &g_Timing7680x4320_p_25 },
    { 204, AVI_PAR_64_27, PR_1X, &g_Timing7680x4320_p_30 },
    { 205, AVI_PAR_64_27, PR_1X, &g_Timing7680x4320_p_48 },
    { 206, AVI_PAR_64_27, PR_1X, &g_Timing7680x4320_p_50 },
    { 207, AVI_PAR_64_27, PR_1X, &g_Timing7680x4320_p_60 },
    { 208, AVI_PAR_64_27, PR_1X, &g_Timing7680x4320_p_100 },
    { 209, AVI_PAR_64_27, PR_1X, &g_Timing7680x4320_p_120 },
    { 210, AVI_PAR_64_27, PR_1X, &g_Timing10240x4320_p_24 },
    { 211, AVI_PAR_64_27, PR_1X, &g_Timing10240x4320_p_25 },
    { 212, AVI_PAR_64_27, PR_1X, &g_Timing10240x4320_p_30 },
    { 213, AVI_PAR_64_27, PR_1X, &g_Timing10240x4320_p_48 },
    { 214, AVI_PAR_64_27, PR_1X, &g_Timing10240x4320_p_50 },
    { 215, AVI_PAR_64_27, PR_1X, &g_Timing10240x4320_p_60 },
    { 216, AVI_PAR_64_27, PR_1X, &g_Timing10240x4320_p_100 },
    { 217, AVI_PAR_64_27, PR_1X, &g_Timing10240x4320_p_120 },
    { 218, AVI_PAR_NODATA, PR_1X, &g_Timing4096x2160_p_100 },
    { 219, AVI_PAR_NODATA, PR_1X, &g_Timing4096x2160_p_120 },

};
DDU32 g_ulNumSupportedCEModes = sizeof(g_SupportedCeShortVideoModes) / sizeof(g_SupportedCeShortVideoModes[0]);

/////////////////////////////////////////////////////////////////////////
// HDMI VIC table
//  4kx2k mode table
//    <Index of the entry into the Table><Aspect Ratio><PR Mask><Timing Info >
//    Ref : HDMI1.4 spec
// Since there is no VIC ID defined for these, making the VIC IDs of these timings same as SVD VICs.
// This is used to skip adding these modes as SVD modes are of higher priority and don't want to override those here
/////////////////////////////////////////////////////////////////////////
CE_SHORT_VIDEO_MODE g_4kx2kModes[] = {
    { 0, AVI_PAR_NODATA, PR_1X, NULL }, // No Timing for DMT ID 0, added so that it helps to get   required Timing just using it as table index
    { 95, AVI_PAR_16_9, PR_1X, &g_Timing3840x2160_p_30 },
    { 94, AVI_PAR_16_9, PR_1X, &g_Timing3840x2160_p_25 },
    { 93, AVI_PAR_16_9, PR_1X, &g_Timing3840x2160_p_24 },
    { 98, AVI_PAR_16_9, PR_1X, &g_Timing4096x2160_p_24 },
};

DDU32 g_ulNum4kx2kModes = sizeof(g_4kx2kModes) / sizeof(g_4kx2kModes[0]);
/////////////////////////////////////////////////////////////////////////
//
// Underscan mode support array
// First 2 entries indicate EDID supported X & Y. Next 2 entries indicate
// the underscan mode which need to be enumerated.
//
/////////////////////////////////////////////////////////////////////////
UNDER_SCAN_MODE g_UnderScanModes[] = {
    { 1280, 720, 1184, 666 },  // 1280 x 720 mode
    { 1920, 1080, 1776, 1000 } // 1920 x 1080 mode
};
DDU32 g_ulNumUnderScanModes = sizeof(g_UnderScanModes) / sizeof(g_UnderScanModes[0]);

//////////////////////////////////////////
//
// Table of static modes
//
//////////////////////////////////////////
// #define ALLCOLOR_BPP_MASK 0x1F  // Should be same as in modeentry.h
#define WINDOWS_COLOR_MASK 0x1E // doesn't have 4bpp
STATIC_MODE_TABLE g_StaticModeTable[] = {
    //{640,480, 0, ALLCOLOR_BPP_MASK, &g_TimingVGA_Mode3_640x480_60,0xFF},
    { 320, 200, 0, WINDOWS_COLOR_MASK, &g_Timing640x400_70, 0xFF },
    { 320, 240, 70, WINDOWS_COLOR_MASK, &g_Timing640x400_70, 0xFF },
    { 400, 300, 70, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 512, 384, 70, WINDOWS_COLOR_MASK, &g_Timing1024x768_70, 0xFF },
    //{640,350, 0, WINDOWS_COLOR_MASK, &g_Timing640x350_85,0xFF},
    { 640, 400, 70, WINDOWS_COLOR_MASK, &g_Timing640x400_70, 0xFF },
    //{640,400, 0, WINDOWS_COLOR_MASK, &g_Timing640x400_85,0xFF},
    //{720,400, 0, WINDOWS_COLOR_MASK, &g_Timing720x400_85,0xFF},

    { 640, 480, 60, WINDOWS_COLOR_MASK, &g_Timing640x480_60, 0xFF },
    { 640, 480, 70, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 640, 480, 72, WINDOWS_COLOR_MASK, &g_Timing640x480_72, 0xFF },
    { 640, 480, 75, WINDOWS_COLOR_MASK, &g_Timing640x480_75, 0xFF },
    { 640, 480, 85, WINDOWS_COLOR_MASK, &g_Timing640x480_85, 0xFF },
    { 640, 480, 100, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 640, 480, 120, WINDOWS_COLOR_MASK, NULL, 0xFF },

    //{ 800,600, 0, WINDOWS_COLOR_MASK, &g_Timing800x600_56,0xFF},
    { 800, 600, 60, WINDOWS_COLOR_MASK, &g_Timing800x600_60, 0xFF },
    //{ 800,600, 70, WINDOWS_COLOR_MASK, NULL,0xFF},
    { 800, 600, 72, WINDOWS_COLOR_MASK, &g_Timing800x600_72, 0xFF },
    { 800, 600, 75, WINDOWS_COLOR_MASK, &g_Timing800x600_75, 0xFF },
    { 800, 600, 85, WINDOWS_COLOR_MASK, &g_Timing800x600_85, 0xFF },
    { 800, 600, 100, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 800, 600, 120, WINDOWS_COLOR_MASK, NULL, 0xFF },
    //{ 848,480, 0, WINDOWS_COLOR_MASK, &g_Timing848x480_60,0xFF},

    //{1024,768, 0, WINDOWS_COLOR_MASK, &g_Timing1024x768_43i,0xFF},
    { 1024, 768, 60, WINDOWS_COLOR_MASK, &g_Timing1024x768_60, 0xFF },
    { 1024, 768, 70, WINDOWS_COLOR_MASK, &g_Timing1024x768_70, 0xFF },
    { 1024, 768, 75, WINDOWS_COLOR_MASK, &g_Timing1024x768_75, 0xFF },
    { 1024, 768, 85, WINDOWS_COLOR_MASK, &g_Timing1024x768_85, 0xFF },
    { 1024, 768, 100, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1024, 768, 120, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 1152, 864, 60, WINDOWS_COLOR_MASK, NULL, 0xFF },
    //{1152,864, 70, WINDOWS_COLOR_MASK, NULL,0xFF},
    //{1152,864, 72, WINDOWS_COLOR_MASK, NULL,0xFF},
    { 1152, 864, 75, WINDOWS_COLOR_MASK, &g_Timing1152x864_75, 0xFF },
    { 1152, 864, 85, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1152, 864, 100, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 1280, 600, 60, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 1280, 720, 60, WINDOWS_COLOR_MASK, &g_Timing1280x720_a_60, 0xFF },
    { 1280, 720, 75, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1280, 720, 85, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1280, 720, 100, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 1280, 768, 60, WINDOWS_COLOR_MASK, &g_Timing1280x768_60, 0xFF }, //???? should this be enumerated as GTF/CVT mode
    { 1280, 768, 75, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1280, 768, 85, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1280, 768, 120, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 1280, 800, 60, WINDOWS_COLOR_MASK, &g_Timing1280x800_60, 0xFF },
    { 1280, 800, 75, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1280, 800, 85, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1280, 800, 120, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 1280, 960, 60, WINDOWS_COLOR_MASK, &g_Timing1280x960_60, 0xFF },
    { 1280, 960, 75, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1280, 960, 85, WINDOWS_COLOR_MASK, &g_Timing1280x960_85, 0xFF },
    { 1280, 960, 120, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 1280, 1024, 60, WINDOWS_COLOR_MASK, &g_Timing1280x1024_60, 0xFF },
    //{1280,1024, 70, WINDOWS_COLOR_MASK, NULL,0xFF},
    //{1280,1024, 72, WINDOWS_COLOR_MASK, NULL,0xFF},
    { 1280, 1024, 75, WINDOWS_COLOR_MASK, &g_Timing1280x1024_75, 0xFF },
    { 1280, 1024, 85, WINDOWS_COLOR_MASK, &g_Timing1280x1024_85, 0xFF },
    { 1280, 1024, 100, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1280, 1024, 120, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 1360, 768, 60, WINDOWS_COLOR_MASK, &g_Timing1360x768_60, 0xFF },
    { 1360, 768, 120, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 1366, 768, 60, WINDOWS_COLOR_MASK, &g_Timing1366x768_60, 0xFF },
    { 1366, 768, 60, WINDOWS_COLOR_MASK, &g_Timing1366x768_60rb, 0xFF },

    { 1400, 1050, 60, WINDOWS_COLOR_MASK, &g_Timing1400x1050_60, 0xFF },
    { 1400, 1050, 75, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1400, 1050, 85, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1400, 1050, 120, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 1440, 900, 60, WINDOWS_COLOR_MASK, &g_Timing1440x900_60, 0xFF },
    { 1440, 900, 75, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1440, 900, 85, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1440, 900, 120, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 1600, 900, 60, WINDOWS_COLOR_MASK, &g_Timing1600x900_60rb, 0xFF },
    { 1600, 900, 75, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1600, 900, 85, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1600, 900, 100, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1600, 900, 120, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 1600, 1200, 60, WINDOWS_COLOR_MASK, &g_Timing1600x1200_60, 0xFF },
    //{1600,1200, 0, WINDOWS_COLOR_MASK, &g_Timing1600x1200_65,0xFF},
    { 1600, 1200, 70, WINDOWS_COLOR_MASK, &g_Timing1600x1200_70, 0xFF },
    //{1600,1200, 72, WINDOWS_COLOR_MASK, NULL,0xFF},
    { 1600, 1200, 75, WINDOWS_COLOR_MASK, &g_Timing1600x1200_75, 0xFF },
    { 1600, 1200, 85, WINDOWS_COLOR_MASK, &g_Timing1600x1200_85, 0xFF },
    { 1600, 1200, 100, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1600, 1200, 120, WINDOWS_COLOR_MASK, NULL, 0xFF },

    // New mode added as part of the DCN 634024
    { 1680, 1050, 60, WINDOWS_COLOR_MASK, &g_Timing1680x1050_60, 0xFF },

    { 1680, 1050, 75, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1680, 1050, 85, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1680, 1050, 120, WINDOWS_COLOR_MASK, NULL, 0xFF },

    // Uncommented as part of DCN 634024. To keep in sync with OMP BML
    { 1792, 1344, 60, WINDOWS_COLOR_MASK, &g_Timing1792x1344_60, 0xFF },
    { 1792, 1344, 75, WINDOWS_COLOR_MASK, &g_Timing1792x1344_75, 0xFF },

    { 1792, 1344, 120, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 1856, 1392, 60, WINDOWS_COLOR_MASK, &g_Timing1856x1392_60, 0xFF },
    { 1856, 1392, 75, WINDOWS_COLOR_MASK, &g_Timing1856x1392_75, 0xFF },

    { 1856, 1392, 120, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 1920, 1080, 60, WINDOWS_COLOR_MASK, &g_Timing1920x1080_a_60, 0xFF },
    { 1920, 1080, 75, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1920, 1080, 85, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1920, 1080, 100, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 1920, 1200, 60, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1920, 1200, 75, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1920, 1200, 85, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1920, 1200, 120, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 1920, 1440, 60, WINDOWS_COLOR_MASK, &g_Timing1920x1440_60, 0xFF },
    { 1920, 1440, 75, WINDOWS_COLOR_MASK, &g_Timing1920x1440_75, 0xFF },
    { 1920, 1440, 85, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 1920, 1440, 120, WINDOWS_COLOR_MASK, NULL, 0xFF },

    // Added as part of DCN 634024. To keep in sync with OMP BML
    { 1920, 1440, 100, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 2048, 1152, 60, WINDOWS_COLOR_MASK, &g_Timing2048x1152_60rb, 0xFF },

    { 2048, 1536, 60, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 2048, 1536, 75, WINDOWS_COLOR_MASK, NULL, 0xFF },

    // Following modes are added as part of DCN 634024.
    // To keep static list in sync with OMP BML
    // The timings for the modes below will be generated as
    // part of Standard GTF formula.
    { 2048, 1536, 85, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 2048, 1536, 100, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 2560, 1600, 60, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 2560, 1600, 75, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 2560, 1600, 85, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 2560, 1600, 120, WINDOWS_COLOR_MASK, NULL, 0xFF },

    { 2560, 1920, 60, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 2560, 1920, 75, WINDOWS_COLOR_MASK, NULL, 0xFF },

    // Enabled from Ironlake onwards.
    { 2560, 2048, 60, WINDOWS_COLOR_MASK, NULL, 0xFF },
    { 2560, 2048, 75, WINDOWS_COLOR_MASK, NULL, 0xFF }
};
DDU32 g_ulTotalStaticModes = sizeof(g_StaticModeTable) / sizeof(g_StaticModeTable[0]);

// RCR 1023191 : Support for VESA DisplayID

//////////////////////////////////////////
//
// Table of modes in VESA Standard Timings
// These are timings are specific to DisplayID
// and not EDID
//
// All the NULL fields in the table will be replaced by
// timings from VESA DMT in future. Right now we wont
// be enumerating these modes.
// Display ID VESA Timing Standard Data Block
// Refer Table 4.24
//
// Note: The table is alligned in the
// bitmask order. Don't add anything in
// between.
//
//////////////////////////////////////////
DISPLAYID_DMTID_STANDARD_TIMING g_DisplayID_VESA_DMTModeTable[] = {
    { 0x00, NULL }, // No Timing for DMT ID 0, added so that it helps to get
    // required Timing just using it as table index

    { 0x01, &g_Timing640x350_85 }, // 640 x 350 @ 85 Hz

    { 0x02, &g_Timing640x400_85 }, // 640 x 400 @ 85 Hz

    { 0x03, &g_Timing800x600_85 }, // 800 x 600 @ 85 Hz

    { 0x04, &g_Timing640x480_60 }, // 640 x 480 @ 60 Hz
    { 0x05, &g_Timing640x480_72 }, // 640 x 480 @ 72 Hz
    { 0x06, &g_Timing640x480_75 }, // 640 x 480 @ 75 Hz
    { 0x07, &g_Timing640x480_85 }, // 640 x 480 @ 85 Hz

    { 0x08, &g_Timing800x600_56 }, // 800 x 600 @ 85 Hz
    { 0x09, &g_Timing800x600_60 }, // 800 x 600 @ 60 Hz
    { 0x0A, &g_Timing800x600_72 }, // 800 x 600 @ 72 Hz
    { 0x0B, &g_Timing800x600_75 }, // 800 x 600 @ 75 Hz
    { 0x0C, &g_Timing800x600_85 }, // 800 x 600 @ 85 Hz
    { 0x0D, NULL },                // 800 x 600 @ 120 Hz (RB)

    { 0x0E, &g_Timing848x480_60 }, // 848 x 480 @ 60 Hz

    { 0x0F, &g_Timing1024x768_43i }, // 1024 x 768 @ 43 Hz (INT)
    { 0x10, &g_Timing1024x768_60 },  // 1024 x 768 @ 60 Hz
    { 0x11, &g_Timing1024x768_70 },  // 1024 x 768 @ 70 Hz
    { 0x12, &g_Timing1024x768_75 },  // 1024 x 768 @ 75 Hz
    { 0x13, &g_Timing1024x768_85 },  // 1024 x 768 @ 85 Hz
    { 0x14, NULL },                  // 1024 x 768 @ 120 Hz (RB)

    { 0x15, &g_Timing1152x864_75 }, // 1152 x 864 @ 75 Hz

    { 0x16, NULL }, // 1280 x 768 @ 60 Hz (RB)
    { 0x17, NULL }, // 1280 x 768 @ 60 Hz
    { 0x18, NULL }, // 1280 x 768 @ 75 Hz
    { 0x19, NULL }, // 1280 x 768 @ 85 Hz
    { 0x1A, NULL }, // 1280 x 768 @ 120 Hz (RB)

    { 0x1B, NULL }, // 1280 x 800 @ 60 Hz (RB)
    { 0x1C, NULL }, // 1280 x 800 @ 60 Hz
    { 0x1D, NULL }, // 1280 x 800 @ 75 Hz
    { 0x1E, NULL }, // 1280 x 800 @ 85 Hz
    { 0x1F, NULL }, // 1280 x 800 @ 120 Hz

    { 0x20, &g_Timing1280x960_60 }, // 1280 x 960 @ 60 Hz
    { 0x21, NULL },                 // 1280 x 960 @ 85 Hz
    { 0x22, NULL },                 // 1280 x 960 @ 120 Hz (RB)

    { 0x23, &g_Timing1280x1024_60 }, // 1280 x 1024 @ 60 Hz
    { 0x24, &g_Timing1280x1024_75 }, // 1280 x 1024 @ 75 Hz
    { 0x25, &g_Timing1280x1024_85 }, // 1280 x 1024 @ 85 Hz
    { 0x26, NULL },                  // 1280 x 1024 @ 120 Hz (RB)

    { 0x27, &g_Timing1360x768_60 }, // 1360 x 768 @ 60 Hz
    { 0x28, NULL },                 // 1360 x 768 @ 120 Hz (RB)

    { 0x29, NULL }, // 1400 x 1050 @ 60 Hz (RB)
    { 0x2A, NULL }, // 1400 x 1050 @ 60 Hz
    { 0x2B, NULL }, // 1400 x 1050 @ 75 Hz
    { 0x2C, NULL }, // 1400 x 1050 @ 85 Hz
    { 0x2D, NULL }, // 1400 x 1050 @ 120 Hz (RB)

    { 0x2E, NULL }, // 1440 x 900 @ 60 Hz (RB)
    { 0x2F, NULL }, // 1440 x 900 @ 60 Hz
    { 0x30, NULL }, // 1440 x 900 @ 75 Hz
    { 0x31, NULL }, // 1440 x 900 @ 85 Hz
    { 0x32, NULL }, // 1440 x 900 @ 120 Hz (RB)

    { 0x33, &g_Timing1600x1200_60 }, // 1600 x 1200 @ 60 Hz
    { 0x34, &g_Timing1600x1200_65 }, // 1600 x 1200 @ 65 Hz
    { 0x35, &g_Timing1600x1200_70 }, // 1600 x 1200 @ 70 Hz
    { 0x36, &g_Timing1600x1200_75 }, // 1600 x 1200 @ 75 Hz
    { 0x37, &g_Timing1600x1200_85 }, // 1600 x 1200 @ 85 Hz
    { 0x38, NULL },                  // 1600 x 1200 @ 60 Hz (RB)

    { 0x39, NULL }, // 1680 x 1050 @ 60 Hz (RB)
    { 0x3A, NULL }, // 1680 x 1050 @ 60 Hz
    { 0x3B, NULL }, // 1680 x 1050 @ 75 Hz
    { 0x3C, NULL }, // 1680 x 1050 @ 85 Hz
    { 0x3D, NULL }, // 1680 x 1050 @ 120 Hz (RB)

    { 0x3E, &g_Timing1792x1344_60 }, // 1792 x 1344 @ 60 Hz
    { 0x3F, &g_Timing1792x1344_75 }, // 1792 x 1344 @ 75 Hz
    { 0x40, NULL },                  // 1792 x 1344 @ 120 Hz (RB)

    { 0x41, &g_Timing1856x1392_60 }, // 1856 x 1392 @ 60 Hz
    { 0x42, &g_Timing1856x1392_75 }, // 1856 x 1392 @ 75 Hz
    { 0x43, NULL },                  // 1856 x 1392 @ 120 Hz (RB)

    { 0x44, NULL }, // 1920 x 1200 @ 60 Hz (RB)
    { 0x45, NULL }, // 1920 x 1200 @ 60 Hz
    { 0x46, NULL }, // 1920 x 1200 @ 75 Hz (RB)
    { 0x47, NULL }, // 1920 x 1200 @ 85 Hz
    { 0x48, NULL }, // 1920 x 1200 @ 120 Hz (RB)

    { 0x49, &g_Timing1920x1440_60 }, // 1920 x 1440 @ 60 Hz
    { 0x4A, &g_Timing1920x1440_75 }, // 1920 x 1440 @ 75 Hz
    { 0x4B, NULL },                  // 1920 x 1440 @ 120 Hz (RB)

    { 0x4C, NULL }, // 2560 x 1600 @ 60 Hz (RB)
    { 0x4D, NULL }, // 2560 x 1600 @ 60 Hz
    { 0x4E, NULL }, // 2560 x 1600 @ 75 Hz
    { 0x4F, NULL }, // 2560 x 1600 @ 85 Hz
    { 0x50, NULL }, // 2560 x 1600 @ 120 Hz (RB)

    { 0x51, &g_Timing1366x768_60 },    // 1366 x 768 @ 60 Hz
    { 0x52, NULL },                    // 1920 x 1080 @ 60 Hz
    { 0x53, NULL },                    // 1600 x 900 @ 60 Hz
    { 0x54, &g_Timing2048x1152_60rb }, // 2048 x 1152 @ 60 Hz
    { 0x55, NULL },                    // 1280 x 720 @ 60 Hz
    { 0x56, NULL },                    // 1366 x 768 @ 60 Hz (RB)
    { 0x57, NULL },                    // 4096 x 2160 @ 60 Hz (RB)
    { 0x58, NULL },                    // 4096 x 2160 @ 59.94 Hz (RB)
};

DDU32 g_ulTotalDisplayID_VESA_DMTmodes = sizeof(g_DisplayID_VESA_DMTModeTable) / sizeof(g_DisplayID_VESA_DMTModeTable[0]);

/////////////////////////////////////////////////////////////////////////
//  CE-861D Supported Short Video Descriptors Table
//    <Index of the entry into the Table><Aspect Ratio><PR Mask><Timing Info >
//    Ref : CE-Extension Spec
/////////////////////////////////////////////////////////////////////////
DISPLAYID_CE_STANDARD_TIMING g_DisplayID_CeStandardTiming[] = {

    { 0, AVI_PAR_NODATA, NULL }, // No Timing for CEA VIC ID 0, added so that it helps to get
    // required Timing just using it as table index
{ 1, AVI_PAR_4_3, &g_Timing640x480p_60 },
{ 2, AVI_PAR_4_3, &g_Timing720x480_60 },
{ 3, AVI_PAR_16_9, &g_Timing720x480_60 },
{ 4, AVI_PAR_16_9, &g_Timing1280x720_a_60 },
{ 5, AVI_PAR_16_9, &g_Timing1920x1080_i_a_60 },
{ 6, AVI_PAR_4_3, &g_Timing1440x480_i_60 },
{ 7, AVI_PAR_16_9, &g_Timing1440x480_i_60 },
{ 8, AVI_PAR_4_3, &g_Timing1440x240_60 },
{ 9, AVI_PAR_16_9, &g_Timing1440x240_60 },
{ 10, AVI_PAR_4_3, &g_Timing2880x480_i_60 },
{ 11, AVI_PAR_16_9, &g_Timing2880x480_i_60 },
{ 12, AVI_PAR_4_3, &g_Timing2880x240_60 },
{ 13, AVI_PAR_16_9, &g_Timing2880x240_60 },
{ 14, AVI_PAR_4_3, &g_Timing1440x480_60 },
{ 15, AVI_PAR_16_9, &g_Timing1440x480_60 },
{ 16, AVI_PAR_16_9, &g_Timing1920x1080_a_60 },
{ 17, AVI_PAR_4_3, &g_Timing720x576_50 },
{ 18, AVI_PAR_16_9, &g_Timing720x576_50 },
{ 19, AVI_PAR_16_9, &g_Timing1280x720_50 },
{ 20, AVI_PAR_16_9, &g_Timing1920x1080_i_a_60 },
{ 21, AVI_PAR_4_3, &g_Timing1440x576_i_50 },
{ 22, AVI_PAR_16_9, &g_Timing1440x576_i_50 },
{ 23, AVI_PAR_4_3, &g_Timing1440x288_50 },
{ 24, AVI_PAR_16_9, &g_Timing1440x288_50 },
{ 25, AVI_PAR_4_3, &g_Timing2880x576_i_50 },
{ 26, AVI_PAR_16_9, &g_Timing2880x576_i_50 },
{ 27, AVI_PAR_4_3, &g_Timing2880x288_50 },
{ 28, AVI_PAR_16_9, &g_Timing2880x288_50 },
{ 29, AVI_PAR_4_3, &g_Timing1440x576_50 },
{ 30, AVI_PAR_16_9, &g_Timing1440x576_50 },
{ 31, AVI_PAR_16_9, &g_Timing1920x1080_a_50 },
{ 32, AVI_PAR_16_9, &g_Timing1920x1080_24 },
{ 33, AVI_PAR_16_9, &g_Timing1920x1080_25 },
{ 34, AVI_PAR_16_9, &g_Timing1920x1080_30 },
{ 35, AVI_PAR_4_3, &g_Timing2880x480_60 },
{ 36, AVI_PAR_16_9, &g_Timing2880x480_60 },
{ 37, AVI_PAR_4_3, &g_Timing2880x576_50 },
{ 38, AVI_PAR_16_9, &g_Timing2880x576_50 },
{ 39, AVI_PAR_NODATA, NULL },
{ 40, AVI_PAR_16_9, &g_Timing1920x1080_i_100 },
{ 41, AVI_PAR_16_9, &g_Timing1280x720_100 },
{ 42, AVI_PAR_4_3, &g_Timing720x576_100 },
{ 43, AVI_PAR_16_9, &g_Timing720x576_100 },
{ 44, AVI_PAR_4_3, &g_Timing1440x576_i_100 },
{ 45, AVI_PAR_16_9, &g_Timing1440x576_i_100 },
{ 46, AVI_PAR_16_9, &g_Timing1920x1080_i_120 },
{ 47, AVI_PAR_16_9, &g_Timing1280x720_120 },
{ 48, AVI_PAR_4_3, &g_Timing720x480_120 },
{ 49, AVI_PAR_16_9, &g_Timing720x480_120 },
{ 50, AVI_PAR_4_3, &g_Timing1440x480_i_120 },
{ 51, AVI_PAR_16_9, &g_Timing1440x480_i_120 },
{ 52, AVI_PAR_4_3, &g_Timing720x576_200 },
{ 53, AVI_PAR_16_9, &g_Timing720x576_200 },
{ 54, AVI_PAR_4_3, &g_Timing1440x576_i_200 },
{ 55, AVI_PAR_16_9, &g_Timing1440x576_i_200 },
{ 56, AVI_PAR_4_3, &g_Timing720x480_240 },
{ 57, AVI_PAR_16_9, &g_Timing720x480_240 },
{ 58, AVI_PAR_4_3, &g_Timing1440x480_i_240 },
{ 59, AVI_PAR_16_9, &g_Timing1440x480_i_240 },
{ 60, AVI_PAR_16_9, &g_Timing1280x720_p_24 },
{ 61, AVI_PAR_16_9, &g_Timing1280x720_p_25 },
{ 62, AVI_PAR_16_9, &g_Timing1280x720_p_30 },
{ 63, AVI_PAR_16_9, &g_Timing1920x1080_p_120 },
{ 64, AVI_PAR_16_9, &g_Timing1920x1080_p_100 },
{ 65, AVI_PAR_64_27, &g_Timing1280x720_p_24 },
{ 66, AVI_PAR_64_27, &g_Timing1280x720_p_25 },
{ 67, AVI_PAR_64_27, &g_Timing1280x720_p_30 },
{ 68, AVI_PAR_64_27, &g_Timing1280x720_50 },
{ 69, AVI_PAR_64_27, &g_Timing1280x720_a_60 },
{ 70, AVI_PAR_64_27, &g_Timing1280x720_100 },
{ 71, AVI_PAR_64_27, &g_Timing1280x720_120 },
{ 72, AVI_PAR_64_27, &g_Timing1920x1080_24 },
{ 73, AVI_PAR_64_27, &g_Timing1920x1080_25 },
{ 74, AVI_PAR_64_27, &g_Timing1920x1080_30 },
{ 75, AVI_PAR_64_27, &g_Timing1920x1080_a_50 },
{ 76, AVI_PAR_64_27, &g_Timing1920x1080_a_60 },
{ 77, AVI_PAR_64_27, &g_Timing1920x1080_p_100 },
{ 78, AVI_PAR_64_27, &g_Timing1920x1080_p_120 },
{ 79, AVI_PAR_64_27, &g_Timing1680x720_p_24 },
{ 80, AVI_PAR_64_27, &g_Timing1680x720_p_25 },
{ 81, AVI_PAR_64_27, &g_Timing1680x720_p_30 },
{ 82, AVI_PAR_64_27, &g_Timing1680x720_p_50 },
{ 83, AVI_PAR_64_27, &g_Timing1680x720_p_60 },
{ 84, AVI_PAR_64_27, &g_Timing1680x720_p_100 },
{ 85, AVI_PAR_64_27, &g_Timing1680x720_p_120 },
{ 86, AVI_PAR_64_27, &g_Timing2560x1080_p_24 },
{ 87, AVI_PAR_64_27, &g_Timing2560x1080_p_25 },
{ 88, AVI_PAR_64_27, &g_Timing2560x1080_p_30 },
{ 89, AVI_PAR_64_27, &g_Timing2560x1080_p_50 },
{ 90, AVI_PAR_64_27, &g_Timing2560x1080_p_60 },
{ 91, AVI_PAR_64_27, &g_Timing2560x1080_p_100 },
{ 92, AVI_PAR_64_27, &g_Timing2560x1080_p_120 },
{ 93, AVI_PAR_16_9, &g_Timing3840x2160_p_24 },
{ 94, AVI_PAR_16_9, &g_Timing3840x2160_p_25 },
{ 95, AVI_PAR_16_9, &g_Timing3840x2160_p_30 },
{ 96, AVI_PAR_16_9, &g_Timing3840x2160_p_50 },
{ 97, AVI_PAR_16_9, &g_Timing3840x2160_p_60 },
{ 98, AVI_PAR_NODATA, &g_Timing4096x2160_p_24 },
{ 99, AVI_PAR_NODATA, &g_Timing4096x2160_p_25 },
{ 100, AVI_PAR_NODATA, &g_Timing4096x2160_p_30 },
{ 101, AVI_PAR_NODATA, &g_Timing4096x2160_p_50 },
{ 102, AVI_PAR_NODATA, &g_Timing4096x2160_p_60 },
{ 103, AVI_PAR_64_27, &g_Timing3840x2160_p_24 },
{ 104, AVI_PAR_64_27, &g_Timing3840x2160_p_25 },
{ 105, AVI_PAR_64_27, &g_Timing3840x2160_p_30 },
{ 106, AVI_PAR_64_27, &g_Timing3840x2160_p_50 },
{ 107, AVI_PAR_64_27, &g_Timing3840x2160_p_60 },
{ 108, AVI_PAR_16_9, &g_Timing1280x720_p_48 },
{ 109, AVI_PAR_64_27, &g_Timing1280x720_p_48 },
{ 110, AVI_PAR_64_27, &g_Timing1680x720_p_48 },
{ 111, AVI_PAR_16_9, &g_Timing1920x1080_p_48 },
{ 112, AVI_PAR_64_27, &g_Timing1920x1080_p_48 },
{ 113, AVI_PAR_64_27, &g_Timing2560x1080_p_48 },
{ 114, AVI_PAR_16_9, &g_Timing3840x2160_p_48 },
{ 115, AVI_PAR_NODATA, &g_Timing4096x2160_p_48 },
{ 116, AVI_PAR_64_27, &g_Timing3840x2160_p_48 },
{ 117, AVI_PAR_16_9, &g_Timing3840x2160_p_100 },
{ 118, AVI_PAR_16_9, &g_Timing3840x2160_p_120 },
{ 119, AVI_PAR_64_27, &g_Timing3840x2160_p_100 },
{ 120, AVI_PAR_64_27, &g_Timing3840x2160_p_120 },
{ 121, AVI_PAR_64_27, &g_Timing5120x2160_p_24 },
{ 122, AVI_PAR_64_27, &g_Timing5120x2160_p_25 },
{ 123, AVI_PAR_64_27, &g_Timing5120x2160_p_30 },
{ 124, AVI_PAR_64_27, &g_Timing5120x2160_p_48 },
{ 125, AVI_PAR_64_27, &g_Timing5120x2160_p_50 },
{ 126, AVI_PAR_64_27, &g_Timing5120x2160_p_60 },
{ 127, AVI_PAR_64_27, &g_Timing5120x2160_p_100 },
// Formats 128 to 192 are forbidden
{ 193, AVI_PAR_64_27, &g_Timing5120x2160_p_120 },
{ 194, AVI_PAR_16_9, &g_Timing7680x4320_p_24 },
{ 195, AVI_PAR_16_9, &g_Timing7680x4320_p_25 },
{ 196, AVI_PAR_16_9, &g_Timing7680x4320_p_30 },
{ 197, AVI_PAR_16_9, &g_Timing7680x4320_p_48 },
{ 198, AVI_PAR_16_9, &g_Timing7680x4320_p_50 },
{ 199, AVI_PAR_16_9, &g_Timing7680x4320_p_60 },
{ 200, AVI_PAR_16_9, &g_Timing7680x4320_p_100 },
{ 201, AVI_PAR_16_9, &g_Timing7680x4320_p_120 },
{ 202, AVI_PAR_64_27, &g_Timing7680x4320_p_24 },
{ 203, AVI_PAR_64_27, &g_Timing7680x4320_p_25 },
{ 204, AVI_PAR_64_27, &g_Timing7680x4320_p_30 },
{ 205, AVI_PAR_64_27, &g_Timing7680x4320_p_48 },
{ 206, AVI_PAR_64_27, &g_Timing7680x4320_p_50 },
{ 207, AVI_PAR_64_27, &g_Timing7680x4320_p_60 },
{ 208, AVI_PAR_64_27, &g_Timing7680x4320_p_100 },
{ 209, AVI_PAR_64_27, &g_Timing7680x4320_p_120 },
{ 210, AVI_PAR_64_27, &g_Timing10240x4320_p_24 },
{ 211, AVI_PAR_64_27, &g_Timing10240x4320_p_25 },
{ 212, AVI_PAR_64_27, &g_Timing10240x4320_p_30 },
{ 213, AVI_PAR_64_27, &g_Timing10240x4320_p_48 },
{ 214, AVI_PAR_64_27, &g_Timing10240x4320_p_50 },
{ 215, AVI_PAR_64_27, &g_Timing10240x4320_p_60 },
{ 216, AVI_PAR_64_27, &g_Timing10240x4320_p_100 },
{ 217, AVI_PAR_64_27, &g_Timing10240x4320_p_120 },
{ 218, AVI_PAR_NODATA, &g_Timing4096x2160_p_100 },
{ 219, AVI_PAR_NODATA, &g_Timing4096x2160_p_120 },

};
DDU32 g_ulTotalDisplayID_CEA_modes = sizeof(g_DisplayID_CeStandardTiming) / sizeof(g_DisplayID_CeStandardTiming[0]);

/////////////////////////////////////////////////////////////////////////
//
//  Aspect Ratio table for EDID and DIsplay ID
//
/////////////////////////////////////////////////////////////////////////
