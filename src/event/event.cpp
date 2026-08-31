#include "event/event.hpp"

EventController::EventController(GLFWwindow* glfwWindow):
    m_glfwWindow(glfwWindow)
{
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
{}

void EventController::EventCursorPosCallback(double xpos, double ypos)
{}

void EventController::EventMouseButtonCallback(int button, int action, int mods)
{}

void EventController::EventScrollCallback(double xoffset, double yoffset)
{}