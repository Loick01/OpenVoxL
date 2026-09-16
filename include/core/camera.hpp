#pragma once

#include <glm/glm.hpp>

#include "core/updatable.hpp"

class Hitbox;

enum class CameraState
{
    KeyFree, MouseFree, Orbital, Player
};

class Camera : public EventUpdatable
{
    private:
        const glm::vec3 m_upVector;
        glm::vec3 m_frontVector;
        glm::vec3 m_rightVector;
        glm::vec3 m_position;
        glm::vec3 m_targetTerrain;
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

        GLFWwindow* m_glfwWindow;
        Hitbox& m_playerHitbox;

        void UpdateVectors();
        void UpdateVectorsToTarget(const glm::vec3 target);
        void CheckInputMode();
        
    public:
        Camera(GLFWwindow* glfwWindow, Hitbox& hitbox, const float aspectRatio);

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;

        glm::vec3 GetPosition() const;
        float GetSpeed() const;
        CameraState GetState() const;

        void SetSpeed(const float speed);
        void ResetPosition();
        void SetTargetTerrain(const glm::vec3 target);

        void SetState(const CameraState state);
        void KeyCallback(const std::array<bool,GLFW_KEY_LAST+1>& keys);
        void CursorPosCallback(double xpos, double ypos);

        void EventUpdate(const std::array<bool, GLFW_KEY_LAST+1>& keys, const float deltaTime) override;
};