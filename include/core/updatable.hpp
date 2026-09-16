#pragma once

#include <array>

#include <GLFW/glfw3.h>

class EventUpdatable
{
    public:
        EventUpdatable() = default;

        virtual void EventUpdate(const std::array<bool, GLFW_KEY_LAST+1>& keys, const float deltaTime) = 0;
};