#include "chunk/voxel.hpp"

Voxel::Voxel(const glm::vec3 origin, const unsigned int objectId):
    m_origin(origin), m_objectId(objectId)
{
    BuildVoxel();
}

std::string Voxel::GetFaceId(const unsigned int faceIndex) const
{
    return m_faces[faceIndex].id;
}

std::string Voxel::GetRootFaceId() const 
{
    // TODO : cast int is useless ? (.0 is already removed from id ?)
    return std::to_string((int)m_origin.x) + " " + std::to_string((int)m_origin.y) + " " + std::to_string((int)m_origin.z) + " ";
}

std::vector<glm::vec3> Voxel::GetFaceVertices(const unsigned int faceIndex) const
{
    return m_faces[faceIndex].vertices;
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

unsigned int Voxel::GetIdInChunk() const
{
    return m_idInChunk;
}

unsigned int Voxel::GetObjectId() const
{
    return m_objectId;
}

void Voxel::SetObjectId(const unsigned int objectId)
{
    m_objectId = objectId;
}

void Voxel::SetIdInChunk(const unsigned int idInChunk)
{
    m_idInChunk = idInChunk;
}

void Voxel::BuildVoxel()
{
    const std::string rootId = GetRootFaceId();

    Face currentFace;
    currentFace.id = rootId + std::to_string(0); // Bottom
    currentFace.vertices.push_back(m_origin);
    currentFace.vertices.push_back(m_origin + glm::vec3(1, 0, 0));
    currentFace.vertices.push_back(m_origin + glm::vec3(0, 0, 1));
    currentFace.vertices.push_back(m_origin + glm::vec3(1, 0, 1));
    m_faces.push_back(currentFace);

    currentFace.vertices.clear();
    currentFace.id = rootId + std::to_string(1); // Top
    currentFace.vertices.push_back(m_origin + glm::vec3(0, 1, 1));
    currentFace.vertices.push_back(m_origin + glm::vec3(1, 1, 1));
    currentFace.vertices.push_back(m_origin + glm::vec3(0, 1, 0));
    currentFace.vertices.push_back(m_origin + glm::vec3(1, 1, 0));
    m_faces.push_back(currentFace);

    currentFace.vertices.clear();
    currentFace.id = rootId + std::to_string(2); // Back
    currentFace.vertices.push_back(m_origin + glm::vec3(1, 0, 0));
    currentFace.vertices.push_back(m_origin);
    currentFace.vertices.push_back(m_origin + glm::vec3(1, 1, 0));
    currentFace.vertices.push_back(m_origin + glm::vec3(0, 1, 0));
    m_faces.push_back(currentFace);

    currentFace.vertices.clear();
    currentFace.id = rootId + std::to_string(3); // Front
    currentFace.vertices.push_back(m_origin + glm::vec3(0, 0, 1));
    currentFace.vertices.push_back(m_origin + glm::vec3(1, 0, 1));
    currentFace.vertices.push_back(m_origin + glm::vec3(0, 1, 1));
    currentFace.vertices.push_back(m_origin + glm::vec3(1, 1, 1));
    m_faces.push_back(currentFace);

    currentFace.vertices.clear();
    currentFace.id = rootId + std::to_string(4); // Left
    currentFace.vertices.push_back(m_origin + glm::vec3(0, 0, 0));
    currentFace.vertices.push_back(m_origin + glm::vec3(0, 0, 1));
    currentFace.vertices.push_back(m_origin + glm::vec3(0, 1, 0));
    currentFace.vertices.push_back(m_origin + glm::vec3(0, 1, 1));
    m_faces.push_back(currentFace);

    currentFace.vertices.clear();
    currentFace.id = rootId + std::to_string(5); // Right
    currentFace.vertices.push_back(m_origin + glm::vec3(1, 0, 1));
    currentFace.vertices.push_back(m_origin + glm::vec3(1, 0, 0));
    currentFace.vertices.push_back(m_origin + glm::vec3(1, 1, 1));
    currentFace.vertices.push_back(m_origin + glm::vec3(1, 1, 0));
    m_faces.push_back(currentFace);
}