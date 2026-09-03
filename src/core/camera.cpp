#include "core/camera.hpp"

#include <glm/gtc/matrix_transform.hpp> // glm::lookAt

Camera::Camera(const float aspectRatio):
    m_state(CameraState::MouseFree), m_upVector(glm::vec3(0.f, 1.f, 0.f)),
    m_position(glm::vec3(0.f)), m_fov(75.f), m_nearPlane(0.1f), m_farPlane(100.f),
    m_aspectRatio(aspectRatio), m_yaw(-90.f), m_pitch(0.f),
    m_sensitivity(0.05f), m_speed(5.f)
{
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_frontVector, m_upVector);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
}

void Camera::UpdateCameraVectors()
{
    const float x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    const float y = sin(glm::radians(m_pitch));
    const float z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_frontVector = glm::normalize(glm::vec3(x, y, z));
    m_rightVector = glm::normalize(glm::cross(m_frontVector, m_upVector));
}

void Camera::ProcessKeyEvent(const std::array<bool, GLFW_KEY_LAST+1>& keys, const float deltaTime)
{
    const float frameSpeed = m_speed * deltaTime;

    if (keys[GLFW_KEY_W])
        m_position += frameSpeed * m_frontVector;
    if (keys[GLFW_KEY_S])
        m_position -= frameSpeed * m_frontVector;
    if (keys[GLFW_KEY_D])
        m_position += frameSpeed * m_rightVector;
    if (keys[GLFW_KEY_A])
        m_position -= frameSpeed * m_rightVector;
    if (keys[GLFW_KEY_SPACE])
        m_position += frameSpeed * m_upVector;
    if (keys[GLFW_KEY_LEFT_CONTROL])
        m_position -= frameSpeed * m_upVector;
}

void Camera::CursorPosCallback(double xpos, double ypos)
{
    double deltaX = xpos - m_previousXPos;
    double deltaY = m_previousYPos - ypos;

    m_previousXPos = xpos;
    m_previousYPos = ypos;
    
    m_yaw += deltaX * m_sensitivity;
    m_pitch += deltaY * m_sensitivity;
    m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

    UpdateCameraVectors();
}
