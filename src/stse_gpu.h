#ifndef STSE_GPU_H
#define STSE_GPU_H

#include <stdint.h>

enum STSE_GPU_ResourceType
{
    STSE_GPU_RESOURCE_TYPE_UNIFORM_BUFFER,
    STSE_GPU_RESOURCE_TYPE_STORAGE_BUFFER,
    STSE_GPU_RESOURCE_TYPE_IMAGE_2D
};

struct STSE_GPU_UniformBufferParameters
{
    uint32_t sizeInBytes;
};

struct STSE_GPU_StorageBufferParameters
{
    uint32_t sizeInBytes;
};

struct STSE_GPU_Image2DParameters
{
    uint32_t width;
    uint32_t height;
};

union STSE_GPU_ResourceParameters
{
    struct STSE_GPU_UniformBufferParameters uniformBufferParameters;
    struct STSE_GPU_StorageBufferParameters storageBufferParameters;
    struct STSE_GPU_Image2DParameters image2DParameters;
};

struct STSE_GPU_Resource
{
    const char* pName;
    enum STSE_GPU_ResourceType type;
    union STSE_GPU_ResourceParameters parameters;
};

enum STSE_GPU_PassType
{
    STSE_GPU_PASS_TYPE_COMPUTE
};

struct STSE_GPU_Pass
{
    enum STSE_GPU_PassType type;
    uint32_t inputResourceCount;
    struct STSE_GPU_Resource* pInputResources;
    uint32_t outputResourceCount;
    struct STSE_GPU_Resource* pOutputResources;
};

struct STSE_GPU_Program
{
    uint32_t resourceCount;
    struct STSE_GPU_Resource* pResources;
    uint32_t passCount;
    struct STSE_GPU_Pass* pPasses;
};

void STSE_GPU_initialize(void);

void STSE_GPU_terminate(void);

void STSE_GPU_setProgram(const struct STSE_GPU_Program program);

void STSE_GPU_compileProgram(void);

void STSE_GPU_executeProgram(void);

void STSE_GPU_deleteProgram(void);

#endif
