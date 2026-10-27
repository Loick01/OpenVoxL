#include "world/terrain/terrain.hpp"

#include "setting/logger.hpp"
#include "graphic/stb_image.h" // stbi_load

ChunkType Terrain::GetChunkType(const ChunkLayer layer) const
{
    return m_dataChunks.at(layer).first;
}

const DataChunk& Terrain::GetDataChunk(const ChunkLayer layer) const
{
    return m_dataChunks.at(layer).second;
}

void Terrain::SetChunkType(const ChunkLayer layer, const ChunkType type)
{
    m_dataChunks[layer] = {type, CreateDataChunk(layer, type)};
}

void Terrain::SetDataChunk(const ChunkLayer layer, const DataChunk data)
{
    m_dataChunks.at(layer) = {GetChunkType(layer), data};
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
            Logger::GetInstance().AddLog(LogLevel::Level1, "Unhandled ChunkType used in Terrain::CreateDataChunk");
            break;
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
        m_generator.GenerateHeightMapWithSpline(m_sizeChunk.x*CHUNK_SIZE, m_sizeChunk.z*CHUNK_SIZE, maxHeight, data.spline.plotX, data.spline.plotY);
    else
        m_generator.GenerateHeightMap(m_sizeChunk.x*CHUNK_SIZE, m_sizeChunk.z*CHUNK_SIZE, maxHeight);
    data.heightmap.values = stbi_load("../data/heightmap/terrain.png", &data.heightmap.width, &data.heightmap.depth, &data.heightmap.channel, 1);

    if (data.heightmap.width != m_sizeChunk.x*CHUNK_SIZE || data.heightmap.depth != m_sizeChunk.z*CHUNK_SIZE)
        Logger::GetInstance().AddLog(LogLevel::Level3, "The dimensions of the heightmap do not match with the size of the terrain");
}

void Terrain::UpdateDataChunk(const ChunkLayer layer, DataCheeseChunk& data)
{}

void Terrain::UpdateDataChunk(const ChunkLayer layer, DataCaveChunk& data) 
{
    const unsigned int maxHeight = GetMaxHeightForLayer(layer);

    m_generator.GenerateCaveHeightMap(m_sizeChunk.x*CHUNK_SIZE, m_sizeChunk.z*CHUNK_SIZE, maxHeight, 
        data.nrIteration, data.threshold, data.spline.plotX, data.spline.plotY);

    data.heightmap.values = stbi_load("../data/heightmap/cave.png", &data.heightmap.width, &data.heightmap.depth, &data.heightmap.channel, 1);

    if (data.heightmap.width != m_sizeChunk.x*CHUNK_SIZE || data.heightmap.depth != m_sizeChunk.z*CHUNK_SIZE)
        Logger::GetInstance().AddLog(LogLevel::Level3, "The dimensions of the cave heightmap do not match with the size of the terrain");
}