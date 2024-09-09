#ifndef STSE_H
#define STSE_H

#include <stdint.h>

void STSE_initialize(void);

void STSE_terminate(void);

void STSE_startLoop(void);

void STSE_addWindow(const char* pTitle, const uint32_t width, const uint32_t height, uint32_t* pOutWindowId);

void STSE_deleteWindow(const uint32_t windowId);

#endif
