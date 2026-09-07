#include "ui/debug_window.hpp"

#include <GLFW/glfw3.h>

#include "core/camera.hpp"

namespace
{
    const char* cameraStates[] = {"KeyFree", "MouseFree", "Orbital", "Player"};
}

DebugWindow::DebugWindow(GLFWwindow* glfwWindow, Camera& camera):
    m_glfwWindow(glfwWindow), m_width(500), m_height(500), m_camera(camera),
    m_selectedCameraState((unsigned int)m_camera.GetState())
{
    InitImGui();
}

DebugWindow::~DebugWindow()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    ImPlot::DestroyContext();
}

void DebugWindow::InitImGui()
{
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_glfwWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImPlot::CreateContext();
}

void DebugWindow::Draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    float fps = ImGui::GetIO().Framerate;

    ImGui::NewFrame();
    ImGui::Begin("Debug Window");

    if (ImGui::BeginTabBar("Tabs")){

        if (ImGui::BeginTabItem("Camera")){
            const glm::vec3 cameraPosition = m_camera.GetPosition();
            float cameraSpeed = m_camera.GetSpeed();
            
            ImGui::Text("FPS: %.1f", fps);
            ImGui::Text("Camera Position: %.1f %.1f %.1f", cameraPosition.x, cameraPosition.y, cameraPosition.z);
            if (ImGui::SliderFloat("Camera speed", &cameraSpeed, 1.f, 200.f))
                m_camera.SetSpeed(cameraSpeed);

            if (ImGui::BeginCombo("Camera State", cameraStates[m_selectedCameraState])){ // TODO 
                for (unsigned int i = 0; i < IM_ARRAYSIZE(cameraStates); i++){
                    if (ImGui::Selectable(cameraStates[i])){
                        m_selectedCameraState = i;
                        m_camera.SetState(static_cast<CameraState>(m_selectedCameraState));
                    }
                }
                ImGui::EndCombo();
            }
            
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}