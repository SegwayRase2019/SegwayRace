#include "./Music.h"
#include <algorithm>
#include <SDL2/SDL_mixer.h>
#include <string>

Sound::Sound(class Game *game)

{
}
bool Sound::engine_load = false;
bool Sound::curve_load = false;

Mix_Music *Sound::bgm;
Mix_Chunk *Sound::engine_bgm;
Mix_Chunk *Sound::curve_bgm;
Mix_Chunk *Sound::collision_bgm;
int Sound::ChangeVolume = 50;
int Sound::channel;

void Sound::Sound_Initialize()
{
    // SDL_mixerの初期化（MP3ファイルを使用）
    SDL_Init(SDL_INIT_AUDIO);
    SDL_Init(MIX_INIT_MP3);

    Mix_AllocateChannels(16);

    // オーディオデバイスの初期化
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
    {
        SDL_Log("failed to initialize SDL_mixer.:%s", SDL_GetError());
    }
}

int Sound::Music_Countdown(int i)
{
    Mix_Chunk *chunk;

    switch (i)
    {
    case 0:
    case 1:
    case 2:
    {
        chunk = Mix_LoadWAV("assets/musics/Countdown01.wav");
        break;
    }
    case 3:
    {
        chunk = Mix_LoadWAV("assets/musics/Countdown02.wav");
        break;
    }
    }

    if (chunk == NULL)
    {
        SDL_Log("Failed to load chunk file %s", SDL_GetError());
    }

    Mix_PlayChannel(1, chunk, 0);
}

void Sound::BackGroundMusic()
{

    bgm = Mix_LoadMUS("assets/musics/sample.mp3");
    if (bgm == NULL)
    {
        SDL_Log("Failed to load music file %s", SDL_GetError());
    }
    Mix_PlayMusic(bgm, -1);
}

void Sound::UpdateMusicVolume()
{

    if (ChangeVolume != Game::volume)
    {
        Mix_VolumeMusic(Game::volume);
        printf("now volume is %d\n", Mix_VolumeMusic(-1));
    }
    ChangeVolume = Game::volume;
}

int Sound::Engine_Sound()
{

    if (engine_load == false)
    {

        engine_bgm = Mix_LoadWAV("assets/musics/Engine_Loop01.wav");
        if (engine_bgm == NULL)
        {
            SDL_Log("Failed to load music file %s", SDL_GetError());
        }
        engine_load = true;
    }
    Mix_PlayChannel(-1, engine_bgm, 0);
}

void Sound::Curve_Sound()
{

    if (curve_load == false)
    {

        curve_bgm = Mix_LoadWAV("assets/musics/Racing01.wav");
        if (curve_bgm == NULL)
        {
            SDL_Log("Failed to load music file %s", SDL_GetError());
        }
        curve_load = true;
    }
    Mix_PlayChannel(-1, curve_bgm, 0);
}

void Sound::Collision_Sound()
{
    collision_bgm = Mix_LoadWAV("assets/musics/Racing01.wav");
    if (collision_bgm == NULL)
    {
        SDL_Log("Failed to load music file %s", SDL_GetError());
    }
    Mix_PlayChannel(1, collision_bgm, 0);
}

void Sound::Goal_Music()
{
}