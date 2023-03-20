#pragma once

#include "glm/glm.hpp"

#define SHADER_PATH "./res/shaders/"

//#include "ShaderConfig.h"
#include "glad/glad.h"
#include <unordered_map>
#include <string>

namespace sbl
{
    class ShaderConfig;

    class ShaderProgram
    {
    public:
        ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, bool inWorkspace, bool log = false);
        ShaderProgram();

        void Init(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, bool inWorkspace, bool log = false);

        void SetUniform3f(const std::string& name, float v0, float v1, float v2);
        void SetUniform3f(const std::string& name, glm::vec3 v);
        void SetUniform4f(const std::string& name, glm::vec4 v);
        void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
        void SetUniformMat4f(const std::string& name, const glm::mat4& value);
        void SetUniformMat3f(const std::string& name, const glm::mat3& value);
        void SetUniform1i(const std::string& name, int v0);

        void Bind();
        void Unbind();
        void DeleteProgram();

        void SetConfig(const ShaderConfig& conf);
    private:
        void LogUniform(const std::string& name);
        int GetUniformLocation(const std::string& name);
        void ReadShader(const std::string* sp, std::string* s);
    private:
        GLuint m_VertexShaderID = 0;
        GLuint m_FragmentShaderID = 0;
        GLuint m_ProgramID = 0;
        std::unordered_map<std::string, int> m_UniformLocationCache;
        bool m_LogUniforms;
    };
}