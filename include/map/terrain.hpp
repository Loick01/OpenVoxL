#pragma once 

#include <vector>

#include "map/chunk.hpp"
#include "map/map_generator.hpp"

// Default values
#define FREQUENCY 1.f
#define OCTAVES 0
#define SEED 0

enum class ChunkLayer
{
    Surface, Inner
};

class Terrain
{
    private:
        unsigned int m_nrChunkWidth; // X
        unsigned int m_nrChunkDepth; // Z
        unsigned int m_nrChunkHeight; // Y
        
        MapGenerator m_generator;    
        std::vector<Chunk> m_chunks; 
        std::map<ChunkLayer, std::pair<ChunkType, DataChunk>> m_dataChunks;
        unsigned int m_surfaceChunkHeight; // Number of Chunks used for the height of the surface. Thus Inner layer will have m_nrChunkHeight-m_surfaceChunkHeight

        DataChunk CreateDataChunk(const ChunkType type);

        void UpdateDataChunk(DataFlatChunk& data);
        void UpdateDataChunk(DataFullChunk& data);
        void UpdateDataChunk(DataWaveChunk& data);
        void UpdateDataChunk(DataEditorChunk& data);
        void UpdateDataChunk(DataHeightmapChunk& data);
        void UpdateDataChunk(DataCheeseChunk& data);
    
    public:
        Terrain(const unsigned int width, const unsigned int depth, const unsigned int height);

        glm::ivec3 GetSize() const;
        ChunkType GetChunkType(const ChunkLayer layer) const;
        const DataChunk& GetDataChunk(const ChunkLayer layer) const;
        unsigned int GetSurfaceChunkHeight() const;
        void SetSize(const glm::ivec3 size);
        void SetChunkType(const ChunkLayer layer, const ChunkType type);
        void SetDataChunk(const ChunkLayer layer, const DataChunk data);
        void SetSurfaceChunkHeight(const unsigned int surfaceChunkHeight);
        
        unsigned int GetChunkIndexInGrid(const glm::ivec3& chunkPosition) const;

        void UpdateDataChunk();
        void Create();
        void Load();
        void Draw(const glm::mat4& projection, const glm::mat4& view) const;
};