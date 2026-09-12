#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

struct Face 
{
    std::vector<glm::vec3> vertices;
    std::string id; // TODO uint ?
    unsigned int blockId;
    unsigned int orientation;

    const std::vector<glm::vec3>& GetVertices() const
    {
        return vertices;
    }

    unsigned int GetBlockId() const
    {
        return blockId;
    }
    
    unsigned int GetOrientation() const
    {
        return orientation;
    }
};

class Voxel 
{
    private:
        std::vector<Face> m_faces;
        glm::vec3 m_origin; // Back-bottom-left vertex
        unsigned int m_blockId;
        
        static unsigned int GetFaceTextureId(const unsigned int blockId, const unsigned int orientation);
 
    public:
        Voxel(const glm::vec3 origin, const unsigned int blockId);

        Face* GetFacePtr(const unsigned int faceIndex);
        std::string GetFaceId(const unsigned int faceIndex) const;
        std::string GetRootFaceId() const;

        glm::vec3 GetOrigin() const;
        std::vector<glm::vec3> GetVertices() const;

        void BuildVoxel();
};
