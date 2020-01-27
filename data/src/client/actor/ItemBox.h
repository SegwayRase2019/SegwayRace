#pragma once

#include "./Actor.h"

class ItemBox : public Actor
{
public:
  ItemBox(class Game *game);
  void UpdateActor(float deltaTime) override;
  const Vector2 &GetPosition() const { return Actor::GetPosition(); }

private:
  float mIntervalTime;
  float mCountTime;
  float mCountTime1;
  float mEffectTime;
};