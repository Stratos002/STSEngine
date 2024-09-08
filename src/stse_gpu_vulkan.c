#include "stse_exit.h"
#include "stse_gpu.h"
#include "stse_memory.h"
#include "stse_log.h"

#define VOLK_IMPLEMENTATION
#include "volk.h"

struct STSE_GPU_State
{
    VkInstance instance;
    VkAllocationCallbacks* pAllocator;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
};

static struct STSE_GPU_State* pState = NULL;

static void STSE_GPU_createVkInstance(const VkAllocationCallbacks* pAllocator, VkInstance* pOutInstance)
{
    VkApplicationInfo applicationInfo;
    VkInstanceCreateInfo instanceCreateInfo;
    uint32_t instanceLayerCount;
    const char* pInstanceLayerNames[1] = {
        "VK_LAYER_KHRONOS_validation"
    };

    #ifdef STSE_CONFIGURATION_DEBUG
        instanceLayerCount = 1u;
    #else
        instanceLayerCount = 0u;
    #endif

    STSE_Memory_memzero(sizeof(VkApplicationInfo), &applicationInfo);
    STSE_Memory_memzero(sizeof(VkInstanceCreateInfo), &instanceCreateInfo);

    if(*pOutInstance != VK_NULL_HANDLE)
    {
        STSE_EXIT_exitFailure();
    }

    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext = NULL;
    applicationInfo.pApplicationName = "STSEngine_Application";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = "STSEngine";
    applicationInfo.apiVersion = VK_MAKE_API_VERSION(1, 3, 0, 0);

    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = NULL;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledLayerCount = instanceLayerCount;
    instanceCreateInfo.ppEnabledLayerNames = pInstanceLayerNames;
    instanceCreateInfo.enabledExtensionCount = 0u;
    instanceCreateInfo.ppEnabledExtensionNames = NULL;

    if(vkCreateInstance(&instanceCreateInfo, pAllocator, pOutInstance) != VK_SUCCESS)
    {
        STSE_EXIT_exitFailure();
    }
}

static void STSE_GPU_pickVkPhysicalDevice(const VkInstance instance, VkPhysicalDevice* pOutPhysicalDevice)
{
    uint32_t physicalDeviceCount = 0u;
    VkPhysicalDevice* pPhysicalDevices = NULL;
    uint32_t physicalDeviceIndex = 0u;
    VkPhysicalDeviceProperties physicalDeviceProperties;

    if(instance == VK_NULL_HANDLE)
    {
        STSE_EXIT_exitFailure();
    }

    if(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, pPhysicalDevices) != VK_SUCCESS)
    {
        STSE_EXIT_exitFailure();
    }

    if(physicalDeviceCount == 0u)
    {
        STSE_EXIT_exitFailure();
    }

    STSE_Memory_allocate(sizeof(VkPhysicalDevice) * physicalDeviceCount, (void**)&pPhysicalDevices);

    if(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, pPhysicalDevices) != VK_SUCCESS)
    {
        STSE_EXIT_exitFailure();
    }

    *pOutPhysicalDevice = pPhysicalDevices[0];

    for(physicalDeviceIndex = 0u; physicalDeviceIndex < physicalDeviceCount; ++physicalDeviceIndex)
    {
        vkGetPhysicalDeviceProperties(pPhysicalDevices[physicalDeviceIndex], &physicalDeviceProperties);
        if(physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            *pOutPhysicalDevice = pPhysicalDevices[physicalDeviceIndex];
            break;
        }
    }

    vkGetPhysicalDeviceProperties(*pOutPhysicalDevice, &physicalDeviceProperties);
    STSE_LOG_log(STSE_LOG_OUTPUT_STANDARD, "selected vulkan device : %s", physicalDeviceProperties.deviceName);

    STSE_Memory_deallocate(pPhysicalDevices);
}

static void STSE_GPU_createVkDevice(const VkPhysicalDevice physicalDevice, const VkAllocationCallbacks* pAllocator, VkDevice* pOutDevice)
{
    VkDeviceCreateInfo deviceCreateInfo;

    STSE_Memory_memzero(sizeof(VkDeviceCreateInfo), &deviceCreateInfo);

    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = NULL;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.queueCreateInfoCount = 0u;
    deviceCreateInfo.pQueueCreateInfos = NULL;
    deviceCreateInfo.enabledLayerCount = 0u;
    deviceCreateInfo.ppEnabledLayerNames = NULL;
    deviceCreateInfo.enabledExtensionCount = 0u;
    deviceCreateInfo.ppEnabledExtensionNames = NULL;
    deviceCreateInfo.pEnabledFeatures = NULL;

    if(vkCreateDevice(physicalDevice, &deviceCreateInfo, pAllocator, pOutDevice) != VK_SUCCESS)
    {
        STSE_EXIT_exitFailure();
    }
}

void STSE_GPU_initialize(void)
{
    if(pState != NULL)
    {
        STSE_EXIT_exitFailure();
    }

    STSE_Memory_allocate(sizeof(struct STSE_GPU_State), (void**)&pState);
    STSE_Memory_memzero(sizeof(struct STSE_GPU_State), pState);

    if(volkInitialize() != VK_SUCCESS)
    {
        STSE_EXIT_exitFailure();
    }

    STSE_GPU_createVkInstance(pState->pAllocator, &pState->instance);

    volkLoadInstance(pState->instance);

    STSE_GPU_pickVkPhysicalDevice(pState->instance, &pState->physicalDevice);

    STSE_GPU_createVkDevice(pState->physicalDevice, pState->pAllocator, &pState->device);

    volkLoadDevice(pState->device);
}

void STSE_GPU_terminate(void)
{
    if(pState != NULL)
    {
        vkDestroyDevice(pState->device, pState->pAllocator);
        vkDestroyInstance(pState->instance, pState->pAllocator);
        
        STSE_Memory_deallocate(pState);
        pState = NULL;
    }
}
