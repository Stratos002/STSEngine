#ifndef STSE_LOG_H
#define STSE_LOG_H

#include <stdint.h>
#include <stdlib.h>

enum STSE_Log_Severity
{
    STSE_LOG_SEVERITY_ERROR,    // the program doesn't need to stop, but the current function must
    STSE_LOG_SEVERITY_WARNING,  // doesn't smell gud
    STSE_LOG_SEVERITY_INFO,     // display infos
    STSE_LOG_SEVERITY_MAX
};

void STSE_LOG_log(const enum STSE_Log_Severity severity, const char* pMessage, ...);

#endif