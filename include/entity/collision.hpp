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
        
    public:
        CollisionResolver(const Terrain& terrain);

        // TODO : Rename
        void AxisX(const AABB& hitbox, glm::vec3& move);
        void AxisY(const AABB& hitbox, glm::vec3& move);
        void AxisZ(const AABB& hitbox, glm::vec3& move);

        void Resolve(Hitbox& hitbox, const float frameSpeed);
};