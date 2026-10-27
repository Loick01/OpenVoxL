#include "setting/setting.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/camera.hpp"
#include "entity/collision.hpp"
#include "entity/player.hpp"
#include "graphic/texture.hpp"
#include "world/terrain/terrain.hpp"
#include "world/terrain/terrain_modifier.hpp"

namespace
{
    const char* cameraStates[] = {"KeyFree (E)", "MouseFree (E)", "Orbital (O)", "Player (P)"};
    const char* chunkTypes[] = {"Full", "Flat", "Wave", "Editor", "Heightmap", "Cheese", "Cave"};
}

SettingWindow::SettingWindow(GLFWwindow* glfwWindow, Camera& camera, CollisionResolver& collisionResolver, Player& player, 
Terrain& terrain, TerrainModifier& terrainModifier, const FileReader& fileReader):
    m_glfwWindow(glfwWindow), m_width(500), m_height(500), m_camera(camera), m_collisionResolver(collisionResolver), 
    m_player(player), m_hitbox(m_player.GetHitbox()), m_terrain(terrain), m_terrainModifier(terrainModifier), m_fileReader(fileReader),
    m_indexCameraState((unsigned int)m_camera.GetState()), m_nextSizeChunk(m_terrain.GetSizeChunk()),
    m_indexSurfaceChunkType((unsigned int)m_terrain.GetChunkType(ChunkLayer::Surface)), 
    m_indexBelowChunkType((unsigned int)m_terrain.GetChunkType(ChunkLayer::Below)),
    m_wireframeRendering(false), m_showMainDebug(true), m_editorMode(false)
{
    InitImGui();
    m_heightmapTextureId = LoadTexture2D("../data/heightmap/terrain.png");
    m_caveHeightmapTextureId = LoadTexture2D("../data/heightmap/cave.png");

    OpenStructureDirectory();
}

SettingWindow::~SettingWindow()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    ImPlot::DestroyContext();
}

void SettingWindow::InitImGui()
{
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_glfwWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImPlot::CreateContext();
}

void SettingWindow::ReloadTerrain()
{
    // TODO : Should not be here and use Notifier instead ?
    glm::vec3 terrainCenter = glm::vec3(m_nextSizeChunk*CHUNK_SIZE)/2.f;
    m_terrain.SetSizeChunk(m_nextSizeChunk); // TODO ?
    m_terrain.Create();
    m_terrain.Load();
    m_camera.SetTargetTerrain(terrainCenter);
    terrainCenter.y *= 2.5f; // TODO ?
    m_hitbox.SetPosition(terrainCenter);
}

void SettingWindow::KeyCallback(const std::array<bool,GLFW_KEY_LAST+1>& keys)
{
    if (keys[GLFW_KEY_F3])
        m_showMainDebug = !m_showMainDebug;
}

