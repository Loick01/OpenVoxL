#include "ui/hud.hpp"

#include "ui/config.hpp"

Hud::Hud(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int windowWidth, const unsigned int windowHeight):
    Drawable2D(vertexPath, fragmentPath), m_selectorPosition(0)
{   
    const unsigned int windowCenterX = windowWidth/2;
    const unsigned int windowCenterY = windowHeight/2;
    const unsigned int hotbarPositionX = windowCenterX - HotbarConfig::width/2;
    m_uiQuads.emplace_back(glm::vec2(hotbarPositionX, HotbarConfig::yPosition),
        HotbarConfig::width, HotbarConfig::height); // Hotbar
    m_uiQuads.emplace_back(glm::vec2(hotbarPositionX - HudConfig::selectorOffset, HotbarConfig::yPosition - HudConfig::selectorOffset),
        HudConfig::selectorSize, HudConfig::selectorSize); // Selector // TODO : Selector should not be in Hud ?
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

unsigned int Hud::GetSelectorPosition() const
{
    return m_selectorPosition;
}

void Hud::SelectorNext()
{
    m_selectorPosition = (m_selectorPosition+1)%HotbarConfig::size;
    const glm::vec2 p = m_uiQuads[0].GetOrigin() + glm::vec2(m_selectorPosition * 80.f, 0.f) - glm::vec2(HudConfig::selectorOffset, HudConfig::selectorOffset); // TODO
    m_uiQuads[1].SetPosition(p);
    Load(m_uiQuads);
}

void Hud::SelectorPrevious()
{
    m_selectorPosition = (m_selectorPosition+HotbarConfig::size-1)%HotbarConfig::size;
    const glm::vec2 p = m_uiQuads[0].GetOrigin() + glm::vec2(m_selectorPosition * 80.f, 0.f) - glm::vec2(HudConfig::selectorOffset, HudConfig::selectorOffset); // TODO
    m_uiQuads[1].SetPosition(p);
    Load(m_uiQuads);
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

void Hud::ScrollCallback(const double xoffset, const double yoffset)
{
    if (yoffset > 0)
        SelectorNext();
    else if (yoffset < 0)
        SelectorPrevious();
}