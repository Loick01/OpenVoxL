#include "graphic/skybox.hpp"

#include "graphic/stb_image.h"

Skybox::Skybox(const std::string& vertexPath, const std::string& fragmentPath):
    m_shader(vertexPath, fragmentPath)
{
    CreateBuffers();
    m_pathTextures = {
        "../asset/texture/skybox/px.png",
        "../asset/texture/skybox/nx.png",
        "../asset/texture/skybox/py.png",
        "../asset/texture/skybox/ny.png",
        "../asset/texture/skybox/pz.png",
        "../asset/texture/skybox/nz.png"
    };
    LoadCubemap();
}

Skybox::~Skybox(){
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void Skybox::CreateBuffers() {  
    m_vertices = {
        // Back face
        -1.f, -1.f, -1.f,
        1.f, -1.f, -1.f,
        1.f, 1.f, -1.f,
        -1.f, 1.f, -1.f,
        // Front face
        -1.f, -1.f, 1.f,
        1.f, -1.f, 1.f,
        1.f, 1.f, 1.f,
        -1.f, 1.f, 1.f,
        // Bottom face
        -1.f, -1.f, -1.f,
        1.f, -1.f, -1.f,
        1.f, -1.f, 1.f,
        -1.f, -1.f, 1.f,
        // Top face
        -1.f, 1.f, -1.f,
        1.f, 1.f, -1.f,
        1.f, 1.f, 1.f,
        -1.f, 1.f, 1.f,
        // Left face
        -1.f, -1.f, -1.f,
        -1.f, -1.f, 1.f,
        -1.f, 1.f, 1.f,
        -1.f, 1.f, -1.f,
        // Right face
        1.f, -1.f, 1.f,
        1.f, -1.f, -1.f,
        1.f, 1.f, -1.f,
        1.f, 1.f, 1.f
    };

    m_indices = {
        1, 0, 2,
        0, 3, 2,
        4, 5, 6,
        4, 6, 7,
        8, 9, 10,
        8, 10, 11,
        12, 14, 13,
        12, 15, 14,
        16, 17, 18,
        16, 18, 19,
        20, 21, 22,
        20, 22, 23
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(m_VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()* sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);
}


void Skybox::Draw(const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view) {
    m_shader.Use();
    m_shader.SetMat4("model", model);
    m_shader.SetMat4("projection", projection);
    m_shader.SetMat4("view", view);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0);
}

void Skybox::LoadCubemap(){
    glGenTextures(1, &m_cubemapTextureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTextureId);

    int width, height, nrChannels;
    unsigned char* data;
    for (unsigned int i = 0; i < 6; i++)
    {
        data = stbi_load(m_pathTextures[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            stbi_image_free(data);
            throw std::runtime_error("Cubemap texture failed to load at path : " + m_pathTextures[i]);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::BindCubemap(int unit){
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTextureId);
    m_shader.SetInt("skyboxTexture", unit);
}