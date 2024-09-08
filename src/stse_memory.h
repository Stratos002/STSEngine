#ifndef STSE_MEMORY_H
#define STSE_MEMORY_H

#include <stdint.h>

void STSE_Memory_initialize(void);

void STSE_Memory_terminate(void);

void STSE_Memory_allocate(const uint32_t sizeInBytes, void** ppOutBlock);

void STSE_Memory_deallocate(void* pInBlock);

void STSE_Memory_memset(const uint32_t sizeInBytes, const int32_t value, void* pInOutBlock);

void STSE_Memory_memzero(const uint32_t sizeInBytes, void* pInOutBlock);

#endif
