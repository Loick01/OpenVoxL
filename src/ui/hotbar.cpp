#include "ui/hotbar.hpp"

Hotbar::Hotbar(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int windowWidth, const unsigned int hotbarWidth):
    Drawable(vertexPath, fragmentPath), m_show(true)
{
    for (unsigned int i = 0 ; i < 9 ; i++)
        m_hotbarQuads.emplace_back(glm::vec2(windowWidth/2.-hotbarWidth/2.+80.*i+8., 18.), 68., 68.);
    
    // TODO : BlockId in hotbar
}

void Hotbar::LoadTexture()
{
    m_textureId = LoadTexture2D("../asset/texture/ui/hud.png");
}

void Hotbar::Load()
{   
    unsigned int indexOffset = 0;
    m_countIndex = 0;
    
    for (unsigned int i = 0 ; i < m_hotbarQuads.size() ; i++){
        for (unsigned int k = 0 ; k < 4 ; k++){
            m_vertices.push_back(m_hotbarQuads[i].vertices[k]);
        }
        for (unsigned int k = 0 ; k < 6 ; k++){
            m_indices.push_back(indexOffset + m_hotbarQuads[i].indices[k]);
            m_countIndex++;
        }
        indexOffset += 4;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(glm::vec3), m_vertices.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(m_VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()* sizeof(unsigned int), m_indices.data() , GL_STATIC_DRAW);
}

void Hotbar::Draw() const
{
    
}