#pragma once

#include "../component/SpriteComponent.h"
#include "../../common/math/Math.h"
#include "./Actor.h"
#include "../main/Game.h"

class Stage : public Actor
{
public:
  Stage(class Game *game);
  void Shutdown();

private:
};