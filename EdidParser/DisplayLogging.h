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
 * @file  DisplayLogging.h
 * @brief DisplayLogging.h
 *
 */

#pragma once

#ifdef _YANGRA_DISPLAY_ // Covers the projects KMD Static, DisplayHAL, DisplayOSL, DisplayProtocol
#ifndef _DISP_UTF_      // Not to be used in UTF
// Include the UMD to KMD wrapper and necessary wrapper definitions

#if (_DISP_LLVM_BUILD || _DEBUG || _DISPLAY_EXPORT_ || _DEBUG_EXPORT_)
#define EVENT_MSG_CRITICALITY 4 // Temp define, need to correct later
#include "IntelGfxDisplayUnifiedLog.h"
#else

#ifndef BOOL
typedef int BOOL; // From minwindef.h. This is needed as evntrace.h added this and we are not getting the defines.
#endif

#include "EtwKmdToUmdWrapper.h"
#include "IntelGfxDisplay.h"
#include "IntelGfxDisplayETWLog.h"
#endif

#else                           // A Display KMD file is getting compild in the UTF project. Convert the ETW events into Debug message
#define EVENT_MSG_CRITICALITY 4 // Temp define, need to correct later
#define SORTPP_PASS
#include <windows.h>
#include <tchar.h>

#include "IntelGfxDisplay.h"
#include "IntelGfxDisplayETWLog.h"
#include "IntelGfxETWCrimson.h"
#endif

#ifdef __MINIPORT
#ifndef _DISP_UTF_

typedef struct _DD_ETW_SESSION_DATA
{
    DDU8 Level;
    DDU32 ControlCode;
    DDU64 MatchAnyKeyword;
    DDU64 MatchAllKeyword;
} DD_ETW_SESSION_DATA;

DECLSPEC_NOINLINE __inline void __stdcall GfxPrivateEnableCallBackV2(_In_ LPCGUID SourceId, _In_ DDU32 ControlCode, _In_ DDU8 Level, _In_ DDU64 MatchAnyKeyword, _In_ DDU64 MatchAllKeyword,
                                                                     _In_opt_ PEVENT_FILTER_DESCRIPTOR FilterData, _Inout_opt_ void *CallbackContext);

EXTERN_C __declspec(selectany) REGHANDLE Intel_Gfx_DriverTrackingHandle = (REGHANDLE)0;

DECLSPEC_NOINLINE // __inline
void __stdcall GfxMcGenControlCallbackV2(_In_ LPCGUID SourceId, _In_ ULONG ControlCode, _In_ DDU8 Level, _In_ DDU64 MatchAnyKeyword, _In_ DDU64 MatchAllKeyword,
                                         _In_opt_ PEVENT_FILTER_DESCRIPTOR FilterData, _Inout_opt_ void *CallbackContext);

// instrumentation manifest callback
EXTERN_C __declspec(selectany) REGHANDLE Intel_Gfx_DriverInstrumentationTrackingHandle = (REGHANDLE)0;
DECLSPEC_NOINLINE // __inline
void __stdcall InsrumentationControlCallback(_In_ LPCGUID SourceId, _In_ ULONG ControlCode, _In_ DDU8 Level, _In_ DDU64 MatchAnyKeyword, _In_ DDU64 MatchAllKeyword,
                                             _In_opt_ PEVENT_FILTER_DESCRIPTOR FilterData, _Inout_opt_ void *CallbackContext);

#endif
#endif

// The mapped Dbg msg verbosity based on the current etw logging level
// This is done to provide a way to reduce the verbosity of the dbg messages in line with etw level
EXTERN_C __declspec(selectany) DDU8 EtwLoggingVerbosity;
EXTERN_C __declspec(selectany) DDU8 EtwDebugLogingMask;

// General Logging functions
void LOG_DebugMessage(DDU32 Level, const char *Function, DDU32 Line, const char *DebugMessageFmt, ...);

// Function Entry Exit logging
void LOG_FunctionEntry(const char *Function, DDU32 LineNumber);
void LOG_FunctionExit(const char *Function, DDSTATUS Status, DDU32 LineNumber);
void LOG_DdiEntry(void *MiniportDeviceContext, DD_DIAG_SOURCE_DDI Ddi);
void LOG_DdiExit(void *MiniportDeviceContext, DD_DIAG_SOURCE_DDI Ddi, DDU32 NtStatus);
DDU32 LOG_GetNextIndex(DDU32 *pIndex);

