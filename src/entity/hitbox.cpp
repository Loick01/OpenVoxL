#include "entity/hitbox.hpp"

#include <stdexcept>

Hitbox::Hitbox(const glm::vec3 position, const float height, const float width):
    m_position(position), m_height(height), m_width(width), m_currentMove(0.f)
{
    ComputeAABB();
}

AABB Hitbox::GetBox() const
{
    return m_box;
}

glm::vec3 Hitbox::GetPosition() const
{
    return m_position;
}

glm::vec3 Hitbox::GetFrontVector() const
{
    return m_frontVector;
}

glm::vec3 Hitbox::GetCurrentMove() const
{
    return m_currentMove;
}

void Hitbox::ComputeAABB()
{
    m_box = {
        glm::vec3(m_position.x-m_width/2.f, m_position.y, m_position.z-m_width/2.f),
        glm::vec3(m_position.x+m_width/2.f, m_position.y+m_height, m_position.z+m_width/2.f)
    };
}

void Hitbox::SetFrontVector(const glm::vec3 front)
{
    m_frontVector = front;
}

void Hitbox::SetRightVector(const glm::vec3 right)
{
    m_rightVector = right;
}

void Hitbox::SetPosition(const glm::vec3 p)
{
    m_position = p;
}

void Hitbox::SetCurrentMove(const glm::vec3 m)
{
    m_currentMove = m;
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

void Hitbox::ApplyMove()
{
    m_position += m_currentMove;
    m_currentMove = glm::vec3(0.f);
}