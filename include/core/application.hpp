#pragma once

#include <glad/glad.h>

#include "core/window.hpp"
#include "event/event.hpp"

class Application
{
    private:
        Window m_window; // Window must be initialized before EventController
        EventController m_eventController;
        
    public:
        Application();

        bool Run();
};