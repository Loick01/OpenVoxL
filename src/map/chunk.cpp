#include "map/chunk.hpp"

#include "graphic/texture.hpp"

Chunk::Chunk(const std::string& vertexPath, const std::string& fragmentPath, 
const glm::ivec3 terrainPosition, const glm::vec3 originPosition, const unsigned int blockId):
    m_shader(vertexPath, fragmentPath), m_terrainPosition(terrainPosition), m_originPosition(originPosition), m_blockId(blockId)
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    glGenBuffers(1, &m_textureId_SBBO);
    
    BuildFullChunk();
    
    m_textureId = LoadTexture2D("../asset/texture/block/atlas.png");
}

Chunk::~Chunk()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteBuffers(1, &m_textureId_SBBO); 
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
}

void Chunk::BuildFaces()
{
    m_chunkFaces.clear();
    m_blockIds.clear(); // TODO : Will be removed when I use face textures instead of block textures

    for (Voxel& v : m_voxels) {
        BuildFace(v.GetFaceId(0), v.GetFacePtr(0));
        BuildFace(v.GetFaceId(1), v.GetFacePtr(1));
        BuildFace(v.GetFaceId(2), v.GetFacePtr(2));
        BuildFace(v.GetFaceId(3), v.GetFacePtr(3));
        BuildFace(v.GetFaceId(4), v.GetFacePtr(4));
        BuildFace(v.GetFaceId(5), v.GetFacePtr(5));
        m_blockIds.push_back(m_blockId);
    }
}

void Chunk::BuildFullChunk()
{
    m_voxels.clear();

    for (unsigned int k = 0 ; k < CHUNK_SIZE ; k++) {
        for (unsigned int j = 0 ; j < CHUNK_SIZE ; j++) {
            for (unsigned int i = 0 ; i < CHUNK_SIZE ; i++) {
                Voxel v(m_originPosition + glm::vec3(i, k, j), 0);
                m_voxels.push_back(v);
            }
        }
    }
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
