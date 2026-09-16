#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphic/drawable.hpp"

namespace UiElementIndex {
    // Must respect the order of creation of Quad in Hud constructor  
    const unsigned int Hotbar = 0;
    const unsigned int Selector = 1;
    const unsigned int Cursor = 2;
    const unsigned int HealthBar = 3;
    const unsigned int StaminaBar = 4;
    const unsigned int HealthLine = 5;
    const unsigned int StaminaLine = 6;
}

struct Quad{
    std::vector<glm::vec3> vertices; // z = 0
    std::vector<unsigned int> indices;
};

class Hud : public Drawable {
    private:        
        std::vector<Quad> m_uiQuads;
        const unsigned int m_windowWidth;
        const unsigned int m_windowHeight;
        float m_hotbarWidth;

        unsigned int m_countIndex;

        bool m_showHud;
        
        Quad CreateQuad(const glm::vec2 origin, const float width, const float height); // Parameter origin is the bottom left point
        
    public:
        Hud(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int windowWidth, const unsigned int windowHeight);
        
        void LoadTexture() override;
        void Load() override;
        void Draw() const override;

        void KeyCallback(const std::array<bool,GLFW_KEY_LAST+1>& keys);

        void UpdateHealth(const float health);
        void UpdateStamina(const float stamina);
};
