#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
#include <optional>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    
    bool isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

class Device {
public:
    Device() = default;
    ~Device() = default;
    
    void pickPhysicalDevice(vk::Instance instance, vk::SurfaceKHR surface);
    void createLogicalDevice();
    void destroy();
    
    vk::PhysicalDevice getPhysicalDevice() const { return physicalDevice; }
    vk::Device getLogicalDevice() const { return logicalDevice; }
    bool isValid() const { return static_cast<bool>(logicalDevice); }
    vk::Queue getGraphicsQueue() const { return graphicsQueue; }
    vk::Queue getPresentQueue() const { return presentQueue; }
    QueueFamilyIndices getQueueFamilies() const { return queueFamilies; }
    
private:
    vk::PhysicalDevice physicalDevice;
    vk::Device logicalDevice;
    vk::Queue graphicsQueue;
    vk::Queue presentQueue;
    QueueFamilyIndices queueFamilies;
    vk::SurfaceKHR surface;
    
    QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device);
    bool isDeviceSuitable(vk::PhysicalDevice device);
};
