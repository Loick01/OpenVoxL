#pragma once

#include <array>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "entity/hitbox.hpp"

class Player
{
    private:    
        Hitbox m_hitbox;

        float m_health;
        float m_stamina;
        float m_speed;
        float m_sprint;
        
    public:
        Player(const glm::vec3 position, const float speed, const float sprint);

        void ProcessKeyEvent(const std::array<bool, GLFW_KEY_LAST+1>& keys, const float deltaTime);
};