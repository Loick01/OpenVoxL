#pragma once

#include <optional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "event/mouse_updatable.hpp"

class Camera;
class Hotbar;
class Terrain;

struct RaycastHit
{
    glm::ivec3 voxelPos;
    glm::ivec3 normal; // Targeted face normal
};

struct TimeDelay // TODO : Will not be here
{
    float count;
    float delay;
    
    TimeDelay(const float d) {
        delay = d;
        Reset();
    }

    bool IsFinished() const {
        return count > delay;
    }

    void Reset() {
        count = 0.f;
    }
};

class TerrainModifier : public EventMouseUpdatable
{
    private:
        const Camera& m_camera;
        const Hotbar& m_hotbar;
        Terrain& m_terrain;

        std::optional<RaycastHit> RaycastVoxelGrid(const glm::vec3 startPos, const glm::vec3 endPos) const;
        
        TimeDelay m_updateDelay;

    public: 
        TerrainModifier(const Camera& camera, const Hotbar& hotbar, Terrain& terrain, const float delay);

        TimeDelay& GetUpdateDelay();
        void SetUpdateDelay(const float delay); // TODO : Should be in TimeDelay struct ?

        void EventMouseButtonUpdate(const std::array<bool, GLFW_MOUSE_BUTTON_LAST+1>& mouseButtons, const float deltaTime) override;
};