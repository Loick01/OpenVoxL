#include "graphic/skybox.hpp"

#include "graphic/texture.hpp"

Skybox::Skybox(const std::string& vertexPath, const std::string& fragmentPath):
    m_shader(vertexPath, fragmentPath)
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    Load();
    m_cubemapTextureId = LoadCubemapTexture(
        {
        "../asset/texture/skybox/px.png",
        "../asset/texture/skybox/nx.png",
        "../asset/texture/skybox/py.png",
        "../asset/texture/skybox/ny.png",
        "../asset/texture/skybox/pz.png",
        "../asset/texture/skybox/nz.png"
        }
    );
}

Skybox::~Skybox()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void Skybox::Load() 
{  
    m_vertices = {
        -1.f, -1.f, -1.f,
        1.f, -1.f, -1.f,
        1.f, 1.f, -1.f,
        -1.f, 1.f, -1.f,
        -1.f, -1.f, 1.f,
        1.f, -1.f, 1.f,
        1.f, 1.f, 1.f,
        -1.f, 1.f, 1.f,
    };

    m_indices = {
        0, 1, 2,
        0, 2, 3,
        4, 6, 5,
        4, 7, 6,
        0, 5, 1,
        0, 4, 5,
        7, 2, 6,
        7, 3, 2,
        0, 7, 4,
        0, 3, 7,
        5, 2, 1,
        5, 6, 2
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(m_VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()* sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);
}


void Skybox::Draw(const glm::mat4& projection, const glm::mat4& view) 
{
    glDepthFunc(GL_LEQUAL);
    m_shader.Use();
    m_shader.SetMat4("projection", projection);
    glm::mat4 skyboxView = glm::mat4(glm::mat3(view)); // Remove the translation of the camera
    m_shader.SetMat4("view", skyboxView);
    BindTexture2D(m_shader.GetLocation("skyboxTexture"), m_cubemapTextureId, 0);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0);
    glDepthFunc(GL_LESS);
}