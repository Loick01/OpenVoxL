#include "core/application.hpp"

Application::Application():
    m_window("OpenVoxL"), m_camera(m_window.GetGlfwWindow(), m_player.GetHitbox(), (float)m_window.GetWidth()/(float)m_window.GetHeight()),
    m_eventController(m_window.GetGlfwWindow(), m_camera, m_debug, m_hud),
    m_skybox("../shader/skybox/skybox.vs", "../shader/skybox/skybox.fs"),
    m_terrain(3, 3, 3),
    m_hud("../shader/ui/hud.vs", "../shader/ui/hud.fs", m_window.GetWidth(), m_window.GetHeight()),
    m_debug(m_window.GetGlfwWindow(), m_camera, m_player.GetHitbox(), m_terrain),
    m_player(glm::vec3(0.f), 6.f, 1.5f)
{
    glViewport(0, 0, m_window.GetWidth(), m_window.GetHeight());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

    m_camera.SetTargetTerrain(glm::vec3(m_terrain.GetSize()*CHUNK_SIZE)/2.f);
    // m_soundController.PlayRandonMusic();
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
    
    const std::array<bool, GLFW_KEY_LAST+1>& keys = m_eventController.GetKeys();
    m_player.EventUpdate(keys, deltaTime);
    m_camera.EventUpdate(keys, deltaTime);
    
    // TODO : Use a std::vector<Drawable*> ? (terrain can not be in this)
    m_skybox.Draw(cameraProjectionMatrix, cameraViewMatrix);
    m_terrain.Draw(cameraProjectionMatrix, cameraViewMatrix);
    m_hud.Draw();
    m_debug.Draw();
    
    return m_window.EndFrame();
}