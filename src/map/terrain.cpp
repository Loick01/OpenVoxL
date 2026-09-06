#include "map/terrain.hpp"

#include "graphic/stb_image.h" // stbi_load

Terrain::Terrain(const unsigned int width, const unsigned int depth, const unsigned int height):
    m_nrChunkWidth(width), m_nrChunkDepth(depth), m_nrChunkHeight(height),
    m_generator(m_nrChunkWidth*CHUNK_SIZE, m_nrChunkDepth*CHUNK_SIZE, m_nrChunkHeight*CHUNK_SIZE, SEED, OCTAVE)
{
    m_chunks.reserve(m_nrChunkWidth*m_nrChunkDepth*m_nrChunkHeight);
    
    for (unsigned int k = 0 ; k < m_nrChunkHeight ; k++) { // Y
        for (unsigned int j = 0 ; j < m_nrChunkDepth ; j++) { // Z
            for (unsigned int i = 0 ; i < m_nrChunkWidth ; i++) { // X
                const glm::ivec3 terrainPosition = glm::ivec3(i, k, j);
                m_chunks.emplace_back("../shader/map/chunk.vs", "../shader/map/chunk.fs", terrainPosition, terrainPosition*CHUNK_SIZE, rand()%35);
            }
        }
    }
    
    m_generator.GenerateHeightMap();
    int heightmapWidth, heightmapDepth, channel;
    const unsigned char* heightmap = stbi_load("../data/heightmap/terrain.png", &heightmapWidth, &heightmapDepth, &channel, 1);
    if (heightmapWidth != m_nrChunkWidth*CHUNK_SIZE || heightmapDepth != m_nrChunkDepth*CHUNK_SIZE)
        throw std::runtime_error("The dimensions of the heightmap do not match with the size of the terrain");

    for (Chunk& c : m_chunks) {
        // c.BuildFlatChunk();
        // c.BuildWaveChunk(4.5f, m_nrChunkHeight*CHUNK_SIZE);
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