#include "stsengine.h"
#include "stse_log.h"
#include "stse_assert.h"

#include <stdlib.h>
#include <string.h>

#ifdef STSE_CONFIGURATION_DEBUG

// ========= DEBUG ==========

#define STSE_MEMORY_MAX_ALLOCATION_COUNT 1024

struct STSE_Memory_Allocation
{
    void* pBlock;
    uint32_t size;
    const char* pFile;
    uint32_t line;
};  

struct STSE_Memory_State
{
    uint32_t allocationCount;
    struct STSE_Memory_Allocation allocations[STSE_MEMORY_MAX_ALLOCATION_COUNT];
};

static struct STSE_Memory_State* pState = NULL;

enum STSE_Result STSE_Memory_initialize()
{
    if(pState != NULL)
    {
        STSE_LOG_ERROR("memory system was already initialized");
        return STSE_RESULT_ALREADY_INITIALIZED;
    }

    pState = malloc(sizeof(struct STSE_Memory_State));
    if(pState == NULL)
    {
        STSE_LOG_EXIT_FATAL("memory allocation failure");
    }
    (void)memset(pState, 0, sizeof(struct STSE_Memory_State));

    return STSE_RESULT_SUCCESS;
}

void STSE_Memory_terminate()
{
    if(pState != NULL)
    {
        for(uint32_t allocationIndex = 0u; allocationIndex < pState->allocationCount; ++allocationIndex)
        {
            STSE_LOG_DEBUG_ARGS("memory leak detected : block (size = %dbyte(s)) allocated from file : %s, line : %d was never deallocated", 
                pState->allocations[allocationIndex].size, 
                pState->allocations[allocationIndex].pFile,
                pState->allocations[allocationIndex].line);
        }
        free(pState);
        pState = NULL;
    }
    else
    {
        STSE_LOG_ERROR("memory system was not initialized");
        return;
    }
}

enum STSE_Result STSE_Memory_allocate(const uint32_t size, const char* pInFile, const uint32_t line, void** ppOutBlock)
{
    if(pState == NULL)
    {
        STSE_LOG_ERROR("memory system was not initialized");
        return STSE_RESULT_NOT_INITIALIZED;
    }

    if(pState->allocationCount == STSE_MEMORY_MAX_ALLOCATION_COUNT)
    {
        STSE_LOG_ERROR("max allocation count reached, increase it if needed");
        return STSE_RESULT_MAX_ALLOCATION_COUNT_REACHED;
    }

    STSE_ASSERT(ppOutBlock != NULL);
    *ppOutBlock = malloc(size);
    if(*ppOutBlock == NULL)
    {
        STSE_LOG_EXIT_FATAL("memory allocation failure");
    }

    pState->allocations[pState->allocationCount].pBlock = *ppOutBlock;
    pState->allocations[pState->allocationCount].size = size;
    pState->allocations[pState->allocationCount].pFile = pInFile;
    pState->allocations[pState->allocationCount].line = line;
    ++(pState->allocationCount);

    return STSE_RESULT_SUCCESS;
}

enum STSE_Result STSE_Memory_deallocate(void** ppInOutBlock)
{
    if(pState == NULL)
    {
        STSE_LOG_ERROR("memory system not already initialized");
        return STSE_RESULT_NOT_INITIALIZED;
    }

    STSE_ASSERT(ppInOutBlock != NULL);
    for(uint32_t allocationIndex = 0u; allocationIndex < pState->allocationCount; ++allocationIndex)
    {
        if(pState->allocations[allocationIndex].pBlock == *ppInOutBlock)
        {
            if(allocationIndex != pState->allocationCount - 1)
            {
                memcpy(&pState->allocations[allocationIndex], &pState->allocations[pState->allocationCount - 1], sizeof(struct STSE_Memory_Allocation));
            }

            *ppInOutBlock = NULL;
            --(pState->allocationCount);
            return STSE_RESULT_SUCCESS;
        }
    }
    STSE_LOG_WARNING("trying to deallocate a block that was never allocated");
    return STSE_RESULT_SUCCESS;
}

void STSE_Memory_memset(const uint32_t size, const int32_t value, void* pInOutBlock)
{
    (void)memset(pInOutBlock, value, size);
}

#else 

// ========== NDEBUG ===========

enum STSE_Result STSE_Memory_initialize()
{
    return STSE_RESULT_SUCCESS;
}

void STSE_Memory_terminate()
{
    return;
}

enum STSE_Result STSE_Memory_allocate(const uint32_t size, void** ppOutBlock)
{
    *ppOutBlock = malloc(size);
    if(*ppOutBlock == NULL)
    {
        STSE_LOG_EXIT_FATAL("memory allocation failure");
    }
    return STSE_RESULT_SUCCESS;
}

enum STSE_Result STSE_Memory_deallocate(void** ppInOutBlock)
{
    free(*ppInOutBlock);
    *ppInOutBlock = NULL;
    return STSE_RESULT_SUCCESS;
}

void STSE_Memory_memset(const uint32_t size, const int32_t value, void* pInOutBlock)
{
    (void)memset(pInOutBlock, value, size);
}

#endif