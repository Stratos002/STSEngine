#include "stse_memory.h"
#include "stse_exit.h"

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
        STSE_EXIT_exitFailure();
    }

    pState = malloc(sizeof(struct STSE_Memory_State));
    if(pState == NULL)
    {
        STSE_EXIT_exitFailure();
    }

    memset(pState, 0, sizeof(struct STSE_Memory_State));
}

void STSE_Memory_terminate(void)
{
    if(pState != NULL)
    {
        if(pState->allocationCount != 0u)
        {
            STSE_EXIT_exitFailure();
        }

        free(pState);
        pState = NULL;
    }
}

void STSE_Memory_allocate(const uint32_t sizeInBytes, void** ppOutBlock)
{
    uint32_t allocationIndex = 0u;

    if(pState == NULL)
    {
        STSE_EXIT_exitFailure();
    }

    if(sizeInBytes == 0u || ppOutBlock == NULL)
    {
        STSE_EXIT_exitFailure();
    }

    if(pState->allocationCount == STSE_MEMORY_MAX_ALLOCATION_COUNT)
    {
        STSE_EXIT_exitFailure();
    }

    if(pState->totalAllocationSizeInBytes + sizeInBytes > STSE_MEMORY_MAX_ALLOCATION_TOTAL_SIZE_IN_BYTES)
    {
        STSE_EXIT_exitFailure();
    }

    for(allocationIndex = 0u; allocationIndex < pState->allocationCount; ++allocationIndex)
    {
        if(pState->allocations[allocationIndex].pBlock == *ppOutBlock)
        {
            STSE_EXIT_exitFailure();
        }
    }

    pState->allocations[pState->allocationCount].sizeInBytes = sizeInBytes;
    pState->allocations[pState->allocationCount].pBlock = malloc(sizeInBytes);

    if(pState->allocations[pState->allocationCount].pBlock == NULL)
    {
        STSE_EXIT_exitFailure();
    }

    *ppOutBlock = pState->allocations[pState->allocationCount].pBlock;

    ++pState->allocationCount;
    pState->totalAllocationSizeInBytes += sizeInBytes;
}

void STSE_Memory_deallocate(void* pInBlock)
{
    uint32_t allocationIndex = 0u;

    if(pInBlock == NULL)
    {
        STSE_EXIT_exitFailure();
    }

    if(pState == NULL)
    {
        STSE_EXIT_exitFailure();
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

    STSE_EXIT_exitFailure();
}

void STSE_Memory_memset(const uint32_t sizeInBytes, const int32_t value, void* pInOutBlock)
{
    if(pState == NULL)
    {
        STSE_EXIT_exitFailure();
    }

    memset(pInOutBlock, value, sizeInBytes);
}

void STSE_Memory_memzero(const uint32_t sizeInBytes, void* pInOutBlock)
{
    STSE_Memory_memset(sizeInBytes, 0, pInOutBlock);
}
