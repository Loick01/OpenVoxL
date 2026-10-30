#include "graphic/drawable.hpp"

Drawable3D::Drawable3D(const std::string& vertexPath, const std::string& fragmentPath):
    m_shader(vertexPath, fragmentPath)
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
}

Drawable3D::~Drawable3D()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void Drawable3D::LoadTexture(const std::string& texturePath)
{
    m_textureId = LoadTexture2D(texturePath);
}

void Drawable3D::LoadCmTexture(const std::array<std::string,6>& pathTextures)
{
    m_textureId = LoadCubemapTexture(pathTextures);
}

void Drawable3D::Draw(const glm::mat4& projection, const glm::mat4& view) const
{}

Drawable2D::Drawable2D(const std::string& vertexPath, const std::string& fragmentPath):
    m_shader(vertexPath, fragmentPath), m_show(true)
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
}

Drawable2D::~Drawable2D()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}


void Drawable2D::LoadTexture(const std::string& texturePath)
{
    m_textureId = LoadTexture2D(texturePath);
}

void Drawable2D::Load(const std::vector<Quad>& quads)
{
    m_vertices.clear();
    m_indices.clear();
    
    unsigned int indexOffset = 0;
    m_countIndex = 0;
    
    for (unsigned int i = 0 ; i < quads.size() ; i++){
        for (unsigned int k = 0 ; k < 4 ; k++){
            m_vertices.push_back(quads[i].vertices[k]);
        }
        for (unsigned int k = 0 ; k < 6 ; k++){
            m_indices.push_back(indexOffset + quads[i].indices[k]);
            m_countIndex++;
        }
        indexOffset += 4;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(glm::vec2), m_vertices.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(m_VAO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()* sizeof(unsigned int), m_indices.data() , GL_STATIC_DRAW);
}

void Drawable2D::Draw() const
{
    if (!m_show)
        return;
    
    glDisable(GL_DEPTH_TEST);
    m_shader.Use();
    BindTexture2D(m_shader.GetLocation("text"), m_textureId, 0); // TODO : "text" should be a parameter ?
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_countIndex, GL_UNSIGNED_INT, (void*)0);
    glEnable(GL_DEPTH_TEST);
}