#pragma once

#include <glad/glad.h>

#include "entity/collision.hpp"
#include "entity/player.hpp"
#include "world/terrain/terrain.hpp"
#include "world/terrain/terrain_modifier.hpp"
#include "core/camera.hpp"
#include "core/file/file.hpp"
#include "core/time.hpp"
#include "core/window.hpp"
#include "event/event.hpp"
#include "graphic/skybox.hpp"
#include "setting/setting.hpp"
#include "sound/sound.hpp"
#include "ui/hotbar.hpp"
#include "ui/hud.hpp"

class Application
{
    private:
        Window m_window; // Window must be initialized before EventController
        EventController m_eventController;
        SoundController m_soundController;
        FileReader m_fileReader;

        Camera m_camera;
        Hud m_hud;
        Hotbar m_hotbar;
        Skybox m_skybox;
        Terrain m_terrain;
        TerrainModifier m_terrainModifier;
        Time m_time;

        Player m_player;
        CollisionResolver m_collisionResolver;
        
        SettingWindow m_debug;
        
    public:
        Application();

        bool Run();
};