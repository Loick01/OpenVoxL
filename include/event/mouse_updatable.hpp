#pragma once

#include <array>

#include <GLFW/glfw3.h>

class EventMouseUpdatable
{
    public:
        EventMouseUpdatable() = default;

        virtual void EventMouseButtonUpdate(const std::array<bool, GLFW_MOUSE_BUTTON_LAST+1>& mouseButtons, const float deltaTime) = 0;
};