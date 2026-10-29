#pragma once

#include <array>

#include <GLFW/glfw3.h>

class EventKeyCallback
{
    public:
        EventKeyCallback() = default;

        virtual void KeyCallback(const std::array<bool, GLFW_KEY_LAST+1>& keys) = 0;
};