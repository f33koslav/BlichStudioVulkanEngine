#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <memory>
#include "device.h"
#include "swapchain.h"

class Device;

class VulkanContext {
public:
    VulkanContext();
    ~VulkanContext();
    
    void init(GLFWwindow* window);
    void cleanup();
    
    vk::Instance getInstance() const { return instance; }
    Device& getDevice() { return *device; }
    vk::SurfaceKHR getSurface() const { return surface; }
    vk::Device getLogicalDevice() const { return logicalDevice; }
    
private:
    vk::Instance instance;
    std::unique_ptr<Device> device;
    vk::SurfaceKHR surface;
    vk::DebugUtilsMessengerEXT debugMessenger;
    vk::Device logicalDevice;
    Swapchain swapchain;
    
    bool enableValidationLayers = true;
};
