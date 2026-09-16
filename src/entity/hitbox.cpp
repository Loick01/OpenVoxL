#include "entity/hitbox.hpp"

Hitbox::Hitbox(const glm::vec3 position):
    m_position(position)
{}

glm::vec3 Hitbox::GetPosition() const
{
    return m_position;
}