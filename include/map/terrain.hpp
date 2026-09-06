#pragma once 

#include <vector>

#include "map/chunk.hpp"
#include "map/map_generator.hpp"

#define SEED 0
#define OCTAVE 4

class Terrain
{
    private:
        const unsigned int m_nrChunkWidth; // X
        const unsigned int m_nrChunkDepth; // Z
        const unsigned int m_nrChunkHeight; // Y
        
        MapGenerator m_generator;    
        std::vector<Chunk> m_chunks; 
    
    public:
        Terrain(const unsigned int width, const unsigned int depth, const unsigned int height);

        unsigned int GetChunkIndexInGrid(const glm::ivec3& chunkPosition) const;

        void Load();
        void Draw(const glm::mat4& projection, const glm::mat4& view) const;
};