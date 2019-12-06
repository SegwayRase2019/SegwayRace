#pragma once

#include "./Actor.h"
#include "../main/Game.h"
#include "../../common/math/Math.h"

class Player : public Actor
{
  public:
    Player(class Game *game, int clientID);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t *keyState) override;

    // セッター・ゲッター
    const Vector2 &GetPosition() const { return Actor::GetPosition(); }
    void SetPosition(Vector2 pos) { Actor::SetPosition(pos); }
    float GetRotation() const { return Actor::GetRotation(); }
    void SetRotation(float rot) { Actor::SetRotation(rot); }

  private:
    class Game *mGame;
    int mClientID;
};