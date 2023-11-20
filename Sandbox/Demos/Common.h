#pragma once

#include "SimpleGL.h"
#include "glad/glad.h"
#include "glm/glm.hpp"

namespace sgl = SGL;

#define REGISTER_DEMO(name)                                         \
    struct name : sgl::Application {                                \
        void* data = nullptr;                                       \
        name() noexcept;                                            \
        ~name() noexcept;                                           \
        virtual void init() noexcept override;                      \
        virtual void update(double deltaTime) noexcept override;    \
        virtual void fixedUpdate() noexcept override;               \
    }                                                               \

namespace SGL::Demo {

    inline std::string getDemoShaderPath(std::string const& name) noexcept {
        return std::string(SGL_SANDBOX_DIR) + "Shaders/" + name;
    }

    inline std::string getDemoAssetPath(std::string const& name) noexcept {
        return std::string(SGL_SANDBOX_DIR) + "Assets/" + name;
    }

    REGISTER_DEMO(HelloTriangle);
    REGISTER_DEMO(GeometryShader);
    REGISTER_DEMO(NormalVector);

}
