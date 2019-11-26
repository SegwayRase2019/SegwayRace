#pragma once

#include "./Actor.h"

class Racer : public Actor
{
  public:
    Racer(class Game *game);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t *keyState) override;

  private:
};
