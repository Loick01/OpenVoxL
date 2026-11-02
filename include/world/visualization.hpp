#pragma once

// TODO : Rename this file

#include <map>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "graphic/drawable.hpp"
#include "world/voxel.hpp"

struct StructureData;

class Visualization : public Drawable3D
{
    private:
        std::vector<Voxel> m_voxels;
        
        std::vector<Face*> m_chunkFaces;

        GLuint m_blockId_SSBO;
        GLuint m_faceOrientation_SSBO;

        std::vector<unsigned int> m_blockIds; 
        std::vector<unsigned int> m_faceOrientations;

        void AddVoxel(const glm::ivec3 blockPosition, const unsigned int blockId);
        
        void AddFaceIndices(const unsigned int offset);
        void BuildFaces();

        void VoxelComputeData();
        void VoxelBufferData();
        
        void FillWith(const StructureData& structData);
        void Load() override;
        
    public:
        Visualization(const std::string& vertexPath, const std::string& fragmentPath);
        ~Visualization();

        void UpdateData(const StructureData& structData);

        void Draw(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) const override;
};