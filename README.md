# Studio Vulkan Engine

Custom C++ 3D engine sandbox using Vulkan (MoltenVK on macOS), GLFW, Conan 2, Lua (via sol2), Python (via pybind11), Assimp, Bullet, ImGui, and OpenAL.

Tested on macOS with AppleClang and Homebrew.

---

## 1. Prerequisites

### System packages (macOS)

Install via Homebrew:

```
brew install cmake ninja git python@3.11 llvm
brew install glfw vulkan-headers vulkan-loader molten-vk
brew install helix
```

Verify Vulkan and GLFW are present:

```
brew list | grep -i vulkan
brew list | grep -i glfw
```

### Node / LSP (optional, for Helix/IDE)

```
npm install -g pyright lua-language-server
```

---

## 2. Python & Conan Setup

From your preferred workspace directory:

```
cd <your-workspace>
python3 -m venv .venv
source .venv/bin/activate
pip install conan pybind11
conan profile detect --force
conan --version
```

---

## 3. Clone / Create Project

```
cd <your-workspace>
git clone <this-repo-url> StudioVulkanEngine
cd StudioVulkanEngine
```

---

## 4. Conan Dependencies

```
cd <your-workspace>/StudioVulkanEngine
source ../.venv/bin/activate
conan install . --build=missing -s build_type=Release -s os=Macos -s arch=x86_64
```

---

## 5. Configure & Build

```
mkdir -p build
cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
ninja -j8
```

---

## 6. Run the Engine

```
./engine
```

---

## 7. Vulkan Environment Notes (macOS / MoltenVK)

If Vulkan fails to initialize, set these environment variables (adjust paths based on your Homebrew prefix):

```
# Intel Mac (usually /usr/local)
export VK_ICD_FILENAMES=/usr/local/share/vulkan/icd.d/MoltenVK_icd.json
export VK_LAYER_PATH=/usr/local/share/vulkan/explicit_layer.d

# Apple Silicon (usually /opt/homebrew)
export VK_ICD_FILENAMES=/opt/homebrew/share/vulkan/icd.d/MoltenVK_icd.json
export VK_LAYER_PATH=/opt/homebrew/share/vulkan/explicit_layer.d
```

Find your Homebrew prefix with:

```
brew --prefix
```

---

## 8. Tech Stack

- C++17, CMake, Ninja, Conan 2
- GLFW, Vulkan (MoltenVK)
- glm, Assimp, Bullet
- Lua + sol2, Python + pybind11
- ImGui, OpenAL
