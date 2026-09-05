#pragma once

#include <glad/glad.h>

#include "map/terrain.hpp"
#include "core/camera.hpp"
#include "core/time.hpp"
#include "core/window.hpp"
#include "event/event.hpp"
#include "graphic/skybox.hpp"
#include "sound/sound.hpp"
#include "ui/debug_window.hpp"
#include "ui/hud.hpp"

class Application
{
    private:
        Window m_window; // Window must be initialized before EventController
        EventController m_eventController;
        SoundController m_soundController;
        
        Camera m_camera;
        DebugWindow m_debug;
        Hud m_hud;
        Skybox m_skybox;
        Terrain m_terrain;
        Time m_time;
        
    public:
        Application();

        bool Run();
};