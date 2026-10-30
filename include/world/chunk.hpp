#pragma once

#include <map>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "graphic/drawable.hpp"
#include "world/data_chunk.hpp"
#include "world/voxel.hpp"

#define CHUNK_SIZE 32
#define BLOCK_ID_MAX 35

enum class ChunkNeighbor
{
    Bottom, Top, Back, Front, Left, Right
};

class Chunk : public Drawable3D
{ 
    private:
        glm::ivec3 m_terrainPosition; // Position Column/Row/Depth 
        glm::ivec3 m_layerPosition; // Position of the Chunk in its layer (Surface/Below)
        glm::ivec3 m_terrainSize;
        glm::vec3 m_originPosition; // Back-bottom-left position (m_originPosition = m_terrainPosition*CHUNKSIZE)
        std::vector<Voxel> m_voxels;
        std::vector<int> m_gridVoxel;

        std::vector<unsigned int> m_blockIds;
        std::vector<unsigned int> m_faceOrientations;
        std::map<std::string, Face*> m_chunkFaces;
        std::map<ChunkNeighbor, Chunk*> m_chunkNeighbors;

        GLuint m_blockId_SSBO;
        GLuint m_faceOrientation_SSBO;

        unsigned int GetBlockIndexInGrid(const glm::ivec3& blockPosition) const;
        void AddVoxel(const glm::ivec3 blockPosition, const unsigned int blockId);
        void RemoveVoxel(const glm::ivec3 blockPosition);
        
        void AddFaceIndices(const unsigned int offset);
        void AddFace(const std::string& faceId, Face* face);
        void BuildFaces();

        void VoxelComputeData();
        void VoxelBufferData();

        void LoadNeighborChunk(const glm::ivec3 blockPosition, const ChunkNeighbor previous, const ChunkNeighbor next, 
            const int blockIndex, const unsigned int projectedIndex, const unsigned int axis); // TODO : axis parameter should use Axis enum ?
        
        static unsigned int RuleBlockId(const unsigned int distanceToSurface); // TODO : Rename ? + Create a class RuleBlockId ?
    
    public:
        Chunk(const std::string& vertexPath, const std::string& fragmentPath, const glm::ivec3 terrainPosition, 
            const glm::ivec3 layerPosition, const glm::ivec3 terrainSize, const glm::vec3 originPosition);
        ~Chunk();
        
        const std::vector<Voxel>& GetVoxels() const;

        Chunk* GetChunkNeighbor(const ChunkNeighbor which);
        void SetChunkNeighbor(const ChunkNeighbor which, Chunk* neighbor);
        
        glm::ivec3 GetTerrainPosition() const;

        bool IsEmptyAt(const glm::ivec3& voxelPosition) const;
        bool IsEmptyAt(const unsigned voxelIndex) const;

        void Build(const DataFullChunk& data);
        void Build(const DataFlatChunk& data);
        void Build(const DataWaveChunk& data);
        void Build(const DataEditorChunk& data);
        void Build(const DataHeightmapChunk& data);
        void Build(const DataCheeseChunk& data);
        void Build(const DataCaveChunk& data);

        void Load() override;

        void CreateBlock(const glm::ivec3 blockPosition, const unsigned int blockId);
        void CreateBlockWithoutLoad(const glm::ivec3 blockPosition, const unsigned int blockId);
        // Because I wanted to make sure I wouldn't forget to call Load() when updating a Chunk, I kept AddVoxel()/RemoveVoxel() private, and I added CreateBlockWithoutLoad()
        // This way, when I want to create multiple blocks and Load() only once after all the blocks have been created (for example in Terrain::BuildStructure()), I can use this function

        void DeleteBlock(const glm::ivec3 blockPosition);
        // void DeleteBlockWithoutLoad(const glm::ivec3 blockPosition); // TODO ? I suppose I will need it when I add the eraser tool
        
        void Draw(const glm::mat4& projection, const glm::mat4& view) const override;
        void DrawStructureVisualization(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) const; // TODO : Will not be here
};
