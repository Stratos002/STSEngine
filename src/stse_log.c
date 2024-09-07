#include "stse_log.h"

#include <stdarg.h>
#include <stdio.h>

static const char* pSeverityStrings[STSE_LOG_SEVERITY_MAX] = {
    "ERROR",
    "WARNING",
    "INFO"
};

void STSE_LOG_log(const enum STSE_Log_Severity severity, const char* pMessage, ...)
{
    char formattedMessage[1024];

    va_list args;
    va_start(args, pMessage);
    vsprintf(formattedMessage, pMessage, args);
    va_end(args);

    if(severity < STSE_LOG_SEVERITY_INFO)
    {
        fprintf(stderr, "[%s] : %s\n", pSeverityStrings[severity], formattedMessage);
    }
    else 
    {
        fprintf(stdout, "[%s] : %s\n", pSeverityStrings[severity], formattedMessage);
    }
}