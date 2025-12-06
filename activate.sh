#!/bin/bash
cd ~/BlichStudio/StudioVulkanEngine
source ~/BlichStudio/.venv/bin/activate
export VK_ICD_FILENAMES=/opt/homebrew/share/vulkan/icd.d/MoltenVK_icd.json
export VK_LAYER_PATH=/opt/homebrew/share/vulkan/explicit_layer.d
echo "🎮 Studio Vulkan + Lua + Python active"
