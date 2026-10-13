#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphic/drawable.hpp"
#include "ui/quad.hpp"
#include "ui/hotbar.hpp"

class Hud : public Drawable 
{
    private:        
        Hotbar m_hotbar;
        std::vector<Quad> m_uiQuads;

        float m_hotbarWidth;

        unsigned int m_countIndex;

        bool m_show;
        
    public:
        Hud(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int windowWidth, const unsigned int windowHeight);
        
        void LoadTexture() override;
        void Load() override;
        void Draw() const override;

        void KeyCallback(const std::array<bool,GLFW_KEY_LAST+1>& keys);

        void UpdateHealth(const float health);
        void UpdateStamina(const float stamina);
};
