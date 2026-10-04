#pragma once

#include <glm/glm.hpp>

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
        void AxisX(AABB& hitbox, glm::vec3& move);
        void AxisZ(AABB& hitbox, glm::vec3& move);

        void Resolve(Hitbox& hitbox, const float frameSpeed);
};