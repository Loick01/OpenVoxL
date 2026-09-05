#pragma once 

#include <vector>

#include "map/chunk.hpp"
#include "map/map_generator.hpp"

#define SEED 0
#define OCTAVE 4

class Terrain
{
    private:
        const unsigned int m_nrChunkWidth;
        const unsigned int m_nrChunkDepth;
        // const unsigned int m_nrChunkHeight;
        
        MapGenerator m_generator;    
        std::vector<Chunk> m_chunks; 
    
    public:
        Terrain(const unsigned int width, const unsigned int depth);

        void Load();
        void Draw(const glm::mat4& projection, const glm::mat4& view) const;
};