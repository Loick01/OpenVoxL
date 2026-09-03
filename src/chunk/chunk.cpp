#include "chunk/chunk.hpp"

Chunk::Chunk(const std::string& vertexPath, const std::string& fragmentPath, 
const glm::ivec3 terrainPosition, const glm::vec3 originPosition):
    m_shader(vertexPath, fragmentPath), m_terrainPosition(terrainPosition), m_originPosition(originPosition)
{
    BuildFullChunk();
}

Chunk::~Chunk()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
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

void Chunk::Load() 
{  
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(m_VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()* sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);
}


void Chunk::Draw(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) 
{
    m_shader.Use();
    m_shader.SetMat4("projection", projection);
    m_shader.SetMat4("view", view);
    m_shader.SetMat4("model", model);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0);
}
