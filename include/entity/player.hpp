#pragma once

#include <array>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "event/key_updatable.hpp"
#include "entity/hitbox.hpp"

class Player : public EventKeyUpdatable
{
    private:    
        Hitbox m_hitbox;

        float m_health;
        float m_stamina;
        float m_speed;
        float m_frameSpeed; // TODO : Should be in Hitbox ?
        float m_sprint;
        float m_voxelRange;
        
    public:
        Player(const glm::vec3 position, const float speed, const float sprint, const float range);

        Hitbox& GetHitbox();
        float GetFrameSpeed() const;

        float GetVoxelRange() const;

        void EventKeyUpdate(const std::array<bool, GLFW_KEY_LAST+1>& keys, const float deltaTime) override;
};