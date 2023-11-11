#include "PCH.h"

#include "SimpleGL/Core/IO.h"
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

    static size_t findPairedBrackets(std::string const& code, size_t start) {
        size_t pos = code.find('{', start);
        size_t counter = 1;
        while (counter > 0 && pos != std::string::npos) {
            pos += 1;
            if (code[pos] == '{') {
                counter += 1;
            }
            else if (code[pos] == '}') {
                counter -= 1;
            }
        }
        return pos;
    }

    static ShaderModuleType typeToShaderStage(std::string const& stage) {
        if (stage == "vert") return ShaderModuleType::Vertex;
        if (stage == "frag") return ShaderModuleType::Fragment;
        if (stage == "geom") return ShaderModuleType::Geometry;
        if (stage == "comp") return ShaderModuleType::Compute;
        if (stage == "tesc") return ShaderModuleType::TessellationControl;
        if (stage == "tese") return ShaderModuleType::TessellationEvaluation;
        SGL_LOG_ERROR("Unknown shader stage");
        return ShaderModuleType::None;
    }

    static DataType openGLTypeToDataType(GLenum type) {
        switch (type) {
        case GL_FLOAT: return DataType::Float;
        case GL_FLOAT_VEC2: return DataType::Float2;
        case GL_FLOAT_VEC3: return DataType::Float3;
        case GL_FLOAT_VEC4: return DataType::Float4;
        case GL_INT: return DataType::Int;
        case GL_INT_VEC2: return DataType::Int2;
        case GL_INT_VEC3: return DataType::Int3;
        case GL_INT_VEC4: return DataType::Int4;
        case GL_FLOAT_MAT3: return DataType::Mat3;
        case GL_FLOAT_MAT4: return DataType::Mat4;
        case GL_BOOL: return DataType::Bool;
        case GL_SAMPLER_2D: return DataType::Sampler2D;
        case GL_SAMPLER_CUBE: return DataType::SamplerCube;
        }
        SGL_LOG_ERROR("Unknown data type");
        return DataType::None;
    }

    ShaderModule::~ShaderModule() {
        if (handle == 0) return;
        glDeleteShader(handle);
    }

    Shader::Shader(std::string const& path) noexcept {
        const char* uniformsToken = "@uniforms:";
        const size_t uniformsTokenLength = strlen(uniformsToken);
        const char* stageToken = "@stage:";
        const size_t stageTokenLength = strlen(stageToken);

        std::vector<ShaderModule*> shaders;
        std::string uniforms;
        auto code = readFile(path);
        size_t pos = 0;

        // Find uniforms
        pos = code.find(uniformsToken, pos);
        if (pos != std::string::npos) {
            size_t bracketBeg = code.find('{', pos);
            size_t bracketEnd = findPairedBrackets(code, bracketBeg);
            uniforms = code.substr(bracketBeg + 1, bracketEnd - bracketBeg - 1);
        }

        // Find stages
        pos = code.find(stageToken, pos);
        while (pos != std::string::npos) {
            size_t bracketBeg = code.find('{', pos);
            size_t typeBeg = pos + stageTokenLength;
            auto type = code.substr(typeBeg, bracketBeg - typeBeg);
            type.erase(std::remove_if(type.begin(), type.end(), ::isspace), type.end());
            size_t bracketEnd = findPairedBrackets(code, bracketBeg);
            auto src = code.substr(bracketBeg + 1, bracketEnd - bracketBeg - 1);

            src = uniforms + src;

            SGL_LOG_INFO("Loading shader stage [{0}] from file {1}", type, path);
            shaders.push_back(new ShaderModule(src.c_str(), typeToShaderStage(type)));

            pos = code.find(stageToken, bracketEnd + 1);
        }

        handle = glCreateProgram();
        linkShader(shaders);
    }

    Shader::Shader(std::initializer_list<ShaderModule*> shaders) noexcept {
        handle = glCreateProgram();
        linkShader(shaders);
    }

    Shader::Shader(Shader&& other) noexcept {
        handle = other.handle;
        other.handle = 0;
    }

    Shader::~Shader() {
        if (handle == 0) return;
        glDeleteProgram(handle);
    }

    void Shader::bind() const noexcept {
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

    bool Shader::hasUniform(std::string const& name) const noexcept {
        return uniformIndices.find(name) != uniformIndices.end();
    }

    void Shader::setUniform(std::string const& name, void const* data) noexcept {
        auto it = uniformIndices.find(name);
        if (it != uniformIndices.end()) {
            uint32_t index = it->second;
            switch (uniforms[index].type) {
            case DataType::Bool: setBool(uniforms[index].location, *(bool*)data); return;
            case DataType::Int: setInt(uniforms[index].location, *(int*)data); return;
            case DataType::Float: setFloat(uniforms[index].location, *(float*)data); return;
            case DataType::Float2: setVec2(uniforms[index].location, *(glm::vec2*)data); return;
            case DataType::Float3: setVec3(uniforms[index].location, *(glm::vec3*)data); return;
            case DataType::Float4: setVec4(uniforms[index].location, *(glm::vec4*)data); return;
            case DataType::Mat3: setMat3(uniforms[index].location, *(glm::mat3*)data); return;
            case DataType::Mat4: setMat4(uniforms[index].location, *(glm::mat4*)data); return;
            }
            SGL_LOG_WARN("Uniform {0} with unknown or unsupported DataType", name);
        }
        else {
            SGL_LOG_WARN("Uniform {0} not found", name);
        }
    }

    void Shader::setUniformBinding(std::string const& name, uint32_t binding) noexcept {
        auto it = uniformIndices.find(name);
        if (it != uniformIndices.end()) {
            uint32_t index = it->second;
            switch (uniforms[index].type) {
            case DataType::Sampler2D:
            case DataType::SamplerCube:
                setInt(uniforms[index].location, binding); return;
            }
            SGL_LOG_WARN("Uniform {0} with unknown or unsupported DataType", name);
        }
        else {
            SGL_LOG_WARN("Uniform {0} not found", name);
        }
    }

    void Shader::setUniformBlockBinding(std::string const& name, uint32_t binding) noexcept {
        auto it = uniformBlockIndices.find(name);
        if (it != uniformBlockIndices.end()) {
            uint32_t index = it->second;
            glUniformBlockBinding(handle, index, binding);
        }
        else {
            SGL_LOG_WARN("Uniform block {0} not found", name);
        }
    }

    void Shader::setUniformBinding(uint32_t location, uint32_t binding) noexcept {
        glUniform1i(location, binding);
    }

    void Shader::setUniformBlockBinding(uint32_t location, uint32_t binding) noexcept {
        glUniformBlockBinding(handle, location, binding);
    }

    void Shader::getUniform(std::string const& name, void* ptr, size_t size) noexcept {
        auto it = uniformIndices.find(name);
        if (it != uniformIndices.end()) {
            uint32_t index = it->second;
            if (size == getDataTypeSize(uniforms[index].type)) {
                SGL_ASSERT(false, "Size not large enough to retrieve the uniform");
                return;
            }
            switch (uniforms[index].type) {
            case DataType::Float:
            case DataType::Float2:
            case DataType::Float3:
            case DataType::Float4:
            case DataType::Mat3:
            case DataType::Mat4:
                glGetUniformfv(handle, uniforms[index].location, (GLfloat*)ptr); return;
            case DataType::Int:
            case DataType::Int2:
            case DataType::Int3:
            case DataType::Int4:
            case DataType::Bool:
                glGetUniformiv(handle, uniforms[index].location, (GLint*)ptr); return;
            case DataType::Sampler2D:
            case DataType::SamplerCube:
                glGetUniformiv(handle, uniforms[index].location, (GLint*)ptr); return;
            }
        }
        else {
            SGL_LOG_WARN("Uniform {0} not found", name);
        }
    }

    uint32_t Shader::getUniformLocation(std::string const& name) noexcept {
        auto it = uniformIndices.find(name);
        if (it != uniformIndices.end()) {
            uint32_t index = it->second;
            return uniforms[index].location;
        }
        else {
            SGL_LOG_WARN("Uniform {0} not found", name);
            return SGL_INVALID_LOCATION;
        }
    }

    uint32_t Shader::getUniformBlockLocation(std::string const& name) noexcept {
        auto it = uniformBlockIndices.find(name);
        if (it != uniformBlockIndices.end()) {
            return it->second;
        }
        else {
            SGL_LOG_WARN("Uniform block {0} not found", name);
            return SGL_INVALID_LOCATION;
        }
    }

    uint32_t Shader::getUniformBinding(std::string const& name) noexcept {
        auto it = uniformIndices.find(name);
        uint32_t binding = SGL_INVALID_BINDING;
        if (it != uniformIndices.end()) {
            uint32_t index = it->second;
            switch (uniforms[index].type) {
            case DataType::Sampler2D:
            case DataType::SamplerCube:
                glGetUniformiv(handle, uniforms[index].location, (GLint*)&binding);
            }
        }
        else {
            SGL_LOG_WARN("Uniform {0} not found", name);
        }
        return binding;
    }

    uint32_t Shader::getUniformBlockBinding(std::string const& name) noexcept {
        auto it = uniformBlockIndices.find(name);
        if (it != uniformBlockIndices.end()) {
            uint32_t index = it->second;
            return uniformBlocks[index].binding;
        }
        else {
            SGL_LOG_WARN("Uniform block {0} not found", name);
            return SGL_INVALID_BINDING;
        }
    }

    void Shader::linkShader(std::vector<ShaderModule*> const& shaders) noexcept {
        for (auto shader : shaders) {
            glAttachShader(handle, shader->handle);
        }
        glLinkProgram(handle);
        GLint success;
        GLchar infoLog[512];
        glGetProgramiv(handle, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(handle, 512, NULL, infoLog);
            SGL_ASSERT(false, "Shader link failed: {0}", infoLog);
        }

        GLint count;
        GLint size;
        GLenum type;
        GLchar name[512];
        GLsizei length;

        glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &count);
        for (GLint i = 0; i < count; ++i) {
            glGetActiveAttrib(handle, i, 512, &length, &size, &type, name);
            uint32_t location = glGetAttribLocation(handle, name);

            auto nameStr = std::string(name);
            attributeIndices.insert(std::make_pair(nameStr, (uint32_t)attributes.size()));
            attributes.emplace_back(nameStr, openGLTypeToDataType(type), size, location);
        }

        glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &count);
        for (GLint i = 0; i < count; ++i) {
            glGetActiveUniform(handle, i, 512, &length, &size, &type, name);
            uint32_t location = glGetUniformLocation(handle, name);

            auto nameStr = std::string(name);
            uniformIndices.insert(std::make_pair(nameStr, (uint32_t)uniforms.size()));
            uniforms.emplace_back(nameStr, openGLTypeToDataType(type), size, location);
        }

        glGetProgramiv(handle, GL_ACTIVE_UNIFORM_BLOCKS, &count);
        for (GLint i = 0; i < count; ++i) {
            GLint binding;
            GLint uniformCount;
            glGetActiveUniformBlockiv(handle, i, GL_UNIFORM_BLOCK_BINDING, &binding);
            glGetActiveUniformBlockiv(handle, i, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
            glGetActiveUniformBlockiv(handle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniformCount);

            std::vector<GLint> uniformIndices(uniformCount);
            glGetActiveUniformBlockiv(handle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, uniformIndices.data());
            std::vector<ShaderVariable> variables;
            for (auto index : uniformIndices) {
                variables.push_back(uniforms[index]);
            }

            glGetActiveUniformBlockName(handle, i, 512, &length, name);

            auto nameStr = std::string(name);
            uniformBlockIndices.insert(std::make_pair(nameStr, (uint32_t)uniformBlocks.size()));
            uniformBlocks.emplace_back(nameStr, size, binding, variables);
        }
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
