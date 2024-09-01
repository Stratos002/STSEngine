#ifndef STSE_LOG_H
#define STSE_LOG_H

#include <stdint.h>
#include <stdlib.h>

enum STSE_Log_Severity
{
    STSE_LOG_SEVERITY_FATAL,    // the program should exit
    STSE_LOG_SEVERITY_ERROR,    // the program doesn't need to stop, but the current function must
    STSE_LOG_SEVERITY_WARNING,  // doesn't smell gud
    STSE_LOG_SEVERITY_DEBUG,    // on debug configuration only
    STSE_LOG_SEVERITY_INFO,     // display infos
    STSE_LOG_SEVERITY_MAX
};

void STSE_Log_printToConsole(
    const enum STSE_Log_Severity severity, 
    const char* pMessage, 
    const char* pInFile, 
    const uint32_t line);

void STSE_Log_printToConsoleArgs(
    const enum STSE_Log_Severity severity, 
    const char* pMessage, 
    const char* pInFile, 
    const uint32_t line, 
    ...);

#define STSE_LOG_EXIT_FATAL_ARGS(message, ...)                                                      \
    STSE_Log_printToConsoleArgs(STSE_LOG_SEVERITY_FATAL, message, __FILE__, __LINE__, __VA_ARGS__); \
    exit(-1)

#define STSE_LOG_EXIT_FATAL(message)                                                \
    STSE_Log_printToConsole(STSE_LOG_SEVERITY_FATAL, message, __FILE__, __LINE__);  \
    exit(-1)

#define STSE_LOG_ERROR_ARGS(message, ...) STSE_Log_printToConsoleArgs(STSE_LOG_SEVERITY_ERROR, message, __FILE__, __LINE__, __VA_ARGS__)
#define STSE_LOG_ERROR(message) STSE_Log_printToConsole(STSE_LOG_SEVERITY_ERROR, message, __FILE__, __LINE__)

#define STSE_LOG_WARNING_ARGS(message, ...) STSE_Log_printToConsoleArgs(STSE_LOG_SEVERITY_WARNING, message, __FILE__, __LINE__, __VA_ARGS__)
#define STSE_LOG_WARNING(message) STSE_Log_printToConsole(STSE_LOG_SEVERITY_WARNING, message, __FILE__, __LINE__)

#ifdef STSE_CONFIGURATION_DEBUG
    #define STSE_LOG_DEBUG_ARGS(message, ...) STSE_Log_printToConsoleArgs(STSE_LOG_SEVERITY_DEBUG, message, __FILE__, __LINE__, __VA_ARGS__)
    #define STSE_LOG_DEBUG(message) STSE_Log_printToConsole(STSE_LOG_SEVERITY_DEBUG, message, __FILE__, __LINE__)
#else
    #define STSE_LOG_DEBUG_ARGS(message, ...)
    #define STSE_LOG_DEBUG(message)
#endif

#define STSE_LOG_INFO_ARGS(message, ...) STSE_Log_printToConsoleArgs(STSE_LOG_SEVERITY_INFO, message, __FILE__, __LINE__, __VA_ARGS__)
#define STSE_LOG_INFO(message) STSE_Log_printToConsole(STSE_LOG_SEVERITY_INFO, message, __FILE__, __LINE__)

#endif