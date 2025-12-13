#include "vulkan_context.h"
#include <iostream>
#include <vector>
#include <cstring>

static VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
    std::cerr << "[VULKAN] " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

VulkanContext::VulkanContext() {}

VulkanContext::~VulkanContext() {
    cleanup();
}

void VulkanContext::init(GLFWwindow* window) {
    std::cout << "Initializing Vulkan..." << std::endl;
    
    // Get GLFW extensions
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    
    std::vector<const char*> layers;
    if (enableValidationLayers) {
        layers.push_back("VK_LAYER_KHRONOS_validation");
    }
    
    // Create instance
    vk::ApplicationInfo appInfo(
        "Studio Vulkan Engine",
        VK_MAKE_VERSION(1, 0, 0),
        "StudioVulkanEngine",
        VK_MAKE_VERSION(1, 0, 0),
        VK_API_VERSION_1_3
    );
    
    vk::InstanceCreateInfo createInfo(
        vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR,
        &appInfo,
        static_cast<uint32_t>(layers.size()),
        layers.data(),
        static_cast<uint32_t>(extensions.size()),
        extensions.data()
    );
    
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugCreateInfo.pfnUserCallback = debugCallback;
    debugCreateInfo.pUserData = nullptr;
    
    createInfo.pNext = &debugCreateInfo;
    
    try {
        instance = vk::createInstance(createInfo);
        std::cout << "✅ Vulkan instance created" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "❌ Failed to create Vulkan instance: " << e.what() << std::endl;
        throw;
    }
    
    // Create surface
    VkSurfaceKHR vkSurface;
    if (glfwCreateWindowSurface(static_cast<VkInstance>(instance), window, nullptr, &vkSurface) != VK_SUCCESS) {
        std::cerr << "❌ Failed to create window surface" << std::endl;
        throw std::runtime_error("Surface creation failed");
    }
    surface = vk::SurfaceKHR(vkSurface);
    std::cout << "✅ Window surface created" << std::endl;
    
    // Create device
    device = std::make_unique<Device>();
    device->pickPhysicalDevice(instance, surface);
    device->createLogicalDevice();
    logicalDevice = device->getLogicalDevice();
    swapchain.create(
        device->getPhysicalDevice(),
        device->getLogicalDevice(),
        surface,
        1280, 720
    );
    
    std::cout << "✅ Vulkan initialization complete" << std::endl;
}

void VulkanContext::cleanup() {
    if (!instance) return;
    
    // Swapchain FIRST (uses device)
    if (swapchain.getSwapchain()) {
        swapchain.destroy(device->getLogicalDevice());
    }
    
    // Device SECOND
    if (device && device->isValid()) {
        device->destroy();
    }
    
    if (surface) {
        instance.destroySurfaceKHR(surface);
        surface = nullptr;
    }
    
    if (instance) {
        instance.destroy();
    }
    std::cout << "✅ Vulkan cleanup complete" << std::endl;
}
