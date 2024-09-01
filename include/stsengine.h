#ifndef STSENGINE_H
#define STSENGINE_H

#include "stse_result.h"

enum STSE_Result STSE_returnResultIfFailure(const enum STSE_Result result, const char* pMessage);

enum STSE_Result STSE_initialize();

void STSE_terminate();

#endif