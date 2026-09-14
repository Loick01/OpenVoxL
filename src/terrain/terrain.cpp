#include "terrain/terrain.hpp"

#include "graphic/stb_image.h" // stbi_load

Terrain::Terrain(const unsigned int width, const unsigned int depth, const unsigned int height):
    m_nrChunkWidth(width), m_nrChunkDepth(depth), m_nrChunkHeight(height), m_surfaceChunkHeight(height),
    m_generator(FREQUENCY, OCTAVES, SEED, FastNoise::SimplexFractal)
{
    SetChunkType(ChunkLayer::Surface, ChunkType::Heightmap);
    SetChunkType(ChunkLayer::Below, ChunkType::Cheese);

    Create();
    Load();
}

glm::ivec3 Terrain::GetSize() const
{
    return glm::ivec3(m_nrChunkWidth, m_nrChunkHeight, m_nrChunkDepth);
}

ChunkType Terrain::GetChunkType(const ChunkLayer layer) const
{
    return m_dataChunks.at(layer).first;
}

const DataChunk& Terrain::GetDataChunk(const ChunkLayer layer) const
{
    return m_dataChunks.at(layer).second;
}

unsigned int Terrain::GetSurfaceChunkHeight() const
{
    return m_surfaceChunkHeight;
}

unsigned int Terrain::GetMaxHeightForLayer(const ChunkLayer layer) const
{
    switch (layer) {
        case ChunkLayer::Surface : 
            return m_surfaceChunkHeight*CHUNK_SIZE;
        case ChunkLayer::Below : 
            return (m_nrChunkHeight-m_surfaceChunkHeight)*CHUNK_SIZE;
        default :   
            throw std::runtime_error("Terrain::GetMaxHeightForLayer : This ChunkLayer value should not be used here");
    }
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

void Terrain::SetChunkType(const ChunkLayer layer, const ChunkType type)
{
    m_dataChunks[layer] = {type, CreateDataChunk(layer, type)};
}

void Terrain::SetDataChunk(const ChunkLayer layer, const DataChunk data)
{
    m_dataChunks.at(layer) = {GetChunkType(layer), data};
}

void Terrain::SetSurfaceChunkHeight(const unsigned int surfaceChunkHeight)
{
    m_surfaceChunkHeight = surfaceChunkHeight;
}

DataChunk Terrain::CreateDataChunk(const ChunkLayer layer, const ChunkType type)
{
    DataChunk data;
    switch (type) {
        case ChunkType::Full : {
            data = DataFullChunk();
            break; 
        }
        case ChunkType::Flat : {
            data = DataFlatChunk();
            break; 
        }
        case ChunkType::Wave : {
            data = DataWaveChunk();
            break;
        } 
        case ChunkType::Editor : {
            data = DataEditorChunk();
            break; 
        }
        case ChunkType::Heightmap : {
            data = DataHeightmapChunk(m_generator.GetNoiseParams());
            break; 
        }
        case ChunkType::Cheese : {
            data = DataCheeseChunk(&m_generator, m_generator.GetNoiseParams());
            break;
        }
        case ChunkType::Cave : {            
            data = DataCaveChunk();
            break;
        }
        default:
            throw std::runtime_error("Terrain::Create : This ChunkType value should not be used here");
    }
    
    std::visit( 
        [this, &layer](auto& d)
        {
            UpdateDataChunk(layer, d);
        },
        data
    );

    return data;
}

void Terrain::UpdateDataChunk(const ChunkLayer layer, DataFlatChunk& data)
{}

void Terrain::UpdateDataChunk(const ChunkLayer layer, DataFullChunk& data)
{}

void Terrain::UpdateDataChunk(const ChunkLayer layer, DataWaveChunk& data)
{
    data.maxBlockHeight = GetMaxHeightForLayer(layer);
}

void Terrain::UpdateDataChunk(const ChunkLayer layer, DataEditorChunk& data)
{}

void Terrain::UpdateDataChunk(const ChunkLayer layer, DataHeightmapChunk& data)
{
    const unsigned int maxHeight = GetMaxHeightForLayer(layer);
    
    m_generator.SetNoiseParams(data.noiseParams);

    if (data.spline.use)
        m_generator.GenerateHeightMapWithSpline(m_nrChunkWidth*CHUNK_SIZE, m_nrChunkDepth*CHUNK_SIZE, maxHeight, data.spline.plotX, data.spline.plotY);
    else
        m_generator.GenerateHeightMap(m_nrChunkWidth*CHUNK_SIZE, m_nrChunkDepth*CHUNK_SIZE, maxHeight);
    data.heightmap.values = stbi_load("../data/heightmap/terrain.png", &data.heightmap.width, &data.heightmap.depth, &data.heightmap.channel, 1);

    if (data.heightmap.width != m_nrChunkWidth*CHUNK_SIZE || data.heightmap.depth != m_nrChunkDepth*CHUNK_SIZE)
        throw std::runtime_error("The dimensions of the heightmap do not match with the size of the terrain");
}

void Terrain::UpdateDataChunk(const ChunkLayer layer, DataCheeseChunk& data)
{}

void Terrain::UpdateDataChunk(const ChunkLayer layer, DataCaveChunk& data) 
{
    const unsigned int maxHeight = GetMaxHeightForLayer(layer);

    m_generator.GenerateCaveHeightMap(m_nrChunkWidth*CHUNK_SIZE, m_nrChunkDepth*CHUNK_SIZE, maxHeight, 
        data.nrIteration, data.threshold, data.spline.plotX, data.spline.plotY);

    data.heightmap.values = stbi_load("../data/heightmap/cave.png", &data.heightmap.width, &data.heightmap.depth, &data.heightmap.channel, 1);

    if (data.heightmap.width != m_nrChunkWidth*CHUNK_SIZE || data.heightmap.depth != m_nrChunkDepth*CHUNK_SIZE)
        throw std::runtime_error("The dimensions of the cave heightmap do not match with the size of the terrain");
}

void Terrain::Create()
{
    m_chunks.clear();
    m_chunks.reserve(m_nrChunkWidth*m_nrChunkDepth*m_nrChunkHeight);
    const glm::ivec3 terrainSize = glm::ivec3(m_nrChunkWidth, m_nrChunkHeight, m_nrChunkDepth);
    const unsigned int nrChunkBelowSurface = m_nrChunkHeight-m_surfaceChunkHeight;
    
    for (unsigned int k = 0 ; k < m_nrChunkHeight ; k++) { // Y
        for (unsigned int j = 0 ; j < m_nrChunkDepth ; j++) { // Z
            for (unsigned int i = 0 ; i < m_nrChunkWidth ; i++) { // X
                const glm::ivec3 positionInTerrain = glm::ivec3(i, k, j);

                // It works because I only use two ChunkLayer
                glm::ivec3 positionInLayer = positionInTerrain;
                if (k >= nrChunkBelowSurface) // // Current Chunk belongs to the surface
                    positionInLayer -= glm::ivec3(0, nrChunkBelowSurface, 0);
                
                m_chunks.emplace_back("../shader/terrain/chunk.vs", "../shader/terrain/chunk.fs", positionInTerrain,
                    positionInLayer, terrainSize, positionInTerrain*CHUNK_SIZE);
            }
        }
    }

    for (Chunk& c : m_chunks) {
        const glm::ivec3 positionInTerrain = c.GetTerrainPosition();
        const unsigned int chunkIndexInGrid = GetChunkIndexInGrid(positionInTerrain);
        if (positionInTerrain.y > 0)
            c.SetChunkNeighbor(ChunkNeighbor::Bottom, &m_chunks[chunkIndexInGrid-m_nrChunkDepth*m_nrChunkWidth]);
        if (positionInTerrain.y < m_nrChunkHeight-1)
            c.SetChunkNeighbor(ChunkNeighbor::Top, &m_chunks[chunkIndexInGrid+m_nrChunkDepth*m_nrChunkWidth]);
        if (positionInTerrain.z > 0)
            c.SetChunkNeighbor(ChunkNeighbor::Back, &m_chunks[chunkIndexInGrid-m_nrChunkWidth]);
        if (positionInTerrain.z < m_nrChunkDepth-1)
            c.SetChunkNeighbor(ChunkNeighbor::Front, &m_chunks[chunkIndexInGrid+m_nrChunkWidth]);
        if (positionInTerrain.x > 0)
            c.SetChunkNeighbor(ChunkNeighbor::Left, &m_chunks[chunkIndexInGrid-1]);
        if (positionInTerrain.x < m_nrChunkWidth-1)
            c.SetChunkNeighbor(ChunkNeighbor::Right, &m_chunks[chunkIndexInGrid+1]);
    }

    for (auto& it : m_dataChunks) {
        const ChunkLayer layer = it.first;
        
        std::visit( 
            [this, &layer](auto& data)
            {
                UpdateDataChunk(layer, data);
            },
            it.second.second
        );
    }
    
    for (Chunk& c : m_chunks) {
        std::visit( 
            [&c](const auto& data)
            {
                c.Build(data);
            },
            c.GetTerrainPosition().y >= m_nrChunkHeight-m_surfaceChunkHeight ? // It works because I only use two ChunkLayer
                m_dataChunks[ChunkLayer::Surface].second :
                m_dataChunks[ChunkLayer::Below].second
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