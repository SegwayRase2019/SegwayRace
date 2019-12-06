#pragma once

#include "../../common/math/Math.h"
#include <cstdint>
#include <string>
#include <functional>
#include <SDL2/SDL.h>

class Button
{
public:
  Button(const std::string &name, class Font *font,
         std::function<void()> onClick,
         const Vector2 &pos, const Vector2 &dims);
  ~Button();

  void SetName(const std::string &name);

  // ゲッター・セッター
  SDL_Texture *GetNameTex() { return mNameTex; }
  const Vector2 &GetPosition() const { return mPosition; }
  void SetHighlighted(bool sel) { mHighlighted = sel; }
  bool GetHighlighted() const { return mHighlighted; }

  bool ContainsPoint(const Vector2 &pt) const;
  void OnClick();

private:
  std::function<void()> mOnClick;
  std::string mName;
  SDL_Texture *mNameTex;
  class Font *mFont;
  Vector2 mPosition;
  Vector2 mDimensions;
  bool mHighlighted;
};