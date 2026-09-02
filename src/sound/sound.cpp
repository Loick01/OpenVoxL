#define MINIAUDIO_IMPLEMENTATION
#include "sound/sound.hpp"

#include <stdexcept>

SoundController::SoundController()
{
    if (ma_engine_init(NULL, &m_engine) != MA_SUCCESS)
        throw std::runtime_error("Error while initializing miniaudio");

    LoadMusic("../asset/sound/music/black1.mp3");
    LoadMusic("../asset/sound/music/black2.mp3");
    LoadMusic("../asset/sound/music/black3.mp3");
    LoadMusic("../asset/sound/music/black4.mp3");
    LoadMusic("../asset/sound/music/black5.mp3");
}

SoundController::~SoundController()
{
    for (ma_sound* s : m_musics)
        ma_sound_uninit(s);
    
    ma_engine_uninit(&m_engine);
}

void SoundController::LoadMusic(const std::string& musicFilepath)
{
    ma_sound* music = new ma_sound;

    if (ma_sound_init_from_file(&m_engine, musicFilepath.c_str(), 0, NULL, NULL, music) != MA_SUCCESS) {
        delete music;
        throw std::runtime_error("Error while initializing this audio file : " + musicFilepath);
    }

    ma_sound_set_end_callback(music, SoundEndCallback, this);
    m_musics.push_back(music);
}

void SoundController::SoundEndCallback(void* pUserData, ma_sound* pSound) 
{
    SoundController* instance = static_cast<SoundController*>(pUserData);
    instance->PlayRandonMusic();
}

void SoundController::PlayRandonMusic()
{
    ma_sound* randomMusic = m_musics[std::rand()%m_musics.size()];

    ma_result result = ma_sound_start(randomMusic);
    if (result != MA_SUCCESS)
        throw std::runtime_error("Error while playing : " + result);
}