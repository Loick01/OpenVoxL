#pragma once


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include <string>
#include <vector> // TODO : Remove ? (Only used for m_loadedStructures)

#include "core/notifier.hpp"
#include "event/key_callback.hpp"
#include "setting/logger.hpp"
#include "world/data_chunk.hpp" // TODO : Remove ?
#include "world/structure.hpp"

class Camera;
class CollisionResolver;
class FileReader;
class Hitbox;
class Player;
class Terrain;
class TerrainModifier;

enum class Setting 
{
    UpdateValues
};

// TODO : Should not be here ?
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

// TODO : Should not be here ?
struct StructureInfo
{
    const std::string name; // Without extension
    GLuint iconTextureId; // TODO : use ImTextureID instead of GLuint

    bool operator==(const StructureInfo& other) const 
    {
        return name == other.name;
    }
};

class SettingWindow : public EventKeyCallback, public Notifier<Setting>
{
    private:    
        GLFWwindow* m_glfwWindow;
        Camera& m_camera;
        CollisionResolver& m_collisionResolver;
        Player& m_player;
        Hitbox& m_hitbox;
        Terrain& m_terrain;
        TerrainModifier& m_terrainModifier;
        const FileReader& m_fileReader; // TODO : Try to remove ?
            
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

        // TODO : Remove ?
        std::vector<StructureInfo> m_loadedStructures;
        unsigned int m_selectedStructure;
        
        bool m_wireframeRendering;
        bool m_showMainDebug;
        bool m_editorMode;

        void InitImGui();
        
        void OpenStructureDirectory();
        void WriteStructureIcon(const std::string& name, const unsigned int iconSize);
        void ShowStructureIcons();
        
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
        SettingWindow(GLFWwindow* glfwWindow, Camera& camera, CollisionResolver& collisionResolver, Player& player, 
            Terrain& terrain, TerrainModifier& terrainModifier, const FileReader& fileReader);
        ~SettingWindow();
        
        StructureData GetSelectedStructure() const;

        void KeyCallback(const std::array<bool, GLFW_KEY_LAST+1>& keys) override;
        void Draw();
};