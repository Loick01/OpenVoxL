#include "ui/hud.hpp"

#include "graphic/texture.hpp"

Hud::Hud(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int windowWidth, const unsigned int windowHeight):
    m_windowWidth(windowWidth), m_windowHeight(windowHeight), m_shader(vertexPath, fragmentPath)
{
    // Must respect the sizes from the used texture
    m_hotbarWidth = 724.0f;
    float hotbarHeight = 84.0f;
    float selectorSize = 92.0f;
    float cursorSize = 36.0f;

    m_uiQuads.push_back(CreateQuad(glm::vec2(m_windowWidth/2.-m_hotbarWidth/2. ,10.), m_hotbarWidth, hotbarHeight));
    m_uiQuads.push_back(CreateQuad(glm::vec2(m_windowWidth/2.-m_hotbarWidth/2.-4., 6.), selectorSize, selectorSize)); // By default, selector is on the first element in the hotbar
    m_uiQuads.push_back(CreateQuad(glm::vec2(m_windowWidth/2.-cursorSize/2.0, m_windowHeight/2.-cursorSize/2.), cursorSize, cursorSize));
    m_uiQuads.push_back(CreateQuad(glm::vec2(m_windowWidth/2.-m_hotbarWidth/2., 20.+hotbarHeight), m_hotbarWidth/2-10., 16.));
    m_uiQuads.push_back(CreateQuad(glm::vec2(m_windowWidth/2.0+10., 20.+hotbarHeight), m_hotbarWidth/2-10., 16.));
    m_uiQuads.push_back(CreateQuad(glm::vec2(m_windowWidth/2.-m_hotbarWidth/2.+4., 20.+hotbarHeight), m_hotbarWidth/2-16., 14.));
    m_uiQuads.push_back(CreateQuad(glm::vec2(m_windowWidth/2.+12., 20.+hotbarHeight), m_hotbarWidth/2-16., 14.));

    // for (unsigned int i = 0 ; i < 9 ; i++){
    //     Quad q = CreateQuad(glm::vec2(m_windowWidth/2.-m_hotbarWidth/2.+80.*i+8., 18.), 68., 68.);
    //     m_uiQuads.push_back(q);
    // }

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    Load();
    m_textureId = LoadTexture2D("../asset/texture/ui/hud.png");
    m_shader.SetInt("windowWidth", m_windowWidth);
    m_shader.SetInt("windowHeight", m_windowHeight);
}

Hud::~Hud()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

Quad Hud::CreateQuad(const glm::vec2 origin, const float width, const float height)
{
    std::vector<glm::vec2> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(origin);
    vertices.push_back(glm::vec2(origin[0]+width,origin[1]));
    vertices.push_back(glm::vec2(origin[0],origin[1]+height));
    vertices.push_back(glm::vec2(origin[0]+width,origin[1]+height));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(3);

    Quad q{vertices, indices};
    return q;
}

void Hud::Load()
{
    std::vector<glm::vec2> vertices;
    std::vector<unsigned int> indices;
    
    unsigned int indexOffset = 0;
    m_countIndex = 0;
    
    for (unsigned int i = 0 ; i < m_uiQuads.size() ; i++){
        for (unsigned int k = 0 ; k < 4 ; k++){
            vertices.push_back(m_uiQuads[i].vertices[k]);
        }
        for (unsigned int k = 0 ; k < 6 ; k++){
            indices.push_back(indexOffset + m_uiQuads[i].indices[k]);
            m_countIndex++;
        }
        indexOffset += 4;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec2), vertices.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(m_VAO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()* sizeof(unsigned int), indices.data() , GL_STATIC_DRAW);
}

void Hud::Draw()
{
    glDisable(GL_DEPTH_TEST);
    m_shader.Use();
    BindTexture2D(m_shader.GetLocation("hudTexture"), m_textureId, 0);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_countIndex, GL_UNSIGNED_INT, (void*)0);
    glEnable(GL_DEPTH_TEST);
}

void Hud::UpdateHealth(const float health)
{}

void Hud::UpdateStamina(const float stamina)
{}