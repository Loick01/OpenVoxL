#include "entity/player.hpp"

Player::Player(const glm::vec3 position, const float speed, const float sprint):
    m_hitbox(position, 1.8f, 0.6f), m_health(100.f), m_stamina(100.f), m_speed(speed), m_sprint(sprint)
{}

Hitbox& Player::GetHitbox()
{
    return m_hitbox;
}

float Player::GetFrameSpeed() const
{
    return m_frameSpeed;
}

void Player::EventUpdate(const std::array<bool, GLFW_KEY_LAST+1>& keys, const float deltaTime)
{
    m_frameSpeed = m_speed * deltaTime;
    
    if (keys[GLFW_KEY_W])
        m_hitbox.RequestMove(Direction::Front);
    if (keys[GLFW_KEY_S])
        m_hitbox.RequestMove(Direction::Back);
    if (keys[GLFW_KEY_D])
        m_hitbox.RequestMove(Direction::Right);
    if (keys[GLFW_KEY_A])
        m_hitbox.RequestMove(Direction::Left);
}