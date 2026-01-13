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
 * @file  DisplayDefs.h
 * @ingroup driver
 * @brief Defines all basic types used inside display.
 *
 */
#pragma once

#ifndef _DD_DEF_
#define _DD_DEF_

///////////////////////////////////////////////////////////////////////////////
//
// Windows definitions
//

#include <stdint.h>
#include <stdbool.h>
#include <guiddef.h> // This needs to be redefined for Linux

//-----------------------------------------------------------------------------
// basic data types
//

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

// arg types
#define DD_IN IN
#define DD_OUT OUT
#define DD_IN_OUT

typedef int8_t DDS8;    // signed 8 bit data type
typedef uint8_t DDU8;   // unsigned 8 bit data type
typedef int16_t DDS16;  // signed 16 bit data type
typedef uint16_t DDU16; // unsigned 16 bit data type
typedef int32_t DDS32;  // signed 32 bit data type
typedef uint32_t DDU32; // unsigned 32 bit data type
typedef int64_t DDS64;  // signed 64 bit data type
typedef uint64_t DDU64; // unsigned 64 bit data type
typedef wchar_t WCHAR;  // Wide string character
typedef wchar_t DDWSTR; // Wide string character
typedef char CHAR;

typedef uintptr_t DDU64_PTR;

// For LLVM builds use bool so that wrong DD_BOOL assignments are caught
#if (_DISP_LLVM_BUILD)
typedef bool DD_BOOL;
#else
typedef unsigned char DD_BOOL;
#endif

// for strings

typedef CHAR *PDDSTR;
typedef WCHAR *PDDWSTR;
typedef const CHAR *PDDCSTR;
typedef const WCHAR *PDDCWSTR;

typedef void *HANDLE;

#define DD_INLINE __inline
#define DD_S_INLINE static __inline

// data type value range limits
#define DDMINS8 INT8_MIN
#define DDMAXS8 INT8_MAX
#define DDMINS16 INT16_MIN
#define DDMAXS16 INT16_MAX
#define DDMINS32 INT32_MIN
#define DDMAXS32 INT32_MAX
#define DDMAXU8 UINT8_MAX
#define DDMAXU16 UINT16_MAX
#define DDMAXU32 UINT32_MAX
#define DDMAXU64_PTR UINT64_MAX
#define DDMAXU64 UINT64_MAX

#if (defined(_DISPLAY_INTERNAL_) && !defined(_DEBUG_EXPORT_))

// These are windows specific defines.
// Might have to redefine these or somehow confine these to Windows specific header includes

// Defined values for the exception filter expression
#define EXCEPTION_EXECUTE_HANDLER 1
#define EXCEPTION_CONTINUE_SEARCH 0
#define EXCEPTION_CONTINUE_EXECUTION (-1)

#define PASSIVE_LEVEL 0  // Passive release level
#define LOW_LEVEL 0      // Lowest interrupt level
#define APC_LEVEL 1      // APC interrupt level
#define DISPATCH_LEVEL 2 // Dispatcher level
#define CMCI_LEVEL 5     // CMCI handler level

#define CLOCK_LEVEL 13   // Interval clock level
#define IPI_LEVEL 14     // Interprocessor interrupt level
#define DRS_LEVEL 14     // Deferred Recovery Service level
#define POWER_LEVEL 14   // Power failure level
#define PROFILE_LEVEL 15 // timer used for profiling.
#define HIGH_LEVEL 15    // Highest interrupt level

#ifndef FALSE
#define FALSE 0 // To match ntdef.h
#endif

#ifndef TRUE
#define TRUE 1 // To match ntdef.h
#endif

#ifndef C_ASSERT
#define C_ASSERT(e) // static_assert(e, "")
#endif

#define ANYSIZE_ARRAY 1

#endif // _DISPLAY_INTERNAL_

///////////////////////////////////////////////////////////////////////////////
//
// Common user defined data types
//
///////////////////////////////////////////////////////////////////////////////

#define DD_IS_NULL(pArg) (NULL == (void *)pArg)
#define DD_IS_NOT_NULL(pArg) (NULL != (void *)pArg)

typedef enum _DD_REQUEST
{
    DD_REQUEST_UNKNOWN = 0,
    DD_REQUEST_GET,
    DD_REQUEST_SET,
} DD_REQUEST;

typedef enum _DD_ENABLE_DISABLE
{
    DD_ACTION_UNKNOWN = 0,
    DD_ENABLE,
    DD_DISABLE
} DD_ENABLE_DISABLE;

