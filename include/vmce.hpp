#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <string.h>
#include <vector>

namespace vmce
{
    inline const std::vector<const char *> validation_layers = {
        "VK_LAYER_KHRONOS_validation"
    };

    #ifdef VMCE_DEBUG
        inline constexpr bool enable_validation_layers = true;
    #else
        inline constexpr bool enable_validation_layers = false;
    #endif
}
