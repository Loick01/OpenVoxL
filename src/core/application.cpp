#include "core/application.hpp"

Application::Application():
    m_window("OpenVoxL"), m_eventController(m_window.GetGlfwWindow()),
    m_skybox("../shader/skybox.vs", "../shader/skybox.fs")
{
    glViewport(0, 0, m_window.GetWidth(), m_window.GetHeight());

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

    m_skybox.BindCubemap(0);
}

bool Application::Run()
{
    m_eventController.PollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_eventController.HandleWindowEvent();
    
    m_skybox.Draw(glm::mat4(1.f), glm::mat4(1.f), glm::mat4(1.f));
    
    return m_window.EndFrame();
}