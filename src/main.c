#include "stse.h"
#include "stse_log.h"

#include <stdlib.h>

int main()
{
    enum STSE_Result result = STSE_RESULT_SUCCESS;

    result = STSE_initialize();
    if(result != STSE_RESULT_SUCCESS)
    {
        STSE_LOG_log(STSE_LOG_SEVERITY_ERROR, "could not initialize STSEngine :(");
        exit(result);
    }

    STSE_terminate();
    
    return 0;
}