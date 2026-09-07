#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

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
        unsigned int m_selectedCameraState;
        bool m_wireframeRendering;

        void InitImGui();

    public:
        DebugWindow(GLFWwindow* glfwWindow, Camera& camera, Terrain& terrain);
        ~DebugWindow();

        void Draw();
};