#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

class Camera;

class DebugWindow
{
    private:    
        GLFWwindow* m_glfwWindow;
        Camera& m_camera;
        
        int m_width;
        int m_height;
        unsigned int m_selectedCameraState;

        void InitImGui();

    public:
        DebugWindow(GLFWwindow* glfwWindow, Camera& camera);
        ~DebugWindow();

        void Draw();
};