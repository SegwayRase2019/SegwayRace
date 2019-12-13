#pragma once

#include <vector>
#include "../../common/math/Math.h"
#include <cstdint>

class Actor
{
public:
  enum State
  {
    EActive,
    EInactive,
    EDead
  };

  Actor(class Game *game);
  virtual ~Actor();

  void Update(float deltaTime);
  void UpdateComponents(float deltaTime);
  virtual void UpdateActor(float deltaTime);

  void ProcessInput(const uint8_t *command);
  virtual void ActorInput(const uint8_t *command);

  // Getter/Setter
  const Vector2 &GetPosition() const { return mPosition; }
  void SetPosition(const Vector2 &pos) { mPosition = pos; }
  float GetScale() const { return mScale; }
  void SetScale(float scale) { mScale = scale; }
  float GetRotation() const { return mRotation; }
  void SetRotation(float rotation) { mRotation = rotation; }

  Vector2 GetForward() const { return Vector2(-Math::Sin(mRotation), -Math::Cos(mRotation)); }

  State GetState() const { return mState; }
  void SetState(State state) { mState = state; }

  class Game *GetGame() { return mGame; }

  // コンポーネントの追加と削除
  void AddComponent(class Component *component);
  void RemoveComponent(class Component *component);
    class Game *mGame;

private:
  State mState;

  Vector2 mPosition;
  float mScale;
  float mRotation;

  std::vector<class Component *> mComponents;

};