void SettingWindow::ShowMainWindow()
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
            
            if (ImGui::Checkbox("Editor mode", &m_editorMode)) {
                m_nextSizeChunk = glm::ivec3(1);
                m_terrain.SetSurfaceChunkHeight(1);
                m_terrain.SetChunkType(ChunkLayer::Surface, ChunkType::Editor);

                ReloadTerrain();
            }
                
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Logs")){
            
            Logger& logger = Logger::GetInstance();
            
            if (ImGui::Button("Clear"))
                logger.Clear();

            ImGui::BeginChild(
                "LogScrollRegion",
                ImVec2(0, 0),
                ImGuiChildFlags_Border,
                ImGuiWindowFlags_HorizontalScrollbar
            );
        
            // TODO : Use ImGuiListClipper ?
            for (const Log& log : logger.GetLogs()){
                const ImVec4 labelColor = ImVec4(log.labelColor[0], log.labelColor[1], log.labelColor[2], 1.f); 
                
                ImGui::PushStyleColor(ImGuiCol_Text, labelColor);
                ImGui::TextUnformatted(log.label.data());
                ImGui::PopStyleColor();

                ImGui::SameLine();
                ImGui::TextUnformatted(log.message.c_str());
                ImGui::Separator();
            }

            ImGui::EndChild();

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

        if (ImGui::BeginTabItem("Player")){
            const glm::vec3 playerPosition = m_hitbox.GetPosition();
            float walkSpeed = m_player.GetSpeed();
            float jumpStrength = m_player.GetJumpStrength();
            float gravity = m_collisionResolver.GetGravity();
            float updateDelay = m_terrainModifier.GetUpdateDelay().delay;
            float reachDistance = m_terrainModifier.GetReachDistance();

            ImGui::Text("Player Position: %.1f/%.1f/%.1f", playerPosition.x, playerPosition.y, playerPosition.z);
            
            if (ImGui::SliderFloat("Player walk speed", &walkSpeed, 1.f, 50.f))
                m_player.SetSpeed(walkSpeed);

            if (ImGui::SliderFloat("Player jump strength", &jumpStrength, 0.5f, 100.f))
                m_player.SetJumpStrength(jumpStrength);

            if (ImGui::SliderFloat("Gravity", &gravity, 0.f, -100.f))
                m_collisionResolver.SetGravity(gravity);

            if (ImGui::SliderFloat("Update delay", &updateDelay, 0.f, 10.f))
                m_terrainModifier.SetUpdateDelay(updateDelay);

            if (ImGui::SliderFloat("Reach distance", &reachDistance, 0.f, 20.f))
                m_terrainModifier.SetReachDistance(reachDistance);

            ImGui::EndTabItem();
        }

        if (m_editorMode && ImGui::BeginTabItem("Editor")){
            
            static char name[128] = "";
            ImGui::InputText("Structure Name", name, sizeof(name));

            if (ImGui::Button("Save Structure")) {
                m_terrain.SaveEditorChunkInFile(name);
            }
            
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Structure")){
            ImGui::Text("TODO");
            
            ImGui::EndTabItem();
        }

        if (!m_editorMode && ImGui::BeginTabItem("Terrain")){
            int surfaceChunkHeight = m_terrain.GetSurfaceChunkHeight();

            ImGui::SliderInt("Terrain Chunk Width", &m_nextSizeChunk.x, 1, 10);
            
            if (ImGui::SliderInt("Terrain Chunk Height", &m_nextSizeChunk.y, 1, 10)) {
                if (surfaceChunkHeight > m_nextSizeChunk.y)
                    m_terrain.SetSurfaceChunkHeight(m_nextSizeChunk.y);
            }

            ImGui::SliderInt("Terrain Chunk Depth", &m_nextSizeChunk.z, 1, 10);

            if (ImGui::SliderInt("Surface Chunk Height", &surfaceChunkHeight, 0, m_nextSizeChunk.y))
                m_terrain.SetSurfaceChunkHeight(surfaceChunkHeight);
            
            // TODO : Merge "Surface" with "Below"
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
                ReloadTerrain();
                
                m_heightmapTextureId = LoadTexture2D("../data/heightmap/terrain.png");
                m_caveHeightmapTextureId = LoadTexture2D("../data/heightmap/cave.png");
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Heightmap")){
            // const ImVec2 windowSize = ImGui::GetContentRegionAvail();
            // const glm::ivec3 terrainBlockSize = m_terrain.GetSizeChunk()*CHUNK_SIZE;
            // const unsigned int heightmapWidth = terrainBlockSize.x;
            // const unsigned int heightmapDepth = terrainBlockSize.z;
            
            // TODO : Not 512x512
            ImGui::Image((ImTextureID)(intptr_t)m_heightmapTextureId, ImVec2(512, 512));
            ImGui::Image((ImTextureID)(intptr_t)m_caveHeightmapTextureId, ImVec2(512, 512));

            ImGui::EndTabItem();
        }
        
        ImGui::EndTabBar();
    }
    
    ImGui::End();
}

void SettingWindow::Draw()
{    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
    if (m_showMainDebug)
        ShowMainWindow();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}