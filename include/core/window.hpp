#pragma once

#include <stdexcept>
#include <string>

#include <GLFW/glfw3.h>

class Window
{
    private:
        GLFWwindow* m_glfwWindow;
        
        std::string m_title;
        unsigned int m_width;
        unsigned int m_height;
        
        void InitGlfw();
        
    public:
        Window(const std::string& title);
        ~Window();

        GLFWwindow* GetGlfwWindow();
        unsigned int GetWidth() const;
        unsigned int GetHeight() const;

        bool EndFrame();
        void PollEvents();
};