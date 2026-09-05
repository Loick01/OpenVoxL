#include "map/terrain.hpp"

Terrain::Terrain(const unsigned int width, const unsigned int depth):
    m_width(width), m_depth(depth)
{
    m_chunks.reserve(m_width*m_depth);
    
    for (unsigned int j = 0 ; j < m_depth ; j++) {
        for (unsigned int i = 0 ; i < m_width ; i++) {
            m_chunks.emplace_back("../shader/map/chunk.vs", "../shader/map/chunk.fs", glm::ivec3(i, 0, j), glm::vec3(i*CHUNK_SIZE, 0.f, j*CHUNK_SIZE), j*m_width+i);
        }
    }
    
    Load();
}

void Terrain::Load()
{
    for (Chunk& c : m_chunks)
        c.Load();
}

void Terrain::Draw(const glm::mat4& projection, const glm::mat4& view) const
{
    for (const Chunk& c : m_chunks)
        c.Draw(projection, view);
}