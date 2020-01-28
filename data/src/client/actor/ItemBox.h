#pragma once

#include "./Actor.h"

class ItemBox : public Actor
{
public:
  ItemBox(class Game *game, int id);
  void UpdateActor(float deltaTime) override;
  const Vector2 &GetPosition() const { return Actor::GetPosition(); }
  void SetState(State state) { mState = state; }

private:
  int mId;
  float mIntervalTime;
  float mCountTime;
  float mCountTime1;
  float mEffectTime;
};