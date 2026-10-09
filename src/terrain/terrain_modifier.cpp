#include "terrain/terrain_modifier.hpp"

#include "core/camera.hpp"
#include "terrain/terrain.hpp"

TerrainModifier::TerrainModifier(const Camera& camera, Terrain& terrain):
    m_camera(camera), m_terrain(terrain)
{}

// http://www.cse.yorku.ca/~amana/research/grid.pdf
std::optional<RaycastHit> TerrainModifier::RaycastVoxelGrid(const glm::vec3 startPos, const glm::vec3 endPos) const
{
    const glm::ivec3 startVoxelPos = glm::ivec3(std::floor(startPos.x), std::floor(startPos.y), std::floor(startPos.z));
    const glm::ivec3 endVoxelPos = glm::ivec3(std::floor(endPos.x), std::floor(endPos.y), std::floor(endPos.z));
    glm::ivec3 currentVoxelPos = startVoxelPos;
    glm::ivec3 normal = glm::ivec3(0, 1, 0); // DO not use glm::ivec3(0) for default value

    const glm::vec3 ray = endPos - startPos;
    const glm::vec3 rayAbs = glm::abs(ray);

    const glm::ivec3 step{
        ray.x >= 0.0f ? 1 : -1,
        ray.y >= 0.0f ? 1 : -1,
        ray.z >= 0.0f ? 1 : -1
    };

    float tMaxX = 
        (rayAbs.x != 0.f) ? 
            step.x > 0 ? 
                (startVoxelPos.x+1 - startPos.x) / rayAbs.x
            :
                (startPos.x - startVoxelPos.x) / rayAbs.x 
        :
            std::numeric_limits<float>::infinity();
        
    float tMaxY = 
        (rayAbs.y != 0.f) ? 
            step.y > 0 ? 
                (startVoxelPos.y+1 - startPos.y) / rayAbs.y
            :
                (startPos.y - startVoxelPos.y) / rayAbs.y 
        :
            std::numeric_limits<float>::infinity();
    
    float tMaxZ = 
        (rayAbs.z != 0.f) ? 
            step.z > 0 ? 
                (startVoxelPos.z+1 - startPos.z) / rayAbs.z
            :
                (startPos.z - startVoxelPos.z) / rayAbs.z
        :
            std::numeric_limits<float>::infinity();

    const glm::vec3 tDelta = glm::vec3(
        (rayAbs.x != 0.f) ? 1.f / rayAbs.x : std::numeric_limits<float>::infinity(),
        (rayAbs.y != 0.f) ? 1.f / rayAbs.y : std::numeric_limits<float>::infinity(),
        (rayAbs.z != 0.f) ? 1.f / rayAbs.z : std::numeric_limits<float>::infinity()
    );

    if (m_terrain.IsSolidAt(currentVoxelPos))
        return RaycastHit{currentVoxelPos, normal}; // The ray starts inside a solid voxel (no face is actually targeted)

    while (currentVoxelPos != endVoxelPos) {
        if (tMaxX < tMaxY && tMaxX < tMaxZ) {
            tMaxX += tDelta.x;
            currentVoxelPos.x += step.x;
            normal = {-step.x, 0, 0};
        } else if (tMaxY <  tMaxZ) {
            tMaxY += tDelta.y;
            currentVoxelPos.y += step.y;
            normal = {0, -step.y, 0};
        } else {
            tMaxZ += tDelta.z;
            currentVoxelPos.z += step.z;
            normal = {0, 0, -step.z};
        }
        
        if (m_terrain.IsSolidAt(currentVoxelPos))
            return RaycastHit{currentVoxelPos, normal};
    }
    
    return std::nullopt;
}

void TerrainModifier::EventMouseButtonUpdate(const std::array<bool, GLFW_MOUSE_BUTTON_LAST+1>& mouseButtons, const float deltaTime)
{
    if (mouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
        const std::optional<RaycastHit> rayHitOpt = RaycastVoxelGrid(m_camera.GetPosition(), m_camera.GetPosition()+m_camera.GetFrontVector()*4.5f); // TODO : Voxel range distance
        if (rayHitOpt) {
            const RaycastHit& rayHit = rayHitOpt.value();
            // rayHit.voxelPos is in Terrain (IsSolidAt is used in RaycastVoxelGrid())
            Chunk& c = m_terrain.GetChunkFromVoxel(rayHit.voxelPos);
            c.DeleteBlock(rayHit.voxelPos%CHUNK_SIZE);
        }
    } else if (mouseButtons[GLFW_MOUSE_BUTTON_RIGHT]) {
        const std::optional<RaycastHit> rayHitOpt = RaycastVoxelGrid(m_camera.GetPosition(), m_camera.GetPosition()+m_camera.GetFrontVector()*4.5f); // TODO : Voxel range distance
        if (rayHitOpt) {
            const RaycastHit rayHit = rayHitOpt.value();
            const glm::ivec3 targetVoxelPos = rayHit.voxelPos + rayHit.normal;
            
            if (m_terrain.IsOutOfTerrain(targetVoxelPos))
                return;
            
            Chunk& c = m_terrain.GetChunkFromVoxel(targetVoxelPos);
            c.CreateBlock(targetVoxelPos%CHUNK_SIZE, 0); // TODO : blockId
        }
    }
}