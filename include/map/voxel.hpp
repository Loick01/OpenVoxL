#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

struct Face 
{
    std::vector<glm::vec3> vertices;
    std::string id; // TODO uint ? And I'm not really sure face id are necessary

    const std::vector<glm::vec3>& GetVertices() const
    {
        return vertices;
    }
};

class Voxel 
{
    private:
        std::vector<Face> m_faces;
        glm::vec3 m_origin; // Back-bottom-left vertex
        unsigned int m_blockId;
        // unsigned int m_idInChunk; // TODO Rename
        
    public:
        Voxel(const glm::vec3 origin, const unsigned int blockId);

        Face* GetFacePtr(const unsigned int faceIndex);
        std::string GetFaceId(const unsigned int faceIndex) const;
        std::string GetRootFaceId() const;

        glm::vec3 GetOrigin() const;
        std::vector<glm::vec3> GetVertices() const;
        // unsigned int GetIdInChunk() const;
        unsigned int GetBlockId() const;
        void SetBlockId(const unsigned int blockId);
        // void SetIdInChunk(const unsigned int idInChunk);

        void BuildVoxel();
};
