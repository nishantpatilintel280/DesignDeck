#pragma once

#include <stdlib.h>
#include <malloc.h>
#include "..\DisplayDefs.h"

typedef struct _DD_ALLOCATE_MEM_ARGS
{
    void** ppDestMem;
    DDU32   Size;
    DD_BOOL IsPageable;
    DDU32   Flags;
    DDU8* pFileName;
    DDU8* pFunctionName;
    DDU32   LineNumber;
} DD_ALLOCATE_MEM_ARGS;

/**
 * @brief
 *
 *
 * @param  size
 * @return void*
 */
void* Utf_Malloc(uint32_t size)
{
    void* pMem = NULL;
    pMem = malloc(size);
    if (pMem)
    {
        memset(pMem, 0, size);
        // InterlockedIncrement(&g_UTFCtxt.AllocCount);
    }
    return pMem;
}

/**
 * @brief
 *
 *
 * @param  pMem
 * @return void
 */
void Utf_Free(void** pMem)
{
    if (pMem && *pMem)
    {
        // DDASSERT(g_UTFCtxt.AllocCount > 0);
        free(*pMem);
        *pMem = NULL;
        // InterlockedDecrement(&g_UTFCtxt.AllocCount);
    }
}

#define UTF_SAFE_FREE(pMem) Utf_Free((&(pMem)))
#define UTF_MALLOC(size) Utf_Malloc(size)

/**
 * @brief
 *
 *
 * @param  DD_ALLOCATE_MEM_ARGS
 * @return void
 */
void ExtAllocateMem(DD_ALLOCATE_MEM_ARGS* pArg)
{
    // arg validation
    if (NULL == pArg)
    {
    }
    else
    {
        // function body
        *pArg->ppDestMem = UTF_MALLOC(pArg->Size);
    }
}

/**
 * @brief
 *
 *
 * @param  pArg
 * @return void
 */
void ExtFreeMem(void* pArg)
{
    // arg validation
    if (NULL == pArg)
    {
    }
    else
    {
        // function body
        UTF_SAFE_FREE(pArg);
    }
}

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

// allocate non-paged zeroed-out mem block
DD_INLINE void* __DD_AllocMem(DDU32 Size, DDU8* pFileName, DDU8* pFunctionName, DDU32 LineNumber)
{
    DD_ALLOCATE_MEM_ARGS AllocMemArg;
    void* pBuffer = NULL;

    if (Size > 0)
    {
        AllocMemArg.Flags = 0;
        AllocMemArg.IsPageable = FALSE;
        AllocMemArg.ppDestMem = &pBuffer;
        AllocMemArg.Size = Size;
        AllocMemArg.pFileName = pFileName;
        AllocMemArg.pFunctionName = pFunctionName;
        AllocMemArg.LineNumber = LineNumber;
        ExtAllocateMem(&AllocMemArg);
    }

    return pBuffer;
}

#define DD_ALLOC_MEM(size) __DD_AllocMem(size, (DDU8 *)__FILE__, (DDU8 *)__FUNCTION__, __LINE__)
#define DD_SAFE_FREE(ptr) __DD_SafeFreeMem((void **)&(ptr))
