#pragma once

#include <GLFW/glfw3.h>

class Camera;

class EventController
{
    private:
        GLFWwindow* m_glfwWindow;
        
        Camera& m_camera;

        void EventKeyCallback(int key, int scancode, int action, int mods);
        void EventCursorPosCallback(double xpos, double ypos);
        void EventMouseButtonCallback(int button, int action, int mods);
        void EventScrollCallback(double xoffset, double yoffset);

    public:
        EventController(GLFWwindow* glfwWindow, Camera& camera);
        
        void PollEvents();
        void HandleWindowEvent();
};