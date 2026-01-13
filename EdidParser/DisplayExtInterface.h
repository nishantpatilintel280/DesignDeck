#pragma once
#include "DisplayDefs.h"

#define DD_ALLOC_MEM(size) __DD_AllocMem(size, NULL, NULL, __LINE__)

// free mem and Nullify pointer
#define DD_SAFE_FREE(ptr) __DD_SafeFreeMem((void **)&(ptr))

// Method to allocate memory
void ExtAllocateMem(DD_ALLOCATE_MEM_ARGS* pArgs);

typedef struct _DD_ALLOCATE_MEM_ARGS
{
    void** ppDestMem;
    DDU32 Size;
    DD_BOOL IsPageable;
    DDU32 Flags;
    DDU8* pFileName;
    DDU8* pFunctionName;
    DDU32 LineNumber;
    void* pAddress;
} DD_ALLOCATE_MEM_ARGS;


// (** internal to Display **)
// frees memory and clears pointer
DD_INLINE void __DD_SafeFreeMem(void** ppMem)
{
    if (ppMem && *ppMem)
    {
        ExtFreeMem(*ppMem);
        *ppMem = NULL;
    }
}


// (** internal to Display **)
// allocate non-paged zeroed-out mem block
DD_INLINE void* __DD_AllocMem(DDU32 Size, DDU8* pFileName, DDU8* pFunctionName, DDU32 LineNumber)
{
    DD_ALLOCATE_MEM_ARGS AllocMemArg;
    void* pBuffer = NULL;
    extern void* _ReturnAddress(void); // ntddk.h has the declaration, doing extern here as dont want to include OS headers in Display headers.
    if (Size > 0)
    {
        AllocMemArg.Flags = 0;
        AllocMemArg.IsPageable = FALSE;
        AllocMemArg.ppDestMem = &pBuffer;
        AllocMemArg.Size = Size;
        AllocMemArg.pFileName = pFileName;
        AllocMemArg.pFunctionName = pFunctionName;
        AllocMemArg.LineNumber = LineNumber;
        AllocMemArg.pAddress = _ReturnAddress();
        ExtAllocateMem(&AllocMemArg);
    }

    return pBuffer;
}