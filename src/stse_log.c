#include "stse_log.h"

#include <stdarg.h>
#include <stdio.h>

void STSE_LOG_log(const enum STSE_LOG_OUTPUT output, const char* pMessage, ...)
{
    char formattedMessage[1024];
    va_list args;

    va_start(args, pMessage);
    vsprintf(formattedMessage, pMessage, args);
    va_end(args);

    if(output == STSE_LOG_OUTPUT_ERROR)
    {
        fprintf(stderr, "[%s] : %s\n", "ERROR", formattedMessage);
    }
    else 
    {
        fprintf(stdout, "[%s] : %s\n", "STANDARD", formattedMessage);
    }
}
