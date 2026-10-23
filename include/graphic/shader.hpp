#pragma once 

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "debug/logger.hpp"

class Shader
{
    private:
        static std::string OpenShader(const std::string& shaderPath)
        {
            std::ifstream shaderFile;
            std::stringstream shaderStream;

            try {
                shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
                shaderFile.open(shaderPath);
                shaderStream << shaderFile.rdbuf();
                shaderFile.close();
            } catch (std::ifstream::failure e) {
                Logger::GetInstance().AddLog(LogLevel::Level2, "Failed to open the shader : " + shaderPath);
            }

            return shaderStream.str();
        }

        static GLuint CreateShader(const GLenum shaderType, const char* shaderCode)
        {
            GLuint shaderId;
            GLint success;
            GLchar infoLog[512];
            
            shaderId = glCreateShader(shaderType);
            glShaderSource(shaderId, 1, &shaderCode, nullptr);
            glCompileShader(shaderId);
            glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
            if (!success){
                glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
                Logger::GetInstance().AddLog(LogLevel::Level2, std::string("Failed to create the shader : ") + infoLog);
            }

            return shaderId;
        }

        static GLuint CreateProgram(const GLuint vertexId, const GLuint fragmentId)
        {
            GLuint programId;
            GLint success;
            GLchar infoLog[512];

            programId = glCreateProgram();
            glAttachShader(programId, vertexId);
            glAttachShader(programId, fragmentId);
            glLinkProgram(programId);
            glGetProgramiv(programId, GL_LINK_STATUS, &success);
            if (!success){
                glGetProgramInfoLog(programId, 512, nullptr, infoLog);
                Logger::GetInstance().AddLog(LogLevel::Level2, std::string("Failed to link shaders to the program : ") + infoLog);
            }

            // TODO : Should not be here ?
            glDeleteShader(vertexId);
            glDeleteShader(fragmentId);
            
            return programId;
        }

    public:
        GLuint m_programId;

        Shader(const std::string& vertexPath, const std::string& fragmentPath)
        {
            const std::string vertexCode = OpenShader(vertexPath);
            const std::string fragmentCode = OpenShader(fragmentPath);
            
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            GLuint vertexId = CreateShader(GL_VERTEX_SHADER, vShaderCode);
            GLuint fragmentId = CreateShader(GL_FRAGMENT_SHADER, fShaderCode);

            m_programId = CreateProgram(vertexId, fragmentId);
        }

        GLint GetLocation(const std::string& name) const
        {
            return glGetUniformLocation(m_programId, name.c_str());
        }

        void Use() const
        {
            glUseProgram(m_programId);
        }

        void SetBool(const std::string& name, const bool value) const
        {
            glUniform1i(glGetUniformLocation(m_programId, name.c_str()), (int)value);
        }

        void SetInt(const std::string& name, const int value) const
        {
            glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value);
        }

        void SetFloat(const std::string& name, const float value) const
        {
            glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
        }

        void SetVec3(const std::string& name, const glm::vec3& value) const
        {
            glUniform3fv(glGetUniformLocation(m_programId, name.c_str()), 1, glm::value_ptr(value));
        }

        void SetVec2(const std::string& name, const glm::vec2& value) const
        {
            glUniform2fv(glGetUniformLocation(m_programId, name.c_str()), 1, glm::value_ptr(value));
        }

        void SetMat4(const std::string& name, const glm::mat4& value) const
        {
            glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
        }
};