typedef union _DD_LARGE_INTEGER
{
    struct
    {
        DDU32 LowPart;
        DDS32 HighPart;
    };
    DDS64 QuadPart;
} DD_LARGE_INTEGER;

#define MAX_8BIT_NUM 256
#define MAX_10BIT_NUM 1023
#define MAX_10BIT_VALUE 1023.0f
#define MAX_16BIT_NUM 65536
#define MAX_16BIT_VALUE 65535.0f
#define MAX_24BIT_NUM 16777216
#define MAX_24BIT_VALUE 16777215
#define MAX_32BIT_VALUE 4294967295

#define UNITY_PIXEL_BOOST_FACTOR (1 * MAX_24BIT_VALUE)
#define GAMMA_UNITY 16777216 // 1 << 24

#define MAX_RELATIVE_LUT_VALUE MAX_24BIT_VALUE // this is assuming all correction LUTs are 8.24 format
#define MAX_PARS_POSSIBLE_WITH_1_VIC 2

#define CONVERT_16BIT_TO_8_24BIT(InputIn16Bit) ((DDU32)((((double)InputIn16Bit) / MAX_16BIT_VALUE) * MAX_24BIT_NUM))
#define CONVERT_8_24BIT_TO_8_16BIT(InputIn32Bit) (DD_MIN(InputIn32Bit, MAX_24BIT_NUM) >> 8)   // HW format 8.16
#define CONVERT_8_24BIT_TO_3_16BIT(InputIn32Bit) (DD_MIN(InputIn32Bit, GAMMA_UNITY) >> 8)     // HW format 3.16 i.e 512,513 and 514
#define CONVERT_8_24BIT_TO_0_16BIT(InputIn32Bit) (DD_MIN(InputIn32Bit, GAMMA_UNITY - 1) >> 8) // HW format 0.16 i.e 0 to 511 entries

#define CLIPVALUE(value, lower, higher) ((value < lower) ? lower : ((value > higher) ? higher : value))

#define TWOS_TO_SIGNED_MAGNITUDE(a) (a < 0) ? (~a + 1) : a // for -ve cursor positions compute 2's complement to get the +ve offset and then set the signed bit for HW
#define TWOS_TO_SIGNED_SIGN(a) (a < 0) ? 1 : 0             // for -ve cursor positions compute 2's complement to get the +ve offset and then set the signed bit for HW

#define ACCURACY_MULTIPLIER_100 100
#define RATIO_WITH_ACCURACY_MULTIPLIER(a, b, Accuracy) ((a * Accuracy) / b)
#define MAKE_EVEN(a) (((a & BIT0) == 0) ? a : (a + 1))

// Get frame delay in micro sec from refresh rate
#define GET_FRAME_DELAY_IN_US(RefreshRate) ((RefreshRate > 0) ? ((1000 * 1000) / RefreshRate) : 33333) // delay is for 30Hz if input RR = 0

// ========================================================
//  Timer related struct
// ========================================================

// Structure used to measure time delatas
typedef struct _DD_ELAPSED_TIME
{
    DDU32 StartTime;
    DDU32 ElapsedTime;
} DD_ELAPSED_TIME;

typedef enum _DD_TIMER_TYPE
{
    NOTIFICATION,
    SYNCHRONIZATION,
} DD_TIMER_TYPE;

typedef struct _DD_PERIODIC_TIMER_OBJECT
{
    void *pKDPC;
    void *pKTIMER;
    void *pExTimerParams;
} DD_PERIODIC_TIMER_OBJECT;

///////////////////////////////////////////////////////////////////////////////

#if defined(_DISPLAY_INTERNAL_) || defined(_DEBUG_EXPORT_) // ndef BIT0 // reusing SB defn

///////////////////////////////////////////////////////////////////////////////
//
// Bit definition
// Bit manipulation macros
//
///////////////////////////////////////////////////////////////////////////////
#define __BIT(x) (1UL << (x)) // internal to this file
#define BIT64(x) ((DDU64)1 << (x))

