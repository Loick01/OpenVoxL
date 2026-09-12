#include "map/voxel.hpp"

namespace {
    constexpr glm::vec3 voxelVertices[24] = {
        {0, 0, 0}, {1, 0, 0}, {0, 0, 1}, {1, 0, 1}, // Bottom
        {0, 1, 1}, {1, 1, 1}, {0, 1, 0}, {1, 1, 0}, // Top
        {1, 0, 0}, {0, 0, 0}, {1, 1, 0}, {0, 1, 0}, // Back
        {0, 0, 1}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1}, // Front
        {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1}, // Left
        {1, 0, 1}, {1, 0, 0}, {1, 1, 1}, {1, 1, 0}  // Right
    };
}

Voxel::Voxel(const glm::vec3 origin, const unsigned int blockId):
    m_origin(origin), m_blockId(blockId)
{
    BuildVoxel();
}

Face* Voxel::GetFacePtr(const unsigned int faceIndex)
{
    return &m_faces[faceIndex];
}

std::string Voxel::GetFaceId(const unsigned int faceIndex) const
{
    return m_faces[faceIndex].id;
}

std::string Voxel::GetRootFaceId() const 
{
    return std::to_string((int)m_origin.x) + " " + std::to_string((int)m_origin.y) + " " + std::to_string((int)m_origin.z) + " ";
}

glm::vec3 Voxel::GetOrigin() const
{
    return m_origin;
}

std::vector<glm::vec3> Voxel::GetVertices() const
{
    std::vector<glm::vec3> vertices;
    for (unsigned int i = 0 ; i < m_faces.size() ; i++){
        for (unsigned int j = 0 ; j < 4 ; j++){
            vertices.push_back(m_faces[i].vertices[j]);
        }
    }
    return vertices;
}

// unsigned int Voxel::GetBlockId() const
// {
//     return m_blockId;
// }

// void Voxel::SetBlockId(const unsigned int blockId)
// {
//     m_blockId = blockId;
// }

unsigned int Voxel::GetFaceTextureId(const unsigned int blockId, const unsigned int orientation)
{
    if (blockId == 13) { // TODO : List of blocks with different textures
        return blockId + std::min(orientation, 2u); // TODO
    }
    return blockId;
}

void Voxel::BuildVoxel()
{
    const std::string rootId = GetRootFaceId();

    Face currentFace;
    for (unsigned int i = 0 ; i < 6 ; i++) {
        currentFace.id = rootId + std::to_string(i);
        for (unsigned int n = 0 ; n < 4 ; n++)
            currentFace.vertices.push_back(m_origin + voxelVertices[i*4+n]);
        currentFace.blockId = GetFaceTextureId(m_blockId, i);
        currentFace.orientation = i;
        m_faces.push_back(currentFace);
        currentFace.vertices.clear();
    }
}