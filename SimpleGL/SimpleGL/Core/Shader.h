#pragma once

#include "SimpleGL/Core/Types.h"
#include "SimpleGL/Core/Buffer.h"
#include "glm/glm.hpp"

namespace SGL {

#define SGL_INVALID_INDEX ~0U
#define SGL_INVALID_LOCATION ~0U
#define SGL_INVALID_BINDING ~0U

    struct ShaderModule {

        unsigned int handle;

        ShaderModule(char const* const code, ShaderModuleType type);
        ShaderModule(ShaderModule const&) = delete;
        ShaderModule(ShaderModule&&) = delete;
        ~ShaderModule();

    };

    struct ShaderVariable {
        std::string name;
        DataType type;
        uint32_t count;
        uint32_t location;

        ShaderVariable(std::string const& name, DataType type, uint32_t count, uint32_t location)
            : name(name), type(type), count(count), location(location) {}
    };

    struct ShaderUniformBlock {
        std::string name;
        uint32_t size;
        uint32_t binding;
        std::vector<ShaderVariable> variables;

        ShaderUniformBlock(std::string const& name, uint32_t size, uint32_t binding, std::vector<ShaderVariable> const& variables)
            : name(name), size(size), binding(binding), variables(variables) {}
    };

    struct Shader {

        unsigned int handle;
        std::vector<ShaderVariable> attributes;
        std::vector<ShaderVariable> uniforms;
        std::vector<ShaderUniformBlock> uniformBlocks;
        std::unordered_map<std::string, uint32_t> attributeIndices;
        std::unordered_map<std::string, uint32_t> uniformIndices;
        std::unordered_map<std::string, uint32_t> uniformBlockIndices;

        Shader(std::string const& path) noexcept;
        Shader(std::initializer_list<ShaderModule*> shaders) noexcept;
        Shader(Shader const&) = delete;
        Shader(Shader&& other) noexcept;
        ~Shader();

        void bind() const noexcept;
        void bind(StorageBuffer const& buffer, uint32_t index, std::string const& name) const noexcept;
        void bind(UniformBuffer const& buffer, uint32_t index, std::string const& name) const noexcept;

        bool hasUniform(std::string const& name) const noexcept;
        void setUniform(std::string const& name, void const* data) noexcept;
        void setUniformBinding(std::string const& name, uint32_t binding) noexcept;
        void setUniformBlockBinding(std::string const& name, uint32_t binding) noexcept;

        void setUniformBinding(uint32_t location, uint32_t binding) noexcept;
        void setUniformBlockBinding(uint32_t location, uint32_t binding) noexcept;

        void getUniform(std::string const& name, void* ptr, size_t size) noexcept;
        uint32_t getUniformLocation(std::string const& name) noexcept;
        uint32_t getUniformBlockLocation(std::string const& name) noexcept;
        uint32_t getUniformBinding(std::string const& name) noexcept;
        uint32_t getUniformBlockBinding(std::string const& name) noexcept;

        void linkShader(std::vector<ShaderModule*> const& shaders) noexcept;

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
