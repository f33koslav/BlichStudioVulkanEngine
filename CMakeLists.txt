cmake_minimum_required(VERSION 3.24)
project(StudioVulkanEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_library(engine SHARED
    src/main.cpp
    src/render/vulkan_context.cpp
    src/render/device.cpp
    src/render/swapchain.cpp
)

target_include_directories(engine PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(engine PUBLIC Vulkan::Vulkan glfw)

add_executable(engine_exe src/main.cpp)

target_include_directories(engine_exe PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(engine_exe PRIVATE engine Vulkan::Vulkan glfw)
