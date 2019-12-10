#pragma once

#include "../main/Game.h"
#include "../../common/math/Math.h"
#include <string>
#include <unordered_map>
#include <SDL2/SDL_ttf.h>

class Font
{
public:
  Font(class Game *game);
  ~Font();

  bool Load(const std::string &fileName);
  void Unload();

  SDL_Texture *RenderText(const std::string &textKey, const Vector3 &color = Color::White, int pointSize = 30);
  SDL_Texture *RenderText(const std::string &textKey);

private:
  std::unordered_map<int, TTF_Font *> mFontData;
  class Game *mGame;
};