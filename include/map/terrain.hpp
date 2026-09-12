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
    Surface, Below
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
        unsigned int m_surfaceChunkHeight; // Number of Chunks used for the height of the surface. Thus Below layer will have m_nrChunkHeight-m_surfaceChunkHeight

        DataChunk CreateDataChunk(const ChunkLayer layer, const ChunkType type);

        void UpdateDataChunk(const ChunkLayer layer, DataFlatChunk& data);
        void UpdateDataChunk(const ChunkLayer layer, DataFullChunk& data);
        void UpdateDataChunk(const ChunkLayer layer, DataWaveChunk& data);
        void UpdateDataChunk(const ChunkLayer layer, DataEditorChunk& data);
        void UpdateDataChunk(const ChunkLayer layer, DataHeightmapChunk& data);
        void UpdateDataChunk(const ChunkLayer layer, DataCheeseChunk& data);
    
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
        
        unsigned int GetMaxHeightForLayer(const ChunkLayer layer) const;
        unsigned int GetChunkIndexInGrid(const glm::ivec3& chunkPosition) const;

        void Create();
        void Load();
        void Draw(const glm::mat4& projection, const glm::mat4& view) const;
};