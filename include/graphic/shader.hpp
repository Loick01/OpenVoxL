#pragma once 

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

class Shader
{
    public:
        unsigned int m_programId;

        Shader(const std::string& vertexPath, const std::string& fragmentPath)
        {
            std::string vertexCode, fragmentCode;
            std::ifstream vShaderFile, fShaderFile;
            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            try{
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                vShaderFile.close();
                fShaderFile.close();
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch(std::ifstream::failure e){
                throw std::runtime_error("Failed to read shader files : " + vertexPath + ", " + fragmentPath);
            }
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            unsigned int vertex, fragment;
            int success;
            char infoLog[512];

            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, nullptr);
            glCompileShader(vertex);
            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
            if (!success){
                glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
                throw std::runtime_error(std::string("Error in vertex shader : ") + infoLog);
            }

            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, nullptr);
            glCompileShader(fragment);
            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if (!success){
                glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
                throw std::runtime_error(std::string("Error in fragment shader : ") + infoLog);
            }

            m_programId = glCreateProgram();
            glAttachShader(m_programId, vertex);
            glAttachShader(m_programId, fragment);
            glLinkProgram(m_programId);
            glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
            if (!success){
                glGetProgramInfoLog(m_programId, 512, nullptr, infoLog);
                throw std::runtime_error(std::string("Error while linking shaders : ") + infoLog);
            }

            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void Use()
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
