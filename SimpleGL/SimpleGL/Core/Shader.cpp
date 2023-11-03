#include "PCH.h"

#include "SimpleGL/Core/Shader.h"
#include "glad/glad.h"

namespace SGL {

    ShaderModule::ShaderModule(char const* const code, ShaderModuleType type) {
        handle = [&]() -> unsigned int {
            switch (type)
            {
            case ShaderModuleType::Geometry:
                return glCreateShader(GL_GEOMETRY_SHADER);
            case ShaderModuleType::Vertex:
                return glCreateShader(GL_VERTEX_SHADER);
            case ShaderModuleType::Fragment:
                return glCreateShader(GL_FRAGMENT_SHADER);
            case ShaderModuleType::Compute:
                return glCreateShader(GL_COMPUTE_SHADER);
            case ShaderModuleType::TessellationControl:
                return glCreateShader(GL_TESS_CONTROL_SHADER);
            case ShaderModuleType::TessellationEvaluation:
                return glCreateShader(GL_TESS_EVALUATION_SHADER);
            default:
                return 0;
            }
        }();
        glShaderSource(handle, 1, &code, nullptr);
        glCompileShader(handle);
        int success;
        char infoLog[512];
        glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(handle, 512, nullptr, infoLog);
            SGL_LOG_ERROR("Shader compile failed: {0}", infoLog);
        }
    }

    ShaderModule::~ShaderModule() {
        if (handle == 0) return;
        glDeleteShader(handle);
    }

    Shader::Shader(std::initializer_list<ShaderModule*> shaders) {
        handle = glCreateProgram();
        for (auto iter = shaders.begin(); iter != shaders.end(); iter++) {
            glAttachShader(handle, (*iter)->handle);
        }
        glLinkProgram(handle);
        int success;
        char infoLog[512];
        glGetProgramiv(handle, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(handle, 512, NULL, infoLog);
            SGL_LOG_ERROR("Shader link failed: {0}", infoLog);
        }
    }

    Shader::Shader(Shader&& other) noexcept {
        handle = other.handle;
        other.handle = 0;
    }

    Shader::~Shader() {
        if (handle == 0) return;
        glDeleteProgram(handle);
    }

    void Shader::use() const noexcept {
        glUseProgram(handle);
    }

    void Shader::bind(StorageBuffer const& buffer, uint32_t index, std::string const& name) const noexcept {
        uint32_t blockID = glGetProgramResourceIndex(buffer.handle, GL_SHADER_STORAGE_BLOCK, name.c_str());
        glShaderStorageBlockBinding(handle, blockID, index);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, buffer.handle);
    }

    void Shader::bind(UniformBuffer const& buffer, uint32_t index, std::string const& name) const noexcept {
        uint32_t blockID = glGetUniformBlockIndex(handle, name.c_str());
        glUniformBlockBinding(handle, blockID, index);
        glBindBufferBase(GL_UNIFORM_BUFFER, index, buffer.handle);
    }

    void Shader::setBool(const std::string& name, bool value) const noexcept {
        glUniform1i(glGetUniformLocation(handle, name.c_str()), (int)value);
    }
 
    void Shader::setInt(const std::string& name, int value) const noexcept {
        glUniform1i(glGetUniformLocation(handle, name.c_str()), value);
    }
 
    void Shader::setFloat(const std::string& name, float value) const noexcept {
        glUniform1f(glGetUniformLocation(handle, name.c_str()), value);
    }
 
    void Shader::setVec2(const std::string& name, const glm::vec2& value) const noexcept {
        glUniform2fv(glGetUniformLocation(handle, name.c_str()), 1, &value[0]);
    }
 
    void Shader::setVec3(const std::string& name, const glm::vec3& value) const noexcept {
        glUniform3fv(glGetUniformLocation(handle, name.c_str()), 1, &value[0]);
    }
 
    void Shader::setVec4(const std::string& name, const glm::vec4& value) const noexcept {
        glUniform4fv(glGetUniformLocation(handle, name.c_str()), 1, &value[0]);
    }
 
    void Shader::setMat3(const std::string& name, const glm::mat3& mat) const noexcept {
        glUniformMatrix3fv(glGetUniformLocation(handle, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
 
    void Shader::setMat4(const std::string& name, const glm::mat4& mat) const noexcept {
        glUniformMatrix4fv(glGetUniformLocation(handle, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::setBool(unsigned int location, bool value) const noexcept {
        glUniform1i(location, (int)value);
    }

    void Shader::setInt(unsigned int location, int value) const noexcept {
        glUniform1i(location, value);
    }

    void Shader::setFloat(unsigned int location, float value) const noexcept {
        glUniform1f(location, value);
    }

    void Shader::setVec2(unsigned int location, const glm::vec2& value) const noexcept {
        glUniform2fv(location, 1, &value[0]);
    }

    void Shader::setVec3(unsigned int location, const glm::vec3& value) const noexcept {
        glUniform3fv(location, 1, &value[0]);
    }

    void Shader::setVec4(unsigned int location, const glm::vec4& value) const noexcept {
        glUniform4fv(location, 1, &value[0]);
    }

    void Shader::setMat3(unsigned int location, const glm::mat3& mat) const noexcept {
        glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::setMat4(unsigned int location, const glm::mat4& mat) const noexcept {
        glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
    }

}
