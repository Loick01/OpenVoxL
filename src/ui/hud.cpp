#include "ui/hud.hpp"

Hud::Hud(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int windowWidth, const unsigned int windowHeight):
    Drawable(vertexPath, fragmentPath), m_show(true), m_hotbar(vertexPath, fragmentPath, windowWidth, 724.f)
{
    // Must respect the sizes from the texture
    m_hotbarWidth = 724.0f;
    float hotbarHeight = 84.0f;
    float selectorSize = 92.0f;
    float cursorSize = 36.0f;
    
    m_uiQuads.emplace_back(glm::vec2(windowWidth/2.-m_hotbarWidth/2. ,10.), m_hotbarWidth, hotbarHeight);
    m_uiQuads.emplace_back(glm::vec2(windowWidth/2.-m_hotbarWidth/2.-4., 6.), selectorSize, selectorSize); // By default, selector is on the first element in the hotbar
    m_uiQuads.emplace_back(glm::vec2(windowWidth/2.-cursorSize/2.0, windowHeight/2.-cursorSize/2.), cursorSize, cursorSize);
    m_uiQuads.emplace_back(glm::vec2(windowWidth/2.-m_hotbarWidth/2., 20.+hotbarHeight), m_hotbarWidth/2-10., 16.);
    m_uiQuads.emplace_back(glm::vec2(windowWidth/2.0+10., 20.+hotbarHeight), m_hotbarWidth/2-10., 16.);
    m_uiQuads.emplace_back(glm::vec2(windowWidth/2.-m_hotbarWidth/2.+4., 20.+hotbarHeight), m_hotbarWidth/2-16., 14.);
    m_uiQuads.emplace_back(glm::vec2(windowWidth/2.+12., 20.+hotbarHeight), m_hotbarWidth/2-16., 14.);
    
    Load();
    LoadTexture();

    m_shader.Use();
    m_shader.SetInt("windowWidth", windowWidth);
    m_shader.SetInt("windowHeight", windowHeight);
}

void Hud::LoadTexture()
{
    m_textureId = LoadTexture2D("../asset/texture/ui/hud.png");
}

void Hud::Load()
{   
    unsigned int indexOffset = 0;
    m_countIndex = 0;
    
    for (unsigned int i = 0 ; i < m_uiQuads.size() ; i++){
        for (unsigned int k = 0 ; k < 4 ; k++){
            m_vertices.push_back(m_uiQuads[i].vertices[k]);
        }
        for (unsigned int k = 0 ; k < 6 ; k++){
            m_indices.push_back(indexOffset + m_uiQuads[i].indices[k]);
            m_countIndex++;
        }
        indexOffset += 4;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(glm::vec3), m_vertices.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(m_VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()* sizeof(unsigned int), m_indices.data() , GL_STATIC_DRAW);
}

void Hud::Draw() const
{
    if (!m_show)
        return;
    
    glDisable(GL_DEPTH_TEST);
    m_shader.Use();
    BindTexture2D(m_shader.GetLocation("hudTexture"), m_textureId, 0);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_countIndex, GL_UNSIGNED_INT, (void*)0);
    glEnable(GL_DEPTH_TEST);

    m_hotbar.Draw();
}

void Hud::KeyCallback(const std::array<bool,GLFW_KEY_LAST+1>& keys)
{
    if (keys[GLFW_KEY_Q])
        m_show = !m_show;
}

void Hud::UpdateHealth(const float health)
{}

void Hud::UpdateStamina(const float stamina)
{}