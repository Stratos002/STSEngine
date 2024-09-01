#ifndef STSE_MEMORY_H
#define STSE_MEMORY_H

#include "stsengine.h"

#include <stdint.h>

enum STSE_Result STSE_Memory_initialize();

void STSE_Memory_terminate();

#ifdef STSE_CONFIGURATION_DEBUG
    enum STSE_Result STSE_Memory_allocate(const uint32_t size, const char* pInFile, const uint32_t line, void** ppOutBlock);
    #define STSE_MEMORY_ALLOCATE(size, ppOutBlock) STSE_Memory_allocate(size, __FILE__, __LINE__, (void**)(ppOutBlock))
#else
    enum STSE_Result STSE_Memory_allocate(const uint32_t size, void** ppOutBlock);
    #define STSE_MEMORY_ALLOCATE(size, ppOutBlock) STSE_Memory_allocate(size, (void**)(ppOutBlock))
#endif

enum STSE_Result STSE_Memory_deallocate(void** ppInOutBlock);
#define STSE_MEMORY_DEALLOCATE(ppInOutBlock) STSE_Memory_deallocate((void**)(ppInOutBlock))

void STSE_Memory_memset(const uint32_t size, const int32_t value, void* pInOutBlock);
#define STSE_MEMORY_MEMSET(size, value, pInOutBlock) STSE_Memory_memset(size, value, (void*)(pInOutBlock))
#define STSE_MEMORY_MEMZERO(size, pInOutBlock) STSE_Memory_memset(size, 0, (void*)(pInOutBlock))

#endif