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
 
    VkPhysicalDevice* physicalDevices = new VkPhysicalDevice[deviceCount];
    vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices);
 
    delete[] physicalDevices;  // 마지막에 메모리 해제
    vkDestroyInstance(instance, nullptr);
    return 0;
}
