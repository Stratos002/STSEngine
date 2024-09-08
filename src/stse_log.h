#ifndef STSE_LOG_H
#define STSE_LOG_H

enum STSE_LOG_OUTPUT
{
    STSE_LOG_OUTPUT_STANDARD,
    STSE_LOG_OUTPUT_ERROR
};

void STSE_LOG_log(const enum STSE_LOG_OUTPUT output, const char* pMessage, ...);

#endif
