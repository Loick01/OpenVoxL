#pragma once

#include <array>

#define GLFW_INCLUDE_NONE // ???
#include <GLFW/glfw3.h>

class Camera;
class DebugWindow;
class Hud;

class EventController
{
    private:
        GLFWwindow* m_glfwWindow;
        
        Camera& m_camera;
        DebugWindow& m_debug;
        Hud& m_hud;

        std::array<bool, GLFW_KEY_LAST+1> m_keys;

        const unsigned int m_hotbarSize;
        unsigned int m_handBlockIndex; // [0, m_hotbarSize]

        void EventKeyCallback(int key, int scancode, int action, int mods);
        void EventCursorPosCallback(double xpos, double ypos);
        void EventMouseButtonCallback(int button, int action, int mods);
        void EventScrollCallback(double xoffset, double yoffset);

    public:
        EventController(GLFWwindow* glfwWindow, Camera& camera, DebugWindow& debug, Hud& hud);
        
        const std::array<bool,GLFW_KEY_LAST+1>& GetKeys() const;
        void PollEvents();
        void HandleWindowEvent();
};