#include "world/terrain/terrain.hpp"

#include <set>

#include "core/file/file.hpp"
#include "setting/logger.hpp"

void Terrain::SaveEditorChunkInFile(const std::string& name) const
{
    if (m_sizeChunk != glm::ivec3(1)) {
        Logger::GetInstance().AddLog(LogLevel::Level2, "Terrain::SaveEditorChunkInFile should only be used in Editor mode, when m_sizeChunk == 1x1x1");
        return;
    }

    const Chunk& editorChunk = m_chunks[0];

    // TODO : Should be in Chunk ?
    StructureData structData;
    for (const Voxel& v : editorChunk.GetVoxels()) {
        const glm::ivec3 bp = v.GetOrigin() - glm::vec3(CHUNK_SIZE/2);
        structData.blocks.emplace_back(v.GetBlockId(), bp);
    }

    m_fileReader.SaveStructureFile(name, structData);
}

void Terrain::BuildStructure(const glm::ivec3 bp, const StructureData& structData)
{   
    if (IsOutOfTerrain(bp)) { // TODO : Remove ? This should not happen, because the player should not be able to select a block outside the Terrain
        Logger::GetInstance().AddLog(LogLevel::Level2, "The selected voxel's position for building the structure is outside the terrain");
        return;
    }
    
    std::set<unsigned int> chunkLoaded; // Indices of the Chunks that will need to be Load()
    
    for (const BlockData& block : structData.blocks) {
        const glm::ivec3 worldPosition = bp + block.position;
        
        if (IsOutOfTerrain(worldPosition))
            continue;
    
        Chunk& c = GetChunkFromVoxel(worldPosition);
        if (c.IsEmptyAt(worldPosition%CHUNK_SIZE)) {
            c.CreateBlockWithoutLoad(worldPosition%CHUNK_SIZE, block.id);
            chunkLoaded.insert(GetChunkIndexInGrid(c.GetTerrainPosition()));
        }
    }
    
    for (unsigned int i : chunkLoaded)
        m_chunks[i].Load();
}