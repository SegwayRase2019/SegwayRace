#pragma once
#include "./Canvas.h"

class Startwindow : public Canvas
{
public:
  Startwindow(class Game *game);
  ~Startwindow();

  void Draw(SDL_Renderer *renderer) override;
  void Update(float deltaTime) override;

  void HandleKeyPress(int key) override;

private:
  SDL_Texture *mPlTex;
  SDL_Texture *mText;
  SDL_Texture *mPushText;
};