#define BIT0 __BIT(0)
#define BIT1 __BIT(1)
#define BIT2 __BIT(2)
#define BIT3 __BIT(3)
#define BIT4 __BIT(4)
#define BIT5 __BIT(5)
#define BIT6 __BIT(6)
#define BIT7 __BIT(7)
#define BIT8 __BIT(8)
#define BIT9 __BIT(9)
#define BIT10 __BIT(10)
#define BIT11 __BIT(11)
#define BIT12 __BIT(12)
#define BIT13 __BIT(13)
#define BIT14 __BIT(14)
#define BIT15 __BIT(15)
#define BIT16 __BIT(16)
#define BIT17 __BIT(17)
#define BIT18 __BIT(18)
#define BIT19 __BIT(19)
#define BIT20 __BIT(20)
#define BIT21 __BIT(21)
#define BIT22 __BIT(22)
#define BIT23 __BIT(23)
#define BIT24 __BIT(24)
#define BIT25 __BIT(25)
#define BIT26 __BIT(26)
#define BIT27 __BIT(27)
#define BIT28 __BIT(28)
#define BIT29 __BIT(29)
#define BIT30 __BIT(30)
#define BIT31 __BIT(31)
#endif // _DISPLAY_INTERNAL_ or _DEBUG_EXPORT_

//-----------------------------------------------------------------------------
// To set n bits we need to do (2 ^ n) - 1
// Eg: To set 3 bits, we need to do (2 ^ 3) - 1 --> 8 - 1 = 7 --> b111
//-----------------------------------------------------------------------------
#define DD_SET_N_BITS(n) ((DDU32)(1 << (n)) - 1)

//-----------------------------------------------------------------------------
// Macro  to generate set of 1's given
// a range
// Bit mask
//-----------------------------------------------------------------------------
#define DD_BITRANGE_MASK(ulHighBit, ulLowBit) ((DD_SET_N_BITS(ulHighBit) | (1 << ulHighBit)) & (~((DDU32)(1 << ulLowBit) - 1)))

//-----------------------------------------------------------------------------
// Macro: DD_BITFIELD_RANGE
// PURPOSE: Calculates the number of bits between the startbit and the endbit
// and count is inclusive of both bits. The bits are 0 based.
//-----------------------------------------------------------------------------
#define DD_BITFIELD_RANGE(ulLowBit, ulHighBit) ((ulHighBit) - (ulLowBit) + 1)

//-----------------------------------------------------------------------------
// Macro: DD_BITFIELD_BIT
// PURPOSE: Used to declare single bit width
//-----------------------------------------------------------------------------
#define DD_BITFIELD_BIT(bit) 1

// size in multiple of DDU32
#define DD_SIZE32(x) ((DDU32)(sizeof(x) / sizeof(DDU32)))

#define DD_IS_ALIGN(A, B) (((A) % (B)) == 0)

#define DD_SET_BIT(A, b) (A |= __BIT(b))
#define DD_CLEAR_BIT(A, b) (A &= ~__BIT(b))
#define DD_IS_BIT_SET(A, b) ((A & __BIT(b)) != 0)
#define DD_IS_BIT_CLEAR(A, b) ((A & __BIT(b)) == 0)
#define DD_IS_EVEN(A) DD_IS_BIT_CLEAR(A, 0)
#define DD_IS_ODD(A) DD_IS_BIT_SET(A, 0)
#define DD_IS_MASK_SET(A, mask) ((A & mask) == mask)
#define DD_IS_MASK_CLEAR(A, mask) ((((DDU32)A) | ~((DDU32)mask)) == ~((DDU32)mask))

#define DD_ROUND_TO_4K(LENGTH) (((DDU32)(LENGTH) + 0x1000 - 1) & ~(0x1000 - 1))
#define DD_DIVIDE_BY_4K(Value) ((Value) >> 12)
#define DD_MULT_BY_4K(Value) ((Value) << 12)
#define DD_IS_4K_ALIGNED(Value) ((Value & 0xfffff000) == Value)
#define DD_IS_ONLY_BIT_SET(A) ((A != 0) && ((A & (A - 1)) == 0))
#define DD_IS_POWER_OF_2(A) ((A & (A - 1)) ? FALSE : TRUE)

// Macros for quick use
#define DD_GET_BYTE(dwValue, nStartPos) ((dwValue >> nStartPos) & 0xFF)
#define DD_GET_N_BITS(dwValue, highbit, lowbit) ((dwValue << (31 - (highbit))) >> (31 - ((highbit) + (lowbit))))
// This macro will convert between little/big endian
#define DD_SWAP_ENDIAN(dwValue) ((DD_GET_BYTE(dwValue, 0) << 24) | (DD_GET_BYTE(dwValue, 8) << 16) | (DD_GET_BYTE(dwValue, 16) << 8) | (DD_GET_BYTE(dwValue, 24)))
#define DD_VOID_PTR_INC(p, n) ((void *)((char *)(p) + (n)))
#define DD_VOID_PTR_DEC(p, n) ((void *)((char *)(p) - (n)))
#define DD_OFFSETOF(str, memVar) (DDU64_PTR) & (((str *)0)->memVar) // Macro to get relative offset of a member variable in a stucture
#define DD_CONTAINING_RECORD(address, type, field) ((type *)((DDU8 *)(address) - (DDU64_PTR)(&((type *)0)->field)))

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// All standard macros for primitive operations go here:
//
///////////////////////////////////////////////////////////////////////////////

