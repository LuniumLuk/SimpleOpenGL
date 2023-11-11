#pragma once

#include "SimpleGL.h"
#include "glad/glad.h"
#include "glm/glm.hpp"

namespace sgl = SGL;

inline std::string getShaderPath(std::string const& name) noexcept {
    return std::string(SGL_SANDBOX_DIR) + "Shaders/" + name;
}

