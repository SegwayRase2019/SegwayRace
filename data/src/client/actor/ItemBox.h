#pragma once

#include "./Actor.h"

class ItemBox : public Actor
{
public:
  ItemBox(class Game* game);
  void UpdateActor(float deltaTime) override;

private:
  float mIntervalTime;
  float mCountTime;
};