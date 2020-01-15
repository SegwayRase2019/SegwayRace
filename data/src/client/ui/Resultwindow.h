#pragma once
#include "./Canvas.h"
#include "../../common/Common.h"

class Resultwindow : public Canvas
{
public:
  Resultwindow(class Game *game);
  ~Resultwindow();

  void Draw(SDL_Renderer *renderer) override;

  void HandleKeyPress(int key) override;

private:
  SDL_Texture *mRankingUI[MAX_CLIENTS];
  SDL_Texture *mRacerUI[MAX_CLIENTS];

  int mRank[MAX_CLIENTS];
};