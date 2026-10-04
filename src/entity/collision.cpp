#include "entity/collision.hpp"

#include "entity/hitbox.hpp"
#include "terrain/terrain.hpp"

CollisionResolver::CollisionResolver(const Terrain& terrain):
    m_terrain(terrain)
{}

void CollisionResolver::AxisX(AABB& box, glm::vec3& move)
{
    if (move.x == 0.f) return;

    float targetMinX = box.min.x + move.x;
    float targetMaxX = box.max.x + move.x;

    const glm::ivec3 rangeMin = glm::ivec3(std::floor(targetMinX), std::floor(box.min.y), std::floor(box.min.z));
    const glm::ivec3 rangeMax = glm::ivec3(std::floor(targetMaxX), std::floor(box.max.y), std::floor(box.max.z));
    for (int i = rangeMin.x ; i <= rangeMax.x ; i++) {
        for (int k = rangeMin.y ; k <= rangeMax.y ; k++) {
            for (int j = rangeMin.z ; j <= rangeMax.z ; j++) {
                if (!m_terrain.IsSolidAt(glm::ivec3(i, k, j)))
                    continue;

                if (move.x > 0.f)
                    move.x = i - box.max.x;
                else 
                    move.x = i+1 - box.min.x;

                return;
            }
        }  
    }
}

void CollisionResolver::AxisZ(AABB& box, glm::vec3& move)
{
    if (move.z == 0.f) return;

    float targetMinZ = box.min.z + move.z;
    float targetMaxZ = box.max.z + move.z;

    const glm::ivec3 rangeMin = glm::ivec3(std::floor(box.min.x), std::floor(box.min.y), std::floor(targetMinZ));
    const glm::ivec3 rangeMax = glm::ivec3(std::floor(box.max.x), std::floor(box.max.y), std::floor(targetMaxZ));
    for (int i = rangeMin.x ; i <= rangeMax.x ; i++) {
        for (int k = rangeMin.y ; k <= rangeMax.y ; k++) {
            for (int j = rangeMin.z ; j <= rangeMax.z ; j++) {
                if (!m_terrain.IsSolidAt(glm::ivec3(i, k, j)))
                    continue;

                if (move.z > 0.f)
                    move.z = j - box.max.z;
                else 
                    move.z = j+1 - box.min.z;

                return;
            }
        }  
    }
}

void CollisionResolver::Resolve(Hitbox& hitbox, const float frameSpeed)
{
    glm::vec3 move = hitbox.GetCurrentMove();
    
    if (move == glm::vec3(0.f))
        return;
    
    move = glm::normalize(move)*frameSpeed;
    
    hitbox.ComputeAABB();
    AABB box = hitbox.GetBox();

    AxisX(box, move);
    box.min.x += move.x;
    box.max.x += move.x;
    AxisZ(box, move);
    box.min.z += move.z;
    box.max.z += move.z;

    hitbox.SetCurrentMove(move);
    hitbox.ApplyMove(); // TODO : Should not be here
}