#define DD_MIN(a, b) ((a) < (b) ? (a) : (b))
#define DD_MAX(a, b) ((a) < (b) ? (b) : (a))
#define DD_CLAMP_MIN_MAX(a, min, max) ((a) < (min) ? (min) : DD_MIN((a), (max)))
#define DD_ROUND_UP_DIV(x, y) (((x) % (y) == 0) ? ((x) / (y)) : (((x) / (y)) + 1))
#define DD_ROUND_DIV(x, y) (((x) + (y) / 2) / (y))
#define DD_ROUND_DOWN_DIV(x, y) ((x) / (y))
#define DD_ROUND(p, q) ((2 * (p) + (q)) / (2 * (q))) // used for rounding to nearest integer when arguments are of integer type
#define DD_ROUNDTONEARESTINT(p) (int)(p + 0.5)
#define DD_ABS(x) ((x) < 0 ? -(x) : (x))
#define DD_DIFF(a, b) (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))
#define DD_GET_ELAPSED_COUNT(curr, last, count_max) (((curr) >= (last)) ? ((curr) - (last)) : ((count_max) - (last) + (curr) + 1))
#define DD_SWAP(X, Y) \
    {                 \
        (X) ^= (Y);   \
        (Y) ^= (X);   \
        (X) ^= (Y);   \
    }
#define DD_IS_EQUAL_WITH_TOLERANCE(a, b, t) (DD_DIFF(a, b) <= t)
#define DD_IS_IN_RANGE(x, min, max) (((x) >= (min)) && ((x) <= (max)))

/**
 * @brief Macro to get twos compliment of a DDU16 variable.
 **/
#define DD_TWOS_COMPLEMENT(x) (DDU8)((~(x) + 1) & 0x3F)

/**
 * @brief Macro to swap two bytes of a DDU16 variable.
 **/
#define DD_DDU16_SWAP_ENDIAN(Value) ((DD_GET_BYTE(Value, 0) << 8) | (DD_GET_BYTE(Value, 8)))
#define DD_MAKEDDU32(Byte1, Byte2, Byte3, Byte4) (Byte1 | Byte2 << 8 | Byte3 << 16 | Byte4 << 24)

// This macro swaps endian and returns the value with desired byte size
#define DD_RELOCATE_BYTE(pValue, returnSize, byteIndex, bitsToShift) (((returnSize) >= (byteIndex)) ? (pValue[returnSize - byteIndex] << bitsToShift) : 0)
#define DD_DDU32_SWAP_ENDIAN(pValue, returnSize) \
    (DD_RELOCATE_BYTE(pValue, returnSize, 4, 24) | DD_RELOCATE_BYTE(pValue, returnSize, 3, 16) | DD_RELOCATE_BYTE(pValue, returnSize, 2, 8) | DD_RELOCATE_BYTE(pValue, returnSize, 1, 0))

// Required for CVT Timing Calculation
#define DD_ROUNDTO3DECIMALS(z) (float)(DD_ROUNDTONEARESTINT((z)*1000)) / 1000
#define DD_CHECK_ASPECTRATIO(x, CellGran, Vpixels) (DDU32)(CellGran * ((DDU32)((Vpixels * x)) / CellGran))

#define DD_ALIGN(x, a) (((x) + ((a)-1)) - (((x) + ((a)-1)) & ((a)-1))) // Alt implementation
// with bitwise not (~) has issue with uint32 align used with 64-bit value, since ~'ed value will remain 32-bit.

/**
 * @brief Macro to get HDCP stream type.
 **/
