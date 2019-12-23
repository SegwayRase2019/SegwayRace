#pragma once

#include "./Actor.h"
#include "../main/Game.h"
#include "../../common/math/Math.h"

class Player : public Actor
{
public:
  Player(class Game *game, int clientID);

  void Update(float deltaTime);
  void UpdateActor(float deltaTime) override;
  void ActorInput(const uint8_t *keyState) override;

  // セッター・ゲッター
  const Vector2 &GetPosition() const { return Actor::GetPosition(); }
  void SetPosition(Vector2 pos) { Actor::SetPosition(pos); }
  float GetRotation() const { return Actor::GetRotation(); }
  void SetRotation(float rot) { Actor::SetRotation(rot); }

  void ComputeRocalTransform();
  const Matrix3 &GetRocalTransform() const { return mRocalTransform; }

private:
  Matrix3 mRocalTransform;
  bool mRecomputeTransform;
  class Game *mGame;
  int mClientID;
};