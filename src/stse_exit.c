#include "stse_exit.h"
#include "stse_log.h"

#include <stdlib.h>

void STSE_EXIT_exitFailure(void)
{
    exit(-1);
}

void STSE_EXIT_exitSuccess(void)
{
    exit(0);
}
