#define MINIAUDIO_IMPLEMENTATION
#include "sound/sound.hpp"

#include "debug/logger.hpp"

SoundController::SoundController():
    m_audioInitialized(false)
{
    const ma_result result = ma_engine_init(NULL, &m_engine);

    if (result != MA_SUCCESS) {
        Logger::GetInstance().AddLog(LogLevel::Level2, "Failed to initialize miniaudio : " + std::string(ma_result_description(result)));
        return;
    }

    m_audioInitialized = true;
    
    LoadMusic("../asset/sound/music/black1.mp3");
    LoadMusic("../asset/sound/music/black2.mp3");
    LoadMusic("../asset/sound/music/black3.mp3");
    LoadMusic("../asset/sound/music/black4.mp3");
    LoadMusic("../asset/sound/music/black5.mp3");
}

SoundController::~SoundController()
{
    if (m_audioInitialized) {
        for (ma_sound* s : m_musics)
            ma_sound_uninit(s);
        
        ma_engine_uninit(&m_engine);
    }
}

void SoundController::LoadMusic(const std::string& musicFilepath)
{
    if (!m_audioInitialized) {
        Logger::GetInstance().AddLog(LogLevel::Level2, "miniaudio has not been initialized. Can't load the audio : " + musicFilepath);
        return;
    }
    
    ma_sound* music = new ma_sound;
    const ma_result result = ma_sound_init_from_file(&m_engine, musicFilepath.c_str(), 0, NULL, NULL, music);

    if (result != MA_SUCCESS) {
        Logger::GetInstance().AddLog(LogLevel::Level2, "Failed to load the audio " + musicFilepath + " : " + std::string(ma_result_description(result)));
        delete music;
        return;
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
    if (!m_audioInitialized) {
        Logger::GetInstance().AddLog(LogLevel::Level2, "miniaudio has not been initialized. Can't play random audio");
        return;
    }
    
    if (m_musics.empty()){
        Logger::GetInstance().AddLog(LogLevel::Level2, "No music has been loaded. Can't play random audio");
        return;
    }

    ma_sound* randomMusic = m_musics[std::rand()%m_musics.size()];
    const ma_result result = ma_sound_start(randomMusic);
    
    if (result != MA_SUCCESS)
        Logger::GetInstance().AddLog(LogLevel::Level2, "Failed to start playing random audio : " + std::string(ma_result_description(result)));
}