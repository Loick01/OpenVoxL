#pragma once

#include <glm/glm.hpp>

enum class Direction // TODO : Should not be here
{
    Front, Back, Left, Right
};

struct AABB
{
    glm::vec3 min;
    glm::vec3 max;

    AABB operator+(const glm::vec3 v) const {
        return {min+v, max+v};
    }
};

class Hitbox
{
    private:    
        AABB m_box;
        
        glm::vec3 m_position;  
        glm::vec3 m_frontVector;
        glm::vec3 m_rightVector;
        glm::vec3 m_currentMove;

        float m_height;
        float m_width;
    
    public:
        Hitbox(const glm::vec3 position, const float height, const float width);

        AABB GetBox() const;
        glm::vec3 GetPosition() const;
        glm::vec3 GetFrontVector() const;
        glm::vec3 GetCurrentMove() const;

        void ComputeAABB();

        void SetFrontVector(const glm::vec3 front);
        void SetRightVector(const glm::vec3 right);

        void SetPosition(const glm::vec3 p);
        void SetCurrentMove(const glm::vec3 m);

        void RequestMove(const Direction dir);
        void ApplyMove();
};