#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum class CameraState
{
    KeyFree, MouseFree, Orbital, Player
};

class Camera
{
    private:
        const glm::vec3 m_upVector;
        glm::vec3 m_frontVector;
        glm::vec3 m_rightVector;
        glm::vec3 m_position;
        const float m_nearPlane;
        const float m_farPlane;
        const float m_aspectRatio;
        const float m_sensitivity;
        float m_fov;
        float m_speed;

        double m_previousXPos;
        double m_previousYPos;
        float m_yaw;
        float m_pitch;
        
        CameraState m_state;

        void UpdateCameraVectors();
        
    public:
        Camera(const float aspectRatio);

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;

        void ProcessKeyEvent(const std::array<bool, GLFW_KEY_LAST+1>& keys, const float deltaTime);
        void CursorPosCallback(double xpos, double ypos);
};