// Structure dump
void LOG_StructureDump(const char *Function, DDU32 Line, const char *StructureName, DDU32 Size, char *Data, const char *Message);

#ifndef GFXDBG_CRITICAL
#define GFXDBG_CRITICAL (0x00000001)
#define GFXDBG_NORMAL (0x00000002)
#define GFXDBG_VERBOSE (0x00000004)
#endif

// custom control to selectively log non-critical messages, by setting DD_LOG_DETAILED = 1, GFXDBG_CRITICAL will always be logged
#if _RELEASE_INTERNAL || _DEBUG
#define DISP_DBG_MSG(Level, DebugMessageFmt, ...) LOG_DebugMessage(Level, __FUNCTION__, __LINE__, DebugMessageFmt, __VA_ARGS__)
#define DISP_STRUCT_DUMP(StructureName, Data, Message) LOG_StructureDump(__FUNCTION__, __LINE__, #StructureName, sizeof(StructureName), (char *)Data, Message)
#define DISP_BINARY_DUMP(Data, Size, Message) LOG_StructureDump(__FUNCTION__, __LINE__, "BinaryDump", Size, (char *)Data, Message)
#define DISP_FUNC_ENTRY() LOG_FunctionEntry(__FUNCTION__, __LINE__)
#define DISP_FUNC_WO_STATUS_ENTRY() LOG_FunctionEntry(__FUNCTION__, __LINE__)
#if DD_LOG_DETAILED
#define DISP_FUNC_EXIT() LOG_FunctionExit(__FUNCTION__, Status, __LINE__)
#define DISP_FUNC_EXIT_W_STATUS(Status) LOG_FunctionExit(__FUNCTION__, Status, __LINE__)

#define DISP_FUNC_WO_STATUS_EXIT()
#define DISP_FUNC_EXIT_W_STATUS(Status)
     LOG_FunctionExit(__FUNCTION__, DDS_SUCCESS, __LINE__)
#else

// print only critical
// Function entries and the Passing Functions are skipped from geting logged. Failing function are passed on to the logging layer
#define DISP_FUNC_EXIT()      \
    if (Status < DDS_SUCCESS) \
    LOG_FunctionExit(__FUNCTION__, Status, __LINE__)
#define DISP_FUNC_EXIT_W_STATUS(Status) \
    if (Status < DDS_SUCCESS)           \
    LOG_FunctionExit(__FUNCTION__, Status, __LINE__)

#define DISP_FUNC_WO_STATUS_EXIT()
#define DISP_FUNC_EXIT_W_STATUS(Status)
    
#endif
#else

#define DISP_DBG_MSG(Level, DebugMessageFmt, ...)
#define DISP_FUNC_ENTRY()
#define DISP_FUNC_WO_STATUS_ENTRY()
#define DISP_FUNC_EXIT()      \
    if (Status < DDS_SUCCESS) \
    LOG_FunctionExit(NULL, Status, __LINE__)
#define DISP_FUNC_EXIT_W_STATUS(Status) \
    if (Status < DDS_SUCCESS)           \
    LOG_FunctionExit(NULL, Status, __LINE__)

#define DISP_FUNC_WO_STATUS_EXIT()
#define DISP_FUNC_EXIT_W_STATUS(Status)
    
#define DISP_STRUCT_DUMP(StructureName, Data, Message)
#define DISP_BINARY_DUMP(Data, Size, Message)
#endif

#else

#define DISP_DBG_MSG(Level, DebugMessageFmt, ...)
#define DISP_FUNC_ENTRY()
#define DISP_FUNC_WO_STATUS_ENTRY()
#define DISP_FUNC_EXIT()

#define DISP_FUNC_WO_STATUS_EXIT()
#define DISP_FUNC_EXIT_W_STATUS(Status)
    
#define DISP_STRUCT_DUMP(StructureName, Data, Message)
#define DISP_BINARY_DUMP(Data, Size, Message)
#endif
