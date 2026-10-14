#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphic/drawable.hpp"

class Hud : public Drawable2D 
{
    private:
        std::vector<Quad> m_uiQuads;
        
    public:
        Hud(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int windowWidth, const unsigned int windowHeight);

        void KeyCallback(const std::array<bool,GLFW_KEY_LAST+1>& keys);

        void UpdateHealth(const float health);
        void UpdateStamina(const float stamina);
};
