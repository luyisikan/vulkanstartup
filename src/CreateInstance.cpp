#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <assert.h>
#include <memory>
#include <mutex>

#include <vulkan/vulkan.h>

/*
typedef enum VkPhysicalDeviceType {
    VK_PHYSICAL_DEVICE_TYPE_OTHER = 0,
    VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU = 1,
    VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU = 2,
    VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU = 3,
    VK_PHYSICAL_DEVICE_TYPE_CPU = 4,
    VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM = 0x7FFFFFFF
} VkPhysicalDeviceType;

typedef struct VkPhysicalDeviceProperties {
    uint32_t                            apiVersion;
    uint32_t                            driverVersion;
    uint32_t                            vendorID;
    uint32_t                            deviceID;
    VkPhysicalDeviceType                deviceType;
    char                                deviceName[VK_MAX_PHYSICAL_DEVICE_NAME_SIZE];
    uint8_t                             pipelineCacheUUID[VK_UUID_SIZE];
    VkPhysicalDeviceLimits              limits;
    VkPhysicalDeviceSparseProperties    sparseProperties;
} VkPhysicalDeviceProperties;

    VkBool32    geometryShader;
    VkBool32    tessellationShader;

typedef struct VkQueueFamilyProperties {
    VkQueueFlags    queueFlags;
    uint32_t        queueCount;
    uint32_t        timestampValidBits;
    VkExtent3D      minImageTransferGranularity;
} VkQueueFamilyProperties;

typedef struct VkExtent3D {
    uint32_t    width;
    uint32_t    height;
    uint32_t    depth;
} VkExtent3D;

typedef enum VkQueueFlagBits {
    VK_QUEUE_GRAPHICS_BIT = 0x00000001,
    VK_QUEUE_COMPUTE_BIT = 0x00000002,
    VK_QUEUE_TRANSFER_BIT = 0x00000004,
    VK_QUEUE_SPARSE_BINDING_BIT = 0x00000008,
    VK_QUEUE_PROTECTED_BIT = 0x00000010,
#ifdef VK_ENABLE_BETA_EXTENSIONS
    VK_QUEUE_VIDEO_DECODE_BIT_KHR = 0x00000020,
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
    VK_QUEUE_VIDEO_ENCODE_BIT_KHR = 0x00000040,
#endif
    VK_QUEUE_FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
} VkQueueFlagBits;
*/

static const char* DeviceTypesStrings[] = {
    "VK_PHYSICAL_DEVICE_TYPE_OTHER",
    "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU",
    "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU",
    "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU",
    "VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM"
};

static void showVkQueueFlag(VkQueueFlags flags) {
    if (flags & VK_QUEUE_GRAPHICS_BIT) std::cout << " - VK_QUEUE_GRAPHICS_BIT\n";
    if (flags & VK_QUEUE_COMPUTE_BIT) std::cout << " - VK_QUEUE_COMPUTE_BIT\n";
    if (flags & VK_QUEUE_TRANSFER_BIT) std::cout << " - VK_QUEUE_TRANSFER_BIT\n";
    if (flags & VK_QUEUE_SPARSE_BINDING_BIT) std::cout << " - VK_QUEUE_SPARSE_BINDING_BIT\n";
    if (flags & VK_QUEUE_PROTECTED_BIT) std::cout << " - VK_QUEUE_PROTECTED_BIT\n";
#ifdef VK_ENABLE_BETA_EXTENSIONS
    if (flags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) std::cout << " - VK_QUEUE_VIDEO_DECODE_BIT_KHR\n";
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
    if (flags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) std::cout << " - VK_QUEUE_VIDEO_ENCODE_BIT_KHR\n";
#endif
    if (flags & VK_QUEUE_FLAG_BITS_MAX_ENUM) std::cout << " - VK_QUEUE_FLAG_BITS_MAX_ENUM\n";
}

static void showPhysicalDevice(const VkPhysicalDevice &device) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    std::cout << "deviceID = " << deviceProperties.deviceID << "\n";
    std::cout << "deviceType = " << DeviceTypesStrings[deviceProperties.deviceType] << "\n";
    std::cout << "deviceName = " << deviceProperties.deviceName << "\n";

    std::cout << "geometryShader = " << deviceFeatures.geometryShader << "\n";
    std::cout << "tessellationShader = " << deviceFeatures.tessellationShader << "\n";
}

static void showQueueFamily(const VkPhysicalDevice &device) {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    
    for (const auto& qf : queueFamilies) {
        std::cout << "-------\n";
        std::cout << "queueFlags = " << qf.queueFlags << "\n";
        showVkQueueFlag(qf.queueFlags);
        std::cout << "queueCount = " << qf.queueCount << "\n";
        std::cout << "timestampValidBits = " << qf.timestampValidBits << "\n";
        std::cout << "minImageTransferGranularity.width = " << qf.minImageTransferGranularity.width << "\n";
        std::cout << "minImageTransferGranularity.height = " << qf.minImageTransferGranularity.height << "\n";
        std::cout << "minImageTransferGranularity.depth = " << qf.minImageTransferGranularity.depth << "\n";
    }
}

int main() {
	std::cout << "Hello Vulkan : Create New Instance\n";

    VkInstance instance;

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

    if (result != VK_SUCCESS) {
        std::cerr << "Failed to create instance!\n";
        return -1;
    }
    else {
        std::cout << "Success to create instance!\n";
    }

    // try to fetch physical device
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        std::cerr << "Unable to find any physical device!\n";
    }
    else {
        std::cout << "Find " << deviceCount << " physical devices\n";
    }

    // VkPhysicalDevice is onley one handler
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // list all physical devices
    for (const auto& device : devices) {
        showPhysicalDevice(device);
        std::cout << "\n";
        showQueueFamily(device);
        std::cout << "\n";
    }

    // start to fech queue families

}