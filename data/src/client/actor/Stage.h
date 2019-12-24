#pragma once

#include "../component/SpriteComponent.h"
#include "../../common/math/Math.h"
#include "./Actor.h"
#include "../main/Game.h"
#include "../../common/Common.h"

class Stage : public Actor
{
public:
  Stage(class Game *game);
  void Shutdown();

  void SetStatrtPosition();
  float SetLocationX[MAX_CLIENTS] = {1000, 1200, 1400, 1600};
  float SetLocationY[MAX_CLIENTS] = {300, 100, -100, -300};
  Vector2 Start;
  CONTAINER StartPosition[MAX_CLIENTS];

private:
#define MAP_SCALE 10
#define BASIC_POSITION_X 1000
#define POSITON_GAP 200
#define BASIC_POSITION_Y 300
};