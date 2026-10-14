#include "ui/hud.hpp"

#include "ui/config.hpp"

Hud::Hud(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int windowWidth, const unsigned int windowHeight):
    Drawable2D(vertexPath, fragmentPath)
{   
    const unsigned int windowCenterX = windowWidth/2;
    const unsigned int windowCenterY = windowHeight/2;
    const unsigned int hotbarPositionX = windowCenterX - HotbarConfig::width/2;
    m_uiQuads.emplace_back(glm::vec2(hotbarPositionX, HotbarConfig::yPosition),
        HotbarConfig::width, HotbarConfig::height); // Hotbar
    m_uiQuads.emplace_back(glm::vec2(hotbarPositionX - HudConfig::selectorOffset, HotbarConfig::yPosition - HudConfig::selectorOffset),
        HudConfig::selectorSize, HudConfig::selectorSize); // Selector
    m_uiQuads.emplace_back(glm::vec2(windowCenterX - HudConfig::cursorSize/2, windowCenterY - HudConfig::cursorSize/2),
        HudConfig::cursorSize, HudConfig::cursorSize); // Cursor
    m_uiQuads.emplace_back(glm::vec2(hotbarPositionX, HudConfig::barsPositionY + HotbarConfig::height),
        HotbarConfig::width/2 - HudConfig::barsOffsetX, HudConfig::barsHeight); // Health bar
    m_uiQuads.emplace_back(glm::vec2(windowCenterX + HudConfig::barsOffsetX, HudConfig::barsPositionY + HotbarConfig::height),
        HotbarConfig::width/2 - HudConfig::barsOffsetX, HudConfig::barsHeight); // Stamina bar
    m_uiQuads.emplace_back(glm::vec2(hotbarPositionX + HudConfig::lineOffsetX, HudConfig::barsHeight + HotbarConfig::height), 
        HotbarConfig::width/2 - HudConfig::barsOffsetX - HudConfig::lineOffsetX*2, HudConfig::lineHeight); // Health line
    m_uiQuads.emplace_back(glm::vec2(windowCenterX + HudConfig::barsOffsetX + HudConfig::lineOffsetX, HudConfig::barsHeight + HotbarConfig::height), 
        HotbarConfig::width/2 - HudConfig::barsOffsetX - HudConfig::lineOffsetX*2, HudConfig::lineHeight); // Stamina line
    
    Load(m_uiQuads);
    LoadTexture("../asset/texture/ui/hud.png");

    m_shader.Use();
    m_shader.SetInt("windowWidth", windowWidth);
    m_shader.SetInt("windowHeight", windowHeight);
}

void Hud::KeyCallback(const std::array<bool,GLFW_KEY_LAST+1>& keys)
{
    if (keys[GLFW_KEY_Q])
        m_show = !m_show;
}

void Hud::UpdateHealth(const float health)
{}

void Hud::UpdateStamina(const float stamina)
{}