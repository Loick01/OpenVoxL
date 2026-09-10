#include "ui/debug_window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/camera.hpp"
#include "map/terrain.hpp"

namespace
{
    const char* cameraStates[] = {"KeyFree", "MouseFree", "Orbital"}; //, "Player"};
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
    bool needUpdate = false;
    if (ImGui::SliderFloat("Frequency", &data.frequency, 0.5f, 10.f))
        needUpdate = true;
    if (ImGui::SliderInt("Octaves", &data.octaves, 0, 8))
        needUpdate = true;
    if (ImGui::SliderInt("Seed", &data.seed, 0, 8))
        needUpdate = true;
    
    if (ImGui::BeginCombo("Noise Type", noiseTypes[m_indexNoiseType])){ 
        for (unsigned int i = 0; i < IM_ARRAYSIZE(noiseTypes); i++){
            if (ImGui::Selectable(noiseTypes[i])) {
                m_indexNoiseType = i;
                data.noiseType = static_cast<FastNoise::NoiseType>(m_indexNoiseType);
                needUpdate = true;
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::Checkbox("Use spline", &data.useSpline))
        needUpdate = true;

    if (data.useSpline)
        needUpdate |= OpenSplinePlot(data.plotX, data.plotY);
    
    return needUpdate;
}

bool DebugWindow::OpenSettings(DataCheeseChunk& data)
{
    bool needUpdate = false;
    if (ImGui::SliderFloat("Frequency", &data.frequency, 0.5f, 10.f))
        needUpdate = true;
    if (ImGui::SliderFloat("Threshold", &data.threshold, -1.f, 1.f))
        needUpdate = true;
    return needUpdate;
}

bool DebugWindow::OpenSplinePlot(std::vector<float>& plotX, std::vector<float>& plotY)
{
    bool needUpdate = false;

    if (ImPlot::BeginPlot("Surface height spline")) {
        ImPlot::SetupAxis(ImAxis_X1, "Noise value", ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax);
        ImPlot::SetupAxis(ImAxis_Y1, "Height", ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax);
        ImPlot::SetupAxisLimits(ImAxis_X1, 0., 1.);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0., 1.);

        if (!plotX.empty() && !plotY.empty()) { // Should test plotX.size() != plotY.size() ?
            ImPlot::PlotScatter("Points", plotX.data(), plotY.data(), plotX.size());
            ImPlot::PlotLine("Points", plotX.data(), plotY.data(), plotX.size());
        }
        if (ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            ImPlotPoint mouse_pos = ImPlot::GetPlotMousePos();
            float x = mouse_pos.x;
            float y = mouse_pos.y;
            if (plotX.size() != 0){
                for (unsigned int i = 0 ; i < plotX.size() ; i++){
                    if (plotX[i] > x){
                        plotX.insert(plotX.begin()+i, x);
                        plotY.insert(plotY.begin()+i, y);
                        break;
                    }else if (i == plotX.size()-1){
                        plotX.push_back(x);
                        plotY.push_back(y);
                        break;
                    }
                }
            }else{
                plotX.push_back(x);
                plotY.push_back(y);
            }
            needUpdate = true;
        }

        ImGui::SetNextItemWidth(150.0f);
        if (ImGui::SliderFloat("First", &plotY.front(), 0.f, 1.f))
            needUpdate = true;
        ImGui::SameLine();
        ImGui::SetNextItemWidth(150.0f);
        if (ImGui::SliderFloat("Last", &plotY.back(), 0.f, 1.f))
            needUpdate = true;
        ImGui::SameLine();
        if (ImGui::Button("Clean")) {
            // data.spline.Reset(); // TODO : Add Spline struct
            plotX = {0.f, 1.f}; // Remove
            plotY = {0.f, 1.f}; // Remove
            needUpdate = true;
        }

        ImPlot::EndPlot();
    }
    
    return needUpdate;
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
            m_indexCameraState = (unsigned int)m_camera.GetState();

            ImGui::Text("Camera Position: %.1f/%.1f/%.1f", cameraPosition.x, cameraPosition.y, cameraPosition.z);
            ImGui::SameLine();
            if (ImGui::Button("Reset"))
                m_camera.ResetPosition();
            
            if (ImGui::SliderFloat("Camera speed", &cameraSpeed, 1.f, 200.f))
                m_camera.SetSpeed(cameraSpeed);

            if (ImGui::BeginCombo("Camera State", cameraStates[m_indexCameraState])) {
                for (unsigned int i = 0; i < IM_ARRAYSIZE(cameraStates); i++){
                    if (ImGui::Selectable(cameraStates[i]))
                        m_camera.SetState(static_cast<CameraState>(i));
                }
                ImGui::EndCombo();
            }
            
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Terrain")){
            glm::ivec3 terrainSize = m_terrain.GetSize();
            int surfaceChunkHeight = m_terrain.GetSurfaceChunkHeight();

            if (ImGui::SliderInt("Terrain Chunk Width", &terrainSize.x, 1, 10))
                m_terrain.SetSize(terrainSize);
            if (ImGui::SliderInt("Terrain Chunk Height", &terrainSize.y, 1, 10))
                m_terrain.SetSize(terrainSize);
            if (ImGui::SliderInt("Terrain Chunk Depth", &terrainSize.z, 1, 10))
                m_terrain.SetSize(terrainSize);
            if (ImGui::SliderInt("Surface Chunk Height", &surfaceChunkHeight, 0, terrainSize.y))
                m_terrain.SetSurfaceChunkHeight(surfaceChunkHeight);
            
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
                m_camera.SetTargetTerrain(glm::vec3(m_terrain.GetSize()*CHUNK_SIZE)/2.f);
            }
            
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}