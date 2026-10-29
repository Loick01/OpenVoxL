#pragma once

#include <array>
#include <initializer_list>
#include <vector>

#define GLFW_INCLUDE_NONE // ???
#include <GLFW/glfw3.h>

#include "event/cursor_pos_callback.hpp"
#include "event/key_callback.hpp"
#include "event/scroll_callback.hpp"

class EventController
{
    private:
        GLFWwindow* m_glfwWindow;

        std::vector<EventKeyCallback*> m_keyCallbacks;
        std::vector<EventCursorPosCallback*> m_cursorPosCallback;
        std::vector<EventScrollCallback*> m_scrollCallbacks;

        std::array<bool, GLFW_KEY_LAST+1> m_keys;
        std::array<bool, GLFW_MOUSE_BUTTON_LAST+1> m_mouseButtons;

        unsigned int m_handBlockIndex; // [0, m_hotbarSize]

        void ProcessKeyCallback(int key, int scancode, int action, int mods);        
        void ProcessCursorPosCallback(double xpos, double ypos);
        void ProcessScrollCallback(double xoffset, double yoffset);

        void EventMouseButtonCallback(int button, int action, int mods);

    public:
        EventController(GLFWwindow* glfwWindow, 
            std::initializer_list<EventKeyCallback*> keyCallbacks, 
            std::initializer_list<EventCursorPosCallback*> cursorPosCallbacks, 
            std::initializer_list<EventScrollCallback*> scrollCallbacks);
        
        const std::array<bool,GLFW_KEY_LAST+1>& GetKeys() const;
        const std::array<bool,GLFW_MOUSE_BUTTON_LAST+1>& GetMouseButtons() const;

        void PollEvents();
        void HandleWindowEvent();
};