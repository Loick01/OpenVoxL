#include "event/event.hpp"

EventController::EventController(GLFWwindow* glfwWindow):
    m_glfwWindow(glfwWindow)
{}

void EventController::HandleWindowEvent()
{
    if (glfwGetKey(m_glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_glfwWindow, true);
}