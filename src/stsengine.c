#include "stsengine.h"
#include "stse_memory.h"
#include "stse_window.h"
#include "stse_log.h"

enum STSE_Result STSE_initialize()
{
    enum STSE_Result result = STSE_Memory_initialize();
    if(result != STSE_RESULT_SUCCESS)
    {
        return result;
    }

    uint32_t* pBlock = NULL;
    STSE_MEMORY_ALLOCATE(32, &pBlock);
    //STSE_MEMORY_DEALLOCATE(&pBlock);

    return STSE_RESULT_SUCCESS;
}

void STSE_terminate()
{
    STSE_Memory_terminate();
}