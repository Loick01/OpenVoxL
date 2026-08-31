#include "core/application.hpp"

Application::Application():
    m_window("OpenVoxL"), m_eventController(m_window.GetGlfwWindow())
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD");

    glViewport(0, 0, m_window.GetWidth(), m_window.GetHeight());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    
    // glfwSetKeyCallback(window, key_callback);
    // glfwSetCursorPosCallback(window, mouse_cursor_callback);
    // glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glfwSetScrollCallback(window, scroll_callback);
}

bool Application::Run()
{
    m_window.PollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_eventController.HandleWindowEvent();
    
    return m_window.EndFrame();
}