#define HDCP_STREAM_TYPE_0 0
#define HDCP_STREAM_TYPE_1 1
#define DD_GET_HDCP_STREAM_TYPE(OpmLevel) ((OpmLevel <= 1) ? HDCP_STREAM_TYPE_0 : HDCP_STREAM_TYPE_1)
//-----------------------------------------------------------------------------
// unit conversion macros
//-----------------------------------------------------------------------------
#define DD_KILO(a) ((a)*1024)
#define DD_MEGA(a) ((a)*1024 * 1024)
#define DD_GIGA(a) (((DDU64)1024) * 1024 * 1024 * (a))
#define DD_BYTES_TO_MB_ROUND_UP(a) (DDU64)(DD_ROUND_UP_DIV(a, (1024 * 1024)))
#define DD_BYTES_TO_MB_ROUND_DOWN(a) (DDU64)(DD_ROUND_DOWN_DIV(a, (1024 * 1024)))
#define DD_MILLI_HZ_TO_VMIN(DotClockInHz, HTotal, RrInHz1000) (DDU32)(DD_ROUND_UP_DIV(((DotClockInHz)*DD_1K), ((DDU64)(HTotal) * (DDU64)(RrInHz1000))))
#define DD_MILLI_HZ_TO_VMAX_FRACTIONAL(DotClockInHz, HTotal, RrInHz1000) (DDU32)(DD_ROUND_DOWN_DIV(((DotClockInHz)*1001), ((DDU64)(HTotal) * (DDU64)(RrInHz1000))))

// Line time is programmed in units of 0.125us. Calculate the total number of units by multiplying the line time by 8, and then take the nearest integer.
// Multiply the resulted integer units by 125ns to get the precise line time.
#define DD_GET_LINE_TIME_UNITS(HTotal, DotClockInHz) ((DDU32)(DD_ROUND_UP_DIV(((DDU64)8 * (HTotal)*DD_1K), ((DotClockInHz) / DD_1K))))
#define DD_GET_LINE_TIME_IN_NS(HTotal, DotClockInHz) ((DDU32)(DD_GET_LINE_TIME_UNITS(HTotal, DotClockInHz) * DD_LINE_TIME_UNIT_SIZE_IN_NS))
#define DD_GET_LINE_TIME_IN_US(HTotal, DotClockInHz) DD_ROUND_UP_DIV(DD_GET_LINE_TIME_IN_NS(HTotal, DotClockInHz), DD_1K)

#define DD_GET_ADJUSTED_DOT_CLOCK(DotClockInHz, PipeDownScalingFactor) ((((DotClockInHz) * ((PipeDownScalingFactor) / DD_1K)) / DD_1K));

#define DD_GET_FRAMETIME_IN_US(HTotal, VTotal, DotClockInHz) (DD_ROUND_UP_DIV(((DDU64)HTotal * VTotal * DD_MILLION), DotClockInHz))

// Macros added for TEN and Mega
// Didnt use the name MEGA for the second macro since its already defined in CSLBase.c with value 1048576
#define TEN 10
#define KILO_HERTZ 1000
#define MEGA_HERTZ 1000000
#define GIGA_HERTZ 1000000000
#define MAX_VTOTAL_WITHOUT_VRR_MODE 8192

//-----------------------------------------------------------------------------
// Precision related macros
//-----------------------------------------------------------------------------
#define DD_U8_MASK 0x000000FF
#define DD_U16_MASK 0x0000FFFF
#define DD_U32_MASK 0xFFFFFFFF
#define LSB(value) (value & DD_U8_MASK) // Least Significant Byte

//-----------------------------------------------------------------------------
// Precision related macros
//-----------------------------------------------------------------------------
#define DD_PRECISION2DEC 100                                         // multiply the floating point no to get 2 places after decimal
#define DD_PRECISION3DEC 1000                                        // multiply the floating point no to get 3 places after decimal
#define DD_PRECISION4DEC 10000                                       // multiply the floating point no to get 4 places after decimal
#define DD_UNIT_TO_MILLI_UNIT(a) ((a)*1000)                          // Generic macro to convert any unit to milli unit.
#define DD_MILLI_UNIT_TO_UNIT(a) DD_ROUND_DIV(a, 1000)               // Generic macro to convert any milliunit to unit
#define DD_MEGA_UNIT_TO_UNIT_U64(a) (((DDU64)(a)) * MEGA_HERTZ)      // Generic Macro to convert Mega unit to Unit stored as DDU64
#define DD_MEGA_UNIT_TO_KILO_UNIT_U64(a) (((DDU64)(a)) * KILO_HERTZ) // Generic Macro to convert Mega unit to Kilo Unit stored as DDU64
#define DD_LINE_TIME_UNIT_SIZE_IN_NS 125                             // line time is programmed in units of 0.125us.

