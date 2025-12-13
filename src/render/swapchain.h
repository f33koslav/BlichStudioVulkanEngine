#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>

class Swapchain {
public:
    Swapchain() = default;
    ~Swapchain() = default;
    
    void create(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, 
                vk::SurfaceKHR surface, uint32_t width, uint32_t height);
    void destroy(vk::Device device);
    
    // Getters
    vk::SwapchainKHR getSwapchain() const { return swapchain; }
    vk::RenderPass getRenderPass() const { return renderPass; }
    std::vector<vk::Framebuffer> getFramebuffers() const { return framebuffers; }
    vk::Extent2D getExtent() const { return extent; }
    vk::Format getImageFormat() const { return imageFormat; }
    uint32_t getImageCount() const { return images.size(); }
    
private:
    vk::SwapchainKHR swapchain;
    vk::RenderPass renderPass;
    std::vector<vk::Image> images;
    std::vector<vk::ImageView> imageViews;
    std::vector<vk::Framebuffer> framebuffers;
    vk::Extent2D extent;
    vk::Format imageFormat;
    
    // Helper methods
    vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& available);
    vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& available);
    vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height);
    void createImageViews(vk::Device device);
    void createRenderPass(vk::Device device);
    void createFramebuffers(vk::Device device);
};
