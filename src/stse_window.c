#include "stse_window.h"
#include "stse_exit.h"
#include "stse_memory.h"

#ifdef STSE_GRAPHICS_API_VULKAN
    #define GLFW_INCLUDE_VULKAN
#endif

#include "GLFW/glfw3.h"

#include <stdbool.h>

#define STSE_WINDOW_MAX_WINDOW_COUNT 4

struct STSE_Window_Window
{
    uint32_t id;
    GLFWwindow* pWindow;
};

struct STSE_Window_State
{
    uint32_t windowCount;
    struct STSE_Window_Window windows[STSE_WINDOW_MAX_WINDOW_COUNT];
};

static struct STSE_Window_State* pState = NULL;

void STSE_Window_initialize(void)
{
    if(pState != NULL)
    {
        STSE_Exit_exitFailure();
    }

    STSE_Memory_allocate(sizeof(struct STSE_Window_State), (void**)&pState);
    STSE_Memory_memzero(sizeof(struct STSE_Window_State), pState);

    if(glfwInit() != GLFW_TRUE)
    {
        STSE_Exit_exitFailure();
    }
}

void STSE_Window_terminate(void)
{
    if(pState != NULL)
    {
        uint32_t windowIndex = 0u;

        for(windowIndex = 0u; windowIndex < pState->windowCount; ++windowIndex)
        {
            glfwDestroyWindow(pState->windows[windowIndex].pWindow);
        }

        glfwTerminate();

        STSE_Memory_deallocate(pState);
        pState = NULL;
    }
}

void STSE_Window_addWindow(const char* pTitle, const uint32_t width, const uint32_t height, uint32_t* pOutWindowId)
{
    uint32_t newWindowId = 0u;
    uint32_t windowIndex = 0u;
    bool windowIdAlreadyUsed = true;

    if(pState == NULL || width == 0u || height == 0u || pState->windowCount == STSE_WINDOW_MAX_WINDOW_COUNT)
    {
        STSE_Exit_exitFailure();
    }

    while(windowIdAlreadyUsed)
    {
        windowIdAlreadyUsed = false;
        for(windowIndex = 0u; windowIndex < pState->windowCount; ++windowIndex)
        {
            if(pState->windows[windowIndex].id == newWindowId)
            {
                if(newWindowId == ~0u)
                {
                    STSE_Exit_exitFailure();
                }

                windowIdAlreadyUsed = true;
                ++newWindowId;
            }
        }
    }

    #ifdef STSE_GRAPHICS_API_VULKAN
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    #endif

    STSE_Memory_memzero(sizeof(struct STSE_Window_Window), &pState->windows[pState->windowCount]);
    
    pState->windows[pState->windowCount].id = newWindowId;
    pState->windows[pState->windowCount].pWindow = glfwCreateWindow(width, height, pTitle, NULL, NULL);

    if(pState->windows[pState->windowCount].pWindow == NULL)
    {
        STSE_Exit_exitFailure();
    }

    *pOutWindowId = newWindowId;
    ++pState->windowCount;
}

void STSE_Window_deleteWindow(const uint32_t windowId)
{
    uint32_t windowIndex = 0u;

    if(pState == NULL)
    {
        STSE_Exit_exitFailure();
    }

    for(windowIndex = 0u; windowIndex < pState->windowCount; ++windowIndex)
    {
        if(pState->windows[windowIndex].id == windowId)
        {
            glfwDestroyWindow(pState->windows[windowIndex].pWindow);
            
            if(pState->windowCount > 1u)
            {
                pState->windows[windowIndex] = pState->windows[pState->windowCount - 1u];
            }

            --pState->windowCount;
            return;
        }
    }

    STSE_Exit_exitFailure();
}

#ifdef STSE_GRAPHICS_API_VULKAN
    void STSE_Window_createVkSurface(
        const uint32_t windowId, 
        const VkInstance instance, 
        const VkAllocationCallbacks* pAllocator, 
        VkSurfaceKHR* pOutSurface)
    {
        uint32_t windowIndex = 0u;

        if(pState == NULL)
        {
            STSE_Exit_exitFailure();
        }

        for(windowIndex = 0u; windowIndex < pState->windowCount; ++windowIndex)
        {
            if(pState->windows[windowIndex].id == windowId)
            {
                if(glfwCreateWindowSurface(instance, pState->windows[windowIndex].pWindow, pAllocator, pOutSurface) != VK_SUCCESS)
                {
                    STSE_Exit_exitFailure();
                }
                return;
            }
        }
        
        STSE_Exit_exitFailure();
    }

    void STSE_Window_getRequiredVkInstanceExtensions(uint32_t* pOutExtensionCount, const char*** pppOutExtensionNames)
    {
        if(pState == NULL)
        {
            STSE_Exit_exitFailure();
        }
        
        *pppOutExtensionNames = glfwGetRequiredInstanceExtensions(pOutExtensionCount);
    }

#endif
