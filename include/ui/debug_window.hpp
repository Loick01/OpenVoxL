#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

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
        unsigned int m_indexChunkType;
        unsigned int m_indexNoiseType;
        bool m_wireframeRendering;

        void InitImGui();
        
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