#include "stse.h"
#include "stse_memory.h"
#include "stse_gpu.h"
#include "stse_log.h"
#include "stse_window.h"

void STSE_initialize(void)
{
    STSE_Memory_initialize();
    STSE_Window_initialize();
    STSE_GPU_initialize();

    STSE_LOG_log(STSE_LOG_OUTPUT_STANDARD, "STSEngine initialized successfully");
}

void STSE_terminate(void)
{
    STSE_GPU_terminate();
    STSE_Window_terminate();
    STSE_Memory_terminate();

    STSE_LOG_log(STSE_LOG_OUTPUT_STANDARD, "STSEngine terminated successfully");
}

void STSE_startLoop(void)
{
    return;
}

void STSE_addWindow(const char* pTitle, const uint32_t width, const uint32_t height, uint32_t* pOutWindowId)
{
    STSE_Window_addWindow(pTitle, width, height, pOutWindowId);
}

void STSE_deleteWindow(const uint32_t windowId)
{
    STSE_Window_deleteWindow(windowId);
}
