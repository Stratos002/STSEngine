#ifndef STSE_ASSERT_H
#define STSE_ASSERT_H

#include "stse_log.h"

#include <stdlib.h>
#include <stdbool.h>

#ifdef STSE_CONFIGURATION_DEBUG
    #define STSE_ASSERT(expr)                                                                                   \
        if((expr) != true)                                                                                      \
        {                                                                                                       \
            STSE_LOG_DEBUG_ARGS("assertion failed : %s (at file : %s, line : %d)", #expr, __FILE__, __LINE__);  \
            abort();                                                                                            \
        } 
#else
    #define STSE_ASSERT(expr)
#endif

#endif