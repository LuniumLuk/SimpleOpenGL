#pragma once

#include "SimpleGL/Core/Types.h"
#include "SimpleGL/Core/Buffer.h"
#include "glm/glm.hpp"

namespace SGL {

    struct ShaderModule {

        unsigned int handle;

        ShaderModule(char const* const code, ShaderModuleType type);
        ShaderModule(ShaderModule const&) = delete;
        ShaderModule(ShaderModule&&) = delete;
        ~ShaderModule();

    };

    struct Shader {

        unsigned int handle;

        Shader(std::initializer_list<ShaderModule*> shaders);
        Shader(Shader const&) = delete;
        Shader(Shader&& other) noexcept;
        ~Shader();

        void use() const noexcept;
        void bind(StorageBuffer const& buffer, uint32_t index, std::string const& name) const noexcept;
        void bind(UniformBuffer const& buffer, uint32_t index, std::string const& name) const noexcept;

        void setBool(std::string const& name, bool value) const noexcept;
        void setInt(std::string const& name, int value) const noexcept;
        void setFloat(std::string const& name, float value) const noexcept;
        void setVec2(std::string const& name, glm::vec2 const& value) const noexcept;
        void setVec3(std::string const& name, glm::vec3 const& value) const noexcept;
        void setVec4(std::string const& name, glm::vec4 const& value) const noexcept;
        void setMat3(std::string const& name, glm::mat3 const& value) const noexcept;
        void setMat4(std::string const& name, glm::mat4 const& value) const noexcept;

        void setBool(unsigned int location, bool value) const noexcept;
        void setInt(unsigned int location, int value) const noexcept;
        void setFloat(unsigned int location, float value) const noexcept;
        void setVec2(unsigned int location, glm::vec2 const& value) const noexcept;
        void setVec3(unsigned int location, glm::vec3 const& value) const noexcept;
        void setVec4(unsigned int location, glm::vec4 const& value) const noexcept;
        void setMat3(unsigned int location, glm::mat3 const& value) const noexcept;
        void setMat4(unsigned int location, glm::mat4 const& value) const noexcept;

    };

}
