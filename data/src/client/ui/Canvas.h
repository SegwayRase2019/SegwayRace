#pragma once

#include "../../common/math/Math.h"
#include "./Button.h"
#include <SDL2/SDL.h>
#include <cstdint>
#include <string>
#include <functional>
#include <vector>

class Canvas
{
public:
  Canvas(class Game *game);
  virtual ~Canvas();

  virtual void Update(float deltaTima);
  virtual void Draw(SDL_Renderer *renderer);
  virtual void ProcessInput(const uint8_t *keys);
  virtual void HandleKeyPress(int key);

  enum UIState
  {
    EActive,
    EClosing
  };

  void Close();

  UIState GetState() const { return mState; }

  void SetTitle(const std::string &title, const Vector3 &Color = Color::White, int pointSize = 40);

  void AddButton(const std::string &name, std::function<void()> onClick);

protected:
  void DrawTexture(SDL_Renderer *renderer, SDL_Texture *texture, const Vector2 &offset = Vector2::Zero, float scale = 1.0f);

  class Game *mGame;

  class Font *mFont;
  SDL_Texture *mTitle;
  SDL_Texture *mBackGround;
  SDL_Texture *mButtonOn;
  SDL_Texture *mButtonOff;

  Vector2 mTitlePos;
  Vector2 mNextButtonPos;
  Vector2 mBGPos;

  UIState mState;

  int mWindowWidth;
  int mWindowHeight;

  std::vector<Button *> mButtons;
};