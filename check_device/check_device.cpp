#include <iostream>
#include <vulkan/vulkan.h>

int main() {
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    VkInstance instance;
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan instance!" << std::endl;
        return 1;
    }
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);

    if (deviceCount == 0) {
      fprintf(stderr, "No GPU with Vulkan support found!\n");
      return 1;
    }

    std::cout << "Found " << deviceCount << " GPU(s)" << std::endl;

    VkPhysicalDevice* physicalDevices = new VkPhysicalDevice[deviceCount];
    vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices);

    // Device 정보 출력
    for (uint32_t i = 0; i < deviceCount; i++) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);
        
        std::cout << "\nDevice " << i << ":" << std::endl;
        std::cout << "  Name: " << properties.deviceName << std::endl;
        std::cout << "  Type: " << properties.deviceType << std::endl;
        std::cout << "  Driver Version: " << properties.driverVersion << std::endl;
    }

    delete[] physicalDevices;
    vkDestroyInstance(instance, nullptr);
    return 0;
}
