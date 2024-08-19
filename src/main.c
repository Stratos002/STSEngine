#include "stsengine.h"
#include "stse_log.h"

int main()
{
    if(STSE_initialize() != STSE_RESULT_SUCCESS)
    {
        STSE_LOG_EXIT_FATAL("failed to initialize STSEngine");
    }

    STSE_terminate();
    
    return 0;
}