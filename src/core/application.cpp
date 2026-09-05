#include "core/application.hpp"

Application::Application():
    m_window("OpenVoxL"), m_camera((float)m_window.GetWidth()/(float)m_window.GetHeight()),
    m_eventController(m_window.GetGlfwWindow(), m_camera),
    m_skybox("../shader/skybox/skybox.vs", "../shader/skybox/skybox.fs"),
    m_terrain(2, 2),
    m_hud("../shader/ui/hud.vs", "../shader/ui/hud.fs", m_window.GetWidth(), m_window.GetHeight()),
    m_debug(m_window.GetGlfwWindow())
{
    glViewport(0, 0, m_window.GetWidth(), m_window.GetHeight());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

    m_soundController.PlayRandonMusic();
}

bool Application::Run()
{
    m_time.Update();
    const double deltaTime = m_time.GetDeltaTime();
    
    m_eventController.PollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_eventController.HandleWindowEvent();
    
    const glm::mat4 cameraViewMatrix = m_camera.GetViewMatrix();
    const glm::mat4 cameraProjectionMatrix = m_camera.GetProjectionMatrix();
    
    m_camera.ProcessKeyEvent(m_eventController.GetKeys(), deltaTime);
    
    m_skybox.Draw(cameraProjectionMatrix, cameraViewMatrix);
    m_terrain.Draw(cameraProjectionMatrix, cameraViewMatrix);
    m_hud.Draw();
    m_debug.Draw();
    
    return m_window.EndFrame();
}