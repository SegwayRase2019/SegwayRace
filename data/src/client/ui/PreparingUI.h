#pragma once

#include "./Canvas.h"
#include "./../main/Music.h"

class PreparingUI : public Canvas
{
public:
  PreparingUI(class Game *game);
  ~PreparingUI();

  void Update(float deltaTime) override;
  void Draw(SDL_Renderer *renderer) override;

private:
  SDL_Texture *mCountdown[4];
  short int mCount;
  float mInterval;
  float mCountTime;
};