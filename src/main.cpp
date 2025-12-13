#include <iostream>
#include <GLFW/glfw3.h>
#include "render/vulkan_context.h"

int main() {
    std::cout << "🚀 Studio Vulkan Engine Starting..." << std::endl;
    
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Studio Vulkan Engine", nullptr, nullptr);
    
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    VulkanContext vk;
    vk.init(window);
    
    std::cout << "✅ GLFW window created! Vulkan initialization complete..." << std::endl;
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
    
    vk.cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "🎮 Engine shutdown complete" << std::endl;
    return 0;
}
