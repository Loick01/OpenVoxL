#include "core/camera.hpp"

#include "glm/gtc/matrix_transform.hpp" // glm::lookAt

Camera::Camera(const float aspectRatio):
    m_state(CameraState::MouseFree), m_upVector(glm::vec3(0.f, 1.f, 0.f)),
    m_position(glm::vec3(0.f)), m_targetVector(glm::vec3(0.f, 0.f, -1.f)),
    m_fov(75.f), m_nearPlane(0.1f), m_farPlane(100.f), m_aspectRatio(aspectRatio),
    m_sensitivity(0.05f)
{}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_targetVector, m_upVector);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
}

void Camera::CursorPosCallback(double xpos, double ypos)
{
    double deltaX = xpos - m_previousXPos;
    double deltaY = m_previousYPos - ypos;

    m_phi += deltaX * m_sensitivity;
    m_theta += deltaY * m_sensitivity;
    m_theta = glm::clamp(m_theta, -89.0f, 89.0f);

    const float x = cos(glm::radians(m_phi)) * cos(glm::radians(m_theta));
    const float y = sin(glm::radians(m_theta));
    const float z = sin(glm::radians(m_phi)) * cos(glm::radians(m_theta));
    m_targetVector = glm::normalize(glm::vec3(x, y, z));

    m_previousXPos=xpos;
    m_previousYPos=ypos;
}