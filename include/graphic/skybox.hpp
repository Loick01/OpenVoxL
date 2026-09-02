#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "graphic/shader.hpp"

class Skybox {
    private:
        std::vector<float> m_vertices;
        std::vector<unsigned int> m_indices;
        
        Shader m_shader;
        GLuint m_cubemapTextureId;
        
        GLuint m_VBO;
        GLuint m_EBO;
        GLuint m_VAO;

    public:
        Skybox(const std::string& vertexPath, const std::string& fragmentPath);
        ~Skybox();

        void Load();
        void Draw(const glm::mat4& projection, const glm::mat4& view);
};