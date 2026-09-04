#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

class DebugWindow
{
    private:    
        GLFWwindow* m_glfwWindow;
        int m_width;
        int m_height;

        void InitImGui();

    public:
        DebugWindow(GLFWwindow* glfwWindow);
        ~DebugWindow();

        void Draw();
};