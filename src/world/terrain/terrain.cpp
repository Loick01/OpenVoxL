#include "world/terrain/terrain.hpp"

#include "setting/logger.hpp"

Terrain::Terrain(const FileReader& fileReader, const glm::ivec3 sizeChunk):
    m_sizeChunk(sizeChunk), m_surfaceChunkHeight(m_sizeChunk.y),
    m_generator(FREQUENCY, OCTAVES, SEED, FastNoise::SimplexFractal),
    m_fileReader(fileReader)
{
    SetChunkType(ChunkLayer::Surface, ChunkType::Heightmap);
    SetChunkType(ChunkLayer::Below, ChunkType::Cheese);

    Create();
    Load();
}

glm::ivec3 Terrain::GetSizeChunk() const
{
    return m_sizeChunk;
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
            return (m_sizeChunk.y-m_surfaceChunkHeight)*CHUNK_SIZE;
        default :   
            Logger::GetInstance().AddLog(LogLevel::Level1, "Unhandled ChunkLayer used in Terrain::GetMaxHeightForLayer"); // TODO : Should be LogLevel::Level2 ?
            return 0;
    }
}

unsigned int Terrain::GetChunkIndexInGrid(const glm::ivec3& chunkPosition) const
{
    return chunkPosition.y*m_sizeChunk.z*m_sizeChunk.x + chunkPosition.z*m_sizeChunk.x + chunkPosition.x;
}

bool Terrain::IsOutOfTerrain(const glm::ivec3& voxelPosition) const
{
    return voxelPosition.x < 0 || voxelPosition.y < 0 || voxelPosition.z < 0 ||
        voxelPosition.x >= m_sizeChunk.x*CHUNK_SIZE || voxelPosition.y >= m_sizeChunk.y*CHUNK_SIZE || voxelPosition.z >= m_sizeChunk.z*CHUNK_SIZE;
}

bool Terrain::IsSolidAt(const glm::ivec3& voxelPosition) const
{
    if (IsOutOfTerrain(voxelPosition))
        return false;
    
    const Chunk& c = GetChunkFromVoxel(voxelPosition);
    return !c.IsEmptyAt(voxelPosition%CHUNK_SIZE);
}

void Terrain::SetSizeChunk(const glm::ivec3 sizeChunk)
{
    m_sizeChunk = sizeChunk;
}

void Terrain::SetSurfaceChunkHeight(const unsigned int surfaceChunkHeight)
{
    m_surfaceChunkHeight = surfaceChunkHeight;
}

const Chunk& Terrain::GetChunkFromVoxel(const glm::ivec3& voxelPosition) const
{
    return m_chunks[voxelPosition.y/CHUNK_SIZE*m_sizeChunk.x*m_sizeChunk.z + voxelPosition.z/CHUNK_SIZE*m_sizeChunk.x + voxelPosition.x/CHUNK_SIZE];
} 

Chunk& Terrain::GetChunkFromVoxel(const glm::ivec3& voxelPosition)
{
    return m_chunks[voxelPosition.y/CHUNK_SIZE*m_sizeChunk.x*m_sizeChunk.z + voxelPosition.z/CHUNK_SIZE*m_sizeChunk.x + voxelPosition.x/CHUNK_SIZE];
} 

void Terrain::Create()
{
    m_chunks.clear();
    m_chunks.reserve(m_sizeChunk.x*m_sizeChunk.z*m_sizeChunk.y);
    const glm::ivec3 terrainSize = glm::ivec3(m_sizeChunk.x, m_sizeChunk.y, m_sizeChunk.z);
    const unsigned int nrChunkBelowSurface = m_sizeChunk.y-m_surfaceChunkHeight;
    
    for (unsigned int k = 0 ; k < m_sizeChunk.y ; k++) { // Y
        for (unsigned int j = 0 ; j < m_sizeChunk.z ; j++) { // Z
            for (unsigned int i = 0 ; i < m_sizeChunk.x ; i++) { // X
                const glm::ivec3 positionInTerrain = glm::ivec3(i, k, j);

                // It works because I only use two ChunkLayer
                glm::ivec3 positionInLayer = positionInTerrain;
                if (k >= nrChunkBelowSurface) // // Current Chunk belongs to the surface
                    positionInLayer -= glm::ivec3(0, nrChunkBelowSurface, 0);
                
                m_chunks.emplace_back("../shader/world/chunk.vs", "../shader/world/chunk.fs", positionInTerrain,
                    positionInLayer, terrainSize, positionInTerrain*CHUNK_SIZE);
            }
        }
    }

    for (Chunk& c : m_chunks) {
        const glm::ivec3 positionInTerrain = c.GetTerrainPosition();
        const unsigned int chunkIndexInGrid = GetChunkIndexInGrid(positionInTerrain);
        if (positionInTerrain.y > 0)
            c.SetChunkNeighbor(ChunkNeighbor::Bottom, &m_chunks[chunkIndexInGrid-m_sizeChunk.z*m_sizeChunk.x]);
        if (positionInTerrain.y < m_sizeChunk.y-1)
            c.SetChunkNeighbor(ChunkNeighbor::Top, &m_chunks[chunkIndexInGrid+m_sizeChunk.z*m_sizeChunk.x]);
        if (positionInTerrain.z > 0)
            c.SetChunkNeighbor(ChunkNeighbor::Back, &m_chunks[chunkIndexInGrid-m_sizeChunk.x]);
        if (positionInTerrain.z < m_sizeChunk.z-1)
            c.SetChunkNeighbor(ChunkNeighbor::Front, &m_chunks[chunkIndexInGrid+m_sizeChunk.x]);
        if (positionInTerrain.x > 0)
            c.SetChunkNeighbor(ChunkNeighbor::Left, &m_chunks[chunkIndexInGrid-1]);
        if (positionInTerrain.x < m_sizeChunk.x-1)
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
            c.GetTerrainPosition().y >= m_sizeChunk.y-m_surfaceChunkHeight ? // It works because I only use two ChunkLayer
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