#include "stsengine.h"
#include "stse_memory.h"
#include "stse_gpu.h"
#include "stse_window.h"
#include "stse_log.h"

enum STSE_Result STSE_initialize()
{
    enum STSE_Result result = STSE_Memory_initialize();
    if(result != STSE_RESULT_SUCCESS)
    {
        STSE_LOG_ERROR("could not initialize memory system");
        return result;
    }

    result = STSE_GPU_initialize();
    if(result != STSE_RESULT_SUCCESS)
    {
        STSE_LOG_ERROR("could not initialize gpu system");
        return result;
    }

    return STSE_RESULT_SUCCESS;
}

void STSE_terminate()
{
    STSE_GPU_terminate();
    STSE_Memory_terminate();
}