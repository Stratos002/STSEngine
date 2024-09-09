#include "stse_memory.h"
#include "stse_exit.h"
#include "stse_log.h"

#include <stdlib.h>
#include <string.h>

#define STSE_MEMORY_MAX_ALLOCATION_COUNT 1024
#define STSE_MEMORY_MAX_ALLOCATION_TOTAL_SIZE_IN_BYTES 32000

struct STSE_Memory_Allocation
{
    uint32_t sizeInBytes;
    void* pBlock;
};

struct STSE_Memory_State
{
    uint32_t allocationCount;
    uint32_t totalAllocationSizeInBytes;
    struct STSE_Memory_Allocation allocations[STSE_MEMORY_MAX_ALLOCATION_COUNT];
};

static struct STSE_Memory_State* pState = NULL;

void STSE_Memory_initialize(void)
{
    if(pState != NULL)
    {
        STSE_Exit_exitFailure();
    }

    pState = malloc(sizeof(struct STSE_Memory_State));
    if(pState == NULL)
    {
        STSE_Exit_exitFailure();
    }

    memset(pState, 0, sizeof(struct STSE_Memory_State));
}

void STSE_Memory_terminate(void)
{
    if(pState != NULL)
    {
        if(pState->allocationCount != 0u)
        {
            STSE_LOG_log(STSE_LOG_OUTPUT_ERROR, "there is a memory leak");
            STSE_Exit_exitFailure();
        }

        free(pState);
        pState = NULL;
    }
}

void STSE_Memory_allocate(const uint32_t sizeInBytes, void** ppOutBlock)
{
    if(pState == NULL || 
        ppOutBlock == NULL || 
        *ppOutBlock != NULL || 
        pState->allocationCount == STSE_MEMORY_MAX_ALLOCATION_COUNT || 
        pState->totalAllocationSizeInBytes + sizeInBytes > STSE_MEMORY_MAX_ALLOCATION_TOTAL_SIZE_IN_BYTES)
    {
        STSE_Exit_exitFailure();
    }

    if(sizeInBytes == 0u)
    {
        return;
    }

    pState->allocations[pState->allocationCount].sizeInBytes = sizeInBytes;
    pState->allocations[pState->allocationCount].pBlock = malloc(sizeInBytes);

    if(pState->allocations[pState->allocationCount].pBlock == NULL)
    {
        STSE_Exit_exitFailure();
    }

    *ppOutBlock = pState->allocations[pState->allocationCount].pBlock;

    ++pState->allocationCount;
    pState->totalAllocationSizeInBytes += sizeInBytes;
}

void STSE_Memory_deallocate(void* pInBlock)
{
    uint32_t allocationIndex = 0u;

    if(pState == NULL)
    {
        STSE_Exit_exitFailure();
    }

    if(pInBlock == NULL)
    {
        return;
    }

    for(allocationIndex = 0u; allocationIndex < pState->allocationCount; ++allocationIndex)
    {
        if(pState->allocations[allocationIndex].pBlock == pInBlock)
        {
            free(pState->allocations[allocationIndex].pBlock);
            pState->totalAllocationSizeInBytes -= pState->allocations[allocationIndex].sizeInBytes;
            
            pState->allocations[allocationIndex] = pState->allocations[pState->allocationCount - 1u];
            --pState->allocationCount;

            return;
        }
    }

    STSE_Exit_exitFailure();
}

void STSE_Memory_memset(const uint32_t sizeInBytes, const int32_t value, void* pInOutBlock)
{
    if(pState == NULL || pInOutBlock == NULL || sizeInBytes == 0u)
    {
        STSE_Exit_exitFailure();
    }

    memset(pInOutBlock, value, sizeInBytes);
}

void STSE_Memory_memzero(const uint32_t sizeInBytes, void* pInOutBlock)
{
    STSE_Memory_memset(sizeInBytes, 0, pInOutBlock);
}
