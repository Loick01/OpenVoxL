#include "entity/collision.hpp"

#include "entity/hitbox.hpp"
#include "terrain/terrain.hpp"

CollisionResolver::CollisionResolver(const Terrain& terrain):
    m_terrain(terrain)
{}

void CollisionResolver::Resolve(Hitbox& hitbox, const float frameSpeed)
{
    glm::vec3 move = hitbox.GetCurrentMove();
    
    if (move == glm::vec3(0.f))
        return;
    
    move.y = 0.f; // TODO : Remove
    move = glm::normalize(move)*frameSpeed;
    
    const AABB targetBox = hitbox.GetBox() + move;

    const glm::ivec3 rangeMin = glm::ivec3(std::floor(targetBox.min.x), std::floor(targetBox.min.y), std::floor(targetBox.min.z));
    const glm::ivec3 rangeMax = glm::ivec3(std::floor(targetBox.max.x), std::floor(targetBox.max.y), std::floor(targetBox.max.z));
    for (int i = rangeMin.x ; i <= rangeMax.x ; i++) {
        for (int k = rangeMin.y ; k <= rangeMax.y ; k++) {
            for (int j = rangeMin.z ; j <= rangeMax.z ; j++) {
                // if (m_terrain.IsSolidAt(glm::ivec3(i, k, j)))
                //     std::cout << "Solid at " << i << ", " << k << ", " << j << "\n"; 
                // else   
                //     std::cout << "Empty at " << i << ", " << k << ", " << j << "\n"; 
            }
        }  
    }

    hitbox.SetCurrentMove(move);
    hitbox.ApplyMove(); // TODO : Should not be here
}