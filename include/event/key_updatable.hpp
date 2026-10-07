#pragma once

#include <array>

#include <GLFW/glfw3.h>

class EventKeyUpdatable
{
    public:
        EventKeyUpdatable() = default;

        virtual void EventKeyUpdate(const std::array<bool, GLFW_KEY_LAST+1>& keys, const float deltaTime) = 0;
};