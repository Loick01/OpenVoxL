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
    
    public:
        Terrain(const unsigned int width, const unsigned int depth, const unsigned int height, const ChunkType chunkType);

        glm::ivec3 GetSize() const;
        ChunkType GetChunkType() const;
        unsigned int GetChunkIndexInGrid(const glm::ivec3& chunkPosition) const;
        void SetSize(const glm::ivec3 size);

        void Create();
        void Load();
        void Draw(const glm::mat4& projection, const glm::mat4& view) const;
};