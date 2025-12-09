#include <iostream>
#include <vector>
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
    result = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (result != VK_SUCCESS || deviceCount == 0) {
        std::cerr << "No GPU with Vulkan support found!" << std::endl;
        vkDestroyInstance(instance, nullptr);
        return 1;
    }

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

    VkPhysicalDevice chosenDevice = VK_NULL_HANDLE;
    int computeQueueFamilyIndex = -1;

    for (uint32_t i = 0; i < deviceCount; ++i) {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, nullptr);
        if (queueFamilyCount == 0) continue;

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, queueFamilies.data());

        // Prefer compute-only queue (compute bit set, graphics bit not set)
        for (uint32_t j = 0; j < queueFamilyCount; ++j) {
            if ((queueFamilies[j].queueFlags & VK_QUEUE_COMPUTE_BIT) &&
                !(queueFamilies[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
                chosenDevice = physicalDevices[i];
                computeQueueFamilyIndex = static_cast<int>(j);
                break;
            }
        }
        if (chosenDevice != VK_NULL_HANDLE) break;

        // Fallback: any queue with compute bit
        for (uint32_t j = 0; j < queueFamilyCount; ++j) {
            if (queueFamilies[j].queueFlags & VK_QUEUE_COMPUTE_BIT) {
                chosenDevice = physicalDevices[i];
                computeQueueFamilyIndex = static_cast<int>(j);
                break;
            }
        }
        if (chosenDevice != VK_NULL_HANDLE) break;
    }

    if (chosenDevice == VK_NULL_HANDLE) {
        std::cerr << "No suitable compute-capable GPU found!" << std::endl;
        vkDestroyInstance(instance, nullptr);
        return 1;
    }

    // Create logical device with one queue from the chosen family
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = static_cast<uint32_t>(computeQueueFamilyIndex);
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.pEnabledFeatures = nullptr;

    VkDevice device = VK_NULL_HANDLE;
    if (vkCreateDevice(chosenDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
        std::cerr << "Failed to create logical device!" << std::endl;
        vkDestroyInstance(instance, nullptr);
        return 1;
    }

    VkQueue computeQueue;
    vkGetDeviceQueue(device, computeQueueFamilyIndex, 0, &computeQueue);

    std::cout << "Device and compute queue successfully created!" << std::endl;

    // Cleanup
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);
    return 0;
}