#include "stse.h"

int main(void)
{
    uint32_t mainWindowId = 0u;
    
    STSE_initialize();
    STSE_addWindow("STSEngine_Application", 600u, 500u, &mainWindowId);
    STSE_startLoop();
    STSE_deleteWindow(mainWindowId);
    STSE_terminate();

    return 0;
}
