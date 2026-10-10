#pragma once

#include <glm/glm.hpp>

#define GRAVITY -24.f
#define EPSILON 0.001f

struct AABB;
class Hitbox;
class Terrain;

class CollisionResolver // TODO : Rename ?
{
    private:    
        const Terrain& m_terrain;
        
    public:
        CollisionResolver(const Terrain& terrain);

        // TODO : Rename
        void AxisX(const AABB& box, glm::vec3& move);
        void AxisY(const AABB& box, Hitbox& hitbox, glm::vec3& move); // TODO : Hitbox should not be here ?
        void AxisZ(const AABB& box, glm::vec3& move);

        void Resolve(Hitbox& hitbox, const float frameSpeed, const double deltaTime);
};