//-----------------------------------------------------------------------------
// Time related macros
//-----------------------------------------------------------------------------
#define HDCP_100MS_DELAY 100000
#define HDCP_5MS_DELAY 5000
#define DD_SEC_TO_MILLI_SEC 1000                                            // Milli seconds in a Second
#define DD_MILLI_TO_MICRO_SEC 1000                                          // Micro seconds in a milli Second
#define DD_MICRO_TO_100_NANO_SEC 10                                         // 100 Nano seconds in a micro Second
#define DD_100_MICRO_TO_MICRO_SEC 100                                       // 100us units to micro second unit
#define DD_SEC_TO_MICRO_SEC (DD_SEC_TO_MILLI_SEC * DD_MILLI_TO_MICRO_SEC)   // Micro seconds in a Second
#define DD_MICRO_TO_NANO_SEC 1000                                           // Nano to micro sec
#define DD_MILLI_TO_NANO_SEC (DD_MILLI_TO_MICRO_SEC * DD_MICRO_TO_NANO_SEC) // Nano seconds in a milli Second
#define DD_100NS_TO_MS (DD_MICRO_TO_100_NANO_SEC * DD_MILLI_TO_MICRO_SEC)   // Milli seconds in a 100 nano second unit
#define DD_SEC_TO_NANO_SEC (DD_SEC_TO_MICRO_SEC * DD_MICRO_TO_NANO_SEC)     // Nano seconds in a Second

#define DD_MS_TO_HUNDRED_NS(n) (10 * 1000 * (n))         // Convert from ms to 100ns units
#define DD_S_TO_HUNDRED_NS(n) (10 * 1000 * 1000 * (n))   // Convert from sec to 100ns unit
#define DD_HUNDRED_NS_TO_S(n) ((n) / (10 * 1000 * 1000)) // Convert from 100ns units to s
#define DD_GET_TICKS_ELAPSED(_st, _end) ((((DDU64)(_end)) >= ((DDU64)(_st))) ? (((DDU64)(_end)) - ((DDU64)(_st))) : ((DDMAXU64) - ((DDU64)(_st)) + ((DDU64)(_end)) + 1))
#define DD_GET_DDU32_TIME_ELAPSED(_st, _end) ((((DDU32)(_end)) >= ((DDU32)(_st))) ? (((DDU32)(_end)) - ((DDU32)(_st))) : ((DDMAXU32) - ((DDU32)(_st)) + ((DDU32)(_end)) + 1))
#define DD_PRECISE_TICKS_TO_MICRO_SEC(Ticks, Freq) ((((DDU64)(Ticks) / Freq) * DD_SEC_TO_MICRO_SEC) + (((DDU64)(Ticks % Freq) * DD_SEC_TO_MICRO_SEC) / Freq))
#define DD_TICKS_TO_MICRO_SEC(Ticks, Freq) ((Freq == DD_S_TO_HUNDRED_NS(1)) ? (Ticks / DD_MICRO_TO_100_NANO_SEC) : DD_PRECISE_TICKS_TO_MICRO_SEC(Ticks, Freq))
#define DD_PRECISE_MICRO_SEC_TO_TICKS(TimeInUs, Freq) ((((DDU64)(TimeInUs) / DD_SEC_TO_MICRO_SEC) * Freq) + (((DDU64)(TimeInUs % DD_SEC_TO_MICRO_SEC) * Freq) / DD_SEC_TO_MICRO_SEC))
#define DD_MICRO_SEC_TO_TICKS(TimeInUs, Freq) ((Freq == DD_S_TO_HUNDRED_NS(1)) ? (TimeInUs * DD_MICRO_TO_100_NANO_SEC) : DD_PRECISE_MICRO_SEC_TO_TICKS(TimeInUs, Freq))
#define DD_GET_ELAPSED_TIME(_st, _end) (((DDU64)(_end)) - ((DDU64)(_st)))

// Memory operations

// Indicate 256MB as a requirement for GMM to allocate framebuffer during driver disable/BSOD screen
#define MAX_DISPLAY_FRAME_BUFFER (256 * 1024 * 1024)

// memcpy_s source to destination
// copy count bytes from src to dest if destsize is less than count
#define DD_MEM_COPY_SAFE(dest, destsize, src, count) memcpy_s((dest), (destsize), (src), (count))

// Zero out dest memory
#define DD_ZERO_MEM(dest, size) memset(dest, 0, size)

