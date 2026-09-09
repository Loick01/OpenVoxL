#include "map/terrain.hpp"

#include "graphic/stb_image.h" // stbi_load

Terrain::Terrain(const unsigned int width, const unsigned int depth, const unsigned int height, const ChunkType chunkType):
    m_nrChunkWidth(width), m_nrChunkDepth(depth), m_nrChunkHeight(height), m_chunkType(chunkType), m_surfaceChunkHeight(height),
    m_generator(FREQUENCY, OCTAVES, SEED, FastNoise::SimplexFractal)
{
    CreateDataChunk();
    Create();
    Load();
}

glm::ivec3 Terrain::GetSize() const
{
    return glm::ivec3(m_nrChunkWidth, m_nrChunkHeight, m_nrChunkDepth);
}

ChunkType Terrain::GetChunkType() const
{
    return m_chunkType;
}

const DataChunk& Terrain::GetDataChunk() const
{
    return m_dataChunk;
}

unsigned int Terrain::GetSurfaceChunkHeight() const
{
    return m_surfaceChunkHeight;
}

unsigned int Terrain::GetChunkIndexInGrid(const glm::ivec3& chunkPosition) const
{
    return chunkPosition.y*m_nrChunkDepth*m_nrChunkWidth + chunkPosition.z*m_nrChunkWidth + chunkPosition.x;
}

void Terrain::SetSize(const glm::ivec3 size)
{
    m_nrChunkWidth = size.x;
    m_nrChunkHeight = size.y;
    m_nrChunkDepth = size.z;
}

void Terrain::SetChunkType(const ChunkType type)
{
    m_chunkType = type;
    CreateDataChunk();
}

void Terrain::SetDataChunk(const DataChunk data)
{
    m_dataChunk = data;
}

void Terrain::SetSurfaceChunkHeight(const unsigned int surfaceChunkHeight)
{
    m_surfaceChunkHeight = surfaceChunkHeight;
}

void Terrain::CreateDataChunk()
{
    switch (m_chunkType) {
        case ChunkType::Full : {
            m_dataChunk = DataFullChunk{};
            break; 
        }
        case ChunkType::Flat : {
            m_dataChunk = DataFlatChunk{};
            break; 
        }
        case ChunkType::Wave : {
            m_dataChunk = DataWaveChunk{1.f, m_nrChunkHeight*CHUNK_SIZE};
            break;
        } 
        case ChunkType::Editor : {
            m_dataChunk = DataEditorChunk{};
            break; 
        }
        case ChunkType::Heightmap : {
            m_generator.GenerateHeightMap(m_nrChunkWidth*CHUNK_SIZE, m_nrChunkDepth*CHUNK_SIZE, m_nrChunkHeight*CHUNK_SIZE);
            int heightmapWidth, heightmapDepth, channel;
            const unsigned char* heightmap = stbi_load("../data/heightmap/terrain.png", &heightmapWidth, &heightmapDepth, &channel, 1);
            if (heightmapWidth != m_nrChunkWidth*CHUNK_SIZE || heightmapDepth != m_nrChunkDepth*CHUNK_SIZE)
                throw std::runtime_error("The dimensions of the heightmap do not match with the size of the terrain");

            m_dataChunk = DataHeightmapChunk{heightmap, heightmapWidth, heightmapDepth, channel,
                m_generator.GetFrequency(), m_generator.GetOctaves(), m_generator.GetSeed(), m_generator.GetNoiseType()};
            break; 
        }
        case ChunkType::Cheese : {
            m_dataChunk = DataCheeseChunk{&m_generator.GetNoise(), 1.f, 0.f};
            break;
        }
        default:
            throw std::runtime_error("Terrain::Create : This ChunkType value should not be used here");
    }
}

void Terrain::UpdateDataChunk(DataFlatChunk& data)
{}

void Terrain::UpdateDataChunk(DataFullChunk& data)
{}

