#pragma once

//#include <SDL2/ttf.h>
#include <vector>
using namespace std;

class Actor
{
public:
  //
  enum State
  {
    EActive,
    EPaused,
    EDead
  };
  //
  Actor(class Game *game);
  virtual ~Actor();

  //
  void Update(float deltaTime);
  //
  void UpdateComponents(float deltaTime);
  //
  virtual void UpdateActor(float deltaTime);

  //
  void AddComponent(class Component *component);
  void RemoveComponent(class Component *component);

private:
  //
  State mState;
  //
  //Vector2 mPosition;
  float mScale;
  float mRotation;
  //
  vector<class Component *> mComponents;
  class Game *mGame;
};
