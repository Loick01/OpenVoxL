#include "graphic/drawable.hpp"

Drawable::Drawable(const std::string& vertexPath, const std::string& fragmentPath):
    m_shader(vertexPath, fragmentPath)
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
}

Drawable::~Drawable()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void Drawable::Draw() const
{}

void Drawable::Draw(const glm::mat4& projection, const glm::mat4& view) const
{}