#define DD_CENT 100
#define DD_1K 1000
#define DD_MILLION 1000000 // Multiply by 10^6 to conver to Parts Per Million
#define DD_BILLION 1000000000ull

// compare memory
#define DD_MEM_CMP(bufr1, bufr2, buf_size) memcmp((DDU8 *)(bufr1), (DDU8 *)(bufr2), (buf_size))

#define STRING2(x) #x
#define DD_STRINGIFY(x) STRING2(x)
#define DD_LSHIFT_DDU32(Data, NumPlaces) ((Data << NumPlaces) & DD_U32_MASK)
// This macro is used to check timeouts during authentication
#define DD_IS_TIMED_OUT(startTime, endTime, timeout) (((endTime) > ((startTime) + (DDS64)(timeout))) ? TRUE : FALSE)

// Circular buffer related Macros

#define DD_CB_GET_NUM_ENTRIES(Head, Tail, CbSize) \
    (((Tail) >= (Head)) ? ((Tail) - (Head)) : ((CbSize) + (Tail) - (Head))) // Caution: Tail is considered the leading Pointer of the Circular buffer in this definition
#define DD_CB_64BIT_IS_FUTURE_TIME(CurrentTimeInUs, TargetTimeInUs) (((DDS64)((TargetTimeInUs) - (CurrentTimeInUs)) > 0) ? TRUE : FALSE) // using signed type cast to handle timestamp rollover case.
#define DD_CB_32BIT_IS_FUTURE_TIME(CurrentTimeInUs, TargetTimeInUs) (((DDS32)((TargetTimeInUs) - (CurrentTimeInUs)) > 0) ? TRUE : FALSE) // using signed type cast to handle timestamp rollover case.

///////////////////////////////////////////////////////////////////////////////
//
// inline functions
//
DD_S_INLINE DDU32 DDPtrToUlong(const void *p)
{
    return ((DDU32)(DDU64_PTR)p);
}

DD_INLINE DDU64 DD_FindGCD(DDU64 Number1, DDU64 Number2)
{
    DDU64 ulLargerNumber, ulSmallerNumber = 0, ulRemainder = 0;

    if (Number1 > Number2)
    {
        ulLargerNumber  = Number1;
        ulSmallerNumber = Number2;
    }
    else
    {
        ulLargerNumber  = Number2;
        ulSmallerNumber = Number1;
    }

    while (1)
    {
        if (ulSmallerNumber == 0)
            break; // We have got the LCD

        ulRemainder     = ulLargerNumber % ulSmallerNumber;
        ulLargerNumber  = ulSmallerNumber;
        ulSmallerNumber = ulRemainder;
    }

    return ulLargerNumber;
}

DD_INLINE DDU64 DD_FindLCM(DDU64 Number1, DDU64 Number2)
{
    DDU64 LCMResult = 0, Product = 0, GCDResult = 0;

    Product   = Number1 * Number2; // Not expecting any large input numbers in this function. Hence keeping DDU64 for product value too.
    GCDResult = DD_FindGCD(Number1, Number2);

    // LCM x GCD = Num1 X Num2 (Product of the numbers)
    LCMResult = Product / GCDResult;

    return LCMResult;
}

DD_INLINE DDU32 DD_Get_ThreadSleepTime(DDU32 TimeIncrement, DDU32 RefreshRate)
{
    DDU32 ThreadSleepTime, FrameDrawTime;

    // Thread wake happens during Tick event
    // Hence minimum possible wake time is in next tick
    ThreadSleepTime = TimeIncrement / 10;

    if (0 != RefreshRate)
    {
        // 1 sec/RR we want the value micro sec units
        FrameDrawTime = 1 * (1000 * 1000) / (RefreshRate);

        // Thread sleep should be for the longest time between 1/RR and Tick time increment
        // check for the smallest value is done since we are comparing negative values
        if (FrameDrawTime > ThreadSleepTime)
        {
            ThreadSleepTime = FrameDrawTime;
        }
    }

    return ThreadSleepTime;
}

DD_INLINE DDU32 DD_Num_SetBits(DDU32 Input)
{
    DDU32 Count = 0;
    while (Input)
    {
        if (Input & 1)
        {
            Count++;
        }
        Input >>= 1;
    }
    return Count;
}

