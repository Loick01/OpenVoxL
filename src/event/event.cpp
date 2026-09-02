#include "event/event.hpp"

#include <iostream>

EventController::EventController(GLFWwindow* glfwWindow, Camera& camera):
    m_glfwWindow(glfwWindow), m_camera(camera), m_keys{}
{
    glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide the mouse cursor. Will not be here

    glfwSetWindowUserPointer(m_glfwWindow, this);

    glfwSetKeyCallback(m_glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        EventController* controller = static_cast<EventController*>(glfwGetWindowUserPointer(window));
        controller->EventKeyCallback(key, scancode, action, mods);
    });

    glfwSetCursorPosCallback(m_glfwWindow, [](GLFWwindow* window, double xpos, double ypos)
    {
        EventController* controller = static_cast<EventController*>(glfwGetWindowUserPointer(window));
        controller->EventCursorPosCallback(xpos, ypos);
    });

    glfwSetMouseButtonCallback(m_glfwWindow, [](GLFWwindow* window, int button, int action, int mods)
    {
        EventController* controller = static_cast<EventController*>(glfwGetWindowUserPointer(window));
        controller->EventMouseButtonCallback(button, action, mods);
    });

    glfwSetScrollCallback(m_glfwWindow, [](GLFWwindow* window, double xoffset, double yoffset)
    {
        EventController* controller = static_cast<EventController*>(glfwGetWindowUserPointer(window));
        controller->EventScrollCallback(xoffset, yoffset);
    });
}

const std::array<bool,GLFW_KEY_LAST+1>& EventController::GetKeys() const
{
    return m_keys;
}

void EventController::PollEvents()
{
    glfwPollEvents();
}

void EventController::HandleWindowEvent()
{
    if (glfwGetKey(m_glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_glfwWindow, true);
}

void EventController::EventKeyCallback(int key, int scancode, int action, int mods)
{
    if (key < 0 || key > GLFW_KEY_LAST) 
        return;

    if (action == GLFW_PRESS) {
        m_keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        m_keys[key] = false;
    }
}

void EventController::EventCursorPosCallback(double xpos, double ypos)
{
    m_camera.CursorPosCallback(xpos, ypos);
}

void EventController::EventMouseButtonCallback(int button, int action, int mods)
{}

void EventController::EventScrollCallback(double xoffset, double yoffset)
{}