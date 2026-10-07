#pragma once

#include <optional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "event/key_updatable.hpp"

class Camera;
class Terrain;

struct RaycastHit
{
    glm::ivec3 voxelPos;
    glm::ivec3 normal; // Targeted face normal
};

class TerrainModifier : public EventKeyUpdatable
{
    private:
        const Camera& m_camera;
        Terrain& m_terrain;

        std::optional<RaycastHit> RaycastVoxelGrid(const glm::vec3 startPos, const glm::vec3 endPos) const; // TODO : Rename ?
        
    public: 
        TerrainModifier(const Camera& camera, Terrain& terrain);

        void EventKeyUpdate(const std::array<bool, GLFW_KEY_LAST+1>& keys, const float deltaTime) override;
};