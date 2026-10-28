#pragma once

#include <array>

#define GLFW_INCLUDE_NONE // ???
#include <GLFW/glfw3.h>

class Camera;
class SettingWindow;
class Hotbar;
class Hud;

class EventController
{
    private:
        GLFWwindow* m_glfwWindow;
        
        Camera& m_camera;
        SettingWindow& m_settings;
        Hud& m_hud;
        Hotbar& m_hotbar;

        std::array<bool, GLFW_KEY_LAST+1> m_keys;
        std::array<bool, GLFW_MOUSE_BUTTON_LAST+1> m_mouseButtons;

        unsigned int m_handBlockIndex; // [0, m_hotbarSize]

        void EventKeyCallback(int key, int scancode, int action, int mods);
        void EventCursorPosCallback(double xpos, double ypos);
        void EventMouseButtonCallback(int button, int action, int mods);
        void EventScrollCallback(double xoffset, double yoffset);

    public:
        EventController(GLFWwindow* glfwWindow, Camera& camera, SettingWindow& settings, Hud& hud, Hotbar& hotbar);
        
        const std::array<bool,GLFW_KEY_LAST+1>& GetKeys() const;
        const std::array<bool,GLFW_MOUSE_BUTTON_LAST+1>& GetMouseButtons() const;

        void PollEvents();
        void HandleWindowEvent();
};