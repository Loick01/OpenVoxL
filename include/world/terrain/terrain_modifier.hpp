#pragma once

#include <optional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "event/mouse_updatable.hpp"
#include "world/structure.hpp"

class Camera;
class Hotbar;
class Hud;
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
        const Hud& m_hud; // TODO : Remove ?
        Terrain& m_terrain;

        std::optional<RaycastHit> RaycastVoxelGrid(const glm::vec3 startPos, const glm::vec3 endPos) const;
        
        TimeDelay m_updateDelay;
        float m_reachDistance;

        StructureData m_currentStructure;

    public: 
        TerrainModifier(const Camera& camera, const Hotbar& hotbar, const Hud& hud, Terrain& terrain, 
            const float delay, const float distance);

        TimeDelay GetUpdateDelay() const;
        float GetReachDistance() const;
        
        void SetUpdateDelay(const float delay); // TODO : Should be in TimeDelay struct ?
        void SetReachDistance(const float distance);
        void SetCurrentStructure(const StructureData& structure);

        void EventMouseButtonUpdate(const std::array<bool, GLFW_MOUSE_BUTTON_LAST+1>& mouseButtons, const float deltaTime) override;
};