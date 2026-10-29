#pragma once

class EventCursorPosCallback
{
    public:
        EventCursorPosCallback() = default;

        virtual void CursorPosCallback(double xoffset, double yoffset) = 0;
};