#pragma once

#include <GLFW/glfw3.h>

struct Time
{
    double m_lastTime;
    double m_currentTime;
    double m_deltaTime;

    Time():
        m_lastTime(glfwGetTime()) 
    {}

    void Update()
    {
        m_currentTime = glfwGetTime();
        m_deltaTime = m_currentTime-m_lastTime;
        m_lastTime = m_currentTime;
    }

    double GetDeltaTime() const
    {
        return m_deltaTime;
    }
};