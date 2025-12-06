cmake_minimum_required(VERSION 3.20)
project(StudioVulkanEngine LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Conan 2: all package configs live in build/Release/generators
if(NOT DEFINED CONAN_GENERATORS_DIR)
    set(CONAN_GENERATORS_DIR "${CMAKE_BINARY_DIR}/Release/generators")
endif()
list(APPEND CMAKE_PREFIX_PATH "${CONAN_GENERATORS_DIR}")

find_package(glm REQUIRED CONFIG)
find_package(assimp REQUIRED CONFIG)
find_package(Bullet REQUIRED CONFIG)          # <- this is what Conan generated
find_package(sol2 REQUIRED CONFIG)
find_package(lua REQUIRED CONFIG)
find_package(pybind11 REQUIRED CONFIG)
find_package(imgui REQUIRED CONFIG)
find_package(OpenAl REQUIRED CONFIG)
find_package(SPIRV-Tools REQUIRED CONFIG)     # present in your generators
find_package(VulkanLoader REQUIRED CONFIG)    # present in your generators
find_package(VulkanHeaders REQUIRED CONFIG)

# GLFW from Homebrew
find_path(GLFW_INCLUDE_DIR GLFW/glfw3.h
  HINTS /usr/local/include /opt/homebrew/include
)
find_library(GLFW_LIBRARY NAMES glfw glfw3
  HINTS /usr/local/lib /opt/homebrew/lib
)
if(NOT GLFW_INCLUDE_DIR OR NOT GLFW_LIBRARY)
  message(FATAL_ERROR "GLFW not found. Install via 'brew install glfw'.")
endif()

add_executable(engine
    src/main.cpp
)

target_include_directories(engine PRIVATE
    src/
    ${GLFW_INCLUDE_DIR}
)

target_link_libraries(engine PRIVATE
    ${GLFW_LIBRARY}
    Vulkan::Loader
    glm::glm
    assimp::assimp
    Bullet::Bullet
    sol2::sol2
    lua::lua
    pybind11::pybind11
    imgui::imgui
    OpenAL::OpenAL
)

if(APPLE)
    find_library(COCOA_FRAMEWORK Cocoa)
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(METAL_FRAMEWORK Metal)
    target_link_libraries(engine PRIVATE
        ${COCOA_FRAMEWORK}
        ${IOKIT_FRAMEWORK}
        ${METAL_FRAMEWORK}
    )
endif()
