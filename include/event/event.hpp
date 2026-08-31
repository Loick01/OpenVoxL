#pragma once

#include <GLFW/glfw3.h>

class EventController
{
    private:
        GLFWwindow* m_glfwWindow;
        
    public:
        EventController(GLFWwindow* glfwWindow);
        
        void HandleWindowEvent();
};