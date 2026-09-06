#include "map/chunk.hpp"

#include "core/FastNoise.h" // Used in BuildCheeseChunk. Remove ?
#include "graphic/texture.hpp"

Chunk::Chunk(const std::string& vertexPath, const std::string& fragmentPath, const glm::ivec3 terrainPosition,
const glm::ivec3 terrainSize, const glm::vec3 originPosition, const unsigned int blockId):
    m_shader(vertexPath, fragmentPath), m_terrainPosition(terrainPosition), m_terrainSize(terrainSize), m_originPosition(originPosition), m_blockId(blockId)
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    glGenBuffers(1, &m_textureId_SBBO);
    
    m_textureId = LoadTexture2D("../asset/texture/block/atlas.png");
    m_gridVoxel.resize(CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE);
}

Chunk::~Chunk()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteBuffers(1, &m_textureId_SBBO); 
}

const Chunk* Chunk::GetChunkNeighbor(const ChunkNeighbor which) const
{
    std::map<ChunkNeighbor, const Chunk*>::const_iterator it = m_chunkNeighbors.find(which);
    if (it == m_chunkNeighbors.end())
        return nullptr;
    return it->second;
}

void Chunk::SetChunkNeighbor(const ChunkNeighbor which, const Chunk* neighbor)
{
    m_chunkNeighbors[which] = neighbor;
}

glm::ivec3 Chunk::GetTerrainPosition() const
{
    return m_terrainPosition;
}

bool Chunk::IsEmptyAt(const unsigned voxelIndex) const
{
    return m_gridVoxel[voxelIndex] == nullptr;
}

unsigned int Chunk::GetBlockIndexInGrid(const glm::ivec3& blockPosition) const
{
    return blockPosition.y*CHUNK_SIZE*CHUNK_SIZE + blockPosition.z*CHUNK_SIZE + blockPosition.x;
}

void Chunk::AddVoxel(const glm::vec3 blockPosition, const unsigned int blockId)
{
    m_voxels.emplace_back(m_originPosition + blockPosition, blockId);
    m_gridVoxel[GetBlockIndexInGrid(blockPosition)] = &m_voxels.back();
}

void Chunk::AddFaceIndices(const unsigned int offset)
{
    m_indices.push_back(offset + 2);
    m_indices.push_back(offset + 0);
    m_indices.push_back(offset + 3);
    m_indices.push_back(offset + 3);
    m_indices.push_back(offset + 0);
    m_indices.push_back(offset + 1);
}

void Chunk::BuildFace(const std::string& faceId, Face* face)
{
    m_chunkFaces[faceId] = face;
    // TODO : Use texture from face instead from block (+ I can't even use Voxel::m_blockId)
    m_blockIds.push_back(m_blockId); // One blockId for each face
}

