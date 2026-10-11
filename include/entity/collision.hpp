#pragma once

#include <glm/glm.hpp>

#define EPSILON 0.001f

struct AABB;
class Hitbox;
class Terrain;

class CollisionResolver // TODO : Rename ?
{
    private:    
        const Terrain& m_terrain;

        float m_gravity;
        
    public:
        CollisionResolver(const Terrain& terrain, const float gravity);

        float GetGravity() const;

        // TODO : Rename
        bool AxisX(const AABB& box, glm::vec3& move);
        bool AxisY(const AABB& box, glm::vec3& move);
        bool AxisZ(const AABB& box, glm::vec3& move);

        void SetGravity(const float gravity);

        void Resolve(Hitbox& hitbox, const float frameSpeed, const double deltaTime);
};