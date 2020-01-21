#pragma once
#include "./Canvas.h"

class Waitwindow : public Canvas
{
public:
  Waitwindow(class Game *game);
  ~Waitwindow();

  void Draw(SDL_Renderer *renderer) override;
  void Update(float deltaTime) override;

private:
  SDL_Texture *mWaitMsg;
};