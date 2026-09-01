#pragma once

#include <glm/glm.hpp>

enum class CameraState
{
    KeyFree, MouseFree, Orbital, Player
};

class Camera
{
    private:
        const glm::vec3 m_upVector;
        glm::vec3 m_position;
        glm::vec3 m_targetVector;
        const float m_nearPlane;
        const float m_farPlane;
        const float m_aspectRatio;
        const float m_sensitivity;
        float m_fov;
        unsigned int m_speed;

        double m_previousXPos;
        double m_previousYPos;
        float m_phi;
        float m_theta;
        
        CameraState m_state;

    public:
        Camera(const float aspectRatio);

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;

        void CursorPosCallback(double xpos, double ypos);
};