#pragma once

#include <array>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "graphic/shader.hpp"

class Skybox {
    private:
        Shader m_shader;
        
        std::vector<float> m_vertices;
        std::vector<unsigned int> m_indices;
        std::array<std::string,6> m_pathTextures;
        
        GLuint m_VBO;
        GLuint m_EBO;
        GLuint m_VAO;
        GLuint m_cubemapTextureId;

    public:
        Skybox(const std::string& vertexPath, const std::string& fragmentPath);
        ~Skybox();

        void LoadCubemap();
        void BindCubemap(int unit);
        void CreateBuffers();
        void Draw(const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view);
};