void Chunk::BuildFaces()
{
    m_chunkFaces.clear();
    m_blockIds.clear(); // TODO : Will be removed when I use face textures instead of block textures

    for (Voxel& v : m_voxels) {
        const glm::ivec3 blockPosition = v.GetOrigin() - m_originPosition;
        const int blockIndexInGrid = GetBlockIndexInGrid(blockPosition); // Do not use unsigned int

        if (blockPosition.y == 0) {
            if (m_terrainPosition.y == 0)
                BuildFace(v.GetFaceId(0), v.GetFacePtr(0));
            else if (GetChunkNeighbor(ChunkNeighbor::Bottom)->IsEmptyAt(blockIndexInGrid+CHUNK_SIZE*CHUNK_SIZE*(CHUNK_SIZE-1)))
                BuildFace(v.GetFaceId(0), v.GetFacePtr(0));
        } else if (m_gridVoxel[blockIndexInGrid-CHUNK_SIZE*CHUNK_SIZE] == nullptr)
            BuildFace(v.GetFaceId(0), v.GetFacePtr(0));

        if (blockPosition.y == CHUNK_SIZE-1) {
            if (m_terrainPosition.y == m_terrainSize.y-1)
                BuildFace(v.GetFaceId(1), v.GetFacePtr(1));
            else if (GetChunkNeighbor(ChunkNeighbor::Top)->IsEmptyAt(blockIndexInGrid-CHUNK_SIZE*CHUNK_SIZE*(CHUNK_SIZE-1)))
                BuildFace(v.GetFaceId(1), v.GetFacePtr(1));
        } else if (m_gridVoxel[blockIndexInGrid+CHUNK_SIZE*CHUNK_SIZE] == nullptr)
            BuildFace(v.GetFaceId(1), v.GetFacePtr(1));

        if (blockPosition.z == 0) {
            if (m_terrainPosition.z == 0)
                BuildFace(v.GetFaceId(2), v.GetFacePtr(2));
            else if (GetChunkNeighbor(ChunkNeighbor::Back)->IsEmptyAt(blockIndexInGrid+CHUNK_SIZE*(CHUNK_SIZE-1)))
                BuildFace(v.GetFaceId(2), v.GetFacePtr(2));
        } else if (m_gridVoxel[blockIndexInGrid-CHUNK_SIZE] == nullptr)
            BuildFace(v.GetFaceId(2), v.GetFacePtr(2));

        if (blockPosition.z == CHUNK_SIZE-1) {
            if (m_terrainPosition.z == m_terrainSize.z-1)
                BuildFace(v.GetFaceId(3), v.GetFacePtr(3));
            else if (GetChunkNeighbor(ChunkNeighbor::Front)->IsEmptyAt(blockIndexInGrid-CHUNK_SIZE*(CHUNK_SIZE-1)))
                BuildFace(v.GetFaceId(3), v.GetFacePtr(3));
        } else if (m_gridVoxel[blockIndexInGrid+CHUNK_SIZE] == nullptr)
            BuildFace(v.GetFaceId(3), v.GetFacePtr(3));

        if (blockPosition.x == 0) {
            if (m_terrainPosition.x == 0)
                BuildFace(v.GetFaceId(4), v.GetFacePtr(4));
            else if (GetChunkNeighbor(ChunkNeighbor::Left)->IsEmptyAt(blockIndexInGrid+CHUNK_SIZE-1))
                BuildFace(v.GetFaceId(4), v.GetFacePtr(4));
        } else if (m_gridVoxel[blockIndexInGrid-1] == nullptr)
            BuildFace(v.GetFaceId(4), v.GetFacePtr(4));

        if (blockPosition.x == CHUNK_SIZE-1) {
            if (m_terrainPosition.x == m_terrainSize.x-1)
                BuildFace(v.GetFaceId(5), v.GetFacePtr(5));
            else if (GetChunkNeighbor(ChunkNeighbor::Right)->IsEmptyAt(blockIndexInGrid-CHUNK_SIZE+1))
                BuildFace(v.GetFaceId(5), v.GetFacePtr(5));
        } else if (m_gridVoxel[blockIndexInGrid+1] == nullptr)
            BuildFace(v.GetFaceId(5), v.GetFacePtr(5));
    }
}

void Chunk::BuildFullChunk()
{
    m_voxels.clear();

    for (unsigned int k = 0 ; k < CHUNK_SIZE ; k++) { // Y
        for (unsigned int j = 0 ; j < CHUNK_SIZE ; j++) { // Z
            for (unsigned int i = 0 ; i < CHUNK_SIZE ; i++) { // X
                AddVoxel(glm::vec3(i, k, j), m_blockId);
            }
        }
    }
}

void Chunk::BuildFlatChunk()
{
    m_voxels.clear();

    if (m_terrainPosition.y == 0) {
        for (unsigned int j = 0 ; j < CHUNK_SIZE ; j++) { // Z
            for (unsigned int i = 0 ; i < CHUNK_SIZE ; i++) { // X
                AddVoxel(glm::vec3(i, 0.f, j), m_blockId);
            }
        }
    }
}

