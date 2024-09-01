#ifndef STSE_RESULT_H
#define STSE_RESULT_H

#include "stse_log.h"

enum STSE_Result
{
    STSE_RESULT_SUCCESS,
    STSE_RESULT_NOT_INITIALIZED,
    STSE_RESULT_ALREADY_INITIALIZED,
    STSE_RESULT_MAX_ALLOCATION_COUNT_REACHED,
    STSE_RESULT_GRAPHICS_API_FAILURE
};

#define STSE_RESULT_RETURN_IF_ERROR(expr, pMessage)     \
    {                                                   \
        const enum STSE_Result result = (expr);         \
        if(result != STSE_RESULT_SUCCESS)               \
        {                                               \
            STSE_LOG_ERROR(pMessage);                   \
            return result;                              \
        }                                               \
    }

#endif