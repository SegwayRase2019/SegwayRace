#pragma once
#include "Server_func.h"
#include <cmath>
#include <stdlib.h>

class Collision
{
public:
  Collision();

  static int Player_Collision(CONTAINER Posdata);
  static int Stage_Collision(CONTAINER Posdata);

  static int Collision_Judgement(CONTAINER Posdata);

  static bool collision;
  static float distance;

  static int collision_oppnent;

  static CONTAINER PlayerPos[MAX_CLIENTS];

private:
};