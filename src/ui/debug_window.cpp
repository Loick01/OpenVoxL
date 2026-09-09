#include "ui/debug_window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/camera.hpp"
#include "map/terrain.hpp"

namespace
{
    const char* cameraStates[] = {"KeyFree", "MouseFree", "Orbital", "Player"};
    const char* chunkTypes[] = {"Full", "Flat", "Wave", "Editor", "Heightmap", "Cheese"};
    const char* noiseTypes[] = {"Value", "ValueFractal", "Perlin", "PerlinFractal", "Simplex", "SimplexFractal", "Cellular", "WhiteNoise", "Cubic", "CubicFractal"};
}

DebugWindow::DebugWindow(GLFWwindow* glfwWindow, Camera& camera, Terrain& terrain):
    m_glfwWindow(glfwWindow), m_width(500), m_height(500), m_camera(camera), m_terrain(terrain),
    m_indexCameraState((unsigned int)m_camera.GetState()), m_indexChunkType((unsigned int)m_terrain.GetChunkType()),
    m_indexNoiseType(5), m_wireframeRendering(false)
{
    InitImGui();
}

DebugWindow::~DebugWindow()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    ImPlot::DestroyContext();
}

void DebugWindow::InitImGui()
{
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_glfwWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImPlot::CreateContext();
}

bool DebugWindow::OpenSettings(DataFlatChunk& data)
{
    return false;
}

bool DebugWindow::OpenSettings(DataFullChunk& data)
{
    return false;
}

bool DebugWindow::OpenSettings(DataWaveChunk& data)
{
    if (ImGui::SliderFloat("Frequency", &data.frequency, 0.5f, 10.f))
        return true;
    return false;
}

bool DebugWindow::OpenSettings(DataEditorChunk& data)
{
    return false;
}

bool DebugWindow::OpenSettings(DataHeightmapChunk& data)
{
    if (ImGui::SliderInt("Octaves", &data.octaves, 0, 8))
        return true;
    if (ImGui::SliderInt("Seed", &data.seed, 0, 8))
        return true;
    
    bool hasChanged = false;
    if (ImGui::BeginCombo("Noise Type", noiseTypes[m_indexNoiseType])){ 
        for (unsigned int i = 0; i < IM_ARRAYSIZE(noiseTypes); i++){
            if (ImGui::Selectable(noiseTypes[i])) {
                m_indexNoiseType = i;
                data.noiseType = static_cast<FastNoise::NoiseType>(m_indexNoiseType);
                hasChanged = true;
            }
        }
        ImGui::EndCombo();
    }
    return hasChanged;
}

bool DebugWindow::OpenSettings(DataCheeseChunk& data)
{
    if (ImGui::SliderFloat("Frequency", &data.frequency, 0.5f, 10.f))
        return true;
    if (ImGui::SliderFloat("Threshold", &data.threshold, -1.f, 1.f))
        return true;
    return false;
}

void DebugWindow::Draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
    ImGui::Begin("Debug Window");

    if (ImGui::BeginTabBar("Tabs")){

        if (ImGui::BeginTabItem("General")){
            const float fps = ImGui::GetIO().Framerate;
            ImGui::Text("FPS: %.1f", fps);

            if (ImGui::Checkbox("Wireframe", &m_wireframeRendering))
                if (m_wireframeRendering)
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                else
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Camera")){
            const glm::vec3 cameraPosition = m_camera.GetPosition();
            float cameraSpeed = m_camera.GetSpeed();
            
            ImGui::Text("Camera Position: %.1f/%.1f/%.1f", cameraPosition.x, cameraPosition.y, cameraPosition.z);
            ImGui::SameLine();
            if (ImGui::Button("Reset"))
                m_camera.ResetPosition();
            
            if (ImGui::SliderFloat("Camera speed", &cameraSpeed, 1.f, 200.f))
                m_camera.SetSpeed(cameraSpeed);

            if (ImGui::BeginCombo("Camera State", cameraStates[m_indexCameraState])){ // TODO 
                for (unsigned int i = 0; i < IM_ARRAYSIZE(cameraStates); i++){
                    if (ImGui::Selectable(cameraStates[i])) {
                        m_indexCameraState = i;
                        m_camera.SetState(static_cast<CameraState>(m_indexCameraState));
                    }
                }
                ImGui::EndCombo();
            }
            
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Terrain")){
            glm::ivec3 terrainSize = m_terrain.GetSize();
            if (ImGui::SliderInt("Terrain Chunk Width", &terrainSize.x, 1, 10))
                m_terrain.SetSize(terrainSize);
            if (ImGui::SliderInt("Terrain Chunk Height", &terrainSize.y, 1, 10))
                m_terrain.SetSize(terrainSize);
            if (ImGui::SliderInt("Terrain Chunk Depth", &terrainSize.z, 1, 10))
                m_terrain.SetSize(terrainSize);

            if (ImGui::BeginCombo("Chunk Type", chunkTypes[m_indexChunkType])){ 
                for (unsigned int i = 0; i < IM_ARRAYSIZE(chunkTypes); i++){
                    if (ImGui::Selectable(chunkTypes[i])) {
                        m_indexChunkType = i;
                        m_terrain.SetChunkType(static_cast<ChunkType>(m_indexChunkType));
                    }
                }
                ImGui::EndCombo();
            }

            DataChunk dataChunk = m_terrain.GetDataChunk();
            std::visit( 
                [this](auto& dataChunk)
                {
                    if (OpenSettings(dataChunk))
                        m_terrain.SetDataChunk(dataChunk);
                },
                dataChunk
            );
            
            if (ImGui::Button("Load Terrain")) {
                m_terrain.Create();
                m_terrain.Load();
            }
            
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}