#pragma once 

#include <vector>

#include "map/chunk.hpp"
#include "map/map_generator.hpp"

#define SEED 0
#define OCTAVE 4

class Terrain
{
    private:
        unsigned int m_nrChunkWidth; // X
        unsigned int m_nrChunkDepth; // Z
        unsigned int m_nrChunkHeight; // Y
        
        MapGenerator m_generator;    
        std::vector<Chunk> m_chunks; 
        ChunkType m_chunkType;
        DataChunk m_dataChunk;

        void CreateDataChunk();

        void UpdateDataChunk(DataFlatChunk& data);
        void UpdateDataChunk(DataFullChunk& data);
        void UpdateDataChunk(DataWaveChunk& data);
        void UpdateDataChunk(DataEditorChunk& data);
        void UpdateDataChunk(DataHeightmapChunk& data);
        void UpdateDataChunk(DataCheeseChunk& data);
    
    public:
        Terrain(const unsigned int width, const unsigned int depth, const unsigned int height, const ChunkType chunkType);

        glm::ivec3 GetSize() const;
        ChunkType GetChunkType() const;
        const DataChunk& GetDataChunk() const;
        void SetSize(const glm::ivec3 size);
        void SetChunkType(const ChunkType type);
        void SetDataChunk(const DataChunk data);
        
        unsigned int GetChunkIndexInGrid(const glm::ivec3& chunkPosition) const;

        void UpdateDataChunk();
        void Create();
        void Load();
        void Draw(const glm::mat4& projection, const glm::mat4& view) const;
};