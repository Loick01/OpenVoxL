#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include <string>
#include <vector>

struct NoiseParameters;
class DataFlatChunk;
class DataFullChunk;
class DataWaveChunk;
class DataEditorChunk;
class DataHeightmapChunk;
class DataCheeseChunk;
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

        bool m_wireframeRendering;

        void InitImGui();
        
        bool OpenNoiseSettings(NoiseParameters& noiseParams, const std::string& labelSuffix,
            unsigned int& indexNoiseType, unsigned int& indexInterp, unsigned int& indexFractalType, unsigned int& indexCellularDistanceFunction, unsigned int& indexCellularReturnType); // TODO : Remove index... parameters
        bool OpenSplinePlot(std::vector<float>& plotX, std::vector<float>& plotY);
        
        bool OpenSettings(DataFlatChunk& data);
        bool OpenSettings(DataFullChunk& data);
        bool OpenSettings(DataWaveChunk& data);
        bool OpenSettings(DataEditorChunk& data);
        bool OpenSettings(DataHeightmapChunk& data);
        bool OpenSettings(DataCheeseChunk& data);

    public:
        DebugWindow(GLFWwindow* glfwWindow, Camera& camera, Terrain& terrain);
        ~DebugWindow();

        void Draw();
};