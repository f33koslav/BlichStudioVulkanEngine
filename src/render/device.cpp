#include "device.h"
#include <iostream>
#include <set>

#ifndef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
#define VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME "VK_KHR_portability_subset"
#endif

QueueFamilyIndices Device::findQueueFamilies(vk::PhysicalDevice device) {
    QueueFamilyIndices indices;
    
    auto queueFamilies = device.getQueueFamilyProperties();
    
    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphicsFamily = i;
        }
        
        if (device.getSurfaceSupportKHR(i, surface)) {
            indices.presentFamily = i;
        }
        
        if (indices.isComplete()) break;
        i++;
    }
    
    return indices;
}

bool Device::isDeviceSuitable(vk::PhysicalDevice device) {
    auto indices = findQueueFamilies(device);
    return indices.isComplete();
}

void Device::pickPhysicalDevice(vk::Instance instance, vk::SurfaceKHR surf) {
    surface = surf;
    
    auto devices = instance.enumeratePhysicalDevices();
    
    if (devices.empty()) {
        throw std::runtime_error("No Vulkan devices found!");
    }
    
    std::cout << "\n✨ Found " << devices.size() << " GPU(s):" << std::endl;
    
    for (size_t i = 0; i < devices.size(); ++i) {
        auto props = devices[i].getProperties();
        std::cout << "  [" << i << "] " << props.deviceName.data() << std::endl;
    }
    
    // Pick first suitable device
    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            auto props = physicalDevice.getProperties();
            std::cout << "\n✅ Selected: " << props.deviceName.data() << std::endl;
            return;
        }
    }
    
    throw std::runtime_error("No suitable Vulkan device found!");
}

void Device::createLogicalDevice() {
    queueFamilies = findQueueFamilies(physicalDevice);
    
    std::set<uint32_t> uniqueQueueFamilies = {
        queueFamilies.graphicsFamily.value(),
        queueFamilies.presentFamily.value()
    };
    
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    float queuePriority = 1.0f;
    
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        vk::DeviceQueueCreateInfo queueCreateInfo(
            vk::DeviceQueueCreateFlags(),
            queueFamily,
            1,
            &queuePriority
        );
        queueCreateInfos.push_back(queueCreateInfo);
    }
    
    vk::PhysicalDeviceFeatures deviceFeatures;
    
    std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
    };
    
    vk::DeviceCreateInfo createInfo(
        vk::DeviceCreateFlags(),
        static_cast<uint32_t>(queueCreateInfos.size()),
        queueCreateInfos.data(),
        0,
        nullptr,
        static_cast<uint32_t>(deviceExtensions.size()),
        deviceExtensions.data(),
        &deviceFeatures
    );
    
    try {
        logicalDevice = physicalDevice.createDevice(createInfo);
    } catch (const std::exception& e) {
        std::cerr << "❌ Device creation failed: " << e.what() << std::endl;
        throw;
    }
    
    graphicsQueue = logicalDevice.getQueue(queueFamilies.graphicsFamily.value(), 0);
    presentQueue = logicalDevice.getQueue(queueFamilies.presentFamily.value(), 0);
    
    std::cout << "✅ Logical device created" << std::endl;
    std::cout << "  Graphics queue family: " << queueFamilies.graphicsFamily.value() << std::endl;
    std::cout << "  Present queue family: " << queueFamilies.presentFamily.value() << std::endl;
}

void Device::destroy() {
    if (logicalDevice) {
        logicalDevice.destroy();
        logicalDevice = nullptr;
    }
}
