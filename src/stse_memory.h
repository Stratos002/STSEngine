#ifndef STSE_MEMORY_H
#define STSE_MEMORY_H

#include "stse.h"

#include <stdint.h>

enum STSE_Result STSE_Memory_initialize();

void STSE_Memory_terminate();

enum STSE_Result STSE_MEMORY_allocate(void** ppBlock, const uint32_t sizeInBytes);

void STSE_MEMORY_deallocate(const void* pBlock);

#endif