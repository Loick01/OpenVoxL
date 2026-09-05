#pragma once 

#include <vector>

#include "map/chunk.hpp"
#include "map/map_generator.hpp"

class Terrain
{
    private:
        MapGenerator m_generator;
        
        std::vector<Chunk> m_chunks;
        const unsigned int m_width;
        const unsigned int m_depth;
        // const unsigned int m_height; 
    
    public:
        Terrain(const unsigned int width, const unsigned int depth);

        void Load();
        void Draw(const glm::mat4& projection, const glm::mat4& view) const;
};