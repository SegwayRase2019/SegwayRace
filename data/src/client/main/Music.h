#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>
#include <SDL2/SDL.h>
#include "Game.h"

class Sound
{
private:
public:
  Sound(class Game *game);
  //Musicが関数名にMusicがつき、Chunkは関数名にSoundがついている
  static int Music_Countdown(int i);
  void Sound_Initialize();
  void BackGroundMusic();
  static void Goal_Music();
  static int Engine_Sound();
  static void Curve_Sound();
  static void Collision_Sound();
  static void UpdateMusicVolume();
  static void ResultMusic();

  static bool engine_load;
  static bool curve_load;
  static int ChangeVolume;

  static int channel;

  static Mix_Music *bgm;
  static Mix_Chunk *engine_bgm;
  static Mix_Chunk *curve_bgm;
  static Mix_Chunk *collision_bgm;
};