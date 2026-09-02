#pragma once

#include <string>
#include <vector>

#include "sound/miniaudio.h"

class SoundController
{
    private:
        ma_engine m_engine;
        std::vector<ma_sound*> m_musics;

        static void SoundEndCallback(void* pUserData, ma_sound* pSound);

    public:
        SoundController();
        ~SoundController();
        
        void LoadMusic(const std::string& musicFilepath);
        void PlayRandonMusic();
};