DD_INLINE DDU32 DD_Scale(DDU32 SourceVal, DDU32 SourceMin, DDU32 SourceMax, DDU32 TargetMin, DDU32 TargetMax)
{
    DDU32 TargetVal;

    if ((TargetMin > TargetMax) || ((SourceMax - SourceMin) == 0))
    {
        return 0;
    }

    /* defensive */
    SourceVal = DD_CLAMP_MIN_MAX(SourceVal, SourceMin, SourceMax);

    /* avoid overflows */
    TargetVal = DD_ROUND_UP_DIV((DDU64)(SourceVal - SourceMin) * (TargetMax - TargetMin), SourceMax - SourceMin);
    TargetVal += TargetMin;

    return TargetVal;
}

/*
 * Converts a Big Endian Byte Array of length SrcSize to Little Endian Dword
 * Sample Inputs             | Output
 * {0x12, 0x34, 0x56, 0x78}  | 0x12345678
 * {0x12, 0x34, 0x56}        | 0x00123456
 * {0x12, 0x34}              | 0x00001234
 * {0x12}                    | 0x00000012
 */
DD_INLINE void DDU8ArrayToDDU32WithEndianSwap(DDU8 *pSource, DDU32 SrcSize, DDU32 *pDestValue)
{
    DDU8 *pDest = (DDU8 *)(pDestValue);
    if ((NULL != pSource) && (SrcSize != 0) && (SrcSize <= sizeof(DDU32)) && (NULL != pDest))
    {
        *pDestValue = 0;
        for (DDU8 index = 0; index < SrcSize; index++)
        {
            pDest[index] = pSource[SrcSize - index - 1];
        }
    }
}

/*
 * Converts Little endian Dword (Value) to Big Endian Byte Array of length DstSize
 * Sample Input = 0x12345678
 * Length | Output Array
 *    4   | {0x12, 0x34, 0x56, 0x78}
 *    3   | {0x34, 0x56, 0x78}
 *    2   | {0x56, 0x78}
 *    1   | {0x78}
 */
DD_INLINE void DDU32ToDDU8ArrayWithEndianSwap(DDU8 *pDest, DDU32 DstSize, DDU32 Value)
{
    DDU8 *pSource = (DDU8 *)(&Value);
    if ((NULL != pDest) && (DstSize != 0) && (DstSize <= sizeof(DDU32)))
    {
        for (DDU8 index = 0; index < DstSize; index++)
        {
            pDest[DstSize - index - 1] = pSource[index];
        }
    }
}

/*
 * Converts a Byte Array of length SrcSize to Dword (Return value)
 * Sample Inputs             | Output
 * {0x12, 0x34, 0x56, 0x78}  | 0x78563412
 * {0x12, 0x34, 0x56}        | 0x00563412
 * {0x12, 0x34}              | 0x00003412
 * {0x12}                    | 0x00000012
 */
DD_INLINE DDU32 DDU8ArrayToDDU32(DDU8 *pSource, DDU8 SrcSize)
{
    DDU32 Dest = 0;
    DD_MEM_COPY_SAFE(&Dest, sizeof(DDU32), pSource, DD_MIN(SrcSize, sizeof(DDU32)));
    return Dest;
}

/*
 * Converts Dword (Value) to Byte array of Length DstSize
 * Sample Input = 0x12345678
 * Length | Output Array
 *    4   | {0x78, 0x56, 0x34, 0x12}
 *    3   | {0x78, 0x56, 0x34}
 *    2   | {0x78, 0x56}
 *    1   | {0x78}
 */
DD_INLINE void DDU32ToDDU8Array(DDU8 *pDest, DDU8 DestSize, DDU32 Value)
{
    DD_MEM_COPY_SAFE(pDest, DestSize, (DDU8 *)(&Value), DestSize);
}

/**
 * @brief Method to reverse Endianness of an array.
 *
 * @param pData
 * @param DataSize
 * @return void
 */
DD_INLINE void DDU8ArraySwapEndian(DDU8 *pData, DDU32 DataSize)
{
    DDU32 DestIndex, SourceIndex;
    DDU8 PlaceHolder;
    for (DestIndex = 0, SourceIndex = (DataSize - 1); DestIndex < SourceIndex; DestIndex++, SourceIndex--)
    {
        PlaceHolder        = pData[DestIndex];
        pData[DestIndex]   = pData[SourceIndex];
        pData[SourceIndex] = PlaceHolder;
    }
}

// Half Precision Float /FP16 Representation
typedef union _DD_FP16
{
    DDU16 Value;
    struct
    {
        DDU16 Mantissa : 10;
        DDU16 Exponent : 5;
        DDU16 Sign : 1;
    };
} DD_FP16;

#endif // _DD_DEF_
