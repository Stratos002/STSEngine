#include "stse.h"
#include "stse_memory.h"
#include "stse_gpu.h"
#include "stse_log.h"

#include <stdlib.h>

enum STSE_Result STSE_initialize()
{
    enum STSE_Result result = STSE_Memory_initialize();
    if(result != STSE_RESULT_SUCCESS)
    {
        STSE_LOG_log(STSE_LOG_SEVERITY_ERROR, "could not initialize the memory system");
        exit(result);
    }

    result = STSE_GPU_initialize();
    if(result != STSE_RESULT_SUCCESS)
    {
        STSE_LOG_log(const enum STSE_Log_Severity severity, const char *pMessage)
        return result;
    }

    return STSE_RESULT_SUCCESS;
}

void STSE_terminate()
{
    STSE_GPU_terminate();
    STSE_Memory_terminate();
}