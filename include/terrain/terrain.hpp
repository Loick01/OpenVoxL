#pragma once 

#include <vector>

#include "terrain/chunk.hpp"
#include "terrain/map_generator.hpp"

// Default values
#define FREQUENCY 1.f
#define OCTAVES 0
#define SEED 0

enum class ChunkLayer
{
    Surface, Below
};

class FileReader;

class Terrain
{
    private:
        const FileReader& m_fileReader;

        // TODO : Should have m_worldSize = GetSize()*CHUNK_SIZE ? 
        glm::ivec3 m_sizeChunk;
        
        MapGenerator m_generator;    
        std::vector<Chunk> m_chunks; 
        std::map<ChunkLayer, std::pair<ChunkType, DataChunk>> m_dataChunks;
        unsigned int m_surfaceChunkHeight; // Number of Chunks used for the height of the surface. Thus Below layer will have m_nrChunkHeight-m_surfaceChunkHeight
        
        DataChunk CreateDataChunk(const ChunkLayer layer, const ChunkType type);
        
        void UpdateDataChunk(const ChunkLayer layer, DataFlatChunk& data);
        void UpdateDataChunk(const ChunkLayer layer, DataFullChunk& data);
        void UpdateDataChunk(const ChunkLayer layer, DataWaveChunk& data);
        void UpdateDataChunk(const ChunkLayer layer, DataEditorChunk& data);
        void UpdateDataChunk(const ChunkLayer layer, DataHeightmapChunk& data);
        void UpdateDataChunk(const ChunkLayer layer, DataCheeseChunk& data);
        void UpdateDataChunk(const ChunkLayer layer, DataCaveChunk& data);
    
    public:
        Terrain(const FileReader& fileReader, const glm::ivec3 sizeChunk);

        const Chunk& GetChunkFromVoxel(const glm::ivec3& voxelPosition) const;
        Chunk& GetChunkFromVoxel(const glm::ivec3& voxelPosition);

        glm::ivec3 GetSizeChunk() const;
        ChunkType GetChunkType(const ChunkLayer layer) const;
        const DataChunk& GetDataChunk(const ChunkLayer layer) const;
        unsigned int GetSurfaceChunkHeight() const;

        bool IsOutOfTerrain(const glm::ivec3& voxelPosition) const;
        bool IsSolidAt(const glm::ivec3& voxelPosition) const; // TODO : Rename ?

        void SetSizeChunk(const glm::ivec3 sizeChunk); // TODO : Should be private ? Do not use without a call to Load()
        void SetChunkType(const ChunkLayer layer, const ChunkType type);
        void SetDataChunk(const ChunkLayer layer, const DataChunk data);
        void SetSurfaceChunkHeight(const unsigned int surfaceChunkHeight);
        
        unsigned int GetMaxHeightForLayer(const ChunkLayer layer) const;
        unsigned int GetChunkIndexInGrid(const glm::ivec3& chunkPosition) const;

        void SaveEditorChunkInFile(const std::string& name) const;
        void BuildStructure(const glm::ivec3 bp, const std::string& filepath);

        void Create();
        void Load();
        void Draw(const glm::mat4& projection, const glm::mat4& view) const;
};