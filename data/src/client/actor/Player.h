#pragma once

#include "./Actor.h"
#include "../main/Game.h"
#include "../../common/math/Math.h"

class Player : public Actor
{
public:
  enum PlayerState
  {
    EPreparing,
    ERunning,
    EGoal
  };
  Player(class Game *game, int clientID);

  void UpdateActor(float deltaTime) override;
  void ActorInput(const uint8_t *keyState) override;

  // セッター・ゲッター
  const Vector2 &GetPosition() const { return Actor::GetPosition(); }
  void SetPosition(Vector2 pos) { Actor::SetPosition(pos); }
  float GetRotation() const { return Actor::GetRotation(); }
  void SetRotation(float rot) { Actor::SetRotation(rot); }
  const PlayerState &GetPlayerState() const { return mPlState; }
  void SetPlayerState(PlayerState state) { mPlState = state; }

  void ComputeRocalTransform();

private:
  class Game *mGame;
  int mClientID;
  PlayerState mPlState;
};