#ifndef STSE_WINDOW_H
#define STSE_WINDOW_H

#ifdef STSE_GRAPHICS_API_VULKAN
    #include "volk.h"
#endif

#include <stdint.h>

void STSE_Window_initialize(void);

void STSE_Window_terminate(void);

void STSE_Window_addWindow(const char* pTitle, const uint32_t width, const uint32_t height, uint32_t* pOutWindowId);

void STSE_Window_deleteWindow(const uint32_t windowId);

#ifdef STSE_GRAPHICS_API_VULKAN
    void STSE_Window_createVkSurface(
        const uint32_t windowId, 
        const VkInstance instance, 
        const VkAllocationCallbacks* pAllocator, 
        VkSurfaceKHR* pOutSurface);

    void STSE_Window_getRequiredVkInstanceExtensions(uint32_t* pOutExtensionCount, const char*** pppOutExtensionNames);
#endif

#endif
