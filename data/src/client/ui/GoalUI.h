#pragma once

#include "./Canvas.h"
#include "./../main/Music.h"

class GoalUI : public Canvas
{
public:
  GoalUI(class Game *game);
  ~GoalUI();

  void Update(float deltaTime) override;
  void Draw(SDL_Renderer *renderer) override;

private:
  SDL_Texture *mGoalUIa;
  SDL_Texture *mGoalUIb;
  float mCountTime, mMaxCount;
  bool isBigger;
};