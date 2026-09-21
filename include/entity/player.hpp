#pragma once

#include <array>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "core/updatable.hpp"
#include "entity/hitbox.hpp"

class Player : public EventUpdatable
{
    private:    
        Hitbox m_hitbox;

        float m_health;
        float m_stamina;
        float m_speed;
        float m_frameSpeed; // TODO : Should be in Hitbox ?
        float m_sprint;
        
    public:
        Player(const glm::vec3 position, const float speed, const float sprint);

        Hitbox& GetHitbox();
        float GetFrameSpeed() const;

        void EventUpdate(const std::array<bool, GLFW_KEY_LAST+1>& keys, const float deltaTime) override;
};