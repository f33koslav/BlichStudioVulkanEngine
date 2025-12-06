#!/bin/bash
set -e

echo "🚀 Studio Vulkan Engine Build (macOS)"

# Activate Studio Python environment (relative path)
source ../.venv/bin/activate

# Vulkan environment for MoltenVK
export VK_ICD_FILENAMES=/opt/homebrew/share/vulkan/icd.d/MoltenVK_icd.json
export VK_LAYER_PATH=/opt/homebrew/share/vulkan/explicit_layer.d

mkdir -p build
cd build

conan install .. --build=missing -s build_type=Release \
                 -s os=Macos -s arch=arm64

cmake .. -G Ninja \
         -DCMAKE_BUILD_TYPE=Release \
         -DCMAKE_OSX_ARCHITECTURES=arm64

ninja -j8

echo "✅ Engine ready!"
echo "Run: VK_ICD_FILENAMES=$VK_ICD_FILENAMES ./engine"
