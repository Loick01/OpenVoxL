#include "ui/hotbar.hpp"

Hotbar::Hotbar(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int windowWidth, const unsigned int hotbarWidth):
    Drawable2D(vertexPath, fragmentPath)
{
    for (unsigned int i = 0 ; i < 9 ; i++)
        m_hotbarQuads.emplace_back(glm::vec2(windowWidth/2.-hotbarWidth/2.+80.*i+8., 18.), 68., 68.);
    
    Load(m_hotbarQuads);
    
    // TODO : BlockId in hotbar
}