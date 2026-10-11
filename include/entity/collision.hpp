#pragma once

#include <glm/glm.hpp>

#define GRAVITY -30.f
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
        bool AxisX(const AABB& box, glm::vec3& move);
        bool AxisY(const AABB& box, glm::vec3& move);
        bool AxisZ(const AABB& box, glm::vec3& move);

        void Resolve(Hitbox& hitbox, const float frameSpeed, const double deltaTime);
};