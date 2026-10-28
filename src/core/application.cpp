#include "core/application.hpp"

Application::Application():
    m_window("OpenVoxL"), m_camera(m_window.GetGlfwWindow(), m_player.GetHitbox(), (float)m_window.GetWidth()/(float)m_window.GetHeight()),
    m_eventController(m_window.GetGlfwWindow(), m_camera, m_settings, m_hud, m_hotbar),
    m_skybox("../shader/skybox/skybox.vs", "../shader/skybox/skybox.fs"),
    m_terrain(m_fileReader, glm::ivec3(3)), m_terrainModifier(m_camera, m_hotbar, m_hud, m_terrain, 0.2f, 4.5f),
    m_hud("../shader/ui/hud.vs", "../shader/ui/hud.fs", m_window.GetWidth(), m_window.GetHeight()),
    m_hotbar("../shader/ui/hotbar.vs", "../shader/ui/hotbar.fs", m_window.GetWidth(), m_window.GetHeight()),
    m_settings(m_window.GetGlfwWindow(), m_camera, m_collisionResolver, m_player, m_terrain, m_terrainModifier, m_fileReader),
    m_player(glm::vec3(0.f), 6.5f, 1.5f, 4.5f, 9.f), m_collisionResolver(m_terrain, -30.f)
{
    glViewport(0, 0, m_window.GetWidth(), m_window.GetHeight());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

    const glm::vec3 terrainCenter = glm::vec3(m_terrain.GetSizeChunk()*CHUNK_SIZE)/2.f;
    m_camera.SetTargetTerrain(terrainCenter);
    m_player.GetHitbox().SetPosition(terrainCenter);
    // m_soundController.PlayRandonMusic();

    UpdateFromSetting(Setting::UpdateValues);
    m_settings.AddCallback([this](Setting e){UpdateFromSetting(e);});
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
    const std::array<bool, GLFW_MOUSE_BUTTON_LAST+1>& mouseButtons = m_eventController.GetMouseButtons();
    
    m_player.EventKeyUpdate(keys, deltaTime);
    m_collisionResolver.Resolve(m_player.GetHitbox(), m_player.GetFrameSpeed(), deltaTime);
    m_terrainModifier.EventMouseButtonUpdate(mouseButtons, deltaTime);
    m_camera.EventKeyUpdate(keys, deltaTime);
    
    m_skybox.Draw(cameraProjectionMatrix, cameraViewMatrix);
    m_terrain.Draw(cameraProjectionMatrix, cameraViewMatrix);
    
    m_hud.Draw();
    m_hotbar.Draw();
    
    m_settings.Draw();
    
    return m_window.EndFrame();
}

void Application::UpdateFromSetting(const Setting e)
{
    // TODO : switch(e) ?
    m_terrainModifier.SetCurrentStructure(m_settings.GetSelectedStructure());
}