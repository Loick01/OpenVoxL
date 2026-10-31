#pragma once

// TODO : Rename this file

#include <map>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "graphic/drawable.hpp"
#include "world/voxel.hpp"

class Visualization : public Drawable3D
{
    private:
        std::vector<Voxel> m_voxels;
        
        std::map<std::string, Face*> m_chunkFaces;

        GLuint m_blockId_SSBO;
        GLuint m_faceOrientation_SSBO;

        std::vector<unsigned int> m_blockIds; 
        std::vector<unsigned int> m_faceOrientations;

        void AddVoxel(const glm::ivec3 blockPosition, const unsigned int blockId);
        
        void AddFaceIndices(const unsigned int offset);
        void AddFace(const std::string& faceId, Face* face);
        void BuildFaces();

        void VoxelComputeData();
        void VoxelBufferData();
        
    public:
        Visualization(const std::string& vertexPath, const std::string& fragmentPath);
        ~Visualization();

        void Load() override;
        void Draw(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) const; // TODO : override
};