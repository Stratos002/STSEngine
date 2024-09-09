#include "stse_exit.h"

#include <stdlib.h>

void STSE_Exit_exitFailure(void)
{
    exit(-1);
}

void STSE_Exit_exitSuccess(void)
{
    exit(0);
}
