#include "entity/collision.hpp"

#include "entity/hitbox.hpp"
#include "terrain/terrain.hpp"

CollisionResolver::CollisionResolver(const Terrain& terrain):
    m_terrain(terrain)
{}

void CollisionResolver::AxisX(const AABB& box, glm::vec3& move)
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
                    move.x = i - box.max.x - EPSILON;
                else 
                    move.x = i+1 - box.min.x + EPSILON;

                return;
            }
        }  
    }
}

void CollisionResolver::AxisY(const AABB& box, Hitbox& hitbox, glm::vec3& move)
{
    if (move.y == 0.f) return;

    float targetMinY = box.min.y + move.y;
    float targetMaxY = box.max.y + move.y;

    const glm::ivec3 rangeMin = glm::ivec3(std::floor(box.min.x), std::floor(targetMinY), std::floor(box.min.z));
    const glm::ivec3 rangeMax = glm::ivec3(std::floor(box.max.x), std::floor(targetMaxY), std::floor(box.max.z));
    for (int i = rangeMin.x ; i <= rangeMax.x ; i++) {
        for (int k = rangeMin.y ; k <= rangeMax.y ; k++) {
            for (int j = rangeMin.z ; j <= rangeMax.z ; j++) {
                if (!m_terrain.IsSolidAt(glm::ivec3(i, k, j)))
                    continue;

                if (move.y > 0.f)
                    move.y = k - box.max.y - EPSILON;
                else 
                    move.y = k+1 - box.min.y + EPSILON;

                hitbox.SetVerticalVelocity(0.f); // TODO : Should not be here + AxisX/Y/Z() should return move.x/y/z ?

                return;
            }
        }  
    }
}

void CollisionResolver::AxisZ(const AABB& box, glm::vec3& move)
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
                    move.z = j - box.max.z - EPSILON;
                else 
                    move.z = j+1 - box.min.z + EPSILON;

                return;
            }
        }  
    }
}

void CollisionResolver::Resolve(Hitbox& hitbox, const float frameSpeed, const double deltaTime)
{
    glm::vec3 move = hitbox.GetCurrentMove();
    move.y = 0.f;
    
    if (glm::length(move) > 0.f)
        move = glm::normalize(move)*frameSpeed;

    float verticalVelocity = hitbox.GetVerticalVelocity();
    verticalVelocity += GRAVITY * deltaTime;
    hitbox.SetVerticalVelocity(verticalVelocity);

    move.y = verticalVelocity * deltaTime;
    
    hitbox.ComputeAABB();
    AABB box = hitbox.GetBox();

    AxisX(box, move);
    box.min.x += move.x;
    box.max.x += move.x;
    AxisZ(box, move);
    box.min.z += move.z;
    box.max.z += move.z;
    AxisY(box, hitbox, move);

    hitbox.SetCurrentMove(move);
    hitbox.ApplyMove();
}