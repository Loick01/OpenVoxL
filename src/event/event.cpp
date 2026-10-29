#include "event/event.hpp"

EventController::EventController(GLFWwindow* glfwWindow, 
std::initializer_list<EventKeyCallback*> keyCallbacks,
std::initializer_list<EventCursorPosCallback*> cursorPosCallbacks,
std::initializer_list<EventScrollCallback*> scrollCallbacks):
    m_glfwWindow(glfwWindow), m_keyCallbacks(keyCallbacks), m_cursorPosCallback(cursorPosCallbacks), m_scrollCallbacks(scrollCallbacks),
    m_keys{}, m_mouseButtons{}, m_handBlockIndex(0)
{
    glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide the mouse cursor. Will not be here

    glfwSetWindowUserPointer(m_glfwWindow, this);

    glfwSetKeyCallback(m_glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        EventController* controller = static_cast<EventController*>(glfwGetWindowUserPointer(window));
        controller->ProcessKeyCallback(key, scancode, action, mods);
    });

    glfwSetCursorPosCallback(m_glfwWindow, [](GLFWwindow* window, double xpos, double ypos)
    {
        EventController* controller = static_cast<EventController*>(glfwGetWindowUserPointer(window));
        controller->ProcessCursorPosCallback(xpos, ypos);
    });

    glfwSetMouseButtonCallback(m_glfwWindow, [](GLFWwindow* window, int button, int action, int mods)
    {
        EventController* controller = static_cast<EventController*>(glfwGetWindowUserPointer(window));
        controller->EventMouseButtonCallback(button, action, mods);
    });

    glfwSetScrollCallback(m_glfwWindow, [](GLFWwindow* window, double xoffset, double yoffset)
    {
        EventController* controller = static_cast<EventController*>(glfwGetWindowUserPointer(window));
        controller->ProcessScrollCallback(xoffset, yoffset);
    });
}

const std::array<bool,GLFW_KEY_LAST+1>& EventController::GetKeys() const
{
    return m_keys;
}

const std::array<bool,GLFW_MOUSE_BUTTON_LAST+1>& EventController::GetMouseButtons() const
{
    return m_mouseButtons;
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

void EventController::ProcessKeyCallback(int key, int scancode, int action, int mods)
{
    if (key < 0 || key > GLFW_KEY_LAST) 
        return;

    if (action == GLFW_PRESS) {
        m_keys[key] = true;
        for (EventKeyCallback* e : m_keyCallbacks)
            e->KeyCallback(GetKeys());
    } else if (action == GLFW_RELEASE) {
        m_keys[key] = false;
    }
}

void EventController::ProcessCursorPosCallback(double xpos, double ypos)
{
    for (EventCursorPosCallback* e : m_cursorPosCallback)
        e->CursorPosCallback(xpos, ypos);
}

void EventController::ProcessScrollCallback(double xoffset, double yoffset)
{
    for (EventScrollCallback* e : m_scrollCallbacks)
        e->ScrollCallback(xoffset, yoffset);
}

void EventController::EventMouseButtonCallback(int button, int action, int mods)
{
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST)
        return;

    if (action == GLFW_PRESS)
        m_mouseButtons[button] = true;
    else if (action == GLFW_RELEASE)
        m_mouseButtons[button] = false;
}