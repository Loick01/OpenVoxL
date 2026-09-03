#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

struct Face 
{
    std::vector<glm::vec3> vertices;
    std::string id; // TODO uint ?
};

class Voxel 
{
    private:
        std::vector<Face> m_faces;
        glm::vec3 m_origin; // Back-bottom-left vertex
        unsigned int m_objectId; // TODO Rename
        unsigned int m_idInChunk; // TODO Rename
        
    public:
        Voxel(const glm::vec3 origin, const unsigned int objectId);

        std::string GetFaceId(const unsigned int faceIndex) const;
        std::string GetRootFaceId() const;
        std::vector<glm::vec3> GetFaceVertices(const unsigned int faceIndex) const;

        glm::vec3 GetOrigin() const;
        std::vector<glm::vec3> GetVertices() const;
        unsigned int GetIdInChunk() const;
        unsigned int GetObjectId() const;
        void SetObjectId(const unsigned int objectId);
        void SetIdInChunk(const unsigned int idInChunk);

        void BuildVoxel();
};
