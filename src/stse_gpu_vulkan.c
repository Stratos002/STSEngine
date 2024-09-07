#ifdef STSE_USE_VULKAN

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

static void STSE_GPU_logVkError(const VkResult result, const char* pMessage)
{
    STSE_LOG_ERROR_ARGS("vulkan error : %s (result = %d)", pMessage, (int32_t)result);
}

/*
- variable declarations & initialization
- check input variables
- initialize out variables
- do the rest
*/

static enum STSE_Result STSE_GPU_createVkInstance(
    const char* pApplicationName, 
    const uint32_t layerCount, 
    const char** ppLayerNames, 
    const uint32_t extensionCount, 
    const char** ppExtensionNames, 
    VkInstance* pOutInstance)
{
    VkInstanceCreateInfo instanceCreateInfo = VK_NULL_HANDLE;
    VkResult vkResult = VK_SUCCESS;
    VkApplicationInfo applicationInfo = VK_NULL_HANDLE;

    *pOutInstance = VK_NULL_HANDLE;
    
    STSE_MEMORY_MEMZERO(sizeof(VkApplicationInfo), &applicationInfo);
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext = NULL;
    applicationInfo.pApplicationName = pApplicationName;
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = "STSEngine";
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.apiVersion = VK_MAKE_API_VERSION(1, 3, 0, 0);  

    STSE_MEMORY_MEMZERO(sizeof(VkInstanceCreateInfo), &instanceCreateInfo);
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = NULL;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledLayerCount = layerCount;
    instanceCreateInfo.ppEnabledLayerNames = ppLayerNames;
    instanceCreateInfo.enabledExtensionCount = extensionCount;
    instanceCreateInfo.ppEnabledExtensionNames = ppExtensionNames;

    vkResult = vkCreateInstance(&instanceCreateInfo, pState->pAllocator, &pState->instance);
    if(vkResult != VK_SUCCESS)
    {
        STSE_GPU_logVkError(vkResult, "could not create instance");
        return STSE_RESULT_GRAPHICS_API_FAILURE;
    }

    return STSE_RESULT_SUCCESS;
}

static enum STSE_Result STSE_GPU_pickVkPhysicalDevice(const VkInstance instance, VkPhysicalDevice* pOutPhysicalDevice)
{
    VkResult vkResult = VK_SUCCESS;
    enum STSE_Result result = STSE_RESULT_SUCCESS;
    uint32_t physicalDeviceCount = 0u;
    uint32_t physicalDeviceIndex = 0u;
    VkPhysicalDevice* pPhysicalDevices = NULL;
    VkPhysicalDeviceProperties physicalDeviceProperties = VK_NULL_HANDLE;

    STSE_ASSERT(instance != VK_NULL_HANDLE);
    
    *pOutPhysicalDevice = VK_NULL_HANDLE;
    
    vkResult = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, NULL);
    if(result != VK_SUCCESS)
    {
        STSE_GPU_logVkError(result, "could not enumerate physical devices");
        return STSE_RESULT_GRAPHICS_API_FAILURE;
    }

    if(physicalDeviceCount == 0u)
    {
        STSE_LOG_ERROR("no vulkan supporting device found");
        return STSE_RESULT_GRAPHICS_API_FAILURE;
    }

    result = STSE_MEMORY_ALLOCATE(sizeof(VkPhysicalDevice) * physicalDeviceCount, &pPhysicalDevices);
    if(result != STSE_RESULT_SUCCESS)
    {
        STSE_LOG_ERROR("failed to allocate memory for physical devices");
        return result;
    }
    
    vkResult = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, pPhysicalDevices);
    if(vkResult != VK_SUCCESS)
    {
        STSE_GPU_logVkError(vkResult, "could not enumerate physical devices");
        return STSE_RESULT_GRAPHICS_API_FAILURE;
    }
    
    for(physicalDeviceIndex = 0u; physicalDeviceIndex < physicalDeviceCount; ++physicalDeviceIndex)
    {
        vkGetPhysicalDeviceProperties(pPhysicalDevices[physicalDeviceIndex], &physicalDeviceProperties);
        if(physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            *pOutPhysicalDevice = pPhysicalDevices[physicalDeviceIndex];
        }
    }

    if(*pOutPhysicalDevice == VK_NULL_HANDLE)
    {
        STSE_LOG_DEBUG("no discrete GPU found, falling back on the first available physical device");
        *pOutPhysicalDevice = pPhysicalDevices[0];
    }

    vkGetPhysicalDeviceProperties(*pOutPhysicalDevice, &physicalDeviceProperties);
    STSE_LOG_INFO_ARGS("selected gpu : name : %s", physicalDeviceProperties.deviceName);

    result = STSE_MEMORY_DEALLOCATE(&pPhysicalDevices);
    if(result != STSE_RESULT_SUCCESS)
    {
        STSE_LOG_ERROR("failed to deallocate physical devices");
        return result;
    }
    
    return result;
}

static enum STSE_Result STSE_GPU_createVkDevice()
{
    return STSE_RESULT_SUCCESS;
}

enum STSE_Result STSE_GPU_initialize()
{
    enum STSE_Result result = STSE_RESULT_SUCCESS;
    VkResult vkResult = VK_SUCCESS;

    if(pState != NULL)
    {
        STSE_LOG_ERROR("gpu system was already initialized");
        return STSE_RESULT_ALREADY_INITIALIZED;
    }

    STSE_RESULT_RETURN_ERROR(volkInitialize(), 
        "failed to initialize Volk");

    STSE_RESULT_RETURN_ERROR(STSE_MEMORY_ALLOCATE(sizeof(struct STSE_GPU_State), &pState), 
        "failed to allocate memory for GPU state");
    
    STSE_MEMORY_MEMZERO(sizeof(struct STSE_GPU_State), pState);

    const char* pInstanceLayerNames[1] = {
        "VK_LAYER_KHRONOS_validation" // this layer is made available by setting a path to the SDK
    };

    const uint32_t instanceLayerCount = sizeof(pInstanceLayerNames) / sizeof(pInstanceLayerNames[0]);

    #ifndef STSE_CONFIGURATION_DEBUG
        instanceLayerCount -= 1u;
    #endif  

    const char** ppInstanceExtensionNames = NULL;
    const uint32_t instanceExtensionCount = 0u;

    STSE_RESULT_RETURN_ERROR(STSE_GPU_createVkInstance(
        "prout", 
        instanceLayerCount, 
        pInstanceLayerNames, 
        instanceExtensionCount, 
        ppInstanceExtensionNames,
        &pState->instance), 
            "failed to create vulkan instance");

    volkLoadInstance(pState->instance);

    STSE_RESULT_RETURN_ERROR(STSE_GPU_pickVkPhysicalDevice(pState->instance, &pState->physicalDevice), 
        "failed to pick a vulkan physical device");

    return STSE_RESULT_SUCCESS;
}

void STSE_GPU_terminate()
{
    if(pState != NULL)
    {
        vkDestroyInstance(pState->instance, pState->pAllocator);

        if(STSE_MEMORY_DEALLOCATE(&pState) != STSE_RESULT_SUCCESS)
        {
            STSE_LOG_WARNING("failed to deallocate GPU system");
        }
    }
}

#endif