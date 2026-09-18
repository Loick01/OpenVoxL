#pragma once

#include <glm/glm.hpp>

enum class Direction // TODO : Should not be here
{
    Front, Back, Left, Right
};

class Hitbox
{
    private:    
        glm::vec3 m_position;  
        glm::vec3 m_frontVector;
        glm::vec3 m_rightVector;
        glm::vec3 m_currentMove;
    
    public:
        Hitbox(const glm::vec3 position);

        glm::vec3 GetPosition() const;
        glm::vec3 GetFrontVector() const;

        void SetFrontVector(const glm::vec3 front);
        void SetRightVector(const glm::vec3 right);

        void SetPosition(const glm::vec3 p);

        void RequestMove(const Direction dir);
        void ApplyMove(const float frameSpeed); // frameSpeed = speed*deltaTime);
};