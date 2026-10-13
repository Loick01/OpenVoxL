#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "graphic/shader.hpp"
#include "graphic/texture.hpp"
#include "ui/quad.hpp"

class Drawable3D
{
    protected:
        std::vector<glm::vec3> m_vertices;
        std::vector<unsigned int> m_indices;
        
        Shader m_shader;
        GLuint m_VBO, m_VAO, m_EBO;
        GLuint m_textureId;

    public: 
        Drawable3D(const std::string& vertexPath, const std::string& fragmentPath);
        virtual ~Drawable3D();
        
        virtual void LoadTexture(const std::string& texturePath);
        virtual void LoadCmTexture(const std::array<std::string,6>& pathTextures);
        virtual void Load() = 0;
        virtual void Draw() const;
        virtual void Draw(const glm::mat4& projection, const glm::mat4& view) const;
};

class Drawable2D
{
    protected:
        std::vector<glm::vec2> m_vertices;
        std::vector<unsigned int> m_indices;
        
        Shader m_shader;
        GLuint m_VBO, m_VAO, m_EBO;
        GLuint m_textureId;

        unsigned int m_countIndex;
        bool m_show;
        
    public:
        Drawable2D(const std::string& vertexPath, const std::string& fragmentPath);
        virtual ~Drawable2D();
        
        void LoadTexture(const std::string& texturePath);
        void Load(const std::vector<Quad>& quads);
        virtual void Draw() const;
};