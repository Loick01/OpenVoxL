#pragma once

#include <glad/glad.h>

#include "core/camera.hpp"
#include "core/time.hpp"
#include "core/window.hpp"
#include "event/event.hpp"
#include "graphic/skybox.hpp"

class Application
{
    private:
        Window m_window; // Window must be initialized before EventController
        EventController m_eventController;
        
        Camera m_camera;
        Skybox m_skybox;
        Time m_time;
        
    public:
        Application();

        bool Run();
};