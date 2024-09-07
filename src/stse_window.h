#ifndef STSE_WINDOW_H
#define STSE_WINDOW_H

#include "stse.h"

#include <stdint.h>

enum STSE_Result STSE_Window_initialize();

void STSE_Window_terminate();

enum STSE_Result STSE_Window_addWindow(uint32_t* pOutWindowHandle);

void STSE_Window_removeWindow(const uint32_t windowHandle);

#endif