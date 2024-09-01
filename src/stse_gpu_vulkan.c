#ifdef STSE_USE_VULKAN

#include "stse_gpu.h"
#include "stse_memory.h"
#include "stse_log.h"

#include "volk.h"

struct STSE_GPU_State
{
    VkInstance instance;
    VkAllocationCallbacks* pAllocator;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
};

static struct STSE_GPU_State* pState = NULL;

static enum STSE_Result STSE_GPU_checkVkResult(const VkResult result)
{
    if(result != VK_SUCCESS)
    {
        STSE_LOG_ERROR("vulkan error : %d", result);
        return STSE_RESULT_GRAPHICS_API_FAILURE;
    }
    return STSE_RESULT_SUCCESS;
}

static enum STSE_Result STSE_GPU_createVkInstance(
    const char* pApplicationName, 
    const uint32_t layerCount, 
    const char** ppLayerNames, 
    const uint32_t extensionCount, 
    const char** ppExtensionNames, 
    VkInstance* pOutInstance)
{
    VkApplicationInfo applicationInfo;
    STSE_MEMORY_MEMZERO(sizeof(VkApplicationInfo), &applicationInfo);
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext = NULL;
    applicationInfo.pApplicationName = pApplicationName;
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = "STSEngine";
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.apiVersion = VK_MAKE_API_VERSION(1, 3, 0, 0);  

    VkInstanceCreateInfo instanceCreateInfo;
    STSE_MEMORY_MEMZERO(sizeof(VkInstanceCreateInfo), &instanceCreateInfo);
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = NULL;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledLayerCount = layerCount;
    instanceCreateInfo.ppEnabledLayerNames = ppLayerNames;
    instanceCreateInfo.enabledExtensionCount = extensionCount;
    instanceCreateInfo.ppEnabledExtensionNames = ppExtensionNames;

    STSE_RESULT_RETURN_IF_ERROR(STSE_GPU_checkVkResult(vkCreateInstance(&instanceCreateInfo, pState->pAllocator, &pState->instance)), 
        "failed to create VkInstance");

    return STSE_RESULT_SUCCESS;
}

static enum STSE_Result STSE_GPU_pickVkPhysicalDevice(const VkInstance instance, VkPhysicalDevice* pOutPhysicalDevice)
{
    *pOutPhysicalDevice = VK_NULL_HANDLE;
    uint32_t physicalDeviceCount = 0u;
    STSE_RESULT_RETURN_IF_ERROR(STSE_GPU_checkVkResult(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, NULL))
        ,"failed to enumerate physical devices");
    if(physicalDeviceCount == 0u)
    {
        STSE_LOG_ERROR("no vulkan supporting device found");
        return STSE_RESULT_GRAPHICS_API_FAILURE;
    }

    VkPhysicalDevice* pPhysicalDevices = NULL;
    STSE_RESULT_RETURN_ERROR(STSE_MEMORY_ALLOCATE(sizeof(VkPhysicalDevice) * physicalDeviceCount, &pPhysicalDevices), 
        "failed to allocate memory for physical devices");
    
    STSE_GPU_RETURN_ERROR_VK(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, pPhysicalDevices));
    
    for(uint32_t physicalDeviceIndex = 0u; physicalDeviceIndex < physicalDeviceCount; ++physicalDeviceIndex)
    {
        VkPhysicalDeviceProperties physicalDeviceProperties;
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
        
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(pPhysicalDevices[0], &physicalDeviceProperties);
        STSE_LOG_INFO_ARGS("selected gpu : name : %s", physicalDeviceProperties.deviceName);
        
        STSE_RESULT_RETURN_ERROR(STSE_MEMORY_DEALLOCATE(&pPhysicalDevices), 
            "failed to deallocate physical devices");
    }
    
    return STSE_RESULT_SUCCESS;
}

static enum STSE_Result STSE_GPU_createVkDevice()
{
    STSE_GPU_RETURN_ERROR_VK()

    return STSE_RESULT_SUCCESS;
}

enum STSE_Result STSE_GPU_initialize()
{
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