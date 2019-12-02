#pragma once

#include "./Actor.h"

class Racer : public Actor
{
  public:
    Racer(class Game *game);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t *keyState) override;
    Vector2 GetCenterPosition() { return mPos; }

  private:
    Vector2 mPos;
};
