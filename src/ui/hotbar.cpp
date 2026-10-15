#include "ui/hotbar.hpp"

#include "ui/config.hpp"

Hotbar::Hotbar(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int windowWidth, const unsigned int windowHeight):
    Drawable2D(vertexPath, fragmentPath), m_selectorPosition(0)
{
    const unsigned int windowCenterX = windowWidth/2;
    const unsigned int hotbarPositionX = windowCenterX - HotbarConfig::width/2;

    for (unsigned int i = 0 ; i < HotbarConfig::size ; i++) {
        m_hotbarQuads.emplace_back(glm::vec2(hotbarPositionX + 80 * i + HotbarConfig::itemOffset, HotbarConfig::yPosition + HotbarConfig::itemOffset),
            HotbarConfig::itemSize, HotbarConfig::itemSize);
        
        m_blockIds[i] = i; // TODO : Remove ?
    }

    Load(m_hotbarQuads);
    LoadTexture("../asset/texture/block/atlas.png");

    m_shader.Use();
    m_shader.SetInt("windowWidth", windowWidth);
    m_shader.SetInt("windowHeight", windowHeight);

    glGenBuffers(1, &m_blockId_SSBO);

    UpdateBlockIds();
}

Hotbar::~Hotbar()
{
    glDeleteBuffers(1, &m_blockId_SSBO);
}

void Hotbar::UpdateBlockIds()
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_blockId_SSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, m_blockIds.size()*sizeof(unsigned int), m_blockIds.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_blockId_SSBO);
}

unsigned int Hotbar::GetSelectedBlockId() const
{
    return m_blockIds[m_selectorPosition];
}

void Hotbar::SelectorNext()
{
    m_selectorPosition = (m_selectorPosition+1)%HotbarConfig::size;
}

void Hotbar::SelectorPrevious()
{
    m_selectorPosition = (m_selectorPosition+HotbarConfig::size-1)%HotbarConfig::size;
}

void Hotbar::ScrollCallback(const double xoffset, const double yoffset)
{

    if (yoffset > 0)
        SelectorNext();
    else if (yoffset < 0)
        SelectorPrevious();

    // m_hudShader.SetInt("selectorLocation", m_handBlockIndex); // TODO : Update selector Quad in Hud 
}

void Hotbar::Draw() const
{
    if (!m_show)
        return;
    
    glDisable(GL_DEPTH_TEST);
    m_shader.Use();
    BindTexture2D(m_shader.GetLocation("text"), m_textureId, 0); // TODO : "text" should be a parameter ?
    glBindVertexArray(m_VAO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_blockId_SSBO);
    glDrawElements(GL_TRIANGLES, m_countIndex, GL_UNSIGNED_INT, (void*)0);
    glEnable(GL_DEPTH_TEST);
}