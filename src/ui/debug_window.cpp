#include "ui/debug_window.hpp"

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/camera.hpp"
#include "graphic/texture.hpp"
#include "terrain/terrain.hpp"

namespace
{
    const char* cameraStates[] = {"KeyFree", "MouseFree", "Orbital"}; //, "Player"};
    const char* chunkTypes[] = {"Full", "Flat", "Wave", "Editor", "Heightmap", "Cheese", "Cave"};

    const char* noiseTypes[] = {"Value", "ValueFractal", "Perlin", "PerlinFractal", "Simplex", "SimplexFractal", "Cellular", "WhiteNoise", "Cubic", "CubicFractal"};
    const char* interps[] = {"Linear", "Hermite", "Quintic"};
    const char* fractalTypes[] = {"FBM", "Billow", "RigidMulti"};
    const char* cellularDistanceFunctions[] = {"Euclidean", "Manhattan", "Natural"};
    const char* cellularReturnTypes[] = {"CellValue", "NoiseLookup", "Distance", "Distance2", "Distance2Add", "Distance2Sub", "Distance2Mul", "Distance2Div"};
}

DebugWindow::DebugWindow(GLFWwindow* glfwWindow, Camera& camera, Terrain& terrain):
    m_glfwWindow(glfwWindow), m_width(500), m_height(500), m_camera(camera), m_terrain(terrain),
    m_indexCameraState((unsigned int)m_camera.GetState()), 
    m_indexSurfaceChunkType((unsigned int)m_terrain.GetChunkType(ChunkLayer::Surface)), 
    m_indexBelowChunkType((unsigned int)m_terrain.GetChunkType(ChunkLayer::Below)),
    m_indexNoiseTypeForHeightmap(5), m_indexInterpForHeightmap(0), m_indexFractalTypeForHeightmap(0), m_indexCellularDistanceFunctionForHeightmap(0), m_indexCellularReturnTypeForHeightmap(0), // TODO : Remove
    m_indexNoiseTypeForCheese(5), m_indexInterpForCheese(0), m_indexFractalTypeForCheese(0), m_indexCellularDistanceFunctionForCheese(0), m_indexCellularReturnTypeForCheese(0), // TODO : Remove
    m_wireframeRendering(false), m_showMainDebug(true), m_showHeightmapDebug(false)
{
    InitImGui();
    m_heightmapTextureId = LoadTexture2D("../data/heightmap/terrain.png");
    // m_caveHeightmapTextureId = LoadTexture2D("../data/heightmap/cave.png");
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

bool DebugWindow::OpenNoiseSettings(NoiseParameters& noiseParams, const std::string& labelSuffix, 
    unsigned int& indexNoiseType, unsigned int& indexInterp, unsigned int& indexFractalType, unsigned int& indexCellularDistanceFunction, unsigned int& indexCellularReturnType)
{
    bool needUpdate = false;
    if (ImGui::SliderFloat(("Frequency##"+labelSuffix).c_str(), &noiseParams.frequency, 0.5f, 10.f))
        needUpdate = true;
    if (ImGui::SliderInt(("Octaves##"+labelSuffix).c_str(), &noiseParams.octaves, 0, 8))
        needUpdate = true;
    if (ImGui::SliderInt(("Seed##"+labelSuffix).c_str(), &noiseParams.seed, 0, 255))
        needUpdate = true;
    
    if (ImGui::BeginCombo(("Noise Type##"+labelSuffix).c_str(), noiseTypes[indexNoiseType])){ 
        for (unsigned int i = 0; i < IM_ARRAYSIZE(noiseTypes); i++){
            if (ImGui::Selectable(noiseTypes[i])) {
                indexNoiseType = i;
                noiseParams.noiseType = static_cast<FastNoise::NoiseType>(indexNoiseType);
                needUpdate = true;
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo(("Interp##"+labelSuffix).c_str(), interps[indexInterp])){ 
        for (unsigned int i = 0; i < IM_ARRAYSIZE(interps); i++){
            if (ImGui::Selectable(interps[i])) {
                indexInterp = i;
                noiseParams.interp = static_cast<FastNoise::Interp>(indexInterp);
                needUpdate = true;
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo(("Fractal Type##"+labelSuffix).c_str(), fractalTypes[indexFractalType])){ 
        for (unsigned int i = 0; i < IM_ARRAYSIZE(fractalTypes); i++){
            if (ImGui::Selectable(fractalTypes[i])) {
                indexFractalType = i;
                noiseParams.fractalType = static_cast<FastNoise::FractalType>(indexFractalType);
                needUpdate = true;
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo(("Cellular Distance Function##"+labelSuffix).c_str(), cellularDistanceFunctions[indexCellularDistanceFunction])){ 
        for (unsigned int i = 0; i < IM_ARRAYSIZE(cellularDistanceFunctions); i++){
            if (ImGui::Selectable(cellularDistanceFunctions[i])) {
                indexCellularDistanceFunction = i;
                noiseParams.cellularDistanceFunction = static_cast<FastNoise::CellularDistanceFunction>(indexCellularDistanceFunction);
                needUpdate = true;
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo(("Cellular Return Type##"+labelSuffix).c_str(), cellularReturnTypes[indexCellularReturnType])){ 
        for (unsigned int i = 0; i < IM_ARRAYSIZE(cellularReturnTypes); i++){
            if (ImGui::Selectable(cellularReturnTypes[i])) {
                indexCellularReturnType = i;
                noiseParams.cellularReturnType = static_cast<FastNoise::CellularReturnType>(indexCellularReturnType);
                needUpdate = true;
            }
        }
        ImGui::EndCombo();
    }

    return needUpdate;
}

bool DebugWindow::OpenSplinePlot(SplineData& spline)
{
    bool needUpdate = false;

    if (ImPlot::BeginPlot("Surface height spline")) {
        ImPlot::SetupAxis(ImAxis_X1, "Noise value", ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax);
        ImPlot::SetupAxis(ImAxis_Y1, "Height", ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax);
        ImPlot::SetupAxisLimits(ImAxis_X1, 0., 1.);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0., 1.);

        std::vector<float>& plotX = spline.plotX;
        std::vector<float>& plotY = spline.plotY;
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
            spline.Reset();
            needUpdate = true;
        }

        ImPlot::EndPlot();
    }
    
    return needUpdate;
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
    if (ImGui::SliderFloat("Frequency##Wave", &data.frequency, 0.5f, 10.f))
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

    needUpdate |= OpenNoiseSettings(data.noiseParams, "Heightmap",
        m_indexNoiseTypeForHeightmap, m_indexInterpForHeightmap, m_indexFractalTypeForHeightmap, m_indexCellularDistanceFunctionForHeightmap, m_indexCellularReturnTypeForHeightmap);

    if (ImGui::Checkbox("Use spline", &data.spline.use))
        needUpdate = true;

    if (data.spline.use)
        needUpdate |= OpenSplinePlot(data.spline);
    
    return needUpdate;
}

bool DebugWindow::OpenSettings(DataCheeseChunk& data)
{
    bool needUpdate = false;

    if (ImGui::SliderFloat("Threshold##Cheese", &data.threshold, -1.f, 1.f))
        needUpdate = true;

    needUpdate |= OpenNoiseSettings(data.noiseParams, "Cheese",
        m_indexNoiseTypeForCheese, m_indexInterpForCheese, m_indexFractalTypeForCheese, m_indexCellularDistanceFunctionForCheese, m_indexCellularReturnTypeForCheese);

    return needUpdate;
}

bool DebugWindow::OpenSettings(DataCaveChunk& data)
{
    bool needUpdate = false;

    if (ImGui::SliderInt("Iteration##Cave", &data.nrIteration, 0, 10))
        needUpdate = true;
    if (ImGui::SliderInt("Threshold##Cave", &data.threshold, 0, 8))
        needUpdate = true;

    if (ImGui::Checkbox("Use spline", &data.spline.use))
        needUpdate = true;
    
    if (data.spline.use)
        needUpdate |= OpenSplinePlot(data.spline);

    return needUpdate;
}

void DebugWindow::KeyCallback(const std::array<bool,GLFW_KEY_LAST+1>& keys)
{
    if (keys[GLFW_KEY_F3])
        m_showMainDebug = !m_showMainDebug;
    if (keys[GLFW_KEY_F4])
        m_showHeightmapDebug = !m_showHeightmapDebug;
}

void DebugWindow::ShowMainWindow()
{
    ImGui::Begin("Main Debug", &m_showMainDebug);

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
            
            // TODO : Merge Surface with Below
            if (ImGui::CollapsingHeader("Surface")) {
                if (ImGui::BeginCombo("Surface Chunk Type", chunkTypes[m_indexSurfaceChunkType])){ 
                    for (unsigned int i = 0; i < IM_ARRAYSIZE(chunkTypes); i++){
                        if (ImGui::Selectable(chunkTypes[i])) {
                            m_indexSurfaceChunkType = i;
                            m_terrain.SetChunkType(ChunkLayer::Surface, static_cast<ChunkType>(m_indexSurfaceChunkType));
                        }
                    }
                    ImGui::EndCombo();
                }

                DataChunk surfaceDataChunk = m_terrain.GetDataChunk(ChunkLayer::Surface);
                std::visit( 
                    [this](auto& data)
                    {
                        if (OpenSettings(data))
                            m_terrain.SetDataChunk(ChunkLayer::Surface, data);
                    },
                    surfaceDataChunk
                );
            }

            if (ImGui::CollapsingHeader("Below")) {
                if (ImGui::BeginCombo("Below Chunk Type", chunkTypes[m_indexBelowChunkType])){ 
                    for (unsigned int i = 0; i < IM_ARRAYSIZE(chunkTypes); i++){
                        if (ImGui::Selectable(chunkTypes[i])) {
                            m_indexBelowChunkType = i;
                            m_terrain.SetChunkType(ChunkLayer::Below, static_cast<ChunkType>(m_indexBelowChunkType));
                        }
                    }
                    ImGui::EndCombo();
                }

                DataChunk belowDataChunk = m_terrain.GetDataChunk(ChunkLayer::Below);
                std::visit( 
                    [this](auto& data)
                    {
                        if (OpenSettings(data))
                            m_terrain.SetDataChunk(ChunkLayer::Below, data);
                    },
                    belowDataChunk
                );
            }
            
            if (ImGui::Button("Load Terrain")) {
                m_terrain.Create();
                m_terrain.Load();
                m_camera.SetTargetTerrain(glm::vec3(m_terrain.GetSize()*CHUNK_SIZE)/2.f);
                m_heightmapTextureId = LoadTexture2D("../data/heightmap/terrain.png");
                // m_caveHeightmapTextureId = LoadTexture2D("../data/heightmap/cave.png");
            }
            ImGui::SameLine();
            if (ImGui::Button("Show Heightmaps")) {
                m_showHeightmapDebug = !m_showHeightmapDebug;
            }
            
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    
    ImGui::End();
}

void DebugWindow::ShowHeightmapWindow()
{
    ImGui::Begin("Heightmap Debug", &m_showHeightmapDebug);

    // const ImVec2 windowSize = ImGui::GetContentRegionAvail();
    // const glm::ivec3 terrainBlockSize = m_terrain.GetSize()*CHUNK_SIZE;
    // const unsigned int heightmapWidth = terrainBlockSize.x;
    // const unsigned int heightmapDepth = terrainBlockSize.z;
    
    ImGui::Image((ImTextureID)(intptr_t)m_heightmapTextureId, ImVec2(512, 512)); // TODO : Not 512x512

    ImGui::End();
}

void DebugWindow::Draw()
{    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
    if (m_showMainDebug)
        ShowMainWindow();
    if (m_showHeightmapDebug)
        ShowHeightmapWindow();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}