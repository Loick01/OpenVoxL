#pragma once


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include <string>

#include "terrain/data_chunk.hpp"

class Camera;
class CollisionResolver;
class Hitbox;
class Player;
class Terrain;
class TerrainModifier;

struct NoiseSettings
{
    unsigned int indexNoiseType;
    unsigned int indexInterp;
    unsigned int indexFractalType;
    unsigned int indexCellularDistanceFunction;
    unsigned int indexCellularReturnType;

    NoiseSettings():
        indexNoiseType(5),
        indexInterp(0),
        indexFractalType(0),
        indexCellularDistanceFunction(0), 
        indexCellularReturnType(0)
    {}
};

class DebugWindow
{
    private:    
        GLFWwindow* m_glfwWindow;
        Camera& m_camera;
        CollisionResolver& m_collisionResolver;
        Player& m_player;
        Hitbox& m_hitbox;
        Terrain& m_terrain;
        TerrainModifier& m_terrainModifier;
            
        glm::ivec3 m_nextSizeChunk; // Next Terrain::m_sizeChunk
         
        int m_width;
        int m_height;
        unsigned int m_indexCameraState;
        unsigned int m_indexSurfaceChunkType;
        unsigned int m_indexBelowChunkType;

        NoiseSettings m_heightmapNoiseSettings;
        NoiseSettings m_cheeseNoiseSettings;
        // TODO : Add m_caveNoiseSettings ?

        // TODO : Remove ? + Rename
        GLuint m_heightmapTextureId;
        GLuint m_caveHeightmapTextureId;
        
        bool m_wireframeRendering;
        bool m_showMainDebug;
        bool m_editorMode;

        void InitImGui();
        
        bool OpenNoiseSettings(NoiseParameters& noiseParams, const std::string& labelSuffix, NoiseSettings& noiseSettings);
        bool OpenSplinePlot(SplineData& spline);
        
        bool OpenSettings(DataFlatChunk& data);
        bool OpenSettings(DataFullChunk& data);
        bool OpenSettings(DataWaveChunk& data);
        bool OpenSettings(DataEditorChunk& data);
        bool OpenSettings(DataHeightmapChunk& data);
        bool OpenSettings(DataCheeseChunk& data);
        bool OpenSettings(DataCaveChunk& data);

        void ReloadTerrain();
        
        void ShowMainWindow();

    public:
        DebugWindow(GLFWwindow* glfwWindow, Camera& camera, CollisionResolver& collisionResolver, Player& player, 
            Terrain& terrain, TerrainModifier& terrainModifier);
        ~DebugWindow();
        
        void KeyCallback(const std::array<bool,GLFW_KEY_LAST+1>& keys);
        void Draw();
};