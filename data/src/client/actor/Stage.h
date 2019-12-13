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
  float SetLocationX[MAX_CLIENTS] = {4600, 4800, 5000, 5200};
  float SetLocationY[MAX_CLIENTS] = {5350, 5350, 5350, 5350};
  Vector2 Start;
  CONTAINER StartPosition[MAX_CLIENTS];

private:
#define MAP_SCALE 10
#define BASIC_POSITION_X 1100
#define POSITON_GAP 200
#define BASIC_POSITION_Y 0
};