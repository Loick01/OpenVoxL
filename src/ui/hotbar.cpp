#include "ui/hotbar.hpp"

#include "ui/config.hpp"

Hotbar::Hotbar(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int windowWidth, const unsigned int windowHeight):
    Drawable2D(vertexPath, fragmentPath)
{
    const unsigned int windowCenterX = windowWidth/2;
    const unsigned int hotbarPositionX = windowCenterX - HotbarConfig::width/2;

    for (unsigned int i = 0 ; i < 9 ; i++)
        m_hotbarQuads.emplace_back(glm::vec2(hotbarPositionX + 80 * i + HotbarConfig::itemOffset, HotbarConfig::yPosition + HotbarConfig::itemOffset),
            HotbarConfig::itemSize, HotbarConfig::itemSize);
    
    Load(m_hotbarQuads);

    m_shader.Use();
    m_shader.SetInt("windowWidth", windowWidth);
    m_shader.SetInt("windowHeight", windowHeight);
    
    // TODO : BlockId in hotbar
}