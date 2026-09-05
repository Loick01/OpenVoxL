#include "map/terrain.hpp"

#include "graphic/stb_image.h" // stbi_load

Terrain::Terrain(const unsigned int width, const unsigned int depth):
    m_nrChunkWidth(width), m_nrChunkDepth(depth), m_generator(m_nrChunkWidth*CHUNK_SIZE, m_nrChunkDepth*CHUNK_SIZE, 1*CHUNK_SIZE, SEED, OCTAVE)
{
    m_chunks.reserve(m_nrChunkWidth*m_nrChunkDepth);
    
    for (unsigned int j = 0 ; j < m_nrChunkDepth ; j++) {
        for (unsigned int i = 0 ; i < m_nrChunkWidth ; i++) {
            m_chunks.emplace_back("../shader/map/chunk.vs", "../shader/map/chunk.fs", glm::ivec3(i, 0, j), glm::vec3(i*CHUNK_SIZE, 0.f, j*CHUNK_SIZE), j*m_nrChunkWidth+i);
        }
    }
    
    m_generator.GenerateHeightMap();
    int heightmapWidth, heightmapDepth, channel;
    const unsigned char* heightmap = stbi_load("../data/heightmap/terrain.png", &heightmapWidth, &heightmapDepth, &channel, 1);

    for (Chunk& c : m_chunks) {
        // c.BuildWaveChunk(4.5f);
        c.BuildHeightmapChunk(heightmap, heightmapWidth, heightmapDepth);
        // c.BuildCheeseChunk(m_generator.GetNoise(), 4.f);
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