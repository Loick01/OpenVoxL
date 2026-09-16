#include "entity/player.hpp"

Player::Player(const glm::vec3 position, const float speed, const float sprint):
    m_hitbox(position), m_health(100.f), m_stamina(100.f), m_speed(speed), m_sprint(sprint)
{}

Hitbox& Player::GetHitbox()
{
    return m_hitbox;
}

void Player::EventUpdate(const std::array<bool, GLFW_KEY_LAST+1>& keys, const float deltaTime)
{
    const float frameSpeed = m_speed * deltaTime;

    // if (keys[GLFW_KEY_W])
    //     m_position += frameSpeed * m_frontVector;
    // if (keys[GLFW_KEY_S])
    //     m_position -= frameSpeed * m_frontVector;
    // if (keys[GLFW_KEY_D])
    //     m_position += frameSpeed * m_rightVector;
    // if (keys[GLFW_KEY_A])
    //     m_position -= frameSpeed * m_rightVector;
    // if (keys[GLFW_KEY_SPACE])
    //     m_position += frameSpeed * m_upVector;
    // if (keys[GLFW_KEY_LEFT_CONTROL])
    //     m_position -= frameSpeed * m_upVector;
}