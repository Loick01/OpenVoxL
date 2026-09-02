#pragma once

#include <array>

#include <GLFW/glfw3.h>

class Camera;

class EventController
{
    private:
        GLFWwindow* m_glfwWindow;
        
        Camera& m_camera;

        std::array<bool, GLFW_KEY_LAST+1> m_keys;

        const unsigned int m_hotbarSize;
        unsigned int m_handBlockIndex; // [0, m_hotbarSize]

        void EventKeyCallback(int key, int scancode, int action, int mods);
        void EventCursorPosCallback(double xpos, double ypos);
        void EventMouseButtonCallback(int button, int action, int mods);
        void EventScrollCallback(double xoffset, double yoffset);

    public:
        EventController(GLFWwindow* glfwWindow, Camera& camera);
        
        const std::array<bool,GLFW_KEY_LAST+1>& GetKeys() const;
        void PollEvents();
        void HandleWindowEvent();
};