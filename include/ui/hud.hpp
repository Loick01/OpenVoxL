#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include "graphic/shader.hpp"

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

// Should not be here ?
struct Quad{
    std::vector<glm::vec2> vertices;
    std::vector<unsigned int> indices;
};

class Hud{
    private:
        std::vector<Quad> m_uiQuads;
        const unsigned int m_windowWidth;
        const unsigned int m_windowHeight;
        float m_hotbarWidth;

        Shader m_shader;
        GLuint m_textureId;
        
        GLuint m_VAO;
        GLuint m_VBO;
        GLuint m_EBO;
        unsigned int m_countIndex; 
        
        Quad CreateQuad(const glm::vec2 origin, const float width, const float height); // Parameter origin is the bottom left point
        
    public:
        Hud(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int windowWidth, const unsigned int windowHeight);
        ~Hud();

        void Load();
        void Draw();

        void UpdateHealth(const float health);
        void UpdateStamina(const float stamina);
};
