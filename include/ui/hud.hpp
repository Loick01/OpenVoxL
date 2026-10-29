#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "event/key_callback.hpp"
#include "event/scroll_callback.hpp"
#include "graphic/drawable.hpp"

class Hud : public EventScrollCallback, public EventKeyCallback, public Drawable2D 
{
    private:
        std::vector<Quad> m_uiQuads;

        unsigned int m_selectorPosition;
        
    public:
        Hud(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int windowWidth, const unsigned int windowHeight);

        unsigned int GetSelectorPosition() const;
        void SelectorNext();
        void SelectorPrevious();

        void UpdateHealth(const float health);
        void UpdateStamina(const float stamina);

        void KeyCallback(const std::array<bool, GLFW_KEY_LAST+1>& keys) override;
        void ScrollCallback(const double xoffset, const double yoffset) override;
};
