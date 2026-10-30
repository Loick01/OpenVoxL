#include "world/visualization.hpp"

// TODO : Rename this file

Visualization::Visualization(const std::string& vertexPath, const std::string& fragmentPath):
    Drawable3D(vertexPath, fragmentPath)
{
    glGenBuffers(1, &m_blockId_SSBO);
    glGenBuffers(1, &m_faceOrientation_SSBO);
    
    LoadTexture("../asset/texture/block/atlas.png");

    // TODO : Remove
    for (unsigned int k = 0 ; k < 2 ; k++) // Y
        AddVoxel(glm::ivec3(0, k, 0), rand()%30);
}

Visualization::~Visualization()
{}

void Visualization::AddVoxel(const glm::ivec3 blockPosition, const unsigned int blockId)
{
    m_voxels.emplace_back(glm::vec3(blockPosition), blockId);
}

void Visualization::AddFaceIndices(const unsigned int offset)
{
    m_indices.push_back(offset + 2);
    m_indices.push_back(offset + 0);
    m_indices.push_back(offset + 3);
    m_indices.push_back(offset + 3);
    m_indices.push_back(offset + 0);
    m_indices.push_back(offset + 1);
}

void Visualization::AddFace(const std::string& faceId, Face* face)
{
    m_chunkFaces[faceId] = face;
}

void Visualization::BuildFaces() 
{
    m_chunkFaces.clear();

    for (Voxel& v : m_voxels) {
        AddFace(v.GetFaceId(0), v.GetFacePtr(0));
        AddFace(v.GetFaceId(1), v.GetFacePtr(1));
        AddFace(v.GetFaceId(2), v.GetFacePtr(2));
        AddFace(v.GetFaceId(3), v.GetFacePtr(3));
        AddFace(v.GetFaceId(4), v.GetFacePtr(4));
        AddFace(v.GetFaceId(5), v.GetFacePtr(5));
    }
}

void Visualization::VoxelComputeData()
{
    m_vertices.clear();
    m_indices.clear();
    // m_blockIds.clear();
    // m_faceOrientations.clear();

    // unsigned int offset = 0;
    // for(std::map<std::string, Face*>::iterator it = m_chunkFaces.begin(); it != m_chunkFaces.end(); ++it) {
    //     Face* face = it->second;
    //     const std::vector<glm::vec3>& faceVertices = face->GetVertices();
    //     m_vertices.insert(m_vertices.end(), faceVertices.begin(), faceVertices.end());
    //     m_blockIds.push_back(face->GetBlockId());
    //     m_faceOrientations.push_back(face->GetOrientation());

    //     AddFaceIndices(offset);
    //     offset += 4;
    // }
}

void Visualization::VoxelBufferData()
{
    // glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    // glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), m_vertices.data(), GL_DYNAMIC_DRAW);

    // glBindVertexArray(m_VAO);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()* sizeof(unsigned int), m_indices.data(), GL_DYNAMIC_DRAW);

    // glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_blockId_SSBO);
    // glBufferData(GL_SHADER_STORAGE_BUFFER, m_blockIds.size()*sizeof(unsigned int), m_blockIds.data(), GL_DYNAMIC_DRAW);
    // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_blockId_SSBO);

    // glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_faceOrientation_SSBO);
    // glBufferData(GL_SHADER_STORAGE_BUFFER, m_faceOrientations.size()*sizeof(unsigned int), m_faceOrientations.data(), GL_DYNAMIC_DRAW);
    // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_faceOrientation_SSBO);
}

void Visualization::Load()
{
    BuildFaces();
    // VoxelComputeData();
    // VoxelBufferData();
}

void Visualization::Draw(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) const
{
    // m_shader.Use();
    // m_shader.SetMat4("projection", projection);
    // m_shader.SetMat4("view", view);
    // m_shader.SetMat4("model", model);
    // BindTexture2D(m_shader.GetLocation("atlas"), m_textureId, 0);
    // glBindVertexArray(m_VAO);
    // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_blockId_SSBO);
    // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_faceOrientation_SSBO);
    // glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0);
}