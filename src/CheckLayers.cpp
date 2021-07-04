#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <assert.h>
#include <memory>
#include <mutex>

#include <vulkan/vulkan.h>

int main() {
	std::cout << "Hello Vulkan : Checking Layers\n";

    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::cout << "Get Layers Count = " << layerCount << "\n";

	// get all extension properties
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// list all available extension properties
	std::cout << "Available Layers:\n";

	/**
	* typedef struct VkLayerProperties {
    *   char        layerName[VK_MAX_EXTENSION_NAME_SIZE];
    *   uint32_t    specVersion;
    *   uint32_t    implementationVersion;
    *   char        description[VK_MAX_DESCRIPTION_SIZE];
	* } VkLayerProperties;
	*/

	for (const auto& layer : availableLayers) {
		std::cout << "-" << layer.layerName << "\n"
			<< "  - specVersion :" << layer.specVersion  << '\n'
			<< "  - specVersion :" << layer.implementationVersion << '\n'
			<< "  - description :" << layer.description << '\n'
			;
	}

	return 1;

}