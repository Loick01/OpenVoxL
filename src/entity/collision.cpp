#include "entity/collision.hpp"

#include "entity/hitbox.hpp"

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
        for (int j = rangeMin.y ; j <= rangeMax.y ; j++) {
            for (int k = rangeMin.z ; k <= rangeMax.z ; k++) {
                // TODO : Check if there is a collision
            }
        }  
    }

    hitbox.SetCurrentMove(move);
    hitbox.ApplyMove(); // TODO : Should not be here
}