void Terrain::UpdateDataChunk(DataWaveChunk& data)
{
    data.maxBlockHeight = m_nrChunkHeight*CHUNK_SIZE;
}

void Terrain::UpdateDataChunk(DataEditorChunk& data)
{}

void Terrain::UpdateDataChunk(DataHeightmapChunk& data)
{
    m_generator.SetFrequency(data.frequency);
    m_generator.SetOctaves(data.octaves);
    m_generator.SetSeed(data.seed);
    m_generator.SetNoiseType(data.noiseType);
    m_generator.GenerateHeightMap(m_nrChunkWidth*CHUNK_SIZE, m_nrChunkDepth*CHUNK_SIZE, m_nrChunkHeight*CHUNK_SIZE);
    data.heightmap = stbi_load("../data/heightmap/terrain.png", &data.heightmapWidth, &data.heightmapDepth, &data.channel, 1);
    if (data.heightmapWidth != m_nrChunkWidth*CHUNK_SIZE || data.heightmapDepth != m_nrChunkDepth*CHUNK_SIZE)
        throw std::runtime_error("The dimensions of the heightmap do not match with the size of the terrain");
}

void Terrain::UpdateDataChunk(DataCheeseChunk& data)
{}

void Terrain::Create()
{
    m_chunks.clear();
    m_chunks.reserve(m_nrChunkWidth*m_nrChunkDepth*m_nrChunkHeight);
    const glm::ivec3 terrainSize = glm::ivec3(m_nrChunkWidth, m_nrChunkHeight, m_nrChunkDepth);
    
    for (unsigned int k = 0 ; k < m_nrChunkHeight ; k++) { // Y
        for (unsigned int j = 0 ; j < m_nrChunkDepth ; j++) { // Z
            for (unsigned int i = 0 ; i < m_nrChunkWidth ; i++) { // X
                const glm::ivec3 chunkPosition = glm::ivec3(i, k, j);
                m_chunks.emplace_back("../shader/map/chunk.vs", "../shader/map/chunk.fs", chunkPosition, terrainSize, chunkPosition*CHUNK_SIZE, rand()%35);
            }
        }
    }

    for (Chunk& c : m_chunks) {
        const glm::ivec3 chunkPosition = c.GetTerrainPosition(); // Position of the chunk in the terrain
        const unsigned int chunkIndexInGrid = GetChunkIndexInGrid(chunkPosition);
        if (chunkPosition.y > 0)
            c.SetChunkNeighbor(ChunkNeighbor::Bottom, &m_chunks[chunkIndexInGrid-m_nrChunkDepth*m_nrChunkWidth]);
        if (chunkPosition.y < m_nrChunkHeight-1)
            c.SetChunkNeighbor(ChunkNeighbor::Top, &m_chunks[chunkIndexInGrid+m_nrChunkDepth*m_nrChunkWidth]);
        if (chunkPosition.z > 0)
            c.SetChunkNeighbor(ChunkNeighbor::Back, &m_chunks[chunkIndexInGrid-m_nrChunkWidth]);
        if (chunkPosition.z < m_nrChunkDepth-1)
            c.SetChunkNeighbor(ChunkNeighbor::Front, &m_chunks[chunkIndexInGrid+m_nrChunkWidth]);
        if (chunkPosition.x > 0)
            c.SetChunkNeighbor(ChunkNeighbor::Left, &m_chunks[chunkIndexInGrid-1]);
        if (chunkPosition.x < m_nrChunkWidth-1)
            c.SetChunkNeighbor(ChunkNeighbor::Right, &m_chunks[chunkIndexInGrid+1]);
    }

    std::visit( 
        [this](auto& m_dataChunk)
        {
            UpdateDataChunk(m_dataChunk);
        },
        m_dataChunk
    );
    
    for (Chunk& c : m_chunks) {
        std::visit( 
            [&c](const auto& m_dataChunk)
            {
                c.Build(m_dataChunk);
            },
            m_dataChunk
        );
    }
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