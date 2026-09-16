#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "graphic/shader.hpp"
#include "graphic/texture.hpp"

class Drawable // TODO : Rename Drawable3D ? (Drawable<T> if I need Drawable2D for Hud)
{
    protected:
        std::vector<glm::vec3> m_vertices;
        std::vector<unsigned int> m_indices;
        
        Shader m_shader;
        GLuint m_VBO, m_VAO, m_EBO;
        GLuint m_textureId;

    public: 
        Drawable(const std::string& vertexPath, const std::string& fragmentPath);
        virtual ~Drawable();
        
        virtual void LoadTexture() = 0;
        
        virtual void Load() = 0;
        virtual void Draw() const;
        virtual void Draw(const glm::mat4& projection, const glm::mat4& view) const;
};