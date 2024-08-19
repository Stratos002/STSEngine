#include "stse_log.h"
#include "stse_assert.h"

#include <stdarg.h>
#include <stdio.h>

static const char* pSeverityStrings[STSE_LOG_SEVERITY_MAX] = {
    "FATAL",
    "ERROR",
    "WARNING",
    "DEBUG"
};

static void STSE_Log_printPrefix(const enum STSE_Log_Severity severity, const char* pInFile, const uint32_t line)
{
    STSE_ASSERT(severity < STSE_LOG_SEVERITY_MAX);
    printf("[%s](file : %s, line : %d) - ", pSeverityStrings[severity], pInFile, line);
}

void STSE_Log_printToConsole(
    const enum STSE_Log_Severity severity, 
    const char* pMessage, 
    const char* pInFile, 
    const uint32_t line) 
{
    STSE_Log_printPrefix(severity, pInFile, line);
    printf("%s\n", pMessage);
}

void STSE_Log_printToConsoleArgs(
    const enum STSE_Log_Severity severity, 
    const char* pMessage, 
    const char* pInFile, 
    const uint32_t line, 
    ...)
{
	STSE_Log_printPrefix(severity, pInFile, line);
	
    va_list args;
	va_start(args, line);
	vprintf(pMessage, args);
    printf("\n");
	va_end(args);
}