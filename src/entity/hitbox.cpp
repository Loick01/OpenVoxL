#include "entity/hitbox.hpp"

#include <stdexcept>

Hitbox::Hitbox(const glm::vec3 position):
    m_position(position), m_currentMove(0.f)
{}

glm::vec3 Hitbox::GetPosition() const
{
    return m_position;
}

glm::vec3 Hitbox::GetFrontVector() const
{
    return m_frontVector;
}

void Hitbox::SetFrontVector(const glm::vec3 front)
{
    m_frontVector = front;
}

void Hitbox::SetRightVector(const glm::vec3 right)
{
    m_rightVector = right;
}

void Hitbox::RequestMove(const Direction dir)
{
    switch (dir) {
        case Direction::Front :
            m_currentMove += m_frontVector;
            break;
        case Direction::Back :
            m_currentMove -= m_frontVector;
            break;
        case Direction::Right :
            m_currentMove += m_rightVector;
            break;
        case Direction::Left :
            m_currentMove -= m_rightVector;
            break;
        default:    
            throw std::runtime_error("Hitbox::Move : This Direction value should not be used here");
    }
}

void Hitbox::ApplyMove(const float frameSpeed)
{
    m_currentMove.y = 0.f;
    if (m_currentMove != glm::vec3(0.f)) {
        m_position += glm::normalize(m_currentMove)*frameSpeed;
        m_currentMove = glm::vec3(0.f);
    }
}