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
	std::cout << "Hello Vulkan : Checking Logical Devices\n";

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::cout << "Get Extension Count = " << extensionCount << "\n";

	// get all extension properties
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, 
											extensions.data());

	// list all available extension properties
	std::cout << "available extensions:\n";

	for (const auto& extension : extensions) {
		std::cout << '\t' << extension.extensionName << " : specVersion " << extension.specVersion  << '\n';
	}

	return 1;
}