void Chunk::BuildWaveChunk(const float frequency, const unsigned int maxBlockHeight)
{
    m_voxels.clear();

    for (unsigned int k = 0 ; k < CHUNK_SIZE ; k++) { // Y
        for (unsigned int j = 0 ; j < CHUNK_SIZE ; j++) { // Z
            const float t = (float)j/CHUNK_SIZE+m_terrainPosition.z-1;

            for (unsigned int i = 0 ; i < CHUNK_SIZE ; i++) { // X
                const float s = (float)i/CHUNK_SIZE+m_terrainPosition.x-1; // (i+m_terrainPosition.x*CHUNK_SIZE)/CHUNK_SIZE = i/CHUNK_SIZE+m_terrainPosition.x
                float v = (std::sin(frequency*(s+t))+1.0f)*0.5f; // [0, 1s]
                const unsigned int maxHeight = v*(maxBlockHeight-1);
                const unsigned int blockHeightPosition = m_originPosition.y + k;

                if (blockHeightPosition <= maxHeight)
                    AddVoxel(glm::vec3(i, k, j), m_blockId);
            }
        }
    }
}

void Chunk::BuildHeightmapChunk(const unsigned char* heightmap, const unsigned int heightmapWidth, const unsigned int heightmapDepth)
{
    m_voxels.clear();

    for (unsigned int k = 0 ; k < CHUNK_SIZE ; k++) { // Y
        for (unsigned int j = 0 ; j < CHUNK_SIZE ; j++) { // Z
            for (unsigned int i = 0 ; i < CHUNK_SIZE ; i++) { // X
                const unsigned int hmIndex = m_originPosition.z*heightmapWidth  + m_originPosition.x + j*heightmapWidth + i; 
                const unsigned int blockHeightPosition = m_originPosition.y + k;
                if (blockHeightPosition <= heightmap[hmIndex])
                    AddVoxel(glm::vec3(i, k, j), m_blockId);
            }
        }
    }
}

void Chunk::BuildCheeseChunk(const FastNoise& noise, const float frequency)
{
    m_voxels.clear();

    for (unsigned int k = 0 ; k < CHUNK_SIZE ; k++) { // Y
        for (unsigned int j = 0 ; j < CHUNK_SIZE ; j++) { // Z
            for (unsigned int i = 0 ; i < CHUNK_SIZE ; i++) { // X
                const float density = noise.GetNoise(frequency*(m_originPosition.x + i), frequency*(m_originPosition.y + k), frequency*(m_originPosition.z + j));
                if (density > 0.f)
                    AddVoxel(glm::vec3(i, k, j), m_blockId);
            }
        }
    }

}

void Chunk::BuildEditorChunk()
{
    const glm::vec3 centerPosition = glm::vec3(CHUNK_SIZE/2);
    AddVoxel(centerPosition, m_blockId);
}

void Chunk::VoxelComputeData()
{
    m_vertices.clear();
    m_indices.clear();
    // m_blockIds.clear();

    unsigned int offset = 0;
    for(std::map<std::string, Face*>::iterator it = m_chunkFaces.begin(); it != m_chunkFaces.end(); ++it) {
        Face* face = it->second;
        const std::vector<glm::vec3>& faceVertices = face->GetVertices();
        m_vertices.insert(m_vertices.end(), faceVertices.begin(), faceVertices.end());

        AddFaceIndices(offset);
        offset += 4;
    }
}

void Chunk::VoxelBufferData()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), m_vertices.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(m_VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()* sizeof(unsigned int), m_indices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_textureId_SBBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, m_blockIds.size()*sizeof(unsigned int), m_blockIds.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_textureId_SBBO);
}

void Chunk::Load() 
{  
    BuildFaces();
    VoxelComputeData();
    VoxelBufferData();
}

void Chunk::Draw(const glm::mat4& projection, const glm::mat4& view) const
{
    m_shader.Use();
    m_shader.SetMat4("projection", projection);
    m_shader.SetMat4("view", view);
    BindTexture2D(m_shader.GetLocation("atlas"), m_textureId, 0);
    glBindVertexArray(m_VAO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_textureId_SBBO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0);
}
