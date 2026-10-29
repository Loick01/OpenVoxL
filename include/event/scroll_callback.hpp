#pragma once

class EventScrollCallback
{
    public:
        EventScrollCallback() = default;

        virtual void ScrollCallback(double xpos, double ypos) = 0;
};