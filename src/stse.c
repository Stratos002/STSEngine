#include "stse.h"
#include "stse_memory.h"
#include "stse_gpu.h"
#include "stse_log.h"

void STSE_initialize(void)
{
    STSE_Memory_initialize();
    STSE_GPU_initialize();

    STSE_LOG_log(STSE_LOG_OUTPUT_STANDARD, "STSEngine initialized successfully");
}

void STSE_terminate(void)
{
    STSE_GPU_terminate();
    STSE_Memory_terminate();

    STSE_LOG_log(STSE_LOG_OUTPUT_STANDARD, "STSEngine terminated successfully");
}
