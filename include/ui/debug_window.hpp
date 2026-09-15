#pragma once


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include <string>

#include "terrain/data_chunk.hpp"

class Camera;
class Terrain;

class DebugWindow
{
    private:    
        GLFWwindow* m_glfwWindow;
        Camera& m_camera;
        Terrain& m_terrain;
                
        int m_width;
        int m_height;
        unsigned int m_indexCameraState;
        unsigned int m_indexSurfaceChunkType;
        unsigned int m_indexBelowChunkType;

        // TODO : Merge indexForHeightmap with indexForCheese
        unsigned int m_indexNoiseTypeForHeightmap;
        unsigned int m_indexInterpForHeightmap;
        unsigned int m_indexFractalTypeForHeightmap;
        unsigned int m_indexCellularDistanceFunctionForHeightmap;
        unsigned int m_indexCellularReturnTypeForHeightmap;

        unsigned int m_indexNoiseTypeForCheese;
        unsigned int m_indexInterpForCheese;
        unsigned int m_indexFractalTypeForCheese;
        unsigned int m_indexCellularDistanceFunctionForCheese;
        unsigned int m_indexCellularReturnTypeForCheese;

        // TODO : Remove ? + Rename
        GLuint m_heightmapTextureId;
        GLuint m_caveHeightmapTextureId;
        
        bool m_wireframeRendering;
        bool m_showMainDebug;
        bool m_showHeightmapDebug;

        void InitImGui();
        
        bool OpenNoiseSettings(NoiseParameters& noiseParams, const std::string& labelSuffix,
            unsigned int& indexNoiseType, unsigned int& indexInterp, unsigned int& indexFractalType, unsigned int& indexCellularDistanceFunction, unsigned int& indexCellularReturnType); // TODO : Remove index... parameters
        bool OpenSplinePlot(SplineData& spline);
        
        bool OpenSettings(DataFlatChunk& data);
        bool OpenSettings(DataFullChunk& data);
        bool OpenSettings(DataWaveChunk& data);
        bool OpenSettings(DataEditorChunk& data);
        bool OpenSettings(DataHeightmapChunk& data);
        bool OpenSettings(DataCheeseChunk& data);
        bool OpenSettings(DataCaveChunk& data);
        
        void ShowMainWindow();
        void ShowHeightmapWindow();

    public:
        DebugWindow(GLFWwindow* glfwWindow, Camera& camera, Terrain& terrain);
        ~DebugWindow();
        
        void KeyCallback(const std::array<bool,GLFW_KEY_LAST+1>& keys);
        void Draw();
};