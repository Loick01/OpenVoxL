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
        bool m_wireframeRendering;

        void InitImGui();
        
        void OpenSettings(DataFlatChunk& data);
        void OpenSettings(DataFullChunk& data);
        void OpenSettings(DataWaveChunk& data);
        void OpenSettings(DataEditorChunk& data);
        void OpenSettings(DataHeightmapChunk& data);
        void OpenSettings(DataCheeseChunk& data);

    public:
        DebugWindow(GLFWwindow* glfwWindow, Camera& camera, Terrain& terrain);
        ~DebugWindow();

        void Draw();
};