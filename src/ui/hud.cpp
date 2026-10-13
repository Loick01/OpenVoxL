#include "ui/hud.hpp"

Hud::Hud(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int windowWidth, const unsigned int windowHeight):
    Drawable2D(vertexPath, fragmentPath)
{
    // Must respect the sizes from the texture
    m_hotbarWidth = 724.0f;
    float hotbarHeight = 84.0f;
    float selectorSize = 92.0f;
    float cursorSize = 36.0f;
    
    m_uiQuads.emplace_back(glm::vec2(windowWidth/2.-m_hotbarWidth/2. ,10.), m_hotbarWidth, hotbarHeight);
    m_uiQuads.emplace_back(glm::vec2(windowWidth/2.-m_hotbarWidth/2.-4., 6.), selectorSize, selectorSize); // By default, selector is on the first element in the hotbar
    m_uiQuads.emplace_back(glm::vec2(windowWidth/2.-cursorSize/2.0, windowHeight/2.-cursorSize/2.), cursorSize, cursorSize);
    m_uiQuads.emplace_back(glm::vec2(windowWidth/2.-m_hotbarWidth/2., 20.+hotbarHeight), m_hotbarWidth/2-10., 16.);
    m_uiQuads.emplace_back(glm::vec2(windowWidth/2.0+10., 20.+hotbarHeight), m_hotbarWidth/2-10., 16.);
    m_uiQuads.emplace_back(glm::vec2(windowWidth/2.-m_hotbarWidth/2.+4., 20.+hotbarHeight), m_hotbarWidth/2-16., 14.);
    m_uiQuads.emplace_back(glm::vec2(windowWidth/2.+12., 20.+hotbarHeight), m_hotbarWidth/2-16., 14.);
    
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