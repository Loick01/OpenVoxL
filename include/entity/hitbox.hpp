#pragma once

#include <glm/glm.hpp>

class Hitbox
{
    private:    
        glm::vec3 m_position;
    
    public:
        Hitbox(const glm::vec3 position);

        glm::vec3 GetPosition() const;
};