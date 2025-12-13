#include "swapchain.h"
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <algorithm>
#include <limits>
#include <cstring>

class Device;

vk::SurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && 
            availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

vk::PresentModeKHR Swapchain::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
            return availablePresentMode;
        }
    }
    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D Swapchain::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        vk::Extent2D actualExtent = {
            std::clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
            std::clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
        };
        return actualExtent;
    }
}

void Swapchain::create(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, 
                       vk::SurfaceKHR surface, uint32_t width, uint32_t height) {
    auto capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
    auto formats = physicalDevice.getSurfaceFormatsKHR(surface);
    auto presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
    
    auto surfaceFormat = chooseSwapSurfaceFormat(formats);
    auto presentMode = chooseSwapPresentMode(presentModes);
    extent = chooseSwapExtent(capabilities, width, height);
    imageFormat = surfaceFormat.format;
    
    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
        imageCount = capabilities.maxImageCount;
    }
    
    vk::SwapchainCreateInfoKHR createInfo(
        vk::SwapchainCreateFlagsKHR(),
        surface,
        imageCount,
        imageFormat,
        surfaceFormat.colorSpace,
        extent,
        1,
        vk::ImageUsageFlagBits::eColorAttachment
    );
    
    swapchain = logicalDevice.createSwapchainKHR(createInfo);
    images = logicalDevice.getSwapchainImagesKHR(swapchain);
    
    std::cout << "✅ Swapchain created: " << images.size() << " images, " 
              << extent.width << "x" << extent.height << std::endl;
    
    createImageViews(logicalDevice);
    createRenderPass(logicalDevice);
    createFramebuffers(logicalDevice);
}

void Swapchain::createImageViews(vk::Device device) {
    imageViews.resize(images.size());
    
    for (size_t i = 0; i < images.size(); i++) {
        vk::ImageViewCreateInfo createInfo(
            vk::ImageViewCreateFlags(),
            images[i],
            vk::ImageViewType::e2D,
            imageFormat,
            vk::ComponentMapping(),
            vk::ImageSubresourceRange(
                vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1
            )
        );
        imageViews[i] = device.createImageView(createInfo);
    }
    
    std::cout << "✅ " << imageViews.size() << " image views created" << std::endl;
}

void Swapchain::createRenderPass(vk::Device device) {
    vk::AttachmentDescription colorAttachment{};
    colorAttachment.format = imageFormat;
    colorAttachment.samples = vk::SampleCountFlagBits::e1;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

    vk::AttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::SubpassDescription subpass{};
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    vk::RenderPassCreateInfo renderPassInfo{};
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    renderPass = device.createRenderPass(renderPassInfo);
    std::cout << "✅ Render pass created" << std::endl;
}

void Swapchain::createFramebuffers(vk::Device device) {
    framebuffers.resize(imageViews.size());
    
    for (size_t i = 0; i < imageViews.size(); i++) {
        vk::FramebufferCreateInfo framebufferInfo(
            vk::FramebufferCreateFlags(),
            renderPass,
            1,
            &imageViews[i],
            extent.width,
            extent.height,
            1
        );
        
        framebuffers[i] = device.createFramebuffer(framebufferInfo);
    }
    
    std::cout << "✅ " << framebuffers.size() << " framebuffers created" << std::endl;
}

void Swapchain::destroy(vk::Device device) {
    for (auto fb : framebuffers) {
        device.destroyFramebuffer(fb);
    }
    for (auto view : imageViews) {
        device.destroyImageView(view);
    }
    device.destroyRenderPass(renderPass);
    device.destroySwapchainKHR(swapchain);
    std::cout << "✅ Swapchain cleaned up" << std::endl;
}